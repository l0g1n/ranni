#include "utils.h"

BOOL UtilCreateService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName, _In_ LPTSTR szDriverPath)
{
    BOOL ReturnValue = FALSE;

    SC_HANDLE hService = CreateService(
        hSCM,
        szDriverName,
        szDriverName,
        SERVICE_ALL_ACCESS,
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        szDriverPath,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (hService == NULL && GetLastError() != ERROR_SERVICE_EXISTS) {
        ErrorPrint("CreateService failed, error code: 0x%x", GetLastError());
        goto EXIT;
    }

    ReturnValue = TRUE;

EXIT:
    if (hService) {
        CloseServiceHandle(hService);
    }

    return ReturnValue;
}

BOOL UtilStartService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName)
{
    BOOL ReturnValue = FALSE;

    SC_HANDLE hService = OpenService(hSCM, szDriverName, SERVICE_ALL_ACCESS);

    if (hService == NULL) {
        ErrorPrint("OpenService failed, error code: 0x%x", GetLastError());
        goto EXIT;
    }

    if (!StartService(hService, 0, NULL)) {
        if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING) {
            ErrorPrint("StartService failed, error code: 0x%x", GetLastError());
            goto EXIT;
        }
    }

    if (FALSE == UtilWaitForServiceState(hService, SERVICE_RUNNING)) {
        goto EXIT;
    }

    ReturnValue = TRUE;

EXIT:
    if (hService) {
        CloseServiceHandle(hService);
    }

    return ReturnValue;
}

BOOL UtilStopService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName)
{
    BOOL ReturnValue = FALSE;

    SC_HANDLE      hService = OpenService(hSCM, szDriverName, SERVICE_ALL_ACCESS);
    SERVICE_STATUS ServiceStatus;

    if (hService == NULL) {
        if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) {
            ReturnValue = TRUE;
        } else {
            ErrorPrint("OpenService failed, error code: 0x%x", GetLastError());
        }
        goto EXIT;
    }

    if (FALSE == ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus)) {
        if (GetLastError() != ERROR_SERVICE_NOT_ACTIVE) {
            ErrorPrint("ControlService failed, error code: 0x%x", GetLastError());
            goto EXIT;
        }
    }

    ReturnValue = TRUE;

EXIT:
    if (hService) {
        CloseServiceHandle(hService);
    }

    return ReturnValue;
}

BOOL UtilDeleteService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName)
{
    BOOL ReturnValue = FALSE;

    SC_HANDLE hService = OpenService(hSCM, szDriverName, SERVICE_ALL_ACCESS);

    if (hService == NULL) {
        if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) {
            ReturnValue = TRUE;
        } else {
            ErrorPrint("OpenService failed, error code: 0x%x", GetLastError());
        }
        goto EXIT;
    }

    if (!DeleteService(hService)) {
        if (GetLastError() != ERROR_SERVICE_MARKED_FOR_DELETE) {
            ErrorPrint("DeleteService failed, error code: 0x%x", GetLastError());
            goto EXIT;
        }
    }

    ReturnValue = TRUE;

EXIT:
    if (hService) {
        CloseServiceHandle(hService);
    }

    return ReturnValue;
}

BOOL UtilGetServiceState(_In_ SC_HANDLE hService, _Out_ DWORD *State)
{
    *State = 0;

    SERVICE_STATUS_PROCESS ServiceStatus;
    DWORD                  BytesNeeded;

    if (FALSE == QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE) &ServiceStatus, sizeof(ServiceStatus), &BytesNeeded)) {
        ErrorPrint("QueryServiceStatusEx failed, error code: 0x%x", GetLastError());
        return FALSE;
    }

    *State = ServiceStatus.dwCurrentState;

    return TRUE;
}

BOOL UtilWaitForServiceState(_In_ SC_HANDLE hService, _In_ DWORD State)
{
    DWORD ServiceState;

    while (true) {
        if (FALSE == UtilGetServiceState(hService, &ServiceState)) {
            return FALSE;
        }

        if (ServiceState == State) {
            break;
        }

        Sleep(1000);
    }

    return TRUE;
}

BOOL UtilOpenDevice(_In_ LPTSTR szWin32DeviceName, _Out_ HANDLE *phDevice)
{
    BOOL ReturnValue = FALSE;

    HANDLE hDevice = CreateFile(
        szWin32DeviceName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hDevice == INVALID_HANDLE_VALUE) {
        ErrorPrint("CreateFile(%ls) failed, error code: 0x%x", szWin32DeviceName, GetLastError());
        goto EXIT;
    }

    ReturnValue = TRUE;

EXIT:
    *phDevice = hDevice;

    return ReturnValue;
}

BOOL UtilLoadDriver(_In_ LPTSTR szDriverName, _In_ LPTSTR szDriverFileName, _In_ LPTSTR szWin32DeviceName, _Out_ HANDLE *pDevice)
{
    BOOL ReturnValue = FALSE;

    DWORD     dwSize;
    TCHAR    *pPathSeparator;
    TCHAR     szDriverPath[MAX_PATH] = TEXT("");
    SC_HANDLE hSCM                   = NULL;
    HANDLE    hDevice                = NULL;

    *pDevice = NULL;

    dwSize = GetModuleFileName(NULL, szDriverPath, (DWORD) std::size(szDriverPath));

    if (dwSize == 0) {
        ErrorPrint("GetModuleFileName failed, error code: 0x%x", GetLastError());
        goto EXIT;
    }

    pPathSeparator = _tcsrchr(szDriverPath, TEXT('\\'));

    if (pPathSeparator != NULL) {
        pPathSeparator[1] = TEXT('\0');
        _tcscat_s(szDriverPath, MAX_PATH, szDriverFileName);
    } else {
        ErrorPrint("_tcsrchr failed to find \\ in driver path");
        goto EXIT;
    }

    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL) {
        ErrorPrint("OpenSCManager failed, error code: 0x%x", GetLastError());
        goto EXIT;
    }

    ReturnValue = UtilUnloadDriver(INVALID_HANDLE_VALUE, hSCM, szDriverName);

    if (ReturnValue == FALSE) {
        ErrorPrint("UtilUnloadDriver failed");
        goto EXIT;
    }

    ReturnValue = UtilCreateService(hSCM, szDriverName, szDriverPath);

    if (ReturnValue == FALSE) {
        ErrorPrint("UtilCreateService failed");
        goto EXIT;
    }

    ReturnValue = UtilStartService(hSCM, szDriverName);

    if (ReturnValue == FALSE) {
        ErrorPrint("UtilStartService failed");
        goto EXIT;
    }

    ReturnValue = UtilOpenDevice(szWin32DeviceName, &hDevice);

    if (ReturnValue == FALSE) {
        ErrorPrint("UtilOpenDevice failed");
        goto EXIT;
    }

    *pDevice = hDevice;

    ReturnValue = TRUE;

EXIT:
    if (hSCM != NULL) {
        CloseServiceHandle(hSCM);
    }

    return ReturnValue;
}

BOOL UtilUnloadDriver(_In_ HANDLE hDevice, _In_opt_ SC_HANDLE hPassedSCM, _In_ LPTSTR szDriverName)
{
    BOOL      ReturnValue = FALSE;
    SC_HANDLE hSCM        = hPassedSCM;

    if (hSCM == NULL) {
        hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (hSCM == NULL) {
            ErrorPrint("OpenSCManager failed, error code: 0x%x", GetLastError());
            goto EXIT;
        }
    }

    if (hDevice != NULL && hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(hDevice);
        hDevice = INVALID_HANDLE_VALUE;
    }

    ReturnValue = UtilStopService(hSCM, szDriverName);

    if (ReturnValue == FALSE) {
        ErrorPrint("UtilStopService failed");
        goto EXIT;
    }

    ReturnValue = UtilDeleteService(hSCM, szDriverName);

    if (ReturnValue == FALSE) {
        ErrorPrint("UtilDeleteService failed");
        goto EXIT;
    }

    ReturnValue = TRUE;

EXIT:
    if (hPassedSCM == NULL && hSCM != NULL) {
        CloseServiceHandle(hSCM);
    }

    return ReturnValue;
}

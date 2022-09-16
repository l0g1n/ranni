#pragma once

//
// logging macros
//

#define InfoPrint(str, ...)  printf(##str "\n", __VA_ARGS__)
#define ErrorPrint(str, ...) printf("ERROR: %u: "##str "\n", __LINE__, __VA_ARGS__)

//
// utility routines
//

BOOL UtilCreateService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName, _In_ LPTSTR szDriverPath);
BOOL UtilStartService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName);
BOOL UtilStopService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName);
BOOL UtilDeleteService(_In_ SC_HANDLE hSCM, _In_ LPTSTR szDriverName);
BOOL UtilGetServiceState(_In_ SC_HANDLE hService, _Out_ DWORD *State);
BOOL UtilWaitForServiceState(_In_ SC_HANDLE hService, _In_ DWORD State);

BOOL UtilOpenDevice(_In_ LPTSTR szWin32DeviceName, _Out_ HANDLE *phDevice);

BOOL UtilLoadDriver(_In_ LPTSTR szDriverName, _In_ LPTSTR szDriverFileName, _In_ LPTSTR szWin32DeviceName, _Out_ HANDLE *pDevice);
BOOL UtilUnloadDriver(_In_ HANDLE hDevice, _In_opt_ SC_HANDLE hPassedSCM, _In_ LPTSTR szDriverName);

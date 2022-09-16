#include "utils.h"

#include "../ranni-k/common.h"

int main()
{
    TCHAR szDriverName[]      = TEXT("ranni-k");
    TCHAR szDriverFileName[]  = TEXT("ranni-k.sys");
    TCHAR szWin32DeviceName[] = TEXT("\\\\.\\ranni");

    HANDLE hDevice;
    BOOL   Result;

    Result = UtilLoadDriver(szDriverName, szDriverFileName, szWin32DeviceName, &hDevice);

    if (Result == FALSE) {
        ErrorPrint("UtilLoadDriver failed");
        goto EXIT;
    }

    Result = DeviceIoControl(
        hDevice,
        IOCTL_RANNI_PING,
        NULL,
        0,
        NULL,
        0,
        NULL,
        NULL
    );

    if (Result == FALSE) {
        ErrorPrint("DeviceIoControl failed");
        goto EXIT;
    }

EXIT:
    Result = UtilUnloadDriver(hDevice, NULL, szDriverName);

    return 0;
}

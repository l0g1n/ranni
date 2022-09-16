#include "utils.h"

int main()
{
    TCHAR szDriverName[]      = TEXT("ranni-k");
    TCHAR szDriverFileName[]  = TEXT("ranni-k.sys");
    TCHAR szWin32DeviceName[] = TEXT("\\\\.\\ranni");

    HANDLE hDriver;

    BOOL Result = UtilLoadDriver(szDriverName, szDriverFileName, szWin32DeviceName, &hDriver);
    if (Result == FALSE) {
        ErrorPrint("UtilLoadDriver failed");
    }

    UtilUnloadDriver(hDriver, NULL, szDriverName);

    return 0;
}

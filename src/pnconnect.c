#include "WinInternal.h"
#include "pnconnect.h"
#include "Helpers.h"
#include "Log.h"

int
WINAPI
main(void)
{
    NTSTATUS Status;
    HANDLE hpnDevice = NULL;

    Status = OpenDevice(&hpnDevice);
    LogStatus(Status, L"OpenDevice");

    Status = DeviceIoControlNoThrow(hpnDevice, IOCTL_PNRDR_ENSURE_REDIRECTOR_STARTED);
    LogStatus(Status, L"EnsureRedirectorStarted");
    if (Status)
    {
        Status = DeviceIoControlNoThrow(hpnDevice, IOCTL_PNRDR_START_MINI_REDIRECTOR);
        LogStatus(Status, L"StartRedirector");
    }

    // Cleanup
    if(hpnDevice)
        NtClose(hpnDevice);
}

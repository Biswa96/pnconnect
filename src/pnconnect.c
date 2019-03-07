#include "WinInternal.h"
#include "pnconnect.h"
#include "Helpers.h"
#include "Log.h"
#include <Npapi.h>
#include <stdio.h>

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

    ULONG res = 0;
    HANDLE hEnum = NULL;
    NETRESOURCEW NetResource;

    RtlZeroMemory(&NetResource, sizeof NetResource);
    NetResource.lpRemoteName = L"\\\\wsl$";

    res = NPOpenEnum(RESOURCE_GLOBALNET,
                     RESOURCETYPE_DISK,
                     RESOURCEUSAGE_CONNECTABLE,
                     &NetResource,
                     &hEnum);

    res = CoInitializeEx(0, COINIT_MULTITHREADED);
    res = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                               SecurityDelegation, NULL, EOAC_STATIC_CLOAKING, NULL);

    ULONG Count = INFINITE, BufferSize = 0;
    LPNETRESOURCEW Buffer = NULL;
    HANDLE HeapHandle = GetProcessHeap();
    Buffer = RtlAllocateHeap(HeapHandle, HEAP_ZERO_MEMORY, sizeof (char));

    res = NPEnumResource(hEnum, &Count, Buffer, &BufferSize);
    if (res == ERROR_MORE_DATA)
    {
        PVOID Temp = NULL;
        Temp = RtlReAllocateHeap(HeapHandle, HEAP_ZERO_MEMORY, Buffer, BufferSize);
        Buffer = Temp;
        Temp = NULL;

        res = NPEnumResource(hEnum, &Count, Buffer, &BufferSize);
        if (res == ERROR_SUCCESS)
        {
            wprintf(L"\n Running Distributions: %lu\n", Count);

            for (ULONG i = 0; i < Count; i++)
            {
                wprintf(L" Scope: %lu\n Type: %lu\n DisplayType: %lu\n"
                        L" Usage: %lu\n Remote Name: %ls\n Provider: %ls\n",
                        Buffer->dwScope, Buffer->dwType, Buffer->dwDisplayType,
                        Buffer->dwUsage, Buffer->lpRemoteName, Buffer->lpProvider);
            }
        }
    }
    else
        LogResult(res, L"NPEnumResource");

    // Cleanup
    if (Buffer)
        RtlFreeHeap(HeapHandle, 0, Buffer);
    if (hEnum)
        NPCloseEnum(hEnum);
    if(hpnDevice)
        NtClose(hpnDevice);
}

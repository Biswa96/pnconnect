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
    LogResult(res, L"NPOpenEnum");

    res = CoInitializeEx(0, COINIT_MULTITHREADED);
    res = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                               SecurityDelegation, NULL, EOAC_STATIC_CLOAKING, NULL);

    ULONG Count = INFINITE, BufferSize = 0;
    LPNETRESOURCEW Buffer = NULL;
    HANDLE HeapHandle = GetProcessHeap();
    Buffer = RtlAllocateHeap(HeapHandle, HEAP_ZERO_MEMORY, sizeof (char));

    res = NPEnumResource(hEnum, &Count, Buffer, &BufferSize);
    if (res == WN_MORE_DATA)
    {
        PVOID Temp = NULL;
        Temp = RtlReAllocateHeap(HeapHandle, HEAP_ZERO_MEMORY, Buffer, BufferSize);
        Buffer = Temp;
        Temp = NULL;

        wprintf(L"\n");
        while (TRUE)
        {
            res = NPEnumResource(hEnum, &Count, Buffer, &BufferSize);
            if (res == WN_SUCCESS)
            {
                PWSTR Output = NULL;

                for (ULONG i = 0; i < Count; i++)
                {
                    // info from https://docs.microsoft.com/en-us/windows/desktop/api/winnetwk/ns-winnetwk-_netresourcew

                    switch (Buffer->dwScope)
                    {
                        case RESOURCE_CONNECTED:
                            Output = L"RESOURCE_CONNECTED";
                            break;
                        case RESOURCE_GLOBALNET:
                            Output = L"RESOURCE_GLOBALNET";
                            break;
                        case RESOURCE_CONTEXT:
                            Output = L"RESOURCE_CONTEXT";
                            break;
                        default:
                            Output = L"RESOURCE_UNKNOWN";
                            break;
                    }
                    wprintf(L"Scope       : %ls (%lu) \n", Output, Buffer->dwScope);

                    switch (Buffer->dwType)
                    {
                        case RESOURCETYPE_DISK:
                            Output = L"RESOURCETYPE_DISK";
                            break;
                        case RESOURCETYPE_PRINT:
                            Output = L"RESOURCETYPE_PRINT";
                            break;
                        case RESOURCETYPE_ANY:
                            Output = L"RESOURCETYPE_ANY";
                            break;
                        default:
                            Output = L"RESOURCETYPE_UNKNOWN";
                            break;
                    }
                    wprintf(L"Type        : %ls (%lu) \n", Output, Buffer->dwType);

                    switch (Buffer->dwDisplayType)
                    {
                        case RESOURCEDISPLAYTYPE_NETWORK:
                            Output = L"RESOURCEDISPLAYTYPE_NETWORK";
                            break;
                        case RESOURCEDISPLAYTYPE_DOMAIN:
                            Output = L"RESOURCEDISPLAYTYPE_DOMAIN";
                            break;
                        case RESOURCEDISPLAYTYPE_SERVER:
                            Output = L"RESOURCEDISPLAYTYPE_SERVER";
                            break;
                        case RESOURCEDISPLAYTYPE_SHARE:
                            Output = L"RESOURCEDISPLAYTYPE_SHARE";
                            break;
                        case RESOURCEDISPLAYTYPE_DIRECTORY:
                            Output = L"RESOURCEDISPLAYTYPE_DIRECTORY";
                            break;
                        case RESOURCEDISPLAYTYPE_GENERIC:
                            Output = L"RESOURCEDISPLAYTYPE_GENERIC";
                            break;
                        default:
                            Output = L"RESOURCEDISPLAYTYPE_UNKNOWN";
                            break;
                    }
                    wprintf(L"DisplayType : %ls (%lu) \n", Output, Buffer->dwDisplayType);

                    if (Buffer->dwScope == RESOURCE_GLOBALNET)
                    {
                        if (Buffer->dwUsage & RESOURCEUSAGE_CONNECTABLE)
                            Output = L"RESOURCEUSAGE_CONNECTABLE";
                        if (Buffer->dwUsage & RESOURCEUSAGE_CONTAINER)
                            Output = L"RESOURCEUSAGE_CONTAINER";
                        wprintf(L"Usage       : %ls (%lu) \n", Output, Buffer->dwUsage);
                    }

                    wprintf(L"Local Name  : %ls \n", Buffer->lpLocalName);
                    wprintf(L"Remote Name : %ls \n", Buffer->lpRemoteName);
                    wprintf(L"Comment     : %ls \n", Buffer->lpComment);
                    wprintf(L"Provider    : %ls \n", Buffer->lpProvider);

                    wprintf(L"\n");
                    Buffer += sizeof(*Buffer);
                }
            }
            else
                break;
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
    CoUninitialize();
}

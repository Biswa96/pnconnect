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

        wprintf(L"\n");
        while(TRUE){
            res = NPEnumResource(hEnum, &Count, Buffer, &BufferSize);
            if (res == ERROR_SUCCESS)
            {
                for (ULONG i = 0; i < Count; i++)
                {
                    // info from https://docs.microsoft.com/en-us/windows/desktop/api/winnetwk/ns-winnetwk-_netresourcew
                    wprintf(L" Scope: %lu ", Buffer->dwScope);
                    switch(Buffer->dwScope){
                        case RESOURCE_CONNECTED:
                            wprintf(L"RESOURCE_CONNECTED ");
                            break;
                        case RESOURCE_GLOBALNET:
                            wprintf(L"RESOURCE_GLOBALNET ");
                            break;
                        case RESOURCE_CONTEXT:
                            wprintf(L"RESOURCE_CONTEXT ");
                            break;
                        default:
                            wprintf(L"UNNOWN VALUE ");
                            break;
                    }

                    wprintf(L"\n Type:  %lu ", Buffer->dwType);
                    switch(Buffer->dwType){
                        case RESOURCETYPE_DISK:
                            wprintf(L"RESOURCETYPE_DISK ");
                            break;
                        case RESOURCETYPE_PRINT:
                            wprintf(L"RESOURCETYPE_PRINT ");
                            break;
                        case RESOURCETYPE_ANY:
                            wprintf(L"RESOURCETYPE_ANY ");
                            break;
                        default:
                            wprintf(L"UNNOWN VALUE ");
                            break;
                    }

                    wprintf(L"\n DisplayType: %lu ", Buffer->dwDisplayType);
                    switch(Buffer->dwType){
                        case RESOURCEDISPLAYTYPE_NETWORK:
                            wprintf(L"RESOURCEDISPLAYTYPE_NETWORK ");
                            break;
                        case RESOURCEDISPLAYTYPE_DOMAIN:
                            wprintf(L"RESOURCEDISPLAYTYPE_DOMAIN ");
                            break;
                        case RESOURCEDISPLAYTYPE_SERVER:
                            wprintf(L"RESOURCEDISPLAYTYPE_SERVER ");
                            break;
                        case RESOURCEDISPLAYTYPE_SHARE:
                            wprintf(L"RESOURCEDISPLAYTYPE_SHARE ");
                            break;
                        case RESOURCEDISPLAYTYPE_DIRECTORY:
                            wprintf(L"RESOURCEDISPLAYTYPE_DIRECTORY ");
                            break;
                        case RESOURCEDISPLAYTYPE_GENERIC:
                            wprintf(L"RESOURCEDISPLAYTYPE_GENERIC ");
                            break;
                        default:
                            wprintf(L"UNNOWN VALUE ");
                            break;
                    }

                    wprintf(L"\n Usage: %lu ", Buffer->dwUsage);
                    if(Buffer->dwScope == RESOURCE_GLOBALNET) {
                        if(Buffer->dwUsage & RESOURCEUSAGE_CONNECTABLE ){
                            wprintf(L"RESOURCEUSAGE_CONNECTABLE ");
                        }
                        if(Buffer->dwUsage & RESOURCEUSAGE_CONTAINER ){
                            wprintf(L"RESOURCEUSAGE_CONTAINER ");
                        }
                    }

                    wprintf(L"\n Local Name: %ls\n", Buffer->lpLocalName);
                    wprintf(L" Remote Name: %ls\n", Buffer->lpRemoteName);
                    wprintf(L" Comment: %ls\n", Buffer->lpComment);
                    wprintf(L" Provider: %ls\n", Buffer->lpProvider);

                    wprintf(L"\n");
                    Buffer += (sizeof(struct _NETRESOURCEW));
                }
            }else{
                break;
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

#include <Windows.h>
#include <winternl.h>
#include "ntapi.h"
#include "Helpers.h"
#include "Log.h"
#include "pnioctls.h"

void
WINAPI
RemoveNetworkDrive(PWSTR DriveLetter, BOOLEAN fForce)
{
    ULONG res;
    wchar_t TargetPath[MAX_PATH];

    RtlZeroMemory(TargetPath, MAX_PATH);
    res = QueryDosDeviceW(DriveLetter, TargetPath, MAX_PATH);
    if (!res)
        LogResult(GetLastError(), L"QueryDosDeviceW");

    NTSTATUS Status;
    HANDLE FileHandle = NULL;
    IO_STATUS_BLOCK IoStatusBlock;
    ULONG FsControlCode;

    if (fForce)
        FsControlCode = FSCTL_P9RX_CONNECTION_FORCE_CLOSE;
    else
        FsControlCode = FSCTL_P9RX_CONNECTION_REQUEST_CLOSE;

    Status = OpenConnection(&FileHandle, TargetPath);
    LogStatus(Status, L"OpenConnection");

    Status = NtFsControlFile(FileHandle,
                             NULL,
                             NULL,
                             NULL,
                             &IoStatusBlock,
                             FsControlCode,
                             NULL, 0,
                             NULL, 0);
    LogStatus(Status, L"NtFsControlFile");

    res = DefineDosDeviceW(DDD_RAW_TARGET_PATH | DDD_REMOVE_DEFINITION | DDD_EXACT_MATCH_ON_REMOVE,
                           DriveLetter,
                           TargetPath);
    if (!res)
        LogResult(GetLastError(), L"DefineDosDeviceW");

    if (FileHandle)
        NtClose(FileHandle);
    return;
}

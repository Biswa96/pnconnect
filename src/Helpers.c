#include "WinInternal.h"

NTSTATUS
NTAPI
OpenDevice(PHANDLE DeviceHandle)
{
    NTSTATUS Status;
    UNICODE_STRING ObjectName = { 0 };
    OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
    IO_STATUS_BLOCK IoStatusBlock;

    Status = RtlInitUnicodeStringEx(&ObjectName, L"\\Device\\P9Rdr");
    ObjectAttributes.Length = sizeof ObjectAttributes;
    ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
    ObjectAttributes.ObjectName = &ObjectName;

    Status = NtCreateFile(DeviceHandle,
                          GENERIC_READ,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL,
                          0,
                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                          CREATE_NEW,
                          0,
                          NULL,
                          0);

    return Status;
}

NTSTATUS
NTAPI
DeviceIoControlNoThrow(HANDLE DeviceHandle,
                       ULONG IoControlCode)
{
    NTSTATUS Status;
    HANDLE hEvent = NULL;
    IO_STATUS_BLOCK IoStatusBlock;

    hEvent = CreateEventExW(NULL, NULL, 0, EVENT_ALL_ACCESS);
    Status = NtDeviceIoControlFile(DeviceHandle,
                                   hEvent,
                                   NULL,
                                   NULL,
                                   &IoStatusBlock,
                                   IoControlCode,
                                   NULL,
                                   0,
                                   NULL,
                                   0);
    if (Status == STATUS_PENDING)
        WaitForSingleObjectEx(hEvent, INFINITE, FALSE);

    if(hEvent)
        NtClose(hEvent);

    return Status;
}

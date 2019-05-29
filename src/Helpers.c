#include <Windows.h>
#include <winternl.h>
#include "ntapi.h"

NTSTATUS
NTAPI
OpenConnection(PHANDLE FileHandle, PWSTR TargetPath)
{
    NTSTATUS Status;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING ObjectName;
    OBJECT_ATTRIBUTES ObjectAttributes;

    RtlZeroMemory(&ObjectName, sizeof ObjectName);
    RtlInitUnicodeString(&ObjectName, TargetPath);

    RtlZeroMemory(&ObjectAttributes, sizeof ObjectAttributes);
    ObjectAttributes.Length = sizeof ObjectAttributes;
    ObjectAttributes.ObjectName = &ObjectName;
    ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;

    Status = NtCreateFile(FileHandle,
                          SYNCHRONIZE,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL,
                          FILE_ATTRIBUTE_NORMAL,
                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                          FILE_OPEN,
                          FILE_SYNCHRONOUS_IO_NONALERT | FILE_CREATE_TREE_CONNECTION,
                          NULL,
                          0);
    return Status;
}

NTSTATUS
NTAPI
OpenDevice(PHANDLE DeviceHandle)
{
    NTSTATUS Status;
    UNICODE_STRING ObjectName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;

    RtlZeroMemory(&ObjectName, sizeof ObjectName);
    RtlInitUnicodeString(&ObjectName, L"\\Device\\P9Rdr");

    RtlZeroMemory(&ObjectAttributes, sizeof ObjectAttributes);
    ObjectAttributes.Length = sizeof ObjectAttributes;
    ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
    ObjectAttributes.ObjectName = &ObjectName;

    Status = NtCreateFile(DeviceHandle,
                          SYNCHRONIZE,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL,
                          FILE_ATTRIBUTE_NORMAL,
                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                          FILE_OPEN,
                          FILE_SYNCHRONOUS_IO_NONALERT,
                          NULL,
                          0);
    return Status;
}

NTSTATUS
NTAPI
DeviceIoControlNoThrow(HANDLE DeviceHandle, ULONG IoControlCode)
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

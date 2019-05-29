#ifndef HELPERS_H
#define HELPERS_H

NTSTATUS
NTAPI
OpenConnection(PHANDLE FileHandle, PWSTR TargetPath);

NTSTATUS
NTAPI
OpenDevice(PHANDLE DeviceHandle);

NTSTATUS
NTAPI
DeviceIoControlNoThrow(HANDLE DeviceHandle, ULONG IoControlCode);

#endif // HELPERS_H

#ifndef WININTERNAL_H
#define WININTERNAL_H

#include <Windows.h>

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

// Flags from winternl.h
#ifndef __MINGW32__
#define FILE_SYNCHRONOUS_IO_NONALERT 32
#endif
#define OBJ_CASE_INSENSITIVE 64

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

NTSTATUS
NTAPI
NtCreateFile(PHANDLE FileHandle,
             ACCESS_MASK DesiredAccess,
             POBJECT_ATTRIBUTES ObjectAttributes,
             PIO_STATUS_BLOCK IoStatusBlock,
             PLARGE_INTEGER AllocationSize,
             ULONG FileAttributes,
             ULONG ShareAccess,
             ULONG CreateDisposition,
             ULONG CreateOptions,
             PVOID EaBuffer,
             ULONG EaLength);

NTSTATUS
NTAPI
NtClose(HANDLE Handle);

NTSTATUS
NTAPI
NtDeviceIoControlFile(HANDLE FileHandle,
                      HANDLE Event,
                      PVOID ApcRoutine,
                      PVOID ApcContext,
                      PIO_STATUS_BLOCK IoStatusBlock,
                      ULONG IoControlCode,
                      PVOID InputBuffer,
                      ULONG InputBufferLength,
                      PVOID OutputBuffer,
                      ULONG OutputBufferLength);

PVOID
NTAPI
RtlAllocateHeap(PVOID HeapHandle,
                ULONG Flags,
                SIZE_T Size);

BOOLEAN
NTAPI
RtlFreeHeap(HANDLE HeapHandle,
            ULONG Flags,
            PVOID P);

PVOID
NTAPI
RtlReAllocateHeap(HANDLE Heap,
                  ULONG Flags,
                  PVOID Ptr,
                  SIZE_T Size);

NTSTATUS
NTAPI
RtlInitUnicodeStringEx(PUNICODE_STRING DestinationString,
                       PWSTR SourceString);

#undef RtlZeroMemory
void
NTAPI
RtlZeroMemory(PVOID Destination,
              SIZE_T Length);

#endif // WININTERNAL_H

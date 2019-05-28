#ifndef NTAPI_H
#define NTAPI_H

NTSYSAPI
PVOID
NTAPI
RtlAllocateHeap(
    _In_ PVOID HeapHandle,
    _In_opt_ ULONG Flags,
    _In_ SIZE_T Size);

NTSYSAPI
BOOLEAN
NTAPI
RtlFreeHeap(
    _In_ PVOID HeapHandle,
    _In_opt_ ULONG Flags,
    _In_ PVOID BaseAddress);

NTSYSAPI
PVOID
NTAPI
RtlReAllocateHeap(
    _In_ PVOID HeapHandle,
    _In_ ULONG Flags,
    _In_ PVOID BaseAddress,
    _In_ SIZE_T Size);

#undef RtlZeroMemory
VOID
NTAPI
RtlZeroMemory(
    _In_ PVOID Destination,
    _In_ SIZE_T Length);

#endif // NTAPI_H

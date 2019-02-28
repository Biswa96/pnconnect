#include <Windows.h>
#include <stdio.h>

void
WINAPI
LogResult(HRESULT hResult, PWSTR Function)
{
    if (hResult != 0)
        wprintf(L"%ls Error: %lu\n", Function, (hResult & 0xFFFF));
}

void
WINAPI
LogStatus(NTSTATUS Status, PWSTR Function)
{
    if (Status != 0)
        wprintf(L"%ls Status: 0x%08lX\n", Function, Status);
}

#include <Windows.h>
#include <stdio.h>

void
WINAPI
LogStatus(NTSTATUS Status, PWSTR Function)
{
    if (!Status)
        return;

    wprintf(L"%ls Status: 0x%08lX\n", Function, Status);
}

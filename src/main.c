#include <Windows.h>
#include "pnconnect.h"
#include "pndisconnect.h"

int WINAPI main(void)
{
    // Not so important, need admin privileges
    EnsureRedirectorStarted();

    ListConnections();

    int wargc;
    PWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);
    if (wargc < 2) return 0;

    if(!_wcsicmp(wargv[1], L"-d"))
        RemoveNetworkDrive(wargv[2], TRUE);

    return 0;
}

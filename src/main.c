#include <Windows.h>
#include "pnconnect.h"

int WINAPI main(void)
{
    // Not so important, need admin privileges
    // EnsureRedirectorStarted();

    ListConnections();
}

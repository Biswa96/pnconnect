#ifndef LOG_H
#define LOG_H

void
WINAPI
LogResult(HRESULT hResult, PWSTR Function);

void
WINAPI
LogStatus(NTSTATUS Status, PWSTR Function);

#endif // LOG_H

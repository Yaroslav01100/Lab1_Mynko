#include <stdio.h>
#include <windows.h>

#define PROCESS_TIMEOUT 10000

int main(){
    STARTUPINFOW su;
    PROCESS_INFORMATION pi;

    memset(&su, 0x00, sizeof(su));
    memset(&pi, 0x00, sizeof(pi));
    su.cb = sizeof(su);

    int r = CreateProcessW(L"c:\\windows\\system32\\mspaint.exe", NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &su, &pi);
    if (r) {
        printf("CreateProcess() worked\n");

        DWORD waitResult = WaitForSingleObject(pi.hProcess, PROCESS_TIMEOUT);

        if (waitResult == WAIT_OBJECT_0) {

            DWORD exitCode;
            if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
                printf("Process exited with code: %d\n", exitCode);
                if (exitCode == 0) {
                    printf("Process completed successfully.\n");
                } else {
                    printf("Process encountered an error during execution.\n");
                }
            } else {
                printf("Failed to get exit code.\n");
            }
        } else if (waitResult == WAIT_TIMEOUT) {

            printf("Process timed out. Terminating...\n");
            if (TerminateProcess(pi.hProcess, 1)) {
                printf("Process terminated successfully.\n");
            } else {
                printf("Failed to terminate process.\n");
            }
        } else {

            printf("WaitForSingleObject() failed.\n");
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

    } else {
        DWORD e = GetLastError();
        printf("CreateProcess() failed :(, error: 0x%x \n", e);
    }
}

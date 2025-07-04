#include <windows.h>
#include <stdio.h>
/*
Exploiting Exported Functions in a DLL

This program thinks it's just loading the 'HelloWorld' function
from the DLL path, but in reality, the DLL is additionally running arbitrary code
as the DLL is loaded and unloaded from the process.

Steps: 
1. Load DLL into current process (LoadLibraryA: 1 argument--DLL FULL path*)
2. Get the address of the exported function (GetProcAddress)
3. Call the function (function pointer cast to the correct type)

*Full path is required. Path for Process A will not neccessarily point to the same location 
in Process B.
*/

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <DLL Path>\n", argv[0]);
        return 1;
    }

    HMODULE hModule = LoadLibraryA(argv[1]);
    if (hModule == NULL) {
        printf("Failed to load DLL: %d\n", GetLastError());
        return 1;
    }

    int (*func)() = (int (*)())GetProcAddress(hModule, "HelloWorld");
    if (func) {
        func();
    }

    FreeLibrary(hModule);
    return 0;
}


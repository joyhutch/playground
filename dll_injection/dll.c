
/*
MinGW C NOT visual C 
*/


#include <windows.h>

// exported function 
__declspec(dllexport) void HelloWorld() {
    MessageBoxA(NULL, "Hello, World!", "My DLL", MB_OK);
}

void shhhh() {
    MessageBoxA(NULL, "they'll never know", "My DLL", MB_OK);
}

// Window's style entry point--called when the DLL is loaded/attached to thread or detached from process
BOOL APIENTRY DllMain(HMODULE hModule, // DLL module handle
    DWORD ul_reason_for_call, // reason for calling 
    LPVOID lpReserved // reserved
) {
    // anything run in DllMain will block the calling thread
    // so be careful with what is added here; but otherwise, 
    // can more or less do anything you want here
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            shhhh();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            MessageBoxA(NULL, "Goodbye, World!", "My DLL", MB_OK);
            break;
    }
    return TRUE;
}
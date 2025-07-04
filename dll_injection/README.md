# DLL & Injection
Lab code for different explorations of DLLs and forcing remote processes to load a DLL.

## A Basic DLL 
`gcc dll.c -shared -o bin\simple.dll`

Example of a simple DLL generated from C. To be specific, "TLS in a DLL" according to Microsoft (sure). 

The `-shared` flag enables dynamic linking and loading of the DLL by other processes (assuming they can access the DLL; e.g. the DLL is located in sys directory, PATH, or same dir)

Can be loaded via `LoadLibraryA()`


## Exported Function Exploitation
`gcc exported_func.c -o runme.exe`

Example of exploiting exported functions from `DllMain` and running 'hidden' functions (in the local process)

## rDLL injection
`gcc 
Example of forcing a (given) remote process to load a DLL.

- Pro:

- Cons:
    - our DLL is on disk
    

## Shellcode Remote DLL Injection (sRDI)

Example of injecting a DLL into a remote process operating *entirely in memory __without__ letting the file touch the disk* 

Reuses remote injection example from `simple_process_injection/inject-remote.cpp`. POC demonstrates executing a DLL like shellcode without modifying our remote injector.

Uses donut to generate PID x64 shellcode having the DLL embedded inside, stabled with a stub to load the DLL upon execution.

Donut: https://thewover.github.io/Introducing-Donut/
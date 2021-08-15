#include <cstring>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using XInputGetStateProc = DWORD (WINAPI *)(_In_ DWORD dwUserIndex, _Out_ void *pState);
XInputGetStateProc getStateProc = nullptr;

DWORD WINAPI entry(LPVOID lpParameter) {
    (void) lpParameter;
    FILE* con;

    AllocConsole();
    freopen_s(&con, "CONOUT$", "w", stdout);
    std::printf("Incredible\n");

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    (void) hinstDLL;
    (void) lpReserved;
    static HMODULE xinput910 = nullptr;

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {
            char dllPath[MAX_PATH];
            GetSystemDirectoryA(dllPath, MAX_PATH);
            if(strcat_s(dllPath, MAX_PATH, "\\XINPUT9_1_0.dll") != 0) {
                return FALSE;
            }

            xinput910 = LoadLibraryA(dllPath);
            getStateProc = (XInputGetStateProc)GetProcAddress(xinput910, "XInputGetState");

            if (CreateThread(nullptr, 0, entry, nullptr, 0, nullptr) == nullptr) {
                return FALSE;
            }

            break;
        }
        case DLL_PROCESS_DETACH: {
            FreeLibrary(xinput910);
            break;
        }
        default:
            break;
    }

    return TRUE;
}

extern "C" __declspec(dllexport) DWORD WINAPI XInputGetState(_In_ DWORD dwUserIndex, _Out_ void *pState) {
    return getStateProc(dwUserIndex, pState);
}

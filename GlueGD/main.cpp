#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllexport) void run(DWORD geometryDashVersion) {
    FILE* con;

    AllocConsole();
    freopen_s(&con, "CONOUT$", "w", stdout);

    printf("Geometry Dash Version: %d", geometryDashVersion);
}

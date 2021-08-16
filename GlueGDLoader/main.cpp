/* <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2020 Aly Cerruti
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <array>
#include <cstring>
#include <cstdio>
#include <locale>
#include <codecvt>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using XInputGetStateProc = DWORD (WINAPI *)(_In_ DWORD dwUserIndex, _Out_ void *pState);
XInputGetStateProc getStateProc = nullptr;

#include <libloaderapi.h>
#include <ImageHlp.h>
#include <PathCch.h>

using GlueRunProc = void (*)(DWORD geometryDashVersion);

DWORD WINAPI entry(LPVOID lpParameter) {
    (void) lpParameter;
    // FILE* con;

    // AllocConsole();
    // freopen_s(&con, "CONOUT$", "w", stdout);

    char gdExePath[MAX_PATH];
    GetModuleFileNameA(NULL, gdExePath, MAX_PATH);
    
    DWORD gdCompileStamp;

    {
        LOADED_IMAGE gdImage;
        MapAndLoad(gdExePath, NULL, &gdImage, FALSE, TRUE);

        gdCompileStamp = gdImage.FileHeader->FileHeader.TimeDateStamp;

        UnMapAndLoad(&gdImage);
    }

    // printf("Geometry Dash Version: %d", gdCompileStamp);

    wchar_t glueDllPath[MAX_PATH];
    GetModuleFileNameW(NULL, glueDllPath, MAX_PATH);
    PathCchRemoveFileSpec(glueDllPath, MAX_PATH);
    PathCchCombine(glueDllPath, MAX_PATH, glueDllPath, L"GlueGD.dll");

    HMODULE glueDll = LoadLibraryW(glueDllPath);
    GlueRunProc glueRun = (GlueRunProc)GetProcAddress(glueDll, "run");

    glueRun(gdCompileStamp);

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

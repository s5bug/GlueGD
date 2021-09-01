/* GlueGDLoader - A fake XInput DLL that loads GlueGD
 * Copyright (C) 2021 Aly Cerruti
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
#include <cstdlib>
#include <filesystem>
#include <locale>
#include <string>
#include <ztd/text.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wil/stl.h>
#include <wil/win32_helpers.h>

using XInputGetStateProc = DWORD (WINAPI *)(_In_ DWORD dwUserIndex, _Out_ void *pState);
XInputGetStateProc getStateProc = nullptr;

#include <libloaderapi.h>
#include <ImageHlp.h>
#include <PathCch.h>

#include <sol/sol.hpp>

using GlueRunProc = void (*)(DWORD geometryDashVersion, HMODULE glueHmod, std::vector<std::filesystem::path> moduleSearchPaths);

DWORD WINAPI entry(LPVOID lpParameter) {
    (void) lpParameter;

    std::wstring gdExePath;
    wil::GetModuleFileNameW(NULL, gdExePath);

    std::filesystem::path gdExeDir = std::filesystem::path(gdExePath).parent_path();
    
    DWORD gdCompileStamp;

    {
        // TODO: reimplement ImageHlp to use wstring
        std::string ansiGdExePath = ztd::text::transcode(gdExePath, ztd::text::wide_utf16, ztd::text::ascii, ztd::text::replacement_handler);

        LOADED_IMAGE gdImage;
        MapAndLoad(ansiGdExePath.c_str(), NULL, &gdImage, FALSE, TRUE);

        gdCompileStamp = gdImage.FileHeader->FileHeader.TimeDateStamp;

        UnMapAndLoad(&gdImage);
    }

    std::filesystem::path configPath;
    std::wstring env_cfg;
    if(wil::TryGetEnvironmentVariableW(L"GLUEGD_CONFIG", env_cfg) != 0) {
        configPath = env_cfg;
    } else {
        configPath = std::filesystem::current_path() / "gluegd_config.lua";
    }

    sol::state lua;
    lua.open_libraries(sol::lib::base);
    sol::protected_function_result loadConfig = lua.do_file(configPath.string());
    if(!loadConfig.valid()) {
        std::stringstream message;
        message << "Lua config at " << configPath.string() << " invalid! Error from Lua:" << std::endl << std::endl;

        sol::error err = loadConfig;
        std::string what = err.what();
        message << what << std::endl << std::endl;

        message << "GlueGD will not load." << std::endl;

        std::string uMessage = message.str();
        std::wstring winMessage = ztd::text::transcode(uMessage, ztd::text::compat_utf8, ztd::text::wide_utf16, ztd::text::replacement_handler);

        MessageBoxW(NULL, winMessage.c_str(), L"GlueGDLoader Error", MB_OK);

        return 1;
    }

    sol::protected_function configFunc = loadConfig;

    sol::protected_function_result configResult = configFunc(gdCompileStamp, gdExeDir.string(), configPath.parent_path().string());

    if(!configResult.valid()) {
        std::stringstream message;
        message << "Lua config at " << configPath.string() << " errored! Error from Lua:" << std::endl << std::endl;

        sol::error err = configResult;
        std::string what = err.what();
        message << what << std::endl << std::endl;

        message << "GlueGD will not load." << std::endl;

        std::string uMessage = message.str();
        std::wstring winMessage = ztd::text::transcode(uMessage, ztd::text::compat_utf8, ztd::text::wide_utf16, ztd::text::replacement_handler);

        MessageBoxW(NULL, winMessage.c_str(), L"GlueGDLoader Config Error", MB_OK);

        return 1;
    }

    sol::table config = configResult;

    std::string dllPath = config["dll_path"];
    std::wstring wDllPath = ztd::text::transcode(dllPath, ztd::text::compat_utf8, ztd::text::wide_utf16, ztd::text::replacement_handler);

    HMODULE glueDll = LoadLibraryW(wDllPath.c_str());
    GlueRunProc glueRun = (GlueRunProc)GetProcAddress(glueDll, "run");

    std::vector<std::string> moduleSearchPathStrings = config["module_search_path"].get<std::vector<std::string>>();
    std::vector<std::filesystem::path> moduleSearchPaths;

    std::transform(
        moduleSearchPathStrings.begin(), moduleSearchPathStrings.end(),
        std::back_inserter(moduleSearchPaths),
        [](std::string pathString) {
            return std::filesystem::path(pathString);
        }
    );

    glueRun(gdCompileStamp, glueDll, moduleSearchPaths);

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    (void) hinstDLL;
    (void) lpReserved;
    static HMODULE xinput910 = nullptr;

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {
            std::wstring systemDirectory;
            wil::GetSystemDirectoryW(systemDirectory);
            std::filesystem::path dllPath = std::filesystem::path(systemDirectory) / "XINPUT9_1_0.dll";

            xinput910 = LoadLibraryW(dllPath.wstring().c_str());
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

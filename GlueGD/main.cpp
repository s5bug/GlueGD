/* GlueGD - A library that manages Geometry Dash mods
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

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <span>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Psapi.h>

#include <cocos2dx/cocos2d.h>

using CocosDirectorRunWithSceneProc = void (cocos2d::CCDirector::*)(cocos2d::CCScene* pScene);

void __fastcall cocosRunWithSceneHook(cocos2d::CCDirector* thisx, void* edx, cocos2d::CCScene* pScene) {
    (void) edx;

    thisx->pushScene(pScene);
    thisx->startAnimation();

    printf("Hello from Geometry Dash's main thread\n");
    printf("The scene is %s\n", pScene->description());

    cocos2d::CCSize winSz = thisx->getWinSizeInPixels();
    printf("The window size is %f %f", winSz.width, winSz.height);

    return;
}

extern "C" __declspec(dllexport) void run(HMODULE hmod, DWORD geometryDashVersion) {
    (void) hmod;

    FILE* con;

    AllocConsole();
    freopen_s(&con, "CONOUT$", "w", stdout);

    printf("Geometry Dash Version: %d\n", geometryDashVersion);

    HMODULE libcocos2d = NULL;

    HMODULE hMods[1024];
    DWORD cbNeeded;
    if(EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded)) {
        for(size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            HMODULE theModule = hMods[i];

            wchar_t modulePath[MAX_PATH];
            GetModuleFileNameW(theModule, modulePath, MAX_PATH);

            wchar_t* moduleFileName = 1 + wcsrchr(modulePath, L'\\');
            if(_wcsicmp(L"libcocos2d.dll", moduleFileName) == 0) {
                libcocos2d = theModule;
                break;
            }
        }
    }

    if(libcocos2d == NULL) {
        printf("Failed to acquire libcocos2d module. Exiting GlueGD...\n");
        return;
    }

    FARPROC runWithSceneProc = GetProcAddress(libcocos2d, "?runWithScene@CCDirector@cocos2d@@QAEXPAVCCScene@2@@Z");
    uintptr_t runWithSceneAddr = reinterpret_cast<uintptr_t>(runWithSceneProc);

    printf("Address of runWithScene: %" PRIxPTR "\n", runWithSceneAddr);
    
    uintptr_t cocosRunWithSceneHookAddr = reinterpret_cast<uintptr_t>(&cocosRunWithSceneHook);
    std::vector<uint8_t> newCode = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    uintptr_t offset = cocosRunWithSceneHookAddr - (runWithSceneAddr + newCode.size());
    std::memcpy(newCode.data() + 1, &offset, sizeof(offset));

    DWORD oldProtections;
    VirtualProtect(runWithSceneProc, 0x5, PAGE_EXECUTE_READWRITE, &oldProtections);
    std::memcpy(runWithSceneProc, newCode.data(), newCode.size());
    VirtualProtect(runWithSceneProc, 0x5, oldProtections, &oldProtections);
}

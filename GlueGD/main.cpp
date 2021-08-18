/* GlueGD - A library that manages Geometry Dash mods
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

#include <algorithm>
#include <cstdio>
#include <cstdint>
#include <cinttypes>
#include <span>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Psapi.h>

#include <cocos2dx/cocos2d.h>

#ifndef IMGUI_IMPL_OPENGL_ES2
#error "GlueGD only works with IMGUI_IMPL_OPENGL_ES2 defined"
#endif

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_opengl3.h"

using CocosDirectorRunWithSceneProc = void (cocos2d::CCDirector::*)(cocos2d::CCScene* pScene);
using CocosDirectorDrawSceneProc = void (cocos2d::CCDirector::*)();
HMODULE libcocos2d = NULL;

using CocosSwapBuffersWrapperProc = void (*)(GLFWwindow*);
CocosSwapBuffersWrapperProc cocosSwapBuffersWrapper = NULL;

HDC adaptGlfwWindowHdc(GLFWwindow* window) {
    uintptr_t windowAddr = reinterpret_cast<uintptr_t>(window);
    // TODO why does RobTop have an HDC instead of an HWND in the Window struct??
    uintptr_t hdcAddr = windowAddr + 0x244;
    HDC* hdcPtr = reinterpret_cast<HDC*>(hdcAddr);

    return *hdcPtr;
}

void __fastcall cocosSwapBuffersHook(cocos2d::CCEGLView* thisx, void* edx) {
    (void) edx;

    GLFWwindow* window = thisx->getWindow();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glFlush();

    if(window != NULL) {
        cocosSwapBuffersWrapper(window);
    }

    return;
}

void __fastcall cocosRunWithSceneHook(cocos2d::CCDirector* thisx, void* edx, cocos2d::CCScene* pScene) {
    (void) edx;

    // We know for certain that we're not rendering a scene right now, so we can patch the method
    // We also know that libcocos2d is non-null because it had to be non-null for us to get here
    FARPROC swapBuffersProc = GetProcAddress(libcocos2d, "?swapBuffers@CCEGLView@cocos2d@@UAEXXZ");
    uintptr_t swapBuffersAddr = reinterpret_cast<uintptr_t>(swapBuffersProc);
    uint8_t* swapBuffersData = reinterpret_cast<uint8_t*>(swapBuffersAddr);

    std::span<uint8_t> swapBuffersCode = std::span(swapBuffersData, 0x20);
    std::vector<uint8_t> needle = { 0x50 /* PUSH EAX */, 0xE8 /* CALL */ };
    auto ccallLoc = std::search(swapBuffersCode.begin(), swapBuffersCode.end(), needle.begin(), needle.end());

    if(ccallLoc != swapBuffersCode.end()) {
        // Point to the CALL instruction
        uint8_t* ccallPtr = 1 + &(*ccallLoc);
        uintptr_t ccallAddr = reinterpret_cast<uintptr_t>(ccallPtr);

        uintptr_t cocosSwapBuffersWrapperOffset;
        std::memcpy(&cocosSwapBuffersWrapperOffset, ccallPtr + 1, sizeof(uintptr_t));
        // We take (callInstruction + callOperand + callLength)
        cocosSwapBuffersWrapper = reinterpret_cast<CocosSwapBuffersWrapperProc>(ccallAddr + cocosSwapBuffersWrapperOffset + 0x05);

        uintptr_t cocosSwapBuffersHookAddr = reinterpret_cast<uintptr_t>(&cocosSwapBuffersHook);
        std::vector<uint8_t> newCode = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        uintptr_t offset = cocosSwapBuffersHookAddr - (swapBuffersAddr + newCode.size());
        std::memcpy(newCode.data() + 1, &offset, sizeof(offset));

        DWORD oldProtections;
        VirtualProtect(swapBuffersProc, 0x5, PAGE_EXECUTE_READWRITE, &oldProtections);
        std::memcpy(swapBuffersProc, newCode.data(), newCode.size());
        VirtualProtect(swapBuffersProc, 0x5, oldProtections, &oldProtections);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void) io;

        HDC hDC = adaptGlfwWindowHdc(thisx->getOpenGLView()->getWindow());
        HWND hwnd = WindowFromDC(hDC);
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplOpenGL3_Init();
    } else {
        printf("Couldn't locate glfwSwapBuffers. Not hooking GUI...\n");
    }

    // Then let's run the original cocos2d code
    thisx->pushScene(pScene);
    thisx->startAnimation();

    return;
}

extern "C" __declspec(dllexport) void run(HMODULE hmod, DWORD geometryDashVersion) {
    (void) hmod;

    FILE* con;

    AllocConsole();
    freopen_s(&con, "CONOUT$", "w", stdout);

    printf("Geometry Dash Version: %d\n", geometryDashVersion);

    libcocos2d = GetModuleHandleW(L"libcocos2d.dll");

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

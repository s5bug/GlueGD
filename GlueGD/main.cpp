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
#include <queue>
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

using CocosPollEventsWrapperProc = void (*)();
CocosPollEventsWrapperProc cocosPollEventsWrapper = NULL;

// For updating to newer versions: check usages of GDI32.SwapBuffers in libcocos2d
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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void __fastcall cocosPollEventsHook(cocos2d::CCEGLView* thisx, void* edx) {
    (void) edx;
    (void) thisx; // GLFW context is global

    ImGuiIO& io = ImGui::GetIO();

    bool doReplace;
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);

        doReplace = true;

        if(io.WantCaptureMouse) {
            switch(msg.message) {
                case WM_CAPTURECHANGED:
                case WM_LBUTTONDBLCLK:
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_MBUTTONDBLCLK:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MOUSEACTIVATE:
                case WM_MOUSEHOVER:
                case WM_MOUSEHWHEEL:
                case WM_MOUSELEAVE:
                case WM_MOUSEMOVE:
                case WM_MOUSEWHEEL:
                case WM_NCHITTEST:
                case WM_NCLBUTTONDBLCLK:
                case WM_NCLBUTTONDOWN:
                case WM_NCLBUTTONUP:
                case WM_NCMBUTTONDBLCLK:
                case WM_NCMBUTTONDOWN:
                case WM_NCMBUTTONUP:
                case WM_NCMOUSEHOVER:
                case WM_NCMOUSELEAVE:
                case WM_NCMOUSEMOVE:
                case WM_NCRBUTTONDBLCLK:
                case WM_NCRBUTTONDOWN:
                case WM_NCRBUTTONUP:
                case WM_NCXBUTTONDBLCLK:
                case WM_NCXBUTTONDOWN:
                case WM_NCXBUTTONUP:
                case WM_RBUTTONDBLCLK:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_XBUTTONDBLCLK:
                case WM_XBUTTONDOWN:
                case WM_XBUTTONUP:
                    doReplace = false;
                    break;
                default:
                    break;
            }
        }

        if(io.WantCaptureKeyboard) {
            switch(msg.message) {
                case WM_ACTIVATE:
                case WM_APPCOMMAND:
                case WM_CHAR:
                case WM_DEADCHAR:
                case WM_HOTKEY:
                case WM_KEYDOWN:
                case WM_KEYUP:
                case WM_KILLFOCUS:
                case WM_SETFOCUS:
                case WM_SYSDEADCHAR:
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_UNICHAR:
                    doReplace = false;
                    break;
                default:
                    break;
            }
        }

        if(doReplace) {
            DispatchMessage(&msg);
        }

        ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
    }

    cocosPollEventsWrapper();
}

void patchEventPolling() {
    // cocos2d::CCEGLView::pollEvents is just a wrapper around glfwPollEvents
    FARPROC pollEventsProc = GetProcAddress(libcocos2d, "?pollEvents@CCEGLView@cocos2d@@QAEXXZ");
    uintptr_t pollEventsAddr = reinterpret_cast<uintptr_t>(pollEventsProc);
    uint8_t* pollEventsData = reinterpret_cast<uint8_t*>(pollEventsAddr);

    if(pollEventsData[0x00] == 0xE9) {
        uintptr_t cocosPollEventsWrapperOffset;
        std::memcpy(&cocosPollEventsWrapperOffset, pollEventsData + 1, sizeof(uintptr_t));
        cocosPollEventsWrapper = reinterpret_cast<CocosPollEventsWrapperProc>(pollEventsAddr + cocosPollEventsWrapperOffset + 0x05);

        uintptr_t cocosPollEventsHookAddr = reinterpret_cast<uintptr_t>(&cocosPollEventsHook);
        std::vector<uint8_t> newCode = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
        uintptr_t offset = cocosPollEventsHookAddr - (pollEventsAddr + newCode.size());
        std::memcpy(newCode.data() + 1, &offset, sizeof(offset));

        DWORD oldProtections;
        VirtualProtect(pollEventsProc, 0x5, PAGE_EXECUTE_READWRITE, &oldProtections);
        std::memcpy(pollEventsProc, newCode.data(), newCode.size());
        VirtualProtect(pollEventsProc, 0x5, oldProtections, &oldProtections);
    } else {
        printf("Something is wrong with CCEGLView::pollEvents! Is your GD installation modified?\n");
        printf("Not installing GUI event hook. GUI will be nonfunctional...\n");
    }
}

void patchSwapBuffers(cocos2d::CCDirector* thisx) {
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

        patchEventPolling();
    } else {
        printf("Couldn't locate glfwSwapBuffers. Not hooking GUI...\n");
    }
}

void __fastcall cocosRunWithSceneHook(cocos2d::CCDirector* thisx, void* edx, cocos2d::CCScene* pScene) {
    (void) edx;

    patchSwapBuffers(thisx);

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

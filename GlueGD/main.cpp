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

#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cocos2dx/cocos2d.h>

#define WM_GEOMETRYDASH_WINDOW_FOUND_MSG (WM_APP + 0)
#define WM_GEOMETRYDASH_WINDOW_SHOWN_MSG (WM_APP + 1)

DWORD messageLoopThreadId = 0;
HWND geometryDashWindow = NULL;

void CALLBACK handleOpenedWindow(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
) {
    (void) hWinEventHook;
    (void) event;
    (void) idChild;
    (void) idEventThread;
    (void) dwmsEventTime;

    if(idObject == OBJID_WINDOW) {
        int windowTextLength = 1 + GetWindowTextLengthW(hwnd);
        std::vector<wchar_t> windowText = std::vector<wchar_t>((size_t) windowTextLength);
        GetWindowTextW(hwnd, windowText.data(), windowTextLength);

        if(wcscmp(L"Geometry Dash", windowText.data()) == 0) {
            geometryDashWindow = hwnd;
            PostThreadMessageW(messageLoopThreadId, WM_GEOMETRYDASH_WINDOW_FOUND_MSG, NULL, NULL);
        }
    }
}

void CALLBACK handleShownWindow(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
) {
    (void) hWinEventHook;
    (void) event;
    (void) idChild;
    (void) idEventThread;
    (void) dwmsEventTime;

    if(idObject == OBJID_WINDOW && geometryDashWindow == hwnd) {
        PostThreadMessageW(messageLoopThreadId, WM_GEOMETRYDASH_WINDOW_SHOWN_MSG, NULL, NULL);
    }
}

extern "C" __declspec(dllexport) void run(HMODULE hmod, DWORD geometryDashVersion) {
    FILE* con;

    AllocConsole();
    freopen_s(&con, "CONOUT$", "w", stdout);

    printf("Geometry Dash Version: %d\n", geometryDashVersion);

    messageLoopThreadId = GetCurrentThreadId();

    HWINEVENTHOOK windowListener = SetWinEventHook(
        EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE,
        hmod, handleOpenedWindow,
        GetCurrentProcessId(), 0,
        WINEVENT_INCONTEXT | WINEVENT_SKIPOWNTHREAD
    );

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if(msg.message == WM_GEOMETRYDASH_WINDOW_FOUND_MSG) {
            break;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnhookWinEvent(windowListener);

    if(geometryDashWindow == NULL) {
        printf("Failed to acquire Geometry Dash window. Stopping GlueGD.\n");
        return;
    }

    printf("Found window: %p\n", geometryDashWindow);

    HWINEVENTHOOK windowShowListener = SetWinEventHook(
        EVENT_OBJECT_SHOW, EVENT_OBJECT_SHOW,
        hmod, handleShownWindow,
        GetCurrentProcessId(), 0,
        WINEVENT_INCONTEXT | WINEVENT_SKIPOWNTHREAD
    );

    while (GetMessage(&msg, NULL, 0, 0)) {
        if(msg.message == WM_GEOMETRYDASH_WINDOW_SHOWN_MSG) {
            break;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnhookWinEvent(windowShowListener);

    printf("Window shown\n");

    // TODO initialize ImGui and render _over_ the existing window: We don't
    // want to render as part of GD's UI kit, we want to be able to fully steal
    // events.
}

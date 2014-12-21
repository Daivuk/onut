#include "onut.h"
#include "StringUtils.h"
#include "Window.h"

namespace onut {
    LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
        if (msg == WM_DESTROY ||
            msg == WM_CLOSE) {
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(handle, msg, wparam, lparam);
    }

    Window::Window(const POINT& resolution) {
        // Define window style
        WNDCLASS wc = { 0 };
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = WinProc;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"OakNutWindow";
        RegisterClass(&wc);

        // Centered position
        auto screenW = GetSystemMetrics(SM_CXSCREEN);
        auto screenH = GetSystemMetrics(SM_CYSCREEN);
        auto posX = (screenW - resolution.x) / 2;
        auto posY = (screenH - resolution.y) / 2;

        // Create the window
        m_handle = CreateWindow(L"OakNutWindow", 
            utf8ToUtf16(OSettings->getGameName()).c_str(),
            WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
            posX, posY, resolution.x, resolution.y,
            nullptr, nullptr, nullptr, nullptr);
    }

    HWND Window::getHandle() {
        return m_handle;
    }
}

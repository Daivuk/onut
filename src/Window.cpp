#include "onut.h"
#include "StringUtils.h"
#include "Window.h"
#include <windowsx.h>

namespace onut
{
    Window* pWindow = nullptr;

    LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_DESTROY ||
            msg == WM_CLOSE)
        {
            PostQuitMessage(0);
            return 0;
        }
        else if (msg == WM_MOUSEMOVE)
        {
            auto xPos = GET_X_LPARAM(lparam);
            auto yPos = GET_Y_LPARAM(lparam);
            OInput->mousePos.x = static_cast<decltype(POINT().x)>(xPos);
            OInput->mousePos.y = static_cast<decltype(POINT().x)>(yPos);
            OInput->mousePosf.x = static_cast<float>(xPos);
            OInput->mousePosf.y = static_cast<float>(yPos);
            OMousePos = OInput->mousePosf;
            return 0;
        }
        else if (msg == WM_SIZE)
        {
            if (ORenderer)
            {
                ORenderer->onResize();
            }
            return 0;
        }
        else if (msg == WM_SETCURSOR)
        {
            if (pWindow->m_cursor)
            {
                SetCursor(pWindow->m_cursor);
                return 0;
            }
        }

        return DefWindowProc(handle, msg, wparam, lparam);
    }

    Window::Window(const POINT& resolution, bool isResizable) :
        m_cursor(0)
    {
        pWindow = this;

        // Define window style
        WNDCLASS wc = {0};
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
        if (isResizable)
        {
            m_handle = CreateWindow(L"OakNutWindow",
                                    utf8ToUtf16(OSettings->getGameName()).c_str(),
                                    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                    posX, posY, resolution.x, resolution.y,
                                    nullptr, nullptr, nullptr, nullptr);
        }
        else
        {
            m_handle = CreateWindow(L"OakNutWindow",
                                    utf8ToUtf16(OSettings->getGameName()).c_str(),
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                    posX, posY, resolution.x, resolution.y,
                                    nullptr, nullptr, nullptr, nullptr);
        }
    }

    HWND Window::getHandle()
    {
        return m_handle;
    }

    void Window::setCursor(HCURSOR cursor)
    {
        m_cursor = cursor;
    }
}

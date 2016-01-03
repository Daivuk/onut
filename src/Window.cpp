#include "onut.h"
#include "Utils.h"
#include "Window.h"
#include <windowsx.h>

namespace onut
{
    LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_DESTROY ||
            msg == WM_CLOSE)
        {
            PostQuitMessage(0);
            return 0;
        }
        else if (msg == WM_SIZE)
        {
            if (ORenderer)
            {
                ORenderer->onResize(POINT{LOWORD(lparam), HIWORD(lparam)});
            }
            if (OUIContext)
            {
                OUIContext->resize(sUIVector2{static_cast<float>(LOWORD(lparam)), static_cast<float>(HIWORD(lparam))});
            }
            if (OWindow)
            {
                if (OWindow->onResize)
                {
                    OWindow->onResize(POINT{LOWORD(lparam), HIWORD(lparam)});
                }
            }
            return 0;
        }
        else if (msg == WM_SETCURSOR)
        {
            if (OWindow->m_cursor)
            {
                SetCursor(OWindow->m_cursor);
                return 0;
            }
        }
        else if (msg == WM_SYSCOMMAND)
        {
            if (wparam == SC_KEYMENU && (lparam >> 16) <= 0)
            {
                return 0;
            }
        }
        else if (msg == WM_CHAR)
        {
            auto c = (char)wparam;
            if (OWindow->onWrite)
            {
                OWindow->onWrite(c);
                return 0;
            }
        }
        else if (msg == WM_KEYDOWN)
        {
            if (OWindow->onKey)
            {
                OWindow->onKey(static_cast<uintptr_t>(wparam));
                return 0;
            }
        }
        else if (msg == WM_COMMAND)
        {
            if (OWindow->onMenu)
            {
                OWindow->onMenu(LOWORD(wparam));
                return 0;
            }
        }
        else if (msg == WM_DROPFILES)
        {
            if (OWindow->onDrop)
            {
                char lpszFile[MAX_PATH] = {0};
                UINT uFile = 0;
                HDROP hDrop = (HDROP)wparam;

                uFile = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, NULL);
                if (uFile != 1)
                {
                    MessageBoxA(handle, "Dropping multiple files is not supported.", NULL, MB_ICONERROR);
                    DragFinish(hDrop);
                    return 0;
                }
                lpszFile[0] = '\0';
                if (DragQueryFileA(hDrop, 0, lpszFile, MAX_PATH))
                {
                    OWindow->onDrop(lpszFile);
                }

                DragFinish(hDrop);
                return 0;
            }
        }

        return DefWindowProc(handle, msg, wparam, lparam);
    }

    Window::Window(const POINT& resolution, bool isResizable) :
        m_cursor(0)
    {
        auto bIsFullscreen = OSettings->getBorderlessFullscreen();

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

        if (bIsFullscreen)
        {
            OSettings->setResolution({screenW, screenH});
            long posX = 0;
            long posY = 0;
            m_handle = CreateWindow(L"OakNutWindow",
                                    utf8ToUtf16(OSettings->getGameName()).c_str(),
                                    WS_POPUP | WS_VISIBLE,
                                    posX, posY, screenW, screenH,
                                    nullptr, nullptr, nullptr, nullptr);
        }
        else
        {
            auto posX = (screenW - resolution.x) / 2;
            auto posY = (screenH - resolution.y) / 2;

            // Create the window
            if (!isResizable)
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

            RECT clientRect;
            GetClientRect(m_handle, &clientRect);
            auto wDiff = resolution.x - (clientRect.right - clientRect.left);
            auto hDiff = resolution.y - (clientRect.bottom - clientRect.top);
            auto newW = resolution.x + wDiff;
            auto newH = resolution.y + hDiff;
            posX = (screenW - newW) / 2;
            posY = (screenH - newH) / 2;
            SetWindowPos(m_handle, NULL, posX, posY, newW, newH, 0);

            DragAcceptFiles(m_handle, TRUE);
        }
    }

    Window::~Window()
    {
    }

    HWND Window::getHandle()
    {
        return m_handle;
    }

    void Window::setCursor(HCURSOR cursor)
    {
        m_cursor = cursor;
    }

    void Window::setCaption(const std::string& newName)
    {
        SetWindowTextA(m_handle, newName.c_str());
    }
}

#if defined(WIN32)

// Onut
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Strings.h>
#include <onut/UIContext.h>

// Internal
#include "WindowWIN32.h"

// Third party
#include <windowsx.h>

namespace onut
{
    LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_DESTROY ||
            msg == WM_CLOSE)
        {
            if (oWindow)
            {
                if (oWindow->onQuit)
                {
                    if (!oWindow->onQuit()) return 0;
                }
            }
            PostQuitMessage(0);
            return 0;
        }
        else if (msg == WM_SIZE)
        {
            Point newRes(static_cast<int>(LOWORD(lparam)), static_cast<int>(HIWORD(lparam)));
            if (oRenderer)
            {
                oRenderer->onResize(newRes);
            }
            if (oUIContext)
            {
                oUIContext->resize(Vector2{static_cast<float>(newRes.x), static_cast<float>(newRes.y)});
            }
            if (oWindow)
            {
                if (oWindow->onResize)
                {
                    oWindow->onResize(newRes);
                }
            }
            return 0;
        }
        else if (msg == WM_SETCURSOR)
        {
            if (oWindow)
            {
                if (oWindow->getCursor())
                {
                    SetCursor(oWindow->getCursor());
                    return 0;
                }
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
            if (oWindow)
            {
                if (oWindow->onWrite)
                {
                    oWindow->onWrite(c);
                    return 0;
                }
            }
        }
        else if (msg == WM_KEYDOWN)
        {
            if (oWindow)
            {
                if (oWindow->onKey)
                {
                    oWindow->onKey(static_cast<uintptr_t>(wparam));
                    return 0;
                }
            }
        }
        else if (msg == WM_COMMAND)
        {
            if (oWindow)
            {
                if (oWindow->onMenu)
                {
                    oWindow->onMenu(static_cast<uint32_t>(LOWORD(wparam)));
                    return 0;
                }
            }
        }
        else if (msg == WM_DROPFILES)
        {
            if (oWindow)
            {
                if (oWindow->onDrop)
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
                        oWindow->onDrop(lpszFile);
                    }

                    DragFinish(hDrop);
                    return 0;
                }
            }
        }

        return DefWindowProc(handle, msg, wparam, lparam);
    }

    OWindowRef Window::create()
    {
        return std::shared_ptr<Window>(new WindowWIN32());
    }

    WindowWIN32::WindowWIN32()
        : m_cursor(0)
    {
        auto bIsFullscreen = oSettings->getBorderlessFullscreen();
        Point resolution = oSettings->getResolution();

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
            oSettings->setResolution({screenW, screenH});
            long posX = 0;
            long posY = 0;
            m_handle = CreateWindow(L"OakNutWindow",
                                    utf8ToUtf16(oSettings->getGameName()).c_str(),
                                    WS_POPUP | WS_VISIBLE,
                                    posX, posY, screenW, screenH,
                                    nullptr, nullptr, nullptr, nullptr);
        }
        else
        {
            auto posX = (screenW - resolution.x) / 2;
            auto posY = (screenH - resolution.y) / 2;

            // Create the window
            if (!oSettings->getIsResizableWindow())
            {
                m_handle = CreateWindow(L"OakNutWindow",
                                        utf8ToUtf16(oSettings->getGameName()).c_str(),
                                        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                        posX, posY, resolution.x, resolution.y,
                                        nullptr, nullptr, nullptr, nullptr);
            }
            else
            {
                m_handle = CreateWindow(L"OakNutWindow",
                                        utf8ToUtf16(oSettings->getGameName()).c_str(),
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

    WindowWIN32::~WindowWIN32()
    {
    }

    HWND WindowWIN32::getHandle()
    {
        return m_handle;
    }

    void WindowWIN32::setCursor(HCURSOR cursor)
    {
        m_cursor = cursor;
        SetCursor(m_cursor);
    }

    HCURSOR WindowWIN32::getCursor() const
    {
        return m_cursor;
    }

    void WindowWIN32::setCaption(const std::string& newName)
    {
        SetWindowTextA(m_handle, newName.c_str());
    }
}

#endif

// Onut
#include <onut/Input.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Strings.h>
#include <onut/UIContext.h>

// Internal
#include "InputDeviceDI8.h"
#include "JSBindings.h"
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
            if (oInputDevice)
            {
                auto pInputDeviceDI8 = ODynamicCast<InputDeviceDI8>(oInputDevice);
                if (pInputDeviceDI8)
                {
                    if (pInputDeviceDI8->getCursor())
                    {
                        SetCursor(pInputDeviceDI8->getCursor());
                        return 0;
                    }
                }
            }
        }
        else if (msg == WM_LBUTTONDBLCLK)
        {
            oInput->onDoubleClicked();
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
            // Dumb implementation first
            {
                auto c = (char)wparam;
                if (oWindow)
                {
                    if (oWindow->onWrite)
                    {
                        oWindow->onWrite(c);
                    }
                }
            }

            // Proper, UTF8 version with proper function keys
            {
                switch (wparam) 
                { 
                    case 0x08: 
                    case 0x0A: 
                    case 0x1B: 
                    case 0x09: 
                    case 0x0D: 
                        if (oWindow->onWriteFunc)
                        {
                            oWindow->onWriteFunc((WriteFunc)wparam);
                        }
                        onut::js::onWriteFunc((int)wparam);
                        break;
                    default:
                        std::wstring wstr;
                        wstr += (wchar_t)wparam;
                        auto strUTF8 = onut::utf16ToUtf8(wstr);
                        if (oWindow->onWriteUTF8)
                        {
                            oWindow->onWriteUTF8(strUTF8);
                        }
                        onut::js::onWriteString(strUTF8);
                        break;
                }
            }

            return 0;
        }
        else if (msg == WM_LBUTTONDOWN)
        {
            if (oWindow && oWindow->onButtonDown) oWindow->onButtonDown(0);
            if (oInput)
            {
                oInput->setStateDown(static_cast<onut::Input::State>(static_cast<int>(OMouse1)));
            }
        }
        else if (msg == WM_LBUTTONUP)
        {
            if (oWindow && oWindow->onButtonUp) oWindow->onButtonUp(0);
            if (oInput)
            {
                oInput->setStateUp(static_cast<onut::Input::State>(static_cast<int>(OMouse1)));
            }
        }
        else if (msg == WM_RBUTTONDOWN)
        {
            if (oWindow && oWindow->onButtonDown) oWindow->onButtonDown(1);
            if (oInput)
            {
                oInput->setStateDown(static_cast<onut::Input::State>(static_cast<int>(OMouse2)));
            }
        }
        else if (msg == WM_RBUTTONUP)
        {
            if (oWindow && oWindow->onButtonUp) oWindow->onButtonUp(1);
            if (oInput)
            {
                oInput->setStateUp(static_cast<onut::Input::State>(static_cast<int>(OMouse2)));
            }
        }
        else if (msg == WM_MBUTTONDOWN)
        {
            if (oWindow && oWindow->onButtonDown) oWindow->onButtonDown(2);
            if (oInput)
            {
                oInput->setStateDown(static_cast<onut::Input::State>(static_cast<int>(OMouse3)));
            }
        }
        else if (msg == WM_MBUTTONUP)
        {
            if (oWindow && oWindow->onButtonUp) oWindow->onButtonUp(2);
            if (oInput)
            {
                oInput->setStateUp(static_cast<onut::Input::State>(static_cast<int>(OMouse3)));
            }
        }
        else if (msg == WM_KEYDOWN)
        {
            if (oWindow)
            {
                if (oWindow->onKeyNoRepeat && (lparam & 0xFF) == 1)
                {
                    oWindow->onKeyNoRepeat(static_cast<uintptr_t>(wparam));
                }
                if (oWindow->onKey)
                {
                    oWindow->onKey(static_cast<uintptr_t>(wparam));
                    if (wparam == VK_END || wparam == VK_HOME || 
                        wparam == VK_LEFT || wparam == VK_UP || wparam == VK_RIGHT || wparam == VK_DOWN ||
                        wparam == VK_DELETE)
                    {
                        if (oWindow->onWriteFunc)
                        {
                            oWindow->onWriteFunc((WriteFunc)wparam);
                        }
                        onut::js::onWriteFunc((int)wparam);
                    }
                    return 0;
                }
            }
        }
        else if (msg == WM_KEYUP)
        {
            if (oWindow)
            {
                if (oWindow->onKeyUp)
                {
                    oWindow->onKeyUp(static_cast<uintptr_t>(wparam));
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
        else if (msg == WM_SETFOCUS)
        {
            if (oWindow)
            {
                static_cast<WindowWIN32*>(oWindow.get())->m_hasFocus = true;
            }
        }
        else if (msg == WM_KILLFOCUS)
        {
            if (oWindow)
            {
                static_cast<WindowWIN32*>(oWindow.get())->m_hasFocus = false;
            }
        }
        else if (msg == WM_INPUT)
        {
            if (oWindow)
            {
                UINT dwSize = sizeof(RAWINPUT);
                static BYTE lpb[sizeof(RAWINPUT)];

                GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

                RAWINPUT* raw = (RAWINPUT*)lpb;

                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    static_cast<WindowWIN32*>(oWindow.get())->m_accumMouseDelta[0] += raw->data.mouse.lLastX;
                    static_cast<WindowWIN32*>(oWindow.get())->m_accumMouseDelta[1] += raw->data.mouse.lLastY;
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
    {
        m_isFullScreen = oSettings->getBorderlessFullscreen();
        m_resSetting = oSettings->getResolution();

        // Define window style
        WNDCLASSA wc = {0};
        wc.style = CS_OWNDC | CS_DBLCLKS;
        wc.lpfnWndProc = WinProc;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = "OakNutWindow";
        RegisterClassA(&wc);

        // Centered position
        auto screenW = GetSystemMetrics(SM_CXSCREEN);
        auto screenH = GetSystemMetrics(SM_CYSCREEN);

        if (m_isFullScreen)
        {
            oSettings->setResolution({screenW, screenH});
            long posX = 0;
            long posY = 0;
            m_handle = CreateWindowA("OakNutWindow",
                                    oSettings->getGameName().c_str(),
                                    WS_POPUP | WS_VISIBLE,
                                    posX, posY, screenW, screenH,
                                    nullptr, nullptr, nullptr, nullptr);
        }
        else
        {
            auto posX = (screenW - m_resSetting.x) / 2;
            auto posY = (screenH - m_resSetting.y) / 2 - 20;

            // Create the window
            if (!oSettings->getIsResizableWindow())
            {
                m_handle = CreateWindowA("OakNutWindow",
                                        oSettings->getGameName().c_str(),
                                        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
                                        posX, posY, m_resSetting.x, m_resSetting.y,
                                        nullptr, nullptr, nullptr, nullptr);
            }
            else
            {
                if (oSettings->getStartMaximized())
                {
                    m_handle = CreateWindowA("OakNutWindow",
                                            oSettings->getGameName().c_str(),
                                            WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE,
                                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                            nullptr, nullptr, nullptr, nullptr);
                }
                else
                {
                    m_handle = CreateWindowA("OakNutWindow",
                                            oSettings->getGameName().c_str(),
                                            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                            posX, posY, m_resSetting.x, m_resSetting.y,
                                            nullptr, nullptr, nullptr, nullptr);
                }
            }

            if (!oSettings->getStartMaximized())
            {
                RECT clientRect;
                GetClientRect(m_handle, &clientRect);
                auto wDiff = m_resSetting.x - (clientRect.right - clientRect.left);
                auto hDiff = m_resSetting.y - (clientRect.bottom - clientRect.top);
                auto newW = m_resSetting.x + wDiff;
                auto newH = m_resSetting.y + hDiff;
                posX = (screenW - newW) / 2;
                posY = (screenH - newH) / 2 - 20;
                SetWindowPos(m_handle, NULL, posX, posY, newW, newH, 0);
            }

            DragAcceptFiles(m_handle, TRUE);
        }

        ShowWindow(m_handle, SW_SHOW);
        UpdateWindow(m_handle);

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

        RAWINPUTDEVICE rid;
        rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
        rid.usUsage = HID_USAGE_GENERIC_MOUSE; // Mouse
        rid.dwFlags = RIDEV_INPUTSINK; // or 0 if you only want input when focused
        rid.hwndTarget = m_handle;
        RegisterRawInputDevices(&rid, 1, sizeof(rid));
    }

    WindowWIN32::~WindowWIN32()
    {
    }

    HWND WindowWIN32::getHandle()
    {
        return m_handle;
    }

    void WindowWIN32::setCaption(const std::string& newName)
    {
        SetWindowTextA(m_handle, newName.c_str());
    }

    void WindowWIN32::setFullscreen(bool isFullscreen)
    {
        if (m_isFullScreen == isFullscreen || !m_handle) return; // Do nothing
        m_isFullScreen = isFullscreen;

        // Centered position
        auto screenW = GetSystemMetrics(SM_CXSCREEN);
        auto screenH = GetSystemMetrics(SM_CYSCREEN);

        if (m_isFullScreen)
        {
            oSettings->setResolution({screenW, screenH});
            SetWindowLong(m_handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
            SetWindowPos(m_handle, NULL, 0, 0, screenW, screenH, 0);
            oRenderer->onResize(Point(screenW, screenH));
            if (oWindow->onResize)
            {
                oWindow->onResize(Point(screenW, screenH));
            }
        }
        else
        {
            auto posX = (screenW - m_resSetting.x) / 2;
            auto posY = (screenH - m_resSetting.y) / 2;

            // Create the window
            if (!oSettings->getIsResizableWindow())
            {
                SetWindowLong(m_handle, GWL_STYLE, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE);
            }
            else
            {
                SetWindowLong(m_handle, GWL_STYLE, WS_POPUP | WS_OVERLAPPEDWINDOW | WS_VISIBLE);
            }
            SetWindowPos(m_handle, NULL, posX, posY, m_resSetting.x, m_resSetting.y, 0);

            RECT clientRect;
            GetClientRect(m_handle, &clientRect);
            auto wDiff = m_resSetting.x - (clientRect.right - clientRect.left);
            auto hDiff = m_resSetting.y - (clientRect.bottom - clientRect.top);
            auto newW = m_resSetting.x + wDiff;
            auto newH = m_resSetting.y + hDiff;
            posX = (screenW - newW) / 2;
            posY = (screenH - newH) / 2;
            SetWindowPos(m_handle, NULL, posX, posY, newW, newH, 0);
        }
    }

    bool WindowWIN32::pollEvents()
    {
        MSG msg = { 0 };
        m_accumMouseDelta[0] = 0;
        m_accumMouseDelta[1] = 0;
        if (oSettings->getIsEditorMode())
        {
            if (GetMessage(&msg, 0, 0, 0) >= 0)
            {
                if (msg.message == WM_INPUT)
                {
                    DispatchMessage(&msg);
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (msg.message == WM_QUIT)
                {
                    return false;
                }

                // Empty remaining queued messages in case. Because Vsync will slow down the flow of messages
                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_INPUT)
                    {
                        DispatchMessage(&msg);
                    }
                    else
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }

                    if (msg.message == WM_QUIT)
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_INPUT)
                {
                    DispatchMessage(&msg);
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (msg.message == WM_QUIT)
                {
                    return false;
                }
            }
        }
        if (oInput)
        {
            oInput->setStateValue(OMouseX, (float)m_accumMouseDelta[0]);
            oInput->setStateValue(OMouseY, (float)m_accumMouseDelta[1]);
        }
        return true;
    }

    void WindowWIN32::setClipboard(const std::string& text)
    {
        if (OpenClipboard(m_handle))
        {
            EmptyClipboard();
            HGLOBAL hClipboardData;
            hClipboardData = GlobalAlloc(GMEM_DDESHARE, text.size() + 1);
            if (hClipboardData)
            {
                char* pchData;
                pchData = (char*)GlobalLock(hClipboardData);
                if (pchData)
                {
                    strcpy(pchData, text.data());
                    GlobalUnlock(hClipboardData);
                    SetClipboardData(CF_TEXT, hClipboardData);
                }
            }
            CloseClipboard();
        }
    }

    std::string WindowWIN32::getClipboard()
    {
        std::string ret;
        if (OpenClipboard(m_handle))
        {
            HANDLE hClipboardData = GetClipboardData(CF_TEXT);
            if (hClipboardData)
            {
                char* pchData = (char*)GlobalLock(hClipboardData);
                if (pchData)
                {
                    ret = pchData;
                    GlobalUnlock(hClipboardData);
                }
            }
            CloseClipboard();
        }
        return ret;
    }
}

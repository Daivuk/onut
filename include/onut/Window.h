#pragma once
// Onut
#include <onut/Point.h>

// Third party
#if defined(WIN32)
#include <Windows.h>
#endif

// STL
#include <functional>
#include <string>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Window);

namespace onut
{
    class Window
    {
    public:
        static OWindowRef create(const Point& resolution, bool isResizable = false);

        Window(const Point& resolution, bool isResizable = false);
        virtual ~Window();

#if defined(WIN32)
        HWND getHandle();
        void setCursor(HCURSOR cursor);
        HCURSOR getCursor() const;
#endif
        void setCaption(const std::string& newName);

        std::function<void(char)>                   onWrite;
        std::function<void(uintptr_t)>              onKey;
        std::function<void(uint32_t)>               onMenu;
        std::function<void(const std::string&)>     onDrop;
        std::function<void(const Point& newSize)>   onResize;
        std::function<bool()>                       onQuit;

    private:
#if defined(WIN32)
        HWND                                    m_handle;
        HCURSOR                                 m_cursor;
#endif
    };
}

extern OWindowRef oWindow;

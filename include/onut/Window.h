#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

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
    enum class WriteFunc : uintptr_t
    {
        Backspace = 0x08,
        Linefeed = 0x0A,
        Escape = 0x1B,
        Tab = 0x09,
        CariageReturn = 0x0D
    };

    class Window
    {
    public:
        static OWindowRef create();

        virtual ~Window();

#if defined(WIN32)
        virtual HWND getHandle() = 0;
#endif
        virtual void setCaption(const std::string& newName) = 0;
        virtual void setFullscreen(bool isFullscreen) {}
        virtual bool pollEvents() = 0;

        std::function<void(char)>                   onWrite;
        std::function<void(WriteFunc)>              onWriteFunc;
        std::function<void(const std::string&)>     onWriteUTF8;
        std::function<void(uintptr_t)>              onKey;
        std::function<void(uint32_t)>               onMenu;
        std::function<void(const std::string&)>     onDrop;
        std::function<void(const Point& newSize)>   onResize;
        std::function<bool()>                       onQuit;

    protected:
        Window();
    };
}

extern OWindowRef oWindow;

#endif

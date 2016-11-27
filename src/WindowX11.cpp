#if defined(__unix__)

// Onut
//#include <onut/Renderer.h>
#include <onut/Settings.h>
//#include <onut/Strings.h>
//#include <onut/UIContext.h>

// Internal
#include "WindowX11.h"

namespace onut
{
    OWindowRef Window::create()
    {
        return std::shared_ptr<Window>(new WindowX11());
    }

    WindowX11::WindowX11()
    {
    }

    WindowX11::~WindowX11()
    {
    }

    void WindowX11::setCaption(const std::string& newName)
    {
    }
}

#endif

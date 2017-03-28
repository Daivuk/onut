// Onut
//#include <onut/Renderer.h>
#include <onut/Settings.h>
//#include <onut/Strings.h>
//#include <onut/UIContext.h>

// Internal
#include "WindowOSX.h"

namespace onut
{
    OWindowRef Window::create()
    {
        return std::shared_ptr<Window>(new WindowOSX());
    }

    WindowOSX::WindowOSX()
    {
    }

    WindowOSX::~WindowOSX()
    {
    }

    void WindowOSX::setCaption(const std::string& newName)
    {
    }
}

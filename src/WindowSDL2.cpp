// Onut
#include <onut/Log.h>
//#include <onut/Renderer.h>
#include <onut/Settings.h>
//#include <onut/Strings.h>
//#include <onut/UIContext.h>

// Internal
#include "WindowSDL2.h"
#include "InputDeviceSDL2.h"

// STL
#include <cassert>

namespace onut
{
    OWindowRef Window::create()
    {
        return std::shared_ptr<Window>(new WindowSDL2());
    }

    WindowSDL2::WindowSDL2()
    {
        // Init SDL
        auto ret = SDL_Init(SDL_INIT_VIDEO);
        if (ret < 0)
        {
            OLog("SDL_Init failed");
            assert(false);
            return;
        }

        // Create SDL Window
        const auto& resolution = oSettings->getResolution();
        bool borderLessFullscreen = oSettings->getBorderlessFullscreen();

        Uint32 flags = SDL_WINDOW_OPENGL |
                       SDL_WINDOW_SHOWN;
        if (borderLessFullscreen)
        {
            OLog("We go full screen");
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP |
                     SDL_WINDOW_BORDERLESS;
        }
        m_pWindow = SDL_CreateWindow(oSettings->getGameName().c_str(),
                                     SDL_WINDOWPOS_CENTERED, 
                                     SDL_WINDOWPOS_CENTERED,
                                     resolution.x,
                                     resolution.y,
                                     flags);
        if (!m_pWindow)
        {
            OLog("SDL_CreateWindow failed");
            assert(false);
            return;
        }
    }

    WindowSDL2::~WindowSDL2()
    {
        if (m_pWindow)
        {
            SDL_DestroyWindow(m_pWindow);
        }
        SDL_Quit();
    }

    void WindowSDL2::setCaption(const std::string& newName)
    {
    }

    SDL_Window* WindowSDL2::getSDLWindow() const
    {
        return m_pWindow;
    }

    bool WindowSDL2::pollEvents()
    {
        if (!m_pWindow) return false;

        SDL_Event event;
        auto pInputDeviceSDL2 = ODynamicCast<InputDeviceSDL2>(oInputDevice);
        bool hadEvents = false;
        while (SDL_PollEvent(&event))
        {
            hadEvents = true;

            switch (event.type)
            {
                case SDL_QUIT:
                    return false;
                case SDL_TEXTINPUT:
                {
                    auto len = strlen(event.text.text);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        auto c = event.text.text[i];
                        if (oWindow)
                        {
                            if (oWindow->onWrite)
                            {
                                oWindow->onWrite(c);
                            }
                        }
                    }
                    break;
                }
                case SDL_KEYDOWN:
                {
                    if (oWindow)
                    {
                        if (oWindow->onKey)
                        {
                            oWindow->onKey(static_cast<uintptr_t>(event.key.keysym.sym));

                            if (oWindow->onWrite)
                            {
                                switch (event.key.keysym.scancode)
                                {
                                    case SDL_SCANCODE_BACKSPACE:
                                        oWindow->onWrite('\b');
                                        break;
                                    case SDL_SCANCODE_RETURN:
                                        oWindow->onWrite('\r');
                                        break;
                                    case SDL_SCANCODE_ESCAPE:
                                        oWindow->onWrite('\x1b');
                                        break;
                                    case SDL_SCANCODE_TAB:
                                        oWindow->onWrite('\t');
                                        break;
                                    default: break;
                                }
                            }
                        }
                    }
                    break;
                }
            }

            // Everytime we poll, we need to update inputs
            pInputDeviceSDL2->updateSDL2();
        }

        if (!hadEvents) pInputDeviceSDL2->updateSDL2();

        return true;
    }
}

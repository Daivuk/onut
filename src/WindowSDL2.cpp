// Onut
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>

// Internal
#include "WindowSDL2.h"
#include "InputDeviceSDL2.h"
#include "GamePadSDL2.h"

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
        bool resizable = oSettings->getIsResizableWindow();

        Uint32 flags = SDL_WINDOW_OPENGL |
                       SDL_WINDOW_SHOWN;
        if (borderLessFullscreen)
        {
            OLog("We go full screen");
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP |
                     SDL_WINDOW_BORDERLESS;
        }
        else if (resizable)
        {
            flags |= SDL_WINDOW_RESIZABLE;
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

        oInput->setStateValue(OMouseZ, 0);
        oInput->setStateValue(OMouseW, 0);

        SDL_Event event;
        auto pInputDeviceSDL2 = ODynamicCast<InputDeviceSDL2>(oInputDevice);
        bool hadEvents = false;
        while (SDL_PollEvent(&event))
        {
            hadEvents = true;

            switch (event.type)
            {
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        {
                            if (oRenderer) oRenderer->onResize({event.window.data1, event.window.data2});
                            break;
                        }
                    }
                    break;
                }
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
                case SDL_CONTROLLERDEVICEADDED:
                {
                    OLog("Controller added: " + std::to_string(event.cdevice.which));
                    auto pGamePad = ODynamicCast<GamePadSDL2>(OGetGamePad(event.cdevice.which));
                    if (pGamePad)
                    {
                        pGamePad->onAdded();
                    }
                    break;
                }
                case SDL_MOUSEWHEEL:
                {
                    oInput->setStateValue(OMouseZ, oInput->getStateValue(OMouseZ) + event.wheel.y);
                    oInput->setStateValue(OMouseW, oInput->getStateValue(OMouseW) + event.wheel.x);
                    break;
                }
                case SDL_CONTROLLERDEVICEREMOVED:
                {
#if SDL_VERSION_ATLEAST(2, 0, 4)
                    SDL_GameController* pFromInstanceId = SDL_GameControllerFromInstanceID(event.cdevice.which);
                    if (pFromInstanceId)
                    {
                        for (int i = 0; i < 4; ++i)
                        {
                            auto pGamePad = ODynamicCast<GamePadSDL2>(OGetGamePad(i));
                            if (pGamePad)
                            {
                                if (pGamePad->getSDLController() == pFromInstanceId)
                                {
                                    OLog("Controller removed: " + std::to_string(i));
                                    pGamePad->onRemoved();
                                    // Move down the controllers after
                                    for (; i < 3; ++i)
                                    {
                                        auto pGamePadFrom = ODynamicCast<GamePadSDL2>(OGetGamePad(i + 1));
                                        auto pGamePadTo = ODynamicCast<GamePadSDL2>(OGetGamePad(i));
                                        pGamePadTo->setSDLController(pGamePadFrom->getSDLController());
                                        pGamePadFrom->setSDLController(nullptr);
                                    }
                                    break;
                                }
                            }
                        }
                    }
#endif
                    break;
                }
            }

            // Everytime we poll, we need to update inputs
            pInputDeviceSDL2->updateSDL2();
            for (int i = 0; i < 4; ++i)
            {
                auto pGamePad = ODynamicCast<GamePadSDL2>(OGetGamePad(i));
                if (pGamePad)
                {
                    pGamePad->updateSDL2();
                }
            }
        }

        if (!hadEvents)
        {
            pInputDeviceSDL2->updateSDL2();
            for (int i = 0; i < 4; ++i)
            {
                auto pGamePad = ODynamicCast<GamePadSDL2>(OGetGamePad(i));
                if (pGamePad)
                {
                    pGamePad->updateSDL2();
                }
            }
        }

        return true;
    }
}

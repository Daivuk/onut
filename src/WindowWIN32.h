#ifndef WINDOWWIN32_H_INCLUDED
#define WINDOWWIN32_H_INCLUDED

// Onut
#include <onut/Window.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(WindowWIN32)

namespace onut
{
    class WindowWIN32 final : public Window
    {
    public:
        WindowWIN32();
        ~WindowWIN32();

        HWND getHandle();
        void setCaption(const std::string& newName) override;
        void setFullscreen(bool isFullscreen) override;
        bool pollEvents() override;
        void setClipboard(const std::string& text) override;
        std::string getClipboard() override;
        bool hasFocus() const override { return m_hasFocus; };

    private:
        friend LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        HWND m_handle;
        bool m_isFullScreen = false;
        Point m_resSetting;
        bool m_hasFocus = true;
        int m_accumMouseDelta[2] = { 0, 0 };
    };
}

#endif

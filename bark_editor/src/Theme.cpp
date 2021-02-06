#include <onut/Font.h>
#include <onut/Texture.h>
#include "Theme.h"

Theme::Theme()
{
    font        = OGetFont("font.fnt");

    menu_icon   = OGetTexture("menu_icon.png");
    x_icon      = OGetTexture("x_icon.png");

    scrollbar   = OGetTexture("scrollbar.png");
}

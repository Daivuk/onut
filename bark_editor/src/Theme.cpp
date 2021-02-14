#include <onut/Font.h>
#include <onut/Texture.h>
#include "Theme.h"

Theme::Theme()
{
    font        = OGetFont("font.fnt");

    menu_icon   = OGetTexture("menu_icon.png");
    x_icon      = OGetTexture("x_icon.png");

    asset_icons[(int)eAssetType::Folder]            = OGetTexture("Folder_icon.png");
    asset_icons[(int)eAssetType::ExpandedFolder]    = OGetTexture("ExpandedFolder_icon.png");
    asset_icons[(int)eAssetType::File]              = OGetTexture("File_icon.png");

    asset_icons[(int)eAssetType::Scene]             = OGetTexture("Scene_icon.png");
    asset_icons[(int)eAssetType::Prefab]            = OGetTexture("Prefab_icon.png");

    asset_icons[(int)eAssetType::Texture]           = OGetTexture("Texture_icon.png");
    asset_icons[(int)eAssetType::Sound]             = OGetTexture("Sound_icon.png");
    asset_icons[(int)eAssetType::TiledMap]          = OGetTexture("TiledMap_icon.png");
    asset_icons[(int)eAssetType::SpriteAnim]        = OGetTexture("SpriteAnim_icon.png");
    asset_icons[(int)eAssetType::Model]             = OGetTexture("Model_icon.png");
    asset_icons[(int)eAssetType::Shader]            = OGetTexture("Shader_icon.png");
    asset_icons[(int)eAssetType::Script]            = OGetTexture("Script_icon.png");
    asset_icons[(int)eAssetType::Music]             = OGetTexture("Music_icon.png");
    asset_icons[(int)eAssetType::UI]                = OGetTexture("UI_icon.png");

    {
        auto prev = oGenerateMipmaps;
        oGenerateMipmaps = false;

        scrollbar   = OGetTexture("scrollbar.png");
        dotted      = OGetTexture("dotted.png");

        oGenerateMipmaps = prev;
    }
}

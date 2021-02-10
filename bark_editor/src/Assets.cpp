#include <map>
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Strings.h>
#include <onut/Sound.h>
#include <onut/Music.h>
#include "Assets.h"
#include "GUIContext.h"
#include "Project.h"
#include "globals.h"

static const std::map<std::string, eAssetType> ASSET_TYPES = {
    { "SCENE", eAssetType::Scene },
    { "PREFAB", eAssetType::Prefab },

    { "JPEG", eAssetType::Texture },
    { "JPG", eAssetType::Texture },
    { "PNG", eAssetType::Texture },
    { "BMP", eAssetType::Texture },
    { "PSD", eAssetType::Texture },
    { "TGA", eAssetType::Texture },
    { "GIF", eAssetType::Texture },
    { "HDR", eAssetType::Texture },
    { "PIC", eAssetType::Texture },
    { "PPM", eAssetType::Texture },
    { "PGM", eAssetType::Texture },
    { "TEXTURE", eAssetType::Texture },

    { "WAV", eAssetType::Sound },
    { "SOUND", eAssetType::Sound },

    { "OGG", eAssetType::Music },
    { "MUSIC", eAssetType::Music },

    { "TMX", eAssetType::TiledMap },

    { "XML", eAssetType::SpriteAnim },
    { "SPRITEANIM", eAssetType::SpriteAnim },

    { "AMF", eAssetType::Model },
    { "3DS", eAssetType::Model },
    { "AC", eAssetType::Model },
    { "ASE", eAssetType::Model },
    { "ASSBIN", eAssetType::Model },
    { "ASSXML", eAssetType::Model },
    { "B3D", eAssetType::Model },
    { "BVH", eAssetType::Model },
    { "COLLADA", eAssetType::Model },
    { "DXF", eAssetType::Model },
    { "CSM", eAssetType::Model },
    { "HMP", eAssetType::Model },
    { "IRRMESH", eAssetType::Model },
    { "IRR", eAssetType::Model },
    { "LWO", eAssetType::Model },
    { "LWS", eAssetType::Model },
    { "MD2", eAssetType::Model },
    { "MD3", eAssetType::Model },
    { "MD5", eAssetType::Model },
    { "MDC", eAssetType::Model },
    { "MDL", eAssetType::Model },
    { "NFF", eAssetType::Model },
    { "NDO", eAssetType::Model },
    { "OFF", eAssetType::Model },
    { "OBJ", eAssetType::Model },
    { "OGRE", eAssetType::Model },
    { "OPENGEX", eAssetType::Model },
    { "PLY", eAssetType::Model },
    { "MS3D", eAssetType::Model },
    { "COB", eAssetType::Model },
    { "BLEND", eAssetType::Model },
    { "IFC", eAssetType::Model },
    { "XGL", eAssetType::Model },
    { "FBX", eAssetType::Model },
    { "Q3D", eAssetType::Model },
    { "Q3BSP", eAssetType::Model },
    { "RAW", eAssetType::Model },
    { "SIB", eAssetType::Model },
    { "SMD", eAssetType::Model },
    { "STL", eAssetType::Model },
    { "TERRAGEN", eAssetType::Model },
    { "3D", eAssetType::Model },
    { "X", eAssetType::Model },
    { "X3D", eAssetType::Model },
    { "GLTF", eAssetType::Model },
    { "3MF", eAssetType::Model },
    { "MMD", eAssetType::Model },
    { "MODEL", eAssetType::Model },

    { "VS", eAssetType::Shader },
    { "ONUTVS", eAssetType::Shader },
    { "PS", eAssetType::Shader },
    { "ONUTPS", eAssetType::Shader },

    { "JS", eAssetType::Script },

    { "UI", eAssetType::UI },
};

Assets::Assets(const std::string& in_assets_path)
    : assets_path(in_assets_path)
{
    root.expanded = true; // Cannot collapse root

    auto files = onut::findAllFiles(assets_path);

    // Make sure they are sorted. The filesystem on the OS might be in a different order
    std::sort(files.begin(), files.end());

    for (const auto& file : files)
    {
        putFile(file);
    }
}

Assets::~Assets()
{
}

AssetDir* Assets::getAssetDir(AssetDir* parent, const std::vector<std::string>& dirs)
{
    AssetDir* ret = parent;
    for (const auto& dir : dirs)
    {
        bool found = false;
        for (auto& sub_dir : ret->sub_dirs)
        {
            if (sub_dir.name == dir)
            {
                found = true;
                ret = &sub_dir;
                break;
            }
        }

        if (!found)
        {
            // Create a new sub dir here
            AssetDir sub_dir;
            sub_dir.name = dir;
            ret->sub_dirs.push_back(sub_dir);
            ret = &ret->sub_dirs.back();
        }
    }

    return ret;
}

void Assets::putFile(const std::string& file)
{
    auto rel_file   = onut::makeRelativePath(file, assets_path);
    auto dirs       = onut::splitString(rel_file, '/');

    dirs.pop_back(); // Last item should be our file, remove it from the dirs

    AssetDir* dir = getAssetDir(&root, dirs);

    Asset asset;

    //asset.name      = onut::getFilenameWithoutExtension(rel_file);
    asset.name      = onut::getFilename(rel_file);
    asset.filename  = onut::getFilename(rel_file);
    asset.full_path = file;

    {
        auto ext = onut::getExtension(rel_file);
        auto it  = ASSET_TYPES.find(ext);
        if (it != ASSET_TYPES.end())
        {
            asset.type = it->second;
        }
        else
        {
            asset.type = eAssetType::File;
        }
    }

    dir->assets.push_back(asset);
}

void Assets::deselectAll(AssetDir* parent_dir)
{
    parent_dir->selected = false;
    for (auto& dir : parent_dir->sub_dirs)
    {
        deselectAll(&dir);
    }
    for (auto& asset : parent_dir->assets)
    {
        asset.selected = false;
    }
}

void Assets::addSelection(GUIContext* ctx, AssetDir* dir)
{
    if (ctx->keys.ctrl)
    {
        if (ctx->keys.shift)
        {
        }
        else
        {
            dir->selected = !dir->selected;
        }
    }
    else
    {
        if (ctx->keys.shift)
        {
        }
        else
        {
            deselectAll(&root);
            dir->selected = true;
        }
    }
}

void Assets::addSelection(GUIContext* ctx, Asset* asset)
{
    if (ctx->keys.ctrl)
    {
        if (ctx->keys.shift)
        {
        }
        else
        {
            asset->selected = !asset->selected;
        }
    }
    else
    {
        if (ctx->keys.shift)
        {
        }
        else
        {
            deselectAll(&root);
            asset->selected = true;
        }
    }
}

void Assets::open(Asset* asset)
{
    if (!asset) return;

    switch (asset->type)
    {
        case eAssetType::Scene:
        case eAssetType::Prefab:
            g_project->openScene(asset->filename);
            break;

        case eAssetType::Sound:
            // Too much code, didn't feel like thinking this through...
            if (playing_sound && playing_sound->isPlaying())
            {
                playing_sound->stop();
                if (asset->filename != playing_sound_name)
                {
                    auto sound = g_content_mgr->getResourceAs<OSound>(asset->filename);
                    if (sound) playing_sound = sound->createInstance();
                    if (playing_sound) playing_sound->play();
                    playing_sound_name = asset->filename;
                }
            }
            else
            {
                auto sound = g_content_mgr->getResourceAs<OSound>(asset->filename);
                if (sound) playing_sound = sound->createInstance();
                if (playing_sound) playing_sound->play();
                playing_sound_name = asset->filename;
            }
            break;

        case eAssetType::Music:
            if (playing_music && playing_music->isPlaying())
            {
                playing_music->stop();
                if (asset->filename != playing_music_name)
                {
                    playing_music = g_content_mgr->getResourceAs<OMusic>(asset->filename);
                    if (playing_music) playing_music->play();
                    playing_music_name = asset->filename;
                }
            }
            else
            {
                playing_music = g_content_mgr->getResourceAs<OMusic>(asset->filename);
                if (playing_music) playing_music->play();
                playing_music_name = asset->filename;
            }
            break;

        case eAssetType::Shader:
        case eAssetType::Script:
            onut::openFile(asset->full_path);
            break;
    }
}

#pragma once

#include <onut/ForwardDeclaration.h>
#include <string>
#include <map>

OForwardDeclare(Music);
OForwardDeclare(SoundInstance);

class GUIContext;

enum class eAssetType : int
{
    Folder,
    ExpandedFolder,
    File,

    Scene,
    Prefab,

    Texture,
    Sound,
    TiledMap,
    SpriteAnim,
    Model,
    Shader,
    Script,
    Music,
    UI,

    COUNT
};

struct Asset
{
    eAssetType  type;
    bool        selected = false;
    std::string name;
    std::string filename;
    std::string full_path;
};

struct AssetDir
{
    bool                    expanded = false;
    bool                    selected = false;
    std::string             name;
    std::vector<AssetDir>   sub_dirs;
    std::vector<Asset>      assets;
};

class Assets final
{
public:
    std::string assets_path;
    AssetDir    root;

    OMusicRef           playing_music;
    std::string         playing_music_name;
    OSoundInstanceRef   playing_sound;
    std::string         playing_sound_name;

    Assets(const std::string& assets_path);
    ~Assets();

    void deselectAll(AssetDir* dir);
    void addSelection(GUIContext* ctx, AssetDir* dir);
    void addSelection(GUIContext* ctx, Asset* asset);

    void open(Asset* asset);

    void putFile(const std::string& file);
    AssetDir* getAssetDir(AssetDir* parent, const std::vector<std::string>& dirs);
};

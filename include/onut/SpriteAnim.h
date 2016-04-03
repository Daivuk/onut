#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/Resource.h>
#include <onut/Updater.h>

// STL
#include <string>
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager);
OForwardDeclare(SpriteAnim);
OForwardDeclare(SpriteAnimInstance);
OForwardDeclare(Texture);

namespace onut
{
    class SpriteAnim final : public Resource
    {
    public:
        struct Frame
        {
            OTextureRef pTexture;
            Vector4 UVs = {0, 0, 1, 1};
            Vector2 origin = {.5f, .5f};
        };
        using Frames = std::vector<Frame>;

        struct Anim
        {
            std::string name;
            Frames frames;
            float duration = 1.f;
            bool loop = false;
            std::string next;
        };

        static OSpriteAnimRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);

        void addAnim(const Anim& anim);
        Anim* getAnim(const std::string& name);
        std::vector<std::string> getAnimNames() const;
        std::string getFilename() const { return m_filename; }

    private:
        using AnimMap = std::unordered_map<std::string, Anim>;

        AnimMap m_anims;
        std::string m_filename;
    };
}

namespace onut
{
    class SpriteAnimInstance final : public UpdateTarget
    {
    public:
        SpriteAnimInstance();
        SpriteAnimInstance(const OSpriteAnimRef& pSpriteAnim);
        SpriteAnimInstance(const std::string& filename, const OContentManagerRef& pContentManager = nullptr);

        void play(const std::string& animName, float framePerSecond = 0.f);
        void playBackward(const std::string& animName, float framePerSecond = 0.f);
        void queueAnim(const std::string& animName);
        void stop(bool reset = false);
        bool isPlaying() const;

        int getFrameId() const;
        OSpriteAnimRef getDefinition() const { return m_pSpriteAnim; }
        SpriteAnim::Anim* getCurrentAnim() const { return m_pCurrentAnim; }

        OTextureRef getTexture() const;
        const Vector4& getUVs() const;
        const Vector2& getOrigin() const;

        void update() override;

    private:
        using AnimQueue = std::vector<std::string>;

        void playNextQueuedAnim();

        OSpriteAnimRef m_pSpriteAnim;
        SpriteAnim::Anim* m_pCurrentAnim = nullptr;
        float m_frame = 0.f;
        float m_speed = 1.f;
        bool m_bIsPlaying = false;
        AnimQueue m_animQueue;
    };
};

OSpriteAnimRef OGetSpriteAnim(const std::string& name);
OSpriteAnimInstanceRef OPlaySpriteAnim(const std::string& filename, const std::string& animName);

#ifndef SPRITEANIMCOMPONENT_H_INCLUDED
#define SPRITEANIMCOMPONENT_H_INCLUDED


// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteAnim);
OForwardDeclare(SpriteAnimComponent);
OForwardDeclare(SpriteAnimInstance);

namespace onut
{
    class SpriteAnimComponent final : public Component
    {
    public:
        SpriteAnimComponent();

        void setSpriteAnim(const OSpriteAnimRef& pSpriteAnim);
        const OSpriteAnimRef& getSpriteAnim() const;

        void setScale(const Vector2& scale);
        const Vector2& getScale() const;

        void setColor(const Color& color);
        const Color& getColor() const;

        void setDefaultAnim(const std::string& anim);
        const std::string& getDefaultAnim() const;

        void setPlayPaused(bool playPaused);
        bool getPlayPaused() const;

        void play(const std::string& animName, float framePerSecond = 0.f);
        void playBackward(const std::string& animName, float framePerSecond = 0.f);
        void queueAnim(const std::string& animName);
        void stop(bool reset = false);
        bool isPlaying() const;
        void setFPS(float fps);

    private:
        void onCreate() override;
        void onRender2d() override;

        OSpriteAnimInstanceRef m_pSpriteAnimInstance;
        Vector2 m_scale = Vector2(1);
        Color m_color = Color::White;
        OSpriteAnimRef m_pSpriteAnim;
        bool m_playPaused = false;
        std::string m_defaultAnim;
        std::string m_currentlyPlaying;
    };
};

#endif

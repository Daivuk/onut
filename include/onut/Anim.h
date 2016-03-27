#pragma once
// Onut
#include <onut/Curve.h>
#include <onut/Timing.h>
#include <onut/Tween.h>
#include <onut/Updater.h>

// STL
#include <vector>

namespace onut
{
    enum class LoopType
    {
        None,
        Loop,
        PingPongOnce,
        PingPongLoop
    };

    template<typename Ttype>
    class Anim final : public UpdateTarget
    {
    public:
        using KeyFrameCallback = std::function<void()>;
        using KeyFrameCallbacks = std::vector<KeyFrameCallback>;

        struct KeyFrame
        {
            KeyFrame(const Ttype& in_value, float in_duration, Tween in_tween = Tween::Linear, const KeyFrameCallback& in_callback = nullptr) :
                value(in_value),
                duration(in_duration),
                tween(in_tween),
                callback(in_callback)
            {}

            Ttype value;
            float duration;
            Tween tween;
            KeyFrameCallback callback;
        };
        using KeyFrames = std::vector<KeyFrame>;

        Anim() {}
        Anim(const Ttype& rvalue) :
            m_value(rvalue)
        {}

        Anim(const Anim& other)
            : m_value(other.m_value)
            , m_progress(other.m_progress)
            , m_totalTime(other.m_totalTime)
            , m_loop(other.m_loop)
            , m_keyFrames(other.m_keyFrames)
            , m_isPingPonging(other.m_isPingPonging)
            , m_hasCallbacks(other.m_hasCallbacks)
            , m_pMyUpdater(other.m_pMyUpdater)
        {
            if (m_isPlaying)
            {
                m_pMyUpdater->registerTarget(this);
            }
        }
        Anim& operator=(const Anim& other)
        {
            if (m_isPlaying)
            {
                m_pMyUpdater->unregisterTarget(this);
            }
            m_value = other.m_value;
            m_progress = other.m_progress;
            m_totalTime = other.m_totalTime;
            m_loop = other.m_loop;
            m_keyFrames = other.m_keyFrames;
            m_isPingPonging = other.m_isPingPonging;
            m_hasCallbacks = other.m_hasCallbacks;
            m_pMyUpdater = other.m_pMyUpdater;
            if (m_isPlaying)
            {
                m_pMyUpdater->registerTarget(this);
            }
            return *this;
        }

        const Ttype& get() const
        {
            return m_value;
        }

        operator const Ttype&() const
        {
            return m_value;
        }

        const Ttype& operator=(const Ttype& rvalue)
        {
            stop(false);
            m_totalTime = 0.f;
            m_value = rvalue;
            m_progress = 0.f;
            return m_value;
        }

        void play(const Ttype& from, const Ttype& to, float duration, Tween tween = Tween::Linear, LoopType loop = LoopType::None, const KeyFrameCallback& in_callback = nullptr)
        {
            m_keyFrames.clear();
            m_value = from;
            m_keyFrames.push_back(KeyFrame(from, 0.f, Tween::None));
            m_keyFrames.push_back(KeyFrame(to, duration, tween, in_callback));
            m_loop = loop;
            m_progress = 0.f;
            m_totalTime = duration;
            m_isPingPonging = false;
            m_hasCallbacks = in_callback != nullptr;
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->registerTarget(this);
        }

        void queue(const Ttype& to, float duration, Tween tween = Tween::Linear, const KeyFrameCallback& in_callback = nullptr)
        {
            if (m_keyFrames.empty())
            {
                m_hasCallbacks = false;
                m_totalTime = 0.f;
                if (duration > 0.f)
                {
                    m_keyFrames.push_back(KeyFrame(m_value, 0.f, Tween::None));
                }
            }
            m_keyFrames.push_back(KeyFrame(to, duration, tween, in_callback));
            m_totalTime += duration;
            m_hasCallbacks |= in_callback != nullptr;
        }

        void playFromCurrent(const Ttype& to, float duration, Tween tween = Tween::Linear, LoopType loop = LoopType::None, const KeyFrameCallback& in_callback = nullptr)
        {
            m_keyFrames.clear();
            m_keyFrames.push_back(KeyFrame(m_value, 0.f, Tween::None));
            m_keyFrames.push_back(KeyFrame(to, duration, tween, in_callback));
            m_loop = loop;
            m_progress = 0;
            m_totalTime = duration;
            m_isPingPonging = false;
            m_hasCallbacks = in_callback != nullptr;
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->registerTarget(this);
        }

        void playKeyFrames(const Ttype& from, const KeyFrames& keyFrames, LoopType loop = LoopType::None)
        {
            if (keyFrames.empty())
            {
                stop();
                return;
            }
            m_value = from;
            m_keyFrames.clear();
            m_keyFrames.push_back(KeyFrame(m_value, 0.f, Tween::None));
            m_keyFrames.insert(m_keyFrames.end(), keyFrames.begin(), keyFrames.end());
            m_totalTime = 0.f;
            m_hasCallbacks = false;
            for (auto& keyFrame : m_keyFrames)
            {
                m_hasCallbacks |= keyFrame.callback != nullptr;
                m_totalTime += keyFrame.duration;
            }
            m_loop = loop;
            m_progress = 0;
            m_isPingPonging = false;
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->registerTarget(this);
        }

        void playKeyFramesFromCurrent(const KeyFrames& keyFrames, LoopType loop = LoopType::None)
        {
            if (keyFrames.empty())
            {
                stop();
                return;
            }
            m_keyFrames.clear();
            m_keyFrames.push_back(KeyFrame(m_value, 0.f, Tween::None));
            m_keyFrames.insert(m_keyFrames.end(), keyFrames.begin(), keyFrames.end());
            m_totalTime = 0.f;
            m_hasCallbacks = false;
            for (auto& keyFrame : m_keyFrames)
            {
                m_hasCallbacks |= keyFrame.callback != nullptr;
                m_totalTime += keyFrame.duration;
            }
            m_loop = loop;
            m_progress = 0;
            m_isPingPonging = false;
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->registerTarget(this);
        }

        void stop(bool goToEnd = false)
        {
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->unregisterTarget(this);
            if (goToEnd)
            {
                if (!m_keyFrames.empty())
                {
                    if (m_isPingPonging)
                    {
                        m_value = m_keyFrames.front().value;
                        m_keyFrames.clear();
                    }
                    else
                    {
                        m_value = m_keyFrames.back().value;
                        auto callback = m_keyFrames.back().callback;
                        m_keyFrames.clear();
                        if (callback)
                        {
                            callback();
                        }
                    }
                }
            }
        }

        void pause()
        {
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->unregisterTarget(this);
        }

        void play(LoopType loop = LoopType::None)
        {
            m_loop = loop;
            if (!m_pMyUpdater) m_pMyUpdater = oUpdater;
            m_pMyUpdater->registerTarget(this);
        }

        bool isPlaying() const
        {
            return isUpdateTargetRegistered();
        }

    protected:
        void update() override
        {
            // Move progress
            auto lastProgress = m_progress;
            float progress;
            if (m_isPingPonging)
            {
                m_progress -= ODT;
                progress = m_progress;
                if (m_progress <= 0.f)
                {
                    switch (m_loop)
                    {
                        case LoopType::None:
                            stop(true);
                            return;
                        case LoopType::Loop:
                            m_progress += m_totalTime;
                            break;
                        case LoopType::PingPongOnce:
                            stop(true);
                            return;
                        case LoopType::PingPongLoop:
                            m_isPingPonging = false;
                            m_progress = -m_progress;
                            break;
                    }
                }
            }
            else
            {
                m_progress += ODT;
                progress = m_progress;
                if (m_progress >= m_totalTime)
                {
                    switch (m_loop)
                    {
                        case LoopType::None:
                            stop(true);
                            return;
                        case LoopType::Loop:
                            m_progress -= m_totalTime;
                            break;
                        case LoopType::PingPongOnce:
                        case LoopType::PingPongLoop:
                            m_isPingPonging = true;
                            m_progress = m_totalTime - (m_progress - m_totalTime);
                            break;
                    }
                }
            }

            // Find keyframes
            auto pKeyFrameFrom = m_keyFrames.data();
            auto pKeyFrameTo = pKeyFrameFrom + 1;
            size_t len = m_keyFrames.size() - 1;
            float accumTime = 0.f;
            for (size_t i = 0; i < len; ++i)
            {
                accumTime += pKeyFrameFrom->duration;
                if (m_progress <= accumTime + pKeyFrameTo->duration) break;
                ++pKeyFrameFrom;
                ++pKeyFrameTo;
            }

            // Find percent of keyframes
            float percent = (m_progress - accumTime) / pKeyFrameTo->duration;

            // Apply tween
            percent = OApplyTween(percent, pKeyFrameTo->tween);

            // Lerp
            m_value = OLerp(pKeyFrameFrom->value, pKeyFrameTo->value, percent);

            // Resolve and play callbacks last.
            // Here we create a copy of the callbacks to trigger,
            // because a callback might destroy the owner.
            if (m_hasCallbacks)
            {
                KeyFrameCallbacks callbacks;
                pKeyFrameFrom = m_keyFrames.data();
                accumTime = 0.f;
                len = m_keyFrames.size();
                for (size_t i = 0; i < len; ++i)
                {
                    accumTime += pKeyFrameFrom->duration;
                    if (lastProgress < accumTime && progress >= accumTime)
                    {
                        if (pKeyFrameFrom->callback)
                        {
                            callbacks.push_back(pKeyFrameFrom->callback);
                        }
                    }
                    if (progress < accumTime) break;
                    ++pKeyFrameFrom;
                }
                for (auto& callback : callbacks)
                {
                    callback();
                }
            }
        }

        void setUpdater(const OUpdaterRef& pUpdater)
        {
            m_pMyUpdater = pUpdater;
        }

    private:
        Ttype m_value;
        float m_progress = 0.f;
        float m_totalTime = 1.f;
        LoopType m_loop = LoopType::None;
        KeyFrames m_keyFrames;
        bool m_isPingPonging = false;
        bool m_hasCallbacks = false;
        OUpdaterRef m_pMyUpdater;
    };
}

#define ODontLoop onut::LoopType::None
#define OLoop onut::LoopType::Loop
#define OPingPong onut::LoopType::PingPongOnce
#define OPingPongLoop onut::LoopType::PingPongLoop

#define OAnim onut::Anim;
using OAnimBool = onut::Anim<bool>;
using OAnimInt = onut::Anim<int>;
using OAnimFloat = onut::Anim<float>;
using OAnimDouble = onut::Anim<double>;
using OAnimPoint = onut::Anim<Point>;
using OAnimVector2 = onut::Anim<Vector2>;
using OAnimVector3 = onut::Anim<Vector3>;
using OAnimVector4 = onut::Anim<Vector4>;
using OAnimRect = onut::Anim<Rect>;
using OAnimMatrix = onut::Anim<Matrix>;
using OAnimColor = onut::Anim<Color>;
using OAnimString = onut::Anim<std::string>;

// Anim helpers
#define OSequence(T, ...)                       std::vector<onut::Anim<T>::KeyFrame>(__VA_ARGS__)
#define OAnimWait(val, t)                       {val,t,OTeleport}
#define OAnimAppleStyleBounce(from, to)         {from,0.f,OTeleport},{to,.25f,OEaseOut},{from,.5f,OBounceOut}

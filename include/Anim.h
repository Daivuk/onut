#pragma once
#include <cassert>
#include <chrono>
#include <vector>
#include <functional>
#include <set>

namespace onut
{
    /**
    @enum
    Tween type
    */
    enum class TweenType
    {
        /**
        No Tween. It's going to jump to destination at the end of the anim
        */
        NONE,

        /**
        Linear interpolation
        */
        LINEAR,

        /**
        Animation will start slow and accelerate near the end
        */
        EASE_IN,

        /**
        Animation will slow down at the end
        */
        EASE_OUT,

        /**
        Animation will start slow, then accelerate and slow down at the end
        */
        EASE_BOTH,

        /**
        Bounce effect at the beggining of the animation.
        @see TweenType::BOUNCE_OUT
        */
        BOUNCE_IN,

        /**
        Bounce effect at the end of the animation
        */
        BOUNCE_OUT,

        /**
        Spring at the start of the animation. It will go between starting value at first, then swing fast to the destination
        */
        SPRING_IN,

        /**
        Spring at the end of the animation. It will go past the destination, then swing back to it
        */
        SPRING_OUT
    };

    /**
    @enum
    Loop type
    */
    enum class LoopType
    {
        /**
        No loop. Play once
        */
        NONE,

        /**
        Wrap back to the beggining of the animation then continue playing
        */
        LOOP,

        /**
        At the end of the animation, it will play back in reverse.
        */
        PINGPONG_ONCE,

        /**
        Same as PINGPONG_ONCE, but will start again after and loop indefinitally.
        */
        PINGPONG_LOOP
    };

    /**
    @fn
    Apply a tween effect to a progression
    @param t Time of the animation between 0 and 1
    @param tween Tween to apply to t
    @return The transformed time based on the templated tween.
    @see TweenType
    */
    template<typename Tprecision>
    Tprecision applyTween(const Tprecision t, TweenType tween)
    {
        static const Tprecision SPRING_DIS = static_cast<Tprecision>(.3);
        static const Tprecision INV_SPRING_DIS = static_cast<Tprecision>(.7);

        switch (tween)
        {
            case TweenType::NONE:
                return static_cast<Tprecision>(0);
            case TweenType::LINEAR:
                return t;
            case TweenType::EASE_IN:
                return t * t;
            case TweenType::EASE_OUT:
            {
                auto inv = static_cast<Tprecision>(1) - t;
                return static_cast<Tprecision>(1) - inv * inv;
            }
            case TweenType::EASE_BOTH:
            {
                if (t < static_cast<Tprecision>(.5))
                {
                    return t * t * static_cast<Tprecision>(2);
                }
                else
                {
                    auto clamped = (t - static_cast<Tprecision>(.5)) * static_cast<Tprecision>(2);
                    auto inv = 1 - clamped;
                    clamped = 1 - inv * inv;
                    return clamped * static_cast<Tprecision>(.5) + static_cast<Tprecision>(.5);
                }
                return static_cast<Tprecision>(0);
            }
            case TweenType::BOUNCE_IN:
            {
                auto inv = static_cast<Tprecision>(1) - t;
                Tprecision ret;
                if (inv < (static_cast<Tprecision>(1) / static_cast<Tprecision>(2.75)))
                {
                    ret = (static_cast<Tprecision>(7.5625) * inv * inv);
                }
                else if (inv < (static_cast<Tprecision>(2) / static_cast<Tprecision>(2.75)))
                {
                    auto postFix = inv - (static_cast<Tprecision>(1.5) / static_cast<Tprecision>(2.75));
                    ret = static_cast<Tprecision>(7.5625) * postFix * postFix + static_cast<Tprecision>(.75);
                }
                else if (inv < (static_cast<Tprecision>(2.5) / static_cast<Tprecision>(2.75)))
                {
                    auto postFix = inv - (static_cast<Tprecision>(2.25) / static_cast<Tprecision>(2.75));
                    ret = static_cast<Tprecision>(7.5625) * postFix * postFix + static_cast<Tprecision>(.9375);
                }
                else
                {
                    auto postFix = inv - (static_cast<Tprecision>(2.625) / static_cast<Tprecision>(2.75));
                    ret = static_cast<Tprecision>(7.5625) * postFix * postFix + static_cast<Tprecision>(.984375);
                }
                return 1 - ret;
            }
            case TweenType::BOUNCE_OUT:
                if (t < (static_cast<Tprecision>(1) / static_cast<Tprecision>(2.75)))
                {
                    return (static_cast<Tprecision>(7.5625) * t * t);
                }
                else if (t < (static_cast<Tprecision>(2) / static_cast<Tprecision>(2.75)))
                {
                    auto postFix = t - (static_cast<Tprecision>(1.5) / static_cast<Tprecision>(2.75));
                    return (static_cast<Tprecision>(7.5625) * postFix * postFix + static_cast<Tprecision>(.75));
                }
                else if (t < (static_cast<Tprecision>(2.5) / static_cast<Tprecision>(2.75)))
                {
                    auto postFix = t - (static_cast<Tprecision>(2.25) / static_cast<Tprecision>(2.75));
                    return (static_cast<Tprecision>(7.5625) * postFix * postFix + static_cast<Tprecision>(.9375));
                }
                else
                {
                    auto postFix = t - (static_cast<Tprecision>(2.625) / static_cast<Tprecision>(2.75));
                    return (static_cast<Tprecision>(7.5625) * postFix * postFix + static_cast<Tprecision>(.984375));
                }
            case TweenType::SPRING_IN:
            {
                Tprecision ret;
                if (t < SPRING_DIS)
                {
                    ret = t / SPRING_DIS;
                    ret = static_cast<Tprecision>(1) - (static_cast<Tprecision>(1) - ret) * (static_cast<Tprecision>(1) - ret);
                    ret = ret * -SPRING_DIS;
                }
                else
                {
                    ret = (t - SPRING_DIS) / INV_SPRING_DIS;
                    ret *= ret;
                    ret = ret * (static_cast<Tprecision>(1) + SPRING_DIS) - SPRING_DIS;
                }
                return ret;
            }
            case TweenType::SPRING_OUT:
            {
                auto inv = static_cast<Tprecision>(1) - t;
                if (inv < SPRING_DIS)
                {
                    inv = inv / SPRING_DIS;
                    inv = static_cast<Tprecision>(1) - (static_cast<Tprecision>(1) - inv) * (static_cast<Tprecision>(1) - inv);
                    inv = inv * -SPRING_DIS;
                }
                else
                {
                    inv = (inv - SPRING_DIS) / INV_SPRING_DIS;
                    inv *= inv;
                    inv = inv * (static_cast<Tprecision>(1) + SPRING_DIS) - SPRING_DIS;
                }
                return static_cast<Tprecision>(1) - inv;
            }
        }
        return t;
    }

    /**
    @fn
    Invert a tween enum.
    @param tween The Tween to reverse
    @return The reversed Tween
    @note Here is the reversed values
    Tween                 | Reversed Tween
    --------------------- | ---------------------
    TweenType::NONE       | TweenType::NONE
    TweenType::LINEAR     | TweenType::LINEAR
    TweenType::EASE_IN    | TweenType::EASE_OUT
    TweenType::EASE_OUT   | TweenType::EASE_IN
    TweenType::EASE_BOTH  | TweenType::EASE_BOTH
    TweenType::BOUNCE_IN  | TweenType::BOUNCE_OUT
    TweenType::BOUNCE_OUT | TweenType::BOUNCE_IN
    TweenType::SPRING_IN  | TweenType::SPRING_OUT
    TweenType::SPRING_OUT | TweenType::SPRING_IN
    */
    TweenType invertTween(TweenType tween);

    /**
    @class
    Animation base class
    */
    class IAnim
    {
    public:
        /**
        Constructor
        */
        IAnim() {};

        /**
        Virtual destructor
        */
        virtual ~IAnim() {}

        /**
        Pure virtual. You must implement this. It is called everytime it's AnimManager is updapted,
        if the animation is currently playing.
        */
        virtual void updateAnim() = 0;

        /**
        Returns wheter or not the animation is playing
        @return true if playing
        */
        virtual bool isPlaying() const = 0;

    protected:
        friend class AnimManager;

        virtual void stopButDontUnregister(bool goToEnd) = 0;
    };

    /**
    @class
    Animation manager. Updates a set of animations.
    This object is not thread safe. But can be used by any threads that update it regularly.
    So you should have an instance per threads for animations instantiaced in those threads.
    */
    class AnimManager
    {
    public:
        /**
        Constructor
        */
        AnimManager() {};

        /**
        Destructor
        */
        virtual ~AnimManager() {}

        /**
        Call this once per frame. It updates all anims in the manager
        */
        virtual void update();

        /**
        Stop all currently running anims
        @param goToEnd If true, anims will go to their final destinations. Otherwise they will stop where they left.
        */
        virtual void stopAllAnims(bool goToEnd = false);

        /**
        Register an anim.
        @note This is done for you when animation starts. Don't call this
        */
        virtual void registerAnim(IAnim* pAnim);

        /**
        Unregister an anim.
        @note This is done for you when animation stops/destroy. Don't call this
        */
        virtual void unregisterAnim(IAnim* pAnim);

        /**
        Get a global instance of the animation manager.
        This is the default manager used if not specified by the animations.
        @return Pointer to the global AnimManager.
        */
        static AnimManager* getGlobalManager() { return &s_globalInstance; }

    protected:
        static AnimManager s_globalInstance;

        std::vector<IAnim*>                                                                     m_anims;
        std::set<std::vector<IAnim*>::size_type, std::greater<std::vector<IAnim*>::size_type>>  m_scheduledForRemoval;
        std::vector<IAnim*>::size_type                                                          m_updateIndex = 0;
        bool                                                                                    m_isUpdating = false;
    };

    /**
    @fn
    Default lerp function.
    If you have special objects that need to be lerped different, like a string, you can redefine this.

    Default implementations include:
    - Integers
    - Floating points
    - Any classes that implenent math operators. Like Vector2, Vector3, etc
    - std::string

    Template arguments:
    - Ttype: Type of the values to lerp
    - Tprecision: Precision of the time period. Default float
    - TtypePrecision: Precision of your internal type, if the case. In example, Vector2 store floats. Default float
    */
    template<typename Ttype,
        typename Tprecision = float,
        typename TtypePrecision = float,
        typename std::enable_if<!std::is_integral<Ttype>::value, Ttype>::type* = nullptr,
        typename std::enable_if<!std::is_same<Ttype, std::string>::value && !std::is_same<Ttype, std::wstring>::value>::type* = nullptr>
        Ttype lerp(const Ttype& from, const Ttype& to, Tprecision t)
    {
        auto ret = from + (to - from) * static_cast<TtypePrecision>(t);
        return std::move(ret);
    }
    template<typename Ttype,
        typename Tprecision = float,
        typename TtypePrecision = float,
        typename std::enable_if<std::is_integral<Ttype>::value, Ttype>::type* = nullptr,
        typename std::enable_if<!std::is_same<Ttype, std::string>::value && !std::is_same<Ttype, std::wstring>::value>::type* = nullptr>
        Ttype lerp(const Ttype& from, const Ttype& to, Tprecision t)
    {
        auto ret = static_cast<TtypePrecision>(from)+
            (static_cast<TtypePrecision>(to)-static_cast<TtypePrecision>(from)) *
            static_cast<TtypePrecision>(t);
        ret = round(ret);
        return std::move(static_cast<Ttype>(ret));
    }
    template<typename Tprecision = float, typename TtypePrecision = float>
    bool lerpBool(const bool& from, const bool& to, Tprecision t)
    {
        auto ret = static_cast<TtypePrecision>(from)+
            (static_cast<TtypePrecision>(to)-static_cast<TtypePrecision>(from)) *
            static_cast<TtypePrecision>(t);
        ret = round(ret);
        return static_cast<int>(ret) ? true : false;
    }
    template<typename Ttype,
        typename Tprecision = float,
        typename TtypePrecision = float,
        typename std::enable_if<!std::is_integral<Ttype>::value, Ttype>::type* = nullptr,
        typename std::enable_if<std::is_same<Ttype, std::string>::value || std::is_same<Ttype, std::wstring>::value>::type* = nullptr>
        Ttype lerp(const Ttype& from, const Ttype& to, Tprecision t)
    {
        const auto& fromLen = from.size();
        const auto& toLen = to.size();
        auto newLen = static_cast<Tprecision>(fromLen)+
            (static_cast<Tprecision>(toLen)-static_cast<Tprecision>(fromLen)) * t;
        newLen = round(newLen);
        if (toLen > fromLen)
        {
            auto ret = to.substr(0, static_cast<std::string::size_type>(newLen));
            return std::move(ret);
        }
        else
        {
            auto ret = from.substr(0, static_cast<std::string::size_type>(newLen));
            return std::move(ret);
        }
    }

    /**
    @class
    Anim class. This is what you should use

    Template arguments:
    - Ttype: Type of the internal value (float, int, Vector2, std::string, etc)
    - Tprecision: Precision to be used on the tween. Default is float. If you have a very long animation with very slow tween across it, you might want to use double here
    - Tlerp: Lerp fonction. Default is: from + (to - from) * t
    You can implement your own if you use a special type that needs to be lerped differently, or you want to change it's default behaviour. Your function must respect this signature:
    Ttype yourLerpFunction(const Ttype& from, const Ttype& to, Tprecision t);
    */
    template<typename Ttype,
        typename Tprecision = float,
        Ttype(*Tlerp)(const Ttype&, const Ttype&, Tprecision) = lerp>
    class Anim : public IAnim
    {
    public:
        /**
        User keyframe.
        */
        struct KeyFrame
        {
            KeyFrame(const Ttype& in_goal, Tprecision in_duration, TweenType in_tween = TweenType::NONE, const std::function<void()>& in_callback = nullptr) :
                goal(in_goal),
                duration(in_duration),
                tween(in_tween),
                callback(in_callback)
            {}
            /**
            Destination value
            */
            Ttype                    goal;

            /**
            Duration of the animation
            */
            Tprecision               duration;

            /**
            Tween used to animate from previous position to goal
            */
            TweenType                tween;

            /**
            Callback function when this keyframe is reached
            */
            std::function<void()>    callback;
        };

        /**
        Constructor
        @param rvalue Initial value of the animation. Default is undefined behaviour
        @param pAnimManager Pointer to the animation manager. Defaults to the global manager.
        */
        Anim(AnimManager* pAnimManager = AnimManager::getGlobalManager()) :
            m_pAnimManager(pAnimManager)
        {}
        Anim(const Ttype& rvalue, AnimManager* pAnimManager = AnimManager::getGlobalManager()) :
            m_value(rvalue),
            m_retValue(rvalue),
            m_pAnimManager(pAnimManager)
        {}
        Anim(const Anim& other)
            : m_isPlaying(other.m_isPlaying)
            , m_value(other.m_value)
            , m_retValue(other.m_retValue)
            , m_startTime(other.m_startTime)
            , m_oldTime(other.m_oldTime)
            , m_keyFrames(other.m_keyFrames)
            , m_loop(other.m_loop)
            , m_isPingPonging(other.m_isPingPonging)
            , m_cachedCallback(other.m_cachedCallback)
            , m_pAnimManager(other.m_pAnimManager)
        {
            if (m_isPlaying)
            {
                m_pAnimManager->registerAnim(this);
            }
        }
        Anim& operator=(const Anim& other)
        {
            if (m_isPlaying)
            {
                m_pAnimManager->unregisterAnim(this);
            }
            m_isPlaying = other.m_isPlaying;
            m_value = other.m_value;
            m_retValue = other.m_retValue;
            m_startTime = other.m_startTime;
            m_oldTime = other.m_oldTime;
            m_keyFrames = other.m_keyFrames;
            m_loop = other.m_loop;
            m_isPingPonging = other.m_isPingPonging;
            m_cachedCallback = other.m_cachedCallback;
            m_pAnimManager = other.m_pAnimManager;
            if (m_isPlaying)
            {
                m_pAnimManager->registerAnim(this);
            }
            return *this;
        }

        /**
        Destructor. It will unregister itself from the AnimManager if it's currently playing
        */
        virtual ~Anim()
        {
            if (m_isPlaying)
            {
                stop(false);
            }
        }

        /**
        Get the current value of the animation
        @return Const reference on the current value
        */
        const Ttype& get() const
        {
            return m_retValue;
        }

        operator const Ttype&() const
        {
            return m_retValue;
        }

        /**
        Assign a new value. This changes the start position. So it might affect the currently playing animation.
        @param rvalue Value to set it
        @return Const reference on the newly set value
        */
        const Ttype& operator=(const Ttype& rvalue)
        {
            m_retValue = m_value = rvalue;
            return m_value;
        }

        /**
        Start the animation
        @param from Starting value of the animation
        @param goal Destination value
        @param duration Duration of the animation
        @param tween Tween to use. Default TweenType::LINEAR
        @param loop Looping state. Default LoopType::NONE
        */
        void start(const Ttype& from, const Ttype& goal, Tprecision duration, TweenType tween = TweenType::LINEAR, LoopType loop = LoopType::NONE)
        {
            startKeyframed(from, {{goal, duration, tween}}, loop);
        }

        /**
        Start the animation
        @param goal Destination value. It will start at current value
        @param duration Duration of the animation
        @param tween Tween to use. Default TweenType::LINEAR
        @param loop Looping state. Default LoopType::NONE
        */
        void startFromCurrent(const Ttype& goal, Tprecision duration, TweenType tween = TweenType::LINEAR, LoopType loop = LoopType::NONE)
        {
            startKeyframed(m_value, {{goal, duration, tween}}, loop);
        }

        /**
        Start the animation
        @param keyFrames Sequences of Anim::KeyFrame
        @param loop Looping state. Default LoopType::NONE
        */
        void startFromCurrentKeyframed(const std::vector<KeyFrame>& keyFrames, LoopType loop = LoopType::NONE)
        {
            startKeyframed(m_value, keyFrames, loop);
        }

        /**
        Start the animation
        @param startValue Starting value of the animation
        @param keyFrames Sequences of Anim::KeyFrame
        @param loop Looping state. Default LoopType::NONE
        */
        void startKeyframed(const Ttype& startValue, const std::vector<KeyFrame>& keyFrames, LoopType loop = LoopType::NONE)
        {
            assert(!keyFrames.empty());

            stop(false);

            // Update value to the start point if specified
            m_retValue = m_value = startValue;

            // Clear previous keyframes
            m_keyFrames.clear();

            // Convert to internal keyframes
            m_oldTime = m_startTime = std::chrono::steady_clock::now();
            double timeIn = 0.0;
            for (auto& keyFrame : keyFrames)
            {
                timeIn += static_cast<double>(keyFrame.duration);

                auto timeInPrecise = m_startTime +
                    std::chrono::microseconds(static_cast<long long>(timeIn * 1000000.0));
                m_keyFrames.push_back(InternalKeyFrame{keyFrame.goal, timeInPrecise, keyFrame.tween, keyFrame.callback});
            }

            // Start the animation
            m_isPlaying = true;
            m_loop = loop;

            m_pAnimManager->registerAnim(this);
        }

        /**
        Stop the animation if currently playing.
        @param goToEnd Set the value to the final value in the animation sequences
        */
        void stop(bool goToEnd = false)
        {
            if (m_isPlaying)
            {
                m_pAnimManager->unregisterAnim(this);
                m_isPlaying = false;
                if (goToEnd)
                {
                    if (!m_keyFrames.empty())
                    {
                        m_retValue = m_value = m_keyFrames.back().goal;
                    }
                }
                m_keyFrames.clear();
            }
        }

        /**
        Returns wheter or not the animation is playing
        @return true if playing
        */
        virtual bool isPlaying() const { return m_isPlaying; }

    protected:
        virtual void stopButDontUnregister(bool goToEnd)
        {
            if (m_isPlaying)
            {
                m_isPlaying = false;
                if (goToEnd)
                {
                    if (!m_keyFrames.empty())
                    {
                        m_retValue = m_value = m_keyFrames.back().goal;
                    }
                }
                m_keyFrames.clear();
            }
        }

    public:
        /**
        Update the animation, and store the new value.
        @return true if done playing.
        */
        void updateAnim()
        {
            if (m_isPlaying)
            {
                auto now = std::chrono::steady_clock::now();
                auto& last = m_keyFrames.back();
                if (now >= last.endAt)
                {
                    auto lastCallback = last.callback;
                    switch (m_loop)
                    {
                        case LoopType::NONE:
                        {
                            stop(true);
                            if (lastCallback)
                            {
                                lastCallback();
                            }
                            return;
                        }
                        case LoopType::PINGPONG_ONCE:
                            if (m_isPingPonging)
                            {
                                stop(true);
                                if (lastCallback)
                                {
                                    lastCallback();
                                }
                                return;
                            }
                        case LoopType::PINGPONG_LOOP:
                        {
                            m_isPingPonging = true;
                            auto newStart = last.endAt;

                            // Reverse the time and their position
                            auto& fromTime = m_startTime;
                            auto lastTime = last.endAt;
                            auto from = m_value;
                            auto oldGoal = last.goal;
                            auto callback = m_cachedCallback;
                            for (auto& keyFrame : m_keyFrames)
                            {
                                // Reverse time
                                auto oldEnd = keyFrame.endAt;
                                auto duration = (oldEnd - fromTime);
                                keyFrame.endAt = newStart + (lastTime - oldEnd) + duration;
                                fromTime = oldEnd;

                                // Reverse tween
                                keyFrame.tween = invertTween(keyFrame.tween);

                                // Reverse goals
                                std::swap(keyFrame.goal, from);

                                // Invert callbacks
                                std::swap(keyFrame.callback, callback);
                            }
                            m_cachedCallback = callback;

                            // Reverse the keyframes
                            std::reverse(m_keyFrames.begin(), m_keyFrames.end());

                            m_value = oldGoal;
                            m_startTime = newStart;
                            break;
                        }
                        case LoopType::LOOP:
                        {
                            // Shift all time to continue from the new start
                            auto newStart = last.endAt;
                            for (auto& keyFrame : m_keyFrames)
                            {
                                keyFrame.endAt += newStart - m_startTime;
                            }
                            m_startTime = newStart;
                        }
                    }
                    if (lastCallback)
                    {
                        lastCallback();
                    }
                }
                auto from = m_value;
                auto fromTime = m_startTime;
                std::vector<std::function<void()>> callbacks; // Not the fastest shit in town
                for (auto& keyFrame : m_keyFrames)
                {
                    if (now < keyFrame.endAt)
                    {
                        auto duration = std::chrono::duration<double>(keyFrame.endAt - fromTime);
                        auto timeIn = std::chrono::duration<double>(now - fromTime);
                        auto percent = static_cast<Tprecision>(timeIn.count() / duration.count());
                        percent = applyTween<Tprecision>(percent, keyFrame.tween);

                        m_oldTime = now;
                        // Lerp
                        m_retValue = Tlerp(from, keyFrame.goal, percent);
                        for (auto& callback : callbacks)
                        {
                            callback();
                        }
                        return;
                    }
                    if (m_oldTime <= keyFrame.endAt)
                    {
                        if (keyFrame.callback)
                        {
                            callbacks.push_back(keyFrame.callback);
                        }
                    }
                    from = keyFrame.goal;
                    fromTime = keyFrame.endAt;
                }
                m_oldTime = now;

                // We have to call that at the end, because we might have been destroyed
                for (auto& callback : callbacks)
                {
                    callback();
                }
                m_retValue = m_value;
                return;
            }
            else
            {
                m_retValue = m_value;
                m_pAnimManager->unregisterAnim(this);
            }
        }

    private:
        struct InternalKeyFrame
        {
            InternalKeyFrame(const Ttype& in_goal,
                             const std::chrono::system_clock::time_point& in_endAt,
                             TweenType in_tween, std::function<void()> in_callback) :
                             goal(in_goal),
                             endAt(in_endAt),
                             tween(in_tween),
                             callback(in_callback)
            {}
            Ttype                                    goal;
            std::chrono::system_clock::time_point    endAt;
            TweenType                                tween;
            std::function<void()>                    callback;
        };

        bool                                    m_isPlaying = false;
        Ttype                                   m_value;
        Ttype                                   m_retValue;
        std::chrono::system_clock::time_point   m_startTime;
        std::chrono::system_clock::time_point   m_oldTime;
        std::vector<InternalKeyFrame>           m_keyFrames;
        LoopType                                m_loop;
        bool                                    m_isPingPonging = false;
        std::function<void()>                   m_cachedCallback = nullptr; // Used for pingpong
        AnimManager*                            m_pAnimManager = nullptr;
    };
}

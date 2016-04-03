#pragma once
// Onut
#include <onut/Crypto.h>
#include <onut/Maths.h>
#include <onut/UIEvents.h>

// STL
#include <chrono>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager)
OForwardDeclare(Texture)
OForwardDeclare(UIContext)
OForwardDeclare(UIControl)
OForwardDeclare(UITextBox)

namespace onut
{
    class UIImageComponent;
    class UIScale9Component;
    class UITextComponent;

    class UIContext final : public std::enable_shared_from_this<UIContext>
    {
    public:
        class IUIRenderCallback
        {
        public:
            virtual void render(const std::shared_ptr<void>& pControl, const Rect& rect) const = 0;
        };
        template<typename TobjType, typename TcallbackType>
        class UIRenderCallback : public IUIRenderCallback
        {
        public:
            UIRenderCallback(const TcallbackType& callback) : m_callback(callback) {}

            void render(const std::shared_ptr<void>& pControl, const Rect& rect) const
            {
                m_callback(std::static_pointer_cast<TobjType>(pControl), rect);
            }

        private:
            TcallbackType m_callback;
        };

        class IUITextCaretCallback
        {
        public:
            virtual decltype(std::string().size()) getCaretPos(const std::shared_ptr<void>& pControl, const Vector2& localPos) const = 0;
        };
        template<typename TobjType, typename TcallbackType>
        class UITextCaretCallback : public IUITextCaretCallback
        {
        public:
            UITextCaretCallback(const TcallbackType& callback) : m_callback(callback) {}

            decltype(std::string().size()) getCaretPos(const std::shared_ptr<void>& pControl, const Vector2& localPos) const
            {
                return m_callback(std::static_pointer_cast<TobjType>(pControl), localPos);
            }

        private:
            TcallbackType m_callback;
        };

        using IUIRenderCallbackRef = std::shared_ptr<IUIRenderCallback>;
        using IUITextCaretCallbackRef = std::shared_ptr<IUITextCaretCallback>;
        using GetTextSizeCallback = std::function<Vector2(const OUITextBoxRef& pTextBox, const std::string &text)>;
        using DrawTextInsertCallback = std::function<void(const Rect& rect)>;

        static OUIContextRef create(const Vector2& screenSize);

        const Vector2& getScreenSize() const { return m_screenSize; }

        void resize(const Vector2& screenSize);
        void write(char c);
        void keyDown(uintptr_t key);
        void focus(const OUIControlRef& pFocus);
        void clearState();
        void drawRect(const OUIControlRef&, const Rect&, const Color&);
        void drawTexturedRect(const OUIControlRef& pControl, const Rect& rect, const UIImageComponent& imageComponent);
        void drawScale9Rect(const OUIControlRef& pControl, const Rect& rect, const UIScale9Component& scale9Component);
        void drawText(const OUIControlRef& pControl, const Rect& rect, const UITextComponent& textComponent);
        void pushClip(const Rect& rect);
        void popClip();

        void renderScale9Component(const OUIControlRef& pControl, const Rect& rect, const UIScale9Component& scale9Component);
        void renderTextComponent(const OUIControlRef& pControl, const Rect& rect, const UITextComponent& textComponent);

        const OUIControlRef& getHoverControl() const { return m_pHoverControl; }
        const OUIControlRef& getDownControl(int mouseButton = 1) const { return m_pDownControls[mouseButton - 1]; }
        const OUIControlRef& getFocusControl() const { return m_pFocus; }

        GetTextSizeCallback getTextSize = nullptr;
        DrawTextInsertCallback drawInsert = nullptr;
        std::chrono::steady_clock::duration doubleClickTime = std::chrono::milliseconds(500);
        bool useNavigation = false;
        OContentManagerWeak pContentManager;

        // Styles related function
        template<typename TobjType, typename TcallbackType>
        void addStyle(const char* szStyle, const TcallbackType& renderCallback)
        {
            auto styleId = OHash(szStyle);
            auto& m = m_callbacks[std::type_index(typeid(TobjType))];
            m[styleId] = std::make_shared<UIRenderCallback<TobjType, TcallbackType>>(renderCallback);
        }

        template<typename TobjType, typename TcallbackType>
        void addTextCaretSolver(const char* szStyle, const TcallbackType& callback)
        {
            auto styleId = OHash(szStyle);
            auto& m = m_textCaretSolvers[std::type_index(typeid(TobjType))];
            m[styleId] = std::make_shared<UITextCaretCallback<TobjType, TcallbackType>>(callback);
        }

        template<typename TobjType>
        const IUIRenderCallbackRef getStyle(unsigned int styleId) const
        {
            auto& mIt = m_callbacks.find(std::type_index(typeid(TobjType)));
            if (mIt == m_callbacks.end())
            {
                return nullptr;
            }
            auto& it = mIt->second.find(styleId);
            if (it != mIt->second.end())
            {
                return it->second;
            }
            else if (styleId != 0)
            {
                it = mIt->second.find(0);
                if (it != mIt->second.end())
                {
                    return it->second;
                }
            }
            return nullptr;
        }

        template<typename TobjType>
        const IUITextCaretCallbackRef getTextCaretSolver(unsigned int styleId) const
        {
            auto& mIt = m_textCaretSolvers.find(std::type_index(typeid(TobjType)));
            if (mIt == m_textCaretSolvers.end())
            {
                return nullptr;
            }
            auto& it = mIt->second.find(styleId);
            if (it != mIt->second.end())
            {
                return it->second;
            }
            else if (styleId != 0)
            {
                it = mIt->second.find(0);
                if (it != mIt->second.end())
                {
                    return it->second;
                }
            }
            return nullptr;
        }

    private:
        UIContext(const Vector2& screenSize);

        friend UIControl;

        using Writes = std::vector<char>;
        using Keys = std::vector<uintptr_t>;
        using RenderStyleMap = std::unordered_map<unsigned int, IUIRenderCallbackRef>;
        using TextCaretStyleMap = std::unordered_map<unsigned int, IUITextCaretCallbackRef>;
        using RenderStyleMapByType = std::unordered_map<std::type_index, RenderStyleMap>;
        using TextCaretStyleMapByType = std::unordered_map<std::type_index, TextCaretStyleMap>;
        using Clips = std::vector<Rect>;

        void resolve();
        void dispatchEvents();
        void reset();
        OTextureRef UIContext::getTextureForState(const OUIControlRef& pControl, const std::string &filename);

        RenderStyleMapByType m_callbacks;
        TextCaretStyleMapByType m_textCaretSolvers;

        UIMouseEvent m_mouseEvents[3];
        UIMouseEvent m_lastMouseEvents[3];
        Vector2 m_hoverLocalMousePos;
        Vector2 m_screenSize;

        OUIControlRef m_pHoverControl = nullptr;
        OUIControlRef m_pDownControls[3];
        OUIControlRef m_pFocus = nullptr;

        OUIControlRef m_pLastHoverControl = nullptr;
        OUIControlRef m_pLastDownControls[3];
        OUIControlRef m_pLastFocus = nullptr;

        Writes m_writes;
        Keys m_keyDowns;
        Clips m_clips;

        std::chrono::steady_clock::time_point m_clickTimes[3];
        Vector2 m_clicksPos[3];
    };
};

extern OUIContextRef oUIContext;

// Onut
#include <onut/ContentManager.h>
#include <onut/Font.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/UIComponents.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UITextBox.h>

// Third parties
#if defined(WIN32)
#include <Windows.h>
#endif

OUIContextRef oUIContext;

namespace onut
{
    OUIContextRef UIContext::create(const Vector2& screenSize)
    {
        return std::shared_ptr<UIContext>(new UIContext(screenSize));
    }

    UIContext::UIContext(const Vector2& screenSize) :
        m_screenSize(screenSize)
    {
#if defined(WIN32)
        doubleClickTime = std::chrono::milliseconds(GetDoubleClickTime());
#endif
        pContentManager = oContentManager;
        m_pDownControls[0] = nullptr;
        m_pDownControls[1] = nullptr;
        m_pDownControls[2] = nullptr;
        m_pLastDownControls[0] = nullptr;
        m_pLastDownControls[1] = nullptr;
        m_pLastDownControls[2] = nullptr;
    }

    void UIContext::resize(const Vector2& screenSize)
    {
        m_screenSize = screenSize;
    }

    void UIContext::pushClip(const Rect& rect)
    {
        m_clips.push_back(rect);
        oSpriteBatch->end();
        oRenderer->renderStates.scissorEnabled = true;
        oRenderer->renderStates.scissor = iRect{
            static_cast<int>(rect.x),
            static_cast<int>(rect.y),
            static_cast<int>(rect.x + rect.z),
            static_cast<int>(rect.y + rect.w)
        };
        oSpriteBatch->begin();
    }

    void UIContext::popClip()
    {
        m_clips.pop_back();
        oSpriteBatch->end();
        if (m_clips.empty())
        {
            oRenderer->renderStates.scissorEnabled = false;
        }
        else
        {
            auto& rect = m_clips.back();
            oRenderer->renderStates.scissorEnabled = true;
            oRenderer->renderStates.scissor = iRect{
                static_cast<int>(rect.x),
                static_cast<int>(rect.y),
                static_cast<int>(rect.x + rect.z),
                static_cast<int>(rect.y + rect.w)
            };
        }
        oSpriteBatch->begin();
    }

    void UIContext::resolve()
    {
        // If it's the first mouse down since last frame, and there is an hover
        // control. Put him into down state
        for (int i = 0; i < 3; ++i)
        {
            if (!m_lastMouseEvents[i].isMouseDown &&
                m_mouseEvents[i].isMouseDown &&
                m_pHoverControl)
            {
                m_pDownControls[i] = m_pHoverControl;
            }

            if (!m_mouseEvents[i].isMouseDown)
            {
                m_pDownControls[i] = nullptr;
            }

            // Don't allow hovering of other controls if a control is in DOWN state
            if (m_pDownControls[i] &&
                m_pHoverControl != m_pDownControls[i])
            {
                m_pHoverControl = nullptr;
            }
        }

        if (m_pDownControls[0])
        {
            m_pFocus = m_pDownControls[0];
        }
    }

    void UIContext::dispatchEvents()
    {
        // Do writes
        if (m_pFocus)
        {
            for (auto c : m_writes)
            {
                m_pFocus->onWriteInternal(c, OThis);
            }
        }
        m_writes.clear();

        // Do key events
        if (m_pFocus)
        {
            UIKeyEvent evt;
            evt.pContext = shared_from_this();
            for (auto key : m_keyDowns)
            {
                evt.key = key;
                m_pFocus->onKeyDownInternal(evt);
                if (m_pFocus->onKeyDown)
                {
                    m_pFocus->onKeyDown(m_pFocus, evt);
                }
            }
        }
        m_keyDowns.clear();

        if (m_pHoverControl != m_pLastHoverControl)
        {
            if (m_pLastHoverControl)
            {
                if (m_pLastHoverControl->onMouseLeave)
                {
                    m_pLastHoverControl->onMouseLeave(m_pLastHoverControl, m_mouseEvents[0]);
                }
            }
            if (m_pHoverControl)
            {
                if (m_pHoverControl->onMouseEnter)
                {
                    m_pHoverControl->onMouseEnter(m_pHoverControl, m_mouseEvents[0]);
                }
            }
        }
        if (m_pHoverControl)
        {
            if (!useNavigation)
            {
                if (m_mouseEvents[0].scroll != 0)
                {
                    m_pHoverControl->onMouseScrollInternal(m_mouseEvents[0]);
                    if (m_pHoverControl->onMouseScroll)
                    {
                        m_pHoverControl->onMouseScroll(m_pHoverControl, m_mouseEvents[0]);
                    }
                }
            }
        }
        for (int i = 0; i < 3; ++i)
        {
            if (m_pDownControls[i] != m_pLastDownControls[i])
            {
                if (m_pLastDownControls[i])
                {
                    if (!i) m_pLastDownControls[i]->onMouseUpInternal(m_mouseEvents[i]);
                    if (i == 0)
                    {
                        if (m_pLastDownControls[i]->onMouseUp)
                        {
                            m_pLastDownControls[i]->onMouseUp(m_pLastDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 1)
                    {
                        if (m_pLastDownControls[i]->onRightMouseUp)
                        {
                            m_pLastDownControls[i]->onRightMouseUp(m_pLastDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 2)
                    {
                        if (m_pLastDownControls[i]->onMiddleMouseUp)
                        {
                            m_pLastDownControls[i]->onMiddleMouseUp(m_pLastDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    if (m_pHoverControl == m_pLastDownControls[i] &&
                        !m_mouseEvents[i].isMouseDown)
                    {
                        if (!i) m_pLastDownControls[i]->onClickInternal(m_mouseEvents[i]);
                        if (i == 0)
                        {
                            if (m_pLastDownControls[i]->onClick)
                            {
                                m_pLastDownControls[i]->onClick(m_pLastDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 1)
                        {
                            if (m_pLastDownControls[i]->onRightClick)
                            {
                                m_pLastDownControls[i]->onRightClick(m_pLastDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 2)
                        {
                            if (m_pLastDownControls[i]->onMiddleClick)
                            {
                                m_pLastDownControls[i]->onMiddleClick(m_pLastDownControls[i], m_mouseEvents[i]);
                            }
                        }
                    }
                }
                if (m_pDownControls[i])
                {
                    if (!i) m_pDownControls[i]->onMouseDownInternal(m_mouseEvents[i]);
                    if (i == 0)
                    {
                        if (m_pDownControls[i]->onMouseDown)
                        {
                            m_pDownControls[i]->onMouseDown(m_pDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 1)
                    {
                        if (m_pDownControls[i]->onRightMouseDown)
                        {
                            m_pDownControls[i]->onRightMouseDown(m_pDownControls[i], m_mouseEvents[i]);
                        }
                    }
                    else if (i == 2)
                    {
                        if (m_pDownControls[i]->onMiddleMouseDown)
                        {
                            m_pDownControls[i]->onMiddleMouseDown(m_pDownControls[i], m_mouseEvents[i]);
                        }
                    }

                    // Check for double click events
                    auto now = std::chrono::steady_clock::now();
                    if (now - m_clickTimes[i] <= doubleClickTime && !useNavigation &&
                        m_clicksPos[i].x > m_mouseEvents[i].mousePos.x - 3 &&
                        m_clicksPos[i].y > m_mouseEvents[i].mousePos.y - 3 &&
                        m_clicksPos[i].x < m_mouseEvents[i].mousePos.x + 3 &&
                        m_clicksPos[i].y < m_mouseEvents[i].mousePos.y + 3)
                    {
                        if (i == 0)
                        {
                            if (m_pDownControls[i]->onDoubleClick)
                            {
                                m_pDownControls[i]->onDoubleClick(m_pDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 1)
                        {
                            if (m_pDownControls[i]->onRightDoubleClick)
                            {
                                m_pDownControls[i]->onRightDoubleClick(m_pDownControls[i], m_mouseEvents[i]);
                            }
                        }
                        else if (i == 2)
                        {
                            if (m_pDownControls[i]->onMiddleDoubleClick)
                            {
                                m_pDownControls[i]->onMiddleDoubleClick(m_pDownControls[i], m_mouseEvents[i]);
                            }
                        }
                    }
                    m_clickTimes[i] = now;
                    m_clicksPos[i] = m_mouseEvents[i].mousePos;
                }
            }
        }

        // Focus gain/lost
        if (m_pFocus != m_pLastFocus)
        {
            if (m_pLastFocus)
            {
                UIFocusEvent evt;
                evt.pContext = shared_from_this();
                m_pLastFocus->onLoseFocusInternal(evt);
                if (m_pLastFocus->onLoseFocus)
                {
                    m_pLastFocus->onLoseFocus(m_pLastFocus);
                }
            }
            if (m_pFocus)
            {
                UIFocusEvent evt;
                evt.pContext = shared_from_this();
                m_pFocus->onGainFocusInternal(evt);
                if (m_pFocus->onGainFocus)
                {
                    m_pFocus->onGainFocus(m_pFocus);
                }
            }
        }

        if (!useNavigation)
        {
            if (m_lastMouseEvents[0].mousePos.x != m_mouseEvents[0].mousePos.x ||
                m_lastMouseEvents[0].mousePos.y != m_mouseEvents[0].mousePos.y)
            {
                bool bAte = false;
                for (int i = 0; i < 3; ++i)
                {
                    if (m_pDownControls[i])
                    {
                        auto worldRect = m_pDownControls[i]->getWorldRect(OThis);
                        m_mouseEvents[i].localMousePos.x = m_mouseEvents[i].mousePos.x - worldRect.x;
                        m_mouseEvents[i].localMousePos.y = m_mouseEvents[i].mousePos.y - worldRect.y;
                        m_pDownControls[i]->onMouseMoveInternal(m_mouseEvents[i]);
                        if (m_pDownControls[i]->onMouseMove)
                        {
                            m_pDownControls[i]->onMouseMove(m_pDownControls[i], m_mouseEvents[i]);
                        }
                        bAte = true;
                        break;
                    }
                }
                if (!bAte && m_pHoverControl)
                {
                    m_pHoverControl->onMouseMoveInternal(m_mouseEvents[0]);
                    if (m_pHoverControl->onMouseMove)
                    {
                        m_pHoverControl->onMouseMove(m_pHoverControl, m_mouseEvents[0]);
                    }
                }
            }
        }
    }

    void UIContext::reset()
    {
        for (int i = 0; i < 3; ++i) m_lastMouseEvents[i] = m_mouseEvents[i];

        m_pLastHoverControl = m_pHoverControl;
        for (int i = 0; i < 3; ++i) m_pLastDownControls[i] = m_pDownControls[i];
        m_pLastFocus = m_pFocus;

        m_clips.clear();
    }

    void UIContext::write(char c)
    {
        m_writes.push_back(c);
    }

    void UIContext::keyDown(uintptr_t key)
    {
        m_keyDowns.push_back(key);
    }

    void UIContext::focus(const OUIControlRef& pFocus)
    {
        m_pFocus = pFocus;
    }

    void UIContext::clearState()
    {
        m_pLastHoverControl = nullptr;
        for (int i = 0; i < 3; ++i) m_pLastDownControls[i] = nullptr;
        m_pLastFocus = nullptr;

        m_pHoverControl = nullptr;
        for (int i = 0; i < 3; ++i) m_pDownControls[i] = nullptr;
        m_pFocus = nullptr;

        m_clips.clear();
        m_writes.clear();
        m_keyDowns.clear();

        for (int i = 0; i < 3; ++i)
        {
            m_clicksPos[i].x = -100;
            m_clicksPos[i].y = -100;
        }
    }

    void UIContext::renderScale9Component(const OUIControlRef& pControl, const Rect& rect, const UIScale9Component& scale9Component)
    {
        if (scale9Component.image.filename.empty())
        {
            drawRect(pControl, rect, scale9Component.image.color);
        }
        else if (scale9Component.isScaled9)
        {
            drawScale9Rect(pControl, rect, scale9Component);
        }
        else
        {
            drawTexturedRect(pControl, rect, scale9Component.image);
        }
    }

    void UIContext::renderTextComponent(const OUIControlRef& pControl, const Rect& rect, const UITextComponent& textComponent)
    {
        auto textRect = rect;
        textRect.x += textComponent.font.padding.x;
        textRect.y += textComponent.font.padding.y;
        textRect.z -= textComponent.font.padding.x + textComponent.font.padding.z;
        textRect.w -= textComponent.font.padding.y + textComponent.font.padding.w;
        drawText(pControl, textRect, textComponent);
    }

    OTextureRef UIContext::getTextureForState(const OUIControlRef& pControl, const std::string &filename)
    {
        static std::string stateFilename;
        stateFilename = filename;
        OTextureRef pTexture;
        auto pContentManagerRef = pContentManager.lock();
        switch (pControl->getState(OThis))
        {
            case UIControl::State::Normal:
                pTexture = pContentManagerRef->getResourceAs<OTexture>(filename.c_str());
                break;
            case UIControl::State::Disabled:
                stateFilename.insert(filename.size() - 4, "_disabled");
                pTexture = pContentManagerRef->getResourceAs<OTexture>(stateFilename.c_str());
                if (!pTexture) pTexture = pContentManagerRef->getResourceAs<OTexture>(filename.c_str());
                break;
            case UIControl::State::Hover:
                stateFilename.insert(filename.size() - 4, "_hover");
                pTexture = pContentManagerRef->getResourceAs<OTexture>(stateFilename.c_str());
                if (!pTexture) pTexture = pContentManagerRef->getResourceAs<OTexture>(filename.c_str());
                break;
            case UIControl::State::Down:
                stateFilename.insert(filename.size() - 4, "_down");
                pTexture = pContentManagerRef->getResourceAs<OTexture>(stateFilename.c_str());
                if (!pTexture) pTexture = pContentManagerRef->getResourceAs<OTexture>(filename.c_str());
                break;
        }
        return pTexture;
    };

    void UIContext::drawRect(const OUIControlRef& pControl, const Rect& rect, const Color& color)
    {
        oSpriteBatch->drawRect(nullptr, rect, color);
    };

    void UIContext::drawTexturedRect(const OUIControlRef& pControl, const Rect& rect, const UIImageComponent& imageComponent)
    {
        if (imageComponent.pTexture)
        {
            oSpriteBatch->drawRect(imageComponent.pTexture, rect, imageComponent.color);
        }
        else
        {
            oSpriteBatch->drawRect(getTextureForState(pControl, imageComponent.filename),
                                   rect,
                                   imageComponent.color);
        }
    };

    void UIContext::drawScale9Rect(const OUIControlRef& pControl, const Rect& rect, const UIScale9Component& scale9Component)
    {
        if (scale9Component.isRepeat)
        {
            oSpriteBatch->drawRectScaled9RepeatCenters(getTextureForState(pControl, scale9Component.image.filename),
                                                       (rect),
                                                       (scale9Component.padding),
                                                       (scale9Component.image.color));
        }
        else
        {
            oSpriteBatch->drawRectScaled9(getTextureForState(pControl, scale9Component.image.filename),
                                          (rect),
                                          (scale9Component.padding),
                                          (scale9Component.image.color));
        }
    };

    void UIContext::drawText(const OUIControlRef& pControl, const Rect& rect, const UITextComponent& textComponent)
    {
        if (textComponent.text.empty()) return;
        auto align = (textComponent.font.align);
        auto oRect = (rect);
        auto pFont = pContentManager.lock()->getResourceAs<OFont>(textComponent.font.typeFace.c_str());
        auto oColor = (textComponent.font.color);
        if (pControl->getState(OThis) == UIControl::State::Disabled)
        {
            oColor = {.4f, .4f, .4f, 1};
        }
        oColor.Premultiply();

        if (pFont)
        {
            if (pControl->getStyleName() == "password")
            {
                std::string pwd;
                pwd.resize(textComponent.text.size(), '*');
                if (pControl->hasFocus(OThis) && ODynamicCast<UITextBox>(pControl)->isCursorVisible())
                {
                    pwd.back() = '_';
                }
                pFont->draw(pwd, ORectAlign<>(oRect, align), Vector2(align), oColor);
            }
            else
            {
                pFont->draw(textComponent.text, ORectAlign<>(oRect, align), Vector2(align), oColor);
            }
        }
    };
};

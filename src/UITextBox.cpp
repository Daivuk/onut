// Onut
#include <onut/UIContext.h>
#include <onut/UITextBox.h>

// Private
#include "UIJson.h"

// STL
#include <iomanip>
#include <sstream>

namespace onut
{
    OUITextBoxRef UITextBox::create()
    {
        return std::shared_ptr<UITextBox>(new UITextBox());
    }

    void UITextBox::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UITextBox*>(&other);
        if (pOther)
        {
            textComponent = pOther->textComponent;
            scale9Component = pOther->scale9Component;
            m_isNumerical = pOther->m_isNumerical;
            m_decimalPrecision = pOther->m_decimalPrecision;
            min = pOther->min;
            max = pOther->max;
            step = pOther->step;
        }
        UIControl::operator=(other);
    }

    bool UITextBox::isCursorVisible() const
    {
        auto now = std::chrono::steady_clock::now();
        auto diff = now - m_cursorTime;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        return (ms.count() % 1000) < 500;
    }

    void UITextBox::numerifyText()
    {
        if (m_isNumerical)
        {
            std::stringstream ss(textComponent.text);
            float value;
            if (!(ss >> value))
            {
                value = 0.f;
            }
            if (value < min) value = min;
            if (value > max) value = max;
            std::stringstream ssOut;
            ssOut << std::fixed << std::setprecision(static_cast<std::streamsize>(m_decimalPrecision)) << value;
            textComponent.text = ssOut.str();
        }
    }

    float UITextBox::getFloat() const
    {
        if (m_isNumerical)
        {
            std::stringstream ss(textComponent.text);
            float value;
            if (!(ss >> value))
            {
                value = 0.f;
            }
            return value;
        }
        return 0.f;
    }

    int UITextBox::getInt() const
    {
        if (m_isNumerical)
        {
            std::stringstream ss(textComponent.text);
            int value;
            if (!(ss >> value))
            {
                value = 0;
            }
            return value;
        }
        return 0;
    }

    void UITextBox::setFloat(float f)
    {
        textComponent.text = std::to_string(f);
        numerifyText();
    }

    void UITextBox::setInt(int i)
    {
        textComponent.text = std::to_string(i);
        numerifyText();
    }

    void UITextBox::setIsNumerical(bool isNumerical)
    {
        m_isNumerical = isNumerical;
        numerifyText();
    }

    void UITextBox::setIsDecimalPrecision(int decimalPrecision)
    {
        m_decimalPrecision = decimalPrecision;
        numerifyText();
    }


    void UITextBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
        scale9Component = getJsonScale9Component(jsonNode);
        m_isNumerical = getJsonBool(jsonNode["numerical"]);
        m_decimalPrecision = getJsonInt(jsonNode["precision"]);
        numerifyText();
    }

    void UITextBox::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
        setJsonScale9Component(jsonNode, scale9Component, allocator);
        setJsonBool(jsonNode, "numerical", m_isNumerical, allocator);
        setJsonInt(jsonNode, "precision", m_decimalPrecision, allocator);
    }

    void UITextBox::renderControl(const OUIContextRef& context, const Rect& rect)
    {
        const auto& callback = context->getStyle<UITextBox>(getStyle());
        if (callback)
        {
            callback->render(OThis, rect);
        }
        else
        {
            context->renderScale9Component(OThis, rect, scale9Component);

            if (isCursorVisible() && hasFocus(context))
            {
                auto prevText = textComponent.text;
                textComponent.text = prevText + "_";
                context->renderTextComponent(OThis, rect, textComponent);
                textComponent.text = prevText;
            }
            else
            {
                context->renderTextComponent(OThis, rect, textComponent);
            }
        }
    }

    void UITextBox::onGainFocusInternal(const UIFocusEvent& evt)
    {
        // Select all on focus
        selectAll();
        m_cursorTime = std::chrono::steady_clock::now();
        m_isTextChanged = false;
    }

    void UITextBox::onLoseFocusInternal(const UIFocusEvent& evt)
    {
        // Deselect all
        m_selectedTextRegion[0] = 0;
        m_selectedTextRegion[1] = 0;

        // Accept
        if (m_isTextChanged)
        {
            m_isTextChanged = false;
            numerifyText();
            if (onTextChanged)
            {
                UITextBoxEvent evt2;
                evt2.pContext = evt.pContext;
                onTextChanged(ODynamicCast<UITextBox>(OThis), evt2);
            }
        }
    }

    void UITextBox::onMouseDownInternal(const UIMouseEvent& evt)
    {
        m_isSelecting = true;
        if (getIsNumerical())
        {
            m_mousePosOnDown = evt.mousePos.y;
            m_valueOnDown = getFloat();
        }
        else
        {
            const auto& callback = evt.pContext->getTextCaretSolver<UITextBox>(getStyle());
            if (callback)
            {
                auto caretPos = callback->getCaretPos(OThis, evt.localMousePos);
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = caretPos;
            }
        }
    }

    void UITextBox::onMouseMoveInternal(const UIMouseEvent& evt)
    {
        if (!m_isSelecting) return; // Don't care
        if (getIsNumerical())
        {
            if (!m_isSpinning)
            {
                auto mouseDiff = evt.mousePos.y - m_mousePosOnDown;
                if (std::abs(mouseDiff) >= 3.f)
                {
                    m_isSpinning = true;
                    m_mousePosOnDown = evt.mousePos.y;
                    if (onNumberSpinStart)
                    {
                        UITextBoxEvent evt2;
                        evt2.pContext = evt.pContext;
                        onNumberSpinStart(ODynamicCast<UITextBox>(OThis), evt2);
                    }
                }
            }
            if (m_isSpinning)
            {
                auto mouseDiff = evt.mousePos.y - m_mousePosOnDown;
                auto valueDiff = mouseDiff * -step;
                setFloat(m_valueOnDown + valueDiff);
                selectAll();
                if (onTextChanged)
                {
                    UITextBoxEvent evt2;
                    evt2.pContext = evt.pContext;
                    onTextChanged(ODynamicCast<UITextBox>(OThis), evt2);
                }
            }
        }
        else
        {
            const auto& callback = evt.pContext->getTextCaretSolver<UITextBox>(getStyle());
            if (callback)
            {
                auto caretPos = callback->getCaretPos(OThis, evt.localMousePos);
                m_cursorPos = caretPos;
                m_selectedTextRegion[0] = std::min<>(m_cursorPos, m_selectedTextRegion[0]);
                m_selectedTextRegion[1] = std::max<>(m_cursorPos, m_selectedTextRegion[1]);
            }
        }
    }

    void UITextBox::onMouseUpInternal(const UIMouseEvent& evt)
    {
        m_isSelecting = false;
        if (m_isSpinning)
        {
            m_isSpinning = false;
            if (onNumberSpinEnd)
            {
                UITextBoxEvent evt2;
                evt2.pContext = evt.pContext;
                onNumberSpinEnd(ODynamicCast<UITextBox>(OThis), evt2);
                evt.pContext->focus(nullptr);
            }
        }
    }

    void UITextBox::selectAll()
    {
        m_selectedTextRegion[0] = 0;
        m_selectedTextRegion[1] = textComponent.text.size();
        m_cursorPos = m_selectedTextRegion[1];
    }

    void UITextBox::onWriteInternal(char c, const OUIContextRef& context)
    {
        m_cursorTime = std::chrono::steady_clock::now();
        m_isTextChanged = true;
        const auto& callback = context->getStyle<UITextBox>(getStyle());
        if (!callback)
        {
            m_selectedTextRegion[1] = m_selectedTextRegion[0] = m_cursorPos;
        }
        if (c == '\b') // Backspace
        {
            if (m_selectedTextRegion[1] - m_selectedTextRegion[0])
            {
                textComponent.text = textComponent.text.substr(0, m_selectedTextRegion[0]) + textComponent.text.substr(m_selectedTextRegion[1]);
                m_cursorPos = m_selectedTextRegion[1] = m_selectedTextRegion[0];
            }
            else if (m_cursorPos)
            {
                textComponent.text = textComponent.text.substr(0, m_cursorPos - 1) + textComponent.text.substr(m_cursorPos);
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos - 1;
            }
        }
        else if (c == '\r') // Return
        {
            context->focus(nullptr);
        }
        else if (c == '\x1b') // Escape
        {
            context->focus(nullptr);
        }
        else if (c == '\t')
        {
            // We go to the next textfield
            auto pParent = getParent();
            OUIControlRef pNext = nullptr;
            if (pParent)
            {
                float closest[2] = {100000.f, 100000.f};
                auto myRect = getWorldRect(context);
                for (auto pChild : pParent->getChildren())
                {
                    if (pChild->getType() == Type::TextBox &&
                        pChild.get() != this)
                    {
                        auto childRect = pChild->getWorldRect(context);
                        float dists[2] = {
                            childRect.x - myRect.x,
                            childRect.y - myRect.y};
                        if (dists[0] <= -5.f || dists[1] <= -5.f) continue;
                        if (dists[1] < closest[1])
                        {
                            closest[0] = dists[0];
                            closest[1] = dists[1];
                            pNext = pChild;
                            continue;
                        }
                        if (dists[0] < closest[0] &&
                            dists[1] < closest[1] + 5.f)
                        {
                            closest[0] = dists[0];
                            closest[1] = dists[1];
                            pNext = pChild;
                            continue;
                        }
                    }
                }
            }
            if (pNext)
            {
                context->focus(pNext);
            }
            else
            {
                context->focus(nullptr);
            }
        }
        else // Normal character
        {
            std::stringstream ss;
            ss << textComponent.text.substr(0, m_selectedTextRegion[0]);
            ss << c;
            ss << textComponent.text.substr(m_selectedTextRegion[1]);
            textComponent.text = ss.str();
            m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_selectedTextRegion[0] + 1;
        }
    }

    void UITextBox::onKeyDownInternal(const UIKeyEvent& evt)
    {
        m_cursorTime = std::chrono::steady_clock::now();
        const auto& callback = evt.pContext->getStyle<UITextBox>(getStyle());
        switch (evt.key)
        {
            case KEY_END:
                if (callback)
                {
                    m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = textComponent.text.size();
                }
                break;
            case KEY_HOME:
                if (callback)
                {
                    m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = 0;
                }
                break;
            case KEY_LEFT:
            case KEY_UP:
                if (callback)
                {
                    if (m_cursorPos > 0)
                    {
                        --m_cursorPos;
                        m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos;
                    }
                }
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if (callback)
                {
                    if (m_cursorPos < textComponent.text.size())
                    {
                        ++m_cursorPos;
                        m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos;
                    }
                }
                break;
            case KEY_DELETE:
                m_isTextChanged = true;
                if (!callback)
                {
                    selectAll();
                }
                if (m_selectedTextRegion[0] != m_selectedTextRegion[1])
                {
                    textComponent.text = textComponent.text.substr(0, m_selectedTextRegion[0]) + textComponent.text.substr(m_selectedTextRegion[1]);
                    m_cursorPos = m_selectedTextRegion[1] = m_selectedTextRegion[0];
                }
                else if (m_cursorPos < textComponent.text.size())
                {
                    textComponent.text = textComponent.text.substr(0, m_cursorPos) + textComponent.text.substr(m_cursorPos + 1);
                }
                break;
        }
    }
};

// Onut
#include <onut/UIButton.h>
#include <onut/UIContext.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUIButtonRef UIButton::create()
    {
        return std::shared_ptr<UIButton>(new UIButton());
    }

    UIButton::UIButton()
    {
        textComponent.font.align = onut::Align::Center;
    }

    void UIButton::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UIButton*>(&other);
        if (pOther)
        {
            textComponent = pOther->textComponent;
            scale9Component = pOther->scale9Component;
        }
        UIControl::operator=(other);
    }

    void UIButton::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
        scale9Component = getJsonScale9Component(jsonNode);
    }

    void UIButton::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
        setJsonScale9Component(jsonNode, scale9Component, allocator);
    }

    void UIButton::renderControl(const OUIContextRef& context, const Rect& rect)
    {
        const auto& callback = context->getStyle<UIButton>(getStyle());
        if (callback)
        {
            callback->render(OThis, rect);
        }
        else
        {
            context->renderScale9Component(OThis, rect, scale9Component);
            context->renderTextComponent(OThis, rect, textComponent);
        }
    }
};

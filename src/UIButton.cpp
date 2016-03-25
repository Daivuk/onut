#include "onut/UIButton.h"
#include "onut/UIContext.h"

#include "UIJson.h"

namespace onut
{
    OUIButtonRef UIButton::create()
    {
        return OMake<UIButton>();
    }

    UIButton::UIButton()
    {
        textComponent.font.align = onut::Align::Center;
    }

    UIButton::UIButton(const UIButton& other) :
        UIControl(other)
    {
        textComponent = other.textComponent;
        scale9Component = other.scale9Component;
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

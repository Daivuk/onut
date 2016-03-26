// Onut
#include <onut/UIContext.h>
#include <onut/UILabel.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUILabelRef UILabel::create()
    {
        return OMake<UILabel>();
    }

    UILabel::UILabel()
    {
        textComponent.font.align = onut::Align::Left;
    }

    UILabel::UILabel(const UILabel& other) :
        UIControl(other)
    {
        textComponent = other.textComponent;
    }

    void UILabel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
    }

    void UILabel::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
    }

    void UILabel::renderControl(const OUIContextRef& context, const Rect& rect)
    {
        const auto& callback = context->getStyle<UILabel>(getStyle());
        if (callback)
        {
            callback->render(OThis, rect);
        }
        else
        {
            context->renderTextComponent(OThis, rect, textComponent);
        }
    }
};

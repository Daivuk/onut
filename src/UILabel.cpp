// Onut
#include <onut/UIContext.h>
#include <onut/UILabel.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUILabelRef UILabel::create()
    {
        return std::shared_ptr<UILabel>(new UILabel());
    }

    UILabel::UILabel()
    {
        textComponent.font.align = onut::Align::Left;
    }

    void UILabel::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UILabel*>(&other);
        if (pOther)
        {
            textComponent = pOther->textComponent;
        }
        UIControl::operator=(other);
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

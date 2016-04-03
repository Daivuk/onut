// Onut
#include <onut/UIContext.h>
#include <onut/UIImage.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUIImageRef UIImage::create()
    {
        return std::shared_ptr<UIImage>(new UIImage());
    }

    void UIImage::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UIImage*>(&other);
        if (pOther)
        {
            scale9Component = pOther->scale9Component;
        }
        UIControl::operator=(other);
    }

    void UIImage::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        scale9Component = getJsonScale9Component(jsonNode);
    }

    void UIImage::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonScale9Component(jsonNode, scale9Component, allocator);
    }

    void UIImage::renderControl(const OUIContextRef& context, const Rect& rect)
    {
        const auto& callback = context->getStyle<UIImage>(getStyle());
        if (callback)
        {
            callback->render(OThis, rect);
        }
        else
        {
            context->renderScale9Component(OThis, rect, scale9Component);
        }
    }
};

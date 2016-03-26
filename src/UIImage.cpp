// Onut
#include <onut/UIContext.h>
#include <onut/UIImage.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUIImageRef UIImage::create()
    {
        return OMake<UIImage>();
    }

    UIImage::UIImage(const UIImage& other) :
        UIControl(other)
    {
        scale9Component = other.scale9Component;
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

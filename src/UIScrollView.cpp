// Onut
#include <onut/UIContext.h>
#include <onut/UIScrollView.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUIScrollViewRef UIScrollView::create()
    {
        return std::shared_ptr<UIScrollView>(new UIScrollView());
    }

    void UIScrollView::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UIScrollView*>(&other);
        if (pOther)
        {
            isScrollH = pOther->isScrollH;
            isScrollV = pOther->isScrollV;
            padding = pOther->padding;
            m_scrollH = pOther->m_scrollH;
            m_scrollV = pOther->m_scrollV;
        }
        UIControl::operator=(other);
    }

    void UIScrollView::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        isScrollH = getJsonBool(jsonNode["scrollH"]);
        isScrollV = getJsonBool(jsonNode["scrollV"]);
        padding = getJsonPadding(jsonNode["padding"]);
    }

    void UIScrollView::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonBool(jsonNode, "scrollH", isScrollH, allocator, false);
        setJsonBool(jsonNode, "scrollV", isScrollV, allocator, true);
        setJsonPadding(jsonNode, padding, allocator);
    }
};

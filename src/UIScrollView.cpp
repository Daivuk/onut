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

    UIScrollView::UIScrollView()
        : m_scroll(0)
    {
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

    float UIScrollView::getTotalHeight() const
    {
        float total = 0.0f;

        const auto &children = getChildren();
        for (const auto& pChild : children)
        {
            const auto &child_rect = pChild->rect;
            total = std::max(total, child_rect.y + child_rect.w);
        }

        return total;
    }

    void UIScrollView::onChildrenChanged()
    {
        float contentSize = getTotalHeight();
        auto scrollMaxSize = contentSize - m_worldRect.w;
        if (m_scroll.get() > scrollMaxSize) m_scroll = scrollMaxSize;
        if (m_scroll.get() < 0) m_scroll = 0.f;
    }

    bool UIScrollView::onMouseScrollInternal(const UIMouseEvent& evt)
    {
        float newScroll = m_scroll.get() - evt.scroll;
        
        float contentSize = getTotalHeight();
        m_worldRect = getWorldRect(evt.pContext);
        auto scrollMaxSize = contentSize - m_worldRect.w;
        if (newScroll > scrollMaxSize) newScroll = scrollMaxSize;
        if (newScroll < 0) newScroll = 0.f;

        m_scroll.playFromCurrent(newScroll, 0.15f, OTweenEaseOut);

        return true;
    }

    void UIScrollView::adjustRect(Rect& rect)
    {
        rect.y -= m_scroll.get();
    }
};

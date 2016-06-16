// Onut
#include <onut/UIContext.h>
#include <onut/UITreeView.h>

// Private
#include "UIJson.h"

namespace onut
{
    OUITreeViewRef UITreeView::create()
    {
        return std::shared_ptr<UITreeView>(new UITreeView());
    }

    void UITreeView::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UITreeView*>(&other);
        if (pOther)
        {
            for (auto pOtherItem : pOther->m_items)
            {
                m_items.push_back(std::shared_ptr<UITreeViewItem>(new UITreeViewItem(*pOtherItem)));
            }
        }
        UIControl::operator=(other);
    }

    void UITreeView::addItem(OUITreeViewItemRef pItem)
    {
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        auto pParent = pItem->getParent();
        if (pParent) pParent->removeItem(pItem);
        pItem->setTreeView(ODynamicCast<UITreeView>(OThis));
        m_items.push_back(pItem);
    }

    void UITreeView::addItemBefore(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pBefore)
    {
        if (!pBefore)
        {
            addItem(pItem);
            return;
        }
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        auto pParent = pItem->getParent();
        if (pParent) pParent->removeItem(pItem);
        pItem->setTreeView(ODynamicCast<UITreeView>(OThis));
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pBefore)
            {
                m_items.insert(it, pItem);
                return;
            }
        }
    }

    void UITreeView::addItemAfter(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pAfter)
    {
        if (!pAfter)
        {
            addItem(pItem);
            return;
        }
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        auto pParent = pItem->getParent();
        if (pParent) pParent->removeItem(pItem);
        pItem->setTreeView(ODynamicCast<UITreeView>(OThis));
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pAfter)
            {
                ++it;
                m_items.insert(it, pItem);
                return;
            }
        }
    }

    void UITreeView::removeItem(OUITreeViewItemRef pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                pItem->m_pParent.reset();
                pItem->setTreeView(nullptr);
                return;
            }
        }
    }

    void UITreeView::clear()
    {
        m_items.clear();
    }

    OUITreeViewItemRef UITreeView::getItemAtPosition(const Vector2& pos, const Rect& rect, bool* pPickedExpandButton, Rect* pItemRect) const
    {
        // Render it's items
        Rect itemRect = {rect.x, rect.y, rect.z, itemHeight};
        itemRect.y -= m_scroll;
        for (auto pItem : m_items)
        {
            auto pRet = getItemAtPosition(pItem, pos, itemRect, pPickedExpandButton, pItemRect);
            if (pRet)
            {
                return pRet;
            }
        }
        return nullptr;
    }

    float UITreeView::getTotalHeight(const OUITreeViewItemRef& pItem) const
    {
        float ret = itemHeight;
        if (pItem)
        {
            if (pItem->isExpanded)
            {
                for (auto pChild : pItem->getItems())
                {
                    ret += getTotalHeight(pChild);
                }
            }
        }
        else
        {
            for (auto pChild : m_items)
            {
                ret += getTotalHeight(pChild);
            }
        }
        return ret;
    }

    OUITreeViewItemRef UITreeView::getItemAtPosition(const OUITreeViewItemRef& pItem, const Vector2& pos, Rect& rect, bool* pPickedExpandButton, Rect* pItemRect) const
    {
        if (pos.y >= rect.y &&
            pos.y <= rect.y + rect.w)
        {
            if (pos.x >= rect.x + expandClickWidth ||
                pos.x <= rect.x)
            {
                if (pItemRect) *pItemRect = rect;
                return pItem;
            }
            else if (pPickedExpandButton)
            {
                if (pItemRect) *pItemRect = rect;
                *pPickedExpandButton = true;
                return pItem;
            }
        }
        rect.y += itemHeight;
        if (pItem->isExpanded)
        {
            if (!pItem->m_items.empty())
            {
                auto xOffset = expandedXOffset;
                rect.x += xOffset;
                rect.z -= xOffset;
                for (auto pHisItem : pItem->m_items)
                {
                    auto pRet = getItemAtPosition(pHisItem, pos, rect, pPickedExpandButton, pItemRect);
                    if (pRet)
                    {
                        return pRet;
                    }
                }
                rect.z += xOffset;
                rect.x -= xOffset;
            }
        }
        return nullptr;
    }

    void UITreeView::unselectAll()
    {
        m_selectedItems.clear();
        for (auto pItem : m_items)
        {
            unselectItem(pItem);
        }
    }

    void UITreeView::unselectItem(const OUITreeViewItemRef& pItem)
    {
        pItem->m_isSelected = false;
        for (auto pHisItem : pItem->m_items)
        {
            unselectItem(pHisItem);
        }
    }

    void UITreeView::addSelectedItem(const OUITreeViewItemRef& pItem)
    {
        m_selectedItems.push_back(pItem);
        pItem->m_isSelected = true;
        expandTo(pItem);
    }

    OUITreeViewItemRef UITreeView::getSelectedItem() const
    {
        if (m_selectedItems.empty()) return nullptr;
        return m_selectedItems.front();
    }

    void UITreeView::expandTo(const OUITreeViewItemRef& pItem)
    {
        if (auto pParent = pItem->getParent())
        {
            pParent->isExpanded = true;
            expandTo(pParent);
        }
    }

    void UITreeView::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        expandedXOffset = getJsonFloat(jsonNode["expandedXOffset"], 18.f);
        expandClickWidth = getJsonFloat(jsonNode["expandClickWidth"], 18.f);
        itemHeight = getJsonFloat(jsonNode["itemHeight"], 18.f);
    }

    void UITreeView::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonFloat(jsonNode, "expandedXOffset", expandedXOffset, allocator, 18.f);
        setJsonFloat(jsonNode, "expandClickWidth", expandClickWidth, allocator, 18.f);
        setJsonFloat(jsonNode, "itemHeight", itemHeight, allocator, 18.f);
    }

    void UITreeView::renderControl(const OUIContextRef& context, const Rect& rect)
    {
        const auto& callback = context->getStyle<UITreeView>(getStyle());
        if (callback)
        {
            callback->render(OThis, rect);
        }

        // Render it's items
        Rect itemRect = {rect.x, rect.y, rect.z, itemHeight};
        itemRect.y -= m_scroll;
        const auto& itemCallback = context->getStyle<UITreeViewItem>(getStyle());
        if (itemCallback)
        {
            for (auto pItem : m_items)
            {
                pItem->render(itemCallback, ODynamicCast<OUITreeView>(OThis), rect, itemRect);
            }
        }

        if (allowReorder && m_isDragging && !m_selectedItems.empty())
        {
            auto pItem = m_selectedItems.front();
            itemRect.x = m_dragMousePos.x;
            itemRect.y = m_dragMousePos.y;
            itemRect.y += 8.f;
            itemRect.x += 8.f;
            pItem->renderDrag(itemCallback, ODynamicCast<OUITreeView>(OThis), rect, itemRect);
            if (m_dragAfterItem || m_dragBeforeItem)
            {
                context->drawInsert(m_dragInBetweenRect);
            }
        }
    }

    void UITreeView::onMouseDownInternal(const UIMouseEvent& evt)
    {
        auto worldRect = getWorldRect(evt.pContext);
        bool pickedExpandButton = false;
        auto pPicked = getItemAtPosition(evt.mousePos, worldRect, &pickedExpandButton);
        if (pPicked)
        {
            if (pickedExpandButton)
            {
                pPicked->isExpanded = !pPicked->isExpanded;
            }
            else
            {
                if (!evt.isCtrlDown)
                {
                    unselectAll();
                    addSelectedItem(pPicked);
                }
                else
                {
                    bool found = false;
                    for (auto it = m_selectedItems.begin(); it != m_selectedItems.end(); ++it)
                    {
                        auto pItem = *it;
                        if (pItem == pPicked)
                        {
                            m_selectedItems.erase(it);
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        addSelectedItem(pPicked);
                    }
                }
                if (onSelectionChanged)
                {
                    UITreeViewSelectEvent myEvt;
                    myEvt.pContext = evt.pContext;
                    myEvt.selectedItems = m_selectedItems;
                    onSelectionChanged(ODynamicCast<UITreeView>(OThis), myEvt);
                }
            }
        }
        if (allowReorder)
        {
            m_mousePosOnDragStart = evt.mousePos;
        }
    }

    void UITreeView::onMouseMoveInternal(const UIMouseEvent& evt)
    {
        if (hasFocus(evt.pContext) && evt.isMouseDown)
        {
            if (allowReorder)
            {
                if (!m_isDragging)
                {
                    if (std::abs(m_mousePosOnDragStart.y - evt.mousePos.y) >= 3)
                    {
                        m_isDragging = true;
                    }
                }
                else
                {
                    m_dragMousePos = evt.mousePos;
                    auto worldRect = getWorldRect(evt.pContext);
                    bool pickedExpandButton = false;
                    m_dragBeforeItem = nullptr;
                    m_dragAfterItem = nullptr;
                    auto pPicked = getItemAtPosition(evt.mousePos, worldRect, &pickedExpandButton, &m_dragInBetweenRect);
                    if (pPicked)
                    {
                        if (m_dragHoverItem)
                        {
                            m_dragHoverItem->m_isSelected = false;
                            m_dragHoverItem = nullptr;
                        }
                        if (evt.mousePos.y - m_dragInBetweenRect.y < itemHeight / 4)
                        {
                            m_dragBeforeItem = pPicked;
                            m_dragInBetweenRect.w = 2.f;
                            m_dragInBetweenRect.y -= 1.f;
                        }
                        else if (evt.mousePos.y > m_dragInBetweenRect.y + m_dragInBetweenRect.w - itemHeight / 4)
                        {
                            m_dragAfterItem = pPicked;
                            m_dragInBetweenRect.y += m_dragInBetweenRect.w - 1.f;
                            m_dragInBetweenRect.w = 2.f;
                        }
                        if (!pPicked->getIsSelected() && !m_dragBeforeItem && !m_dragAfterItem)
                        {
                            m_dragHoverItem = pPicked;
                            m_dragHoverItem->m_isSelected = true;
                        }
                    }
                }
            }
        }
    }

    void UITreeView::onMouseUpInternal(const UIMouseEvent& evt)
    {
        if (!m_isDragging) return;

        auto dragHover = m_dragHoverItem;
        auto dragBefore = m_dragBeforeItem;
        auto dragAfter = m_dragAfterItem;

        m_isDragging = false;
        if (m_dragHoverItem)
        {
            m_dragHoverItem->m_isSelected = false;
        }
        m_dragHoverItem = nullptr;
        m_dragBeforeItem = nullptr;
        m_dragAfterItem = nullptr;

        if (dragHover)
        {
            for (auto pItem : m_selectedItems)
            {
                dragHover->addItem(pItem);
            }
            dragHover->isExpanded = true;
            if (onMoveItemInto)
            {
                UITreeViewMoveEvent myEvt;
                myEvt.pContext = evt.pContext;
                myEvt.pTarget = dragHover;
                myEvt.selectedItems = m_selectedItems;
                onMoveItemInto(ODynamicCast<UITreeView>(OThis), myEvt);
            }
        }
        else if (dragBefore)
        {
            int modified = 0;
            for (auto pItem : m_selectedItems)
            {
                if (pItem == dragBefore) continue;
                if (auto pParent = dragBefore->getParent())
                {
                    pParent->addItemBefore(pItem, dragBefore);
                    ++modified;
                }
            }
            dragBefore->isExpanded = true;
            if (onMoveItemBefore && modified)
            {
                UITreeViewMoveEvent myEvt;
                myEvt.pContext = evt.pContext;
                myEvt.pTarget = dragBefore;
                myEvt.selectedItems = m_selectedItems;
                onMoveItemBefore(ODynamicCast<UITreeView>(OThis), myEvt);
            }
        }
        else if (dragAfter)
        {
            int modified = 0;
            for (auto pItem : m_selectedItems)
            {
                if (pItem == dragAfter) continue;
                if (auto pParent = dragAfter->getParent())
                {
                    pParent->addItemAfter(pItem, dragAfter);
                    ++modified;
                }
            }
            dragAfter->isExpanded = true;
            if (onMoveItemAfter && modified)
            {
                UITreeViewMoveEvent myEvt;
                myEvt.pContext = evt.pContext;
                myEvt.pTarget = dragAfter;
                myEvt.selectedItems = m_selectedItems;
                onMoveItemAfter(ODynamicCast<UITreeView>(OThis), myEvt);
            }
        }
    }

    void UITreeView::onMouseScrollInternal(const UIMouseEvent& evt)
    {
        m_scroll -= evt.scroll;
        auto contentSize = getTotalHeight();
        auto worldRect = getWorldRect(evt.pContext);
        auto scrollMaxSize = contentSize - worldRect.w;
        if (m_scroll > scrollMaxSize) m_scroll = scrollMaxSize;
        if (m_scroll < 0) m_scroll = 0.f;
    }

    void UITreeView::sortItemsByName()
    {
        std::sort(m_items.begin(), m_items.end(), [](const OUITreeViewItemRef& a, const OUITreeViewItemRef& b) -> bool
        {
            return a->text < b->text;
        });
    }
};

namespace onut
{
    OUITreeViewItemRef UITreeViewItem::create()
    {
        return OMake<UITreeViewItem>();
    }

    OUITreeViewItemRef UITreeViewItem::create(const std::string& text)
    {
        return OMake<UITreeViewItem>(text);
    }

    UITreeViewItem::UITreeViewItem(const std::string& text) :
        text(text)
    {
    }

    UITreeViewItem::UITreeViewItem(const UITreeViewItem& other)
    {
        for (auto pOtherItem : other.m_items)
        {
            m_items.push_back(std::shared_ptr<UITreeViewItem>(new UITreeViewItem(*pOtherItem)));
        }
        isExpanded = other.isExpanded;
        text = other.text;
        pSharedUserData = other.pSharedUserData;
        pUserData = other.pUserData;
        opacity = other.opacity;
    }

    void UITreeViewItem::setTreeView(const OUITreeViewRef& pTreeView)
    {
        m_pTreeView = pTreeView;
        for (auto pItem : m_items)
        {
            pItem->setTreeView(pTreeView);
        }
    }

    void UITreeViewItem::addItem(OUITreeViewItemRef pItem)
    {
        if (auto pParent = pItem->getParent())
        {
            pParent->removeItem(pItem);
        }
        pItem->m_pParent = OThis;
        pItem->setTreeView(m_pTreeView.lock());
        m_items.push_back(pItem);
    }

    void UITreeViewItem::addItemBefore(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pBefore)
    {
        if (!pBefore)
        {
            addItem(pItem);
            return;
        }
        if (auto pParent = pItem->getParent())
        {
            pParent->removeItem(pItem);
        }
        pItem->m_pParent = OThis;
        pItem->setTreeView(m_pTreeView.lock());
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pBefore)
            {
                m_items.insert(it, pItem);
                return;
            }
        }
    }

    void UITreeViewItem::addItemAfter(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pAfter)
    {
        if (!pAfter)
        {
            addItem(pItem);
            return;
        }
        if (auto pParent = pItem->getParent())
        {
            pParent->removeItem(pItem);
        }
        pItem->m_pParent = OThis;
        pItem->setTreeView(m_pTreeView.lock());
        for (auto it = m_items.begin(); it != m_items.end(); ++it)
        {
            if (*it == pAfter)
            {
                m_items.insert(it + 1, pItem);
                return;
            }
        }
    }

    void UITreeViewItem::removeItem(OUITreeViewItemRef pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                pItem->setTreeView(nullptr);
                pItem->m_pParent.reset();
                return;
            }
        }
    }
};

#pragma once
// Onut
#include <onut/UIComponents.h>
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UITreeView)
OForwardDeclare(UITreeViewItem)

namespace onut
{
    class UITreeView final : public UIControl
    {
    public:
        using SelectionChangedCallback = std::function<void(const OUITreeViewRef&, const UITreeViewSelectEvent&)>;
        using MoveEventCallback = std::function<void(const OUITreeViewRef&, const UITreeViewMoveEvent&)>;
        using TreeViewItems = std::vector<OUITreeViewItemRef>;

        static OUITreeViewRef create();

        UITreeView(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::TreeView; }

        UIFontComponent font;
        float expandedXOffset = 16.f;
        float expandClickWidth = 18.f;
        float itemHeight = 18.f;
        bool allowReorder = false;

        void addItem(OUITreeViewItemRef pItem);
        void addItemBefore(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pBefore);
        void addItemAfter(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pAfter);
        void removeItem(OUITreeViewItemRef pItem);
        void clear();

        void unselectAll();
        void unselectItem(const OUITreeViewItemRef& pItem);
        void addSelectedItem(const OUITreeViewItemRef& pItem);
        OUITreeViewItemRef getSelectedItem() const;

        void sortItemsByName();

        void expandTo(const OUITreeViewItemRef& pItem);

        SelectionChangedCallback onSelectionChanged;
        MoveEventCallback onMoveItemInto;
        MoveEventCallback onMoveItemBefore;
        MoveEventCallback onMoveItemAfter;

    protected:
        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
        void onMouseDownInternal(const UIMouseEvent& evt) override;
        void onMouseMoveInternal(const UIMouseEvent& evt) override;
        void onMouseUpInternal(const UIMouseEvent& evt) override;
        void onMouseScrollInternal(const UIMouseEvent& evt) override;

    private:
        UITreeView() {}

        OUITreeViewItemRef getItemAtPosition(const Vector2& pos, const Rect& rect, bool* pPickedExpandButton = nullptr, Rect* pItemRect = nullptr) const;
        OUITreeViewItemRef getItemAtPosition(const OUITreeViewItemRef& pItem, const Vector2& pos, Rect& rect, bool* pPickedExpandButton = nullptr, Rect* pItemRect = nullptr) const;
        float getTotalHeight(const OUITreeViewItemRef& pItem = nullptr) const;

        TreeViewItems m_items;
        TreeViewItems m_selectedItems;
        float m_scroll = 0.f;
        bool m_isDragging = false;
        Vector2 m_mousePosOnDragStart;
        Vector2 m_dragMousePos;
        OUITreeViewItemRef m_dragHoverItem = nullptr;
        OUITreeViewItemRef m_dragBeforeItem = nullptr;
        OUITreeViewItemRef m_dragAfterItem = nullptr;
        Rect m_dragInBetweenRect;
    };
};

namespace onut
{
    class UITreeViewItem : public std::enable_shared_from_this<UITreeViewItem>
    {
        friend UITreeView;

    public:
        static OUITreeViewItemRef create();
        static OUITreeViewItemRef create(const std::string& text);

        UITreeViewItem() {}
        UITreeViewItem(const std::string& text);
        UITreeViewItem(const UITreeViewItem& other);

        bool isExpanded = false;
        std::string text;
        void* pUserData = nullptr;
        std::shared_ptr<void> pSharedUserData = nullptr;
        float opacity = 1.f;

        OUITreeViewItemRef getParent() const { return m_pParent.lock(); }

        void addItem(OUITreeViewItemRef pItem);
        void addItemBefore(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pBefore);
        void addItemAfter(OUITreeViewItemRef pItem, const OUITreeViewItemRef& pAfter);
        void removeItem(OUITreeViewItemRef pItem);

        OUITreeViewRef getTreeView() const { return m_pTreeView.lock(); }
        void setTreeView(const OUITreeViewRef& pTreeView);

        const UITreeView::TreeViewItems& getItems() const { return m_items; }

        bool getIsSelected() const { return m_isSelected; }

    private:
        template<typename TfnCallback>
        void render(const TfnCallback& itemCallback, const OUITreeViewRef& pTreeView, const Rect& treeViewRect, Rect& rect)
        {
            itemCallback->render(OThis, rect);
            rect.y += pTreeView->itemHeight;
            if (isExpanded)
            {
                if (!m_items.empty())
                {
                    auto xOffset = pTreeView->expandedXOffset;
                    rect.x += xOffset;
                    rect.z -= xOffset;
                    for (auto pItem : m_items)
                    {
                        pItem->render(itemCallback, pTreeView, treeViewRect, rect);
                    }
                    rect.z += xOffset;
                    rect.x -= xOffset;
                }
            }
        }

        template<typename TfnCallback>
        void renderDrag(const TfnCallback& itemCallback, const OUITreeViewRef& pTreeView, const Rect& treeViewRect, Rect& rect)
        {
            auto isSelected = m_isSelected;
            m_isSelected = false;
            opacity = .5f;
            itemCallback->render(OThis, rect);
            opacity = 1.f;
            m_isSelected = isSelected;
        }

        UITreeView::TreeViewItems m_items;
        OUITreeViewWeak m_pTreeView;
        bool m_isSelected = false;
        OUITreeViewItemWeak m_pParent;
    };
};

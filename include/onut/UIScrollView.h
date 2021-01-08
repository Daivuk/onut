#ifndef UISCROLLVIEW_H_INCLUDED
#define UISCROLLVIEW_H_INCLUDED

// Onut
#include <onut/Anim.h>
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UIScrollView)

namespace onut
{
    class UIScrollView final : public UIControl
    {
    public:
        static OUIScrollViewRef create();

        UIScrollView(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::ScrollView; }

        bool isScrollH = false;
        bool isScrollV = true;
        Vector4 padding;

    protected:
        UIScrollView();

        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        bool onMouseScrollInternal(const UIMouseEvent& evt) override;
        void onChildrenChanged() override;
        float getTotalHeight() const;
        void adjustRect(Rect& rect) override;

    private:
        float m_scrollH = 0;
        float m_scrollV = 0;
        OAnimFloat m_scroll;
        Rect m_worldRect; // Last known
    };
};

#endif

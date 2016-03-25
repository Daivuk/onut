#pragma once
#include "onut/UIControl.h"

#include "onut/ForwardDeclaration.h"
OForwardDeclare(UIPanel)

namespace onut
{
    class UIPanel final : public UIControl
    {
    public:
        static OUIPanelRef create();

        UIPanel() {}
        UIPanel(const UIPanel& other);

        Type getType() const override { return Type::Panel; }

        Color color = Color::White;

    protected:
        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

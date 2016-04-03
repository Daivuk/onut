#pragma once
// Onut
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UIPanel)

namespace onut
{
    class UIPanel final : public UIControl
    {
    public:
        static OUIPanelRef create();

        UIPanel(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::Panel; }

        Color color = Color::White;

    protected:
        UIPanel() {}

        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

#pragma once
// Onut
#include <onut/UIComponents.h>
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UILabel)

namespace onut
{
    class UILabel final : public UIControl
    {
    public:
        static OUILabelRef create();

        UILabel(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::Label; }

        UITextComponent textComponent;

    protected:
        UILabel();

        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

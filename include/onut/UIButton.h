#pragma once
// Onut
#include <onut/UIComponents.h>
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UIButton)

namespace onut
{
    class UIButton final : public UIControl
    {
    public:
        static OUIButtonRef create();

        UIButton(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::Button; }
        bool isNavigatable() const override { return true; }

        UIScale9Component scale9Component;
        UITextComponent textComponent;

    protected:
        UIButton();

        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

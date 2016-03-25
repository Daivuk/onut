#pragma once
#include "onut/UIComponents.h"
#include "onut/UIControl.h"

#include "onut/ForwardDeclaration.h"
OForwardDeclare(UIButton)

namespace onut
{
    class UIButton final : public UIControl
    {
    public:
        static OUIButtonRef create();

        UIButton();
        UIButton(const UIButton& other);

        Type getType() const override { return Type::Button; }
        bool isNavigatable() const override { return true; }

        UIScale9Component scale9Component;
        UITextComponent textComponent;

    protected:
        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

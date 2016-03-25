#pragma once
#include "onut/UIComponents.h"
#include "onut/UIControl.h"

#include "onut/ForwardDeclaration.h"
OForwardDeclare(UIImage)

namespace onut
{
    class UIImage final : public UIControl
    {
    public:
        static OUIImageRef create();

        UIImage() {}
        UIImage(const UIImage& other);

        Type getType() const override { return Type::Image; }

        UIScale9Component scale9Component;

    protected:
        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

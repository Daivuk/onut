#ifndef UIIMAGE_H_INCLUDED
#define UIIMAGE_H_INCLUDED

// Onut
#include <onut/UIComponents.h>
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UIImage)

namespace onut
{
    class UIImage final : public UIControl
    {
    public:
        static OUIImageRef create();

        UIImage(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::Image; }

        UIScale9Component scale9Component;

    protected:
        UIImage() {}

        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
    };
};

#endif

#pragma once
// Onut
#include <onut/UIComponents.h>
#include <onut/UIControl.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UICheckBox)

namespace onut
{
    class UICheckBox final : public UIControl
    {
    public:
        enum class CheckBehavior
        {
            /** Normal checkbox. Can toggle it on and off regardless of sibblings */
            Normal,
            /** Behaves like a radio button. Only one sibblings can be checked. And one HAS to be checked */
            Exclusive,
            /** Like exclusive, but all sibblings can be toggled off */
            Optional
        };

        using CheckChangedCallback = std::function<void(const OUICheckBoxRef&, const UICheckEvent&)>;

        static OUICheckBoxRef create();

        UICheckBox(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::CheckBox; }

        UIIconComponent iconComponent;
        UITextComponent textComponent;
        CheckBehavior behavior = CheckBehavior::Normal;

        bool getIsChecked() const { return m_isChecked; }
        void setIsChecked(bool in_isChecked);
        bool isNavigatable() const override { return true; }

        CheckChangedCallback onCheckChanged;

    protected:
        UICheckBox() {}

        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;
        void onClickInternal(const UIMouseEvent& evt) override;

    private:
        bool m_isChecked = false;
    };
};

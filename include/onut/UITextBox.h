#pragma once
// Onut
#include <onut/UIComponents.h>
#include <onut/UIControl.h>

// STL
#include <chrono>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UITextBox)

namespace onut
{
    class UITextBox final : public UIControl
    {
    public:
        using TextEventCallback = std::function<void(const OUITextBoxRef&, const UITextBoxEvent&)>;

        static OUITextBoxRef create();

        UITextBox(const UIControl& other) = delete;
        void operator=(const UIControl& other) override;

        Type getType() const override { return Type::TextBox; }

        UITextComponent textComponent;
        UIScale9Component scale9Component;
        float min = -std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::max();
        float step = 1.f;

        const std::string::size_type* getSelectedTextRegion() const { return m_selectedTextRegion; }
        std::string::size_type getCursorPos() const { return m_cursorPos; }
        bool isCursorVisible() const;
        bool isNavigatable() const override { return true; }

        TextEventCallback onTextChanged;
        TextEventCallback onNumberSpinStart;
        TextEventCallback onNumberSpinEnd;

        void selectAll();

        bool getIsNumerical() const { return m_isNumerical; }
        void setIsNumerical(bool isNumerical);
        int getDecimalPrecision() const { return m_decimalPrecision; }
        void setIsDecimalPrecision(int decimalPrecision);

        float getFloat() const;
        int getInt() const;
        void setFloat(float f);
        void setInt(int i);

    protected:
        void load(const rapidjson::Value& jsonNode) override;
        void save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const override;
        void renderControl(const OUIContextRef& context, const Rect& rect) override;

        void onGainFocusInternal(const UIFocusEvent& evt) override;
        void onLoseFocusInternal(const UIFocusEvent& evt) override;
        void onMouseDownInternal(const UIMouseEvent& evt) override;
        void onMouseMoveInternal(const UIMouseEvent& evt) override;
        void onMouseUpInternal(const UIMouseEvent& evt) override;
        void onWriteInternal(char c, const OUIContextRef& context) override;
        void onKeyDownInternal(const UIKeyEvent& evt) override;

    private:
        UITextBox() {}

        void numerifyText();

        std::string::size_type m_selectedTextRegion[2];
        bool m_isSelecting = false;
        std::string::size_type m_cursorPos = 0;
        std::chrono::steady_clock::time_point m_cursorTime;
        bool m_isTextChanged = false;
        bool m_isNumerical = false;
        int m_decimalPrecision = 0;
        float m_mousePosOnDown;
        float m_valueOnDown;
        bool m_isSpinning = false;
    };
};

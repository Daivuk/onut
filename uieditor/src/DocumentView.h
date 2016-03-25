#pragma once
#include "onut/ContentManager.h"
#include "onut/UICheckBox.h"
#include "onut/UIContext.h"
#include "onut/UIControl.h"
#include "onut/UIPanel.h"
#include "onut/UITextBox.h"
#include "onut/UITreeView.h"

enum class eDocumentState : uint8_t
{
    IDLE,
    MOVING_GIZO,
    MOVING_HANDLE
};

class DocumentView
{
public:
    DocumentView(const std::string& filename);
    virtual ~DocumentView();

    void setSelected(const OUIControlRef& in_pSelected, bool bUpdateSceneGraph = true);

    void update();
    void render();
    void save();

    OUIContextRef pUIContext = nullptr;
    OUIContextRef pUIContextUserStyle = nullptr;
    OUIContextRef pUIContextEditorStyle = nullptr;
    OUIControlRef pUIScreen = nullptr;
    OUIControlRef pSelected = nullptr;
    OContentManagerRef pContentManager;

    void onGizmoStart(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent);
    void onGizmoEnd(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent);

    void controlCreated(const OUIControlRef& pControl, const OUIControlRef& pParent);
    void controlDeleted(const OUIControlRef& pControl);
    void updateSelectedGizmoRect();
    void updateInspector();
    void repopulateTreeView(const OUIControlRef& pControl);
    void onKeyDown(uintptr_t key);

    bool isBusy() const { return m_state != eDocumentState::IDLE; }

    const std::string& getFilename() const { return m_filename; }

    void cut();
    void copy();
    void paste();
    void del();

private:
    void onGizmoHandleStart(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent);
    void onGizmoHandleEnd(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent);

    void updateSelectionWithRect(const Rect& rect);
    void updateGizmoRect();
    void updateMovingHandle();
    void updateMovingGizmo();
    void deleteSelection();
    void setDirty(bool isDirty);
    void concludeTransform(const OUIControlRef& pControl, const Rect& previousRect);

    void snapX(float x, float &ret, const Rect& rect, float &closest, bool& found);
    float snapX(const OUIControlRef& pControl, float x);
    void snapY(float y, float &ret, const Rect& rect, float &closest, bool& found);
    float snapY(const OUIControlRef& pControl, float y);
    bool getXAutoGuide(const Rect& rect, float& x, bool& side);
    bool getYAutoGuide(const Rect& rect, float& y, bool& side);
    void xAutoGuideAgainst(const Rect& otherRect, bool& found, const Rect& rect, float& x, bool& side, float& closest);
    void yAutoGuideAgainst(const Rect& otherRect, bool& found, const Rect& rect, float& y, bool& side, float& closest);

    OUIPanelRef m_pGizmo = nullptr;
    OUIControlRef m_gizmoHandles[8];
    OUIControlRef m_guides[2];
    OUIControlRef m_pCurrentHandle = nullptr;
    OUIControlRef m_pClipBoard = nullptr;
    OUITreeViewRef m_pSceneGraph = nullptr;
    Vector2 m_mousePosOnDown;
    Rect m_controlRectOnDown;
    Rect m_controlWorldRectOnDown;
    eDocumentState m_state = eDocumentState::IDLE;
    bool m_autoGuide = true;
    float m_autoPadding = 8.f;
    bool m_isDirty = false;
    std::string m_filename;
    bool m_bStartMoving = false;
    OUICheckBoxRef m_pChkSnap = nullptr;
    OUICheckBoxRef m_pChkSnapMargins = nullptr;
    OUITextBoxRef m_pTxtSnapMargins = nullptr;
};

class IControlInspectorBind
{
public:
    virtual void updateInspector(const OUIControlRef& pControl) = 0;
    virtual void updateControl(const OUIControlRef& pControl) = 0;
};

template<typename Ttype, typename TtargetControl>
class ControlInspectorBind : public IControlInspectorBind
{
public:
    using Tgetter = std::function < Ttype(const std::shared_ptr<TtargetControl>&) > ;
    using Tsetter = std::function < void(const std::shared_ptr<TtargetControl>&, const Ttype&) >;
    using TgetInspector = std::function < Ttype() >;
    using TsetInspector = std::function < void(const Ttype&) >;

    ControlInspectorBind(const std::string& actionName,
                         Ttype* pInspectorValue,
                         const Tgetter& getter,
                         const Tsetter& setter,
                         const TgetInspector& getInspector = nullptr,
                         const TsetInspector& setInspector = nullptr) :
                         m_actionName(actionName),
                         m_pInspectorValue(pInspectorValue),
                         m_getter(getter),
                         m_setter(setter),
                         m_getInspector(getInspector),
                         m_setInspector(setInspector)
    {
    }

private:
    std::string     m_actionName;
    Ttype*          m_pInspectorValue;
    Tgetter         m_getter;
    Tsetter         m_setter;
    TgetInspector   m_getInspector;
    TsetInspector   m_setInspector;

public:
    void updateInspector(const OUIControlRef& pControl) override
    {
        auto tControl = ODynamicCast<TtargetControl>(pControl);
        if (!tControl) return;
        if (m_pInspectorValue)
        {
            *m_pInspectorValue = m_getter(tControl);
        }
        else
        {
            m_setInspector(m_getter(tControl));
        }
    }

    void updateControl(const OUIControlRef& pControl) override
    {
        auto tControl = ODynamicCast<TtargetControl>(pControl);
        if (!tControl) return;

        auto prevVar = m_getter(tControl);
        Ttype newVal;
        if (m_pInspectorValue)
        {
            newVal = *m_pInspectorValue;
        }
        else
        {
            newVal = m_getInspector();
        }

        oActionManager->doAction(OMake<onut::Action>(m_actionName,
            [=]{
            m_setter(tControl, newVal);
            g_pDocument->updateInspector();
        },
            [=]{
            m_setter(tControl, prevVar);
            g_pDocument->updateInspector();
        }));
    }
};

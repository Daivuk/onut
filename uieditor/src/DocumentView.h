#pragma once
#include "onut.h"

enum class eDocumentState : uint8_t
{
    IDLE,
    MOVING_GIZO,
    MOVING_HANDLE
};

class DocumentView
{
public:
    DocumentView();
    virtual ~DocumentView();

    void setSelected(onut::UIControl* in_pSelected, bool bUpdateSceneGraph = true);

    void update();
    void render();

    onut::UIContext* pUIContext = nullptr;
    onut::UIControl* pUIScreen = nullptr;
    onut::UIControl* pSelected = nullptr;

    void onGizmoStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent);
    void onGizmoEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent);

    void controlCreated(onut::UIControl* pControl, onut::UIControl* pParent);
    void updateSelectedGizmoRect();
    void updateInspector();
    void repopulateTreeView(onut::UIControl* pControl);

private:
    void onGizmoHandleStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent);
    void onGizmoHandleEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent);

    void updateSelectionWithRect(const onut::sUIRect& rect);
    void updateGizmoRect();
    void updateMovingHandle();
    void updateMovingGizmo();
    void deleteSelection();

    void snapX(float x, float &ret, const onut::sUIRect& rect, float &closest, bool& found);
    float snapX(onut::UIControl* pControl, float x);
    void snapY(float y, float &ret, const onut::sUIRect& rect, float &closest, bool& found);
    float snapY(onut::UIControl* pControl, float y);
    bool getXAutoGuide(const onut::sUIRect& rect, float& x, bool& side);
    bool getYAutoGuide(const onut::sUIRect& rect, float& y, bool& side);
    void xAutoGuideAgainst(const onut::sUIRect& otherRect, bool& found, const onut::sUIRect& rect, float& x, bool& side, float& closest);
    void yAutoGuideAgainst(const onut::sUIRect& otherRect, bool& found, const onut::sUIRect& rect, float& y, bool& side, float& closest);

    onut::UIPanel*      m_pGizmo = nullptr;
    onut::UIControl*    m_gizmoHandles[8];
    onut::UIControl*    m_guides[2];
    onut::UIControl*    m_pCurrentHandle = nullptr;
    onut::UITreeView*   m_pSceneGraph = nullptr;
    Vector2             m_mousePosOnDown;
    onut::sUIRect       m_controlRectOnDown;
    onut::sUIRect       m_controlWorldRectOnDown;
    eDocumentState      m_state = eDocumentState::IDLE;
    bool                m_autoGuide = true;
    float               m_autoPadding = 8.f;
};

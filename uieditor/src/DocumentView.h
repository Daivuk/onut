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

private:
    void onGizmoHandleStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent);
    void onGizmoHandleEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent);

    void updateSelectionWithRect(const onut::sUIRect& rect);
    void updateMovingHandle();
    void updateMovingGizmo();
    void updateInspector();

    onut::UIPanel*      m_pGizmo = nullptr;
    onut::UIControl*    m_gizmoHandles[8];
    onut::UIControl*    m_pCurrentHandle = nullptr;
    onut::UITreeView*   m_pSceneGraph = nullptr;
    Vector2             m_mousePosOnDown;
    onut::sUIRect       m_gizmoRectOnDown;
    eDocumentState      m_state = eDocumentState::IDLE;
};

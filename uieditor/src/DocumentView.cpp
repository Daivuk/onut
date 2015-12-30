#include "ActionManager.h"
#include "DocumentView.h"
#include "viewStyles.h"

extern onut::UIControl*     g_pUIScreen;
extern onut::UIContext*     g_pUIContext;
extern onut::ActionManager  g_actionManager;

extern onut::UICheckBox*    g_pInspector_UIControl_chkEnabled;
extern onut::UICheckBox*    g_pInspector_UIControl_chkVisible;
extern onut::UICheckBox*    g_pInspector_UIControl_chkClickThrough;
extern onut::UICheckBox*    g_pInspector_UIControl_chkClipChildren;
extern onut::UITextBox*     g_pInspector_UIControl_txtName;
extern onut::UITextBox*     g_pInspector_UIControl_txtStyle;
extern onut::UITextBox*     g_pInspector_UIControl_txtX;
extern onut::UITextBox*     g_pInspector_UIControl_txtY;
extern onut::UICheckBox*    g_pInspector_UIControl_chkXPercent;
extern onut::UICheckBox*    g_pInspector_UIControl_chkYPercent;
extern onut::UITextBox*     g_pInspector_UIControl_txtWidth;
extern onut::UITextBox*     g_pInspector_UIControl_txtHeight;
extern onut::UICheckBox*    g_pInspector_UIControl_chkWidthPercent;
extern onut::UICheckBox*    g_pInspector_UIControl_chkHeightPercent;
extern onut::UICheckBox*    g_pInspector_UIControl_chkWidthRelative;
extern onut::UICheckBox*    g_pInspector_UIControl_chkHeightRelative;
extern onut::UICheckBox*    g_pInspector_UIControl_chkTOP_LEFT;
extern onut::UICheckBox*    g_pInspector_UIControl_chkTOP;
extern onut::UICheckBox*    g_pInspector_UIControl_chkTOP_RIGHT;
extern onut::UICheckBox*    g_pInspector_UIControl_chkLEFT;
extern onut::UICheckBox*    g_pInspector_UIControl_chkCENTER;
extern onut::UICheckBox*    g_pInspector_UIControl_chkRIGHT;
extern onut::UICheckBox*    g_pInspector_UIControl_chkBOTTOM_LEFT;
extern onut::UICheckBox*    g_pInspector_UIControl_chkBOTTOM;
extern onut::UICheckBox*    g_pInspector_UIControl_chkBOTTOM_RIGHT;
extern onut::UITextBox*     g_pInspector_UIControl_txtAnchorX;
extern onut::UITextBox*     g_pInspector_UIControl_txtAnchorY;
extern onut::UICheckBox*    g_pInspector_UIControl_chkXAnchorPercent;
extern onut::UICheckBox*    g_pInspector_UIControl_chkYAnchorPercent;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorTOP_LEFT;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorTOP;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorTOP_RIGHT;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorLEFT;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorCENTER;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorRIGHT;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorBOTTOM_LEFT;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorBOTTOM;
extern onut::UIButton*      g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT;

static HCURSOR curARROW = nullptr;
static HCURSOR curSIZENWSE = nullptr;
static HCURSOR curSIZENESW = nullptr;
static HCURSOR curSIZEWE = nullptr;
static HCURSOR curSIZENS = nullptr;
static HCURSOR curSIZEALL = nullptr;

extern void createUIStyles(onut::UIContext* pContext);
static const Color g_panelBGColor = OColorHex(2d2d30);
static const Color g_panelTitleBGColor = OColorHex(3f3f46);

DocumentView::DocumentView(const std::string& filename)
{
    m_filename = filename;
    auto path = onut::getPath(filename);
    if (!path.empty())
    {
        contentManager.addSearchPath(path);
        contentManager.addSearchPath(path + "/../fonts");
    }

    if (!curARROW) curARROW = LoadCursor(nullptr, IDC_ARROW);
    if (!curSIZENWSE) curSIZENWSE = LoadCursor(nullptr, IDC_SIZENWSE);
    if (!curSIZENESW) curSIZENESW = LoadCursor(nullptr, IDC_SIZENESW);
    if (!curSIZEWE) curSIZEWE = LoadCursor(nullptr, IDC_SIZEWE);
    if (!curSIZENS) curSIZENS = LoadCursor(nullptr, IDC_SIZENS);
    if (!curSIZEALL) curSIZEALL = LoadCursor(nullptr, IDC_SIZEALL);

    pUIContextUserStyle = new onut::UIContext(onut::sUIVector2{800, 600});
    pUIContextEditorStyle = new onut::UIContext(onut::sUIVector2{800, 600});
    createViewUIStyles(pUIContextUserStyle);
    createUIStyles(pUIContextEditorStyle);

    pUIContextEditorStyle->addStyle<onut::UIPanel>("", [](const onut::UIPanel* pPanel, const onut::sUIRect& rect)
    {
        OSB->drawRect(nullptr, onut::UI2Onut(rect), g_panelTitleBGColor);
        OSB->drawRect(nullptr, onut::UI2Onut(rect).Grow(-1), g_panelBGColor);
    });

    // We don't render controls
    //pUIContextEditorStyle->addStyle<onut::UIControl>("", [](const onut::UIControl* pPanel, const onut::sUIRect& rect)
    //{
    //    OSB->drawRect(nullptr, {rect.position.x, rect.position.y, rect.size.x, 1}, g_panelTitleBGColor);
    //    OSB->drawRect(nullptr, {rect.position.x, rect.position.y, 1, rect.size.y}, g_panelTitleBGColor);
    //    OSB->drawRect(nullptr, {rect.position.x, rect.position.y + rect.size.y, rect.size.x, 1}, g_panelTitleBGColor);
    //    OSB->drawRect(nullptr, {rect.position.x + rect.size.x, rect.position.y, 1, rect.size.y}, g_panelTitleBGColor);
    //});

    pUIContext = pUIContextUserStyle;
    
    if (m_filename.empty())
    {
        pUIScreen = new onut::UIControl(/*"../../assets/ui/editor.json"*/);
    }
    else
    {
        pUIScreen = new onut::UIControl(m_filename.c_str());
    }
    pUIScreen->retain();
    pUIScreen->name = "Root";
    pUIScreen->widthType = onut::eUIDimType::DIM_RELATIVE;
    pUIScreen->heightType = onut::eUIDimType::DIM_RELATIVE;

    m_guides[0] = g_pUIScreen->getChild("hGuide");
    m_guides[1] = g_pUIScreen->getChild("vGuide");

    // Dotted line gizmo for selection
    m_pGizmo = g_pUIScreen->getChild<onut::UIPanel>("gizmo");
    m_pGizmo->isVisible = false;
    m_gizmoHandles[0] = m_pGizmo->getChild("topLeftHandle");
    m_gizmoHandles[1] = m_pGizmo->getChild("topHandle");
    m_gizmoHandles[2] = m_pGizmo->getChild("topRightHandle");
    m_gizmoHandles[3] = m_pGizmo->getChild("leftHandle");
    m_gizmoHandles[4] = m_pGizmo->getChild("rightHandle");
    m_gizmoHandles[5] = m_pGizmo->getChild("bottomLeftHandle");
    m_gizmoHandles[6] = m_pGizmo->getChild("bottomHandle");
    m_gizmoHandles[7] = m_pGizmo->getChild("bottomRightHandle");
    m_pGizmo->onMouseDown = std::bind(&DocumentView::onGizmoStart, this, std::placeholders::_1, std::placeholders::_2);
    m_pGizmo->onMouseUp = std::bind(&DocumentView::onGizmoEnd, this, std::placeholders::_1, std::placeholders::_2);
    m_pGizmo->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZEALL); };
    m_pGizmo->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    for (auto& pGizmoHandle : m_gizmoHandles)
    {
        pGizmoHandle->onMouseDown = std::bind(&DocumentView::onGizmoHandleStart, this, std::placeholders::_1, std::placeholders::_2);
        pGizmoHandle->onMouseUp = std::bind(&DocumentView::onGizmoHandleEnd, this, std::placeholders::_1, std::placeholders::_2);
        pGizmoHandle->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    }
    m_gizmoHandles[0]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZENWSE); };
    m_gizmoHandles[1]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZENS); };
    m_gizmoHandles[2]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZENESW); };
    m_gizmoHandles[3]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZEWE); };
    m_gizmoHandles[4]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZEWE); };
    m_gizmoHandles[5]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZENESW); };
    m_gizmoHandles[6]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZENS); };
    m_gizmoHandles[7]->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curSIZENWSE); };

    // Add some dummy nodes in the scene graph for show
    m_pSceneGraph = g_pUIScreen->getChild<onut::UITreeView>("sceneGraph");
    m_pSceneGraph->clear();
    repopulateTreeView(pUIScreen);

    m_pChkSnap = g_pUIScreen->getChild<onut::UICheckBox>("chkSnap");
    m_pChkSnapMargins = g_pUIScreen->getChild<onut::UICheckBox>("chkSnapMargins");
    m_pTxtSnapMargins = g_pUIScreen->getChild<onut::UITextBox>("txtSnapMargins");
    m_autoPadding = m_pTxtSnapMargins->getFloat();
    m_pTxtSnapMargins->onTextChanged = [this](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
    {
        m_autoPadding = m_pTxtSnapMargins->getFloat();
    };

    // Transform handles
    const float HANDLE_SIZE = 6.f;
    const float HANDLE_PADDING = 3.f;

    updateInspector();

    setDirty(true);
}

DocumentView::~DocumentView()
{
    if (m_pClipBoard) m_pClipBoard->release();
    pUIScreen->release();
    delete pUIContextUserStyle;
    delete pUIContextEditorStyle;
}

void DocumentView::controlCreated(onut::UIControl* pControl, onut::UIControl* pParent)
{
    repopulateTreeView(pControl);
}

void DocumentView::controlDeleted(onut::UIControl* pControl)
{
    auto pItem = static_cast<onut::UITreeViewItem*>(pControl->pUserData);
    if (pItem->getParent())
    {
        pItem->getParent()->removeItem(pItem);
    }
    else
    {
        m_pSceneGraph->removeItem(pItem);
    }
}

void DocumentView::updateSelectedGizmoRect()
{
    m_pGizmo->isVisible = (pSelected != nullptr);
    if (pSelected)
    {
        m_pGizmo->rect = (pSelected->getWorldRect(*pUIContext));
    }
}

void DocumentView::setSelected(onut::UIControl* in_pSelected, bool bUpdateSceneGraph)
{ 
    if (bUpdateSceneGraph)
    {
        m_pSceneGraph->unselectAll();
    }
    if (in_pSelected == pUIScreen)
    {
        pSelected = nullptr;
    }
    else
    {
        pSelected = in_pSelected;
    }
    updateSelectedGizmoRect();
    if (pSelected)
    {
        if (bUpdateSceneGraph)
        {
            auto pItem = static_cast<onut::UITreeViewItem*>(pSelected->pUserData);
            m_pSceneGraph->addSelectedItem(pItem);
        }
    }
    updateInspector();
}

void DocumentView::onGizmoHandleStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    m_state = eDocumentState::MOVING_HANDLE;
    m_bStartMoving = false;
    m_mousePosOnDown = {mouseEvent.mousePos.x, mouseEvent.mousePos.y};
    m_pCurrentHandle = pControl;
    m_controlRectOnDown = pSelected->rect;
    m_controlWorldRectOnDown = pSelected->getWorldRect(*pUIContext);
}

void DocumentView::concludeTransform(onut::UIControl* pControl, const onut::sUIRect& previousRect)
{
    auto worldRect = pControl->getWorldRect(*pUIContext);
    if (worldRect.position.x != previousRect.position.x ||
        worldRect.position.y != previousRect.position.y ||
        worldRect.size.x != previousRect.size.x ||
        worldRect.size.y != previousRect.size.y)
    {
        g_actionManager.doAction(new onut::Action("Transform",
            [=]{
            pControl->setWorldRect(worldRect, *pUIContext);
            updateSelectionWithRect(worldRect);
            updateInspector();
        },
            [=]{
            pControl->setWorldRect(previousRect, *pUIContext);
            updateSelectionWithRect(previousRect);
            updateInspector();
        },
            [=]{
            pControl->retain();
        },
            [=]{
            pControl->release();
        }
        ));
    }
}

void DocumentView::onGizmoHandleEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    concludeTransform(pSelected, m_controlWorldRectOnDown);

    m_guides[0]->isVisible = (false);
    m_guides[1]->isVisible = (false);

    m_state = eDocumentState::IDLE;
}

void DocumentView::onGizmoStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    static HCURSOR curSIZEALL = LoadCursor(nullptr, IDC_SIZEALL);
    OWindow->setCursor(curSIZEALL);
    m_state = eDocumentState::MOVING_GIZO;
    m_bStartMoving = false;
    m_mousePosOnDown = {mouseEvent.mousePos.x, mouseEvent.mousePos.y};
    m_controlRectOnDown = pSelected->rect;
    m_controlWorldRectOnDown = pSelected->getWorldRect(*pUIContext);
}

void DocumentView::onGizmoEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    concludeTransform(pSelected, m_controlWorldRectOnDown);

    m_guides[0]->isVisible = (false);
    m_guides[1]->isVisible = (false);

    m_state = eDocumentState::IDLE;
}

void DocumentView::cut()
{
    if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == onut::eUIType::UI_TEXTBOX))
    {
        if (pSelected)
        {
            if (m_pClipBoard) m_pClipBoard->release();
            m_pClipBoard = pSelected;
            if (m_pClipBoard) m_pClipBoard->retain();
            deleteSelection();
        }
    }
}

void DocumentView::copy()
{
    if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == onut::eUIType::UI_TEXTBOX))
    {
        if (pSelected)
        {
            if (m_pClipBoard) m_pClipBoard->release();
            m_pClipBoard = pSelected->copy();
            if (m_pClipBoard) m_pClipBoard->retain();
        }
    }
}

extern void createControlAction(onut::UIControl* pControl, onut::UIControl* pParent);
extern onut::UIControl* getCreateParent();

void DocumentView::paste()
{
    if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == onut::eUIType::UI_TEXTBOX))
    {
        if (m_pClipBoard)
        {
            auto pParent = getCreateParent();
            auto pCtrl = m_pClipBoard->copy();
            createControlAction(pCtrl, pParent);
        }
    }
}

void DocumentView::del()
{
    deleteSelection();
}

void DocumentView::update()
{
    switch (m_state)
    {
        case eDocumentState::MOVING_HANDLE:
            updateMovingHandle();
            break;
        case eDocumentState::MOVING_GIZO:
            updateMovingGizmo();
            break;
    }
}

void DocumentView::deleteSelection()
{
    if (!pSelected) return;
    if (!pSelected->getParent()) return; // huh?
    if (m_state != eDocumentState::IDLE) return;
    if ((g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == onut::eUIType::UI_TEXTBOX)) return;

    auto pParent = pSelected->getParent();
    auto pControl = pSelected;

    g_actionManager.doAction(new onut::Action("Delete",
        [=]{
        pParent->remove(pControl);
        controlDeleted(pControl);
        pSelected = nullptr;
        setSelected(nullptr);
    },
        [=]{
        pParent->add(pControl);
        controlCreated(pControl, pParent);
        setSelected(pControl);
    },
        [=]{
        pParent->retain();
        pControl->retain();
    },
        [=]{
        pParent->release();
        pControl->release();
    }));
}

void DocumentView::updateSelectionWithRect(const onut::sUIRect& rect)
{
    if (!pSelected) return;
    pSelected->setWorldRect(rect, *pUIContext);
    updateGizmoRect();
}

void DocumentView::updateGizmoRect()
{
    auto rct = pSelected->getWorldRect(*pUIContext);
    rct.position.x = std::roundf(rct.position.x);
    rct.position.y = std::roundf(rct.position.y);
    m_pGizmo->rect = (rct);
}

void DocumentView::xAutoGuideAgainst(const onut::sUIRect& otherRect, bool& found, const onut::sUIRect& rect, float& x, bool& side, float& closest)
{
    // Direct align guides
    if (rect.position.x > otherRect.position.x - closest &&
        rect.position.x < otherRect.position.x + closest)
    {
        found = true;
        x = otherRect.position.x;
        closest = std::abs(otherRect.position.x - rect.position.x);
        side = false;
    }
    if (rect.position.x > otherRect.position.x + otherRect.size.x - closest &&
        rect.position.x < otherRect.position.x + otherRect.size.x + closest)
    {
        found = true;
        x = otherRect.position.x + otherRect.size.x;
        closest = std::abs(otherRect.position.x + otherRect.size.x - rect.position.x);
        side = false;
    }
    if (rect.position.x + rect.size.x > otherRect.position.x - closest &&
        rect.position.x + rect.size.x < otherRect.position.x + closest)
    {
        found = true;
        x = otherRect.position.x;
        closest = std::abs(otherRect.position.x - (rect.position.x + rect.size.x));
        side = true;
    }
    if (rect.position.x + rect.size.x > otherRect.position.x + otherRect.size.x - closest &&
        rect.position.x + rect.size.x < otherRect.position.x + otherRect.size.x + closest)
    {
        found = true;
        x = otherRect.position.x + otherRect.size.x;
        closest = std::abs(otherRect.position.x + otherRect.size.x - (rect.position.x + rect.size.x));
        side = true;
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (rect.position.x > otherRect.position.x + otherRect.size.x + m_autoPadding - closest &&
            rect.position.x < otherRect.position.x + otherRect.size.x + m_autoPadding + closest)
        {
            found = true;
            x = otherRect.position.x + otherRect.size.x + m_autoPadding;
            closest = std::abs(otherRect.position.x + otherRect.size.x + m_autoPadding - rect.position.x);
            side = false;
        }
        if (rect.position.x + rect.size.x > otherRect.position.x - m_autoPadding - closest &&
            rect.position.x + rect.size.x < otherRect.position.x - m_autoPadding + closest)
        {
            found = true;
            x = otherRect.position.x - m_autoPadding;
            closest = std::abs((otherRect.position.x - m_autoPadding) - (rect.position.x + rect.size.x));
            side = true;
        }
    }
}

void DocumentView::yAutoGuideAgainst(const onut::sUIRect& otherRect, bool& found, const onut::sUIRect& rect, float& y, bool& side, float& closest)
{
    if (rect.position.y > otherRect.position.y - closest &&
        rect.position.y < otherRect.position.y + closest)
    {
        found = true;
        y = otherRect.position.y;
        closest = std::abs(otherRect.position.y - rect.position.y);
        side = false;
    }
    if (rect.position.y > otherRect.position.y + otherRect.size.y - closest &&
        rect.position.y < otherRect.position.y + otherRect.size.y + closest)
    {
        found = true;
        y = otherRect.position.y + otherRect.size.y;
        closest = std::abs(otherRect.position.y + otherRect.size.y - rect.position.y);
        side = false;
    }
    if (rect.position.y + rect.size.y > otherRect.position.y - closest &&
        rect.position.y + rect.size.y < otherRect.position.y + closest)
    {
        found = true;
        y = otherRect.position.y;
        closest = std::abs(otherRect.position.y - (rect.position.y + rect.size.y));
        side = true;
    }
    if (rect.position.y + rect.size.y > otherRect.position.y + otherRect.size.y - closest &&
        rect.position.y + rect.size.y < otherRect.position.y + otherRect.size.y + closest)
    {
        found = true;
        y = otherRect.position.y + otherRect.size.y;
        closest = std::abs(otherRect.position.y + otherRect.size.y - (rect.position.y + rect.size.y));
        side = true;
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (rect.position.y > otherRect.position.y + otherRect.size.y + m_autoPadding - closest &&
            rect.position.y < otherRect.position.y + otherRect.size.y + m_autoPadding + closest)
        {
            found = true;
            y = otherRect.position.y + otherRect.size.y + m_autoPadding;
            closest = std::abs(otherRect.position.y + otherRect.size.y + m_autoPadding - rect.position.y);
            side = false;
        }
        if (rect.position.y + rect.size.y > otherRect.position.y - m_autoPadding - closest &&
            rect.position.y + rect.size.y < otherRect.position.y - m_autoPadding + closest)
        {
            found = true;
            y = otherRect.position.y - m_autoPadding;
            closest = std::abs((otherRect.position.y - m_autoPadding) - (rect.position.y + rect.size.y));
            side = true;
        }
    }
}

void DocumentView::snapX(float x, float &ret, const onut::sUIRect& rect, float &closest, bool& found)
{
    // Direct align guides
    if (x > rect.position.x - closest &&
        x < rect.position.x + closest)
    {
        found = true;
        ret = rect.position.x;
        closest = std::abs(rect.position.x - x);
    }
    if (x > rect.position.x + rect.size.x - closest &&
        x < rect.position.x + rect.size.x + closest)
    {
        found = true;
        ret = rect.position.x + rect.size.x;
        closest = std::abs(rect.position.x + rect.size.x - x);
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (x > rect.position.x + rect.size.x + m_autoPadding - closest &&
            x < rect.position.x + rect.size.x + m_autoPadding + closest)
        {
            found = true;
            ret = rect.position.x + rect.size.x + m_autoPadding;
            closest = std::abs(rect.position.x + rect.size.x + m_autoPadding - x);
        }
        if (x > rect.position.x - m_autoPadding - closest &&
            x < rect.position.x - m_autoPadding + closest)
        {
            found = true;
            ret = rect.position.x - m_autoPadding;
            closest = std::abs((rect.position.x - m_autoPadding) - x);
        }
    }
}

float DocumentView::snapX(onut::UIControl* pControl, float x)
{
    if (!m_pChkSnap->getIsChecked())
    {
        return x;
    }
    auto pParent = pControl->getParent();
    if (!pParent) return false;
    float closest = 8;
    bool found = false;
    float ret = x;
    for (auto pChild : pParent->getChildren())
    {
        if (pChild == pControl) continue;
        snapX(x, ret, pChild->getWorldRect(*pUIContext), closest, found);
    }
    auto& parentRect = pParent->getWorldRect(*pUIContext);
    snapX(x, ret, {parentRect.position, {0, parentRect.size.y}}, closest, found);
    snapX(x, ret, {{parentRect.position.x + parentRect.size.x, parentRect.position.y}, {0, parentRect.size.y}}, closest, found);

    if (found)
    {
        auto& rect = m_guides[1]->rect;
        m_guides[1]->rect = {{ret, rect.position.y}, rect.size};
        m_guides[1]->isVisible = (true);
    }
    else
    {
        m_guides[1]->isVisible = (false);
    }

    return ret;
}

void DocumentView::snapY(float y, float &ret, const onut::sUIRect& rect, float &closest, bool& found)
{
    // Direct align guides
    if (y > rect.position.y - closest &&
        y < rect.position.y + closest)
    {
        found = true;
        ret = rect.position.y;
        closest = std::abs(rect.position.y - y);
    }
    if (y > rect.position.y + rect.size.y - closest &&
        y < rect.position.y + rect.size.y + closest)
    {
        found = true;
        ret = rect.position.y + rect.size.y;
        closest = std::abs(rect.position.y + rect.size.y - y);
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (y > rect.position.y + rect.size.y + m_autoPadding - closest &&
            y < rect.position.y + rect.size.y + m_autoPadding + closest)
        {
            found = true;
            ret = rect.position.y + rect.size.y + m_autoPadding;
            closest = std::abs(rect.position.y + rect.size.y + m_autoPadding - y);
        }
        if (y > rect.position.y - m_autoPadding - closest &&
            y < rect.position.y - m_autoPadding + closest)
        {
            found = true;
            ret = rect.position.y - m_autoPadding;
            closest = std::abs((rect.position.y - m_autoPadding) - y);
        }
    }
}

float DocumentView::snapY(onut::UIControl* pControl, float y)
{
    if (!m_pChkSnap->getIsChecked())
    {
        return y;
    }
    auto pParent = pControl->getParent();
    if (!pParent) return false;
    float closest = 8;
    bool found = false;
    float ret = y;
    for (auto pChild : pParent->getChildren())
    {
        if (pChild == pControl) continue;
        snapY(y, ret, pChild->getWorldRect(*pUIContext), closest, found);
    }
    auto& parentRect = pParent->getWorldRect(*pUIContext);
    snapY(y, ret, {parentRect.position, {parentRect.size.x, 0}}, closest, found);
    snapY(y, ret, {{parentRect.position.x, parentRect.position.y + parentRect.size.y}, {parentRect.size.x, 0}}, closest, found);

    if (found)
    {
        auto& rect = m_guides[0]->rect;
        m_guides[0]->rect = {{rect.position.x, ret}, rect.size};
        m_guides[0]->isVisible = (true);
    }
    else
    {
        m_guides[0]->isVisible = (false);
    }

    return ret;
}

bool DocumentView::getXAutoGuide(const onut::sUIRect& rect, float& x, bool& side)
{
    auto pParent = pSelected->getParent();
    if (!pParent) return false;
    float closest = 8;
    bool found = false;
    for (auto pChild : pParent->getChildren())
    {
        if (pChild == pSelected) continue;
        xAutoGuideAgainst(pChild->getWorldRect(*pUIContext), found, rect, x, side, closest);
    }
    auto& parentRect = pParent->getWorldRect(*pUIContext);
    xAutoGuideAgainst({parentRect.position, {0, parentRect.size.y}}, found, rect, x, side, closest);
    xAutoGuideAgainst({{parentRect.position.x + parentRect.size.x, parentRect.position.y}, {0, parentRect.size.y}}, found, rect, x, side, closest);
    return found;
}

bool DocumentView::getYAutoGuide(const onut::sUIRect& rect, float& y, bool& side)
{
    auto pParent = pSelected->getParent();
    if (!pParent) return false;
    float closest = 8;
    bool found = false;
    for (auto pChild : pParent->getChildren())
    {
        if (pChild == pSelected) continue;
        yAutoGuideAgainst(pChild->getWorldRect(*pUIContext), found, rect, y, side, closest);
    }
    auto& parentRect = pParent->getWorldRect(*pUIContext);
    yAutoGuideAgainst({parentRect.position, {parentRect.size.x, 0}}, found, rect, y, side, closest);
    yAutoGuideAgainst({{parentRect.position.x, parentRect.position.y + parentRect.size.y}, {parentRect.size.x, 0}}, found, rect, y, side, closest);
    return found;
}

void DocumentView::onKeyDown(uintptr_t key)
{
    if (pSelected)
    {
        if (key == VK_LEFT || key == VK_RIGHT || key == VK_UP || key == VK_DOWN)
        {
            if (!dynamic_cast<onut::UITextBox*>(g_pUIContext->getFocusControl()))
            {
                auto newRect = pSelected->getWorldRect(*pUIContext);
                float speed = (OInput->isStateDown(DIK_LSHIFT) || OInput->isStateDown(DIK_RSHIFT)) ? 10.f : 1.f;
                switch (key)
                {
                    case VK_LEFT:
                        newRect.position.x -= speed;
                        break;
                    case VK_RIGHT:
                        newRect.position.x += speed;
                        break;
                    case VK_UP:
                        newRect.position.y -= speed;
                        break;
                    case VK_DOWN:
                        newRect.position.y += speed;
                        break;
                }
                updateSelectionWithRect(newRect);
                updateInspector();
            }
        }
    }
}

void DocumentView::updateMovingGizmo()
{
    auto mouseDiff = OMousePos - m_mousePosOnDown;
    if (!m_bStartMoving)
    {
        if (fabsf(mouseDiff.x) < 3 && fabsf(mouseDiff.y) < 3) return;
        m_bStartMoving = true;
    }
    if (OInput->isStateDown(DIK_LSHIFT))
    {
        if (std::abs(mouseDiff.x) >= std::abs(mouseDiff.y))
        {
            mouseDiff.y = 0;
        }
        else
        {
            mouseDiff.x = 0;
        }
    }
    auto newRect = m_controlWorldRectOnDown;
    newRect.position.x += mouseDiff.x;
    newRect.position.y += mouseDiff.y;

    if (m_pChkSnap->getIsChecked() && pSelected->getParent())
    {
        // Auto snap the rect to it's brothers
        float x, y;
        bool side;
        auto guideRect = m_controlWorldRectOnDown;
        guideRect.position.x += mouseDiff.x;
        guideRect.position.y += mouseDiff.y;
        if (getXAutoGuide(guideRect, x, side))
        {
            float finalOffset;
            if (!side)
            {
                finalOffset = x - m_controlWorldRectOnDown.position.x;
            }
            else
            {
                finalOffset = (x - m_controlWorldRectOnDown.size.x) - m_controlWorldRectOnDown.position.x;
            }
            newRect.position.x = m_controlWorldRectOnDown.position.x + finalOffset;
            auto& rect = m_guides[1]->rect;
            m_guides[1]->rect = {{x, rect.position.y}, rect.size};
            m_guides[1]->isVisible = (true);
        }
        else
        {
            m_guides[1]->isVisible = (false);
        }
        if (getYAutoGuide(guideRect, y, side))
        {
            float finalOffset;
            if (!side)
            {
                finalOffset = y - m_controlWorldRectOnDown.position.y;
            }
            else
            {
                finalOffset = (y - m_controlWorldRectOnDown.size.y) - m_controlWorldRectOnDown.position.y;
            }
            newRect.position.y = m_controlWorldRectOnDown.position.y + finalOffset;
            auto& rect = m_guides[0]->rect;
            m_guides[0]->rect = {{rect.position.x, y}, rect.size};
            m_guides[0]->isVisible = (true);
        }
        else
        {
            m_guides[0]->isVisible = (false);
        }
    }

    updateSelectionWithRect(newRect);
    updateInspector();
}

void DocumentView::updateMovingHandle()
{
    auto mouseDiff = OMousePos - m_mousePosOnDown;
    if (!m_bStartMoving)
    {
        if (fabsf(mouseDiff.x) < 3 && fabsf(mouseDiff.y) < 3) return;
        m_bStartMoving = true;
    }
    auto newRect = m_controlWorldRectOnDown;
    Vector2 anchor{0, 0};
    Vector2 invAnchor{1, 1};

    // Grow/Shrink on both side equally
    if (OInput->isStateDown(DIK_LSHIFT) && (
            m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[2] || 
            m_pCurrentHandle == m_gizmoHandles[5] || 
            m_pCurrentHandle == m_gizmoHandles[7]))
    {
        bool xDominate = true;
        float ratio = m_controlWorldRectOnDown.size.x / m_controlWorldRectOnDown.size.y;
        if (ratio < 1.f)
        {
            ratio = m_controlWorldRectOnDown.size.y / m_controlWorldRectOnDown.size.x;
            xDominate = false;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.position.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.position.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y;
            newMouseDiffX = newMouseDiffY = -std::max<>(-newMouseDiffX, -newMouseDiffY);
            if (xDominate)
            {
                if (newMouseDiffX / newMouseDiffY < ratio)
                {
                    newMouseDiffY = newMouseDiffX / ratio;
                }
                else if (newMouseDiffX / newMouseDiffY > ratio)
                {
                    newMouseDiffX = newMouseDiffY / ratio;
                }
            }
            else
            {
                if (newMouseDiffY / newMouseDiffX < ratio)
                {
                    newMouseDiffX = newMouseDiffY / ratio;
                }
                else if (newMouseDiffY / newMouseDiffX > ratio)
                {
                    newMouseDiffY = newMouseDiffX / ratio;
                }
            }
            if (OInput->isStateDown(DIK_LALT))
            {
                newRect.position.x += newMouseDiffX * invAnchor.x - newMouseDiffX * anchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x * .5f);
                newRect.size.x -= newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y - newMouseDiffY * anchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y * .5f);
                newRect.size.y -= newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * invAnchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x);
                newRect.size.x -= newMouseDiffX;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y);
                newRect.size.y -= newMouseDiffY;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x - m_controlWorldRectOnDown.size.x;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.position.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y;
            newMouseDiffX = std::max<>(newMouseDiffX, -newMouseDiffY);
            newMouseDiffY = -newMouseDiffX;
            if (xDominate)
            {
                if (newMouseDiffX / -newMouseDiffY < ratio)
                {
                    newMouseDiffY = -newMouseDiffX / ratio;
                }
                else if (newMouseDiffX / -newMouseDiffY > ratio)
                {
                    newMouseDiffX = -newMouseDiffY / ratio;
                }
            }
            else
            {
                if (-newMouseDiffY / newMouseDiffX < ratio)
                {
                    newMouseDiffX = -newMouseDiffY / ratio;
                }
                else if (-newMouseDiffY / newMouseDiffX > ratio)
                {
                    newMouseDiffY = -newMouseDiffX / ratio;
                }
            }
            if (OInput->isStateDown(DIK_LALT))
            {
                newRect.position.x += newMouseDiffX * anchor.x - newMouseDiffX * invAnchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x * .5f);
                newRect.size.x += newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y - newMouseDiffY * anchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y * .5f);
                newRect.size.y -= newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * anchor.x;
                newRect.size.x += newMouseDiffX;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y);
                newRect.size.y -= newMouseDiffY;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.position.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y - m_controlWorldRectOnDown.size.y;
            newMouseDiffY = std::max<>(-newMouseDiffX, newMouseDiffY);
            newMouseDiffX = -newMouseDiffY;
            if (xDominate)
            {
                if (-newMouseDiffX / newMouseDiffY < ratio)
                {
                    newMouseDiffY = -newMouseDiffX / ratio;
                }
                else if (-newMouseDiffX / newMouseDiffY > ratio)
                {
                    newMouseDiffX = -newMouseDiffY / ratio;
                }
            }
            else
            {
                if (newMouseDiffY / -newMouseDiffX < ratio)
                {
                    newMouseDiffX = -newMouseDiffY / ratio;
                }
                else if (newMouseDiffY / -newMouseDiffX > ratio)
                {
                    newMouseDiffY = -newMouseDiffX / ratio;
                }
            }
            if (OInput->isStateDown(DIK_LALT))
            {
                newRect.position.x += newMouseDiffX * invAnchor.x - newMouseDiffX * anchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x * .5f);
                newRect.size.x -= newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * anchor.y - newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y * .5f);
                newRect.size.y += newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * invAnchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x);
                newRect.size.x -= newMouseDiffX;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * anchor.y;
                newRect.size.y += newMouseDiffY;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
        }
        else if (m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x - m_controlWorldRectOnDown.size.x;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y - m_controlWorldRectOnDown.size.y;
            newMouseDiffX = newMouseDiffY = std::max<>(newMouseDiffX, newMouseDiffY);
            if (xDominate)
            {
                if (newMouseDiffX / newMouseDiffY < ratio)
                {
                    newMouseDiffY = newMouseDiffX / ratio;
                }
                else if (newMouseDiffX / newMouseDiffY > ratio)
                {
                    newMouseDiffX = newMouseDiffY / ratio;
                }
            }
            else
            {
                if (newMouseDiffY / newMouseDiffX < ratio)
                {
                    newMouseDiffX = newMouseDiffY / ratio;
                }
                else if (newMouseDiffY / newMouseDiffX > ratio)
                {
                    newMouseDiffY = newMouseDiffX / ratio;
                }
            }
            if (OInput->isStateDown(DIK_LALT))
            {
                newRect.position.x += newMouseDiffX * anchor.x - newMouseDiffX * invAnchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x * .5f);
                newRect.size.x += newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * anchor.y - newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y * .5f);
                newRect.size.y += newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * anchor.x;
                newRect.size.x += newMouseDiffX;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * anchor.y;
                newRect.size.y += newMouseDiffY;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
        }
    }
    else if (OInput->isStateDown(DIK_LALT))
    {
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[3] ||
            m_pCurrentHandle == m_gizmoHandles[5])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.position.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x;
            newRect.position.x += newMouseDiff * invAnchor.x - newMouseDiff * anchor.x;
            newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x * .5f);
            newRect.size.x -= newMouseDiff * 2;
            if (newRect.size.x < 0) newRect.size.x = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2] ||
                    m_pCurrentHandle == m_gizmoHandles[4] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x - m_controlWorldRectOnDown.size.x;
            newRect.position.x += newMouseDiff * anchor.x - newMouseDiff * invAnchor.x;
            newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x * .5f);
            newRect.size.x += newMouseDiff * 2;
            if (newRect.size.x < 0) newRect.size.x = 0;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[1] ||
            m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.position.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y;
            newRect.position.y += newMouseDiff * invAnchor.y - newMouseDiff * anchor.y;
            newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y * .5f);
            newRect.size.y -= newMouseDiff * 2;
            if (newRect.size.y < 0) newRect.size.y = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5] ||
                    m_pCurrentHandle == m_gizmoHandles[6] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y - m_controlWorldRectOnDown.size.y;
            newRect.position.y += newMouseDiff * anchor.y - newMouseDiff * invAnchor.y;
            newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y * .5f);
            newRect.size.y += newMouseDiff * 2;
            if (newRect.size.y < 0) newRect.size.y = 0;
        }
    }
    else // Normal sizing
    {
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[3] ||
            m_pCurrentHandle == m_gizmoHandles[5])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.position.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x;
            newRect.position.x += newMouseDiff * invAnchor.x;
            newRect.position.x = std::min<float>(newRect.position.x, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x);
            newRect.size.x -= newMouseDiff;
            if (newRect.size.x < 0) newRect.size.x = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2] ||
                    m_pCurrentHandle == m_gizmoHandles[4] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x - m_controlWorldRectOnDown.size.x;
            newRect.position.x += newMouseDiff * anchor.x;
            newRect.size.x += newMouseDiff;
            if (newRect.size.x < 0) newRect.size.x = 0;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[1] ||
            m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.position.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y;
            newRect.position.y += newMouseDiff * invAnchor.y;
            newRect.position.y = std::min<float>(newRect.position.y, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y);
            newRect.size.y -= newMouseDiff;
            if (newRect.size.y < 0) newRect.size.y = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5] ||
                    m_pCurrentHandle == m_gizmoHandles[6] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y - m_controlWorldRectOnDown.size.y;
            newRect.position.y += newMouseDiff * anchor.y;
            newRect.size.y += newMouseDiff;
            if (newRect.size.y < 0) newRect.size.y = 0;
        }
    }

    updateSelectionWithRect(newRect);
    updateInspector();
}

void DocumentView::render()
{
    auto rect = pUIScreen->getWorldRect(*pUIContext);

    pUIScreen->render(*pUIContext);
}

void DocumentView::repopulateTreeView(onut::UIControl* pControl)
{
    auto pItem = new onut::UITreeViewItem();
    pItem->pUserData = pControl;
    pItem->text = pControl->name;
    pControl->pUserData = pItem;
    auto pParentItem = static_cast<onut::UITreeViewItem*>(pControl->getParent() ? pControl->getParent()->pUserData : nullptr);
    if (pParentItem)
    {
        pParentItem->addItem(pItem);
    }
    else
    {
        m_pSceneGraph->addItem(pItem);
    }
    for (auto& pChild : pControl->getChildren())
    {
        repopulateTreeView(pChild);
    }
}

std::unordered_map<onut::eUIType, onut::UIControl*> g_controlInspectorMap;
std::unordered_map<onut::eUIType, std::vector<IControlInspectorBind*>> g_inspectorBindings;
void updateControlInspector(onut::UIControl* pControl)
{
    for (auto& kv : g_controlInspectorMap)
    {
        kv.second->isVisible = (false);
    }
    if (!pControl) return;
    auto& it = g_controlInspectorMap.find(pControl->getType());
    if (it == g_controlInspectorMap.end()) return;
    it->second->isVisible = (true);

    auto& it2 = g_inspectorBindings.find(pControl->getType());
    if (it2 == g_inspectorBindings.end()) return;
    auto& bindings = it2->second;
    for (auto binding : bindings)
    {
        binding->updateInspector(pControl);
    }
}

void DocumentView::updateInspector()
{
    if (pSelected)
    {
        // Enable controls
        g_pInspector_UIControl_chkEnabled->isEnabled = (true);
        g_pInspector_UIControl_chkVisible->isEnabled = (true);
        g_pInspector_UIControl_chkClickThrough->isEnabled = (true);
        g_pInspector_UIControl_chkClipChildren->isEnabled = (true);
        g_pInspector_UIControl_txtName->isEnabled = (true);
        g_pInspector_UIControl_txtStyle->isEnabled = (true);
        g_pInspector_UIControl_txtX->isEnabled = (true);
        g_pInspector_UIControl_txtY->isEnabled = (true);
        g_pInspector_UIControl_chkXPercent->isEnabled = (true);
        g_pInspector_UIControl_chkYPercent->isEnabled = (true);
        g_pInspector_UIControl_txtWidth->isEnabled = (true);
        g_pInspector_UIControl_txtHeight->isEnabled = (true);
        g_pInspector_UIControl_chkWidthPercent->isEnabled = (true);
        g_pInspector_UIControl_chkHeightPercent->isEnabled = (true);
        g_pInspector_UIControl_chkWidthRelative->isEnabled = (true);
        g_pInspector_UIControl_chkHeightRelative->isEnabled = (true);
        g_pInspector_UIControl_chkTOP_LEFT->isEnabled = (true);
        g_pInspector_UIControl_chkTOP->isEnabled = (true);
        g_pInspector_UIControl_chkTOP_RIGHT->isEnabled = (true);
        g_pInspector_UIControl_chkLEFT->isEnabled = (true);
        g_pInspector_UIControl_chkCENTER->isEnabled = (true);
        g_pInspector_UIControl_chkRIGHT->isEnabled = (true);
        g_pInspector_UIControl_chkBOTTOM_LEFT->isEnabled = (true);
        g_pInspector_UIControl_chkBOTTOM->isEnabled = (true);
        g_pInspector_UIControl_chkBOTTOM_RIGHT->isEnabled = (true);
        g_pInspector_UIControl_txtAnchorX->isEnabled = (true);
        g_pInspector_UIControl_txtAnchorY->isEnabled = (true);
        g_pInspector_UIControl_chkXAnchorPercent->isEnabled = (true);
        g_pInspector_UIControl_chkYAnchorPercent->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorTOP_LEFT->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorTOP->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorTOP_RIGHT->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorLEFT->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorCENTER->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorRIGHT->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorBOTTOM_LEFT->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorBOTTOM->isEnabled = (true);
        g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT->isEnabled = (true);

        // Set their values
        g_pInspector_UIControl_chkEnabled->setIsChecked(pSelected->isEnabled);
        g_pInspector_UIControl_chkVisible->setIsChecked(pSelected->isVisible);
        g_pInspector_UIControl_chkClickThrough->setIsChecked(pSelected->isClickThrough);
        g_pInspector_UIControl_chkClipChildren->setIsChecked(pSelected->clipChildren);
        g_pInspector_UIControl_txtName->textComponent.text = pSelected->name;
        g_pInspector_UIControl_txtStyle->textComponent.text = pSelected->getStyleName();
        if (pSelected->xType == onut::eUIPosType::POS_PERCENTAGE)
        {
            g_pInspector_UIControl_txtX->setFloat(pSelected->rect.position.x * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtX->setFloat(pSelected->rect.position.x);
        }
        if (pSelected->yType == onut::eUIPosType::POS_PERCENTAGE)
        {
            g_pInspector_UIControl_txtY->setFloat(pSelected->rect.position.y * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtY->setFloat(pSelected->rect.position.y);
        }
        g_pInspector_UIControl_chkXPercent->setIsChecked(pSelected->xType == onut::eUIPosType::POS_PERCENTAGE);
        g_pInspector_UIControl_chkYPercent->setIsChecked(pSelected->yType == onut::eUIPosType::POS_PERCENTAGE);
        if (pSelected->widthType == onut::eUIDimType::DIM_PERCENTAGE)
        {
            g_pInspector_UIControl_txtWidth->setFloat(pSelected->rect.size.x * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtWidth->setFloat(pSelected->rect.size.x);
        }
        if (pSelected->heightType == onut::eUIDimType::DIM_PERCENTAGE)
        {
            g_pInspector_UIControl_txtHeight->setFloat(pSelected->rect.size.y * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtHeight->setFloat(pSelected->rect.size.y);
        }
        g_pInspector_UIControl_chkWidthPercent->setIsChecked(pSelected->widthType == onut::eUIDimType::DIM_PERCENTAGE);
        g_pInspector_UIControl_chkHeightPercent->setIsChecked(pSelected->heightType == onut::eUIDimType::DIM_PERCENTAGE);
        g_pInspector_UIControl_chkWidthRelative->setIsChecked(pSelected->widthType == onut::eUIDimType::DIM_RELATIVE);
        g_pInspector_UIControl_chkHeightRelative->setIsChecked(pSelected->heightType == onut::eUIDimType::DIM_RELATIVE);
        switch (pSelected->align)
        {
            case onut::eUIAlign::TOP_LEFT:
                g_pInspector_UIControl_chkTOP_LEFT->setIsChecked(true);
                break;
            case onut::eUIAlign::TOP:
                g_pInspector_UIControl_chkTOP->setIsChecked(true);
                break;
            case onut::eUIAlign::TOP_RIGHT:
                g_pInspector_UIControl_chkTOP_RIGHT->setIsChecked(true);
                break;
            case onut::eUIAlign::LEFT:
                g_pInspector_UIControl_chkLEFT->setIsChecked(true);
                break;
            case onut::eUIAlign::CENTER:
                g_pInspector_UIControl_chkCENTER->setIsChecked(true);
                break;
            case onut::eUIAlign::RIGHT:
                g_pInspector_UIControl_chkRIGHT->setIsChecked(true);
                break;
            case onut::eUIAlign::BOTTOM_LEFT:
                g_pInspector_UIControl_chkBOTTOM_LEFT->setIsChecked(true);
                break;
            case onut::eUIAlign::BOTTOM:
                g_pInspector_UIControl_chkBOTTOM->setIsChecked(true);
                break;
            case onut::eUIAlign::BOTTOM_RIGHT:
                g_pInspector_UIControl_chkBOTTOM_RIGHT->setIsChecked(true);
                break;
        }
        if (pSelected->xAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
        {
            g_pInspector_UIControl_txtAnchorX->setFloat(pSelected->anchor.x * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtAnchorX->setFloat(pSelected->anchor.x);
        }
        if (pSelected->yAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
        {
            g_pInspector_UIControl_txtAnchorY->setFloat(pSelected->anchor.y * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtAnchorY->setFloat(pSelected->anchor.y);
        }
        g_pInspector_UIControl_chkXAnchorPercent->setIsChecked(pSelected->xAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE);
        g_pInspector_UIControl_chkYAnchorPercent->setIsChecked(pSelected->yAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE);
    }
    else
    {
        // Disable all controls
        g_pInspector_UIControl_chkEnabled->isEnabled = (false);
        g_pInspector_UIControl_chkVisible->isEnabled = (false);
        g_pInspector_UIControl_chkClickThrough->isEnabled = (false);
        g_pInspector_UIControl_chkClipChildren->isEnabled = (false);
        g_pInspector_UIControl_txtName->isEnabled = (false);
        g_pInspector_UIControl_txtStyle->isEnabled = (false);
        g_pInspector_UIControl_txtX->isEnabled = (false);
        g_pInspector_UIControl_txtY->isEnabled = (false);
        g_pInspector_UIControl_chkXPercent->isEnabled = (false);
        g_pInspector_UIControl_chkYPercent->isEnabled = (false);
        g_pInspector_UIControl_txtWidth->isEnabled = (false);
        g_pInspector_UIControl_txtHeight->isEnabled = (false);
        g_pInspector_UIControl_chkWidthPercent->isEnabled = (false);
        g_pInspector_UIControl_chkHeightPercent->isEnabled = (false);
        g_pInspector_UIControl_chkWidthRelative->isEnabled = (false);
        g_pInspector_UIControl_chkHeightRelative->isEnabled = (false);
        g_pInspector_UIControl_chkTOP_LEFT->isEnabled = (false);
        g_pInspector_UIControl_chkTOP->isEnabled = (false);
        g_pInspector_UIControl_chkTOP_RIGHT->isEnabled = (false);
        g_pInspector_UIControl_chkLEFT->isEnabled = (false);
        g_pInspector_UIControl_chkCENTER->isEnabled = (false);
        g_pInspector_UIControl_chkRIGHT->isEnabled = (false);
        g_pInspector_UIControl_chkBOTTOM_LEFT->isEnabled = (false);
        g_pInspector_UIControl_chkBOTTOM->isEnabled = (false);
        g_pInspector_UIControl_chkBOTTOM_RIGHT->isEnabled = (false);
        g_pInspector_UIControl_txtAnchorX->isEnabled = (false);
        g_pInspector_UIControl_txtAnchorY->isEnabled = (false);
        g_pInspector_UIControl_chkXAnchorPercent->isEnabled = (false);
        g_pInspector_UIControl_chkYAnchorPercent->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorTOP_LEFT->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorTOP->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorTOP_RIGHT->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorLEFT->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorCENTER->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorRIGHT->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorBOTTOM_LEFT->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorBOTTOM->isEnabled = (false);
        g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT->isEnabled = (false);
    }

    updateControlInspector(pSelected);
}

void DocumentView::setDirty(bool isDirty)
{
    m_isDirty = isDirty;
    auto window = OWindow->getHandle();
    if (m_isDirty)
    {
        if (m_filename.empty())
        {
            SetWindowTextA(window, (OSettings->getGameName() + " - Untitle*").c_str());
        }
        else
        {
            SetWindowTextA(window, (OSettings->getGameName() + " - " + m_filename + "*").c_str());
        }
    }
    else
    {
        if (m_filename.empty())
        {
            SetWindowTextA(window, (OSettings->getGameName() + " - Untitle").c_str()); // That shouldn't be possible
        }
        else
        {
            SetWindowTextA(window, (OSettings->getGameName() + " - " + m_filename).c_str());
        }
    }
}

void DocumentView::save()
{
    if (!m_filename.empty())
    {
        pUIScreen->save(m_filename);
        setDirty(false);
    }
}

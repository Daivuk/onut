#include "DocumentView.h"
#include "viewStyles.h"
#include "styles.h"

#include "onut/ActionManager.h"
#include "onut/Input.h"
#include "onut/Files.h"
#include "onut/Settings.h"
#include "onut/SpriteBatch.h"
#include "onut/UIButton.h"
#include "onut/Window.h"

extern OUIControlRef g_pUIScreen;
extern OUIContextRef g_pUIContext;

extern OUICheckBoxRef g_pInspector_UIControl_chkEnabled;
extern OUICheckBoxRef g_pInspector_UIControl_chkVisible;
extern OUICheckBoxRef g_pInspector_UIControl_chkClickThrough;
extern OUICheckBoxRef g_pInspector_UIControl_chkClipChildren;
extern OUITextBoxRef g_pInspector_UIControl_txtName;
extern OUITextBoxRef g_pInspector_UIControl_txtStyle;
extern OUITextBoxRef g_pInspector_UIControl_txtX;
extern OUITextBoxRef g_pInspector_UIControl_txtY;
extern OUICheckBoxRef g_pInspector_UIControl_chkXPercent;
extern OUICheckBoxRef g_pInspector_UIControl_chkYPercent;
extern OUITextBoxRef g_pInspector_UIControl_txtWidth;
extern OUITextBoxRef g_pInspector_UIControl_txtHeight;
extern OUICheckBoxRef g_pInspector_UIControl_chkWidthPercent;
extern OUICheckBoxRef g_pInspector_UIControl_chkHeightPercent;
extern OUICheckBoxRef g_pInspector_UIControl_chkWidthRelative;
extern OUICheckBoxRef g_pInspector_UIControl_chkHeightRelative;
extern OUICheckBoxRef g_pInspector_UIControl_chkTOP_LEFT;
extern OUICheckBoxRef g_pInspector_UIControl_chkTOP;
extern OUICheckBoxRef g_pInspector_UIControl_chkTOP_RIGHT;
extern OUICheckBoxRef g_pInspector_UIControl_chkLEFT;
extern OUICheckBoxRef g_pInspector_UIControl_chkCENTER;
extern OUICheckBoxRef g_pInspector_UIControl_chkRIGHT;
extern OUICheckBoxRef g_pInspector_UIControl_chkBOTTOM_LEFT;
extern OUICheckBoxRef g_pInspector_UIControl_chkBOTTOM;
extern OUICheckBoxRef g_pInspector_UIControl_chkBOTTOM_RIGHT;
extern OUITextBoxRef g_pInspector_UIControl_txtAnchorX;
extern OUITextBoxRef g_pInspector_UIControl_txtAnchorY;
extern OUICheckBoxRef g_pInspector_UIControl_chkXAnchorPercent;
extern OUICheckBoxRef g_pInspector_UIControl_chkYAnchorPercent;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorTOP_LEFT;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorTOP;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorTOP_RIGHT;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorLEFT;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorCENTER;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorRIGHT;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorBOTTOM_LEFT;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorBOTTOM;
extern OUIButtonRef g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT;

static HCURSOR curARROW = nullptr;
static HCURSOR curSIZENWSE = nullptr;
static HCURSOR curSIZENESW = nullptr;
static HCURSOR curSIZEWE = nullptr;
static HCURSOR curSIZENS = nullptr;
static HCURSOR curSIZEALL = nullptr;

static const Color g_panelBGColor = OColorHex(2d2d30);
static const Color g_panelTitleBGColor = OColorHex(3f3f46);

DocumentView::DocumentView(const std::string& filename)
{
    pContentManager = OContentManager::create();
    pContentManager->clearSearchPaths();

    m_filename = filename;
    auto path = onut::getPath(filename);
    if (!path.empty())
    {
        pContentManager->addSearchPath(path);
        pContentManager->addSearchPath(path + "/../");
    }

    if (!curARROW) curARROW = LoadCursor(nullptr, IDC_ARROW);
    if (!curSIZENWSE) curSIZENWSE = LoadCursor(nullptr, IDC_SIZENWSE);
    if (!curSIZENESW) curSIZENESW = LoadCursor(nullptr, IDC_SIZENESW);
    if (!curSIZEWE) curSIZEWE = LoadCursor(nullptr, IDC_SIZEWE);
    if (!curSIZENS) curSIZENS = LoadCursor(nullptr, IDC_SIZENS);
    if (!curSIZEALL) curSIZEALL = LoadCursor(nullptr, IDC_SIZEALL);

    pUIContextUserStyle = onut::UIContext::create(Vector2{800, 600});
    pUIContextEditorStyle = onut::UIContext::create(Vector2{800, 600});
    pUIContextUserStyle->pContentManager = pContentManager;
    createViewUIStyles(pUIContextUserStyle);
    createUIStyles(pUIContextEditorStyle);

    pUIContextEditorStyle->addStyle<onut::UIPanel>("", [](const OUIPanelRef pPanel, const Rect& rect)
    {
        oSpriteBatch->drawRect(nullptr, (rect), g_panelTitleBGColor);
        oSpriteBatch->drawRect(nullptr, (rect).Grow(-1), g_panelBGColor);
    });

    // We don't render controls
    //pUIContextEditorStyle->addStyle<onut::UIControl>("", [](const OUIControlRef pPanel, const Rect& rect)
    //{
    //    oSpriteBatch->drawRect(nullptr, {rect.x, rect.y, rect.z, 1}, g_panelTitleBGColor);
    //    oSpriteBatch->drawRect(nullptr, {rect.x, rect.y, 1, rect.w}, g_panelTitleBGColor);
    //    oSpriteBatch->drawRect(nullptr, {rect.x, rect.y + rect.w, rect.z, 1}, g_panelTitleBGColor);
    //    oSpriteBatch->drawRect(nullptr, {rect.x + rect.z, rect.y, 1, rect.w}, g_panelTitleBGColor);
    //});

    pUIContext = pUIContextUserStyle;
    
    if (m_filename.empty())
    {
        pUIScreen = onut::UIControl::create(/*"../../assets/ui/editor.json"*/);
    }
    else
    {
        pUIScreen = onut::UIControl::createFromFile(m_filename.c_str(), pContentManager);
    }
    pUIScreen->name = "Root";
    pUIScreen->widthType = OUIControl::DimType::Relative;
    pUIScreen->heightType = OUIControl::DimType::Relative;

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
    m_pGizmo->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZEALL); };
    m_pGizmo->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    for (auto& pGizmoHandle : m_gizmoHandles)
    {
        pGizmoHandle->onMouseDown = std::bind(&DocumentView::onGizmoHandleStart, this, std::placeholders::_1, std::placeholders::_2);
        pGizmoHandle->onMouseUp = std::bind(&DocumentView::onGizmoHandleEnd, this, std::placeholders::_1, std::placeholders::_2);
        pGizmoHandle->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    }
    m_gizmoHandles[0]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZENWSE); };
    m_gizmoHandles[1]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZENS); };
    m_gizmoHandles[2]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZENESW); };
    m_gizmoHandles[3]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZEWE); };
    m_gizmoHandles[4]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZEWE); };
    m_gizmoHandles[5]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZENESW); };
    m_gizmoHandles[6]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZENS); };
    m_gizmoHandles[7]->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curSIZENWSE); };

    // Add some dummy nodes in the scene graph for show
    m_pSceneGraph = g_pUIScreen->getChild<onut::UITreeView>("sceneGraph");
    m_pSceneGraph->clear();
    repopulateTreeView(pUIScreen);

    m_pChkSnap = g_pUIScreen->getChild<onut::UICheckBox>("chkSnap");
    m_pChkSnapMargins = g_pUIScreen->getChild<onut::UICheckBox>("chkSnapMargins");
    m_pTxtSnapMargins = g_pUIScreen->getChild<onut::UITextBox>("txtSnapMargins");
    m_autoPadding = m_pTxtSnapMargins->getFloat();
    m_pTxtSnapMargins->onTextChanged = [this](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
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
}

void DocumentView::controlCreated(const OUIControlRef& pControl, const OUIControlRef& pParent)
{
    repopulateTreeView(pControl);
}

void DocumentView::controlDeleted(const OUIControlRef& pControl)
{
    auto pItem = OStaticCast<OUITreeViewItem>(pControl->pSharedUserData);
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
        m_pGizmo->rect = (pSelected->getWorldRect(pUIContext));
    }
}

void DocumentView::setSelected(const OUIControlRef& in_pSelected, bool bUpdateSceneGraph)
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
            auto pItem = OStaticCast<OUITreeViewItem>(pSelected->pSharedUserData);
            m_pSceneGraph->addSelectedItem(pItem);
        }
    }
    updateInspector();
}

void DocumentView::onGizmoHandleStart(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent)
{
    m_state = eDocumentState::MOVING_HANDLE;
    m_bStartMoving = false;
    m_mousePosOnDown = {mouseEvent.mousePos.x, mouseEvent.mousePos.y};
    m_pCurrentHandle = pControl;
    m_controlRectOnDown = pSelected->rect;
    m_controlWorldRectOnDown = pSelected->getWorldRect(pUIContext);
}

void DocumentView::concludeTransform(const OUIControlRef& pControl, const Rect& previousRect)
{
    auto worldRect = pControl->getWorldRect(pUIContext);
    if (worldRect.x != previousRect.x ||
        worldRect.y != previousRect.y ||
        worldRect.z != previousRect.z ||
        worldRect.w != previousRect.w)
    {
        oActionManager->doAction(OMake<onut::Action>("Transform",
            [=]{
            pControl->setWorldRect(worldRect, pUIContext);
            updateSelectionWithRect(worldRect);
            updateInspector();
        },
            [=]{
            pControl->setWorldRect(previousRect, pUIContext);
            updateSelectionWithRect(previousRect);
            updateInspector();
        }));
    }
}

void DocumentView::onGizmoHandleEnd(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent)
{
    concludeTransform(pSelected, m_controlWorldRectOnDown);

    m_guides[0]->isVisible = (false);
    m_guides[1]->isVisible = (false);

    m_state = eDocumentState::IDLE;
}

void DocumentView::onGizmoStart(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent)
{
    static HCURSOR curSIZEALL = LoadCursor(nullptr, IDC_SIZEALL);
    oWindow->setCursor(curSIZEALL);
    m_state = eDocumentState::MOVING_GIZO;
    m_bStartMoving = false;
    m_mousePosOnDown = {mouseEvent.mousePos.x, mouseEvent.mousePos.y};
    m_controlRectOnDown = pSelected->rect;
    m_controlWorldRectOnDown = pSelected->getWorldRect(pUIContext);
}

void DocumentView::onGizmoEnd(const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent)
{
    concludeTransform(pSelected, m_controlWorldRectOnDown);

    m_guides[0]->isVisible = (false);
    m_guides[1]->isVisible = (false);

    m_state = eDocumentState::IDLE;
}

void DocumentView::cut()
{
    if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == OUIControl::Type::TextBox))
    {
        if (pSelected)
        {
            m_pClipBoard = pSelected;
            deleteSelection();
        }
    }
}

void DocumentView::copy()
{
    if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == OUIControl::Type::TextBox))
    {
        if (pSelected)
        {
            m_pClipBoard = pSelected->copy();
        }
    }
}

extern void createControlAction(const OUIControlRef& pControl, OUIControlRef pParent);
extern OUIControlRef getCreateParent();

void DocumentView::paste()
{
    if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == OUIControl::Type::TextBox))
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
    if ((g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == OUIControl::Type::TextBox)) return;

    auto pParent = pSelected->getParent();
    auto pControl = pSelected;

    oActionManager->doAction(OMake<onut::Action>("Delete",
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
    }));
}

void DocumentView::updateSelectionWithRect(const Rect& rect)
{
    if (!pSelected) return;
    pSelected->setWorldRect(rect, pUIContext);
    updateGizmoRect();
}

void DocumentView::updateGizmoRect()
{
    auto rct = pSelected->getWorldRect(pUIContext);
    rct.x = std::roundf(rct.x);
    rct.y = std::roundf(rct.y);
    m_pGizmo->rect = (rct);
}

void DocumentView::xAutoGuideAgainst(const Rect& otherRect, bool& found, const Rect& rect, float& x, bool& side, float& closest)
{
    // Direct align guides
    if (rect.x > otherRect.x - closest &&
        rect.x < otherRect.x + closest)
    {
        found = true;
        x = otherRect.x;
        closest = std::abs(otherRect.x - rect.x);
        side = false;
    }
    if (rect.x > otherRect.x + otherRect.z - closest &&
        rect.x < otherRect.x + otherRect.z + closest)
    {
        found = true;
        x = otherRect.x + otherRect.z;
        closest = std::abs(otherRect.x + otherRect.z - rect.x);
        side = false;
    }
    if (rect.x + rect.z > otherRect.x - closest &&
        rect.x + rect.z < otherRect.x + closest)
    {
        found = true;
        x = otherRect.x;
        closest = std::abs(otherRect.x - (rect.x + rect.z));
        side = true;
    }
    if (rect.x + rect.z > otherRect.x + otherRect.z - closest &&
        rect.x + rect.z < otherRect.x + otherRect.z + closest)
    {
        found = true;
        x = otherRect.x + otherRect.z;
        closest = std::abs(otherRect.x + otherRect.z - (rect.x + rect.z));
        side = true;
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (rect.x > otherRect.x + otherRect.z + m_autoPadding - closest &&
            rect.x < otherRect.x + otherRect.z + m_autoPadding + closest)
        {
            found = true;
            x = otherRect.x + otherRect.z + m_autoPadding;
            closest = std::abs(otherRect.x + otherRect.z + m_autoPadding - rect.x);
            side = false;
        }
        if (rect.x + rect.z > otherRect.x - m_autoPadding - closest &&
            rect.x + rect.z < otherRect.x - m_autoPadding + closest)
        {
            found = true;
            x = otherRect.x - m_autoPadding;
            closest = std::abs((otherRect.x - m_autoPadding) - (rect.x + rect.z));
            side = true;
        }
    }
}

void DocumentView::yAutoGuideAgainst(const Rect& otherRect, bool& found, const Rect& rect, float& y, bool& side, float& closest)
{
    if (rect.y > otherRect.y - closest &&
        rect.y < otherRect.y + closest)
    {
        found = true;
        y = otherRect.y;
        closest = std::abs(otherRect.y - rect.y);
        side = false;
    }
    if (rect.y > otherRect.y + otherRect.w - closest &&
        rect.y < otherRect.y + otherRect.w + closest)
    {
        found = true;
        y = otherRect.y + otherRect.w;
        closest = std::abs(otherRect.y + otherRect.w - rect.y);
        side = false;
    }
    if (rect.y + rect.w > otherRect.y - closest &&
        rect.y + rect.w < otherRect.y + closest)
    {
        found = true;
        y = otherRect.y;
        closest = std::abs(otherRect.y - (rect.y + rect.w));
        side = true;
    }
    if (rect.y + rect.w > otherRect.y + otherRect.w - closest &&
        rect.y + rect.w < otherRect.y + otherRect.w + closest)
    {
        found = true;
        y = otherRect.y + otherRect.w;
        closest = std::abs(otherRect.y + otherRect.w - (rect.y + rect.w));
        side = true;
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (rect.y > otherRect.y + otherRect.w + m_autoPadding - closest &&
            rect.y < otherRect.y + otherRect.w + m_autoPadding + closest)
        {
            found = true;
            y = otherRect.y + otherRect.w + m_autoPadding;
            closest = std::abs(otherRect.y + otherRect.w + m_autoPadding - rect.y);
            side = false;
        }
        if (rect.y + rect.w > otherRect.y - m_autoPadding - closest &&
            rect.y + rect.w < otherRect.y - m_autoPadding + closest)
        {
            found = true;
            y = otherRect.y - m_autoPadding;
            closest = std::abs((otherRect.y - m_autoPadding) - (rect.y + rect.w));
            side = true;
        }
    }
}

void DocumentView::snapX(float x, float &ret, const Rect& rect, float &closest, bool& found)
{
    // Direct align guides
    if (x > rect.x - closest &&
        x < rect.x + closest)
    {
        found = true;
        ret = rect.x;
        closest = std::abs(rect.x - x);
    }
    if (x > rect.x + rect.z - closest &&
        x < rect.x + rect.z + closest)
    {
        found = true;
        ret = rect.x + rect.z;
        closest = std::abs(rect.x + rect.z - x);
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (x > rect.x + rect.z + m_autoPadding - closest &&
            x < rect.x + rect.z + m_autoPadding + closest)
        {
            found = true;
            ret = rect.x + rect.z + m_autoPadding;
            closest = std::abs(rect.x + rect.z + m_autoPadding - x);
        }
        if (x > rect.x - m_autoPadding - closest &&
            x < rect.x - m_autoPadding + closest)
        {
            found = true;
            ret = rect.x - m_autoPadding;
            closest = std::abs((rect.x - m_autoPadding) - x);
        }
    }
}

float DocumentView::snapX(const OUIControlRef& pControl, float x)
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
        snapX(x, ret, pChild->getWorldRect(pUIContext), closest, found);
    }
    auto& parentRect = pParent->getWorldRect(pUIContext);
    snapX(x, ret, {parentRect.x, parentRect.y, 0, parentRect.w}, closest, found);
    snapX(x, ret, {parentRect.x + parentRect.z, parentRect.y, 0, parentRect.w}, closest, found);

    if (found)
    {
        auto& rect = m_guides[1]->rect;
        m_guides[1]->rect = {ret, rect.y, rect.z, rect.w};
        m_guides[1]->isVisible = (true);
    }
    else
    {
        m_guides[1]->isVisible = (false);
    }

    return ret;
}

void DocumentView::snapY(float y, float &ret, const Rect& rect, float &closest, bool& found)
{
    // Direct align guides
    if (y > rect.y - closest &&
        y < rect.y + closest)
    {
        found = true;
        ret = rect.y;
        closest = std::abs(rect.y - y);
    }
    if (y > rect.y + rect.w - closest &&
        y < rect.y + rect.w + closest)
    {
        found = true;
        ret = rect.y + rect.w;
        closest = std::abs(rect.y + rect.w - y);
    }

    // Padding guides
    if (m_pChkSnapMargins->getIsChecked())
    {
        if (y > rect.y + rect.w + m_autoPadding - closest &&
            y < rect.y + rect.w + m_autoPadding + closest)
        {
            found = true;
            ret = rect.y + rect.w + m_autoPadding;
            closest = std::abs(rect.y + rect.w + m_autoPadding - y);
        }
        if (y > rect.y - m_autoPadding - closest &&
            y < rect.y - m_autoPadding + closest)
        {
            found = true;
            ret = rect.y - m_autoPadding;
            closest = std::abs((rect.y - m_autoPadding) - y);
        }
    }
}

float DocumentView::snapY(const OUIControlRef& pControl, float y)
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
        snapY(y, ret, pChild->getWorldRect(pUIContext), closest, found);
    }
    auto& parentRect = pParent->getWorldRect(pUIContext);
    snapY(y, ret, {parentRect.x, parentRect.y, parentRect.z, 0}, closest, found);
    snapY(y, ret, {parentRect.x, parentRect.y + parentRect.w, parentRect.z, 0}, closest, found);

    if (found)
    {
        auto& rect = m_guides[0]->rect;
        m_guides[0]->rect = {rect.x, ret, rect.z, rect.w};
        m_guides[0]->isVisible = (true);
    }
    else
    {
        m_guides[0]->isVisible = (false);
    }

    return ret;
}

bool DocumentView::getXAutoGuide(const Rect& rect, float& x, bool& side)
{
    auto pParent = pSelected->getParent();
    if (!pParent) return false;
    float closest = 8;
    bool found = false;
    for (auto pChild : pParent->getChildren())
    {
        if (pChild == pSelected) continue;
        xAutoGuideAgainst(pChild->getWorldRect(pUIContext), found, rect, x, side, closest);
    }
    auto& parentRect = pParent->getWorldRect(pUIContext);
    xAutoGuideAgainst({parentRect.x, parentRect.y, 0, parentRect.w}, found, rect, x, side, closest);
    xAutoGuideAgainst({parentRect.x + parentRect.z, parentRect.y, 0, parentRect.w}, found, rect, x, side, closest);
    return found;
}

bool DocumentView::getYAutoGuide(const Rect& rect, float& y, bool& side)
{
    auto pParent = pSelected->getParent();
    if (!pParent) return false;
    float closest = 8;
    bool found = false;
    for (auto pChild : pParent->getChildren())
    {
        if (pChild == pSelected) continue;
        yAutoGuideAgainst(pChild->getWorldRect(pUIContext), found, rect, y, side, closest);
    }
    auto& parentRect = pParent->getWorldRect(pUIContext);
    yAutoGuideAgainst({parentRect.x, parentRect.y, parentRect.z, 0}, found, rect, y, side, closest);
    yAutoGuideAgainst({parentRect.x, parentRect.y + parentRect.w, parentRect.z, 0}, found, rect, y, side, closest);
    return found;
}

void DocumentView::onKeyDown(uintptr_t key)
{
    if (pSelected)
    {
        if (key == VK_LEFT || key == VK_RIGHT || key == VK_UP || key == VK_DOWN)
        {
            if (!ODynamicCast<OUITextBox>(g_pUIContext->getFocusControl()))
            {
                auto newRect = pSelected->getWorldRect(pUIContext);
                float speed = (OInputPressed(OKeyLeftShift) || OInputPressed(OKeyRightShift)) ? 10.f : 1.f;
                switch (key)
                {
                    case VK_LEFT:
                        newRect.x -= speed;
                        break;
                    case VK_RIGHT:
                        newRect.x += speed;
                        break;
                    case VK_UP:
                        newRect.y -= speed;
                        break;
                    case VK_DOWN:
                        newRect.y += speed;
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
    auto mouseDiff = oInput->mousePosf - m_mousePosOnDown;
    if (!m_bStartMoving)
    {
        if (fabsf(mouseDiff.x) < 3 && fabsf(mouseDiff.y) < 3) return;
        m_bStartMoving = true;
    }
    if (OInputPressed(OKeyLeftShift))
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
    newRect.x += mouseDiff.x;
    newRect.y += mouseDiff.y;

    if (m_pChkSnap->getIsChecked() && pSelected->getParent())
    {
        // Auto snap the rect to it's brothers
        float x, y;
        bool side;
        auto guideRect = m_controlWorldRectOnDown;
        guideRect.x += mouseDiff.x;
        guideRect.y += mouseDiff.y;
        if (getXAutoGuide(guideRect, x, side))
        {
            float finalOffset;
            if (!side)
            {
                finalOffset = x - m_controlWorldRectOnDown.x;
            }
            else
            {
                finalOffset = (x - m_controlWorldRectOnDown.z) - m_controlWorldRectOnDown.x;
            }
            newRect.x = m_controlWorldRectOnDown.x + finalOffset;
            auto& rect = m_guides[1]->rect;
            m_guides[1]->rect = {x, rect.y, rect.z, rect.w};
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
                finalOffset = y - m_controlWorldRectOnDown.y;
            }
            else
            {
                finalOffset = (y - m_controlWorldRectOnDown.w) - m_controlWorldRectOnDown.y;
            }
            newRect.y = m_controlWorldRectOnDown.y + finalOffset;
            auto& rect = m_guides[0]->rect;
            m_guides[0]->rect = {rect.x, y, rect.z, rect.w};
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
    auto mouseDiff = oInput->mousePosf - m_mousePosOnDown;
    if (!m_bStartMoving)
    {
        if (fabsf(mouseDiff.x) < 3 && fabsf(mouseDiff.y) < 3) return;
        m_bStartMoving = true;
    }
    auto newRect = m_controlWorldRectOnDown;
    Vector2 anchor{0, 0};
    Vector2 invAnchor{1, 1};

    // Grow/Shrink on both side equally
    if (OInputPressed(OKeyLeftShift) && (
            m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[2] || 
            m_pCurrentHandle == m_gizmoHandles[5] || 
            m_pCurrentHandle == m_gizmoHandles[7]))
    {
        bool xDominate = true;
        float ratio = m_controlWorldRectOnDown.z / m_controlWorldRectOnDown.w;
        if (ratio < 1.f)
        {
            ratio = m_controlWorldRectOnDown.w / m_controlWorldRectOnDown.z;
            xDominate = false;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.x + mouseDiff.x) - m_controlWorldRectOnDown.x;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.y + mouseDiff.y) - m_controlWorldRectOnDown.y;
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
            if (OInputPressed(OKeyLeftAlt))
            {
                newRect.x += newMouseDiffX * invAnchor.x - newMouseDiffX * anchor.x;
                newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z * .5f);
                newRect.z -= newMouseDiffX * 2;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * invAnchor.y - newMouseDiffY * anchor.y;
                newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w * .5f);
                newRect.w -= newMouseDiffY * 2;
                if (newRect.w < 0) newRect.w = 0;
            }
            else
            {
                newRect.x += newMouseDiffX * invAnchor.x;
                newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z);
                newRect.z -= newMouseDiffX;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * invAnchor.y;
                newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w);
                newRect.w -= newMouseDiffY;
                if (newRect.w < 0) newRect.w = 0;
            }
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z + mouseDiff.x) - m_controlWorldRectOnDown.x - m_controlWorldRectOnDown.z;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.y + mouseDiff.y) - m_controlWorldRectOnDown.y;
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
            if (OInputPressed(OKeyLeftAlt))
            {
                newRect.x += newMouseDiffX * anchor.x - newMouseDiffX * invAnchor.x;
                newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z * .5f);
                newRect.z += newMouseDiffX * 2;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * invAnchor.y - newMouseDiffY * anchor.y;
                newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w * .5f);
                newRect.w -= newMouseDiffY * 2;
                if (newRect.w < 0) newRect.w = 0;
            }
            else
            {
                newRect.x += newMouseDiffX * anchor.x;
                newRect.z += newMouseDiffX;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * invAnchor.y;
                newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w);
                newRect.w -= newMouseDiffY;
                if (newRect.w < 0) newRect.w = 0;
            }
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.x + mouseDiff.x) - m_controlWorldRectOnDown.x;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w + mouseDiff.y) - m_controlWorldRectOnDown.y - m_controlWorldRectOnDown.w;
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
            if (OInputPressed(OKeyLeftAlt))
            {
                newRect.x += newMouseDiffX * invAnchor.x - newMouseDiffX * anchor.x;
                newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z * .5f);
                newRect.z -= newMouseDiffX * 2;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * anchor.y - newMouseDiffY * invAnchor.y;
                newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w * .5f);
                newRect.w += newMouseDiffY * 2;
                if (newRect.w < 0) newRect.w = 0;
            }
            else
            {
                newRect.x += newMouseDiffX * invAnchor.x;
                newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z);
                newRect.z -= newMouseDiffX;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * anchor.y;
                newRect.w += newMouseDiffY;
                if (newRect.w < 0) newRect.w = 0;
            }
        }
        else if (m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiffX = snapX(pSelected, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z + mouseDiff.x) - m_controlWorldRectOnDown.x - m_controlWorldRectOnDown.z;
            auto newMouseDiffY = snapY(pSelected, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w + mouseDiff.y) - m_controlWorldRectOnDown.y - m_controlWorldRectOnDown.w;
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
            if (OInputPressed(OKeyLeftAlt))
            {
                newRect.x += newMouseDiffX * anchor.x - newMouseDiffX * invAnchor.x;
                newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z * .5f);
                newRect.z += newMouseDiffX * 2;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * anchor.y - newMouseDiffY * invAnchor.y;
                newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w * .5f);
                newRect.w += newMouseDiffY * 2;
                if (newRect.w < 0) newRect.w = 0;
            }
            else
            {
                newRect.x += newMouseDiffX * anchor.x;
                newRect.z += newMouseDiffX;
                if (newRect.z < 0) newRect.z = 0;
                newRect.y += newMouseDiffY * anchor.y;
                newRect.w += newMouseDiffY;
                if (newRect.w < 0) newRect.w = 0;
            }
        }
    }
    else if (OInputPressed(OKeyLeftAlt))
    {
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[3] ||
            m_pCurrentHandle == m_gizmoHandles[5])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.x + mouseDiff.x) - m_controlWorldRectOnDown.x;
            newRect.x += newMouseDiff * invAnchor.x - newMouseDiff * anchor.x;
            newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z * .5f);
            newRect.z -= newMouseDiff * 2;
            if (newRect.z < 0) newRect.z = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2] ||
                    m_pCurrentHandle == m_gizmoHandles[4] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z + mouseDiff.x) - m_controlWorldRectOnDown.x - m_controlWorldRectOnDown.z;
            newRect.x += newMouseDiff * anchor.x - newMouseDiff * invAnchor.x;
            newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z * .5f);
            newRect.z += newMouseDiff * 2;
            if (newRect.z < 0) newRect.z = 0;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[1] ||
            m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.y + mouseDiff.y) - m_controlWorldRectOnDown.y;
            newRect.y += newMouseDiff * invAnchor.y - newMouseDiff * anchor.y;
            newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w * .5f);
            newRect.w -= newMouseDiff * 2;
            if (newRect.w < 0) newRect.w = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5] ||
                    m_pCurrentHandle == m_gizmoHandles[6] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w + mouseDiff.y) - m_controlWorldRectOnDown.y - m_controlWorldRectOnDown.w;
            newRect.y += newMouseDiff * anchor.y - newMouseDiff * invAnchor.y;
            newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w * .5f);
            newRect.w += newMouseDiff * 2;
            if (newRect.w < 0) newRect.w = 0;
        }
    }
    else // Normal sizing
    {
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[3] ||
            m_pCurrentHandle == m_gizmoHandles[5])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.x + mouseDiff.x) - m_controlWorldRectOnDown.x;
            newRect.x += newMouseDiff * invAnchor.x;
            newRect.x = std::min<float>(newRect.x, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z);
            newRect.z -= newMouseDiff;
            if (newRect.z < 0) newRect.z = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2] ||
                    m_pCurrentHandle == m_gizmoHandles[4] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.x + m_controlWorldRectOnDown.z + mouseDiff.x) - m_controlWorldRectOnDown.x - m_controlWorldRectOnDown.z;
            newRect.x += newMouseDiff * anchor.x;
            newRect.z += newMouseDiff;
            if (newRect.z < 0) newRect.z = 0;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[1] ||
            m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.y + mouseDiff.y) - m_controlWorldRectOnDown.y;
            newRect.y += newMouseDiff * invAnchor.y;
            newRect.y = std::min<float>(newRect.y, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w);
            newRect.w -= newMouseDiff;
            if (newRect.w < 0) newRect.w = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5] ||
                    m_pCurrentHandle == m_gizmoHandles[6] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.y + m_controlWorldRectOnDown.w + mouseDiff.y) - m_controlWorldRectOnDown.y - m_controlWorldRectOnDown.w;
            newRect.y += newMouseDiff * anchor.y;
            newRect.w += newMouseDiff;
            if (newRect.w < 0) newRect.w = 0;
        }
    }

    updateSelectionWithRect(newRect);
    updateInspector();
}

void DocumentView::render()
{
    auto rect = pUIScreen->getWorldRect(pUIContext);

    pUIScreen->render(pUIContext);
}

void DocumentView::repopulateTreeView(const OUIControlRef& pControl)
{
    auto pItem = onut::UITreeViewItem::create();
    pItem->pSharedUserData = pControl;
    pItem->text = pControl->name;
    pControl->pSharedUserData = pItem;
    auto pParentItem = OStaticCast<OUITreeViewItem>(pControl->getParent() ? pControl->getParent()->pSharedUserData : nullptr);
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

std::unordered_map<OUIControl::Type, OUIControlRef> g_controlInspectorMap;
std::unordered_map<OUIControl::Type, std::vector<IControlInspectorBind*>> g_inspectorBindings;
void updateControlInspector(const OUIControlRef& pControl)
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
        if (pSelected->xType == OUIControl::PosType::Percentage)
        {
            g_pInspector_UIControl_txtX->setFloat(pSelected->rect.x * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtX->setFloat(pSelected->rect.x);
        }
        if (pSelected->yType == OUIControl::PosType::Percentage)
        {
            g_pInspector_UIControl_txtY->setFloat(pSelected->rect.y * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtY->setFloat(pSelected->rect.y);
        }
        g_pInspector_UIControl_chkXPercent->setIsChecked(pSelected->xType == OUIControl::PosType::Percentage);
        g_pInspector_UIControl_chkYPercent->setIsChecked(pSelected->yType == OUIControl::PosType::Percentage);
        if (pSelected->widthType == OUIControl::DimType::Percentage)
        {
            g_pInspector_UIControl_txtWidth->setFloat(pSelected->rect.z * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtWidth->setFloat(pSelected->rect.z);
        }
        if (pSelected->heightType == OUIControl::DimType::Percentage)
        {
            g_pInspector_UIControl_txtHeight->setFloat(pSelected->rect.w * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtHeight->setFloat(pSelected->rect.w);
        }
        g_pInspector_UIControl_chkWidthPercent->setIsChecked(pSelected->widthType == OUIControl::DimType::Percentage);
        g_pInspector_UIControl_chkHeightPercent->setIsChecked(pSelected->heightType == OUIControl::DimType::Percentage);
        g_pInspector_UIControl_chkWidthRelative->setIsChecked(pSelected->widthType == OUIControl::DimType::Relative);
        g_pInspector_UIControl_chkHeightRelative->setIsChecked(pSelected->heightType == OUIControl::DimType::Relative);
        switch (pSelected->align)
        {
            case OTopLeft:
                g_pInspector_UIControl_chkTOP_LEFT->setIsChecked(true);
                break;
            case OTop:
                g_pInspector_UIControl_chkTOP->setIsChecked(true);
                break;
            case OTopRight:
                g_pInspector_UIControl_chkTOP_RIGHT->setIsChecked(true);
                break;
            case OLeft:
                g_pInspector_UIControl_chkLEFT->setIsChecked(true);
                break;
            case OCenter:
                g_pInspector_UIControl_chkCENTER->setIsChecked(true);
                break;
            case ORight:
                g_pInspector_UIControl_chkRIGHT->setIsChecked(true);
                break;
            case OBottomLeft:
                g_pInspector_UIControl_chkBOTTOM_LEFT->setIsChecked(true);
                break;
            case OBottom:
                g_pInspector_UIControl_chkBOTTOM->setIsChecked(true);
                break;
            case OBottomRight:
                g_pInspector_UIControl_chkBOTTOM_RIGHT->setIsChecked(true);
                break;
        }
        if (pSelected->xAnchorType == OUIControl::AnchorType::Percentage)
        {
            g_pInspector_UIControl_txtAnchorX->setFloat(pSelected->anchor.x * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtAnchorX->setFloat(pSelected->anchor.x);
        }
        if (pSelected->yAnchorType == OUIControl::AnchorType::Percentage)
        {
            g_pInspector_UIControl_txtAnchorY->setFloat(pSelected->anchor.y * 100.f);
        }
        else
        {
            g_pInspector_UIControl_txtAnchorY->setFloat(pSelected->anchor.y);
        }
        g_pInspector_UIControl_chkXAnchorPercent->setIsChecked(pSelected->xAnchorType == OUIControl::AnchorType::Percentage);
        g_pInspector_UIControl_chkYAnchorPercent->setIsChecked(pSelected->yAnchorType == OUIControl::AnchorType::Percentage);
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
    auto window = oWindow->getHandle();
    if (m_isDirty)
    {
        if (m_filename.empty())
        {
            SetWindowTextA(window, (oSettings->getGameName() + " - Untitle*").c_str());
        }
        else
        {
            SetWindowTextA(window, (oSettings->getGameName() + " - " + m_filename + "*").c_str());
        }
    }
    else
    {
        if (m_filename.empty())
        {
            SetWindowTextA(window, (oSettings->getGameName() + " - Untitle").c_str()); // That shouldn't be possible
        }
        else
        {
            SetWindowTextA(window, (oSettings->getGameName() + " - " + m_filename).c_str());
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

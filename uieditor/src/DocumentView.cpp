#include "DocumentView.h"
#include "viewStyles.h"

extern onut::UIControl* g_pUIScreen;
extern onut::UIContext* g_pUIContext;

extern onut::UICheckBox*    g_pInspector_UIControl_chkEnabled;
extern onut::UICheckBox*    g_pInspector_UIControl_chkVisible;
extern onut::UICheckBox*    g_pInspector_UIControl_chkClickThrough;
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
extern onut::UIButton*      g_pInspector_UIControl_txtAnchorX;
extern onut::UIButton*      g_pInspector_UIControl_txtAnchorY;
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

DocumentView::DocumentView()
{
    pUIContext = new onut::UIContext(onut::sUIVector2{640, 480});
    createViewUIStyles(pUIContext);

    pUIScreen = new onut::UIControl(/*"../../assets/ui/editor.json"*/);
    pUIScreen->setName("Root");
    pUIScreen->setWidthType(onut::eUIDimType::DIM_RELATIVE);
    pUIScreen->setHeightType(onut::eUIDimType::DIM_RELATIVE);

    m_guides[0] = g_pUIScreen->getChild("hGuide");
    m_guides[1] = g_pUIScreen->getChild("vGuide");

    // Dotted line gizmo for selection
    m_pGizmo = g_pUIScreen->getChild<onut::UIPanel>("gizmo");
    m_gizmoHandles[0] = m_pGizmo->getChild("topLeftHandle");
    m_gizmoHandles[1] = m_pGizmo->getChild("topHandle");
    m_gizmoHandles[2] = m_pGizmo->getChild("topRightHandle");
    m_gizmoHandles[3] = m_pGizmo->getChild("leftHandle");
    m_gizmoHandles[4] = m_pGizmo->getChild("rightHandle");
    m_gizmoHandles[5] = m_pGizmo->getChild("bottomLeftHandle");
    m_gizmoHandles[6] = m_pGizmo->getChild("bottomHandle");
    m_gizmoHandles[7] = m_pGizmo->getChild("bottomRightHandle");
    static HCURSOR curARROW = LoadCursor(nullptr, IDC_ARROW);
    static HCURSOR curSIZENWSE = LoadCursor(nullptr, IDC_SIZENWSE);
    static HCURSOR curSIZENESW = LoadCursor(nullptr, IDC_SIZENESW);
    static HCURSOR curSIZEWE = LoadCursor(nullptr, IDC_SIZEWE);
    static HCURSOR curSIZENS = LoadCursor(nullptr, IDC_SIZENS);
    static HCURSOR curSIZEALL = LoadCursor(nullptr, IDC_SIZEALL);
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

    // Transform handles
    const float HANDLE_SIZE = 6.f;
    const float HANDLE_PADDING = 3.f;

    updateInspector();
}

DocumentView::~DocumentView()
{
    delete pUIScreen;
    delete pUIContext;
}

void DocumentView::controlCreated(onut::UIControl* pControl, onut::UIControl* pParent)
{
    auto pItem = new onut::UITreeViewItem();
    pItem->setUserData(pControl);
    pControl->setUserData(pItem);
    auto pParentItem = static_cast<onut::UITreeViewItem*>(pParent->getUserData());
    if (pParentItem)
    {
        pParentItem->addItem(pItem);
    }
    else
    {
        m_pSceneGraph->addItem(pItem);
    }
}

void DocumentView::updateSelectedGizmoRect()
{
    m_pGizmo->setIsVisible(pSelected != nullptr);
    if (pSelected)
    {
        m_pGizmo->setRect(pSelected->getWorldRect(*pUIContext));
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
            auto pItem = static_cast<onut::UITreeViewItem*>(pSelected->getUserData());
            m_pSceneGraph->addSelectedItem(pItem);
        }
    }
    updateInspector();
}

void DocumentView::onGizmoHandleStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    m_state = eDocumentState::MOVING_HANDLE;
    m_mousePosOnDown = {mouseEvent.mousePos.x, mouseEvent.mousePos.y};
    m_pCurrentHandle = pControl;
    m_controlRectOnDown = pSelected->getRect();
    m_controlWorldRectOnDown = pSelected->getWorldRect(*pUIContext);
}

void DocumentView::onGizmoHandleEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    m_guides[0]->setIsVisible(false);
    m_guides[1]->setIsVisible(false);

    m_state = eDocumentState::IDLE;
}

void DocumentView::onGizmoStart(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    static HCURSOR curSIZEALL = LoadCursor(nullptr, IDC_SIZEALL);
    OWindow->setCursor(curSIZEALL);
    m_state = eDocumentState::MOVING_GIZO;
    m_mousePosOnDown = {mouseEvent.mousePos.x, mouseEvent.mousePos.y};
    m_controlRectOnDown = pSelected->getRect();
    m_controlWorldRectOnDown = pSelected->getWorldRect(*pUIContext);
}

void DocumentView::onGizmoEnd(onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
{
    m_guides[0]->setIsVisible(false);
    m_guides[1]->setIsVisible(false);

    m_state = eDocumentState::IDLE;
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
        case eDocumentState::IDLE:
            if (OInput->isStateJustDown(DIK_DELETE))
            {
                if (!(g_pUIContext->getFocusControl() && g_pUIContext->getFocusControl()->getType() == onut::eUIType::UI_TEXTBOX))
                {
                    deleteSelection();
                }
            }
            break;
    }
}

void DocumentView::deleteSelection()
{
    if (!pSelected) return;
    if (!pSelected->getParent()) return; // huh?
    auto pItem = static_cast<onut::UITreeViewItem*>(pSelected->getUserData());
    if (pItem->getParent())
    {
        pItem->getParent()->removeItem(pItem);
    }
    else
    {
        m_pSceneGraph->removeItem(pItem);
    }
    pSelected->getParent()->remove(pSelected);
    pSelected = nullptr;

    setSelected(nullptr);
}

void DocumentView::updateSelectionWithRect(const onut::sUIRect& rect)
{
    if (!pSelected) return;
    pSelected->setRect(rect);
    updateGizmoRect();
}

void DocumentView::updateGizmoRect()
{
    auto rct = pSelected->getWorldRect(*pUIContext);
    rct.position.x = std::roundf(rct.position.x);
    rct.position.y = std::roundf(rct.position.y);
    m_pGizmo->setRect(rct);
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

float DocumentView::snapX(onut::UIControl* pControl, float x)
{
    if (!m_autoGuide)
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
        auto& rect = m_guides[1]->getRect();
        m_guides[1]->setRect({{ret, rect.position.y}, rect.size});
        m_guides[1]->setIsVisible(true);
    }
    else
    {
        m_guides[1]->setIsVisible(false);
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

float DocumentView::snapY(onut::UIControl* pControl, float y)
{
    if (!m_autoGuide)
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
        auto& rect = m_guides[0]->getRect();
        m_guides[0]->setRect({{rect.position.x, ret}, rect.size});
        m_guides[0]->setIsVisible(true);
    }
    else
    {
        m_guides[0]->setIsVisible(false);
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
                auto newRect = pSelected->getRect();
                float speed = (OInput->isStateDown(DIK_LSHIFT) || OInput->isStateDown(DIK_RSHIFT)) ? 10.f : 1.f;
                switch (key)
                {
                    case VK_LEFT:
                        if (pSelected->getXType() == onut::eUIPosType::POS_RELATIVE)
                        {
                            newRect.position.x -= speed;
                        }
                        else if (pSelected->getXType() == onut::eUIPosType::POS_PERCENTAGE)
                        {
                            auto parentWorldRect = pSelected->getParent()->getWorldRect(*pUIContext);
                            newRect.position.x = newRect.position.x * parentWorldRect.size.x;
                            newRect.position.x -= speed;
                            newRect.position.x = newRect.position.x / parentWorldRect.size.x;
                        }
                        break;
                    case VK_RIGHT:
                        if (pSelected->getXType() == onut::eUIPosType::POS_RELATIVE)
                        {
                            newRect.position.x += speed;
                        }
                        else if (pSelected->getXType() == onut::eUIPosType::POS_PERCENTAGE)
                        {
                            auto parentWorldRect = pSelected->getParent()->getWorldRect(*pUIContext);
                            newRect.position.x = newRect.position.x * parentWorldRect.size.x;
                            newRect.position.x += speed;
                            newRect.position.x = newRect.position.x / parentWorldRect.size.x;
                        }
                        break;
                    case VK_UP:
                        if (pSelected->getXType() == onut::eUIPosType::POS_RELATIVE)
                        {
                            newRect.position.y -= speed;
                        }
                        else if (pSelected->getYType() == onut::eUIPosType::POS_PERCENTAGE)
                        {
                            auto parentWorldRect = pSelected->getParent()->getWorldRect(*pUIContext);
                            newRect.position.y = newRect.position.y * parentWorldRect.size.y;
                            newRect.position.y -= speed;
                            newRect.position.y = newRect.position.y / parentWorldRect.size.y;
                        }
                        break;
                    case VK_DOWN:
                        if (pSelected->getXType() == onut::eUIPosType::POS_RELATIVE)
                        {
                            newRect.position.y += speed;
                        }
                        else if (pSelected->getYType() == onut::eUIPosType::POS_PERCENTAGE)
                        {
                            auto parentWorldRect = pSelected->getParent()->getWorldRect(*pUIContext);
                            newRect.position.y = newRect.position.y * parentWorldRect.size.y;
                            newRect.position.y += speed;
                            newRect.position.y = newRect.position.y / parentWorldRect.size.y;
                        }
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
    auto newRect = m_controlRectOnDown;
    newRect.position.x += mouseDiff.x;
    newRect.position.y += mouseDiff.y;

    if (m_autoGuide && pSelected->getParent())
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
            newRect.position.x = m_controlRectOnDown.position.x + finalOffset;
            auto& rect = m_guides[1]->getRect();
            m_guides[1]->setRect({{x, rect.position.y}, rect.size});
            m_guides[1]->setIsVisible(true);
        }
        else
        {
            m_guides[1]->setIsVisible(false);
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
            newRect.position.y = m_controlRectOnDown.position.y + finalOffset;
            auto& rect = m_guides[0]->getRect();
            m_guides[0]->setRect({{rect.position.x, y}, rect.size});
            m_guides[0]->setIsVisible(true);
        }
        else
        {
            m_guides[0]->setIsVisible(false);
        }
    }

    updateSelectionWithRect(newRect);
    updateInspector();
}

void DocumentView::updateMovingHandle()
{
    auto mouseDiff = OMousePos - m_mousePosOnDown;
    auto anchor = pSelected->getAnchorInPercentage();
    auto invAnchor = anchor;
    invAnchor.x = 1 - anchor.x;
    invAnchor.y = 1 - anchor.y;
    auto newRect = m_controlRectOnDown;

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
                newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x * .5f);
                newRect.size.x -= newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y - newMouseDiffY * anchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y * .5f);
                newRect.size.y -= newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * invAnchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x);
                newRect.size.x -= newMouseDiffX;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y);
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
                newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x * .5f);
                newRect.size.x += newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y - newMouseDiffY * anchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y * .5f);
                newRect.size.y -= newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * anchor.x;
                newRect.size.x += newMouseDiffX;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y);
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
                newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x * .5f);
                newRect.size.x -= newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * anchor.y - newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y * .5f);
                newRect.size.y += newMouseDiffY * 2;
                if (newRect.size.y < 0) newRect.size.y = 0;
            }
            else
            {
                newRect.position.x += newMouseDiffX * invAnchor.x;
                newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x);
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
                newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x * .5f);
                newRect.size.x += newMouseDiffX * 2;
                if (newRect.size.x < 0) newRect.size.x = 0;
                newRect.position.y += newMouseDiffY * anchor.y - newMouseDiffY * invAnchor.y;
                newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y * .5f);
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
            newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x * .5f);
            newRect.size.x -= newMouseDiff * 2;
            if (newRect.size.x < 0) newRect.size.x = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[2] ||
                    m_pCurrentHandle == m_gizmoHandles[4] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapX(pSelected, m_controlWorldRectOnDown.position.x + m_controlWorldRectOnDown.size.x + mouseDiff.x) - m_controlWorldRectOnDown.position.x - m_controlWorldRectOnDown.size.x;
            newRect.position.x += newMouseDiff * anchor.x - newMouseDiff * invAnchor.x;
            newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x * .5f);
            newRect.size.x += newMouseDiff * 2;
            if (newRect.size.x < 0) newRect.size.x = 0;
        }
        if (m_pCurrentHandle == m_gizmoHandles[0] ||
            m_pCurrentHandle == m_gizmoHandles[1] ||
            m_pCurrentHandle == m_gizmoHandles[2])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.position.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y;
            newRect.position.y += newMouseDiff * invAnchor.y - newMouseDiff * anchor.y;
            newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y * .5f);
            newRect.size.y -= newMouseDiff * 2;
            if (newRect.size.y < 0) newRect.size.y = 0;
        }
        else if (m_pCurrentHandle == m_gizmoHandles[5] ||
                    m_pCurrentHandle == m_gizmoHandles[6] ||
                    m_pCurrentHandle == m_gizmoHandles[7])
        {
            auto newMouseDiff = snapY(pSelected, m_controlWorldRectOnDown.position.y + m_controlWorldRectOnDown.size.y + mouseDiff.y) - m_controlWorldRectOnDown.position.y - m_controlWorldRectOnDown.size.y;
            newRect.position.y += newMouseDiff * anchor.y - newMouseDiff * invAnchor.y;
            newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y * .5f);
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
            newRect.position.x = std::min<float>(newRect.position.x, m_controlRectOnDown.position.x + m_controlRectOnDown.size.x);
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
            newRect.position.y = std::min<float>(newRect.position.y, m_controlRectOnDown.position.y + m_controlRectOnDown.size.y);
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
    pUIScreen->render(*pUIContext);
}

void DocumentView::repopulateTreeView(onut::UIControl* pControl)
{
    auto pItem = new onut::UITreeViewItem();
    pItem->setUserData(pControl);
    pItem->setText(pControl->getName());
    pControl->setUserData(pItem);
    auto pParentItem = static_cast<onut::UITreeViewItem*>(pControl->getParent() ? pControl->getParent()->getUserData() : nullptr);
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

void DocumentView::updateInspector()
{
    if (pSelected)
    {
        // Enable controls
        g_pInspector_UIControl_chkEnabled->setIsEnabled(true);
        g_pInspector_UIControl_chkVisible->setIsEnabled(true);
        g_pInspector_UIControl_chkClickThrough->setIsEnabled(true);
        g_pInspector_UIControl_txtName->setIsEnabled(true);
        g_pInspector_UIControl_txtStyle->setIsEnabled(true);
        g_pInspector_UIControl_txtX->setIsEnabled(true);
        g_pInspector_UIControl_txtY->setIsEnabled(true);
        g_pInspector_UIControl_chkXPercent->setIsEnabled(true);
        g_pInspector_UIControl_chkYPercent->setIsEnabled(true);
        g_pInspector_UIControl_txtWidth->setIsEnabled(true);
        g_pInspector_UIControl_txtHeight->setIsEnabled(true);
        g_pInspector_UIControl_chkWidthPercent->setIsEnabled(true);
        g_pInspector_UIControl_chkHeightPercent->setIsEnabled(true);
        g_pInspector_UIControl_chkWidthRelative->setIsEnabled(true);
        g_pInspector_UIControl_chkHeightRelative->setIsEnabled(true);
        g_pInspector_UIControl_chkTOP_LEFT->setIsEnabled(true);
        g_pInspector_UIControl_chkTOP->setIsEnabled(true);
        g_pInspector_UIControl_chkTOP_RIGHT->setIsEnabled(true);
        g_pInspector_UIControl_chkLEFT->setIsEnabled(true);
        g_pInspector_UIControl_chkCENTER->setIsEnabled(true);
        g_pInspector_UIControl_chkRIGHT->setIsEnabled(true);
        g_pInspector_UIControl_chkBOTTOM_LEFT->setIsEnabled(true);
        g_pInspector_UIControl_chkBOTTOM->setIsEnabled(true);
        g_pInspector_UIControl_chkBOTTOM_RIGHT->setIsEnabled(true);
        g_pInspector_UIControl_txtAnchorX->setIsEnabled(true);
        g_pInspector_UIControl_txtAnchorY->setIsEnabled(true);
        g_pInspector_UIControl_chkXAnchorPercent->setIsEnabled(true);
        g_pInspector_UIControl_chkYAnchorPercent->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorTOP_LEFT->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorTOP->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorTOP_RIGHT->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorLEFT->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorCENTER->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorRIGHT->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorBOTTOM_LEFT->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorBOTTOM->setIsEnabled(true);
        g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT->setIsEnabled(true);

        // Set their values
        g_pInspector_UIControl_chkEnabled->setIsChecked(pSelected->isEnabled());
        g_pInspector_UIControl_chkVisible->setIsChecked(pSelected->isVisible());
        g_pInspector_UIControl_chkClickThrough->setIsChecked(pSelected->isClickThrough());
        g_pInspector_UIControl_txtName->setText(pSelected->getName());
        g_pInspector_UIControl_txtStyle->setText(pSelected->getStyleName());
        g_pInspector_UIControl_txtX->setText(std::to_string(pSelected->getRect().position.x));
        g_pInspector_UIControl_txtY->setText(std::to_string(pSelected->getRect().position.y));
        g_pInspector_UIControl_chkXPercent->setIsChecked(pSelected->getXType() == onut::eUIPosType::POS_PERCENTAGE);
        g_pInspector_UIControl_chkYPercent->setIsChecked(pSelected->getYType() == onut::eUIPosType::POS_PERCENTAGE);
        g_pInspector_UIControl_txtWidth->setText(std::to_string(pSelected->getRect().size.x));
        g_pInspector_UIControl_txtHeight->setText(std::to_string(pSelected->getRect().size.y));
        g_pInspector_UIControl_chkWidthPercent->setIsChecked(pSelected->getWidthType() == onut::eUIDimType::DIM_PERCENTAGE);
        g_pInspector_UIControl_chkHeightPercent->setIsChecked(pSelected->getHeightType() == onut::eUIDimType::DIM_PERCENTAGE);
        g_pInspector_UIControl_chkWidthRelative->setIsChecked(pSelected->getWidthType() == onut::eUIDimType::DIM_RELATIVE);
        g_pInspector_UIControl_chkHeightRelative->setIsChecked(pSelected->getHeightType() == onut::eUIDimType::DIM_RELATIVE);
        switch (pSelected->getAlign())
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
        g_pInspector_UIControl_txtAnchorX->setCaption(std::to_string(pSelected->getAnchor().x));
        g_pInspector_UIControl_txtAnchorY->setCaption(std::to_string(pSelected->getAnchor().y));
        g_pInspector_UIControl_chkXAnchorPercent->setIsChecked(pSelected->getXAnchorType() == onut::eUIAnchorType::ANCHOR_PERCENTAGE);
        g_pInspector_UIControl_chkYAnchorPercent->setIsChecked(pSelected->getYAnchorType() == onut::eUIAnchorType::ANCHOR_PERCENTAGE);
    }
    else
    {
        // Disable all controls
        g_pInspector_UIControl_chkEnabled->setIsEnabled(false);
        g_pInspector_UIControl_chkVisible->setIsEnabled(false);
        g_pInspector_UIControl_chkClickThrough->setIsEnabled(false);
        g_pInspector_UIControl_txtName->setIsEnabled(false);
        g_pInspector_UIControl_txtStyle->setIsEnabled(false);
        g_pInspector_UIControl_txtX->setIsEnabled(false);
        g_pInspector_UIControl_txtY->setIsEnabled(false);
        g_pInspector_UIControl_chkXPercent->setIsEnabled(false);
        g_pInspector_UIControl_chkYPercent->setIsEnabled(false);
        g_pInspector_UIControl_txtWidth->setIsEnabled(false);
        g_pInspector_UIControl_txtHeight->setIsEnabled(false);
        g_pInspector_UIControl_chkWidthPercent->setIsEnabled(false);
        g_pInspector_UIControl_chkHeightPercent->setIsEnabled(false);
        g_pInspector_UIControl_chkWidthRelative->setIsEnabled(false);
        g_pInspector_UIControl_chkHeightRelative->setIsEnabled(false);
        g_pInspector_UIControl_chkTOP_LEFT->setIsEnabled(false);
        g_pInspector_UIControl_chkTOP->setIsEnabled(false);
        g_pInspector_UIControl_chkTOP_RIGHT->setIsEnabled(false);
        g_pInspector_UIControl_chkLEFT->setIsEnabled(false);
        g_pInspector_UIControl_chkCENTER->setIsEnabled(false);
        g_pInspector_UIControl_chkRIGHT->setIsEnabled(false);
        g_pInspector_UIControl_chkBOTTOM_LEFT->setIsEnabled(false);
        g_pInspector_UIControl_chkBOTTOM->setIsEnabled(false);
        g_pInspector_UIControl_chkBOTTOM_RIGHT->setIsEnabled(false);
        g_pInspector_UIControl_txtAnchorX->setIsEnabled(false);
        g_pInspector_UIControl_txtAnchorY->setIsEnabled(false);
        g_pInspector_UIControl_chkXAnchorPercent->setIsEnabled(false);
        g_pInspector_UIControl_chkYAnchorPercent->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorTOP_LEFT->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorTOP->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorTOP_RIGHT->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorLEFT->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorCENTER->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorRIGHT->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorBOTTOM_LEFT->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorBOTTOM->setIsEnabled(false);
        g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT->setIsEnabled(false);
    }
}

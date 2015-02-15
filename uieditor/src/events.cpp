#include "ActionManager.h"
#include "DocumentView.h"
#include "events.h"

extern DocumentView*        g_pDocument;
extern onut::UIContext*     g_pUIContext;
extern onut::UIControl*     g_pUIScreen;
extern onut::ActionManager  g_actionManager;

onut::UICheckBox*    g_pInspector_UIControl_chkEnabled;
onut::UICheckBox*    g_pInspector_UIControl_chkVisible;
onut::UICheckBox*    g_pInspector_UIControl_chkClickThrough;
onut::UITextBox*     g_pInspector_UIControl_txtName;
onut::UITextBox*     g_pInspector_UIControl_txtStyle;
onut::UITextBox*     g_pInspector_UIControl_txtX;
onut::UITextBox*     g_pInspector_UIControl_txtY;
onut::UICheckBox*    g_pInspector_UIControl_chkXPercent;
onut::UICheckBox*    g_pInspector_UIControl_chkYPercent;
onut::UITextBox*     g_pInspector_UIControl_txtWidth;
onut::UITextBox*     g_pInspector_UIControl_txtHeight;
onut::UICheckBox*    g_pInspector_UIControl_chkWidthPercent;
onut::UICheckBox*    g_pInspector_UIControl_chkHeightPercent;
onut::UICheckBox*    g_pInspector_UIControl_chkWidthRelative;
onut::UICheckBox*    g_pInspector_UIControl_chkHeightRelative;
onut::UICheckBox*    g_pInspector_UIControl_chkTOP_LEFT;
onut::UICheckBox*    g_pInspector_UIControl_chkTOP;
onut::UICheckBox*    g_pInspector_UIControl_chkTOP_RIGHT;
onut::UICheckBox*    g_pInspector_UIControl_chkLEFT;
onut::UICheckBox*    g_pInspector_UIControl_chkCENTER;
onut::UICheckBox*    g_pInspector_UIControl_chkRIGHT;
onut::UICheckBox*    g_pInspector_UIControl_chkBOTTOM_LEFT;
onut::UICheckBox*    g_pInspector_UIControl_chkBOTTOM;
onut::UICheckBox*    g_pInspector_UIControl_chkBOTTOM_RIGHT;
onut::UITextBox*     g_pInspector_UIControl_txtAnchorX;
onut::UITextBox*     g_pInspector_UIControl_txtAnchorY;
onut::UICheckBox*    g_pInspector_UIControl_chkXAnchorPercent;
onut::UICheckBox*    g_pInspector_UIControl_chkYAnchorPercent;
onut::UIButton*      g_pInspector_UIControl_chkAnchorTOP_LEFT;
onut::UIButton*      g_pInspector_UIControl_chkAnchorTOP;
onut::UIButton*      g_pInspector_UIControl_chkAnchorTOP_RIGHT;
onut::UIButton*      g_pInspector_UIControl_chkAnchorLEFT;
onut::UIButton*      g_pInspector_UIControl_chkAnchorCENTER;
onut::UIButton*      g_pInspector_UIControl_chkAnchorRIGHT;
onut::UIButton*      g_pInspector_UIControl_chkAnchorBOTTOM_LEFT;
onut::UIButton*      g_pInspector_UIControl_chkAnchorBOTTOM;
onut::UIButton*      g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT;

onut::UIControl* getCreateParent()
{
    auto pSelected = g_pDocument->pSelected;
    if (!pSelected)
    {
        return g_pDocument->pUIScreen;
    }
    else
    {
        if (OInput->isStateDown(DIK_LSHIFT))
        {
            return pSelected;
        }
        else if (pSelected->getParent())
        {
            return pSelected->getParent();
        }
        else
        {
            return g_pDocument->pUIScreen;
        }
    }
}

void createControlAction(onut::UIControl* pControl, onut::UIControl* pParent)
{
    auto pSelected = g_pDocument->pSelected;
    g_actionManager.doAction(new onut::Action(
        [=]{
        pParent->add(pControl);
        g_pDocument->controlCreated(pControl, pParent);
        g_pDocument->setSelected(pControl);
    },
        [=]{
        g_pDocument->setSelected(pSelected);
        pParent->remove(pControl);
        g_pDocument->controlDeleted(pControl);
    },
        [=]{
        if (pSelected) pSelected->retain();
        pParent->retain();
        pControl->retain();
    },
        [=]{
        if (pSelected) pSelected->release();
        pParent->release();
        pControl->release();
    }));
}

void onCreateControl(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pCtrl = new onut::UIControl();

    pCtrl->setRect({{0, 0}, {100, 100}});
    createControlAction(pCtrl, pParent);
}

void onCreatePanel(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pPanel = new onut::UIPanel();

    pPanel->setRect({{0, 0}, {100, 100}});
    createControlAction(pPanel, pParent);
}

void onCreateButton(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = new onut::UIButton();

    pBtn->setCaption("Button");
    pBtn->setRect({{0, 0}, {64, 24}});
    createControlAction(pBtn, pParent);
}

void onCreateLabel(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pLbl = new onut::UILabel();

    pLbl->setText("Label");
    pLbl->setRect({{0, 0}, {64, 20}});
    createControlAction(pLbl, pParent);
}

void onCreateImage(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pImg = new onut::UIImage();

    pImg->setRect({{0, 0}, {32, 32}});
    createControlAction(pImg, pParent);
}

bool g_bSelected = false;

void onSelect(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto mousePos = evt.mousePos;
    auto rect = g_pUIScreen->getChild("pnlRegion")->getWorldRect(*g_pUIContext);
    mousePos.x -= rect.position.x;
    mousePos.y -= rect.position.y;
    auto pPickedControl = g_pDocument->pUIScreen->getChild(*g_pDocument->pUIContext, mousePos, true, false);
    auto pPreviousSelected = g_pDocument->pSelected;

    g_actionManager.doAction(new onut::Action(
        [=]{
        g_pDocument->setSelected(pPickedControl);
    },
        [=]{
        g_pDocument->setSelected(pPreviousSelected);
    },
        [=]{
        if (pPreviousSelected) pPreviousSelected->retain();
        if (pPickedControl) pPickedControl->retain();
    },
        [=]{
        if (pPreviousSelected) pPreviousSelected->release();
        if (pPickedControl) pPickedControl->release();
    }
    ));

    if (pPickedControl)
    {
        g_bSelected = true;
        g_pDocument->onGizmoStart(pPickedControl, evt);
    }
}

void onSelectUp(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    if (g_bSelected)
    {
        g_bSelected = false;
        if (g_pDocument->pSelected)
        {
            g_pDocument->onGizmoEnd(g_pDocument->pSelected, evt);
        }
    }
}

void onSceneGraphSelectionChanged(onut::UITreeView* pControl, const onut::UITreeViewSelectEvent& evt)
{
    if (evt.pSelectedItems->empty())
    {
        auto pPreviousSelected = g_pDocument->pSelected;
        g_actionManager.doAction(new onut::Action(
            [=]{
            g_pDocument->setSelected(nullptr, true);
        },
            [=]{
            g_pDocument->setSelected(pPreviousSelected, true);
        },
            [=]{
            if (pPreviousSelected) pPreviousSelected->retain();
        },
            [=]{
            if (pPreviousSelected) pPreviousSelected->release();
        }
        ));
    }
    else
    {
        auto pViewItem = evt.pSelectedItems->front();
        auto pSelected = static_cast<onut::UIControl*>(pViewItem->getUserData());

        auto pPreviousSelected = g_pDocument->pSelected;
        g_actionManager.doAction(new onut::Action(
            [=]{
            g_pDocument->setSelected(pSelected, true);
        },
            [=]{
            g_pDocument->setSelected(pPreviousSelected, true);
        },
            [=]{
            if (pSelected) pSelected->retain();
            if (pPreviousSelected) pPreviousSelected->retain();
        },
            [=]{
            if (pSelected) pSelected->release();
            if (pPreviousSelected) pPreviousSelected->release();
        }
        ));
    }
}

void onUIControlNameChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;

    auto text = pTextBox->getText();
    auto pControl = g_pDocument->pSelected;
    auto previousText = pControl->getName();

    g_actionManager.doAction(new onut::Action(
        [=]{
        pControl->setName(text);
        auto pViewItem = static_cast<onut::UITreeViewItem*>(pControl->getUserData());
        if (pViewItem)
        {
            pViewItem->setText(text);
        }
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->setName(previousText);
        auto pViewItem = static_cast<onut::UITreeViewItem*>(pControl->getUserData());
        if (pViewItem)
        {
            pViewItem->setText(previousText);
        }
        g_pDocument->updateInspector();
    },
        [=]{
        if (pTextBox) pTextBox->retain();
        if (pControl) pControl->retain();
    },
        [=]{
        if (pTextBox) pTextBox->release();
        if (pControl) pControl->release();
    }
    ));
}

void doRectChange(onut::UIControl* pControl, const onut::sUIRect& rect)
{
    if (!pControl) return;
    auto previousRect = pControl->getRect();
    g_actionManager.doAction(new onut::Action(
        [=]{
        pControl->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->setRect(previousRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->retain();
    },
        [=]{
        pControl->release();
    }
    ));
}

void onUIControlXChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->getRect();
    rect.position.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->getXType() == onut::eUIPosType::POS_PERCENTAGE)
    {
        rect.position.x /= 100.f;
    }
    doRectChange(g_pDocument->pSelected, rect);
}
void onUIControlYChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->getRect();
    rect.position.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->getYType() == onut::eUIPosType::POS_PERCENTAGE)
    {
        rect.position.y /= 100.f;
    }
    doRectChange(g_pDocument->pSelected, rect);
}

void onUIControlAnchorXChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = g_pDocument->pSelected->getAnchor();
    auto newAnchor = prevAnchor;
    newAnchor.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->getXAnchorType() == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
    {
        newAnchor.x /= 100.f;
    }
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setAnchor(newAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setAnchor(prevAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlAnchorYChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = g_pDocument->pSelected->getAnchor();
    auto newAnchor = prevAnchor;
    newAnchor.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->getYAnchorType() == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
    {
        newAnchor.y /= 100.f;
    }
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setAnchor(newAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setAnchor(prevAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlWidthChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->getRect();
    rect.size.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->getWidthType() == onut::eUIDimType::DIM_PERCENTAGE)
    {
        rect.size.x /= 100.f;
    }
    doRectChange(g_pDocument->pSelected, rect);
}

void onUIControlHeightChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->getRect();
    rect.size.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->getHeightType() == onut::eUIDimType::DIM_PERCENTAGE)
    {
        rect.size.y /= 100.f;
    }
    doRectChange(g_pDocument->pSelected, rect);
}

void onUIControlStyleChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevStyle = pSelected->getStyleName();
    auto newStyle = pTextBox->getText();
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setStyle(newStyle.c_str());
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setStyle(prevStyle.c_str());
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }
    ));
}

void onUIControlEnableChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setIsEnabled(bValue);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setIsEnabled(!bValue);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlVisibleChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setIsVisible(bValue);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setIsVisible(!bValue);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlClickThroughChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setIsClickThrough(bValue);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setIsClickThrough(!bValue);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlXAnchorPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = pSelected->getAnchor();
    auto prevAnchorType = pSelected->getXAnchorType();
    auto newAnchor = prevAnchor;
    auto newAnchorType = prevAnchorType;
    if (pCheckBox->getIsChecked() && prevAnchorType == onut::eUIAnchorType::ANCHOR_PIXEL)
    {
        newAnchor.x = pSelected->getAnchorInPercentage().x;
        newAnchorType = onut::eUIAnchorType::ANCHOR_PERCENTAGE;
    }
    else if (!pCheckBox->getIsChecked() && prevAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
    {
        newAnchor.x = pSelected->getAnchorInPixel().x;
        newAnchorType = onut::eUIAnchorType::ANCHOR_PIXEL;
    }
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setAnchor(newAnchor);
        pSelected->setXAnchorType(newAnchorType);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setAnchor(prevAnchor);
        pSelected->setXAnchorType(prevAnchorType);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlYAnchorPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = pSelected->getAnchor();
    auto prevAnchorType = pSelected->getYAnchorType();
    auto newAnchor = prevAnchor;
    auto newAnchorType = prevAnchorType;
    if (pCheckBox->getIsChecked() && prevAnchorType == onut::eUIAnchorType::ANCHOR_PIXEL)
    {
        newAnchor.y = pSelected->getAnchorInPercentage().y;
        newAnchorType = onut::eUIAnchorType::ANCHOR_PERCENTAGE;
    }
    else if (!pCheckBox->getIsChecked() && prevAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
    {
        newAnchor.y = pSelected->getAnchorInPixel().y;
        newAnchorType = onut::eUIAnchorType::ANCHOR_PIXEL;
    }
    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setAnchor(newAnchor);
        pSelected->setYAnchorType(newAnchorType);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setAnchor(prevAnchor);
        pSelected->setYAnchorType(prevAnchorType);
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlWidthPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    if (pCheckBox->getIsChecked() && g_pDocument->pSelected->getWidthType() != onut::eUIDimType::DIM_PERCENTAGE)
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto worldRect = g_pDocument->pSelected->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setWidthType(onut::eUIDimType::DIM_PERCENTAGE);
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.size.x = worldRect.size.x / parentRect.size.x;
        g_pInspector_UIControl_txtWidth->setText(std::to_string(rect.size.x * 100.f));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkWidthRelative->getIsChecked())
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setWidthType(onut::eUIDimType::DIM_ABSOLUTE);
        rect.size.x = rect.size.x * parentRect.size.x;
        g_pInspector_UIControl_txtWidth->setText(std::to_string(rect.size.x));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
}

void onUIControlHeightPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    if (pCheckBox->getIsChecked() && g_pDocument->pSelected->getHeightType() != onut::eUIDimType::DIM_PERCENTAGE)
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto worldRect = g_pDocument->pSelected->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setHeightType(onut::eUIDimType::DIM_PERCENTAGE);
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.size.y = worldRect.size.y / parentRect.size.y;
        g_pInspector_UIControl_txtHeight->setText(std::to_string(rect.size.y * 100.f));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkHeightRelative->getIsChecked())
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setHeightType(onut::eUIDimType::DIM_ABSOLUTE);
        rect.size.y = rect.size.y * parentRect.size.y;
        g_pInspector_UIControl_txtHeight->setText(std::to_string(rect.size.y));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
}

void onUIControlWidthRelativeChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    if (pCheckBox->getIsChecked() && g_pDocument->pSelected->getWidthType() != onut::eUIDimType::DIM_RELATIVE)
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto worldRect = g_pDocument->pSelected->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setWidthType(onut::eUIDimType::DIM_RELATIVE);
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.size.x = worldRect.size.x - parentRect.size.x;
        g_pInspector_UIControl_txtWidth->setText(std::to_string(rect.size.x));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkWidthPercent->getIsChecked())
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setWidthType(onut::eUIDimType::DIM_ABSOLUTE);
        rect.size.x = parentRect.size.x + rect.size.x;
        g_pInspector_UIControl_txtWidth->setText(std::to_string(rect.size.x));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
}

void onUIControlHeightRelativeChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    if (pCheckBox->getIsChecked() && g_pDocument->pSelected->getHeightType() != onut::eUIDimType::DIM_RELATIVE)
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto worldRect = g_pDocument->pSelected->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setHeightType(onut::eUIDimType::DIM_RELATIVE);
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.size.y = worldRect.size.y - parentRect.size.y;
        g_pInspector_UIControl_txtHeight->setText(std::to_string(rect.size.y));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkHeightPercent->getIsChecked())
    {
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        g_pDocument->pSelected->setHeightType(onut::eUIDimType::DIM_ABSOLUTE);
        rect.size.y = parentRect.size.y + rect.size.y;
        g_pInspector_UIControl_txtHeight->setText(std::to_string(rect.size.y));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
}

void onUIControlXPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    if (pCheckBox->getIsChecked() && g_pDocument->pSelected->getXType() != onut::eUIPosType::POS_PERCENTAGE)
    {
        g_pDocument->pSelected->setXType(onut::eUIPosType::POS_PERCENTAGE);
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.position.x = rect.position.x / parentRect.size.x;
        g_pInspector_UIControl_txtX->setText(std::to_string(rect.position.x * 100.f));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
    else if (!pCheckBox->getIsChecked() && g_pDocument->pSelected->getXType() == onut::eUIPosType::POS_PERCENTAGE)
    {
        g_pDocument->pSelected->setXType(onut::eUIPosType::POS_RELATIVE);
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.position.x = rect.position.x * parentRect.size.x;
        g_pInspector_UIControl_txtX->setText(std::to_string(rect.position.x));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
}

void onUIControlYPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    if (pCheckBox->getIsChecked() && g_pDocument->pSelected->getYType() != onut::eUIPosType::POS_PERCENTAGE)
    {
        g_pDocument->pSelected->setYType(onut::eUIPosType::POS_PERCENTAGE);
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.position.y = rect.position.y / parentRect.size.y;
        g_pInspector_UIControl_txtY->setText(std::to_string(rect.position.y * 100.f));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
    else if (!pCheckBox->getIsChecked() && g_pDocument->pSelected->getYType() == onut::eUIPosType::POS_PERCENTAGE)
    {
        g_pDocument->pSelected->setYType(onut::eUIPosType::POS_RELATIVE);
        auto rect = g_pDocument->pSelected->getRect();
        auto& parentRect = g_pDocument->pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        rect.position.y = rect.position.y * parentRect.size.y;
        g_pInspector_UIControl_txtY->setText(std::to_string(rect.position.y));
        g_pDocument->pSelected->setRect(rect);
        g_pDocument->updateSelectedGizmoRect();
    }
}

void onAnchorClicked(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    if (!g_pDocument->pSelected) return; // Wuuuut?
    auto pSelected = g_pDocument->pSelected;
    onut::sUIVector2 prevAnchor = pSelected->getAnchor();
    if (pControl == g_pInspector_UIControl_chkAnchorTOP_LEFT)
    {
        pSelected->setAnchorPercent({0, 0});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorTOP)
    {
        pSelected->setAnchorPercent({.5f, 0});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorTOP_RIGHT)
    {
        pSelected->setAnchorPercent({1, 0});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorLEFT)
    {
        pSelected->setAnchorPercent({0, .5f});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorCENTER)
    {
        pSelected->setAnchorPercent({.5f, .5f});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorRIGHT)
    {
        pSelected->setAnchorPercent({1, .5f});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorBOTTOM_LEFT)
    {
        pSelected->setAnchorPercent({0, 1});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorBOTTOM)
    {
        pSelected->setAnchorPercent({.5f, 1});
    }
    else if (pControl == g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT)
    {
        pSelected->setAnchorPercent({1, 1});
    }
    onut::sUIVector2 newAnchor = pSelected->getAnchor();

    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setAnchor(newAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setAnchor(prevAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onAlignChkChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!evt.isChecked) return; // Don't care
    if (!g_pDocument->pSelected) return; // Wuuuut?

    auto pSelected = g_pDocument->pSelected;
    auto previousRect = pSelected->getRect();
    auto newRect = previousRect;
    newRect.position.x = 0;
    newRect.position.y = 0;
    auto previousAnchor = pSelected->getAnchor();
    auto previousAlign = pSelected->getAlign();
    if (pCheckBox == g_pInspector_UIControl_chkTOP_LEFT)
    {
        pSelected->setAnchorPercent({0, 0});
        pSelected->setAlign(onut::eUIAlign::TOP_LEFT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkTOP)
    {
        pSelected->setAnchorPercent({.5f, 0});
        pSelected->setAlign(onut::eUIAlign::TOP);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkTOP_RIGHT)
    {
        pSelected->setAnchorPercent({1, 0});
        pSelected->setAlign(onut::eUIAlign::TOP_RIGHT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkLEFT)
    {
        pSelected->setAnchorPercent({0, .5f});
        pSelected->setAlign(onut::eUIAlign::LEFT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkCENTER)
    {
        pSelected->setAnchorPercent({.5f, .5f});
        pSelected->setAlign(onut::eUIAlign::CENTER);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkRIGHT)
    {
        pSelected->setAnchorPercent({1, .5f});
        pSelected->setAlign(onut::eUIAlign::RIGHT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM_LEFT)
    {
        pSelected->setAnchorPercent({0, 1});
        pSelected->setAlign(onut::eUIAlign::BOTTOM_LEFT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM)
    {
        pSelected->setAnchorPercent({.5f, 1});
        pSelected->setAlign(onut::eUIAlign::BOTTOM);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM_RIGHT)
    {
        pSelected->setAnchorPercent({1, 1});
        pSelected->setAlign(onut::eUIAlign::BOTTOM_RIGHT);
    }
    auto newAnchor = pSelected->getAnchor();
    auto newAlign = pSelected->getAlign();

    g_actionManager.doAction(new onut::Action(
        [=]{
        pSelected->setRect(newRect);
        pSelected->setAnchor(newAnchor);
        pSelected->setAlign(newAlign);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setRect(previousRect);
        pSelected->setAnchor(previousAnchor);
        pSelected->setAlign(previousAlign);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void hookUIEvents(onut::UIControl* pUIScreen)
{
    // Tool Bar


    // Tool box
    pUIScreen->getChild("btnCreateControl")->onClick = onCreateControl;
    pUIScreen->getChild("btnCreatePanel")->onClick = onCreatePanel;
    pUIScreen->getChild("btnCreateButton")->onClick = onCreateButton;
    pUIScreen->getChild("btnCreateLabel")->onClick = onCreateLabel;
    pUIScreen->getChild("btnCreateImage")->onClick = onCreateImage;

    // View
    pUIScreen->getChild("pnlView")->onMouseDown = onSelect;
    pUIScreen->getChild("pnlView")->onMouseUp = onSelectUp;
    pUIScreen->getChild("pnlRegion")->onMouseDown = onSelect;
    pUIScreen->getChild("pnlRegion")->onMouseUp = onSelectUp;

    // Scene Graph
    dynamic_cast<onut::UITreeView*>(pUIScreen->getChild("sceneGraph"))->onSelectionChanged = onSceneGraphSelectionChanged;

    // Inspector
    g_pInspector_UIControl_chkEnabled = pUIScreen->getChild<onut::UICheckBox>("chkEnabled");
    g_pInspector_UIControl_chkEnabled->onCheckChanged = onUIControlEnableChanged;

    g_pInspector_UIControl_chkVisible = pUIScreen->getChild<onut::UICheckBox>("chkVisible");
    g_pInspector_UIControl_chkVisible->onCheckChanged = onUIControlVisibleChanged;

    g_pInspector_UIControl_chkClickThrough = pUIScreen->getChild<onut::UICheckBox>("chkClickThrough");
    g_pInspector_UIControl_chkClickThrough->onCheckChanged = onUIControlClickThroughChanged;

    g_pInspector_UIControl_txtName = pUIScreen->getChild<onut::UITextBox>("txtName");
    g_pInspector_UIControl_txtName->onTextChanged = onUIControlNameChanged;

    g_pInspector_UIControl_txtStyle = pUIScreen->getChild<onut::UITextBox>("txtStyle");
    g_pInspector_UIControl_txtStyle->onTextChanged = onUIControlStyleChanged;

    g_pInspector_UIControl_txtX = pUIScreen->getChild<onut::UITextBox>("txtX");
    g_pInspector_UIControl_txtX->onTextChanged = onUIControlXChanged;
    g_pInspector_UIControl_txtY = pUIScreen->getChild<onut::UITextBox>("txtY");
    g_pInspector_UIControl_txtY->onTextChanged = onUIControlYChanged;
    g_pInspector_UIControl_chkXPercent = pUIScreen->getChild<onut::UICheckBox>("chkXPercent");
    g_pInspector_UIControl_chkXPercent->onCheckChanged = onUIControlXPercentChanged;
    g_pInspector_UIControl_chkYPercent = pUIScreen->getChild<onut::UICheckBox>("chkYPercent");
    g_pInspector_UIControl_chkYPercent->onCheckChanged = onUIControlYPercentChanged;

    g_pInspector_UIControl_txtWidth = pUIScreen->getChild<onut::UITextBox>("txtWidth");
    g_pInspector_UIControl_txtWidth->onTextChanged = onUIControlWidthChanged;
    g_pInspector_UIControl_txtHeight = pUIScreen->getChild<onut::UITextBox>("txtHeight");
    g_pInspector_UIControl_txtHeight->onTextChanged = onUIControlHeightChanged;
    g_pInspector_UIControl_chkWidthPercent = pUIScreen->getChild<onut::UICheckBox>("chkWidthPercent");
    g_pInspector_UIControl_chkWidthPercent->onCheckChanged = onUIControlWidthPercentChanged;
    g_pInspector_UIControl_chkHeightPercent = pUIScreen->getChild<onut::UICheckBox>("chkHeightPercent");
    g_pInspector_UIControl_chkHeightPercent->onCheckChanged = onUIControlHeightPercentChanged;
    g_pInspector_UIControl_chkWidthRelative = pUIScreen->getChild<onut::UICheckBox>("chkWidthRelative");
    g_pInspector_UIControl_chkWidthRelative->onCheckChanged = onUIControlWidthRelativeChanged;
    g_pInspector_UIControl_chkHeightRelative = pUIScreen->getChild<onut::UICheckBox>("chkHeightRelative");
    g_pInspector_UIControl_chkHeightRelative->onCheckChanged = onUIControlHeightRelativeChanged;

    g_pInspector_UIControl_chkEnabled = pUIScreen->getChild<onut::UICheckBox>("chkEnabled");
    g_pInspector_UIControl_chkTOP_LEFT = pUIScreen->getChild<onut::UICheckBox>("chkTOP_LEFT");
    g_pInspector_UIControl_chkTOP = pUIScreen->getChild<onut::UICheckBox>("chkTOP");
    g_pInspector_UIControl_chkTOP_RIGHT = pUIScreen->getChild<onut::UICheckBox>("chkTOP_RIGHT");
    g_pInspector_UIControl_chkLEFT = pUIScreen->getChild<onut::UICheckBox>("chkLEFT");
    g_pInspector_UIControl_chkCENTER = pUIScreen->getChild<onut::UICheckBox>("chkCENTER");
    g_pInspector_UIControl_chkRIGHT = pUIScreen->getChild<onut::UICheckBox>("chkRIGHT");
    g_pInspector_UIControl_chkBOTTOM_LEFT = pUIScreen->getChild<onut::UICheckBox>("chkBOTTOM_LEFT");
    g_pInspector_UIControl_chkBOTTOM = pUIScreen->getChild<onut::UICheckBox>("chkBOTTOM");
    g_pInspector_UIControl_chkBOTTOM_RIGHT = pUIScreen->getChild<onut::UICheckBox>("chkBOTTOM_RIGHT");

    g_pInspector_UIControl_txtAnchorX = pUIScreen->getChild<onut::UITextBox>("txtAnchorX");
    g_pInspector_UIControl_txtAnchorX->onTextChanged = onUIControlAnchorXChanged;
    g_pInspector_UIControl_txtAnchorY = pUIScreen->getChild<onut::UITextBox>("txtAnchorY");
    g_pInspector_UIControl_txtAnchorY->onTextChanged = onUIControlAnchorYChanged;
    g_pInspector_UIControl_chkXAnchorPercent = pUIScreen->getChild<onut::UICheckBox>("chkXAnchorPercent");
    g_pInspector_UIControl_chkXAnchorPercent->onCheckChanged = onUIControlXAnchorPercentChanged;
    g_pInspector_UIControl_chkYAnchorPercent = pUIScreen->getChild<onut::UICheckBox>("chkYAnchorPercent");
    g_pInspector_UIControl_chkYAnchorPercent->onCheckChanged = onUIControlYAnchorPercentChanged;
    g_pInspector_UIControl_chkAnchorTOP_LEFT = pUIScreen->getChild<onut::UIButton>("chkAnchorTOP_LEFT");
    g_pInspector_UIControl_chkAnchorTOP = pUIScreen->getChild<onut::UIButton>("chkAnchorTOP");
    g_pInspector_UIControl_chkAnchorTOP_RIGHT = pUIScreen->getChild<onut::UIButton>("chkAnchorTOP_RIGHT");
    g_pInspector_UIControl_chkAnchorLEFT = pUIScreen->getChild<onut::UIButton>("chkAnchorLEFT");
    g_pInspector_UIControl_chkAnchorCENTER = pUIScreen->getChild<onut::UIButton>("chkAnchorCENTER");
    g_pInspector_UIControl_chkAnchorRIGHT = pUIScreen->getChild<onut::UIButton>("chkAnchorRIGHT");
    g_pInspector_UIControl_chkAnchorBOTTOM_LEFT = pUIScreen->getChild<onut::UIButton>("chkAnchorBOTTOM_LEFT");
    g_pInspector_UIControl_chkAnchorBOTTOM = pUIScreen->getChild<onut::UIButton>("chkAnchorBOTTOM");
    g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT = pUIScreen->getChild<onut::UIButton>("chkAnchorBOTTOM_RIGHT");

    g_pInspector_UIControl_chkTOP_LEFT->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkTOP->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkTOP_RIGHT->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkLEFT->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkCENTER->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkRIGHT->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkBOTTOM_LEFT->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkBOTTOM->onCheckChanged = onAlignChkChanged;
    g_pInspector_UIControl_chkBOTTOM_RIGHT->onCheckChanged = onAlignChkChanged;

    g_pInspector_UIControl_chkAnchorTOP_LEFT->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorTOP->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorTOP_RIGHT->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorLEFT->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorCENTER->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorRIGHT->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorBOTTOM_LEFT->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorBOTTOM->onClick = onAnchorClicked;
    g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT->onClick = onAnchorClicked;

    // Turn textbox into text cursor
    static HCURSOR curARROW = LoadCursor(nullptr, IDC_ARROW);
    static HCURSOR curIBEAM = LoadCursor(nullptr, IDC_IBEAM);
    g_pInspector_UIControl_txtName->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtName->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
}

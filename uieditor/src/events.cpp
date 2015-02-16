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

namespace onut
{
    const char* getStringFromType(eUIType type);
}

std::string getControlName(onut::UIControl* pControl)
{
    std::string ret = pControl->getName();
    if (ret.empty())
    {
        ret = onut::getStringFromType(pControl->getType());
    }
    return std::move(ret);
}

void createControlAction(onut::UIControl* pControl, onut::UIControl* pParent)
{
    auto pSelected = g_pDocument->pSelected;
    g_actionManager.doAction(new onut::Action("Create " + getControlName(pControl),
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

    std::string ctrlName = "Unselect";
    if (pPickedControl)
    {
        ctrlName = "Select " + getControlName(pControl);
    }

    g_actionManager.doAction(new onut::Action(ctrlName,
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
        g_actionManager.doAction(new onut::Action("Unselect",
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
        g_actionManager.doAction(new onut::Action("Select " + getControlName(pSelected),
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

    g_actionManager.doAction(new onut::Action("Rename " + getControlName(pControl) + " to " + text,
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

void doRectChange(const std::string& actionName, onut::UIControl* pControl, const onut::sUIRect& rect)
{
    if (!pControl) return;
    auto previousRect = pControl->getRect();
    g_actionManager.doAction(new onut::Action(actionName, 
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
    doRectChange("Edit X position", g_pDocument->pSelected, rect);
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
    doRectChange("Edit Y position", g_pDocument->pSelected, rect);
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
    g_actionManager.doAction(new onut::Action("Edit X anchor",
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
    g_actionManager.doAction(new onut::Action("Edit Y anchor",
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
    doRectChange("Edit Width", g_pDocument->pSelected, rect);
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
    doRectChange("Edit Height", g_pDocument->pSelected, rect);
}

void onUIControlStyleChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevStyle = pSelected->getStyleName();
    auto newStyle = pTextBox->getText();
    g_actionManager.doAction(new onut::Action("Edit Style",
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
    g_actionManager.doAction(new onut::Action("Toggle Enable",
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
    g_actionManager.doAction(new onut::Action("Toggle Visible", 
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
    g_actionManager.doAction(new onut::Action("Toggle ClickThrough", 
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
    g_actionManager.doAction(new onut::Action("Toggle X anchor percent",
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
    g_actionManager.doAction(new onut::Action("Toggle Y anchor percent",
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
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->getRect();
    auto prevDimType = pSelected->getWidthType();
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && prevDimType != onut::eUIDimType::DIM_PERCENTAGE)
    {
        auto worldRect = pSelected->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_PERCENTAGE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.size.x = worldRect.size.x / parentRect.size.x;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkWidthRelative->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_ABSOLUTE;
        newRect.size.x = newRect.size.x * parentRect.size.x;
    }
    g_actionManager.doAction(new onut::Action("Toggle Width percent",
        [=]{
        pSelected->setWidthType(newDimType);
        pSelected->setRect(newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setWidthType(prevDimType);
        pSelected->setRect(prevRect);
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

void onUIControlHeightPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->getRect();
    auto prevDimType = pSelected->getHeightType();
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && prevDimType != onut::eUIDimType::DIM_PERCENTAGE)
    {
        auto worldRect = pSelected->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_PERCENTAGE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.size.y = worldRect.size.y / parentRect.size.y;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkHeightRelative->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_ABSOLUTE;
        newRect.size.y = newRect.size.y * parentRect.size.y;
    }
    g_actionManager.doAction(new onut::Action("Toggle Height percent",
        [=]{
        pSelected->setHeightType(newDimType);
        pSelected->setRect(newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setHeightType(prevDimType);
        pSelected->setRect(prevRect);
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

void onUIControlWidthRelativeChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->getRect();
    auto prevDimType = pSelected->getWidthType();
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && pSelected->getWidthType() != onut::eUIDimType::DIM_RELATIVE)
    {
        auto worldRect = pSelected->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_RELATIVE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.size.x = worldRect.size.x - parentRect.size.x;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkWidthPercent->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_ABSOLUTE;
        newRect.size.x = parentRect.size.x + newRect.size.x;
    }
    g_actionManager.doAction(new onut::Action("Toggle Width relative",
        [=]{
        pSelected->setWidthType(newDimType);
        pSelected->setRect(newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setWidthType(prevDimType);
        pSelected->setRect(prevRect);
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

void onUIControlHeightRelativeChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->getRect();
    auto prevDimType = pSelected->getHeightType();
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && pSelected->getHeightType() != onut::eUIDimType::DIM_RELATIVE)
    {
        auto worldRect = pSelected->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_RELATIVE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.size.y = worldRect.size.y - parentRect.size.y;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkHeightPercent->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newDimType = onut::eUIDimType::DIM_ABSOLUTE;
        newRect.size.y = parentRect.size.y + newRect.size.y;
    }
    g_actionManager.doAction(new onut::Action("Toggle Height relative",
        [=]{
        pSelected->setHeightType(newDimType);
        pSelected->setRect(newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setHeightType(prevDimType);
        pSelected->setRect(prevRect);
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

void onUIControlXPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevType = pSelected->getXType();
    auto prevRect = pSelected->getRect();
    auto newType = prevType;
    auto newRect = prevRect;
    if (pCheckBox->getIsChecked() && prevType != onut::eUIPosType::POS_PERCENTAGE)
    {
        newType = onut::eUIPosType::POS_PERCENTAGE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.position.x = newRect.position.x / parentRect.size.x;
    }
    else if (!pCheckBox->getIsChecked() && prevType == onut::eUIPosType::POS_PERCENTAGE)
    {
        newType = onut::eUIPosType::POS_RELATIVE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.position.x = newRect.position.x * parentRect.size.x;
    }
    g_actionManager.doAction(new onut::Action("Toggle X percent",
        [=]{
        pSelected->setXType(newType);
        pSelected->setRect(newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setXType(prevType);
        pSelected->setRect(prevRect);
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

void onUIControlYPercentChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevType = pSelected->getYType();
    auto prevRect = pSelected->getRect();
    auto newType = prevType;
    auto newRect = prevRect;
    if (pCheckBox->getIsChecked() && prevType != onut::eUIPosType::POS_PERCENTAGE)
    {
        newType = onut::eUIPosType::POS_PERCENTAGE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.position.y = newRect.position.y / parentRect.size.y;
    }
    else if (!pCheckBox->getIsChecked() && prevType == onut::eUIPosType::POS_PERCENTAGE)
    {
        newType = onut::eUIPosType::POS_RELATIVE;
        auto& parentRect = pSelected->getParent()->getWorldRect(*g_pDocument->pUIContext);
        newRect.position.y = newRect.position.y * parentRect.size.y;
    }
    g_actionManager.doAction(new onut::Action("Toggle Y percent",
        [=]{
        pSelected->setYType(newType);
        pSelected->setRect(newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setYType(prevType);
        pSelected->setRect(prevRect);
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

    g_actionManager.doAction(new onut::Action("Change Anchor",
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

    g_actionManager.doAction(new onut::Action("Change Align",
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

std::string fileOpen(const char* szFilters)
{
    auto window = OWindow->getHandle();
    char szFileName[MAX_PATH] = "";

    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = window;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = window;
    ofn.lpstrFilter = szFilters;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = TEXT("json");

    GetOpenFileName(&ofn);

    return ofn.lpstrFile;
}

extern std::unordered_map<onut::eUIType, onut::UIControl*> g_controlInspectorMap;
extern std::unordered_map<onut::eUIType, std::vector<IControlInspectorBind*>> g_inspectorBindings;
float yPos;
std::vector<IControlInspectorBind*>* pBindings;
onut::UIPanel* pPnl;
static HCURSOR curARROW;
static HCURSOR curIBEAM;

void BEGIN_BINDINGS(onut::UIControl* pUIScreen, onut::eUIType type, const std::string& name)
{
    pPnl = pUIScreen->getChild<onut::UIPanel>(name);
    g_controlInspectorMap[type] = pPnl;
    pBindings = &(g_inspectorBindings[type]);
    yPos = 24.f;
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_TEXT_PROPERTY(const std::string& name, const Tgetter& getter, const Tsetter& setter)
{
    auto pLabel = new onut::UILabel();
    auto pTextBox = new onut::UITextBox();

    pLabel->setText(name);
    pLabel->setWidthType(onut::eUIDimType::DIM_PERCENTAGE);
    pLabel->setRect({{4, yPos}, {0.382f, 24}});

    pTextBox->setAlign(onut::eUIAlign::TOP_RIGHT);
    pTextBox->setWidthType(onut::eUIDimType::DIM_PERCENTAGE);
    pTextBox->setRect({{-4, yPos}, {0.618f, 24}});
    pTextBox->setAnchor({1, 0});

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pTextBox);

    auto pBinding = new ControlInspectorBind<std::string, TuiType, Tgetter, Tsetter>
        (std::string("Edit ") + name,
        std::bind(&onut::UITextBox::getText, pTextBox),
        std::bind(&onut::UITextBox::setText, pTextBox, std::placeholders::_1),
        getter, setter);
    pBindings->push_back(pBinding);

    pTextBox->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_FILE_PROPERTY(const std::string& name, const char* szFilter, const Tgetter& getter, const Tsetter& setter)
{
    auto pLabel = new onut::UILabel();
    auto pContainer = new onut::UIControl();
    auto pTextBox = new onut::UITextBox();
    auto pBrowseButton = new onut::UIButton();

    pLabel->setText(name);
    pLabel->setWidthType(onut::eUIDimType::DIM_PERCENTAGE);
    pLabel->setRect({{4, yPos}, {0.382f, 24}});

    pContainer->setAlign(onut::eUIAlign::TOP_RIGHT);
    pContainer->setWidthType(onut::eUIDimType::DIM_PERCENTAGE);
    pContainer->setRect({{-4, yPos}, {0.618f, 24}});
    pContainer->setAnchor({1, 0});

    pTextBox->setWidthType(onut::eUIDimType::DIM_RELATIVE);
    pTextBox->setRect({{0, 0}, {-32, 24}});

    pBrowseButton->setAlign(onut::eUIAlign::TOP_RIGHT);
    pBrowseButton->setAnchor({1, 0});
    pBrowseButton->setRect({{0, 0}, {32, 24}});
    pBrowseButton->setCaption("...");

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pContainer);
    pContainer->add(pTextBox);
    pContainer->add(pBrowseButton);

    auto pBinding = new ControlInspectorBind<std::string, TuiType, Tgetter, Tsetter>
        (std::string("Edit ") + name,
        std::bind(&onut::UITextBox::getText, pTextBox),
        std::bind(&onut::UITextBox::setText, pTextBox, std::placeholders::_1),
        getter, setter);
    pBindings->push_back(pBinding);

    pTextBox->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
    pBrowseButton->onClick = [=](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
    {
        std::string file = fileOpen(szFilter);
        if (!file.empty())
        {
            // Make it relative to our filename
            auto filename = onut::getPath(g_pDocument->getFilename());
            file = onut::makeRelativePath(file, filename);
            pTextBox->setText(file);
            if (pTextBox->onTextChanged)
            {
                onut::UITextBoxEvent evt;
                evt.pContext = g_pUIContext;
                pTextBox->onTextChanged(pTextBox, evt);
            }
        }
    };
}

void hookUIEvents(onut::UIControl* pUIScreen)
{
    curARROW = LoadCursor(nullptr, IDC_ARROW);
    curIBEAM = LoadCursor(nullptr, IDC_IBEAM);

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
    g_pInspector_UIControl_txtName->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtName->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };

    g_pInspector_UIControl_txtStyle = pUIScreen->getChild<onut::UITextBox>("txtStyle");
    g_pInspector_UIControl_txtStyle->onTextChanged = onUIControlStyleChanged;
    g_pInspector_UIControl_txtStyle->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtStyle->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };

    g_pInspector_UIControl_txtX = pUIScreen->getChild<onut::UITextBox>("txtX");
    g_pInspector_UIControl_txtX->onTextChanged = onUIControlXChanged;
    g_pInspector_UIControl_txtX->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtX->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    g_pInspector_UIControl_txtY = pUIScreen->getChild<onut::UITextBox>("txtY");
    g_pInspector_UIControl_txtY->onTextChanged = onUIControlYChanged;
    g_pInspector_UIControl_txtY->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtY->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    g_pInspector_UIControl_chkXPercent = pUIScreen->getChild<onut::UICheckBox>("chkXPercent");
    g_pInspector_UIControl_chkXPercent->onCheckChanged = onUIControlXPercentChanged;
    g_pInspector_UIControl_chkYPercent = pUIScreen->getChild<onut::UICheckBox>("chkYPercent");
    g_pInspector_UIControl_chkYPercent->onCheckChanged = onUIControlYPercentChanged;

    g_pInspector_UIControl_txtWidth = pUIScreen->getChild<onut::UITextBox>("txtWidth");
    g_pInspector_UIControl_txtWidth->onTextChanged = onUIControlWidthChanged;
    g_pInspector_UIControl_txtWidth->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtWidth->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    g_pInspector_UIControl_txtHeight = pUIScreen->getChild<onut::UITextBox>("txtHeight");
    g_pInspector_UIControl_txtHeight->onTextChanged = onUIControlHeightChanged;
    g_pInspector_UIControl_txtHeight->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtHeight->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
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
    g_pInspector_UIControl_txtAnchorX->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtAnchorX->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    g_pInspector_UIControl_txtAnchorY = pUIScreen->getChild<onut::UITextBox>("txtAnchorY");
    g_pInspector_UIControl_txtAnchorY->onTextChanged = onUIControlAnchorYChanged;
    g_pInspector_UIControl_txtAnchorY->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtAnchorY->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
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

    //--- Controls inspector bindings

    // UIPanel
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_PANEL, "pnlInspector_UIPanel");
    //BIND_TEXT_PROPERTY("Caption", onut::UIButton, onut::UIButton::getCaption, onut::UIButton::setCaption);

    // UIButton
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_BUTTON, "pnlInspector_UIButton");
    BIND_TEXT_PROPERTY<onut::UIButton>("Caption",
                                       std::bind(&onut::UIButton::getCaption, std::placeholders::_1),
                                       std::bind(&onut::UIButton::setCaption, std::placeholders::_1, std::placeholders::_2));

    // UILabel
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_LABEL, "pnlInspector_UILabel");
    BIND_TEXT_PROPERTY<onut::UILabel>("Text",
                                      std::bind(&onut::UILabel::getText, std::placeholders::_1),
                                      std::bind(&onut::UILabel::setText, std::placeholders::_1, std::placeholders::_2));

    // UIImage
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_IMAGE, "pnlInspector_UIImage");
    BIND_FILE_PROPERTY<onut::UIImage>("Image",
                                      TEXT("Image Files (*.png)\0*.png\0All Files (*.*)\0*.*\0"),
                                      std::bind(&onut::UIImage::getImage, std::placeholders::_1),
                                      std::bind(&onut::UIImage::setImage, std::placeholders::_1, std::placeholders::_2));
}

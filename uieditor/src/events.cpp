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
onut::UICheckBox*    g_pInspector_UIControl_chkClipChildren;
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

std::string getControlName(onut::UIControl* pControl)
{
    std::string ret = pControl->name;
    if (ret.empty())
    {
        switch (pControl->getType())
        {
            case onut::eUIType::UI_BUTTON: return "UIButton";
            case onut::eUIType::UI_CHECKBOX: return "UICheckBox";
            case onut::eUIType::UI_CONTROL: return "UIControl";
            case onut::eUIType::UI_IMAGE: return "UIImage";
            case onut::eUIType::UI_LABEL: return "UILabel";
            case onut::eUIType::UI_PANEL: return "UIPanel";
            case onut::eUIType::UI_TEXTBOX: return "UITextBox";
            case onut::eUIType::UI_TREEVIEW: return "UITreeView";
        }
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

    pCtrl->rect = {{0, 0}, {100, 100}};
    createControlAction(pCtrl, pParent);
}

void onCreatePanel(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pPanel = new onut::UIPanel();

    pPanel->rect = {{0, 0}, {100, 100}};
    createControlAction(pPanel, pParent);
}

void onCreateButton(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = new onut::UIButton();

    pBtn->textComponent.text = "Button";
    pBtn->rect = {{0, 0}, {64, 24}};
    createControlAction(pBtn, pParent);
}

void onCreateLabel(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pLbl = new onut::UILabel();

    pLbl->textComponent.text = "Label";
    pLbl->rect = {{0, 0}, {64, 20}};
    createControlAction(pLbl, pParent);
}

void onCreateImage(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pImg = new onut::UIImage();

    pImg->rect = {{0, 0}, {32, 32}};
    createControlAction(pImg, pParent);
}

void onCreateCheckBox(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = new onut::UICheckBox();

    pBtn->textComponent.text = "CheckBox";
    pBtn->rect = {{0, 0}, {64, 24}};
    createControlAction(pBtn, pParent);
}

void onCreateTextBox(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = new onut::UITextBox();

    pBtn->textComponent.text = "";
    pBtn->rect = {{0, 0}, {64, 24}};
    createControlAction(pBtn, pParent);
}

void onCreateTreeView(onut::UIControl* pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pTreeView = new onut::UITreeView();

    pTreeView->rect = {{0, 0}, {64, 24}};
    createControlAction(pTreeView, pParent);
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
        auto pSelected = static_cast<onut::UIControl*>(pViewItem->pUserData);

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

    auto text = pTextBox->textComponent.text;
    auto pControl = g_pDocument->pSelected;
    auto previousText = pControl->name;

    g_actionManager.doAction(new onut::Action("Rename " + getControlName(pControl) + " to " + text,
        [=]{
        pControl->name = text;
        auto pViewItem = static_cast<onut::UITreeViewItem*>(pControl->pUserData);
        if (pViewItem)
        {
            pViewItem->text = text;
        }
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->name = previousText;
        auto pViewItem = static_cast<onut::UITreeViewItem*>(pControl->pUserData);
        if (pViewItem)
        {
            pViewItem->text  =previousText;
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
    auto previousRect = pControl->rect;
    g_actionManager.doAction(new onut::Action(actionName, 
        [=]{
        pControl->rect = rect;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->rect = previousRect;
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
    auto rect = g_pDocument->pSelected->rect;
    rect.position.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->xType == onut::eUIPosType::POS_PERCENTAGE)
    {
        rect.position.x /= 100.f;
    }
    doRectChange("Edit X position", g_pDocument->pSelected, rect);
}
void onUIControlYChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->rect;
    rect.position.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->yType == onut::eUIPosType::POS_PERCENTAGE)
    {
        rect.position.y /= 100.f;
    }
    doRectChange("Edit Y position", g_pDocument->pSelected, rect);
}

void onUIControlAnchorXChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = g_pDocument->pSelected->anchor;
    auto newAnchor = prevAnchor;
    newAnchor.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->xAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
    {
        newAnchor.x /= 100.f;
    }
    g_actionManager.doAction(new onut::Action("Edit X anchor",
        [=]{
        pSelected->anchor = newAnchor;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
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
    auto prevAnchor = g_pDocument->pSelected->anchor;
    auto newAnchor = prevAnchor;
    newAnchor.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->yAnchorType == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
    {
        newAnchor.y /= 100.f;
    }
    g_actionManager.doAction(new onut::Action("Edit Y anchor",
        [=]{
        pSelected->anchor = newAnchor;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
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
    auto rect = g_pDocument->pSelected->rect;
    rect.size.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->widthType == onut::eUIDimType::DIM_PERCENTAGE)
    {
        rect.size.x /= 100.f;
    }
    doRectChange("Edit Width", g_pDocument->pSelected, rect);
}

void onUIControlHeightChanged(onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->rect;
    rect.size.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->heightType == onut::eUIDimType::DIM_PERCENTAGE)
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
    auto newStyle = pTextBox->textComponent.text;
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
        pSelected->isEnabled = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->isEnabled = !bValue;
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
        pSelected->isVisible = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->isVisible = !bValue;
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
        pSelected->isClickThrough = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->isClickThrough = !bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->retain();
    },
        [=]{
        pSelected->release();
    }));
}

void onUIControlClipChildrenChanged(onut::UICheckBox* pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    g_actionManager.doAction(new onut::Action("Toggle ClipChildren", 
        [=]{
        pSelected->clipChildren = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->clipChildren = !bValue;
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
    auto prevAnchor = pSelected->anchor;
    auto prevAnchorType = pSelected->xAnchorType;
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
        pSelected->anchor = newAnchor;
        pSelected->xAnchorType = newAnchorType;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
        pSelected->xAnchorType = prevAnchorType;
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
    auto prevAnchor = pSelected->anchor;
    auto prevAnchorType = pSelected->yAnchorType;
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
        pSelected->anchor = newAnchor;
        pSelected->yAnchorType = newAnchorType;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
        pSelected->yAnchorType = prevAnchorType;
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
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->widthType;
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
        pSelected->widthType = (newDimType);
        pSelected->rect = (newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->widthType = (prevDimType);
        pSelected->rect = (prevRect);
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
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->heightType;
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
        pSelected->heightType = (newDimType);
        pSelected->rect = (newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->heightType = (prevDimType);
        pSelected->rect = (prevRect);
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
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->widthType;
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && pSelected->widthType != onut::eUIDimType::DIM_RELATIVE)
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
        pSelected->widthType = (newDimType);
        pSelected->rect = (newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->widthType = (prevDimType);
        pSelected->rect = (prevRect);
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
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->heightType;
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && pSelected->heightType != onut::eUIDimType::DIM_RELATIVE)
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
        pSelected->heightType = (newDimType);
        pSelected->rect = (newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->heightType = (prevDimType);
        pSelected->rect = (prevRect);
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
    auto prevType = pSelected->xType;
    auto prevRect = pSelected->rect;
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
        pSelected->xType = (newType);
        pSelected->rect = (newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->xType = (prevType);
        pSelected->rect = (prevRect);
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
    auto prevType = pSelected->yType;
    auto prevRect = pSelected->rect;
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
        pSelected->yType = (newType);
        pSelected->rect = (newRect);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->yType = (prevType);
        pSelected->rect = (prevRect);
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
    onut::sUIVector2 prevAnchor = pSelected->anchor;
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
    onut::sUIVector2 newAnchor = pSelected->anchor;

    g_actionManager.doAction(new onut::Action("Change Anchor",
        [=]{
        pSelected->anchor = (newAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = (prevAnchor);
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
    auto previousRect = pSelected->rect;
    auto newRect = previousRect;
    newRect.position.x = 0;
    newRect.position.y = 0;
    auto previousAnchor = pSelected->anchor;
    auto previousAlign = pSelected->align;
    if (pCheckBox == g_pInspector_UIControl_chkTOP_LEFT)
    {
        pSelected->setAnchorPercent({0, 0});
        pSelected->align = (onut::eUIAlign::TOP_LEFT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkTOP)
    {
        pSelected->setAnchorPercent({.5f, 0});
        pSelected->align = (onut::eUIAlign::TOP);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkTOP_RIGHT)
    {
        pSelected->setAnchorPercent({1, 0});
        pSelected->align = (onut::eUIAlign::TOP_RIGHT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkLEFT)
    {
        pSelected->setAnchorPercent({0, .5f});
        pSelected->align = (onut::eUIAlign::LEFT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkCENTER)
    {
        pSelected->setAnchorPercent({.5f, .5f});
        pSelected->align = (onut::eUIAlign::CENTER);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkRIGHT)
    {
        pSelected->setAnchorPercent({1, .5f});
        pSelected->align = (onut::eUIAlign::RIGHT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM_LEFT)
    {
        pSelected->setAnchorPercent({0, 1});
        pSelected->align = (onut::eUIAlign::BOTTOM_LEFT);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM)
    {
        pSelected->setAnchorPercent({.5f, 1});
        pSelected->align = (onut::eUIAlign::BOTTOM);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM_RIGHT)
    {
        pSelected->setAnchorPercent({1, 1});
        pSelected->align = (onut::eUIAlign::BOTTOM_RIGHT);
    }
    auto newAnchor = pSelected->anchor;
    auto newAlign = pSelected->align;

    g_actionManager.doAction(new onut::Action("Change Align",
        [=]{
        pSelected->rect = (newRect);
        pSelected->anchor = (newAnchor);
        pSelected->align = (newAlign);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->rect = (previousRect);
        pSelected->anchor = (previousAnchor);
        pSelected->align = (previousAlign);
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

    OPENFILENAMEA ofn = {0};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = window;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = window;
    ofn.lpstrFilter = szFilters;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "json";

    GetOpenFileNameA(&ofn);

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

static COLORREF g_acrCustClr[16]; // array of custom colors

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_COLOR_PROPERTY(const std::string& name, Tgetter getter, Tsetter setter)
{
    auto pLabel = new onut::UILabel();
    auto pButton = new onut::UIPanel();
    auto pAlphaLabel = new onut::UILabel();
    auto pAlphaText = new onut::UITextBox();

    pLabel->textComponent.text = name;
    pLabel->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pButton->setStyle("colorPicker");
    pButton->align = (onut::eUIAlign::TOP_RIGHT);
    pButton->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pButton->rect = {{-4, yPos}, {0.618f, 24}};
    pButton->anchor = {1, 0};

    yPos += 24 + 4;

    pAlphaLabel->textComponent.text = "Alpha:";
    pAlphaLabel->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pAlphaLabel->align = (onut::eUIAlign::TOP_RIGHT);
    pAlphaLabel->rect = {{-4 - 64 - 4, yPos}, {64, 24}};
    pAlphaLabel->anchor = {1, 0};

    pAlphaText->setInt(100);
    pAlphaText->setIsNumerical(true);
    pAlphaText->align = (onut::eUIAlign::TOP_RIGHT);
    pAlphaText->rect = {{-4, yPos}, {64, 24}};
    pAlphaText->anchor = {1, 0};

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pButton);
    pPnl->add(pAlphaLabel);
    pPnl->add(pAlphaText);

    {
        auto pBinding = new ControlInspectorBind<onut::sUIColor, TuiType>
            (std::string("Edit ") + name, &pButton->color, getter, setter);
        pBindings->push_back(pBinding);

        pButton->onClick = [=](onut::UIControl* pControl, const onut::UIMouseEvent& evt)
        {
            CHOOSECOLOR colorChooser = {0};
            DWORD rgbCurrent; // initial color selection
            rgbCurrent = (DWORD)pButton->color.packed;
            auto alphaVal = rgbCurrent & 0x000000ff;
            rgbCurrent = ((rgbCurrent >> 24) & 0x000000ff) | ((rgbCurrent >> 8) & 0x0000ff00) | ((rgbCurrent << 8) & 0x00ff0000);
            colorChooser.lStructSize = sizeof(colorChooser);
            colorChooser.hwndOwner = OWindow->getHandle();
            colorChooser.lpCustColors = (LPDWORD)g_acrCustClr;
            colorChooser.rgbResult = rgbCurrent;
            colorChooser.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&colorChooser) == TRUE)
            {
                onut::sUIColor color;
                rgbCurrent = colorChooser.rgbResult;
                color.packed = ((rgbCurrent << 24) & 0xff000000) | ((rgbCurrent << 8) & 0x00ff0000) | ((rgbCurrent >> 8) & 0x0000ff00) | alphaVal;
                color.unpack();
                pButton->color = color;
                pBinding->updateControl(g_pDocument->pSelected);
            }
        };
    }

    { // alpha
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            "Alpha", nullptr,
            [](TuiType* pControl) {return pControl->color.a * 100.f; },
            [](TuiType* pControl, const float& alpha){pControl->color.a = alpha / 100.f; pControl->color.pack(); },
            [=]{return pAlphaText->getFloat(); },
            [=](const float& alpha) {pAlphaText->setFloat(alpha); });
        pBindings->push_back(pBinding);

        pAlphaText->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
}

template<typename TuiType>
void BIND_SCALE9_COMPONENT(const std::string& name)
{
    auto pLabel = new onut::UILabel();
    auto pContainer = new onut::UIPanel();
    auto pTxtImage = new onut::UITextBox();
    auto pBrowseButton = new onut::UIButton();
    auto pBtnColor = new onut::UIPanel();
    auto pChkScale9 = new onut::UICheckBox();
    auto pChkRepeat = new onut::UICheckBox();
    auto pBtnFit = new onut::UIButton();
    auto pLblPadding = new onut::UILabel();
    auto pTxtPaddingLeft = new onut::UITextBox();
    auto pTxtPaddingRight = new onut::UITextBox();
    auto pTxtPaddingTop = new onut::UITextBox();
    auto pTxtPaddingBottom = new onut::UITextBox();

    pLabel->textComponent.text = name;
    pLabel->rect = {{4, yPos}, {58, 174 - 66}};

    pContainer->setStyle("group");
    pContainer->widthType = (onut::eUIDimType::DIM_RELATIVE);
    pContainer->rect = {{66, yPos}, {-70, 174 - 66}};

    pTxtImage->widthType = (onut::eUIDimType::DIM_RELATIVE);
    pTxtImage->rect = {{4, 4}, {-8 - 32, 24}};

    pBrowseButton->align = (onut::eUIAlign::TOP_RIGHT);
    pBrowseButton->anchor = {1, 0};
    pBrowseButton->rect = {{-4, 4}, {32, 24}};
    pBrowseButton->textComponent.text = "...";

    pBtnColor->setStyle("colorPicker");
    pBtnColor->rect = {{4, 33}, {26, 24}};

    pChkScale9->textComponent.text = "Scale 9";
    pChkScale9->rect = {{34, 33}, {118, 24}};

    pChkRepeat->textComponent.text = "Repeat";
    pChkRepeat->rect = {{98, 26}, {118, 24}};

    pBtnFit->textComponent.text = "Fit";
    pBtnFit->anchor = {1, 0};
    pBtnFit->align = (onut::eUIAlign::TOP_RIGHT);
    pBtnFit->rect = {{-4, 33}, {32, 24}};

    pLblPadding->textComponent.text = "Padding";
    pLblPadding->rect = {{4, 118 - 66}, {53 - 8, 50}};

    pTxtPaddingLeft->textComponent.text = "0";
    pTxtPaddingLeft->rect = {{53, 130 - 66}, {51, 24}};
    pTxtPaddingLeft->setIsNumerical(true);
    pTxtPaddingLeft->setIsDecimalPrecision(0);

    pTxtPaddingRight->textComponent.text = "0";
    pTxtPaddingRight->rect = {{159, 130 - 66}, {51, 24}};
    pTxtPaddingRight->setIsNumerical(true);
    pTxtPaddingRight->setIsDecimalPrecision(0);

    pTxtPaddingTop->textComponent.text = "0";
    pTxtPaddingTop->rect = {{106, 118 - 66}, {51, 24}};
    pTxtPaddingTop->setIsNumerical(true);
    pTxtPaddingTop->setIsDecimalPrecision(0);

    pTxtPaddingBottom->textComponent.text = "0";
    pTxtPaddingBottom->rect = {{106, 144 - 66}, {51, 24}};
    pTxtPaddingBottom->setIsNumerical(true);
    pTxtPaddingBottom->setIsDecimalPrecision(0);

    pPnl->add(pLabel);
    pPnl->add(pContainer);
    pContainer->add(pTxtImage);
    pContainer->add(pBrowseButton);
    pContainer->add(pBtnColor);
    pContainer->add(pChkScale9);
    pContainer->add(pChkRepeat);
    pContainer->add(pBtnFit);
    pContainer->add(pLblPadding);
    pContainer->add(pTxtPaddingLeft);
    pContainer->add(pTxtPaddingRight);
    pContainer->add(pTxtPaddingTop);
    pContainer->add(pTxtPaddingBottom);

    yPos += 174 + 4 - 66;

    auto mouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    auto mouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    pTxtImage->onMouseEnter = mouseEnter;
    pTxtImage->onMouseLeave = mouseLeave;
    pTxtPaddingLeft->onMouseEnter = mouseEnter;
    pTxtPaddingLeft->onMouseLeave = mouseLeave;
    pTxtPaddingRight->onMouseEnter = mouseEnter;
    pTxtPaddingRight->onMouseLeave = mouseLeave;
    pTxtPaddingTop->onMouseEnter = mouseEnter;
    pTxtPaddingTop->onMouseLeave = mouseLeave;
    pTxtPaddingBottom->onMouseEnter = mouseEnter;
    pTxtPaddingBottom->onMouseLeave = mouseLeave;

    auto actionName = std::string("Edit ") + name;
    { // image
        auto pBinding = new ControlInspectorBind<std::string, TuiType>(
            actionName, &(pTxtImage->textComponent.text),
            [](TuiType* pControl) {return pControl->scale9Component.image.filename; },
            [](TuiType* pControl, const std::string& filename){pControl->scale9Component.image.filename = filename; });
        pBindings->push_back(pBinding);
        pTxtImage->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
        // Browse
        pBrowseButton->onClick = [=](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent)
        {
            std::string file = fileOpen("Image Files (*.png)\0*.png\0All Files (*.*)\0*.*\0");
            if (!file.empty())
            {
                // Make it relative to our filename
                auto filename = onut::getPath(g_pDocument->getFilename());
                file = onut::makeRelativePath(file, filename);
                pTxtImage->textComponent.text = file;
                if (pTxtImage->onTextChanged)
                {
                    onut::UITextBoxEvent evt;
                    evt.pContext = g_pUIContext;
                    pTxtImage->onTextChanged(pTxtImage, evt);
                }
            }
        };
    }
    { // color
        auto pBinding = new ControlInspectorBind<onut::sUIColor, TuiType>(
            actionName, &(pBtnColor->color),
            [](TuiType* pControl) {return pControl->scale9Component.image.color; },
            [](TuiType* pControl, const onut::sUIColor& color){pControl->scale9Component.image.color = color; });
        pBindings->push_back(pBinding);
        pBtnColor->onClick = [=](onut::UIControl* pControl, const onut::UIMouseEvent& evt)
        {
            CHOOSECOLOR colorChooser = {0};
            DWORD rgbCurrent; // initial color selection
            rgbCurrent = (DWORD)pBtnColor->color.packed;
            rgbCurrent = ((rgbCurrent >> 24) & 0x000000ff) | ((rgbCurrent >> 8) & 0x0000ff00) | ((rgbCurrent << 8) & 0x00ff0000);
            colorChooser.lStructSize = sizeof(colorChooser);
            colorChooser.hwndOwner = OWindow->getHandle();
            colorChooser.lpCustColors = (LPDWORD)g_acrCustClr;
            colorChooser.rgbResult = rgbCurrent;
            colorChooser.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&colorChooser) == TRUE)
            {
                onut::sUIColor color;
                rgbCurrent = colorChooser.rgbResult;
                color.packed = ((rgbCurrent << 24) & 0xff000000) | ((rgbCurrent << 8) & 0x00ff0000) | ((rgbCurrent >> 8) & 0x0000ff00) | 0x000000ff;
                color.unpack();
                pBtnColor->color = color;
                pBinding->updateControl(g_pDocument->pSelected);
            }
        };
    }
    { // isScale9
        auto pBinding = new ControlInspectorBind<bool, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->scale9Component.isScaled9; },
            [](TuiType* pControl, const bool& isScaled9){pControl->scale9Component.isScaled9 = isScaled9; },
            [=]
            {
                return pChkScale9->getIsChecked();
            },
            [=](const bool& isScaled9)
            {
                pChkScale9->setIsChecked(isScaled9);
                pChkRepeat->isEnabled = isScaled9;
                pTxtPaddingLeft->isEnabled = isScaled9;
                pTxtPaddingRight->isEnabled = isScaled9;
                pTxtPaddingTop->isEnabled = isScaled9;
                pTxtPaddingBottom->isEnabled = isScaled9;
            });
        pBindings->push_back(pBinding);
        pChkScale9->onCheckChanged = [=](onut::UICheckBox* pTextBox, const onut::UICheckEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // repeat
        auto pBinding = new ControlInspectorBind<bool, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->scale9Component.isRepeat; },
            [](TuiType* pControl, const bool& isRepeat){pControl->scale9Component.isRepeat = isRepeat; },
            [=]
            {
                return pChkRepeat->getIsChecked();
            },
            [=](const bool& isScaled9)
            {
                pChkRepeat->setIsChecked(isScaled9);
            });
        pBindings->push_back(pBinding);
        pChkRepeat->onCheckChanged = [=](onut::UICheckBox* pTextBox, const onut::UICheckEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.left
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->scale9Component.padding.left; },
            [](TuiType* pControl, const float& padding){pControl->scale9Component.padding.left = padding; },
            [=]{return pTxtPaddingLeft->getFloat(); },
            [=](const float& padding) {pTxtPaddingLeft->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingLeft->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.right
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->scale9Component.padding.right; },
            [](TuiType* pControl, const float& padding){pControl->scale9Component.padding.right = padding; },
            [=]{return pTxtPaddingRight->getFloat(); },
            [=](const float& padding) {pTxtPaddingRight->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingRight->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.top
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->scale9Component.padding.top; },
            [](TuiType* pControl, const float& padding){pControl->scale9Component.padding.top = padding; },
            [=]{return pTxtPaddingTop->getFloat(); },
            [=](const float& padding) {pTxtPaddingTop->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingTop->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.bottom
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->scale9Component.padding.bottom; },
            [](TuiType* pControl, const float& padding){pControl->scale9Component.padding.bottom = padding; },
            [=]{return pTxtPaddingBottom->getFloat(); },
            [=](const float& padding) {pTxtPaddingBottom->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingBottom->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
}

template<typename TuiType>
void BIND_TEXT_COMPONENT(const std::string& name)
{
    auto pLabel = new onut::UILabel();
    auto pContainer = new onut::UIPanel();
    auto pTxtText = new onut::UITextBox();
    auto pBtnColor = new onut::UIPanel();
    auto pTxtTypeFace = new onut::UITextBox();
    auto pLblSize = new onut::UILabel();
    auto pTxtSize = new onut::UITextBox();
    auto pLblMinSize = new onut::UILabel();
    auto pTxtMinSize = new onut::UITextBox();
    auto pChkWordWrap = new onut::UICheckBox();
    auto pChkAutoFit = new onut::UICheckBox();
    auto pChkEllipsis = new onut::UICheckBox();
    auto pLblPadding = new onut::UILabel();
    auto pTxtPaddingLeft = new onut::UITextBox();
    auto pTxtPaddingRight = new onut::UITextBox();
    auto pTxtPaddingTop = new onut::UITextBox();
    auto pTxtPaddingBottom = new onut::UITextBox();
    auto pAlign = new onut::UIControl();
    auto pAlignTL = new onut::UICheckBox();
    auto pAlignT = new onut::UICheckBox();
    auto pAlignTR = new onut::UICheckBox();
    auto pAlignL = new onut::UICheckBox();
    auto pAlignC = new onut::UICheckBox();
    auto pAlignR = new onut::UICheckBox();
    auto pAlignBL = new onut::UICheckBox();
    auto pAlignB = new onut::UICheckBox();
    auto pAlignBR = new onut::UICheckBox();

    pLabel->textComponent.text = name;
    pLabel->rect = {{4, yPos}, {58, 174}};

    pContainer->setStyle("group");
    pContainer->widthType = (onut::eUIDimType::DIM_RELATIVE);
    pContainer->rect = {{66, yPos}, {-70, 174}};

    pTxtText->widthType = (onut::eUIDimType::DIM_RELATIVE);
    pTxtText->rect = {{4, 4}, {-8, 24}};

    pBtnColor->setStyle("colorPicker");
    pBtnColor->rect = {{4, 33}, {26, 24}};

    pTxtTypeFace->rect = {{34, 33}, {118, 24}};

    pLblSize->textComponent.text = "Size";
    pLblSize->rect = {{4, 61}, {26, 24}};

    pTxtSize->textComponent.text = "0";
    pTxtSize->rect = {{34, 61}, {44, 24}};
    pTxtSize->setIsNumerical(true);
    pTxtSize->setIsDecimalPrecision(1);

    pLblMinSize->textComponent.text = "Min";
    pLblMinSize->rect = {{82, 61}, {24, 24}};

    pTxtMinSize->textComponent.text = "0";
    pTxtMinSize->rect = {{108, 61}, {44, 24}};
    pTxtMinSize->setIsNumerical(true);
    pTxtMinSize->setIsDecimalPrecision(1);

    pChkWordWrap->textComponent.text = "Word wrap";
    pChkWordWrap->rect = {{4, 90}, {87 - 4, 24}};

    pChkAutoFit->textComponent.text = "Auto fit";
    pChkAutoFit->rect = {{4 + 87, 90}, {68 - 4, 24}};

    pChkEllipsis->textComponent.text = "Ellipsis";
    pChkEllipsis->rect = {{4 + 87 + 68, 90}, {55 - 4, 24}};

    pLblPadding->textComponent.text = "Padding";
    pLblPadding->rect = {{4, 118}, {53 - 8, 50}};

    pTxtPaddingLeft->textComponent.text = "0";
    pTxtPaddingLeft->rect = {{53, 130}, {51, 24}};
    pTxtPaddingLeft->setIsNumerical(true);
    pTxtPaddingLeft->setIsDecimalPrecision(0);

    pTxtPaddingRight->textComponent.text = "0";
    pTxtPaddingRight->rect = {{159, 130}, {51, 24}};
    pTxtPaddingRight->setIsNumerical(true);
    pTxtPaddingRight->setIsDecimalPrecision(0);

    pTxtPaddingTop->textComponent.text = "0";
    pTxtPaddingTop->rect = {{106, 118}, {51, 24}};
    pTxtPaddingTop->setIsNumerical(true);
    pTxtPaddingTop->setIsDecimalPrecision(0);

    pTxtPaddingBottom->textComponent.text = "0";
    pTxtPaddingBottom->rect = {{106, 144}, {51, 24}};
    pTxtPaddingBottom->setIsNumerical(true);
    pTxtPaddingBottom->setIsDecimalPrecision(0);

    pAlign->rect = {{156, 32}, {54, 54}};

    pAlignTL->setStyle("align");
    pAlignTL->rect = {{0, 0}, {18, 18}};
    pAlignTL->align = onut::eUIAlign::TOP_LEFT;
    pAlignTL->behavior = onut::eUICheckBehavior::EXCLUSIVE;

    pAlignT->setStyle("align");
    pAlignT->rect = {{0, 0}, {18, 18}};
    pAlignT->align = (onut::eUIAlign::TOP);
    pAlignT->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignT->anchor = {.5f, 0};

    pAlignTR->setStyle("align");
    pAlignTR->rect = {{0, 0}, {18, 18}};
    pAlignTR->align = (onut::eUIAlign::TOP_RIGHT);
    pAlignTR->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignTR->anchor = {1, 0};

    pAlignL->setStyle("align");
    pAlignL->rect = {{0, 0}, {18, 18}};
    pAlignL->align = (onut::eUIAlign::LEFT);
    pAlignL->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignL->anchor = {0, .5f};

    pAlignC->setStyle("align");
    pAlignC->rect = {{0, 0}, {18, 18}};
    pAlignC->align = (onut::eUIAlign::CENTER);
    pAlignC->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignC->anchor = {.5f, .5f};

    pAlignR->setStyle("align");
    pAlignR->rect = {{0, 0}, {18, 18}};
    pAlignR->align = (onut::eUIAlign::RIGHT);
    pAlignR->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignR->anchor = {1, .5f};

    pAlignBL->setStyle("align");
    pAlignBL->rect = {{0, 0}, {18, 18}};
    pAlignBL->align = (onut::eUIAlign::BOTTOM_LEFT);
    pAlignBL->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignBL->anchor = {0, 1};

    pAlignB->setStyle("align");
    pAlignB->rect = {{0, 0}, {18, 18}};
    pAlignB->align = (onut::eUIAlign::BOTTOM);
    pAlignB->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignB->anchor = {.5f, 1};

    pAlignBR->setStyle("align");
    pAlignBR->rect = {{0, 0}, {18, 18}};
    pAlignBR->align = (onut::eUIAlign::BOTTOM_RIGHT);
    pAlignBR->behavior = onut::eUICheckBehavior::EXCLUSIVE;
    pAlignBR->anchor = {1, 1};

    pPnl->add(pLabel);
    pPnl->add(pContainer);
    pContainer->add(pTxtText);
    pContainer->add(pBtnColor);
    pContainer->add(pTxtTypeFace);
    pContainer->add(pLblSize);
    pContainer->add(pTxtSize);
    pContainer->add(pLblMinSize);
    pContainer->add(pTxtMinSize);
    pContainer->add(pChkWordWrap);
    pContainer->add(pChkAutoFit);
    pContainer->add(pChkEllipsis);
    pContainer->add(pLblPadding);
    pContainer->add(pTxtPaddingLeft);
    pContainer->add(pTxtPaddingRight);
    pContainer->add(pTxtPaddingTop);
    pContainer->add(pTxtPaddingBottom);
    pContainer->add(pAlign);
    pAlign->add(pAlignTL);
    pAlign->add(pAlignT);
    pAlign->add(pAlignTR);
    pAlign->add(pAlignL);
    pAlign->add(pAlignC);
    pAlign->add(pAlignR);
    pAlign->add(pAlignBL);
    pAlign->add(pAlignB);
    pAlign->add(pAlignBR);

    yPos += 174 + 4;

    auto mouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    auto mouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    pTxtText->onMouseEnter = mouseEnter;
    pTxtText->onMouseLeave = mouseLeave;
    pTxtTypeFace->onMouseEnter = mouseEnter;
    pTxtTypeFace->onMouseLeave = mouseLeave;
    pTxtSize->onMouseEnter = mouseEnter;
    pTxtSize->onMouseLeave = mouseLeave;
    pTxtMinSize->onMouseEnter = mouseEnter;
    pTxtMinSize->onMouseLeave = mouseLeave;
    pTxtPaddingLeft->onMouseEnter = mouseEnter;
    pTxtPaddingLeft->onMouseLeave = mouseLeave;
    pTxtPaddingRight->onMouseEnter = mouseEnter;
    pTxtPaddingRight->onMouseLeave = mouseLeave;
    pTxtPaddingTop->onMouseEnter = mouseEnter;
    pTxtPaddingTop->onMouseLeave = mouseLeave;
    pTxtPaddingBottom->onMouseEnter = mouseEnter;
    pTxtPaddingBottom->onMouseLeave = mouseLeave;

    //--- Bindings
    auto actionName = std::string("Edit ") + name;
    { // text
        auto pBinding = new ControlInspectorBind<std::string, TuiType>(
            actionName, &(pTxtText->textComponent.text),
            [](TuiType* pControl) {return pControl->textComponent.text; },
            [](TuiType* pControl, const std::string& text){pControl->textComponent.text = text; });
        pBindings->push_back(pBinding);
        pTxtText->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // color
        auto pBinding = new ControlInspectorBind<onut::sUIColor, TuiType>(
            actionName, &(pBtnColor->color),
            [](TuiType* pControl) {return pControl->textComponent.font.color; },
            [](TuiType* pControl, const onut::sUIColor& color){pControl->textComponent.font.color = color; });
        pBindings->push_back(pBinding);
        pBtnColor->onClick = [=](onut::UIControl* pControl, const onut::UIMouseEvent& evt)
        {
            CHOOSECOLOR colorChooser = {0};
            DWORD rgbCurrent; // initial color selection
            rgbCurrent = (DWORD)pBtnColor->color.packed;
            rgbCurrent = ((rgbCurrent >> 24) & 0x000000ff) | ((rgbCurrent >> 8) & 0x0000ff00) | ((rgbCurrent << 8) & 0x00ff0000);
            colorChooser.lStructSize = sizeof(colorChooser);
            colorChooser.hwndOwner = OWindow->getHandle();
            colorChooser.lpCustColors = (LPDWORD)g_acrCustClr;
            colorChooser.rgbResult = rgbCurrent;
            colorChooser.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&colorChooser) == TRUE)
            {
                onut::sUIColor color;
                rgbCurrent = colorChooser.rgbResult;
                color.packed = ((rgbCurrent << 24) & 0xff000000) | ((rgbCurrent << 8) & 0x00ff0000) | ((rgbCurrent >> 8) & 0x0000ff00) | 0x000000ff;
                color.unpack();
                pBtnColor->color = color;
                pBinding->updateControl(g_pDocument->pSelected);
            }
        };
    }
    { // typeFace
        auto pBinding = new ControlInspectorBind<std::string, TuiType>(
            actionName, &(pTxtTypeFace->textComponent.text),
            [](TuiType* pControl) {return pControl->textComponent.font.typeFace; },
            [](TuiType* pControl, const std::string& typeFace){pControl->textComponent.font.typeFace = typeFace; });
        pBindings->push_back(pBinding);
        pTxtTypeFace->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // size
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.size; },
            [](TuiType* pControl, const float& size){pControl->textComponent.font.size = size; },
            [=]{return pTxtSize->getFloat(); },
            [=](const float& size) {pTxtSize->setFloat(size); });
        pBindings->push_back(pBinding);
        pTxtSize->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // minSize
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.minSize; },
            [](TuiType* pControl, const float& minSize){pControl->textComponent.font.minSize = minSize; },
            [=]{return pTxtMinSize->getFloat(); },
            [=](const float& minSize) {pTxtMinSize->setFloat(minSize); });
        pBindings->push_back(pBinding);
        pTxtMinSize->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.left
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.padding.left; },
            [](TuiType* pControl, const float& padding){pControl->textComponent.font.padding.left = padding; },
            [=]{return pTxtPaddingLeft->getFloat(); },
            [=](const float& padding) {pTxtPaddingLeft->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingLeft->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.right
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.padding.right; },
            [](TuiType* pControl, const float& padding){pControl->textComponent.font.padding.right = padding; },
            [=]{return pTxtPaddingRight->getFloat(); },
            [=](const float& padding) {pTxtPaddingRight->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingRight->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.top
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.padding.top; },
            [](TuiType* pControl, const float& padding){pControl->textComponent.font.padding.top = padding; },
            [=]{return pTxtPaddingTop->getFloat(); },
            [=](const float& padding) {pTxtPaddingTop->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingTop->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.bottom
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.padding.bottom; },
            [](TuiType* pControl, const float& padding){pControl->textComponent.font.padding.bottom = padding; },
            [=]{return pTxtPaddingBottom->getFloat(); },
            [=](const float& padding) {pTxtPaddingBottom->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingBottom->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // flags
        auto pBinding = new ControlInspectorBind<uint8_t, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.flags; },
            [](TuiType* pControl, const uint8_t& flags){pControl->textComponent.font.flags = flags; },
            [=]
            {
                uint8_t flags = 0;
                if (pChkWordWrap->getIsChecked()) flags |= onut::sUIFont::WORD_WRAP;
                if (pChkAutoFit->getIsChecked()) flags |= onut::sUIFont::AUTO_FIT_SIZE;
                if (pChkEllipsis->getIsChecked()) flags |= onut::sUIFont::ELLIPSIS;
                return flags;
            },
            [=](const uint8_t& flags)
            {
                pChkWordWrap->setIsChecked(flags & onut::sUIFont::WORD_WRAP ? true : false);
                pChkAutoFit->setIsChecked(flags & onut::sUIFont::AUTO_FIT_SIZE ? true : false);
                pChkEllipsis->setIsChecked(flags & onut::sUIFont::ELLIPSIS ? true : false);
                pTxtMinSize->isEnabled = flags & onut::sUIFont::AUTO_FIT_SIZE ? true : false;
            });
        pBindings->push_back(pBinding);
        auto onCheckChanged = [=](onut::UICheckBox* pTextBox, const onut::UICheckEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
        pChkWordWrap->onCheckChanged = onCheckChanged;
        pChkAutoFit->onCheckChanged = onCheckChanged;
        pChkEllipsis->onCheckChanged = onCheckChanged;
    }
    { // align
        auto pBinding = new ControlInspectorBind<onut::eUIAlign, TuiType>(
            actionName, nullptr,
            [](TuiType* pControl) {return pControl->textComponent.font.align; },
            [](TuiType* pControl, const onut::eUIAlign& align){pControl->textComponent.font.align = align; },
            [=]
            {
                if (pAlignTL->getIsChecked()) return onut::eUIAlign::TOP_LEFT;
                if (pAlignT->getIsChecked()) return onut::eUIAlign::TOP;
                if (pAlignTR->getIsChecked()) return onut::eUIAlign::TOP_RIGHT;
                if (pAlignL->getIsChecked()) return onut::eUIAlign::LEFT;
                if (pAlignC->getIsChecked()) return onut::eUIAlign::CENTER;
                if (pAlignR->getIsChecked()) return onut::eUIAlign::RIGHT;
                if (pAlignBL->getIsChecked()) return onut::eUIAlign::BOTTOM_LEFT;
                if (pAlignB->getIsChecked()) return onut::eUIAlign::BOTTOM;
                if (pAlignBR->getIsChecked()) return onut::eUIAlign::BOTTOM_RIGHT;
                return onut::eUIAlign::TOP_LEFT;
            },
            [=](const onut::eUIAlign& align)
            {   
                switch (align)
                {
                    case onut::eUIAlign::TOP_LEFT: pAlignTL->setIsChecked(true); break;
                    case onut::eUIAlign::TOP: pAlignT->setIsChecked(true); break;
                    case onut::eUIAlign::TOP_RIGHT: pAlignTR->setIsChecked(true); break;
                    case onut::eUIAlign::LEFT: pAlignL->setIsChecked(true); break;
                    case onut::eUIAlign::CENTER: pAlignC->setIsChecked(true); break;
                    case onut::eUIAlign::RIGHT: pAlignR->setIsChecked(true); break;
                    case onut::eUIAlign::BOTTOM_LEFT: pAlignBL->setIsChecked(true); break;
                    case onut::eUIAlign::BOTTOM: pAlignB->setIsChecked(true); break;
                    case onut::eUIAlign::BOTTOM_RIGHT: pAlignBR->setIsChecked(true); break;
                }
            });
        pBindings->push_back(pBinding);
        auto onCheckChanged = [=](onut::UICheckBox* pTextBox, const onut::UICheckEvent& evt)
        {
            if (!evt.isChecked) return;
            pBinding->updateControl(g_pDocument->pSelected);
        };
        pAlignTL->onCheckChanged = onCheckChanged;
        pAlignT->onCheckChanged = onCheckChanged;
        pAlignTR->onCheckChanged = onCheckChanged;
        pAlignL->onCheckChanged = onCheckChanged;
        pAlignC->onCheckChanged = onCheckChanged;
        pAlignR->onCheckChanged = onCheckChanged;
        pAlignBL->onCheckChanged = onCheckChanged;
        pAlignB->onCheckChanged = onCheckChanged;
        pAlignBR->onCheckChanged = onCheckChanged;
    }
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_TEXT_PROPERTY(const std::string& name, Tgetter getter, Tsetter setter)
{
    auto pLabel = new onut::UILabel();
    auto pTextBox = new onut::UITextBox();

    pLabel->textComponent.text = name;
    pLabel->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pTextBox->align = (onut::eUIAlign::TOP_RIGHT);
    pTextBox->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pTextBox->rect = {{-4, yPos}, {0.618f, 24}};
    pTextBox->anchor = {1, 0};

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pTextBox);

    auto pBinding = new ControlInspectorBind<std::string, TuiType>
        (std::string("Edit ") + name, &(pTextBox->textComponent.text), getter, setter);
    pBindings->push_back(pBinding);

    pTextBox->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_NUMERIC_PROPERTY(const std::string& name, Tgetter getter, Tsetter setter, int decimalCount = 0)
{
    auto pLabel = new onut::UILabel();
    auto pTextBox = new onut::UITextBox();

    pLabel->textComponent.text = name;
    pLabel->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pTextBox->align = (onut::eUIAlign::TOP_RIGHT);
    pTextBox->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pTextBox->rect = {{-4, yPos}, {0.618f, 24}};
    pTextBox->anchor = {1, 0};
    pTextBox->setIsNumerical(true);
    pTextBox->setIsDecimalPrecision(decimalCount);

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pTextBox);

    auto pBinding = new ControlInspectorBind<std::string, TuiType>
        (std::string("Edit ") + name, &(pTextBox->textComponent.text), getter, setter);
    pBindings->push_back(pBinding);

    pTextBox->onMouseEnter = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](onut::UIControl* pControl, const onut::UIMouseEvent& mouseEvent){OWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](onut::UITextBox* pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_BOOL_PROPERTY(const std::string& name, Tgetter getter, Tsetter setter)
{
    auto pCheckBox = new onut::UICheckBox();

    pCheckBox->textComponent.text = name;
    pCheckBox->widthType = (onut::eUIDimType::DIM_RELATIVE);
    pCheckBox->rect = {{4, yPos}, {-8, 24}};

    yPos += 24 + 4;

    pPnl->add(pCheckBox);

    auto pBinding = new ControlInspectorBind<bool, TuiType>(
        "Edit " + name, nullptr, getter, setter,
        [=]
    {
        return pCheckBox->getIsChecked();;
    },
        [=](const bool& isChecked)
    {
        pCheckBox->setIsChecked(isChecked);
    });
    pBindings->push_back(pBinding);

    pCheckBox->onCheckChanged = [=](onut::UICheckBox* pTextBox, const onut::UICheckEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_FILE_PROPERTY(const std::string& name, const char* szFilter, Tgetter getter, Tsetter setter)
{
    auto pLabel = new onut::UILabel();
    auto pContainer = new onut::UIControl();
    auto pTextBox = new onut::UITextBox();
    auto pBrowseButton = new onut::UIButton();

    pLabel->textComponent.text = name;
    pLabel->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pContainer->align = (onut::eUIAlign::TOP_RIGHT);
    pContainer->widthType = (onut::eUIDimType::DIM_PERCENTAGE);
    pContainer->rect = {{-4, yPos}, {0.618f, 24}};
    pContainer->anchor = {1, 0};

    pTextBox->widthType = (onut::eUIDimType::DIM_RELATIVE);
    pTextBox->rect = {{0, 0}, {-32, 24}};

    pBrowseButton->align = (onut::eUIAlign::TOP_RIGHT);
    pBrowseButton->anchor = {1, 0};
    pBrowseButton->rect = {{0, 0}, {32, 24}};
    pBrowseButton->textComponent.text = "...";

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pContainer);
    pContainer->add(pTextBox);
    pContainer->add(pBrowseButton);

    auto pBinding = new ControlInspectorBind<std::string, TuiType>
        (std::string("Edit ") + name, &(pTextBox->textComponent.text), getter, setter);
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
            pTextBox->textComponent.text = file;
            if (pTextBox->onTextChanged)
            {
                onut::UITextBoxEvent evt;
                evt.pContext = g_pUIContext;
                pTextBox->onTextChanged(pTextBox, evt);
            }
        }
    };
}

#include "menu.h"

void hookUIEvents(onut::UIControl* pUIScreen)
{
    curARROW = LoadCursor(nullptr, IDC_ARROW);
    curIBEAM = LoadCursor(nullptr, IDC_IBEAM);

    // Tool Bar
    pUIScreen->getChild<onut::UICheckBox>("chkEditorStyle")->onCheckChanged = [&](onut::UICheckBox* pControl, const onut::UICheckEvent& checkEvent){
        if (checkEvent.isChecked)
        {
            g_pDocument->pUIContext = g_pDocument->pUIContextEditorStyle;
        }
        else
        {
            g_pDocument->pUIContext = g_pDocument->pUIContextUserStyle;
        }
    };
    pUIScreen->getChild("toolNew")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_FILE_NEW); };
    pUIScreen->getChild("toolOpen")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_FILE_OPEN); };
    pUIScreen->getChild("toolSave")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_FILE_SAVE); };
    pUIScreen->getChild("toolCut")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_CUT); };
    pUIScreen->getChild("toolCopy")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_COPY); };
    pUIScreen->getChild("toolPaste")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_PASTE); };
    pUIScreen->getChild("toolDelete")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_DELETE); };
    pUIScreen->getChild("toolUndo")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_UNDO); };
    pUIScreen->getChild("toolRedo")->onClick = [](onut::UIControl* pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_REDO); };

    // Tool box
    pUIScreen->getChild("btnCreateControl")->onClick = onCreateControl;
    pUIScreen->getChild("btnCreatePanel")->onClick = onCreatePanel;
    pUIScreen->getChild("btnCreateButton")->onClick = onCreateButton;
    pUIScreen->getChild("btnCreateLabel")->onClick = onCreateLabel;
    pUIScreen->getChild("btnCreateImage")->onClick = onCreateImage;
    pUIScreen->getChild("btnCreateCheckBox")->onClick = onCreateCheckBox;
    pUIScreen->getChild("btnCreateTextBox")->onClick = onCreateTextBox;
    pUIScreen->getChild("btnCreateTreeView")->onClick = onCreateTreeView;

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

    g_pInspector_UIControl_chkClipChildren = pUIScreen->getChild<onut::UICheckBox>("chkClipChildren");
    g_pInspector_UIControl_chkClipChildren->onCheckChanged = onUIControlClipChildrenChanged;

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
    BIND_COLOR_PROPERTY<onut::UIPanel>("Background Color",
                                       [=](onut::UIPanel* pPanel) -> onut::sUIColor {return pPanel->color; },
                                       [=](onut::UIPanel* pPanel, const onut::sUIColor& color){ pPanel->color = color; });

    // UIButton
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_BUTTON, "pnlInspector_UIButton");
    BIND_TEXT_COMPONENT<onut::UIButton>("Caption");
    BIND_SCALE9_COMPONENT<onut::UIButton>("Bg Image");

    // UILabel
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_LABEL, "pnlInspector_UILabel");
    BIND_TEXT_COMPONENT<onut::UILabel>("Text");
    
    // UIImage
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_IMAGE, "pnlInspector_UIImage");
    BIND_SCALE9_COMPONENT<onut::UIImage>("Image");

    // UICheckBox
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_CHECKBOX, "pnlInspector_UICheckBox");
    BIND_TEXT_COMPONENT<onut::UICheckBox>("Text");
    BIND_BOOL_PROPERTY<onut::UICheckBox>("Checked",
                                         [](onut::UICheckBox* pControl) {return pControl->getIsChecked(); },
                                         [](onut::UICheckBox* pControl, const bool& isChecked){pControl->setIsChecked(isChecked); });

    // UITextBox
    BEGIN_BINDINGS(pUIScreen, onut::eUIType::UI_TEXTBOX, "pnlInspector_UITextBox");
    BIND_TEXT_COMPONENT<onut::UITextBox>("Text");
    BIND_SCALE9_COMPONENT<onut::UITextBox>("Bg Image");
    BIND_BOOL_PROPERTY<onut::UITextBox>("Numerical",
                                        [](onut::UITextBox* pControl) {return pControl->getIsNumerical(); },
                                        [](onut::UITextBox* pControl, const bool& isNumerical){pControl->setIsNumerical(isNumerical); });
    BIND_NUMERIC_PROPERTY<onut::UITextBox>("Decimal",
                                        [](onut::UITextBox* pControl) {return std::to_string(pControl->getDecimalPrecision()); },
                                        [](onut::UITextBox* pControl, const std::string& decimal){pControl->setIsDecimalPrecision(std::stoi(decimal)); });
}

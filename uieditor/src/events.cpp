#include "DocumentView.h"
#include "events.h"

#include "onut/ActionManager.h"
#include "onut/Files.h"
#include "onut/Input.h"
#include "onut/UIButton.h"
#include "onut/UICheckBox.h"
#include "onut/UILabel.h"
#include "onut/UIImage.h"
#include "onut/UIPanel.h"
#include "onut/UITextBox.h"
#include "onut/UITreeView.h"
#include "onut/Window.h"

extern DocumentView* g_pDocument;
extern OUIContextRef g_pUIContext;
extern OUIControlRef g_pUIScreen;

OUICheckBoxRef    g_pInspector_UIControl_chkEnabled;
OUICheckBoxRef    g_pInspector_UIControl_chkVisible;
OUICheckBoxRef    g_pInspector_UIControl_chkClickThrough;
OUICheckBoxRef    g_pInspector_UIControl_chkClipChildren;
OUITextBoxRef     g_pInspector_UIControl_txtName;
OUITextBoxRef     g_pInspector_UIControl_txtStyle;
OUITextBoxRef     g_pInspector_UIControl_txtX;
OUITextBoxRef     g_pInspector_UIControl_txtY;
OUICheckBoxRef    g_pInspector_UIControl_chkXPercent;
OUICheckBoxRef    g_pInspector_UIControl_chkYPercent;
OUITextBoxRef     g_pInspector_UIControl_txtWidth;
OUITextBoxRef     g_pInspector_UIControl_txtHeight;
OUICheckBoxRef    g_pInspector_UIControl_chkWidthPercent;
OUICheckBoxRef    g_pInspector_UIControl_chkHeightPercent;
OUICheckBoxRef    g_pInspector_UIControl_chkWidthRelative;
OUICheckBoxRef    g_pInspector_UIControl_chkHeightRelative;
OUICheckBoxRef    g_pInspector_UIControl_chkTOP_LEFT;
OUICheckBoxRef    g_pInspector_UIControl_chkTOP;
OUICheckBoxRef    g_pInspector_UIControl_chkTOP_RIGHT;
OUICheckBoxRef    g_pInspector_UIControl_chkLEFT;
OUICheckBoxRef    g_pInspector_UIControl_chkCENTER;
OUICheckBoxRef    g_pInspector_UIControl_chkRIGHT;
OUICheckBoxRef    g_pInspector_UIControl_chkBOTTOM_LEFT;
OUICheckBoxRef    g_pInspector_UIControl_chkBOTTOM;
OUICheckBoxRef    g_pInspector_UIControl_chkBOTTOM_RIGHT;
OUITextBoxRef     g_pInspector_UIControl_txtAnchorX;
OUITextBoxRef     g_pInspector_UIControl_txtAnchorY;
OUICheckBoxRef    g_pInspector_UIControl_chkXAnchorPercent;
OUICheckBoxRef    g_pInspector_UIControl_chkYAnchorPercent;
OUIButtonRef      g_pInspector_UIControl_chkAnchorTOP_LEFT;
OUIButtonRef      g_pInspector_UIControl_chkAnchorTOP;
OUIButtonRef      g_pInspector_UIControl_chkAnchorTOP_RIGHT;
OUIButtonRef      g_pInspector_UIControl_chkAnchorLEFT;
OUIButtonRef      g_pInspector_UIControl_chkAnchorCENTER;
OUIButtonRef      g_pInspector_UIControl_chkAnchorRIGHT;
OUIButtonRef      g_pInspector_UIControl_chkAnchorBOTTOM_LEFT;
OUIButtonRef      g_pInspector_UIControl_chkAnchorBOTTOM;
OUIButtonRef      g_pInspector_UIControl_chkAnchorBOTTOM_RIGHT;

OUIControlRef getCreateParent()
{
    auto pSelected = g_pDocument->pSelected;
    if (!pSelected)
    {
        return g_pDocument->pUIScreen;
    }
    else
    {
        if (OInputPressed(OKeyLeftShift))
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

std::string getControlName(const OUIControlRef& pControl)
{
    std::string ret = pControl->name;
    if (ret.empty())
    {
        switch (pControl->getType())
        {
            case OUIControl::Type::Button: return "UIButton";
            case OUIControl::Type::CheckBox: return "UICheckBox";
            case OUIControl::Type::Control: return "UIControl";
            case OUIControl::Type::Image: return "UIImage";
            case OUIControl::Type::Label: return "UILabel";
            case OUIControl::Type::Panel: return "UIPanel";
            case OUIControl::Type::TextBox: return "UITextBox";
            case OUIControl::Type::TreeView: return "UITreeView";
        }
    }
    return std::move(ret);
}

void createControlAction(const OUIControlRef& pControl, OUIControlRef pParent)
{
    auto pSelected = g_pDocument->pSelected;
    oActionManager->doAction(OMake<onut::Action>("Create " + getControlName(pControl),
        [=]{
        pParent->add(pControl);
        g_pDocument->controlCreated(pControl, pParent);
        g_pDocument->setSelected(pControl);
    },
        [=]{
        g_pDocument->setSelected(pSelected);
        pParent->remove(pControl);
        g_pDocument->controlDeleted(pControl);
    }));
}

void onCreateControl(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pCtrl = onut::UIControl::create();

    pCtrl->rect = {{0, 0}, {100, 100}};
    createControlAction(pCtrl, pParent);
}

void onCreatePanel(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pPanel = onut::UIPanel::create();

    pPanel->rect = {{0, 0}, {100, 100}};
    createControlAction(pPanel, pParent);
}

void onCreateButton(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = onut::UIButton::create();

    pBtn->textComponent.text = "Button";
    pBtn->rect = {{0, 0}, {64, 24}};
    createControlAction(pBtn, pParent);
}

void onCreateLabel(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pLbl = onut::UILabel::create();

    pLbl->textComponent.text = "Label";
    pLbl->rect = {{0, 0}, {64, 20}};
    createControlAction(pLbl, pParent);
}

void onCreateImage(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pImg = onut::UIImage::create();

    pImg->rect = {{0, 0}, {32, 32}};
    createControlAction(pImg, pParent);
}

void onCreateCheckBox(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = onut::UICheckBox::create();

    pBtn->textComponent.text = "CheckBox";
    pBtn->rect = {{0, 0}, {64, 24}};
    createControlAction(pBtn, pParent);
}

void onCreateTextBox(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pBtn = onut::UITextBox::create();

    pBtn->textComponent.text = "";
    pBtn->rect = {{0, 0}, {64, 24}};
    createControlAction(pBtn, pParent);
}

void onCreateTreeView(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto pParent = getCreateParent();
    auto pTreeView = onut::UITreeView::create();

    pTreeView->rect = {{0, 0}, {64, 24}};
    createControlAction(pTreeView, pParent);
}

bool g_bSelected = false;

void onSelect(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    auto mousePos = evt.mousePos;
    auto rect = g_pUIScreen->getChild("pnlRegion")->getWorldRect(g_pUIContext);
    mousePos.x -= rect.x;
    mousePos.y -= rect.y;
    auto pPickedControl = g_pDocument->pUIScreen->getChild(g_pDocument->pUIContext, mousePos, true, false);
    auto pPreviousSelected = g_pDocument->pSelected;

    std::string ctrlName = "Unselect";
    if (pPickedControl)
    {
        ctrlName = "Select " + getControlName(pControl);
    }

    oActionManager->doAction(OMake<onut::Action>(ctrlName,
        [=]{
        g_pDocument->setSelected(pPickedControl);
    },
        [=]{
        g_pDocument->setSelected(pPreviousSelected);
    }));

    if (pPickedControl)
    {
        g_bSelected = true;
        g_pDocument->onGizmoStart(pPickedControl, evt);
    }
}

void onSelectUp(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
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

void onSceneGraphSelectionChanged(const OUITreeViewRef& pControl, const onut::UITreeViewSelectEvent& evt)
{
    if (evt.selectedItems.empty())
    {
        auto pPreviousSelected = g_pDocument->pSelected;
        oActionManager->doAction(OMake<onut::Action>("Unselect",
            [=]{
            g_pDocument->setSelected(nullptr, true);
        },
            [=]{
            g_pDocument->setSelected(pPreviousSelected, true);
        }));
    }
    else
    {
        auto pViewItem = evt.selectedItems.front();
        auto pSelected = OStaticCast<OUIControl>(pViewItem->pSharedUserData);

        auto pPreviousSelected = g_pDocument->pSelected;
        oActionManager->doAction(OMake<onut::Action>("Select " + getControlName(pSelected),
            [=]{
            g_pDocument->setSelected(pSelected, true);
        },
            [=]{
            g_pDocument->setSelected(pPreviousSelected, true);
        }));
    }
}

void onUIControlNameChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;

    auto text = pTextBox->textComponent.text;
    auto pControl = g_pDocument->pSelected;
    auto previousText = pControl->name;

    oActionManager->doAction(OMake<onut::Action>("Rename " + getControlName(pControl) + " to " + text,
        [=]{
        pControl->name = text;
        auto pViewItem = OStaticCast<OUITreeViewItem>(pControl->pSharedUserData);
        if (pViewItem)
        {
            pViewItem->text = text;
        }
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->name = previousText;
        auto pViewItem = OStaticCast<OUITreeViewItem>(pControl->pSharedUserData);
        if (pViewItem)
        {
            pViewItem->text  =previousText;
        }
        g_pDocument->updateInspector();
    }));
}

void doRectChange(const std::string& actionName, const OUIControlRef& pControl, const Rect& rect)
{
    if (!pControl) return;
    auto previousRect = pControl->rect;
    oActionManager->doAction(OMake<onut::Action>(actionName, 
        [=]{
        pControl->rect = rect;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pControl->rect = previousRect;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    }));
}

void onUIControlXChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->rect;
    rect.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->xType == OUIControl::PosType::Percentage)
    {
        rect.x /= 100.f;
    }
    doRectChange("Edit X position", g_pDocument->pSelected, rect);
}
void onUIControlYChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->rect;
    rect.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->yType == OUIControl::PosType::Percentage)
    {
        rect.y /= 100.f;
    }
    doRectChange("Edit Y position", g_pDocument->pSelected, rect);
}

void onUIControlAnchorXChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = g_pDocument->pSelected->anchor;
    auto newAnchor = prevAnchor;
    newAnchor.x = pTextBox->getFloat();
    if (g_pDocument->pSelected->xAnchorType == OUIControl::AnchorType::Percentage)
    {
        newAnchor.x /= 100.f;
    }
    oActionManager->doAction(OMake<onut::Action>("Edit X anchor",
        [=]{
        pSelected->anchor = newAnchor;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    }));
}

void onUIControlAnchorYChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = g_pDocument->pSelected->anchor;
    auto newAnchor = prevAnchor;
    newAnchor.y = pTextBox->getFloat();
    if (g_pDocument->pSelected->yAnchorType == OUIControl::AnchorType::Percentage)
    {
        newAnchor.y /= 100.f;
    }
    oActionManager->doAction(OMake<onut::Action>("Edit Y anchor",
        [=]{
        pSelected->anchor = newAnchor;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    }));
}

void onUIControlWidthChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->rect;
    rect.z = pTextBox->getFloat();
    if (g_pDocument->pSelected->widthType == OUIControl::DimType::Percentage)
    {
        rect.z /= 100.f;
    }
    doRectChange("Edit Width", g_pDocument->pSelected, rect);
}

void onUIControlHeightChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto rect = g_pDocument->pSelected->rect;
    rect.w = pTextBox->getFloat();
    if (g_pDocument->pSelected->heightType == OUIControl::DimType::Percentage)
    {
        rect.w /= 100.f;
    }
    doRectChange("Edit Height", g_pDocument->pSelected, rect);
}

void onUIControlStyleChanged(const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevStyle = pSelected->getStyleName();
    auto newStyle = pTextBox->textComponent.text;
    oActionManager->doAction(OMake<onut::Action>("Edit Style",
        [=]{
        pSelected->setStyle(newStyle.c_str());
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->setStyle(prevStyle.c_str());
        g_pDocument->updateInspector();
    }));
}

void onUIControlEnableChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    oActionManager->doAction(OMake<onut::Action>("Toggle Enable",
        [=]{
        pSelected->isEnabled = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->isEnabled = !bValue;
        g_pDocument->updateInspector();
    }));
}

void onUIControlVisibleChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    oActionManager->doAction(OMake<onut::Action>("Toggle Visible", 
        [=]{
        pSelected->isVisible = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->isVisible = !bValue;
        g_pDocument->updateInspector();
    }));
}

void onUIControlClickThroughChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    oActionManager->doAction(OMake<onut::Action>("Toggle ClickThrough", 
        [=]{
        pSelected->isClickThrough = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->isClickThrough = !bValue;
        g_pDocument->updateInspector();
    }));
}

void onUIControlClipChildrenChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    bool bValue = pCheckBox->getIsChecked();
    oActionManager->doAction(OMake<onut::Action>("Toggle ClipChildren", 
        [=]{
        pSelected->clipChildren = bValue;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->clipChildren = !bValue;
        g_pDocument->updateInspector();
    }));
}

void onUIControlXAnchorPercentChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = pSelected->anchor;
    auto prevAnchorType = pSelected->xAnchorType;
    auto newAnchor = prevAnchor;
    auto newAnchorType = prevAnchorType;
    if (pCheckBox->getIsChecked() && prevAnchorType == OUIControl::AnchorType::Pixel)
    {
        newAnchor.x = pSelected->getAnchorInPercentage().x;
        newAnchorType = OUIControl::AnchorType::Percentage;
    }
    else if (!pCheckBox->getIsChecked() && prevAnchorType == OUIControl::AnchorType::Percentage)
    {
        newAnchor.x = pSelected->getAnchorInPixel().x;
        newAnchorType = OUIControl::AnchorType::Pixel;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle X anchor percent",
        [=]{
        pSelected->anchor = newAnchor;
        pSelected->xAnchorType = newAnchorType;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
        pSelected->xAnchorType = prevAnchorType;
        g_pDocument->updateInspector();
    }));
}

void onUIControlYAnchorPercentChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = pSelected->anchor;
    auto prevAnchorType = pSelected->yAnchorType;
    auto newAnchor = prevAnchor;
    auto newAnchorType = prevAnchorType;
    if (pCheckBox->getIsChecked() && prevAnchorType == OUIControl::AnchorType::Pixel)
    {
        newAnchor.y = pSelected->getAnchorInPercentage().y;
        newAnchorType = OUIControl::AnchorType::Percentage;
    }
    else if (!pCheckBox->getIsChecked() && prevAnchorType == OUIControl::AnchorType::Percentage)
    {
        newAnchor.y = pSelected->getAnchorInPixel().y;
        newAnchorType = OUIControl::AnchorType::Pixel;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle Y anchor percent",
        [=]{
        pSelected->anchor = newAnchor;
        pSelected->yAnchorType = newAnchorType;
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = prevAnchor;
        pSelected->yAnchorType = prevAnchorType;
        g_pDocument->updateInspector();
    }));
}

void onUIControlWidthPercentChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->widthType;
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && prevDimType != OUIControl::DimType::Percentage)
    {
        auto worldRect = pSelected->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Percentage;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.z = worldRect.z / parentRect.z;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkWidthRelative->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Absolute;
        newRect.z = newRect.z * parentRect.z;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle Width percent",
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
    }));
}

void onUIControlHeightPercentChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->heightType;
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && prevDimType != OUIControl::DimType::Percentage)
    {
        auto worldRect = pSelected->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Percentage;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.w = worldRect.w / parentRect.w;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkHeightRelative->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Absolute;
        newRect.w = newRect.w * parentRect.w;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle Height percent",
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
    }));
}

void onUIControlWidthRelativeChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->widthType;
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && pSelected->widthType != OUIControl::DimType::Relative)
    {
        auto worldRect = pSelected->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Relative;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.z = worldRect.z - parentRect.z;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkWidthPercent->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Absolute;
        newRect.z = parentRect.z + newRect.z;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle Width relative",
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
    }));
}

void onUIControlHeightRelativeChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevRect = pSelected->rect;
    auto prevDimType = pSelected->heightType;
    auto newRect = prevRect;
    auto newDimType = prevDimType;
    if (pCheckBox->getIsChecked() && pSelected->heightType != OUIControl::DimType::Relative)
    {
        auto worldRect = pSelected->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Relative;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.w = worldRect.w - parentRect.w;
    }
    else if (!pCheckBox->getIsChecked() && !g_pInspector_UIControl_chkHeightPercent->getIsChecked())
    {
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newDimType = OUIControl::DimType::Absolute;
        newRect.w = parentRect.w + newRect.w;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle Height relative",
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
    }));
}

void onUIControlXPercentChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevType = pSelected->xType;
    auto prevRect = pSelected->rect;
    auto newType = prevType;
    auto newRect = prevRect;
    if (pCheckBox->getIsChecked() && prevType != OUIControl::PosType::Percentage)
    {
        newType = OUIControl::PosType::Percentage;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.x = newRect.x / parentRect.z;
    }
    else if (!pCheckBox->getIsChecked() && prevType == OUIControl::PosType::Percentage)
    {
        newType = OUIControl::PosType::Relative;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.x = newRect.x * parentRect.z;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle X percent",
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
    }));
}

void onUIControlYPercentChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!g_pDocument->pSelected) return;
    auto pSelected = g_pDocument->pSelected;
    auto prevType = pSelected->yType;
    auto prevRect = pSelected->rect;
    auto newType = prevType;
    auto newRect = prevRect;
    if (pCheckBox->getIsChecked() && prevType != OUIControl::PosType::Percentage)
    {
        newType = OUIControl::PosType::Percentage;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.y = newRect.y / parentRect.w;
    }
    else if (!pCheckBox->getIsChecked() && prevType == OUIControl::PosType::Percentage)
    {
        newType = OUIControl::PosType::Relative;
        auto& parentRect = pSelected->getParent()->getWorldRect(g_pDocument->pUIContext);
        newRect.y = newRect.y * parentRect.w;
    }
    oActionManager->doAction(OMake<onut::Action>("Toggle Y percent",
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
    }));
}

void onAnchorClicked(const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
{
    if (!g_pDocument->pSelected) return; // Wuuuut?
    auto pSelected = g_pDocument->pSelected;
    auto prevAnchor = pSelected->anchor;
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
    auto newAnchor = pSelected->anchor;

    oActionManager->doAction(OMake<onut::Action>("Change Anchor",
        [=]{
        pSelected->anchor = (newAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    },
        [=]{
        pSelected->anchor = (prevAnchor);
        g_pDocument->updateSelectedGizmoRect();
        g_pDocument->updateInspector();
    }));
}

void onAlignChkChanged(const OUICheckBoxRef& pCheckBox, const onut::UICheckEvent& evt)
{
    if (!evt.isChecked) return; // Don't care
    if (!g_pDocument->pSelected) return; // Wuuuut?

    auto pSelected = g_pDocument->pSelected;
    auto previousRect = pSelected->rect;
    auto newRect = previousRect;
    newRect.x = 0;
    newRect.y = 0;
    auto previousAnchor = pSelected->anchor;
    auto previousAlign = pSelected->align;
    if (pCheckBox == g_pInspector_UIControl_chkTOP_LEFT)
    {
        pSelected->setAnchorPercent({0, 0});
        pSelected->align = (OTopLeft);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkTOP)
    {
        pSelected->setAnchorPercent({.5f, 0});
        pSelected->align = (OTop);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkTOP_RIGHT)
    {
        pSelected->setAnchorPercent({1, 0});
        pSelected->align = (OTopRight);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkLEFT)
    {
        pSelected->setAnchorPercent({0, .5f});
        pSelected->align = (OLeft);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkCENTER)
    {
        pSelected->setAnchorPercent({.5f, .5f});
        pSelected->align = (OCenter);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkRIGHT)
    {
        pSelected->setAnchorPercent({1, .5f});
        pSelected->align = (ORight);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM_LEFT)
    {
        pSelected->setAnchorPercent({0, 1});
        pSelected->align = (OBottomLeft);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM)
    {
        pSelected->setAnchorPercent({.5f, 1});
        pSelected->align = (OBottom);
    }
    else if (pCheckBox == g_pInspector_UIControl_chkBOTTOM_RIGHT)
    {
        pSelected->setAnchorPercent({1, 1});
        pSelected->align = (OBottomRight);
    }
    auto newAnchor = pSelected->anchor;
    auto newAlign = pSelected->align;

    oActionManager->doAction(OMake<onut::Action>("Change Align",
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
    }));
}

std::string fileOpen(const char* szFilters)
{
    auto window = oWindow->getHandle();
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

extern std::unordered_map<OUIControl::Type, OUIControlRef> g_controlInspectorMap;
extern std::unordered_map<OUIControl::Type, std::vector<IControlInspectorBind*>> g_inspectorBindings;
float yPos;
std::vector<IControlInspectorBind*>* pBindings;
OUIPanelRef pPnl;
static HCURSOR curARROW;
static HCURSOR curIBEAM;

void BEGIN_BINDINGS(OUIControlRef pUIScreen, OUIControl::Type type, const std::string& name)
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
    auto pLabel = onut::UILabel::create();
    auto pButton = onut::UIPanel::create();
    auto pAlphaLabel = onut::UILabel::create();
    auto pAlphaText = onut::UITextBox::create();

    pLabel->textComponent.text = name;
    pLabel->widthType = (OUIControl::DimType::Percentage);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pButton->setStyle("colorPicker");
    pButton->align = (OTopRight);
    pButton->widthType = (OUIControl::DimType::Percentage);
    pButton->rect = {{-4, yPos}, {0.618f, 24}};
    pButton->anchor = {1, 0};

    yPos += 24 + 4;

    pAlphaLabel->textComponent.text = "Alpha:";
    pAlphaLabel->widthType = (OUIControl::DimType::Percentage);
    pAlphaLabel->align = (OTopRight);
    pAlphaLabel->rect = {{-4 - 64 - 4, yPos}, {64, 24}};
    pAlphaLabel->anchor = {1, 0};

    pAlphaText->setInt(100);
    pAlphaText->setIsNumerical(true);
    pAlphaText->align = (OTopRight);
    pAlphaText->rect = {{-4, yPos}, {64, 24}};
    pAlphaText->anchor = {1, 0};

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pButton);
    pPnl->add(pAlphaLabel);
    pPnl->add(pAlphaText);

    {
        auto pBinding = new ControlInspectorBind<Color, TuiType>
            (std::string("Edit ") + name, &pButton->color, getter, setter);
        pBindings->push_back(pBinding);

        pButton->onClick = [=](const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
        {
            CHOOSECOLOR colorChooser = {0};
            DWORD rgbCurrent; // initial color selection
            rgbCurrent = (DWORD)pButton->color.pack();
            auto alphaVal = rgbCurrent & 0x000000ff;
            rgbCurrent = ((rgbCurrent >> 24) & 0x000000ff) | ((rgbCurrent >> 8) & 0x0000ff00) | ((rgbCurrent << 8) & 0x00ff0000);
            colorChooser.lStructSize = sizeof(colorChooser);
            colorChooser.hwndOwner = oWindow->getHandle();
            colorChooser.lpCustColors = (LPDWORD)g_acrCustClr;
            colorChooser.rgbResult = rgbCurrent;
            colorChooser.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&colorChooser) == TRUE)
            {
                Color color;
                rgbCurrent = colorChooser.rgbResult;
                color.unpack(((rgbCurrent << 24) & 0xff000000) | ((rgbCurrent << 8) & 0x00ff0000) | ((rgbCurrent >> 8) & 0x0000ff00) | alphaVal);
                pButton->color = color;
                pBinding->updateControl(g_pDocument->pSelected);
            }
        };
    }

    { // alpha
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            "Alpha", nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->color.w * 100.f; },
            [](const std::shared_ptr<TuiType>& pControl, const float& alpha){pControl->color.w = alpha / 100.f; pControl->color.pack(); },
            [=]{return pAlphaText->getFloat(); },
            [=](const float& alpha) {pAlphaText->setFloat(alpha); });
        pBindings->push_back(pBinding);

        pAlphaText->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
}

template<typename TuiType>
void BIND_SCALE9_COMPONENT(const std::string& name)
{
    auto pLabel = onut::UILabel::create();
    auto pContainer = onut::UIPanel::create();
    auto pTxtImage = onut::UITextBox::create();
    auto pBrowseButton = onut::UIButton::create();
    auto pBtnColor = onut::UIPanel::create();
    auto pChkScale9 = onut::UICheckBox::create();
    auto pChkRepeat = onut::UICheckBox::create();
    auto pBtnFit = onut::UIButton::create();
    auto pLblPadding = onut::UILabel::create();
    auto pTxtPaddingLeft = onut::UITextBox::create();
    auto pTxtPaddingRight = onut::UITextBox::create();
    auto pTxtPaddingTop = onut::UITextBox::create();
    auto pTxtPaddingBottom = onut::UITextBox::create();

    pLabel->textComponent.text = name;
    pLabel->rect = {{4, yPos}, {58, 174 - 66}};

    pContainer->setStyle("group");
    pContainer->widthType = (OUIControl::DimType::Relative);
    pContainer->rect = {{66, yPos}, {-70, 174 - 66}};

    pTxtImage->widthType = (OUIControl::DimType::Relative);
    pTxtImage->rect = {{4, 4}, {-8 - 32, 24}};

    pBrowseButton->align = (OTopRight);
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
    pBtnFit->align = (OTopRight);
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

    auto mouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    auto mouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
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
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.image.filename; },
            [](const std::shared_ptr<TuiType>& pControl, const std::string& filename){pControl->scale9Component.image.filename = filename; });
        pBindings->push_back(pBinding);
        pTxtImage->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
        // Browse
        pBrowseButton->onClick = [=](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent)
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
        auto pBinding = new ControlInspectorBind<Color, TuiType>(
            actionName, &(pBtnColor->color),
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.image.color; },
            [](const std::shared_ptr<TuiType>& pControl, const Color& color){pControl->scale9Component.image.color = color; });
        pBindings->push_back(pBinding);
        pBtnColor->onClick = [=](const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
        {
            CHOOSECOLOR colorChooser = {0};
            DWORD rgbCurrent; // initial color selection
            rgbCurrent = (DWORD)pBtnColor->color.pack();
            rgbCurrent = ((rgbCurrent >> 24) & 0x000000ff) | ((rgbCurrent >> 8) & 0x0000ff00) | ((rgbCurrent << 8) & 0x00ff0000);
            colorChooser.lStructSize = sizeof(colorChooser);
            colorChooser.hwndOwner = oWindow->getHandle();
            colorChooser.lpCustColors = (LPDWORD)g_acrCustClr;
            colorChooser.rgbResult = rgbCurrent;
            colorChooser.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&colorChooser) == TRUE)
            {
                Color color;
                rgbCurrent = colorChooser.rgbResult;
                color.unpack(((rgbCurrent << 24) & 0xff000000) | ((rgbCurrent << 8) & 0x00ff0000) | ((rgbCurrent >> 8) & 0x0000ff00) | 0x000000ff);
                pBtnColor->color = color;
                pBinding->updateControl(g_pDocument->pSelected);
            }
        };
    }
    { // isScale9
        auto pBinding = new ControlInspectorBind<bool, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.isScaled9; },
            [](const std::shared_ptr<TuiType>& pControl, const bool& isScaled9){pControl->scale9Component.isScaled9 = isScaled9; },
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
        pChkScale9->onCheckChanged = [=](const OUICheckBoxRef& pTextBox, const onut::UICheckEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // repeat
        auto pBinding = new ControlInspectorBind<bool, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.isRepeat; },
            [](const std::shared_ptr<TuiType>& pControl, const bool& isRepeat){pControl->scale9Component.isRepeat = isRepeat; },
            [=]
            {
                return pChkRepeat->getIsChecked();
            },
            [=](const bool& isScaled9)
            {
                pChkRepeat->setIsChecked(isScaled9);
            });
        pBindings->push_back(pBinding);
        pChkRepeat->onCheckChanged = [=](const OUICheckBoxRef& pTextBox, const onut::UICheckEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.left
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.padding.x; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->scale9Component.padding.x = padding; },
            [=]{return pTxtPaddingLeft->getFloat(); },
            [=](const float& padding) {pTxtPaddingLeft->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingLeft->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.right
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.padding.z; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->scale9Component.padding.z = padding; },
            [=]{return pTxtPaddingRight->getFloat(); },
            [=](const float& padding) {pTxtPaddingRight->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingRight->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.top
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.padding.y; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->scale9Component.padding.y = padding; },
            [=]{return pTxtPaddingTop->getFloat(); },
            [=](const float& padding) {pTxtPaddingTop->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingTop->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.bottom
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->scale9Component.padding.w; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->scale9Component.padding.w = padding; },
            [=]{return pTxtPaddingBottom->getFloat(); },
            [=](const float& padding) {pTxtPaddingBottom->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingBottom->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
}

template<typename TuiType>
void BIND_TEXT_COMPONENT(const std::string& name)
{
    auto pLabel = onut::UILabel::create();
    auto pContainer = onut::UIPanel::create();
    auto pTxtText = onut::UITextBox::create();
    auto pBtnColor = onut::UIPanel::create();
    auto pTxtTypeFace = onut::UITextBox::create();
    auto pLblSize = onut::UILabel::create();
    auto pTxtSize = onut::UITextBox::create();
    auto pLblMinSize = onut::UILabel::create();
    auto pTxtMinSize = onut::UITextBox::create();
    auto pChkWordWrap = onut::UICheckBox::create();
    auto pChkAutoFit = onut::UICheckBox::create();
    auto pChkEllipsis = onut::UICheckBox::create();
    auto pLblPadding = onut::UILabel::create();
    auto pTxtPaddingLeft = onut::UITextBox::create();
    auto pTxtPaddingRight = onut::UITextBox::create();
    auto pTxtPaddingTop = onut::UITextBox::create();
    auto pTxtPaddingBottom = onut::UITextBox::create();
    auto pAlign = onut::UIControl::create();
    auto pAlignTL = onut::UICheckBox::create();
    auto pAlignT = onut::UICheckBox::create();
    auto pAlignTR = onut::UICheckBox::create();
    auto pAlignL = onut::UICheckBox::create();
    auto pAlignC = onut::UICheckBox::create();
    auto pAlignR = onut::UICheckBox::create();
    auto pAlignBL = onut::UICheckBox::create();
    auto pAlignB = onut::UICheckBox::create();
    auto pAlignBR = onut::UICheckBox::create();

    pLabel->textComponent.text = name;
    pLabel->rect = {{4, yPos}, {58, 174}};

    pContainer->setStyle("group");
    pContainer->widthType = (OUIControl::DimType::Relative);
    pContainer->rect = {{66, yPos}, {-70, 174}};

    pTxtText->widthType = (OUIControl::DimType::Relative);
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
    pAlignTL->align = OTopLeft;
    pAlignTL->behavior = OUICheckBox::CheckBehavior::Exclusive;

    pAlignT->setStyle("align");
    pAlignT->rect = {{0, 0}, {18, 18}};
    pAlignT->align = (OTop);
    pAlignT->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignT->anchor = {.5f, 0};

    pAlignTR->setStyle("align");
    pAlignTR->rect = {{0, 0}, {18, 18}};
    pAlignTR->align = (OTopRight);
    pAlignTR->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignTR->anchor = {1, 0};

    pAlignL->setStyle("align");
    pAlignL->rect = {{0, 0}, {18, 18}};
    pAlignL->align = (OLeft);
    pAlignL->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignL->anchor = {0, .5f};

    pAlignC->setStyle("align");
    pAlignC->rect = {{0, 0}, {18, 18}};
    pAlignC->align = (OCenter);
    pAlignC->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignC->anchor = {.5f, .5f};

    pAlignR->setStyle("align");
    pAlignR->rect = {{0, 0}, {18, 18}};
    pAlignR->align = (ORight);
    pAlignR->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignR->anchor = {1, .5f};

    pAlignBL->setStyle("align");
    pAlignBL->rect = {{0, 0}, {18, 18}};
    pAlignBL->align = (OBottomLeft);
    pAlignBL->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignBL->anchor = {0, 1};

    pAlignB->setStyle("align");
    pAlignB->rect = {{0, 0}, {18, 18}};
    pAlignB->align = (OBottom);
    pAlignB->behavior = OUICheckBox::CheckBehavior::Exclusive;
    pAlignB->anchor = {.5f, 1};

    pAlignBR->setStyle("align");
    pAlignBR->rect = {{0, 0}, {18, 18}};
    pAlignBR->align = (OBottomRight);
    pAlignBR->behavior = OUICheckBox::CheckBehavior::Exclusive;
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

    auto mouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    auto mouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
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
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.text; },
            [](const std::shared_ptr<TuiType>& pControl, const std::string& text){pControl->textComponent.text = text; });
        pBindings->push_back(pBinding);
        pTxtText->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // color
        auto pBinding = new ControlInspectorBind<Color, TuiType>(
            actionName, &(pBtnColor->color),
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.color; },
            [](const std::shared_ptr<TuiType>& pControl, const Color& color){pControl->textComponent.font.color = color; });
        pBindings->push_back(pBinding);
        pBtnColor->onClick = [=](const OUIControlRef& pControl, const onut::UIMouseEvent& evt)
        {
            CHOOSECOLOR colorChooser = {0};
            DWORD rgbCurrent; // initial color selection
            rgbCurrent = (DWORD)pBtnColor->color.pack();
            rgbCurrent = ((rgbCurrent >> 24) & 0x000000ff) | ((rgbCurrent >> 8) & 0x0000ff00) | ((rgbCurrent << 8) & 0x00ff0000);
            colorChooser.lStructSize = sizeof(colorChooser);
            colorChooser.hwndOwner = oWindow->getHandle();
            colorChooser.lpCustColors = (LPDWORD)g_acrCustClr;
            colorChooser.rgbResult = rgbCurrent;
            colorChooser.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&colorChooser) == TRUE)
            {
                Color color;
                rgbCurrent = colorChooser.rgbResult;
                color.unpack(((rgbCurrent << 24) & 0xff000000) | ((rgbCurrent << 8) & 0x00ff0000) | ((rgbCurrent >> 8) & 0x0000ff00) | 0x000000ff);
                pBtnColor->color = color;
                pBinding->updateControl(g_pDocument->pSelected);
            }
        };
    }
    { // typeFace
        auto pBinding = new ControlInspectorBind<std::string, TuiType>(
            actionName, &(pTxtTypeFace->textComponent.text),
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.typeFace; },
            [](const std::shared_ptr<TuiType>& pControl, const std::string& typeFace){pControl->textComponent.font.typeFace = typeFace; });
        pBindings->push_back(pBinding);
        pTxtTypeFace->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // size
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.size; },
            [](const std::shared_ptr<TuiType>& pControl, const float& size){pControl->textComponent.font.size = size; },
            [=]{return pTxtSize->getFloat(); },
            [=](const float& size) {pTxtSize->setFloat(size); });
        pBindings->push_back(pBinding);
        pTxtSize->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // minSize
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.minSize; },
            [](const std::shared_ptr<TuiType>& pControl, const float& minSize){pControl->textComponent.font.minSize = minSize; },
            [=]{return pTxtMinSize->getFloat(); },
            [=](const float& minSize) {pTxtMinSize->setFloat(minSize); });
        pBindings->push_back(pBinding);
        pTxtMinSize->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.left
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.padding.x; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->textComponent.font.padding.x = padding; },
            [=]{return pTxtPaddingLeft->getFloat(); },
            [=](const float& padding) {pTxtPaddingLeft->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingLeft->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.right
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.padding.z; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->textComponent.font.padding.z = padding; },
            [=]{return pTxtPaddingRight->getFloat(); },
            [=](const float& padding) {pTxtPaddingRight->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingRight->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.top
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.padding.y; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->textComponent.font.padding.y = padding; },
            [=]{return pTxtPaddingTop->getFloat(); },
            [=](const float& padding) {pTxtPaddingTop->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingTop->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // padding.bottom
        auto pBinding = new ControlInspectorBind<float, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.padding.w; },
            [](const std::shared_ptr<TuiType>& pControl, const float& padding){pControl->textComponent.font.padding.w = padding; },
            [=]{return pTxtPaddingBottom->getFloat(); },
            [=](const float& padding) {pTxtPaddingBottom->setFloat(padding); });
        pBindings->push_back(pBinding);
        pTxtPaddingBottom->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
    }
    { // flags
        auto pBinding = new ControlInspectorBind<uint8_t, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.flags; },
            [](const std::shared_ptr<TuiType>& pControl, const uint8_t& flags){pControl->textComponent.font.flags = flags; },
            [=]
            {
                uint8_t flags = 0;
                if (pChkWordWrap->getIsChecked()) flags |= onut::UIFontComponent::WordWrap;
                if (pChkAutoFit->getIsChecked()) flags |= onut::UIFontComponent::AutoFitSize;
                if (pChkEllipsis->getIsChecked()) flags |= onut::UIFontComponent::Ellipsis;
                return flags;
            },
            [=](const uint8_t& flags)
            {
                pChkWordWrap->setIsChecked(flags & onut::UIFontComponent::WordWrap ? true : false);
                pChkAutoFit->setIsChecked(flags & onut::UIFontComponent::AutoFitSize ? true : false);
                pChkEllipsis->setIsChecked(flags & onut::UIFontComponent::Ellipsis ? true : false);
                pTxtMinSize->isEnabled = flags & onut::UIFontComponent::AutoFitSize ? true : false;
            });
        pBindings->push_back(pBinding);
        auto onCheckChanged = [=](const OUICheckBoxRef& pTextBox, const onut::UICheckEvent& evt)
        {
            pBinding->updateControl(g_pDocument->pSelected);
        };
        pChkWordWrap->onCheckChanged = onCheckChanged;
        pChkAutoFit->onCheckChanged = onCheckChanged;
        pChkEllipsis->onCheckChanged = onCheckChanged;
    }
    { // align
        auto pBinding = new ControlInspectorBind<onut::Align, TuiType>(
            actionName, nullptr,
            [](const std::shared_ptr<TuiType>& pControl) {return pControl->textComponent.font.align; },
            [](const std::shared_ptr<TuiType>& pControl, const onut::Align& align){pControl->textComponent.font.align = align; },
            [=]
            {
                if (pAlignTL->getIsChecked()) return OTopLeft;
                if (pAlignT->getIsChecked()) return OTop;
                if (pAlignTR->getIsChecked()) return OTopRight;
                if (pAlignL->getIsChecked()) return OLeft;
                if (pAlignC->getIsChecked()) return OCenter;
                if (pAlignR->getIsChecked()) return ORight;
                if (pAlignBL->getIsChecked()) return OBottomLeft;
                if (pAlignB->getIsChecked()) return OBottom;
                if (pAlignBR->getIsChecked()) return OBottomRight;
                return OTopLeft;
            },
            [=](const onut::Align& align)
            {   
                switch (align)
                {
                    case OTopLeft: pAlignTL->setIsChecked(true); break;
                    case OTop: pAlignT->setIsChecked(true); break;
                    case OTopRight: pAlignTR->setIsChecked(true); break;
                    case OLeft: pAlignL->setIsChecked(true); break;
                    case OCenter: pAlignC->setIsChecked(true); break;
                    case ORight: pAlignR->setIsChecked(true); break;
                    case OBottomLeft: pAlignBL->setIsChecked(true); break;
                    case OBottom: pAlignB->setIsChecked(true); break;
                    case OBottomRight: pAlignBR->setIsChecked(true); break;
                }
            });
        pBindings->push_back(pBinding);
        auto onCheckChanged = [=](const OUICheckBoxRef& pTextBox, const onut::UICheckEvent& evt)
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
    auto pLabel = onut::UILabel::create();
    auto pTextBox = onut::UITextBox::create();

    pLabel->textComponent.text = name;
    pLabel->widthType = (OUIControl::DimType::DIM_PERCENTAGE);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pTextBox->align = (OTopRight);
    pTextBox->widthType = (OUIControl::DimType::DIM_PERCENTAGE);
    pTextBox->rect = {{-4, yPos}, {0.618f, 24}};
    pTextBox->anchor = {1, 0};

    yPos += 24 + 4;

    pPnl->add(pLabel);
    pPnl->add(pTextBox);

    auto pBinding = new ControlInspectorBind<std::string, TuiType>
        (std::string("Edit ") + name, &(pTextBox->textComponent.text), getter, setter);
    pBindings->push_back(pBinding);

    pTextBox->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_NUMERIC_PROPERTY(const std::string& name, Tgetter getter, Tsetter setter, int decimalCount = 0)
{
    auto pLabel = onut::UILabel::create();
    auto pTextBox = onut::UITextBox::create();

    pLabel->textComponent.text = name;
    pLabel->widthType = (OUIControl::DimType::Percentage);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pTextBox->align = (OTopRight);
    pTextBox->widthType = (OUIControl::DimType::Percentage);
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

    pTextBox->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
}

template<typename TuiType, typename Tgetter, typename Tsetter>
void BIND_BOOL_PROPERTY(const std::string& name, Tgetter getter, Tsetter setter)
{
    auto pCheckBox = onut::UICheckBox::create();

    pCheckBox->textComponent.text = name;
    pCheckBox->widthType = (OUIControl::DimType::Relative);
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

    pCheckBox->onCheckChanged = [=](const OUICheckBoxRef& pTextBox, const onut::UICheckEvent& evt)
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
    pLabel->widthType = (OUIControl::DimType::DIM_PERCENTAGE);
    pLabel->rect = {{4, yPos}, {0.382f, 24}};

    pContainer->align = (OTopRight);
    pContainer->widthType = (OUIControl::DimType::DIM_PERCENTAGE);
    pContainer->rect = {{-4, yPos}, {0.618f, 24}};
    pContainer->anchor = {1, 0};

    pTextBox->widthType = (OUIControl::DimType::DIM_RELATIVE);
    pTextBox->rect = {{0, 0}, {-32, 24}};

    pBrowseButton->align = (OTopRight);
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

    pTextBox->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    pTextBox->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    pTextBox->onTextChanged = [=](const OUITextBoxRef& pTextBox, const onut::UITextBoxEvent& evt)
    {
        pBinding->updateControl(g_pDocument->pSelected);
    };
    pBrowseButton->onClick = [=](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent)
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

void hookUIEvents(const OUIControlRef& pUIScreen)
{
    curARROW = LoadCursor(nullptr, IDC_ARROW);
    curIBEAM = LoadCursor(nullptr, IDC_IBEAM);

    // Tool Bar
    pUIScreen->getChild<onut::UICheckBox>("chkEditorStyle")->onCheckChanged = [&](const OUICheckBoxRef& pControl, const onut::UICheckEvent& checkEvent){
        if (checkEvent.isChecked)
        {
            g_pDocument->pUIContext = g_pDocument->pUIContextEditorStyle;
        }
        else
        {
            g_pDocument->pUIContext = g_pDocument->pUIContextUserStyle;
        }
    };
    pUIScreen->getChild("toolNew")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_FILE_NEW); };
    pUIScreen->getChild("toolOpen")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_FILE_OPEN); };
    pUIScreen->getChild("toolSave")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_FILE_SAVE); };
    pUIScreen->getChild("toolCut")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_CUT); };
    pUIScreen->getChild("toolCopy")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_COPY); };
    pUIScreen->getChild("toolPaste")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_PASTE); };
    pUIScreen->getChild("toolDelete")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_DELETE); };
    pUIScreen->getChild("toolUndo")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_UNDO); };
    pUIScreen->getChild("toolRedo")->onClick = [](const OUIControlRef& pControl, const onut::UIMouseEvent& evt){onMenu(MENU_EDIT_REDO); };

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
    ODynamicCast<OUITreeView>(pUIScreen->getChild("sceneGraph"))->onSelectionChanged = onSceneGraphSelectionChanged;

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
    g_pInspector_UIControl_txtName->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtName->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };

    g_pInspector_UIControl_txtStyle = pUIScreen->getChild<onut::UITextBox>("txtStyle");
    g_pInspector_UIControl_txtStyle->onTextChanged = onUIControlStyleChanged;
    g_pInspector_UIControl_txtStyle->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtStyle->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };

    g_pInspector_UIControl_txtX = pUIScreen->getChild<onut::UITextBox>("txtX");
    g_pInspector_UIControl_txtX->onTextChanged = onUIControlXChanged;
    g_pInspector_UIControl_txtX->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtX->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    g_pInspector_UIControl_txtY = pUIScreen->getChild<onut::UITextBox>("txtY");
    g_pInspector_UIControl_txtY->onTextChanged = onUIControlYChanged;
    g_pInspector_UIControl_txtY->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtY->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    g_pInspector_UIControl_chkXPercent = pUIScreen->getChild<onut::UICheckBox>("chkXPercent");
    g_pInspector_UIControl_chkXPercent->onCheckChanged = onUIControlXPercentChanged;
    g_pInspector_UIControl_chkYPercent = pUIScreen->getChild<onut::UICheckBox>("chkYPercent");
    g_pInspector_UIControl_chkYPercent->onCheckChanged = onUIControlYPercentChanged;

    g_pInspector_UIControl_txtWidth = pUIScreen->getChild<onut::UITextBox>("txtWidth");
    g_pInspector_UIControl_txtWidth->onTextChanged = onUIControlWidthChanged;
    g_pInspector_UIControl_txtWidth->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtWidth->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    g_pInspector_UIControl_txtHeight = pUIScreen->getChild<onut::UITextBox>("txtHeight");
    g_pInspector_UIControl_txtHeight->onTextChanged = onUIControlHeightChanged;
    g_pInspector_UIControl_txtHeight->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtHeight->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
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
    g_pInspector_UIControl_txtAnchorX->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtAnchorX->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
    g_pInspector_UIControl_txtAnchorY = pUIScreen->getChild<onut::UITextBox>("txtAnchorY");
    g_pInspector_UIControl_txtAnchorY->onTextChanged = onUIControlAnchorYChanged;
    g_pInspector_UIControl_txtAnchorY->onMouseEnter = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curIBEAM); };
    g_pInspector_UIControl_txtAnchorY->onMouseLeave = [&](const OUIControlRef& pControl, const onut::UIMouseEvent& mouseEvent){oWindow->setCursor(curARROW); };
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
    BEGIN_BINDINGS(pUIScreen, OUIControl::Type::Panel, "pnlInspector_UIPanel");
    BIND_COLOR_PROPERTY<onut::UIPanel>("Background Color",
                                       [=](const OUIPanelRef& pPanel) -> Color {return pPanel->color; },
                                       [=](const OUIPanelRef& pPanel, const Color& color){ pPanel->color = color; });

    // UIButton
    BEGIN_BINDINGS(pUIScreen, OUIControl::Type::Button, "pnlInspector_UIButton");
    BIND_TEXT_COMPONENT<onut::UIButton>("Caption");
    BIND_SCALE9_COMPONENT<onut::UIButton>("Bg Image");

    // UILabel
    BEGIN_BINDINGS(pUIScreen, OUIControl::Type::Label, "pnlInspector_UILabel");
    BIND_TEXT_COMPONENT<onut::UILabel>("Text");
    
    // UIImage
    BEGIN_BINDINGS(pUIScreen, OUIControl::Type::Image, "pnlInspector_UIImage");
    BIND_SCALE9_COMPONENT<onut::UIImage>("Image");

    // UICheckBox
    BEGIN_BINDINGS(pUIScreen, OUIControl::Type::CheckBox, "pnlInspector_UICheckBox");
    BIND_TEXT_COMPONENT<onut::UICheckBox>("Text");
    BIND_BOOL_PROPERTY<onut::UICheckBox>("Checked",
                                         [](const OUICheckBoxRef& pControl) {return pControl->getIsChecked(); },
                                         [](const OUICheckBoxRef& pControl, const bool& isChecked){pControl->setIsChecked(isChecked); });

    // UITextBox
    BEGIN_BINDINGS(pUIScreen, OUIControl::Type::TextBox, "pnlInspector_UITextBox");
    BIND_TEXT_COMPONENT<onut::UITextBox>("Text");
    BIND_SCALE9_COMPONENT<onut::UITextBox>("Bg Image");
    BIND_BOOL_PROPERTY<onut::UITextBox>("Numerical",
                                        [](const OUITextBoxRef& pControl) {return pControl->getIsNumerical(); },
                                        [](const OUITextBoxRef& pControl, const bool& isNumerical){pControl->setIsNumerical(isNumerical); });
    BIND_NUMERIC_PROPERTY<onut::UITextBox>("Decimal",
                                        [](const OUITextBoxRef& pControl) {return std::to_string(pControl->getDecimalPrecision()); },
                                        [](const OUITextBoxRef& pControl, const std::string& decimal){pControl->setIsDecimalPrecision(std::stoi(decimal)); });
}

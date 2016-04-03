// Onut
#include <onut/UICheckBox.h>
#include <onut/UIContext.h>

// Private
#include "UIJson.h"

namespace onut
{
    static std::unordered_map<std::string, UICheckBox::CheckBehavior> checkBehaviorMap = {
        {"NORMAL", UICheckBox::CheckBehavior::Normal},
        {"OPTIONAL", UICheckBox::CheckBehavior::Optional},
        {"EXCLUSIVE", UICheckBox::CheckBehavior::Exclusive}
    };

    OUICheckBoxRef UICheckBox::create()
    {
        return std::shared_ptr<UICheckBox>(new UICheckBox());
    }

    void UICheckBox::operator=(const UIControl& other)
    {
        auto pOther = dynamic_cast<const UICheckBox*>(&other);
        if (pOther)
        {
            iconComponent = pOther->iconComponent;
            textComponent = pOther->textComponent;
            behavior = pOther->behavior;
            m_isChecked = pOther->m_isChecked;
        }
        UIControl::operator=(other);
    }

    void UICheckBox::setIsChecked(bool in_isChecked)
    {
        switch (behavior)
        {
            case CheckBehavior::Normal:
            {
                m_isChecked = in_isChecked;
                break;
            }
            case CheckBehavior::Optional:
            {
                if (!m_isChecked && in_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling.get() == this) continue;
                            if (pSibbling->getType() == Type::CheckBox)
                            {
                                auto pSibblingCheckBox = ODynamicCast<UICheckBox>(pSibbling);
                                if (pSibblingCheckBox->behavior == behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                    }
                                }
                            }
                        }
                    }
                }

                m_isChecked = in_isChecked;
                break;
            }
            case CheckBehavior::Exclusive:
            {
                if (!m_isChecked && in_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling.get() == this) continue;
                            if (pSibbling->getType() == Type::CheckBox)
                            {
                                auto pSibblingCheckBox = ODynamicCast<UICheckBox>(pSibbling);
                                if (pSibblingCheckBox->behavior == behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                    }
                                }
                            }
                        }
                    }

                    m_isChecked = in_isChecked;
                }
                break;
            }
        }
    }

    void UICheckBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        textComponent = getJsonTextComponent(jsonNode["textComponent"]);
        m_isChecked = getJsonBool(jsonNode["checked"], false);
        behavior = getJsonEnum(checkBehaviorMap, jsonNode["behavior"], CheckBehavior::Normal);
    }

    void UICheckBox::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonTextComponent(jsonNode, textComponent, allocator);
        setJsonBool(jsonNode, "checked", m_isChecked, allocator, false);
        setJsonString(jsonNode, "behavior", enumToString(checkBehaviorMap, behavior), allocator, "NORMAL");
    }

    void UICheckBox::renderControl(const OUIContextRef& context, const Rect& rect)
    {
        const auto& callback = context->getStyle<UICheckBox>(getStyle());
        if (callback)
        {
            callback->render(OThis, rect);
        }
        else
        {
            context->renderTextComponent(OThis, rect, textComponent);
        }
    }

    void UICheckBox::onClickInternal(const UIMouseEvent& evt)
    {
        switch (behavior)
        {
            case CheckBehavior::Normal:
            {
                m_isChecked = !m_isChecked;
                if (onCheckChanged)
                {
                    UICheckEvent checkEvt;
                    checkEvt.isChecked = m_isChecked;
                    checkEvt.pContext = evt.pContext;
                    onCheckChanged(ODynamicCast<UICheckBox>(OThis), checkEvt);
                }
                break;
            }
            case CheckBehavior::Optional:
            {
                if (!m_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling.get() == this) continue;
                            if (pSibbling->getType() == Type::CheckBox)
                            {
                                auto pSibblingCheckBox = ODynamicCast<UICheckBox>(pSibbling);
                                if (pSibblingCheckBox->behavior == behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                        if (pSibblingCheckBox->onCheckChanged)
                                        {
                                            UICheckEvent checkEvt;
                                            checkEvt.isChecked = pSibblingCheckBox->m_isChecked;
                                            checkEvt.pContext = evt.pContext;
                                            pSibblingCheckBox->onCheckChanged(ODynamicCast<UICheckBox>(OThis), checkEvt);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                m_isChecked = !m_isChecked;
                if (onCheckChanged)
                {
                    UICheckEvent checkEvt;
                    checkEvt.isChecked = m_isChecked;
                    checkEvt.pContext = evt.pContext;
                    onCheckChanged(ODynamicCast<UICheckBox>(OThis), checkEvt);
                }
                break;
            }
            case CheckBehavior::Exclusive:
            {
                if (!m_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling.get() == this) continue;
                            if (pSibbling->getType() == Type::CheckBox)
                            {
                                auto pSibblingCheckBox = ODynamicCast<UICheckBox>(pSibbling);
                                if (pSibblingCheckBox->behavior == behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                        if (pSibblingCheckBox->onCheckChanged)
                                        {
                                            UICheckEvent checkEvt;
                                            checkEvt.isChecked = pSibblingCheckBox->m_isChecked;
                                            checkEvt.pContext = evt.pContext;
                                            pSibblingCheckBox->onCheckChanged(ODynamicCast<UICheckBox>(OThis), checkEvt);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    m_isChecked = true;
                    if (onCheckChanged)
                    {
                        UICheckEvent checkEvt;
                        checkEvt.isChecked = m_isChecked;
                        checkEvt.pContext = evt.pContext;
                        onCheckChanged(ODynamicCast<UICheckBox>(OThis), checkEvt);
                    }
                }
                break;
            }
        }
    }
};

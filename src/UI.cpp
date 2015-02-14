#include "UI.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

// Copyright (C) 2011 Milo Yip
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef RAPIDJSON_FILEWRITESTREAM_H_
#define RAPIDJSON_FILEWRITESTREAM_H_

#include "rapidjson.h"
#include <cstdio>

namespace rapidjson
{

    //! Wrapper of C file stream for input using fread().
    /*!
    \note implements Stream concept
    */
    class FileWriteStream
    {
    public:
        typedef char Ch;    //!< Character type. Only support char.

        FileWriteStream(FILE* fp, char* buffer, size_t bufferSize) : fp_(fp), buffer_(buffer), bufferEnd_(buffer + bufferSize), current_(buffer_)
        {
            RAPIDJSON_ASSERT(fp_ != 0);
        }

        void Put(char c)
        {
            if (current_ >= bufferEnd_)
                Flush();

            *current_++ = c;
        }

        void PutN(char c, size_t n)
        {
            size_t avail = static_cast<size_t>(bufferEnd_ - current_);
            while (n > avail)
            {
                std::memset(current_, c, avail);
                current_ += avail;
                Flush();
                n -= avail;
                avail = static_cast<size_t>(bufferEnd_ - current_);
            }

            if (n > 0)
            {
                std::memset(current_, c, n);
                current_ += n;
            }
        }

        void Flush()
        {
            if (current_ != buffer_)
            {
                fwrite(buffer_, 1, static_cast<size_t>(current_ - buffer_), fp_);
                current_ = buffer_;
            }
        }

        // Not implemented
        char Peek() const { RAPIDJSON_ASSERT(false); return 0; }
        char Take() { RAPIDJSON_ASSERT(false); return 0; }
        size_t Tell() const { RAPIDJSON_ASSERT(false); return 0; }
        char* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
        size_t PutEnd(char*) { RAPIDJSON_ASSERT(false); return 0; }

    private:
        // Prohibit copy constructor & assignment operator.
        FileWriteStream(const FileWriteStream&);
        FileWriteStream& operator=(const FileWriteStream&);

        FILE* fp_;
        char *buffer_;
        char *bufferEnd_;
        char *current_;
    };

    //! Implement specialized version of PutN() with memset() for better performance.
    template<>
    inline void PutN(FileWriteStream& stream, char c, size_t n)
    {
        stream.PutN(c, n);
    }

} // namespace rapidjson

#endif // RAPIDJSON_FILESTREAM_H_

#include <algorithm>
#include <functional>
#include <sstream>
#include <iomanip>

namespace onut
{
    unsigned int uiHash(const char* s, unsigned int seed)
    {
        unsigned hash = seed;
        while (*s)
        {
            hash = hash * 101 + *s++;
        }
        return hash;
    }

    const char* getStringFromType(eUIType type)
    {
        switch (type)
        {
            case eUIType::UI_CONTROL: return "UIControl";
            case eUIType::UI_BUTTON: return "UIButton";
            case eUIType::UI_PANEL: return "UIPanel";
            case eUIType::UI_LABEL: return "UILabel";
            case eUIType::UI_IMAGE: return "UIImage";
            case eUIType::UI_CHECKBOX: return "UICheckBox";
            case eUIType::UI_TREEVIEW: return "UITreeView";
            case eUIType::UI_TEXTBOX: return "UITextBox";
        }
        return "";
    }

    eUIAlign getAlignFromString(const char* szAlign)
    {
        if (!strcmp(szAlign, "TOP_LEFT"))
        {
            return eUIAlign::TOP_LEFT;
        }
        else if (!strcmp(szAlign, "TOP"))
        {
            return eUIAlign::TOP;
        }
        else if (!strcmp(szAlign, "TOP_RIGHT"))
        {
            return eUIAlign::TOP_RIGHT;
        }
        else if (!strcmp(szAlign, "LEFT"))
        {
            return eUIAlign::LEFT;
        }
        else if (!strcmp(szAlign, "CENTER"))
        {
            return eUIAlign::CENTER;
        }
        else if (!strcmp(szAlign, "RIGHT"))
        {
            return eUIAlign::RIGHT;
        }
        else if (!strcmp(szAlign, "BOTTOM_LEFT"))
        {
            return eUIAlign::BOTTOM_LEFT;
        }
        else if (!strcmp(szAlign, "BOTTOM"))
        {
            return eUIAlign::BOTTOM;
        }
        else if (!strcmp(szAlign, "BOTTOM_RIGHT"))
        {
            return eUIAlign::BOTTOM_RIGHT;
        }
    
        return eUIAlign::TOP_LEFT;
    }

    const char* getStringFromAlign(eUIAlign align)
    {
        switch (align)
        {
            case eUIAlign::TOP_LEFT: return "TOP_LEFT";
            case eUIAlign::TOP: return "TOP";
            case eUIAlign::TOP_RIGHT: return "TOP_RIGHT";
            case eUIAlign::LEFT: return "LEFT";
            case eUIAlign::CENTER: return "CENTER";
            case eUIAlign::RIGHT: return "RIGHT";
            case eUIAlign::BOTTOM_LEFT: return "BOTTOM_LEFT";
            case eUIAlign::BOTTOM: return "BOTTOM";
            case eUIAlign::BOTTOM_RIGHT: return "BOTTOM_RIGHT";
        }
        return "";
    }

    eUIDimType getDimTypeFromString(const char* szDimType)
    {
        if (!strcmp(szDimType, "ABSOLUTE"))
        {
            return eUIDimType::DIM_ABSOLUTE;
        }
        else if (!strcmp(szDimType, "RELATIVE"))
        {
            return eUIDimType::DIM_RELATIVE;
        }
        else if (!strcmp(szDimType, "PERCENTAGE"))
        {
            return eUIDimType::DIM_PERCENTAGE;
        }

        return eUIDimType::DIM_ABSOLUTE;
    }

    const char* getStringFromDimType(eUIDimType dimType)
    {
        switch (dimType)
        {
            case eUIDimType::DIM_ABSOLUTE: return "ABSOLUTE";
            case eUIDimType::DIM_RELATIVE: return "RELATIVE";
            case eUIDimType::DIM_PERCENTAGE: return "PERCENTAGE";
        }
        return "";
    }

    eUIPosType getPosTypeFromString(const char* szPosType)
    {
        if (!strcmp(szPosType, "RELATIVE"))
        {
            return eUIPosType::POS_RELATIVE;
        }
        else if (!strcmp(szPosType, "PERCENTAGE"))
        {
            return eUIPosType::POS_PERCENTAGE;
        }

        return eUIPosType::POS_RELATIVE;
    }

    const char* getStringFromPosType(eUIPosType posType)
    {
        switch (posType)
        {
            case eUIPosType::POS_RELATIVE: return "RELATIVE";
            case eUIPosType::POS_PERCENTAGE: return "PERCENTAGE";
        }
        return "";
    }

    eUIAnchorType getAnchorTypeFromString(const char* szAnchorType)
    {
        if (!strcmp(szAnchorType, "PIXEL"))
        {
            return eUIAnchorType::ANCHOR_PIXEL;
        }
        else if (!strcmp(szAnchorType, "PERCENTAGE"))
        {
            return eUIAnchorType::ANCHOR_PERCENTAGE;
        }

        return eUIAnchorType::ANCHOR_PERCENTAGE;
    }

    const char* getStringFromAnchorType(eUIAnchorType anchorType)
    {
        switch (anchorType)
        {
            case eUIAnchorType::ANCHOR_PIXEL: return "PIXEL";
            case eUIAnchorType::ANCHOR_PERCENTAGE: return "PERCENTAGE";
        }
        return "";
    }

    eUICheckBehavior getJsonCheckBehavior(const char* szCheckBehavior)
    {
        if (!strcmp(szCheckBehavior, "OPTIONAL"))
        {
            return eUICheckBehavior::OPTIONAL;
        }
        else if (!strcmp(szCheckBehavior, "EXCLUSIVE"))
        {
            return eUICheckBehavior::EXCLUSIVE;
        }

        return eUICheckBehavior::NORMAL;
    }

    const char* getStringFromCheckBehavior(eUICheckBehavior checkBehavior)
    {
        switch (checkBehavior)
        {
            case eUICheckBehavior::OPTIONAL: return "OPTIONAL";
            case eUICheckBehavior::EXCLUSIVE: return "EXCLUSIVE";
        }
        return "";
    }

    static const char* getJsonString(const rapidjson::Value& jsonNode, const char* szDefault = "")
    {
        if (jsonNode.IsString())
        {
            return jsonNode.GetString();
        }
        else
        {
            return szDefault;
        }
    }
    
    static float getJsonFloat(const rapidjson::Value& jsonNode, const float default = 0.f)
    {
        if (jsonNode.IsNumber())
        {
            return static_cast<float>(jsonNode.GetDouble());
        }
        else
        {
            return default;
        }
    }
    
    static int getJsonInt(const rapidjson::Value& jsonNode, const int default = 0)
    {
        if (jsonNode.IsInt())
        {
            return jsonNode.GetInt();
        }
        else
        {
            return default;
        }
    }

    static bool getJsonBool(const rapidjson::Value& jsonNode, const bool default = false)
    {
        if (jsonNode.IsBool())
        {
            return jsonNode.GetBool();
        }
        else
        {
            return default;
        }
    }

    static void setJsonFloat(rapidjson::Value& jsonNode, const char* szName, float value, rapidjson::Allocator& allocator, float default = 0.f)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    static void setJsonInt(rapidjson::Value& jsonNode, const char* szName, int value, rapidjson::Allocator& allocator, int default = 0)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    static void setJsonString(rapidjson::Value& jsonNode, const char* szName, const char* szValue, rapidjson::Allocator& allocator, const char* default = "")
    {
        if (!strcmp(szValue, default)) return;
        jsonNode.AddMember(szName, szValue, allocator);
    }

    static void setJsonBool(rapidjson::Value& jsonNode, const char* szName, const bool value, rapidjson::Allocator& allocator, bool default = false)
    {
        if (value == default) return;
        jsonNode.AddMember(szName, value, allocator);
    }

    static void setJsonColor(rapidjson::Value& jsonNode, const char* szName, const sUIColor& value, rapidjson::Allocator& allocator, const sUIColor& default = {1, 1, 1, 1, 0xffffffff})
    {
        if (value.packed == default.packed) return;
        //jsonNode.AddMember(szName, value, allocator);
    }

    static sUIColor getJsonColor(const rapidjson::Value& jsonNode, const sUIColor& default = {1, 1, 1, 1, 0xffffffff})
    {
        if (jsonNode.IsString())
        {
            sUIColor ret;
            auto szStr = jsonNode.GetString();
            auto len = jsonNode.GetStringLength();
            if (len != 8)
            {
                return default;
            }
            ret.packed = static_cast<uint32_t>(strtoul(szStr, NULL, 16));
            ret.r = (float)((ret.packed >> 24) & 0xff) / 255.f;
            ret.g = (float)((ret.packed >> 16) & 0xff) / 255.f;
            ret.b = (float)((ret.packed >> 8) & 0xff) / 255.f;
            ret.a = (float)(ret.packed & 0xff) / 255.f;
            return std::move(ret);
        }
        else
        {
            return default;
        }
    }

    UIContext::UIContext(const sUIVector2& screenSize) :
        m_screenSize(screenSize)
    {
    }

    UIContext::~UIContext()
    {
        for (auto& kv : m_callbacks)
        {
            for (auto& kv2 : kv.second)
            {
                delete kv2.second;
            }
        }
    }

    void UIContext::resize(const sUIVector2& screenSize)
    {
        m_screenSize = screenSize;
    }

    UIProperty::UIProperty()
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = eUIPropertyType::P_INT;
    }

    UIProperty::UIProperty(const rapidjson::Value& jsonNode)
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        if (jsonNode.IsNumber())
        {
            if (jsonNode.IsDouble())
            {
                m_type = eUIPropertyType::P_FLOAT;
                m_float = static_cast<float>(jsonNode.GetDouble());
            }
            else if (jsonNode.IsInt())
            {
                m_type = eUIPropertyType::P_INT;
                m_int = jsonNode.GetInt();
            }
            else
            {
                assert(false);
            }
        }
        else if (jsonNode.IsBool())
        {
            m_type = eUIPropertyType::P_BOOL;
            m_bool = jsonNode.GetBool();
        }
        else if (jsonNode.IsString())
        {
            m_type = eUIPropertyType::P_STRING;
            auto len = jsonNode.GetStringLength();
            auto szString = jsonNode.GetString();
            m_szString = new char[len + 1];
            memcpy(m_szString, szString, len + 1);
        }
        else
        {
            assert(false);
        }
    }

    UIProperty::UIProperty(const UIProperty& other)
    {
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = other.m_type;
        switch (m_type)
        {
            case eUIPropertyType::P_BOOL:
                m_bool = other.m_bool;
                break;
            case eUIPropertyType::P_INT:
                m_int = other.m_int;
                break;
            case eUIPropertyType::P_FLOAT:
                m_float = other.m_float;
                break;
            case eUIPropertyType::P_STRING:
            {
                auto len = strlen(other.m_szString);
                m_szString = new char[len + 1];
                memcpy(m_szString, other.m_szString, len + 1);
                break;
            }
        }
    }

    UIProperty& UIProperty::operator=(const UIProperty& other)
    {
        if (m_type == eUIPropertyType::P_STRING)
        {
            delete[] m_szString;
        }
        m_szString = nullptr;
        m_bool = false;
        m_int = 0;
        m_float = 0.f;
        m_type = other.m_type;
        switch (m_type)
        {
            case eUIPropertyType::P_BOOL:
                m_bool = other.m_bool;
                break;
            case eUIPropertyType::P_INT:
                m_int = other.m_int;
                break;
            case eUIPropertyType::P_FLOAT:
                m_float = other.m_float;
                break;
            case eUIPropertyType::P_STRING:
            {
                auto len = strlen(other.m_szString);
                m_szString = new char[len + 1];
                memcpy(m_szString, other.m_szString, len + 1);
                break;
            }
        }
        return *this;
    }

    UIProperty::~UIProperty()
    {
        if (m_type == eUIPropertyType::P_STRING)
        {
            delete[] m_szString;
        }
    }

    const char* UIProperty::getString() const
    {
        assert(m_type == eUIPropertyType::P_STRING);
        return m_szString;
    }

    int UIProperty::getInt() const
    {
        assert(m_type == eUIPropertyType::P_INT);
        return m_int;
    }

    float UIProperty::getFloat() const
    {
        assert(m_type == eUIPropertyType::P_FLOAT || m_type == eUIPropertyType::P_INT);
        if (m_type == eUIPropertyType::P_FLOAT)
        {
            return m_float;
        }
        else
        {
            return static_cast<float>(m_int);
        }
    }

    bool UIProperty::getBool() const
    {
        assert(m_type == eUIPropertyType::P_BOOL || m_type == eUIPropertyType::P_INT);
        if (m_type == eUIPropertyType::P_BOOL)
        {
            return m_bool;
        }
        else
        {
            return m_int ? true : false;
        }
    }

    UIControl::UIControl()
    {
        m_posType[0] = eUIPosType::POS_RELATIVE;
        m_posType[1] = eUIPosType::POS_RELATIVE;
        m_dimType[0] = eUIDimType::DIM_ABSOLUTE;
        m_dimType[1] = eUIDimType::DIM_ABSOLUTE;
        m_anchorType[0] = eUIAnchorType::ANCHOR_PERCENTAGE;
        m_anchorType[1] = eUIAnchorType::ANCHOR_PERCENTAGE;
    }

    UIControl::UIControl(const char* szFilename)
    {
        // Open json file
        FILE* pFile = nullptr;
        auto fopenRet = fopen_s(&pFile, szFilename, "r");
        assert(!fopenRet);
        rapidjson::FileStream is(pFile);
        rapidjson::Document doc;
        doc.ParseStream<0>(is);

        load(doc);

        // Close the file
        fclose(pFile);
    }

    void UIControl::load(const rapidjson::Value& jsonNode)
    {
        m_rect.position.x = getJsonFloat(jsonNode["x"]);
        m_rect.position.y = getJsonFloat(jsonNode["y"]);
        m_rect.size.x = getJsonFloat(jsonNode["width"], 0.f);
        m_rect.size.y = getJsonFloat(jsonNode["height"], 0.f);
        m_anchor.x = getJsonFloat(jsonNode["xAnchor"]);
        m_anchor.y = getJsonFloat(jsonNode["yAnchor"]);

        m_align = getAlignFromString(getJsonString(jsonNode["align"]));
        m_posType[0] = getPosTypeFromString(getJsonString(jsonNode["xType"]));
        m_posType[1] = getPosTypeFromString(getJsonString(jsonNode["yType"]));
        m_dimType[0] = getDimTypeFromString(getJsonString(jsonNode["widthType"]));
        m_dimType[1] = getDimTypeFromString(getJsonString(jsonNode["heightType"]));
        m_anchorType[0] = getAnchorTypeFromString(getJsonString(jsonNode["anchorType"]));
        m_anchorType[1] = getAnchorTypeFromString(getJsonString(jsonNode["anchorType"]));

        m_name = getJsonString(jsonNode["name"]);
        m_styleName = getJsonString(jsonNode["style"]);
        m_style = uiHash(m_styleName.c_str());

        m_isEnabled = getJsonBool(jsonNode["enabled"], true);
        m_isVisible = getJsonBool(jsonNode["visible"], true);
        m_isClickThrough = getJsonBool(jsonNode["clickThrough"], false);

        // Properties
        auto& properties = jsonNode["properties"];
        if (!properties.IsNull())
        {
            for (auto it = properties.MemberBegin(); it != properties.MemberEnd(); ++it)
            {
                m_properties[it->name.GetString()] = UIProperty(it->value);
            }
        }

        // Load children
        auto& jsonChildren = jsonNode["children"];
        if (jsonChildren.IsArray())
        {
            auto size = jsonChildren.Size();
            for (decltype(size) i = 0; i < size; ++i)
            {
                auto& jsonChild = jsonChildren[i];
                auto jsonChildType = getJsonString(jsonChild["type"], "UIControl");
                UIControl* pChild = nullptr;
                if (!strcmp(jsonChildType, "UIControl"))
                {
                    pChild = new UIControl();
                }
                else if (!strcmp(jsonChildType, "UIButton"))
                {
                    pChild = new UIButton();
                }
                else if (!strcmp(jsonChildType, "UIPanel"))
                {
                    pChild = new UIPanel();
                }
                else if (!strcmp(jsonChildType, "UILabel"))
                {
                    pChild = new UILabel();
                }
                else if (!strcmp(jsonChildType, "UIImage"))
                {
                    pChild = new UIImage();
                }
                else if (!strcmp(jsonChildType, "UICheckBox"))
                {
                    pChild = new UICheckBox();
                }
                else if (!strcmp(jsonChildType, "UITreeView"))
                {
                    pChild = new UITreeView();
                }
                else if (!strcmp(jsonChildType, "UITextBox"))
                {
                    pChild = new UITextBox();
                }
                if (!pChild) continue;
                add(pChild);
                pChild->load(jsonChild);
            }
        }
    }

    void UIControl::save(const std::string& filename) const
    {
        rapidjson::Document doc;
        doc.SetObject();
        auto& allocator = doc.GetAllocator();
        save(doc, allocator);

        // Open json file
        FILE* pFile = nullptr;
        auto fopenRet = fopen_s(&pFile, filename.c_str(), "wb");
        assert(!fopenRet);

        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        doc.Accept(writer);

        auto str = s.GetString();
        fwrite(str, 1, strlen(str), pFile);

        fclose(pFile);
    }

    void UIControl::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        setJsonString(jsonNode, "type", getStringFromType(getType()), allocator);

        setJsonFloat(jsonNode, "x", m_rect.position.x, allocator);
        setJsonFloat(jsonNode, "y", m_rect.position.y, allocator);
        setJsonFloat(jsonNode, "width", m_rect.size.x, allocator);
        setJsonFloat(jsonNode, "height", m_rect.size.y, allocator);
        setJsonFloat(jsonNode, "xAnchor", m_anchor.x, allocator);
        setJsonFloat(jsonNode, "yAnchor", m_anchor.y, allocator);

        setJsonString(jsonNode, "align", getStringFromAlign(m_align), allocator, "TOP_LEFT");
        setJsonString(jsonNode, "xType", getStringFromPosType(m_posType[0]), allocator, "RELATIVE");
        setJsonString(jsonNode, "yType", getStringFromPosType(m_posType[1]), allocator, "RELATIVE");
        setJsonString(jsonNode, "widthType", getStringFromDimType(m_dimType[0]), allocator, "ABSOLUTE");
        setJsonString(jsonNode, "heightType", getStringFromDimType(m_dimType[1]), allocator, "ABSOLUTE");
        setJsonString(jsonNode, "xAnchorType", getStringFromAnchorType(m_anchorType[0]), allocator, "PERCENTAGE");
        setJsonString(jsonNode, "yAnchorType", getStringFromAnchorType(m_anchorType[1]), allocator, "PERCENTAGE");

        setJsonString(jsonNode, "name", m_name.c_str(), allocator);
        setJsonString(jsonNode, "style", m_styleName.c_str(), allocator);

        setJsonBool(jsonNode, "enabled", m_isEnabled, allocator, true);
        setJsonBool(jsonNode, "visible", m_isVisible, allocator, true);
        setJsonBool(jsonNode, "clickThrough", m_isClickThrough, allocator, false);

        for (auto& kv : m_properties)
        {
            switch (kv.second.getType())
            {
                case eUIPropertyType::P_BOOL:
                    jsonNode[kv.first.c_str()] = kv.second.getBool();
                    break;
                case eUIPropertyType::P_FLOAT:
                    jsonNode[kv.first.c_str()] = kv.second.getFloat();
                    break;
                case eUIPropertyType::P_INT:
                    jsonNode[kv.first.c_str()] = kv.second.getInt();
                    break;
                case eUIPropertyType::P_STRING:
                    jsonNode[kv.first.c_str()] = kv.second.getString();
                    break;
            }
        }

        if (!m_children.empty())
        {
            rapidjson::Value jsonChildren(rapidjson::kArrayType);
            for (auto pChild : m_children)
            {
                rapidjson::Value jsonChild(rapidjson::kObjectType);
                pChild->save(jsonChild, allocator);
                jsonChildren.PushBack(jsonChild, allocator);
            }
            jsonNode.AddMember("children", jsonChildren, allocator);
        }
    }

    UIControl::UIControl(const UIControl& other)
    {
        m_isEnabled = other.m_isEnabled;
        m_isClickThrough = other.m_isClickThrough;
        m_isVisible = other.m_isVisible;
        m_rect = other.m_rect;
        m_align = other.m_align;
        m_posType[0] = other.m_posType[0];
        m_posType[1] = other.m_posType[1];
        m_dimType[0] = other.m_dimType[0];
        m_dimType[1] = other.m_dimType[1];
        m_name = other.m_name;
        m_pUserData = other.m_pUserData;
        m_properties = other.m_properties;

        for (auto pChild : m_children)
        {
            switch (pChild->getType())
            {
                case eUIType::UI_CONTROL:
                    add(new UIControl(*pChild));
                    break;
                case eUIType::UI_BUTTON:
                    add(new UIButton(*(UIButton*)pChild));
                    break;
                case eUIType::UI_PANEL:
                    add(new UIPanel(*(UIPanel*)pChild));
                    break;
                case eUIType::UI_LABEL:
                    add(new UILabel(*(UILabel*)pChild));
                    break;
                case eUIType::UI_IMAGE:
                    add(new UIImage(*(UIImage*)pChild));
                    break;
                case eUIType::UI_CHECKBOX:
                    add(new UICheckBox(*(UICheckBox*)pChild));
                    break;
                case eUIType::UI_TREEVIEW:
                    add(new UITreeView(*(UITreeView*)pChild));
                    break;
                case eUIType::UI_TEXTBOX:
                    add(new UITextBox(*(UITextBox*)pChild));
                    break;
            }
        }
    }

    UIControl::~UIControl()
    {
        for (auto pChild : m_children)
        {
            pChild->release();
        }
    }

    void UIControl::add(UIControl* pChild)
    {
        pChild->retain();
        if (pChild->m_pParent)
        {
            pChild->m_pParent->remove(pChild);
        }

        pChild->m_pParent = this;
        m_children.push_back(pChild);
    }

    void UIControl::remove(UIControl* in_pChild)
    {
        if (in_pChild->m_pParent != this) return;

        auto size = m_children.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            auto pChild = m_children[i];
            if (pChild == in_pChild)
            {
                m_children.erase(m_children.begin() + i);
                break;
            }
        }

        in_pChild->m_pParent = nullptr;
        in_pChild->release();
    }

    UIControl* UIControl::getChild(const std::string& name, bool bSearchSubChildren) const
    {
        for (auto pChild : m_children)
        {
            if (pChild->m_name == name)
            {
                return pChild;
            }
        }

        if (bSearchSubChildren)
        {
            for (auto pChild : m_children)
            {
                auto pRet = pChild->getChild(name, bSearchSubChildren);
                if (pRet)
                {
                    return pRet;
                }
            }
        }

        return nullptr;
    }

    void UIControl::getChild(const UIContext& context, 
                             const sUIVector2& mousePos, 
                             bool bSearchSubChildren, 
                             bool bIgnoreClickThrough,
                             const sUIRect& parentRect, 
                             const UIControl** ppHoverControl) const
    {
        if (!isVisible() || isClickThrough() && bIgnoreClickThrough) return;

        sUIRect worldRect = getWorldRect(parentRect);
        auto itend = m_children.rend();
        for (auto it = m_children.rbegin(); it != itend; ++it)
        {
            auto pChild = *it;
            pChild->getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough, worldRect, ppHoverControl);
        }

        if (!*ppHoverControl)
        {
            if (mousePos.x >= worldRect.position.x &&
                mousePos.y >= worldRect.position.y &&
                mousePos.x <= worldRect.position.x + worldRect.size.x &&
                mousePos.y <= worldRect.position.y + worldRect.size.y)
            {
                *ppHoverControl = this;
            }
        }
    }

    UIControl* UIControl::getChild(const UIContext& context, 
                                   const sUIVector2& mousePos, 
                                   bool bSearchSubChildren,
                                   bool bIgnoreClickThrough) const
    {
        sUIRect parentWorldRect;
        if (getParent())
        {
            parentWorldRect = getParent()->getWorldRect(context);
        }
        else
        {
            parentWorldRect = {{0, 0}, context.getScreenSize()};
        }
        const UIControl* pHoverControl = nullptr;
        getChild(context, mousePos, bSearchSubChildren, bIgnoreClickThrough, parentWorldRect, &pHoverControl);
        if (pHoverControl == this)
        {
            pHoverControl = nullptr;
        }
        return (UIControl*)pHoverControl; // cast from const to not const. Chuuut don't tell anyone
    }

    void UIControl::setStyle(const char* szStyle)
    {
        m_styleName = szStyle;
        m_style = uiHash(szStyle);
    }

    void UIControl::retain()
    {
        ++m_refCount;
    }

    void UIControl::release()
    {
        --m_refCount;
        if (m_refCount <= 0)
        {
            delete this;
        }
    }

    void UIControl::update(UIContext& context, const sUIVector2& mousePos, bool bMouseDown)
    {
        // Prepare our data
        sUIRect parentRect = {{0, 0}, context.getScreenSize()};
        context.m_mouseEvent.mousePos = mousePos;
        context.m_mouseEvent.isMouseDown = bMouseDown;
        context.m_mouseEvent.pContext = &context;
        context.m_pHoverControl = nullptr;

        // Update UIs
        updateInternal(context, parentRect);

        // Resolve
        context.resolve();

        // Send events
        context.dispatchEvents();

        // Reset for next frame
        context.reset();
    }

    void UIContext::resolve()
    {
        // If it's the first mouse down since last frame, and there is an hover
        // control. Put him into down state
        if (!m_lastMouseEvent.isMouseDown && 
            m_mouseEvent.isMouseDown &&
            m_pHoverControl)
        {
            m_pDownControl = m_pHoverControl;
        }

        if (!m_mouseEvent.isMouseDown)
        {
            m_pDownControl = nullptr;
        }

        // Don't allow hovering of other controls if a control is in DOWN state
        if (m_pDownControl && 
            m_pHoverControl != m_pDownControl)
        {
            m_pHoverControl = nullptr;
        }

        if (m_pDownControl)
        {
            m_pFocus = m_pDownControl;
        }
    }

    void UIContext::dispatchEvents()
    {
        // Do writes
        if (m_pFocus)
        {
            for (auto c : m_writes)
            {
                m_pFocus->onWriteInternal(c, *this);
            }
        }
        m_writes.clear();

        // Do key events
        if (m_pFocus)
        {
            UIKeyEvent evt;
            evt.pContext = this;
            for (auto key : m_keyDowns)
            {
                evt.key = key;
                m_pFocus->onKeyDownInternal(evt);
                if (m_pFocus->onKeyDown)
                {
                    m_pFocus->onKeyDown(m_pFocus, evt);
                }
            }
        }
        m_keyDowns.clear();

        if (m_pHoverControl != m_pLastHoverControl)
        {
            if (m_pLastHoverControl)
            {
                if (m_pLastHoverControl->onMouseLeave)
                {
                    m_pLastHoverControl->onMouseLeave(m_pLastHoverControl, m_mouseEvent);
                }
            }
            if (m_pHoverControl)
            {
                if (m_pHoverControl->onMouseEnter)
                {
                    m_pHoverControl->onMouseEnter(m_pHoverControl, m_mouseEvent);
                }
            }
        }
        if (m_pHoverControl)
        {
            if (m_lastMouseEvent.mousePos.x != m_mouseEvent.mousePos.x ||
                m_lastMouseEvent.mousePos.y != m_mouseEvent.mousePos.y)
            {
                m_pHoverControl->onMouseMoveInternal(m_mouseEvent);
                if (m_pHoverControl->onMouseMove)
                {
                    m_pHoverControl->onMouseMove(m_pHoverControl, m_mouseEvent);
                }
            }
        }
        if (m_pDownControl != m_pLastDownControl)
        {
            if (m_pLastDownControl)
            {
                m_pLastDownControl->onMouseUpInternal(m_mouseEvent);
                if (m_pLastDownControl->onMouseUp)
                {
                    m_pLastDownControl->onMouseUp(m_pLastDownControl, m_mouseEvent);
                }
                if (m_pHoverControl == m_pLastDownControl &&
                    !m_mouseEvent.isMouseDown)
                {
                    m_pLastDownControl->onClickInternal(m_mouseEvent);
                    if (m_pLastDownControl->onClick)
                    {
                        m_pLastDownControl->onClick(m_pLastDownControl, m_mouseEvent);
                    }
                }
            }
            if (m_pDownControl)
            {
                m_pDownControl->onMouseDownInternal(m_mouseEvent);
                if (m_pDownControl->onMouseDown)
                {
                    m_pDownControl->onMouseDown(m_pDownControl, m_mouseEvent);
                }
            }
        }

        // Focus gain/lost
        if (m_pFocus != m_pLastFocus)
        {
            if (m_pLastFocus)
            {
                UIFocusEvent evt;
                evt.pContext = this;
                m_pLastFocus->onLoseFocusInternal(evt);
                if (m_pLastFocus->onLoseFocus)
                {
                    m_pLastFocus->onLoseFocus(m_pLastFocus);
                }
            }
            if (m_pFocus)
            {
                UIFocusEvent evt;
                evt.pContext = this;
                m_pFocus->onGainFocusInternal(evt);
                if (m_pFocus->onGainFocus)
                {
                    m_pFocus->onGainFocus(m_pFocus);
                }
            }
        }
    }

    void UIContext::reset()
    {
        m_lastMouseEvent = m_mouseEvent;
        m_pLastHoverControl = m_pHoverControl;
        m_pLastDownControl = m_pDownControl;
        m_pLastFocus = m_pFocus;
    }

    void UIContext::write(char c)
    {
        m_writes.push_back(c);
    }

    void UIContext::keyDown(uintptr_t key)
    {
        m_keyDowns.push_back(key);
    }

    void UIContext::focus(UIControl* pFocus)
    {
        m_pFocus = pFocus;
    }

    void UIControl::render(const UIContext& context) const
    {
        sUIRect parentRect = {{0, 0}, context.getScreenSize() };
        renderInternal(context, parentRect);
    }

    sUIRect UIControl::getWorldRect(const UIContext& context) const
    {
        if (m_pParent)
        {
            return std::move(getWorldRect(m_pParent->getWorldRect(context)));
        }
        else
        {        
            sUIRect parentRect = {{0, 0}, context.getScreenSize()};
            return std::move(getWorldRect(parentRect));
        }
    }

    sUIRect UIControl::getWorldRect(const sUIRect& parentRect) const
    {
        sUIRect worldRect;

        // Generate position and width values first
        switch (m_posType[0])
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.x = m_rect.position.x;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.x = parentRect.size.x * m_rect.position.x;
                break;
        }
        switch (m_posType[1])
        {
            case eUIPosType::POS_RELATIVE:
                worldRect.position.y = m_rect.position.y;
                break;
            case eUIPosType::POS_PERCENTAGE:
                worldRect.position.y = parentRect.size.y * m_rect.position.y;
                break;
        }
        switch (m_dimType[0])
        {
            case eUIDimType::DIM_ABSOLUTE:
                worldRect.size.x = m_rect.size.x;
                break;
            case eUIDimType::DIM_RELATIVE:
                worldRect.size.x = parentRect.size.x + m_rect.size.x;
                break;
            case eUIDimType::DIM_PERCENTAGE:
                worldRect.size.x = parentRect.size.x * m_rect.size.x;
                break;
        }
        switch (m_dimType[1])
        {
            case eUIDimType::DIM_ABSOLUTE:
                worldRect.size.y = m_rect.size.y;
                break;
            case eUIDimType::DIM_RELATIVE:
                worldRect.size.y = parentRect.size.y + m_rect.size.y;
                break;
            case eUIDimType::DIM_PERCENTAGE:
                worldRect.size.y = parentRect.size.y * m_rect.size.y;
                break;
        }

        // Then do alignement
        switch (m_align)
        {
            case eUIAlign::TOP_LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::TOP:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::TOP_RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + worldRect.position.y;
                break;
            case eUIAlign::LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::CENTER:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y * .5f + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM_LEFT:
                worldRect.position.x = parentRect.position.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM:
                worldRect.position.x = parentRect.position.x + parentRect.size.x * .5f + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y + worldRect.position.y;
                break;
            case eUIAlign::BOTTOM_RIGHT:
                worldRect.position.x = parentRect.position.x + parentRect.size.x + worldRect.position.x;
                worldRect.position.y = parentRect.position.y + parentRect.size.y + worldRect.position.y;
                break;
        }

        // Finally anchoring
        switch (m_anchorType[0])
        {
            case eUIAnchorType::ANCHOR_PIXEL:
                worldRect.position.x -= m_anchor.x;
                break;
            case eUIAnchorType::ANCHOR_PERCENTAGE:
                worldRect.position.x -= worldRect.size.x * m_anchor.x;
                break;
        }
        switch (m_anchorType[1])
        {
            case eUIAnchorType::ANCHOR_PIXEL:
                worldRect.position.y -= m_anchor.y;
                break;
            case eUIAnchorType::ANCHOR_PERCENTAGE:
                worldRect.position.y -= worldRect.size.y * m_anchor.y;
                break;
        }

        // Snap to pixels
        worldRect.position.x = std::roundf(worldRect.position.x);
        worldRect.position.y = std::roundf(worldRect.position.y);
            
        return std::move(worldRect);
    }

    void UIControl::updateInternal(UIContext& context, const sUIRect& parentRect)
    {
        if (!isEnabled() || isClickThrough()) return;
        sUIRect worldRect = getWorldRect(parentRect);

        // Do children first, inverted
        auto itend = m_children.rend();
        for (auto it = m_children.rbegin(); it != itend; ++it)
        {
            auto pChild = *it;
            pChild->updateInternal(context, worldRect);
        }

        if (!context.m_pHoverControl)
        {
            if (context.m_mouseEvent.mousePos.x >= worldRect.position.x &&
                context.m_mouseEvent.mousePos.y >= worldRect.position.y &&
                context.m_mouseEvent.mousePos.x <= worldRect.position.x + worldRect.size.x &&
                context.m_mouseEvent.mousePos.y <= worldRect.position.y + worldRect.size.y)
            {
                context.m_mouseEvent.localMousePos.x = context.m_mouseEvent.mousePos.x - worldRect.position.x;
                context.m_mouseEvent.localMousePos.y = context.m_mouseEvent.mousePos.y - worldRect.position.y;
                context.m_pHoverControl = this;
            }
        }
    }

    void UIControl::renderInternal(const UIContext& context, const sUIRect& parentRect) const
    {
        if (!isVisible()) return;

        sUIRect worldRect = getWorldRect(parentRect);
        renderControl(context, worldRect);
        for (auto pChild : m_children)
        {
            pChild->renderInternal(context, worldRect);
        }
    }

    void UIControl::setRect(const sUIRect& rect)
    {
        m_rect = rect;
    }

    void UIControl::setWorldRect(const sUIRect& rect, const UIContext& context)
    {
        if (getParent())
        {
            auto parentRect = getParent()->getWorldRect(context);

            sUIRect localRect = rect;

            // Undo anchoring
            switch (m_anchorType[0])
            {
                case eUIAnchorType::ANCHOR_PIXEL:
                    localRect.position.x += m_anchor.x;
                    break;
                case eUIAnchorType::ANCHOR_PERCENTAGE:
                    localRect.position.x += localRect.size.x * m_anchor.x;
                    break;
            }
            switch (m_anchorType[1])
            {
                case eUIAnchorType::ANCHOR_PIXEL:
                    localRect.position.y += m_anchor.y;
                    break;
                case eUIAnchorType::ANCHOR_PERCENTAGE:
                    localRect.position.y += localRect.size.y * m_anchor.y;
                    break;
            }

            // Undo alignement
            switch (m_align)
            {
                case eUIAlign::TOP_LEFT:
                    localRect.position.x = localRect.position.x - parentRect.position.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y;
                    break;
                case eUIAlign::TOP:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x * .5f;
                    localRect.position.y = localRect.position.y - parentRect.position.y;
                    break;
                case eUIAlign::TOP_RIGHT:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y;
                    break;
                case eUIAlign::LEFT:
                    localRect.position.x = localRect.position.x - parentRect.position.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y * .5f;
                    break;
                case eUIAlign::CENTER:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x * .5f;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y * .5f;
                    break;
                case eUIAlign::RIGHT:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y * .5f;
                    break;
                case eUIAlign::BOTTOM_LEFT:
                    localRect.position.x = localRect.position.x - parentRect.position.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y;
                    break;
                case eUIAlign::BOTTOM:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x * .5f;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y;
                    break;
                case eUIAlign::BOTTOM_RIGHT:
                    localRect.position.x = localRect.position.x - parentRect.position.x - parentRect.size.x;
                    localRect.position.y = localRect.position.y - parentRect.position.y - parentRect.size.y;
                    break;
            }

            // Undo position and width values
            switch (m_posType[0])
            {
                case eUIPosType::POS_RELATIVE:
                    localRect.position.x = localRect.position.x;
                    break;
                case eUIPosType::POS_PERCENTAGE:
                    localRect.position.x = localRect.position.x / parentRect.size.x;
                    break;
            }
            switch (m_posType[1])
            {
                case eUIPosType::POS_RELATIVE:
                    localRect.position.y = localRect.position.y;
                    break;
                case eUIPosType::POS_PERCENTAGE:
                    localRect.position.y = localRect.position.y / parentRect.size.y;
                    break;
            }
            switch (m_dimType[0])
            {
                case eUIDimType::DIM_ABSOLUTE:
                    localRect.size.x = localRect.size.x;
                    break;
                case eUIDimType::DIM_RELATIVE:
                    localRect.size.x = localRect.size.x - parentRect.size.x;
                    break;
                case eUIDimType::DIM_PERCENTAGE:
                    localRect.size.x = localRect.size.x / parentRect.size.x;
                    break;
            }
            switch (m_dimType[1])
            {
                case eUIDimType::DIM_ABSOLUTE:
                    localRect.size.y = localRect.size.y;
                    break;
                case eUIDimType::DIM_RELATIVE:
                    localRect.size.y = localRect.size.y - parentRect.size.y;
                    break;
                case eUIDimType::DIM_PERCENTAGE:
                    localRect.size.y = localRect.size.y / parentRect.size.y;
                    break;
            }

            m_rect = std::move(localRect);
        }
        else
        {
            m_rect = rect;
        }
    }

    sUIVector2 UIControl::getAnchorInPixel() const
    {
        sUIVector2 ret = m_anchor;
        if (m_anchorType[0] == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
        {
            ret.x = ret.x * m_rect.size.x;
        }
        if (m_anchorType[1] == onut::eUIAnchorType::ANCHOR_PERCENTAGE)
        {
            ret.y = ret.y * m_rect.size.y;
        }
        return std::move(ret);
    }

    sUIVector2 UIControl::getAnchorInPercentage() const
    {
        sUIVector2 ret = m_anchor;
        if (m_anchorType[0] == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            ret.x = ret.x / m_rect.size.x;
        }
        if (m_anchorType[1] == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            ret.y = ret.y / m_rect.size.y;
        }
        return std::move(ret);
    }

    void UIControl::setAnchor(const sUIVector2& anchor)
    {
        m_anchor = anchor;
    }

    void UIControl::setAnchorPercent(const sUIVector2& anchor)
    {
        if (m_anchorType[0] == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            m_anchor.x = m_rect.size.x * anchor.x;
        }
        else
        {
            m_anchor.x = anchor.x;
        }
        if (m_anchorType[1] == onut::eUIAnchorType::ANCHOR_PIXEL)
        {
            m_anchor.y = m_rect.size.y * anchor.y;
        }
        else
        {
            m_anchor.y = anchor.y;
        }
    }

    void UIControl::setName(const std::string& name)
    {
        m_name = name;
    }

    void UIControl::setIsEnabled(bool bIsEnabled)
    {
        m_isEnabled = bIsEnabled;
    }

    void UIControl::setIsVisible(bool bIsVisible)
    {
        m_isVisible = bIsVisible;
    }

    void UIControl::setIsClickThrough(bool bIsClickThrough)
    {
        m_isClickThrough = bIsClickThrough;
    }

    eUIState UIControl::getState(const UIContext& context) const
    {
        if (isEnabled())
        {
            if (context.m_pHoverControl == this)
            {
                if (context.m_pDownControl == this)
                {
                    return eUIState::DOWN;
                }
                else
                {
                    return eUIState::HOVER;
                }
            }
            else if (context.m_pDownControl == this)
            {
                return eUIState::HOVER;
            }
            else
            {
                return eUIState::NORMAL;
            }
        }
        else
        {
            return eUIState::DISABLED;
        }
    }

    bool UIControl::hasFocus(const UIContext& context) const
    {
        return (context.m_pFocus == this);
    }

    void UIControl::setUserData(void* pUserData)
    {
        m_pUserData = pUserData;
    }

    void UIControl::setAlign(eUIAlign align)
    {
        m_align = align;
    }

    void UIControl::setWidthType(eUIDimType widthType)
    {
        m_dimType[0] = widthType;
    }

    void UIControl::setHeightType(eUIDimType heightType)
    {
        m_dimType[1] = heightType;
    }

    void UIControl::setXType(eUIPosType xType)
    {
        m_posType[0] = xType;
    }

    void UIControl::setYType(eUIPosType yType)
    {
        m_posType[1] = yType;
    }

    void UIControl::setXAnchorType(eUIAnchorType xAnchorType)
    {
        m_anchorType[0] = xAnchorType;
    }

    void UIControl::setYAnchorType(eUIAnchorType yAnchorType)
    {
        m_anchorType[1] = yAnchorType;
    }

    const UIProperty& UIControl::getProperty(const std::string& name) const
    {
        auto& it = m_properties.find(name);
        return it->second;
    }

    void UITreeView::addItem(UITreeViewItem* pItem)
    {
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        pItem->m_pTreeView = this;
        m_items.push_back(pItem);
    }

    void UITreeView::removeItem(UITreeViewItem* pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                delete pItem;
                return;
            }
        }
    }

    UITreeView::~UITreeView()
    {
        for (auto pItem : m_items)
        {
            delete pItem;
        }
    }

    void UITreeView::clear()
    {
        for (auto pItem : m_items)
        {
            delete pItem;
        }
        m_items.clear();
    }

    UITreeViewItem* UITreeView::getItemAtPosition(const sUIVector2& pos, const sUIRect& rect, bool* pPickedExpandButton) const
    {
        // Render it's items
        sUIRect itemRect = {rect.position, {rect.size.x, m_itemHeight}};
        for (auto pItem : m_items)
        {
            auto pRet = getItemAtPosition(pItem, pos, itemRect, pPickedExpandButton);
            if (pRet)
            {
                return pRet;
            }
        }
        return nullptr;
    }

    UITreeViewItem* UITreeView::getItemAtPosition(UITreeViewItem* pItem, const sUIVector2& pos, sUIRect& rect, bool* pPickedExpandButton) const
    {
        if (pos.y >= rect.position.y &&
            pos.y <= rect.position.y + rect.size.y)
        {
            if (pos.x >= rect.position.x + getExpandClickWidth() ||
                pos.x <= rect.position.x)
            {
                return pItem;
            }
            else if (pPickedExpandButton)
            {
                *pPickedExpandButton = true;
                return pItem;
            }
        }
        rect.position.y += getItemHeight();
        if (pItem->m_isExpanded)
        {
            if (!pItem->m_items.empty())
            {
                auto xOffset = getExpandedXOffset();
                rect.position.x += xOffset;
                rect.size.x -= xOffset;
                for (auto pHisItem : pItem->m_items)
                {
                    auto pRet = getItemAtPosition(pHisItem, pos, rect, pPickedExpandButton);
                    if (pRet)
                    {
                        return pRet;
                    }
                }
                rect.size.x += xOffset;
                rect.position.x -= xOffset;
            }
        }
        return nullptr;
    }

    void UITreeView::unselectAll()
    {
        m_selectedItems.clear();
        for (auto pItem : m_items)
        {
            unselectItem(pItem);
        }
    }

    void UITreeView::unselectItem(UITreeViewItem* pItem)
    {
        pItem->m_isSelected = false;
        for (auto pHisItem : pItem->m_items)
        {
            unselectItem(pHisItem);
        }
    }

    void UITreeView::addSelectedItem(UITreeViewItem* pItem)
    {
        m_selectedItems.push_back(pItem);
        pItem->m_isSelected = true;
        expandTo(pItem);
    }

    void UITreeView::expandTo(UITreeViewItem* pItem)
    {
        if (pItem->m_pParent)
        {
            pItem->m_pParent->m_isExpanded = true;
            expandTo(pItem->m_pParent);
        }
    }

    void UITreeViewItem::addItem(UITreeViewItem* pItem)
    {
        for (auto pMyItem : m_items)
        {
            if (pMyItem == pItem) return;
        }
        pItem->m_pParent = this;
        pItem->m_pTreeView = m_pTreeView;
        m_items.push_back(pItem);
    }

    void UITreeViewItem::removeItem(UITreeViewItem* pItem)
    {
        auto size = m_items.size();
        for (decltype(size) i = 0; i < size; ++i)
        {
            if (m_items[i] == pItem)
            {
                m_items.erase(m_items.begin() + i);
                delete pItem;
                return;
            }
        }
    }

    bool UITextBox::isCursorVisible() const
    {
        auto now = std::chrono::steady_clock::now();
        auto diff = now - m_cursorTime;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        return (ms.count() % 1000) < 500;
    }

    void UITextBox::numerifyText()
    {
        if (m_isNumerical)
        {
            std::stringstream ss(m_text);
            float value;
            if (!(ss >> value))
            {
                value = 0.f;
            }
            std::stringstream ssOut;
            ssOut << std::fixed << std::setprecision(static_cast<std::streamsize>(m_decimalPrecision)) << value;
            m_text = ssOut.str();
        }
    }

    float UITextBox::getFloat() const
    {
        if (m_isNumerical)
        {
            std::stringstream ss(m_text);
            float value;
            if (!(ss >> value))
            {
                value = 0.f;
            }
            return value;
        }
        return 0.f;
    }

    int UITextBox::getInt() const
    {
        if (m_isNumerical)
        {
            std::stringstream ss(m_text);
            int value;
            if (!(ss >> value))
            {
                value = 0;
            }
            return value;
        }
        return 0;
    }

    //--- Copy
    UIButton::UIButton(const UIButton& other) :
        UIControl(other)
    {
        m_caption = other.m_caption;
    }

    UIPanel::UIPanel(const UIPanel& other) :
        UIControl(other)
    {
    }

    UILabel::UILabel(const UILabel& other) :
        UIControl(other)
    {
        m_text = other.m_text;
    }

    UIImage::UIImage(const UIImage& other) :
        UIControl(other)
    {
        m_image = other.m_image;
    }

    UICheckBox::UICheckBox(const UICheckBox& other) :
        UIControl(other)
    {
        m_caption = other.m_caption;
        m_isChecked = other.m_isChecked;
        m_behavior = other.m_behavior;
    }

    UITreeView::UITreeView(const UITreeView& other) :
        UIControl(other)
    {
        for (auto pOtherItem : other.m_items)
        {
            m_items.push_back(new UITreeViewItem(*pOtherItem));
        }
    }

    UITreeViewItem::UITreeViewItem(const std::string& text) :
        m_text(text)
    {
    }

    UITreeViewItem::UITreeViewItem(const UITreeViewItem& other)
    {
        for (auto pOtherItem : other.m_items)
        {
            m_items.push_back(new UITreeViewItem(*pOtherItem));
        }
        m_isExpanded = other.m_isExpanded;
        m_text = other.m_text;
    }

    UITextBox::UITextBox(const UITextBox& other) :
        UIControl(other)
    {
        m_text = other.m_text;
        m_isNumerical = other.m_isNumerical;
        m_decimalPrecision = other.m_decimalPrecision;
    }

    //--- Properties
    void UIButton::setCaption(const std::string& caption)
    {
        m_caption = caption;
    }

    void UIPanel::setColor(const sUIColor& color)
    {
        m_color = color;
    }

    void UILabel::setText(const std::string& text)
    {
        m_text = text;
    }

    void UIImage::setImage(const std::string& image)
    {
        m_image = image;
    }

    void UICheckBox::setIsChecked(bool in_isChecked)
    {
        switch (m_behavior)
        {
            case eUICheckBehavior::NORMAL:
            {
                m_isChecked = in_isChecked;
                break;
            }
            case eUICheckBehavior::OPTIONAL:
            {
                if (!m_isChecked && in_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
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
            case eUICheckBehavior::EXCLUSIVE:
            {
                if (!m_isChecked && in_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
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

    void UICheckBox::setBehavior(eUICheckBehavior behavior)
    {
        m_behavior = behavior;
    }

    void UITreeView::setExpandedXOffset(float expandedXOffset)
    {
        m_expandedXOffset = expandedXOffset;
    }

    void UITreeView::setExpandClickWidth(float expandClickWidth)
    {
        m_expandClickWidth = expandClickWidth;
    }

    void UITreeView::setItemHeight(float itemHeight)
    {
        m_itemHeight = itemHeight;
    }

    void UITreeViewItem::setIsExpanded(bool isExpanded)
    {
        m_isExpanded = isExpanded;
    }

    void UITreeViewItem::setText(const std::string& text)
    {
        m_text = text;
    }

    void UITreeViewItem::setUserData(void* pUserData)
    {
        m_pUserData = pUserData;
    }

    void UITextBox::setText(const std::string& text)
    {
        m_text = text;
        numerifyText();
    }

    void UITextBox::setIsNumerical(bool isNumerical)
    {
        m_isNumerical = isNumerical;
        numerifyText();
    }

    void UITextBox::setIsDecimalPrecision(int decimalPrecision)
    {
        m_decimalPrecision = decimalPrecision;
        numerifyText();
    }

    //--- Loads / Saves
    void UIButton::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_caption = getJsonString(jsonNode["caption"]);
    }

    void UIButton::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonString(jsonNode, "caption", m_caption.c_str(), allocator);
    }

    void UIPanel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_color = getJsonColor(jsonNode["color"]);
    }

    void UIPanel::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonColor(jsonNode, "color", m_color, allocator);
    }

    void UILabel::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_text = getJsonString(jsonNode["text"]);
    }

    void UILabel::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonString(jsonNode, "text", m_text.c_str(), allocator);
    }

    void UIImage::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_image = getJsonString(jsonNode["image"]);
    }

    void UIImage::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonString(jsonNode, "image", m_image.c_str(), allocator);
    }

    void UICheckBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_caption = getJsonString(jsonNode["caption"]);
        m_isChecked = getJsonBool(jsonNode["checked"], false);
        m_behavior = getJsonCheckBehavior(getJsonString(jsonNode["behavior"]));
    }

    void UICheckBox::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonString(jsonNode, "caption", m_caption.c_str(), allocator);
        setJsonBool(jsonNode, "checked", m_isChecked, allocator, false);
        setJsonString(jsonNode, "behavior", getStringFromCheckBehavior(m_behavior), allocator);
    }

    void UITreeView::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_expandedXOffset = getJsonFloat(jsonNode["expandedXOffset"], 18.f);
        m_expandClickWidth = getJsonFloat(jsonNode["expandClickWidth"], 18.f);
        m_itemHeight = getJsonFloat(jsonNode["itemHeight"], 18.f);
    }

    void UITreeView::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonFloat(jsonNode, "expandedXOffset", m_expandedXOffset, allocator, 18.f);
        setJsonFloat(jsonNode, "expandClickWidth", m_expandClickWidth, allocator, 18.f);
        setJsonFloat(jsonNode, "itemHeight", m_itemHeight, allocator, 18.f);
    }

    void UITextBox::load(const rapidjson::Value& jsonNode)
    {
        UIControl::load(jsonNode);
        m_text = getJsonString(jsonNode["text"]);
        m_isNumerical = getJsonBool(jsonNode["numerical"]);
        m_decimalPrecision = getJsonInt(jsonNode["precision"]);
        numerifyText();
    }

    void UITextBox::save(rapidjson::Value& jsonNode, rapidjson::Allocator& allocator) const
    {
        UIControl::save(jsonNode, allocator);
        setJsonString(jsonNode, "text", m_text.c_str(), allocator);
        setJsonBool(jsonNode, "numerical", m_isNumerical, allocator);
        setJsonInt(jsonNode, "precision", m_decimalPrecision, allocator);
    }

    //--- Renders
    void UIButton::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UIButton>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UIPanel::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UIPanel>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UILabel::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UILabel>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UIImage::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UIImage>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UICheckBox::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UICheckBox>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    void UITreeView::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UITreeView>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }

        // Render it's items
        sUIRect itemRect = {rect.position, {rect.size.x, m_itemHeight}};
        const auto& itemCallback = context.getStyle<UITreeViewItem>(getStyle());
        if (itemCallback)
        {
            for (auto pItem : m_items)
            {
                pItem->render(itemCallback, this, rect, itemRect);
            }
        }
    }

    void UITextBox::renderControl(const UIContext& context, const sUIRect& rect) const
    {
        const auto& callback = context.getStyle<UITextBox>(getStyle());
        if (callback)
        {
            callback->render(this, rect);
        }
    }

    //--- Internal events
    void UICheckBox::onClickInternal(const UIMouseEvent& evt)
    {
        switch (m_behavior)
        {
            case eUICheckBehavior::NORMAL:
            {
                m_isChecked = !m_isChecked;
                if (onCheckChanged)
                {
                    UICheckEvent checkEvt;
                    checkEvt.isChecked = m_isChecked;
                    checkEvt.pContext = evt.pContext;
                    onCheckChanged(this, checkEvt);
                }
                break;
            }
            case eUICheckBehavior::OPTIONAL:
            {
                if (!m_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                        if (pSibblingCheckBox->onCheckChanged)
                                        {
                                            UICheckEvent checkEvt;
                                            checkEvt.isChecked = pSibblingCheckBox->m_isChecked;
                                            checkEvt.pContext = evt.pContext;
                                            pSibblingCheckBox->onCheckChanged(this, checkEvt);
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
                    onCheckChanged(this, checkEvt);
                }
                break;
            }
            case eUICheckBehavior::EXCLUSIVE:
            {
                if (!m_isChecked)
                {
                    if (getParent())
                    {
                        auto& sibblings = getParent()->getChildren();
                        for (auto pSibbling : sibblings)
                        {
                            if (pSibbling == this) continue;
                            if (pSibbling->getType() == eUIType::UI_CHECKBOX)
                            {
                                auto pSibblingCheckBox = dynamic_cast<UICheckBox*>(pSibbling);
                                if (pSibblingCheckBox->getBehavior() == m_behavior)
                                {
                                    if (pSibblingCheckBox->m_isChecked)
                                    {
                                        pSibblingCheckBox->m_isChecked = false;
                                        if (pSibblingCheckBox->onCheckChanged)
                                        {
                                            UICheckEvent checkEvt;
                                            checkEvt.isChecked = pSibblingCheckBox->m_isChecked;
                                            checkEvt.pContext = evt.pContext;
                                            pSibblingCheckBox->onCheckChanged(this, checkEvt);
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
                        onCheckChanged(this, checkEvt);
                    }
                }
                break;
            }
        }
    }

    void UITreeView::onMouseDownInternal(const UIMouseEvent& evt)
    {
        auto worldRect = getWorldRect(*evt.pContext);
        bool pickedExpandButton = false;
        auto pPicked = getItemAtPosition(evt.mousePos, worldRect, &pickedExpandButton);
        if (pPicked)
        {
            if (pickedExpandButton)
            {
                pPicked->m_isExpanded = !pPicked->m_isExpanded;
            }
            else
            {
                unselectAll();
                addSelectedItem(pPicked);
                if (onSelectionChanged)
                {
                    UITreeViewSelectEvent myEvt;
                    myEvt.pContext = evt.pContext;
                    myEvt.pSelectedItems = &m_selectedItems;
                    onSelectionChanged(this, myEvt);
                }
            }
        }
    }

    void UITextBox::onGainFocusInternal(const UIFocusEvent& evt)
    {
        // Select all on focus
        selectAll();
        m_cursorTime = std::chrono::steady_clock::now();
        m_isTextChanged = false;
    }

    void UITextBox::onLoseFocusInternal(const UIFocusEvent& evt)
    {
        // Deselect all
        m_selectedTextRegion[0] = 0;
        m_selectedTextRegion[1] = 0;

        // Accept
        if (m_isTextChanged)
        {
            m_isTextChanged = false;
            numerifyText();
            if (onTextChanged)
            {
                UITextBoxEvent evt2;
                evt2.pContext = evt.pContext;
                onTextChanged(this, evt2);
            }
        }
    }

    void UITextBox::onMouseDownInternal(const UIMouseEvent& evt)
    {
        m_isSelecting = true;
        const auto& callback = evt.pContext->getTextCaretSolver<UITextBox>(getStyle());
        if (callback)
        {
            auto caretPos = callback->getCaretPos(this, evt.localMousePos);
            m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = caretPos;
        }
    }

    void UITextBox::onMouseMoveInternal(const UIMouseEvent& evt)
    {
        if (!m_isSelecting) return; // Don't care
        const auto& callback = evt.pContext->getTextCaretSolver<UITextBox>(getStyle());
        if (callback)
        {
            auto caretPos = callback->getCaretPos(this, evt.localMousePos);
            m_cursorPos = caretPos;
            m_selectedTextRegion[0] = std::min<>(m_cursorPos, m_selectedTextRegion[0]);
            m_selectedTextRegion[1] = std::max<>(m_cursorPos, m_selectedTextRegion[1]);
        }
    }

    void UITextBox::onMouseUpInternal(const UIMouseEvent& evt)
    {
        m_isSelecting = false;
    }

    void UITextBox::selectAll()
    {
        m_selectedTextRegion[0] = 0;
        m_selectedTextRegion[1] = m_text.size();
        m_cursorPos = m_selectedTextRegion[1];
    }

    void UITextBox::onWriteInternal(char c, UIContext& context)
    {
        m_cursorTime = std::chrono::steady_clock::now();
        m_isTextChanged = true;
        if (c == '\b') // Backspace
        {
            if (m_selectedTextRegion[1] - m_selectedTextRegion[0])
            {
                m_text = m_text.substr(0, m_selectedTextRegion[0]) + m_text.substr(m_selectedTextRegion[1]);
                m_cursorPos = m_selectedTextRegion[1] = m_selectedTextRegion[0];
            }
            else if (m_cursorPos)
            {
                m_text = m_text.substr(0, m_cursorPos - 1) + m_text.substr(m_cursorPos);
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos - 1;
            }
        }
        else if (c == '\r') // Return
        {
            //selectAll();
            context.focus(nullptr);
        }
        else if (c == '\x1b') // Escape
        {
            context.focus(nullptr);
        }
        else if (c == '\t')
        {
            // We go to the next textfield. Ignored feature for now
            context.focus(nullptr);
        }
        else // Normal character
        {
            std::stringstream ss;
            ss << m_text.substr(0, m_selectedTextRegion[0]);
            ss << c;
            ss << m_text.substr(m_selectedTextRegion[1]);
            m_text = ss.str();
            m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_selectedTextRegion[0] + 1;
        }
    }

    void UITextBox::onKeyDownInternal(const UIKeyEvent& evt)
    {
        m_cursorTime = std::chrono::steady_clock::now();
        switch (evt.key)
        {
            case KEY_END:
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_text.size();
                break;
            case KEY_HOME:
                m_cursorPos = m_selectedTextRegion[0] = m_selectedTextRegion[1] = 0;
                break;
            case KEY_LEFT:
            case KEY_UP:
                if (m_cursorPos > 0)
                {
                    --m_cursorPos;
                    m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos;
                }
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if (m_cursorPos < m_text.size())
                {
                    ++m_cursorPos;
                    m_selectedTextRegion[0] = m_selectedTextRegion[1] = m_cursorPos;
                }
                break;
            case KEY_DELETE:
                m_isTextChanged = true;
                if (m_selectedTextRegion[0] != m_selectedTextRegion[1])
                {
                    m_text = m_text.substr(0, m_selectedTextRegion[0]) + m_text.substr(m_selectedTextRegion[1]);
                    m_cursorPos = m_selectedTextRegion[1] = m_selectedTextRegion[0];
                }
                else if (m_cursorPos < m_text.size())
                {
                    m_text = m_text.substr(0, m_cursorPos) + m_text.substr(m_cursorPos + 1);
                }
                break;
        }
    }
};

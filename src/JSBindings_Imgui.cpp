// Private includes
#include "JSBindings_Macros.h"

// Third party
#include <imgui/imgui.h>

namespace onut
{
    namespace js
    {
        bool items_getter(void* data, int idx, const char** out_text)
        {
            std::vector<std::string>* pItems = (std::vector<std::string>*)data;
            if (idx < 0 || idx >= (int)pItems->size()) return false;
            *out_text = pItems->at(idx).c_str();
            return true;
        }

        void createImguiBindings()
        {
            static bool valueChanged = false;
            static ImGuiID activeId = 0;
            auto ctx = pContext;

            JS_INTERFACE_BEGIN();
            {
                // Menu
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::BeginMainMenuBar() ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("beginMainMenuBar", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::EndMainMenuBar();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("endMainMenuBar", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::BeginMenuBar() ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("beginMenuBar", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::EndMenuBar();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("endMenuBar", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto enabled = JS_BOOL(1, true);
                    duk_push_boolean(ctx, ImGui::BeginMenu(label, enabled) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("beginMenu", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::EndMenu();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("endMenu", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto shortcut = JS_STRING(1, nullptr);
                    auto selected = JS_BOOL(2, false);
                    auto enabled = JS_BOOL(3, true);
                    duk_push_boolean(ctx, ImGui::MenuItem(label, shortcut, selected, enabled) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("menuItem", 4);

                // Window
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto name = JS_STRING(0);
                    duk_push_boolean(ctx, ImGui::Begin(name) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("begin", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::End();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("end", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pos = JS_VECTOR2(0);
                    ImGui::SetNextWindowPos({ pos.x, pos.y }, ImGuiCond_FirstUseEver);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setNextWindowPos", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto size = JS_VECTOR2(0);
                    ImGui::SetNextWindowSize({ size.x, size.y }, ImGuiCond_FirstUseEver);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setNextWindowSize", 1);
//     
          /*      JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto name = JS_STRING(0);
                    auto size = JS_VECTOR2(1);
                    auto border = JS_BOOL(2, false);
                    duk_push_boolean(ctx, ImGui::BeginChild(name, { size.x, size.y }, border) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("beginChild", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::EndChild();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("endChild", 0);*/

                // Cursor / Layout
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::Separator();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("separator", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pos_x = JS_FLOAT(0);
                    auto spacing_w = JS_FLOAT(1, -1.0f);
                    ImGui::SameLine(pos_x, spacing_w);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("sameLine", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::NewLine();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("newLine", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::Spacing();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("spacing", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto size = JS_VECTOR2(0);
                    ImGui::Dummy({ size.x, size.y });
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("dummy", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto indent_w = JS_FLOAT(0);
                    ImGui::Indent(indent_w);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("indent", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto indent_w = JS_FLOAT(0);
                    ImGui::Unindent(indent_w);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("unindent", 1);

                // Text
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    ImGui::Text(text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("text", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto col = JS_COLOR(0);
                    auto text = JS_STRING(1);
                    ImGui::TextColored({col.r, col.g, col.b, col.a}, text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("textColored", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    ImGui::TextDisabled(text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("textDisabled", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    ImGui::TextWrapped(text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("textWrapped", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto text = JS_STRING(1);
                    ImGui::LabelText(label, text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("labelText", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    ImGui::BulletText(text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("bulletText", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::Bullet();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("bullet", 0);

                // Main
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    auto size = JS_VECTOR2(1);
                    duk_push_boolean(ctx, ImGui::Button(text,{ size.x, size.y }) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("button", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    duk_push_boolean(ctx, ImGui::SmallButton(text) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("smallButton", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    auto size = JS_VECTOR2(1);
                    duk_push_boolean(ctx, ImGui::InvisibleButton(text,{ size.x, size.y }) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("invisibleButton", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                    auto pp = (std::shared_ptr<OTexture>*)(duk_to_pointer(ctx, -1));
                    duk_pop(ctx);

                    if (pp)
                    {
                        auto size = JS_VECTOR2(1);
                        auto uv0 = JS_VECTOR2(2);
                        auto uv1 = JS_VECTOR2(3, Vector2::One);
                        auto tint_col = JS_COLOR(4);
                        auto border_col = JS_COLOR(5, Color::Transparent);
                        ImGui::Image(pp,
                                    { size.x, size.y },
                                    { uv0.x, uv0.y },
                                    { uv1.x, uv1.y },
                                    { tint_col.r, tint_col.g, tint_col.b, tint_col.a },
                                    { border_col.r, border_col.g, border_col.b, border_col.a });
                    }

                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("image", 6);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                    auto pp = (std::shared_ptr<OTexture>*)(duk_to_pointer(ctx, -1));
                    duk_pop(ctx);

                    if (pp)
                    {
                        auto size = JS_VECTOR2(1);
                        auto uv0 = JS_VECTOR2(2);
                        auto uv1 = JS_VECTOR2(3, Vector2::One);
                        auto frame_padding = JS_INT(4, -1);
                        auto bg_col = JS_COLOR(5, Color::Transparent);
                        auto tint_col = JS_COLOR(6);
                        auto ret = ImGui::ImageButton(pp,
                                    { size.x, size.y },
                                    { uv0.x, uv0.y },
                                    { uv1.x, uv1.y },
                                    frame_padding,
                                    { bg_col.r, bg_col.g, bg_col.b, bg_col.a },
                                    { tint_col.r, tint_col.g, tint_col.b, tint_col.a });

                        duk_push_boolean(ctx, ret ? 1 : 0);
                        return 1;
                    }

                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("imageButton", 6);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    auto checked = JS_BOOL(1);
                    valueChanged = ImGui::Checkbox(text, &checked);
                    duk_push_boolean(ctx, checked ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("checkbox", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    auto v = JS_INT(1);
                    auto v_button = JS_INT(2);
                    valueChanged = ImGui::RadioButton(text, &v, v_button);
                    duk_push_int(ctx, v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("radioButton", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    auto v = JS_INT(1);
                    if (duk_is_array(ctx, 2))
                    {
                        std::vector<std::string> items;
                        auto len = duk_get_length(ctx, 2);
                        for (decltype(len) i = 0; i < len; ++i)
                        {
                            duk_get_prop_index(ctx, 2, i);
                            items.push_back(duk_to_string(ctx, -1));
                            duk_pop(ctx);
                        }
                        valueChanged = ImGui::Combo(text, &v, items_getter, &items, items.size());
                        duk_push_int(ctx, v);
                        return 1;
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("combo", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto fraction = JS_FLOAT(0);
                    auto size = JS_VECTOR2(1, Vector2(-1, 0));
                    auto overlay = JS_STRING(2, nullptr);
                    ImGui::ProgressBar(fraction, { size.x, size.y }, overlay);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("progressBar", 3);

                // Drags
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto v = JS_FLOAT(1);
                    auto v_speed = JS_FLOAT(2, 1.0f);
                    auto v_min = JS_FLOAT(3);
                    auto v_max = JS_FLOAT(4);
                    valueChanged = ImGui::DragFloat(label, &v, v_speed, v_min, v_max);
                    duk_push_number(ctx, (duk_double_t)v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("dragNumber", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto v = JS_VECTOR2(1);
                    auto v_speed = JS_FLOAT(2, 1.0f);
                    auto v_min = JS_FLOAT(3);
                    auto v_max = JS_FLOAT(4);
                    valueChanged = ImGui::DragFloat2(label, &v.x, v_speed, v_min, v_max);
                    newVector2(ctx, v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("dragVector2", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto v = JS_VECTOR3(1);
                    auto v_speed = JS_FLOAT(2, 1.0f);
                    auto v_min = JS_FLOAT(3);
                    auto v_max = JS_FLOAT(4);
                    valueChanged = ImGui::DragFloat3(label, &v.x, v_speed, v_min, v_max);
                    newVector3(ctx, v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("dragVector3", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto v = JS_VECTOR4(1);
                    auto v_speed = JS_FLOAT(2, 1.0f);
                    auto v_min = JS_FLOAT(3);
                    auto v_max = JS_FLOAT(4);
                    valueChanged = ImGui::DragFloat4(label, &v.x, v_speed, v_min, v_max);
                    newVector4(ctx, v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("dragVector4", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto v = JS_INT(1);
                    auto v_speed = JS_FLOAT(2, 1.0f);
                    auto v_min = JS_INT(3);
                    auto v_max = JS_INT(4);
                    valueChanged = ImGui::DragInt(label, &v, v_speed, v_min, v_max);
                    duk_push_int(ctx, v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("dragInt", 5);

                // Input with Keyboard
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto buf = JS_STRING(1);
                    auto buf_size = JS_INT(2, 144);
                    static std::vector<char> textBuf;
                    textBuf.resize(std::max(buf_size + 1, (int)textBuf.capacity()));
                    memcpy(textBuf.data(), buf, strlen(buf) + 1);
                    valueChanged = ImGui::InputText(label, textBuf.data(), buf_size);
                    duk_push_string(ctx, textBuf.data());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("inputText", 3);

                // Sliders
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto v = JS_FLOAT(1);
                    auto v_min = JS_FLOAT(2);
                    auto v_max = JS_FLOAT(3);
                    valueChanged = ImGui::SliderFloat(label, &v, v_min, v_max);
                    duk_push_number(ctx, (duk_double_t)v);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("sliderNumber", 4);

                // Color Picker
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto col = JS_COLOR(1);
                    valueChanged = ImGui::ColorEdit3(label, &col.r);
                    newColor(ctx, col);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("colorPickerRGB", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    auto col = JS_COLOR(1);
                    valueChanged = ImGui::ColorEdit4(label, &col.r, ImGuiColorEditFlags_AlphaPreviewHalf);
                    newColor(ctx, col);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("colorPickerRGBA", 2);

                // Tooltips
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto text = JS_STRING(0);
                    ImGui::SetTooltip(text);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setTooltip", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::BeginTooltip();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("beginTooltip", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::EndTooltip();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("endTooltip", 0);
                
                // Columns
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto count = JS_INT(0, 1);
                    auto id = JS_STRING(1, nullptr);
                    auto border = JS_BOOL(2, true);
                    ImGui::Columns(count, id, border);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("columns", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    ImGui::NextColumn();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("nextColumn", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto column_index = JS_INT(0);
                    auto width = JS_FLOAT(1);
                    ImGui::SetColumnWidth(column_index, width);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setColumnWidth", 3);

                // Utilities
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::IsItemHovered() ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isItemHovered", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::IsItemClicked() ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isItemClicked", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto label = JS_STRING(0);
                    duk_push_boolean(ctx, ImGui::CollapsingHeader(label) ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("collapsingHeader", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, valueChanged ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("valueChanged", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::ValueFinished() ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("valueFinished", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::GetIO().WantCaptureMouse ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("wantCaptureMouse", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::GetIO().WantCaptureKeyboard ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("wantCaptureKeyboard", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, ImGui::GetIO().WantTextInput ? 1 : 0);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("wantTextInput", 0);
            }
            JS_INTERFACE_END("GUI");
        }
    }
}

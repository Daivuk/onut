#pragma once
// Onut
#include <onut/Maths.h>

// STL
#include <cinttypes>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(UIContext);
OForwardDeclare(UITreeViewItem);

namespace onut
{
    using UITreeViewItems = std::vector<OUITreeViewItemRef>;

    class UIMouseEvent
    {
    public:
        Vector2 mousePos;
        Vector2 localMousePos;
        bool isMouseDown = false;
        OUIContextRef pContext;
        int button = 1;
        bool isCtrlDown = false;
        float scroll = 0.f;
    };

    static const uintptr_t KEY_END = 0x23;
    static const uintptr_t KEY_HOME = 0x24;
    static const uintptr_t KEY_LEFT = 0x25;
    static const uintptr_t KEY_UP = 0x26;
    static const uintptr_t KEY_RIGHT = 0x27;
    static const uintptr_t KEY_DOWN = 0x28;
    static const uintptr_t KEY_DELETE = 0x2E;

    class UIKeyEvent
    {
    public:
        uintptr_t key = 0;
        OUIContextRef pContext = nullptr;
    };

    class UICheckEvent
    {
    public:
        bool isChecked = false;
        OUIContextRef pContext = nullptr;
    };

    class UIFocusEvent
    {
    public:
        OUIContextRef pContext = nullptr;
    };

    class UITreeViewSelectEvent
    {
    public:
        UITreeViewItems selectedItems;
        OUIContextRef pContext = nullptr;
    };

    class UITreeViewMoveEvent
    {
    public:
        UITreeViewItems selectedItems;
        OUITreeViewItemRef pTarget = nullptr;
        OUIContextRef pContext = nullptr;
    };

    class UITextBoxEvent
    {
    public:
        OUIContextRef pContext = nullptr;
    };
};

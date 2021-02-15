#pragma once

#include <vector>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/ForwardDeclaration.h>

OForwardDeclare(PrimitiveBatch);
ForwardDeclare(Entity);

class GUIContext;
enum class eUICursorType;

namespace onut
{
    class RenderStates;
};

static const float DOTTED_SCALE = 1.0f / 8.0f;
static const float ORIGIN_SIZE  = 4.0f;

enum class eGizmo2DState
{
    None,
    Move,
    Rotate,
    Size
};

class Gizmo2DContext final
{
public:
    Matrix              transform;
    float               scale       = 1.0f;
    float               inv_scale   = 1.0f;
    onut::RenderStates* rs          = nullptr;
    OPrimitiveBatch*    pb          = nullptr;
    eGizmo2DState       state       = eGizmo2DState::None;

    Matrix  single_select_transform;
    Rect    single_select_rect;
    Vector2 single_select_corners[4];
    Vector2 multi_select_mins;  // In world
    Vector2 multi_select_maxs;  // In world
    Vector2 mouse_pos;          // In world

    struct MoveContext
    {
        Vector2                 world_mouse_pos_on_down;
        std::vector<Matrix>     entity_transforms_on_down;
    } move_ctx;

    struct RotateContext
    {
        Vector2             world_mouse_pos_on_down;
        Vector2             world_center;
        std::vector<Matrix> entity_transforms_on_down;
    } rotate_ctx;

    void reset();

    void doMoveLogic(const std::vector<EntityRef>& entities, GUIContext* ctx);
    void doRotateLogic(const std::vector<EntityRef>& entities, GUIContext* ctx);

    void applyTransforms(std::vector<EntityRef> entities, std::vector<Matrix> transforms_before, std::vector<Matrix> transforms_after);

    void drawDottedRect(const Matrix& transform, const Vector2& size, const Vector2& origin);
    void drawOrigin(const Matrix& transform);
    void drawSelected(const std::vector<EntityRef>& entities, GUIContext* ctx);
    bool drawHandle(const Vector2& pos, const Vector2& dir, eUICursorType* cursor);
};

// Public includes
#include <onut/Axis.h>

// Private includes
#include "JSBindings.h"
#include "JSBindings_Macros.h"

// Third party
#include <json/json.h>

// STL
#include <set>

// Forward declare
#include <onut/ForwardDeclaration.h>
ForwardDeclare(JSComponentDefinition);

class JSComponentDefinition
{
public:
    std::string name;

    bool has_onCreate = false;
    bool has_onUpdate = false;
    bool has_onRender = false;
    bool has_onRender2d = false;
    bool has_onMessage = false;
    bool has_onAddChild = false;
    bool has_onRemoveChild = false;
    bool has_onTriggerEnter = false;
    bool has_onTriggerLeave = false;
    bool has_onEnable = false;
    bool has_onDisable = false;
    bool has_onDestroy = false;
};

class JSComponent : public OComponent
{
public:
    void onCreate()
    {
    }

    void onUpdate()
    {
    }

    void onRender()
    {
    }

    void onRender2d()
    {
    }

    void onMessage(int messageId, void* pData)
    {
    }

    void onAddChild(const OEntityRef& pChild)
    {
    }

    void onRemoveChild(const OEntityRef& pChild)
    {
    }

    void onTriggerEnter(const OCollider2DComponentRef& pCollider)
    {
    }

    void onTriggerLeave(const OCollider2DComponentRef& pCollider)
    {
    }

    void onEnable()
    {
    }

    void onDisable()
    {
    }

    void onDestroy()
    {
    }

    JSComponentDefinitionRef pDefinition;
};

namespace onut
{
    namespace js
    {
        void createImguiBindings();

        std::unordered_map<std::string, OUIControlRef> UImap;

        static void createBindings();
        static bool evalScripts();

        duk_context* pContext = nullptr;

        // Global functions heap pointers
        void* pUpdatePtr = nullptr;
        void* pRenderPtr = nullptr;
        void* pRenderUIPtr = nullptr;

        // Prototypes heap pointers
        void* pVector2Prototype = nullptr;
        void* pVector3Prototype = nullptr;
        void* pVector4Prototype = nullptr;
        void* pRectPrototype = nullptr;
        void* pColorPrototype = nullptr;
        void* pMatrixPrototype = nullptr;

        void* pTexturePrototype = nullptr;
        void* pModelPrototype = nullptr;
        void* pFontPrototype = nullptr;
        void* pShaderPrototype = nullptr;
        void* pMusicPrototype = nullptr;
        void* pSoundPrototype = nullptr;
        void* pSoundInstancePrototype = nullptr;
        void* pTiledMapPrototype = nullptr;
        void* pSpriteAnimPrototype = nullptr;
        void* pSpriteAnimInstancePrototype = nullptr;
        void* pParticleSystemPrototype = nullptr;
        void* pParticleEmitterPrototype = nullptr;
        void* pVertexBufferPrototype = nullptr;
        void* pIndexBufferPrototype = nullptr;

        void* pBoolAnimPrototype = nullptr;
        void* pNumberAnimPrototype = nullptr;
        void* pVector2AnimPrototype = nullptr;
        void* pVector3AnimPrototype = nullptr;
        void* pVector4AnimPrototype = nullptr;
        void* pRectAnimPrototype = nullptr;
        void* pMatrixAnimPrototype = nullptr;
        void* pColorAnimPrototype = nullptr;

        void* pVideoPlayerPrototype = nullptr;
        void* pEntityPrototype = nullptr;

        void* pBinaryFileWriterPrototype = nullptr;
        void* pBinaryFileReaderPrototype = nullptr;

        void* pUIPrototype = nullptr;

        void* pUpdaterPrototype = nullptr;

        std::unordered_map<std::string, JSComponentDefinitionRef> jsComponentDefinitions;

        void updateViewport()
        {
            auto& pRenderTarget = oRenderer->renderStates.renderTarget.get();
            Point resolution;
            if (pRenderTarget) resolution = pRenderTarget->getSize();
            else resolution = oRenderer->getTrueResolution();
            oRenderer->renderStates.viewport = iRect{0, 0, resolution.x, resolution.y};
        }

        // Helpers
#define FLOAT_PROP(__name__, __index__) \
            auto __name__ = 0.0f; \
            if (duk_get_prop_string(ctx, __index__, #__name__)) \
            { \
                __name__ = (float)duk_to_number(ctx, -1); \
                duk_pop(ctx); \
            }

#define INT_PROP(__name__, __index__) \
            auto __name__ = 0; \
            if (duk_get_prop_string(ctx, __index__, #__name__)) \
            { \
                __name__ = duk_to_int(ctx, -1); \
                duk_pop(ctx); \
            }

#define FLOAT_PROP2(__ret__, __name__, __index__) \
            if (duk_get_prop_string(ctx, __index__, __name__)) \
            { \
                __ret__ = (float)duk_to_number(ctx, -1); \
                duk_pop(ctx); \
            }

#define UINT_PROP(__name__, __index__) \
            duk_get_prop_string(ctx, __index__, #__name__); \
            auto __name__ = duk_to_uint(ctx, -1); \
            duk_pop(ctx);

#define STRING_PROP(__name__, __index__) \
            duk_get_prop_string(ctx, __index__, #__name__); \
            auto __name__ = duk_to_string(ctx, -1); \
            duk_pop(ctx);

        void newVector2(duk_context* ctx, const Vector2& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_heapptr(ctx, pVector2Prototype);
            duk_set_prototype(ctx, -2);
        }

        void newVector3(duk_context* ctx, const Vector3& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_number(ctx, val.z);
            duk_put_prop_string(ctx, -2, "z");
            duk_push_heapptr(ctx, pVector3Prototype);
            duk_set_prototype(ctx, -2);
        }

        void newVector4(duk_context* ctx, const Vector4& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_number(ctx, val.z);
            duk_put_prop_string(ctx, -2, "z");
            duk_push_number(ctx, val.w);
            duk_put_prop_string(ctx, -2, "w");
            duk_push_heapptr(ctx, pVector4Prototype);
            duk_set_prototype(ctx, -2);
        }

        void newRect(duk_context* ctx, const Rect& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_number(ctx, val.z);
            duk_put_prop_string(ctx, -2, "w");
            duk_push_number(ctx, val.w);
            duk_put_prop_string(ctx, -2, "h");
            duk_push_heapptr(ctx, pRectPrototype);
            duk_set_prototype(ctx, -2);
        }

        void newColor(duk_context* ctx, const Color& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.r);
            duk_put_prop_string(ctx, -2, "r");
            duk_push_number(ctx, val.g);
            duk_put_prop_string(ctx, -2, "g");
            duk_push_number(ctx, val.b);
            duk_put_prop_string(ctx, -2, "b");
            duk_push_number(ctx, val.a);
            duk_put_prop_string(ctx, -2, "a");
            duk_push_heapptr(ctx, pColorPrototype);
            duk_set_prototype(ctx, -2);
        }

        void newMatrix(duk_context* ctx, const Matrix& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val._11); duk_put_prop_string(ctx, -2, "_11");
            duk_push_number(ctx, val._12); duk_put_prop_string(ctx, -2, "_12");
            duk_push_number(ctx, val._13); duk_put_prop_string(ctx, -2, "_13");
            duk_push_number(ctx, val._14); duk_put_prop_string(ctx, -2, "_14");
            duk_push_number(ctx, val._21); duk_put_prop_string(ctx, -2, "_21");
            duk_push_number(ctx, val._22); duk_put_prop_string(ctx, -2, "_22");
            duk_push_number(ctx, val._23); duk_put_prop_string(ctx, -2, "_23");
            duk_push_number(ctx, val._24); duk_put_prop_string(ctx, -2, "_24");
            duk_push_number(ctx, val._31); duk_put_prop_string(ctx, -2, "_31");
            duk_push_number(ctx, val._32); duk_put_prop_string(ctx, -2, "_32");
            duk_push_number(ctx, val._33); duk_put_prop_string(ctx, -2, "_33");
            duk_push_number(ctx, val._34); duk_put_prop_string(ctx, -2, "_34");
            duk_push_number(ctx, val._41); duk_put_prop_string(ctx, -2, "_41");
            duk_push_number(ctx, val._42); duk_put_prop_string(ctx, -2, "_42");
            duk_push_number(ctx, val._43); duk_put_prop_string(ctx, -2, "_43");
            duk_push_number(ctx, val._44); duk_put_prop_string(ctx, -2, "_44");
            duk_push_heapptr(ctx, pMatrixPrototype);
            duk_set_prototype(ctx, -2);
        }

        Vector2 getVector2(duk_context *ctx, duk_idx_t index, const Vector2& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(x, index);
                FLOAT_PROP(y, index);
                return Vector2(x, y);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, index);
                return Vector2(s, s);
            }
            return in_default;
        }

        Point getPoint(duk_context *ctx, duk_idx_t index, const Point& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                INT_PROP(x, index);
                INT_PROP(y, index);
                return Point(x, y);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = duk_to_int(ctx, index);
                return Point(s, s);
            }
            return in_default;
        }

        Vector3 getVector3(duk_context *ctx, duk_idx_t index, const Vector3& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(x, index);
                FLOAT_PROP(y, index);
                FLOAT_PROP(z, index);
                return Vector3(x, y, z);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, index);
                return Vector3(s, s, s);
            }
            return in_default;
        }

        Vector4 getVector4(duk_context *ctx, duk_idx_t index, const Vector4& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(x, index);
                FLOAT_PROP(y, index);
                FLOAT_PROP(z, index);
                FLOAT_PROP(w, index);
                return Vector4(x, y, z, w);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, index);
                return Vector4(s, s, s, s);
            }
            return in_default;
        }

        Vector4 getRect(duk_context *ctx, duk_idx_t index, const Vector4& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(x, index);
                FLOAT_PROP(y, index);
                FLOAT_PROP(w, index);
                FLOAT_PROP(h, index);
                return Rect(x, y, w, h);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, index);
                return Rect(s, s, s, s);
            }
            return in_default;
        }

        iRect getiRect(duk_context *ctx, duk_idx_t index, const iRect& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                INT_PROP(x, index);
                INT_PROP(y, index);
                INT_PROP(w, index);
                INT_PROP(h, index);
                return iRect{x, y, x + w, y + h};
            }
            else if (duk_is_number(ctx, index))
            {
                INT_PROP(s, index);
                return iRect{s, s, s, s};
            }
            return in_default;
        }

        Color getColor(duk_context *ctx, duk_idx_t index, const Color& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(r, index);
                FLOAT_PROP(g, index);
                FLOAT_PROP(b, index);
                FLOAT_PROP(a, index);
                return Color(r, g, b, a);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, index);
                return Color(s, s, s, s);
            }
            return in_default;
        }

        Matrix getMatrix(duk_context *ctx, duk_idx_t index, const Matrix& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;
            Matrix ret;
            FLOAT_PROP2(ret._11, "_11", index);
            FLOAT_PROP2(ret._12, "_12", index);
            FLOAT_PROP2(ret._13, "_13", index);
            FLOAT_PROP2(ret._14, "_14", index);
            FLOAT_PROP2(ret._21, "_21", index);
            FLOAT_PROP2(ret._22, "_22", index);
            FLOAT_PROP2(ret._23, "_23", index);
            FLOAT_PROP2(ret._24, "_24", index);
            FLOAT_PROP2(ret._31, "_31", index);
            FLOAT_PROP2(ret._32, "_32", index);
            FLOAT_PROP2(ret._33, "_33", index);
            FLOAT_PROP2(ret._34, "_34", index);
            FLOAT_PROP2(ret._41, "_41", index);
            FLOAT_PROP2(ret._42, "_42", index);
            FLOAT_PROP2(ret._43, "_43", index);
            FLOAT_PROP2(ret._44, "_44", index);
            return std::move(ret);
        }

        bool getBool(duk_context *ctx, duk_idx_t index, bool in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;
            if (!duk_is_boolean(ctx, index)) return in_default;
            return duk_to_boolean(ctx, index) == 1 ? true : false;
        }

        float getFloat(duk_context *ctx, duk_idx_t index, float in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;
            if (!duk_is_number(ctx, index)) return in_default;
            return (float)duk_to_number(ctx, index);
        }

        unsigned int getUInt(duk_context *ctx, duk_idx_t index, unsigned int in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;
            if (!duk_is_number(ctx, index)) return in_default;
            return duk_to_uint(ctx, index);
        }

        int getInt(duk_context *ctx, duk_idx_t index, int in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;
            if (!duk_is_number(ctx, index)) return in_default;
            return duk_to_int(ctx, index);
        }

        const char* getString(duk_context *ctx, duk_idx_t index, const char* in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;
            if (!duk_is_string(ctx, index)) return in_default;
            return duk_to_string(ctx, index);
        }

        OUIControlRef getUI(duk_context *ctx, duk_idx_t index, const OUIControlRef& in_default)
        {
            if (index >= duk_get_top(ctx)) return in_default;
            if (duk_is_null_or_undefined(ctx, index)) return in_default;

            duk_get_prop_string(ctx, index, "\xff""\xff""data");
            auto pp = (OUIControlRef*)(duk_to_pointer(ctx, -1));
            duk_pop(ctx);

            if (pp)
            {
                return *pp;
            }
            else
            {
                return nullptr;
            }
        }

        OSpriteAnimInstanceRef getSpriteAnimInstance(duk_context *ctx, duk_idx_t index)
        {
            if (index >= duk_get_top(ctx)) return nullptr;
            if (duk_is_null_or_undefined(ctx, index)) return nullptr;

            if (duk_is_string(ctx, index))
            {
                return OMake<OSpriteAnimInstance>(duk_to_string(ctx, index));
            }
            else
            {
                duk_get_prop_string(ctx, index, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)(duk_to_pointer(ctx, -1));
                duk_pop(ctx);

                if (ppSpriteAnimInstance)
                {
                    return *ppSpriteAnimInstance;
                }
                else
                {
                    return nullptr;
                }
            }
        }

        OEmitterInstance* getParticleEmitter(duk_context *ctx, duk_idx_t index)
        {
            if (index >= duk_get_top(ctx)) return nullptr;
            if (duk_is_null_or_undefined(ctx, index)) return nullptr;

            duk_get_prop_string(ctx, index, "\xff""\xff""data");
            auto pEmitter = (OEmitterInstance*)(duk_to_pointer(ctx, -1));
            duk_pop(ctx);

            return pEmitter;
        }

        FunctionRef getFunction(duk_context* ctx, int index)
        {
            if (index >= duk_get_top(ctx)) return nullptr;
            if (duk_is_function(ctx, index))
            {
                auto pFunction = OMake<Function>(ctx, index);
                return pFunction;
            }
            return nullptr;
        }

        void init()
        {
            pContext = duk_create_heap_default();

            createBindings();
            if (!evalScripts())
            {
                return;
            }

            // Grab global Update and Render pointers if present
            if (duk_get_global_string(pContext, "update"))
            {
                pUpdatePtr = duk_get_heapptr(pContext, -1);
                duk_pop(pContext);
            }
            if (duk_get_global_string(pContext, "render"))
            {
                pRenderPtr = duk_get_heapptr(pContext, -1);
                duk_pop(pContext);
            }
            if (duk_get_global_string(pContext, "renderUI"))
            {
                pRenderUIPtr = duk_get_heapptr(pContext, -1);
                duk_pop(pContext);
            }
        }

        void shutdown()
        {
            duk_destroy_heap(pContext);
            pContext = nullptr;
        }

        static void createVector2Bindings()
        {
            auto ctx = pContext;

            // Vector2(x, y)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                float x, y;
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    x = 0; y = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_null_or_undefined(ctx, 1))
                {
                    y = x = (float)duk_to_number(ctx, 0);
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                }
                else if (duk_is_object(ctx, 0))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    x = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    y = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else
                {
                    return DUK_RET_SYNTAX_ERROR;
                }

                duk_push_this(ctx);
                duk_push_number(ctx, x);
                duk_put_prop_string(ctx, -2, "x");
                duk_push_number(ctx, y);
                duk_put_prop_string(ctx, -2, "y");

                return 0;
            }, 2);
            duk_push_object(ctx);

#define JS_THIS_VECTOR2 \
    duk_push_this(ctx); \
    FLOAT_PROP(x, -1); \
    FLOAT_PROP(y, -1); \
    duk_pop(ctx); \
    Vector2 v(x, y)

            // isEqual(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                duk_push_boolean(ctx, v == other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "isEqual");

            // add(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                newVector2(ctx, v + other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // sub(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                newVector2(ctx, v - other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "sub");

            // mul(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                newVector2(ctx, v * other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "mul");

            // div(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                newVector2(ctx, Vector2(v.x / other.x, v.y / other.y));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "div");

            // length()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2;
                duk_push_number(ctx, (duk_double_t)v.Length());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "length");

            // lengthSquared()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2;
                duk_push_number(ctx, (duk_double_t)v.LengthSquared());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "lengthSquared");

            // dot(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                duk_push_number(ctx, (duk_double_t)v.Dot(other));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "dot");

            // cross(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR2;
                newVector2(ctx, v.Cross(other));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "cross");

            // clamp(min, max)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto minV = getVector2(ctx, 0);
                auto maxV = getVector2(ctx, 1);
                JS_THIS_VECTOR2;
                v.Clamp(minV, maxV);
                newVector2(ctx, v);
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "clamp");

            // normalize()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2;
                v.Normalize();
                newVector2(ctx, v);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "normalize");

            // Done with the object
            pVector2Prototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // distance(v1, v2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                duk_push_number(ctx, Vector2::Distance(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "distance");

            // distanceSquared(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                duk_push_number(ctx, Vector2::DistanceSquared(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "distanceSquared");

            // min(v1, v2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                newVector2(ctx, Vector2::Min(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "min");

            // max(v1, v2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                newVector2(ctx, Vector2::Max(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "max");

            // lerp(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                auto t = JS_FLOAT(2);
                newVector2(ctx, Vector2::Lerp(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "lerp");

            // smoothStep(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                auto t = JS_FLOAT(2);
                newVector2(ctx, Vector2::SmoothStep(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "smoothStep");

            // barycentric(v1, v2, v3, f, g)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                auto v3 = getVector2(ctx, 2);
                auto f = JS_FLOAT(3);
                auto g = JS_FLOAT(4);
                newVector2(ctx, Vector2::Barycentric(v1, v2, v3, f, g));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "barycentric");

            // catmullRom(v1, v2, v3, v4, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto v2 = getVector2(ctx, 1);
                auto v3 = getVector2(ctx, 2);
                auto v4 = getVector2(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector2(ctx, Vector2::CatmullRom(v1, v2, v3, v3, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "catmullRom");

            // hermite(v1, t1, v2, t2, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector2(ctx, 0);
                auto t1 = getVector2(ctx, 1);
                auto v2 = getVector2(ctx, 2);
                auto t2 = getVector2(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector2(ctx, Vector2::Hermite(v1, t1, v2, t2, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "hermite");

            // bezier(p1, p2, p3, p4, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto p1 = getVector2(ctx, 0);
                auto p2 = getVector2(ctx, 1);
                auto p3 = getVector2(ctx, 2);
                auto p4 = getVector2(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector2(ctx, onut::bezier<Vector2>(p1, p2, p3, p4, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "bezier");

            // reflect(ivec, nvec)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto ivec = getVector2(ctx, 0);
                auto nvec = getVector2(ctx, 1);
                newVector2(ctx, Vector2::Reflect(ivec, nvec));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "reflect");

            // refract(ivec, nvec, refractionIndex)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto ivec = getVector2(ctx, 0);
                auto nvec = getVector2(ctx, 1);
                auto refractionIndex = JS_FLOAT(2);
                newVector2(ctx, Vector2::Refract(ivec, nvec, refractionIndex));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "refract");

            // Zero
            newVector2(ctx, Vector2::Zero);
            duk_put_prop_string(ctx, -2, "ZERO");
            newVector2(ctx, Vector2::One);
            duk_put_prop_string(ctx, -2, "ONE");
            newVector2(ctx, Vector2::UnitX);
            duk_put_prop_string(ctx, -2, "UNIT_X");
            newVector2(ctx, Vector2::UnitY);
            duk_put_prop_string(ctx, -2, "UNIT_Y");

            newVector2(ctx, Vector2(0, 0));
            duk_put_prop_string(ctx, -2, "TOP_LEFT");
            newVector2(ctx, Vector2(.5f, 0));
            duk_put_prop_string(ctx, -2, "TOP");
            newVector2(ctx, Vector2(1, 0));
            duk_put_prop_string(ctx, -2, "TOP_RIGHT");
            newVector2(ctx, Vector2(0, .5f));
            duk_put_prop_string(ctx, -2, "LEFT");
            newVector2(ctx, Vector2(.5f, .5f));
            duk_put_prop_string(ctx, -2, "CENTER");
            newVector2(ctx, Vector2(1, .5f));
            duk_put_prop_string(ctx, -2, "RIGHT");
            newVector2(ctx, Vector2(0, 1));
            duk_put_prop_string(ctx, -2, "BOTTOM_LEFT");
            newVector2(ctx, Vector2(.5f, 1));
            duk_put_prop_string(ctx, -2, "BOTTOM");
            newVector2(ctx, Vector2(1, 1));
            duk_put_prop_string(ctx, -2, "BOTTOM_RIGHT");

            duk_put_global_string(ctx, "Vector2");
        }

        static void createVector3Bindings()
        {
            auto ctx = pContext;

            // Vector3(x, y, z)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                float x, y, z;
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    x = 0; y = 0; z = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_null_or_undefined(ctx, 1) && duk_is_null_or_undefined(ctx, 2))
                {
                    z = y = x = (float)duk_to_number(ctx, 0);
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_null_or_undefined(ctx, 2))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    z = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_number(ctx, 2))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    z = (float)duk_to_number(ctx, 2);
                }
                else if (duk_is_object(ctx, 0))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    x = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    y = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    if (duk_get_prop_string(ctx, 0, "z"))
                    {
                        z = (float)duk_to_number(ctx, -1);
                        duk_pop(ctx);
                    }
                    else if (duk_is_number(ctx, 1))
                    {
                        z = (float)duk_to_number(ctx, 1);
                    }
                    else
                    {
                        z = 0;
                    }
                }
                else
                {
                    return DUK_RET_SYNTAX_ERROR;
                }

                duk_push_this(ctx);
                duk_push_number(ctx, x);
                duk_put_prop_string(ctx, -2, "x");
                duk_push_number(ctx, y);
                duk_put_prop_string(ctx, -2, "y");
                duk_push_number(ctx, z);
                duk_put_prop_string(ctx, -2, "z");

                return 0;
            }, 3);
            duk_push_object(ctx);

#define JS_THIS_VECTOR3 \
    duk_push_this(ctx); \
    FLOAT_PROP(x, -1); \
    FLOAT_PROP(y, -1); \
    FLOAT_PROP(z, -1); \
    duk_pop(ctx); \
    Vector3 v(x, y, z)

            // isEqual(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                duk_push_boolean(ctx, v == other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "isEqual");

            // add(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                newVector3(ctx, v + other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // sub(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                newVector3(ctx, v - other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "sub");

            // mul(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                newVector3(ctx, v * other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "mul");

            // transform(matrix)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto matrix = JS_MATRIX(0);
                JS_THIS_VECTOR3;
                newVector3(ctx, Vector3::Transform(v, matrix));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "transform");

            // div(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                newVector3(ctx, Vector3(v.x / other.x, v.y / other.y, v.z / other.z));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "div");

            // length()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3;
                duk_push_number(ctx, (duk_double_t)v.Length());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "length");

            // lengthSquared()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3;
                duk_push_number(ctx, (duk_double_t)v.LengthSquared());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "lengthSquared");

            // dot(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                duk_push_number(ctx, (duk_double_t)v.Dot(other));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "dot");

            // cross(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector3(ctx, 0);
                JS_THIS_VECTOR3;
                newVector3(ctx, v.Cross(other));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "cross");

            // clamp(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto minV = getVector3(ctx, 0);
                auto maxV = getVector3(ctx, 1);
                JS_THIS_VECTOR3;
                v.Clamp(minV, maxV);
                newVector3(ctx, v);
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "clamp");

            // normalize()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3;
                v.Normalize();
                newVector3(ctx, v);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "normalize");

            // Done with the object
            pVector3Prototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // distance(v1, v2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                duk_push_number(ctx, Vector3::Distance(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "distance");

            // distanceSquared(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                duk_push_number(ctx, Vector3::DistanceSquared(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "distanceSquared");

            // min(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                newVector3(ctx, Vector3::Min(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "min");

            // max(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                newVector3(ctx, Vector3::Max(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "max");

            // lerp(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                auto t = JS_FLOAT(2);
                newVector3(ctx, Vector3::Lerp(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "lerp");

            // smoothStep(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                auto t = JS_FLOAT(2);
                newVector3(ctx, Vector3::SmoothStep(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "smoothStep");

            // barycentric(v1, v2, v3, f, g)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                auto v3 = getVector3(ctx, 2);
                auto f = JS_FLOAT(3);
                auto g = JS_FLOAT(4);
                newVector3(ctx, Vector3::Barycentric(v1, v2, v3, f, g));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "barycentric");

            // catmullRom(v1, v2, v3, v4, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto v2 = getVector3(ctx, 1);
                auto v3 = getVector3(ctx, 2);
                auto v4 = getVector3(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector3(ctx, Vector3::CatmullRom(v1, v2, v3, v3, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "catmullRom");

            // hermite(v1, t1, v2, t2, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector3(ctx, 0);
                auto t1 = getVector3(ctx, 1);
                auto v2 = getVector3(ctx, 2);
                auto t2 = getVector3(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector3(ctx, Vector3::Hermite(v1, t1, v2, t2, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "hermite");

            // bezier(p1, p2, p3, p4, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto p1 = getVector3(ctx, 0);
                auto p2 = getVector3(ctx, 1);
                auto p3 = getVector3(ctx, 2);
                auto p4 = getVector3(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector3(ctx, onut::bezier<Vector3>(p1, p2, p3, p4, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "bezier");

            // reflect(ivec, nvec)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto ivec = getVector3(ctx, 0);
                auto nvec = getVector3(ctx, 1);
                newVector3(ctx, Vector3::Reflect(ivec, nvec));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "reflect");

            // refract(ivec, nvec, refractionIndex)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto ivec = getVector3(ctx, 0);
                auto nvec = getVector3(ctx, 1);
                auto refractionIndex = JS_FLOAT(2);
                newVector3(ctx, Vector3::Refract(ivec, nvec, refractionIndex));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "refract");

            // Constants
            newVector3(ctx, Vector3::Zero);
            duk_put_prop_string(ctx, -2, "ZERO");
            newVector3(ctx, Vector3::One);
            duk_put_prop_string(ctx, -2, "ONE");
            newVector3(ctx, Vector3::UnitX);
            duk_put_prop_string(ctx, -2, "UNIT_X");
            newVector3(ctx, Vector3::UnitY);
            duk_put_prop_string(ctx, -2, "UNIT_Y");
            newVector3(ctx, Vector3::UnitZ);
            duk_put_prop_string(ctx, -2, "UNIT_Z");

            duk_put_global_string(ctx, "Vector3");
        }

        static void createVector4Bindings()
        {
            auto ctx = pContext;

            // Vector4(x, y, z, w)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                float x, y, z, w;
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    x = 0; y = 0; z = 0; w = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_null_or_undefined(ctx, 1) && duk_is_null_or_undefined(ctx, 2) && duk_is_null_or_undefined(ctx, 3))
                {
                    w = z = y = x = (float)duk_to_number(ctx, 0);
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_null_or_undefined(ctx, 2) && duk_is_null_or_undefined(ctx, 3))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    z = 0;
                    w = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_number(ctx, 2) && duk_is_null_or_undefined(ctx, 3))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    z = (float)duk_to_number(ctx, 2);
                    w = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_number(ctx, 2) && duk_is_number(ctx, 3))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    z = (float)duk_to_number(ctx, 2);
                    w = (float)duk_to_number(ctx, 3);
                }
                else if (duk_is_object(ctx, 0) && !duk_is_object(ctx, 1))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    x = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    y = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    if (duk_get_prop_string(ctx, 0, "z"))
                    {
                        z = (float)duk_to_number(ctx, -1);
                        duk_pop(ctx);
                    }
                    else if (duk_is_number(ctx, 1))
                    {
                        z = (float)duk_to_number(ctx, 1);
                    }
                    else
                    {
                        z = 0;
                    }
                    if (duk_get_prop_string(ctx, 0, "w"))
                    {
                        w = (float)duk_to_number(ctx, -1);
                        duk_pop(ctx);
                    }
                    else if (duk_is_number(ctx, 2))
                    {
                        w = (float)duk_to_number(ctx, 2);
                    }
                    else
                    {
                        w = 0;
                    }
                }
                else if (duk_is_object(ctx, 0) && duk_is_object(ctx, 1))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    x = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    y = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "x");
                    z = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "y");
                    w = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else
                {
                    return DUK_RET_SYNTAX_ERROR;
                }

                duk_push_this(ctx);
                duk_push_number(ctx, x);
                duk_put_prop_string(ctx, -2, "x");
                duk_push_number(ctx, y);
                duk_put_prop_string(ctx, -2, "y");
                duk_push_number(ctx, z);
                duk_put_prop_string(ctx, -2, "z");
                duk_push_number(ctx, w);
                duk_put_prop_string(ctx, -2, "w");

                return 0;
            }, 4);
            duk_push_object(ctx);

#define JS_THIS_VECTOR4 \
    duk_push_this(ctx); \
    FLOAT_PROP(x, -1); \
    FLOAT_PROP(y, -1); \
    FLOAT_PROP(z, -1); \
    FLOAT_PROP(w, -1); \
    duk_pop(ctx); \
    Vector4 v(x, y, z, w)

            // isEqual(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                duk_push_boolean(ctx, v == other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "isEqual");

            // add(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                newVector4(ctx, v + other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // sub(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                newVector4(ctx, v - other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "sub");

            // mul(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                newVector4(ctx, v * other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "mul");

            // transform(matrix)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto matrix = JS_MATRIX(0);
                JS_THIS_VECTOR4;
                newVector4(ctx, Vector4::Transform(v, matrix));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "transform");

            // div(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                newVector4(ctx, Vector4(v.x / other.x, v.y / other.y, v.z / other.z, v.w / other.w));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "div");

            // length()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4;
                duk_push_number(ctx, (duk_double_t)v.Length());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "length");

            // lengthSquared()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4;
                duk_push_number(ctx, (duk_double_t)v.LengthSquared());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "lengthSquared");

            // dot(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                duk_push_number(ctx, (duk_double_t)v.Dot(other));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "dot");

            // cross(v1, v2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 0);
                JS_THIS_VECTOR4;
                newVector4(ctx, v.Cross(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "cross");

            // clamp(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto minV = getVector4(ctx, 0);
                auto maxV = getVector4(ctx, 1);
                JS_THIS_VECTOR4;
                v.Clamp(minV, maxV);
                newVector4(ctx, v);
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "clamp");

            // normalize()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4;
                v.Normalize();
                newVector4(ctx, v);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "normalize");

            // Done with the object
            pVector4Prototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // distance(v1, v2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                duk_push_number(ctx, Vector4::Distance(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "distance");

            // distanceSquared(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                duk_push_number(ctx, Vector4::DistanceSquared(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "distanceSquared");

            // min(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                newVector4(ctx, Vector4::Min(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "min");

            // max(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                newVector4(ctx, Vector4::Max(v1, v2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "max");

            // lerp(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                auto t = JS_FLOAT(2);
                newVector4(ctx, Vector4::Lerp(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "lerp");

            // smoothStep(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                auto t = JS_FLOAT(2);
                newVector4(ctx, Vector4::SmoothStep(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "smoothStep");

            // barycentric(v1, v2, v3, f, g)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                auto v3 = getVector4(ctx, 2);
                auto f = JS_FLOAT(3);
                auto g = JS_FLOAT(4);
                newVector4(ctx, Vector4::Barycentric(v1, v2, v3, f, g));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "barycentric");

            // catmullRom(v1, v2, v3, v4, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto v2 = getVector4(ctx, 1);
                auto v3 = getVector4(ctx, 2);
                auto v4 = getVector4(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector4(ctx, Vector4::CatmullRom(v1, v2, v3, v3, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "catmullRom");

            // hermite(v1, t1, v2, t2, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getVector4(ctx, 0);
                auto t1 = getVector4(ctx, 1);
                auto v2 = getVector4(ctx, 2);
                auto t2 = getVector4(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector4(ctx, Vector4::Hermite(v1, t1, v2, t2, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "hermite");

            // bezier(p1, p2, p3, p4, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto p1 = getVector4(ctx, 0);
                auto p2 = getVector4(ctx, 1);
                auto p3 = getVector4(ctx, 2);
                auto p4 = getVector4(ctx, 3);
                auto t = JS_FLOAT(4);
                newVector4(ctx, onut::bezier<Vector4>(p1, p2, p3, p4, t));
                return 1;
            }, 5);
            duk_put_prop_string(ctx, -2, "bezier");

            // reflect(ivec, nvec)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto ivec = getVector4(ctx, 0);
                auto nvec = getVector4(ctx, 1);
                newVector4(ctx, Vector4::Reflect(ivec, nvec));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "reflect");

            // refract(ivec, nvec, refractionIndex)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto ivec = getVector4(ctx, 0);
                auto nvec = getVector4(ctx, 1);
                auto refractionIndex = JS_FLOAT(2);
                newVector4(ctx, Vector4::Refract(ivec, nvec, refractionIndex));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "refract");

            // Constants
            newVector4(ctx, Vector4::Zero);
            duk_put_prop_string(ctx, -2, "ZERO");
            newVector4(ctx, Vector4::One);
            duk_put_prop_string(ctx, -2, "ONE");
            newVector4(ctx, Vector4::UnitX);
            duk_put_prop_string(ctx, -2, "UNIT_X");
            newVector4(ctx, Vector4::UnitY);
            duk_put_prop_string(ctx, -2, "UNIT_Y");
            newVector4(ctx, Vector4::UnitZ);
            duk_put_prop_string(ctx, -2, "UNIT_Z");
            newVector4(ctx, Vector4::UnitW);
            duk_put_prop_string(ctx, -2, "UNIT_W");

            duk_put_global_string(ctx, "Vector4");
        }

        static void createRectBindings()
        {
            auto ctx = pContext;

            // Rect(x, y, w, h)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                float x, y, w, h;
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    x = 0; y = 0; w = 0; h = 0;
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_number(ctx, 2) && duk_is_number(ctx, 3))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    w = (float)duk_to_number(ctx, 2);
                    h = (float)duk_to_number(ctx, 3);
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_object(ctx, 2))
                {
                    x = (float)duk_to_number(ctx, 0);
                    y = (float)duk_to_number(ctx, 1);
                    duk_get_prop_string(ctx, 2, "x");
                    w = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 2, "y");
                    h = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else if (duk_is_number(ctx, 1) && duk_is_number(ctx, 2) && duk_is_object(ctx, 0))
                {
                    x = (float)duk_to_number(ctx, 2);
                    y = (float)duk_to_number(ctx, 3);
                    duk_get_prop_string(ctx, 0, "x");
                    w = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    h = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else if (duk_is_object(ctx, 0) && !duk_is_object(ctx, 1))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    x = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    y = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "w");
                    w = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "h");
                    h = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else if (duk_is_object(ctx, 0) && duk_is_object(ctx, 1))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    x = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    y = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "x");
                    w = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "y");
                    h = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else
                {
                    return DUK_RET_SYNTAX_ERROR;
                }

                duk_push_this(ctx);
                duk_push_number(ctx, x);
                duk_put_prop_string(ctx, -2, "x");
                duk_push_number(ctx, y);
                duk_put_prop_string(ctx, -2, "y");
                duk_push_number(ctx, w);
                duk_put_prop_string(ctx, -2, "w");
                duk_push_number(ctx, h);
                duk_put_prop_string(ctx, -2, "h");

                return 0;
            }, 4);
            duk_push_object(ctx);

#define JS_THIS_RECT \
    duk_push_this(ctx); \
    FLOAT_PROP(x, -1); \
    FLOAT_PROP(y, -1); \
    FLOAT_PROP(w, -1); \
    FLOAT_PROP(h, -1); \
    duk_pop(ctx); \
    Rect v(x, y, w, h)

            // isEqual(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getRect(ctx, 0);
                JS_THIS_RECT;
                duk_push_boolean(ctx, v == other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "isEqual");

            // contains(p)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto p = getVector2(ctx, 0);
                JS_THIS_RECT;
                duk_push_boolean(ctx, v.Contains(p));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "contains");

            // grow(by)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto by = getFloat(ctx, 0);
                JS_THIS_RECT;
                newRect(ctx, v.Grow(by));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "grow");

            // distance(p)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto p = getVector2(ctx, 0);
                JS_THIS_RECT;
                duk_push_number(ctx, v.Distance(p));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "distance");

            // fillSize
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto size = JS_VECTOR2(0);
                JS_THIS_RECT;
                newRect(ctx, v.Fill(size));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "fillSize");

            // fillRect
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto rect = JS_RECT(0);
                JS_THIS_RECT;
                newRect(ctx, v.Fill(rect));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "fillRect");

            // fitSize
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto size = JS_VECTOR2(0);
                JS_THIS_RECT;
                newRect(ctx, v.Fit(size));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "fitSize");

            // fitRect
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto rect = JS_RECT(0);
                JS_THIS_RECT;
                newRect(ctx, v.Fit(rect));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "fitRect");

            // Done with the object
            pRectPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "Rect");
        }

        static void createColorBindings()
        {
            auto ctx = pContext;

            // Vector4(x, y, z, w)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                float r, g, b, a;
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    r = 1; g = 1; b = 1; a = 1;
                }
                else if (duk_is_number(ctx, 0) && duk_is_null_or_undefined(ctx, 1) && duk_is_null_or_undefined(ctx, 2) && duk_is_null_or_undefined(ctx, 3))
                {
                    a = b = g = r = (float)duk_to_number(ctx, 0);
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_number(ctx, 2) && duk_is_null_or_undefined(ctx, 3))
                {
                    r = (float)duk_to_number(ctx, 0);
                    g = (float)duk_to_number(ctx, 1);
                    b = (float)duk_to_number(ctx, 2);
                    a = 1;
                }
                else if (duk_is_number(ctx, 0) && duk_is_number(ctx, 1) && duk_is_number(ctx, 2) && duk_is_number(ctx, 3))
                {
                    r = (float)duk_to_number(ctx, 0);
                    g = (float)duk_to_number(ctx, 1);
                    b = (float)duk_to_number(ctx, 2);
                    a = (float)duk_to_number(ctx, 3);
                }
                else if (duk_is_object(ctx, 0))
                {
                    duk_get_prop_string(ctx, 0, "r");
                    r = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "g");
                    g = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "b");
                    b = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "a");
                    a = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else
                {
                    return DUK_RET_SYNTAX_ERROR;
                }

                duk_push_this(ctx);
                duk_push_number(ctx, r);
                duk_put_prop_string(ctx, -2, "r");
                duk_push_number(ctx, g);
                duk_put_prop_string(ctx, -2, "g");
                duk_push_number(ctx, b);
                duk_put_prop_string(ctx, -2, "b");
                duk_push_number(ctx, a);
                duk_put_prop_string(ctx, -2, "a");

                return 0;
            }, 4);
            duk_push_object(ctx);

#define JS_THIS_COLOR \
    duk_push_this(ctx); \
    FLOAT_PROP(r, -1); \
    FLOAT_PROP(g, -1); \
    FLOAT_PROP(b, -1); \
    FLOAT_PROP(a, -1); \
    duk_pop(ctx); \
    Color v(r, g, b, a)

            // isEqual(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getColor(ctx, 0);
                JS_THIS_COLOR;
                duk_push_boolean(ctx, v == other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "isEqual");

            // add(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getColor(ctx, 0);
                JS_THIS_COLOR;
                newColor(ctx, v + other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // sub(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getColor(ctx, 0);
                JS_THIS_COLOR;
                newColor(ctx, v - other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "sub");

            // mul(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getColor(ctx, 0);
                JS_THIS_COLOR;
                newColor(ctx, v * other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "mul");

            // div(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getColor(ctx, 0);
                JS_THIS_COLOR;
                newColor(ctx, Color(v.r / other.r, v.g / other.g, v.b / other.b, v.a / other.a));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "div");

            // toVector3
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                newVector3(ctx, Vector3(v.r, v.g, v.b));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "toVector3");

            // toVector4
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                newVector4(ctx, Vector4(v.r, v.g, v.b, v.a));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "toVector4");

            // Negate
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                v.Negate();
                newColor(ctx, v);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "negate");

            // saturate
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                v.Saturate();
                newColor(ctx, v);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "saturate");

            // premultiply
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                v.Premultiply();
                newColor(ctx, v);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "premultiply");

            // adjustSaturation
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto sat = JS_FLOAT(0);
                JS_THIS_COLOR;
                v.AdjustSaturation(sat);
                newColor(ctx, v);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "adjustSaturation");

            // adjustContrast
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto contrast = JS_FLOAT(0);
                JS_THIS_COLOR;
                v.AdjustContrast(contrast);
                newColor(ctx, v);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "adjustContrast");

            // Done with the object
            pColorPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // modulate(c1, c2)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto c1 = getColor(ctx, 0);
                auto c2 = getColor(ctx, 1);
                newColor(ctx, Color::Modulate(c1, c2));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "modulate");

            // lerp(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto v1 = getColor(ctx, 0);
                auto v2 = getColor(ctx, 1);
                auto t = JS_FLOAT(2);
                newColor(ctx, Color::Lerp(v1, v2, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "lerp");

            // fromHexRGB(hex)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto hex = JS_UINT(0);
                newColor(ctx, Color::fromHexRGB(hex));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "fromHexRGB");

            // fromHexRGBA(hex)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto hex = JS_UINT(0);
                newColor(ctx, Color::fromHexRGBA(hex));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "fromHexRGBA");

            // Constants
            newColor(ctx, Color::Black);
            duk_put_prop_string(ctx, -2, "BLACK");
            newColor(ctx, Color::White);
            duk_put_prop_string(ctx, -2, "WHITE");
            newColor(ctx, Color::Transparent);
            duk_put_prop_string(ctx, -2, "TRANSPARENT");
            newColor(ctx, Color::TransparentWhite);
            duk_put_prop_string(ctx, -2, "TRANSPARENT_WHITE");

            duk_put_global_string(ctx, "Color");
        }

        static void createMatrixBindings()
        {
            auto ctx = pContext;

            // Vector4(x, y, z, w)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                Matrix ret;
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    // Identity
                }
                else if (duk_is_object(ctx, 0) && duk_is_object(ctx, 1) && duk_is_object(ctx, 2) && duk_is_null_or_undefined(ctx, 3))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    ret._11 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    ret._12 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "z");
                    ret._13 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);

                    duk_get_prop_string(ctx, 1, "x");
                    ret._21 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "y");
                    ret._22 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "z");
                    ret._23 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);

                    duk_get_prop_string(ctx, 2, "x");
                    ret._31 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 2, "y");
                    ret._32 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 2, "z");
                    ret._33 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else if (duk_is_object(ctx, 0) && duk_is_object(ctx, 1) && duk_is_object(ctx, 2) && duk_is_object(ctx, 3))
                {
                    duk_get_prop_string(ctx, 0, "x");
                    ret._11 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "y");
                    ret._12 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "z");
                    ret._13 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 0, "w");
                    ret._14 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);

                    duk_get_prop_string(ctx, 1, "x");
                    ret._21 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "y");
                    ret._22 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "z");
                    ret._23 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 1, "w");
                    ret._24 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);

                    duk_get_prop_string(ctx, 2, "x");
                    ret._31 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 2, "y");
                    ret._32 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 2, "z");
                    ret._33 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 2, "w");
                    ret._34 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);

                    duk_get_prop_string(ctx, 3, "x");
                    ret._41 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 3, "y");
                    ret._42 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 3, "z");
                    ret._43 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                    duk_get_prop_string(ctx, 3, "w");
                    ret._44 = (float)duk_to_number(ctx, -1);
                    duk_pop(ctx);
                }
                else
                {
                    ret._11 = (float)duk_require_number(ctx, 0);
                    ret._12 = (float)duk_require_number(ctx, 1);
                    ret._13 = (float)duk_require_number(ctx, 2);
                    ret._14 = (float)duk_require_number(ctx, 3);
                    ret._21 = (float)duk_require_number(ctx, 4);
                    ret._22 = (float)duk_require_number(ctx, 5);
                    ret._23 = (float)duk_require_number(ctx, 6);
                    ret._24 = (float)duk_require_number(ctx, 7);
                    ret._31 = (float)duk_require_number(ctx, 8);
                    ret._32 = (float)duk_require_number(ctx, 9);
                    ret._33 = (float)duk_require_number(ctx, 10);
                    ret._34 = (float)duk_require_number(ctx, 11);
                    ret._41 = (float)duk_require_number(ctx, 12);
                    ret._42 = (float)duk_require_number(ctx, 13);
                    ret._43 = (float)duk_require_number(ctx, 14);
                    ret._44 = (float)duk_require_number(ctx, 15);
                }

                duk_push_this(ctx);
                duk_push_number(ctx, ret._11); duk_put_prop_string(ctx, -2, "_11");
                duk_push_number(ctx, ret._12); duk_put_prop_string(ctx, -2, "_12");
                duk_push_number(ctx, ret._13); duk_put_prop_string(ctx, -2, "_13");
                duk_push_number(ctx, ret._14); duk_put_prop_string(ctx, -2, "_14");
                duk_push_number(ctx, ret._21); duk_put_prop_string(ctx, -2, "_21");
                duk_push_number(ctx, ret._22); duk_put_prop_string(ctx, -2, "_22");
                duk_push_number(ctx, ret._23); duk_put_prop_string(ctx, -2, "_23");
                duk_push_number(ctx, ret._24); duk_put_prop_string(ctx, -2, "_24");
                duk_push_number(ctx, ret._31); duk_put_prop_string(ctx, -2, "_31");
                duk_push_number(ctx, ret._32); duk_put_prop_string(ctx, -2, "_32");
                duk_push_number(ctx, ret._33); duk_put_prop_string(ctx, -2, "_33");
                duk_push_number(ctx, ret._34); duk_put_prop_string(ctx, -2, "_34");
                duk_push_number(ctx, ret._41); duk_put_prop_string(ctx, -2, "_41");
                duk_push_number(ctx, ret._42); duk_put_prop_string(ctx, -2, "_42");
                duk_push_number(ctx, ret._43); duk_put_prop_string(ctx, -2, "_43");
                duk_push_number(ctx, ret._44); duk_put_prop_string(ctx, -2, "_44");

                return 0;
            }, 16);
            duk_push_object(ctx);

#define JS_THIS_MATRIX \
    duk_push_this(ctx); \
    Matrix v; \
    FLOAT_PROP2(v._11, "_11", -1); \
    FLOAT_PROP2(v._12, "_12", -1); \
    FLOAT_PROP2(v._13, "_13", -1); \
    FLOAT_PROP2(v._14, "_14", -1); \
    FLOAT_PROP2(v._21, "_21", -1); \
    FLOAT_PROP2(v._22, "_22", -1); \
    FLOAT_PROP2(v._23, "_23", -1); \
    FLOAT_PROP2(v._24, "_24", -1); \
    FLOAT_PROP2(v._31, "_31", -1); \
    FLOAT_PROP2(v._32, "_32", -1); \
    FLOAT_PROP2(v._33, "_33", -1); \
    FLOAT_PROP2(v._34, "_34", -1); \
    FLOAT_PROP2(v._41, "_41", -1); \
    FLOAT_PROP2(v._42, "_42", -1); \
    FLOAT_PROP2(v._43, "_43", -1); \
    FLOAT_PROP2(v._44, "_44", -1); \
    duk_pop(ctx);

            // isEqual(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getMatrix(ctx, 0);
                JS_THIS_MATRIX;
                duk_push_boolean(ctx, v == other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "isEqual");

            // add(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getMatrix(ctx, 0);
                JS_THIS_MATRIX;
                newMatrix(ctx, v + other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // sub(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getMatrix(ctx, 0);
                JS_THIS_MATRIX;
                newMatrix(ctx, v - other);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "sub");

            // mul(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (duk_is_number(ctx, 0))
                {
                    auto s = JS_FLOAT(0);
                    JS_THIS_MATRIX;
                    newMatrix(ctx, v * s);
                    return 1;
                }
                else
                {
                    auto other = getMatrix(ctx, 0);
                    JS_THIS_MATRIX;
                    newMatrix(ctx, v * other);
                    return 1;
                }
            }, 1);
            duk_put_prop_string(ctx, -2, "mul");

            // div(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (duk_is_number(ctx, 0))
                {
                    auto s = JS_FLOAT(0);
                    JS_THIS_MATRIX;
                    newMatrix(ctx, v / s);
                    return 1;
                }
                else
                {
                    auto other = getMatrix(ctx, 0);
                    JS_THIS_MATRIX;
                    newMatrix(ctx, v / other);
                    return 1;
                }
            }, 1);
            duk_put_prop_string(ctx, -2, "div");

            // translation()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_41, -1);
                FLOAT_PROP(_42, -1);
                FLOAT_PROP(_43, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_41, _42, _43));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "translation");

            // right()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_11, -1);
                FLOAT_PROP(_12, -1);
                FLOAT_PROP(_13, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_11, _12, _13));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "right");

            // axisX()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_11, -1);
                FLOAT_PROP(_12, -1);
                FLOAT_PROP(_13, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_11, _12, _13));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "axisX");

            // left()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_11, -1);
                FLOAT_PROP(_12, -1);
                FLOAT_PROP(_13, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(-_11, -_12, -_13));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "left");

            // front()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_21, -1);
                FLOAT_PROP(_22, -1);
                FLOAT_PROP(_23, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_21, _22, _23));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "front");

            // axisY()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_21, -1);
                FLOAT_PROP(_22, -1);
                FLOAT_PROP(_23, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_21, _22, _23));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "axisY");

            // back()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_21, -1);
                FLOAT_PROP(_22, -1);
                FLOAT_PROP(_23, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(-_21, -_22, -_23));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "back");

            // up()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_31, -1);
                FLOAT_PROP(_32, -1);
                FLOAT_PROP(_33, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_31, _32, _33));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "up");

            // axisZ()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_31, -1);
                FLOAT_PROP(_32, -1);
                FLOAT_PROP(_33, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(_31, _32, _33));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "axisZ");

            // down()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                FLOAT_PROP(_31, -1);
                FLOAT_PROP(_32, -1);
                FLOAT_PROP(_33, -1);
                duk_pop(ctx);
                newVector3(ctx, Vector3(-_31, -_32, -_33));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "down");

            // transpose()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX;
                newMatrix(ctx, v.Transpose());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "transpose");

            // invert()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX;
                newMatrix(ctx, v.Invert());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "invert");

            // determinant()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX;
                duk_push_number(ctx, (duk_double_t)v.Determinant());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "determinant");

            // Done with the object
            pMatrixPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // lerp(from, to, t)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto from = getMatrix(ctx, 0);
                auto to = getMatrix(ctx, 1);
                auto t = JS_FLOAT(2);
                newMatrix(ctx, Matrix::Lerp(from, to, t));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "lerp");

            // createBillboard(object, cameraPosition, cameraUp)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto object = getVector3(ctx, 0);
                auto cameraPosition = getVector3(ctx, 1);
                auto cameraUp = getVector3(ctx, 2);
                newMatrix(ctx, Matrix::CreateBillboard(object, cameraPosition, cameraUp));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "createBillboard");

            // createConstrainedBillboard(object, cameraPosition, rotateAxis)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto object = getVector3(ctx, 0);
                auto cameraPosition = getVector3(ctx, 1);
                auto rotateAxis = getVector3(ctx, 2);
                newMatrix(ctx, Matrix::CreateConstrainedBillboard(object, cameraPosition, rotateAxis));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "createConstrainedBillboard");

            // createTranslation(position)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto position = getVector3(ctx, 0);
                newMatrix(ctx, Matrix::CreateTranslation(position));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createTranslation");

            // createScale(scales)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto scales = getVector3(ctx, 0);
                newMatrix(ctx, Matrix::CreateScale(scales));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createScale");

            // createRotationX(degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto degrees = JS_FLOAT(0);
                newMatrix(ctx, Matrix::CreateRotationX(OConvertToRadians(degrees)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRotationX");

            // createRotationY(degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto degrees = JS_FLOAT(0);
                newMatrix(ctx, Matrix::CreateRotationY(OConvertToRadians(degrees)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRotationY");

            // createRotationZ(degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto degrees = JS_FLOAT(0);
                newMatrix(ctx, Matrix::CreateRotationZ(OConvertToRadians(degrees)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRotationZ");

            // createFromAxisAngle(axis, degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto axis = getVector3(ctx, 0);
                auto degrees = JS_FLOAT(1);
                newMatrix(ctx, Matrix::CreateFromAxisAngle(axis, OConvertToRadians(degrees)));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "createFromAxisAngle");

            // createPerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto fov = JS_FLOAT(0);
                auto aspectRatio = JS_FLOAT(1);
                auto nearPlane = JS_FLOAT(2);
                auto farPlane = JS_FLOAT(3);
                newMatrix(ctx, Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane));
                return 1;
            }, 4);
            duk_put_prop_string(ctx, -2, "createPerspectiveFieldOfView");

            // createPerspective(width, height, nearPlane, farPlane)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto width = JS_FLOAT(0);
                auto height = JS_FLOAT(1);
                auto nearPlane = JS_FLOAT(2);
                auto farPlane = JS_FLOAT(3);
                newMatrix(ctx, Matrix::CreatePerspective(width, height, nearPlane, farPlane));
                return 1;
            }, 4);
            duk_put_prop_string(ctx, -2, "createPerspective");

            // createPerspectiveOffCenter(left, right, bottom, top, nearPlane, farPlane)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto left = JS_FLOAT(0);
                auto right = JS_FLOAT(1);
                auto bottom = JS_FLOAT(2);
                auto top = JS_FLOAT(3);
                auto nearPlane = JS_FLOAT(4);
                auto farPlane = JS_FLOAT(5);
                newMatrix(ctx, Matrix::CreatePerspectiveOffCenter(left, right, bottom, top, nearPlane, farPlane));
                return 1;
            }, 6);
            duk_put_prop_string(ctx, -2, "createPerspectiveOffCenter");

            // createOrthographic(width, height, zNearPlane, zFarPlane)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto width = JS_FLOAT(0);
                auto height = JS_FLOAT(1);
                auto zNearPlane = JS_FLOAT(2);
                auto zFarPlane = JS_FLOAT(3);
                newMatrix(ctx, Matrix::CreateOrthographic(width, height, zNearPlane, zFarPlane));
                return 1;
            }, 4);
            duk_put_prop_string(ctx, -2, "createOrthographic");

            // createOrthographicOffCenter(left, right, bottom, top, zNearPlane, zFarPlane)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto left = JS_FLOAT(0);
                auto right = JS_FLOAT(1);
                auto bottom = JS_FLOAT(2);
                auto top = JS_FLOAT(3);
                auto zNearPlane = JS_FLOAT(4);
                auto zFarPlane = JS_FLOAT(5);
                newMatrix(ctx, Matrix::CreateOrthographicOffCenter(left, right, bottom, top, zNearPlane, zFarPlane));
                return 1;
            }, 6);
            duk_put_prop_string(ctx, -2, "createOrthographicOffCenter");

            // createLookAt(position, target, up)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto position = getVector3(ctx, 0);
                auto target = getVector3(ctx, 1);
                auto up = getVector3(ctx, 2);
                newMatrix(ctx, Matrix::CreateLookAt(position, target, up));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "createLookAt");

            // createWorld(position, forward, up)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto position = getVector3(ctx, 0);
                auto forward = getVector3(ctx, 1);
                auto up = getVector3(ctx, 2);
                newMatrix(ctx, Matrix::CreateWorld(position, forward, up));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "createWorld");

            // createFromYawPitchRoll(yaw, pitch, roll)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto yaw = JS_FLOAT(0);
                auto pitch = JS_FLOAT(1);
                auto roll = JS_FLOAT(2);
                newMatrix(ctx, Matrix::CreateFromYawPitchRoll(roll, pitch, yaw));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "createFromYawPitchRoll");

            // create2DTranslationZoom(cameraPos, zoom)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto camera = getVector2(ctx, 0);
                auto zoon = JS_FLOAT(1, 1.0f);
                newMatrix(ctx, Matrix::Create2DTranslationZoom(OScreenf, camera, zoon));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "create2DTranslationZoom");

            // Constants
            newMatrix(ctx, Matrix::Identity);
            duk_put_prop_string(ctx, -2, "IDENTITY");

            duk_put_global_string(ctx, "Matrix");
        }
        
        static void newUI(duk_context* ctx, const OUIControlRef& pUIControl)
        {
            duk_push_object(ctx);
            auto ppUIControl = new OUIControlRef(pUIControl);
            duk_push_pointer(ctx, ppUIControl);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pUIPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newUpdater(duk_context* ctx, const OUpdaterRef& pUpdater)
        {
            duk_push_object(ctx);
            auto ppUpdater = new OUpdaterRef(pUpdater);
            duk_push_pointer(ctx, ppUpdater);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pUpdaterPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newTexture(duk_context* ctx, const OTextureRef& pTexture)
        {
            duk_push_object(ctx);
            auto ppTexture = new OTextureRef(pTexture);
            duk_push_pointer(ctx, ppTexture);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pTexturePrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newModel(duk_context* ctx, const OModelRef& pModel)
        {
            duk_push_object(ctx);
            auto ppModel = new OModelRef(pModel);
            duk_push_pointer(ctx, ppModel);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pModelPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newFont(duk_context* ctx, const OFontRef& pFont)
        {
            duk_push_object(ctx);
            auto ppFont = new OFontRef(pFont);
            duk_push_pointer(ctx, ppFont);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pFontPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newShader(duk_context* ctx, const OShaderRef& pShader)
        {
            duk_push_object(ctx);
            auto ppShader = new OShaderRef(pShader);
            duk_push_pointer(ctx, ppShader);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pShaderPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newMusic(duk_context* ctx, const OMusicRef& pMusic)
        {
            duk_push_object(ctx);
            auto ppMusic = new OMusicRef(pMusic);
            duk_push_pointer(ctx, ppMusic);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pMusicPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newSound(duk_context* ctx, const OSoundRef& pSound)
        {
            duk_push_object(ctx);
            auto ppSound = new OSoundRef(pSound);
            duk_push_pointer(ctx, ppSound);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pSoundPrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newSoundInstance(duk_context* ctx, const OSoundInstanceRef& pSoundInstance)
        {
            duk_push_object(ctx);
            auto ppSoundInstance = new OSoundInstanceRef(pSoundInstance);
            duk_push_pointer(ctx, ppSoundInstance);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pSoundInstancePrototype);
            duk_set_prototype(ctx, -2);
        }
        
        static void newTiledMap(duk_context* ctx, const OTiledMapRef& pTiledMap)
        {
            duk_push_object(ctx);
            auto ppTiledMap = new OTiledMapRef(pTiledMap);
            pTiledMap->generateCollisions("collisions");
            duk_push_pointer(ctx, ppTiledMap);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pTiledMapPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newSpriteAnim(duk_context* ctx, const OSpriteAnimRef& pSpriteAnim)
        {
            duk_push_object(ctx);
            auto ppSpriteAnim = new OSpriteAnimRef(pSpriteAnim);
            duk_push_pointer(ctx, ppSpriteAnim);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pSpriteAnimPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newSpriteAnimInstance(duk_context* ctx, const OSpriteAnimInstanceRef& pSpriteAnimInstance)
        {
            duk_push_object(ctx);
            auto ppSpriteAnimInstance = new OSpriteAnimInstanceRef(pSpriteAnimInstance);
            duk_push_pointer(ctx, ppSpriteAnimInstance);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pSpriteAnimInstancePrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newParticleSystem(duk_context* ctx, const OParticleSystemRef& pParticleSystem)
        {
            duk_push_object(ctx);
            auto ppParticleSystem = new OParticleSystemRef(pParticleSystem);
            duk_push_pointer(ctx, ppParticleSystem);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pParticleSystemPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newParticleEmitter(duk_context* ctx, const OEmitterInstance& particleEmitter)
        {
            duk_push_object(ctx);
            auto pParticleEmitter = new OEmitterInstance(particleEmitter);
            duk_push_pointer(ctx, pParticleEmitter);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pParticleEmitterPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newVertexBuffer(duk_context* ctx, const OVertexBufferRef& pVertexBuffer)
        {
            duk_push_object(ctx);
            auto ppVertexBuffer = new OVertexBufferRef(pVertexBuffer);
            duk_push_pointer(ctx, ppVertexBuffer);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pVertexBufferPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newIndexBuffer(duk_context* ctx, const OIndexBufferRef& pIndexBuffer)
        {
            duk_push_object(ctx);
            auto ppIndexBuffer = new OIndexBufferRef(pIndexBuffer);
            duk_push_pointer(ctx, ppIndexBuffer);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pIndexBufferPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newEntity(duk_context* ctx, const OEntityRef& pEntity)
        {
            duk_push_object(ctx);
            auto pppEntity = new OEntityRef(pEntity);
            duk_push_pointer(ctx, pppEntity);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pEntityPrototype);
            duk_set_prototype(ctx, -2);
        }

        class JSComponentType
        {
        public:
            void* pConstructor = nullptr;
            void* pPrototype = nullptr;
            int flags = 0;
            void* p_onCreate = nullptr;
            void* p_onUpdate = nullptr;
            void* p_onRender = nullptr;
            void* p_onRender2d = nullptr;
            void* p_onMessage = nullptr;
            void* p_onMessageBroadcast = nullptr;
            void* p_onAddChild = nullptr;
            void* p_onRemoveChild = nullptr;
            void* p_onTriggerEnter = nullptr;
            void* p_onTriggerLeave = nullptr;
            void* p_onEnable = nullptr;
            void* p_onDisable = nullptr;
            void* p_onDestroy = nullptr;
        };

        using JSComponentTypeRef = std::shared_ptr<JSComponentType>;
        static std::unordered_map<std::string, JSComponentTypeRef> jsComponentTypesByName;
        static std::unordered_map<void*, JSComponentTypeRef> jsComponentTypesByPointer;

        static void createUpdaterBindings()
        {
            auto ctx = pContext;

            // Updater() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto ppUpdater = new OUpdaterRef(OUpdater::create());

                duk_push_this(ctx);
                duk_push_pointer(ctx, ppUpdater);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");

                return 0;
            }, 0);
            duk_push_object(ctx);

            // ~Updater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppUpdater = (OUpdaterRef*)duk_to_pointer(ctx, -1);
                if (ppUpdater)
                {
                    delete ppUpdater;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // update()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUpdater = (OUpdaterRef*)duk_to_pointer(ctx, -1);
                if (ppUpdater)
                {
                    (*ppUpdater)->update();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "update");

            // Done with the object
            pUpdaterPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "Updater");
        }

        static void createTextureBindings()
        {
            auto ctx = pContext;

            // Texture() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~Texture()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    delete ppTexture;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // getSize()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    newVector2(ctx, (*ppTexture)->getSizef());
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "getSize");

            // isRenderTarget()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    duk_push_boolean(ctx, (*ppTexture)->isRenderTarget());
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isRenderTarget");

            // isDynamic()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    duk_push_boolean(ctx, (*ppTexture)->isDynamic());
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isDynamic");

            // clearRenderTarget()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto color = JS_COLOR(0, Color::Black);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->clearRenderTarget(color);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "clearRenderTarget");

            // blur(amount)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto amount = JS_FLOAT(0, 16.0f);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->blur(amount);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "blur");

            // sepia(tone, saturation, sepiaAmount)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto tone = getVector3(ctx, 0, Vector3(1.40f, 1.10f, 0.90f));
                auto saturation = getFloat(ctx, 1, 0);
                auto sepiaAmount = getFloat(ctx, 2, .75f);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->sepia(tone, saturation, sepiaAmount);
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "sepia");

            // crt()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->crt();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "crt");

            // cartoon(tone)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto tone = getVector3(ctx, 0, Vector3(2, 5, 1));
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->cartoon(tone);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "cartoon");

            // vignette(amount)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto amount = JS_FLOAT(0, 0.5f);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->vignette(amount);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "vignette");

            // resizeTarget(size)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto size = getPoint(ctx, 0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->resizeTarget(size);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "resizeTarget");

            // setData(data)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTexture = (OTextureRef*)duk_to_pointer(ctx, -1);
                if (ppTexture)
                {
                    (*ppTexture)->setData(reinterpret_cast<const uint8_t*>(pBuffer));
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setData");

            // Done with the object
            pTexturePrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newTexture(ctx, OGetTexture(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            // createDynamic(size)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto size = getPoint(ctx, 0, Point(1, 1));
                newTexture(ctx, OTexture::createDynamic(size));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createDynamic");

            // createFromData(data, size)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                auto size = getPoint(ctx, 1, Point(1, 1));
                newTexture(ctx, OTexture::createFromData(reinterpret_cast<const uint8_t*>(pBuffer), size));
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "createFromData");

            // createRenderTarget(size)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto size = getPoint(ctx, 0, Point(1, 1));
                newTexture(ctx, OTexture::createRenderTarget(size));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRenderTarget");

            // createScreenRenderTarget()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newTexture(ctx, OTexture::createScreenRenderTarget());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "createScreenRenderTarget");

            duk_put_global_string(ctx, "Texture");
        }

        static void createModelBindings()
        {
            auto ctx = pContext;

            // Model() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~Model()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    delete ppModel;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // getMeshCount()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    duk_push_int(ctx, (*ppModel)->getMeshCount());
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "getMeshCount");

            // getMin()
            duk_push_c_function(ctx, [](duk_context* ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    newVector3(ctx, (*ppModel)->getBoundingBox()[0]);
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "getMin");

            // getMax()
            duk_push_c_function(ctx, [](duk_context* ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    newVector3(ctx, (*ppModel)->getBoundingBox()[1]);
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "getMax");

            // getTexture()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto index = JS_INT(0, 0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    newTexture(ctx, (*ppModel)->getMesh(index)->pTexture);
                }
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "getTexture");

            // getElementCount()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto index = JS_INT(0, 0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    duk_push_int(ctx, (*ppModel)->getMesh(index)->elementCount);
                }
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "getElementCount");

            // getVertexBuffer()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto index = JS_INT(0, 0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    newVertexBuffer(ctx, (*ppModel)->getMesh(index)->pVertexBuffer);
                }
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "getVertexBuffer");

            // getIndexBuffer()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto index = JS_INT(0, 0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    newIndexBuffer(ctx, (*ppModel)->getMesh(index)->pIndexBuffer);
                }
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "getIndexBuffer");

            // setTexture()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto index = JS_INT(0, 0);
                auto pInTexture = JS_TEXTURE(1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    (*ppModel)->getMesh(index)->pTexture = pInTexture;
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setTexture");

            // render()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto transform = JS_MATRIX(0, Matrix::Identity);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppModel = (OModelRef*)duk_to_pointer(ctx, -1);
                if (ppModel)
                {
                    (*ppModel)->render(transform);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "render");

            // Done with the object
            pModelPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newModel(ctx, OGetModel(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            // createFromBatch(batch)
            duk_push_c_function(ctx, [](duk_context* ctx)->duk_ret_t
            {
                std::vector<Model::Batch> batches;
                if (duk_is_array(ctx, 0))
                {
                    auto len = duk_get_length(ctx, 0);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 0, i);
                        if (duk_is_object(ctx, -1))
                        {
                            duk_get_prop_string(ctx, -1, "model");
                            auto pModel = JS_MODEL(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "transform");
                            auto transform = JS_MATRIX(-1, Matrix::Identity);
                            duk_pop(ctx);
                            if (pModel)
                            {
                                batches.push_back({ pModel, transform });
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                newModel(ctx, OModel::createFromBatch(batches));

                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromBatch");

            duk_put_global_string(ctx, "Model");
        }

        static void createFontBindings()
        {
            auto ctx = pContext;

            // Font() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~Font()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppFont = (OFontRef*)duk_to_pointer(ctx, -1);
                if (ppFont)
                {
                    delete ppFont;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // measure()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto text = JS_STRING(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppFont = (OFontRef*)duk_to_pointer(ctx, -1);
                if (ppFont)
                {
                    newVector2(ctx, (*ppFont)->measure(text));
                }
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "measure");

            // Done with the object
            pFontPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newFont(ctx, OGetFont(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            duk_put_global_string(ctx, "Font");
        }

        static void createShaderBindings()
        {
            auto ctx = pContext;

            // Shader() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~Shader()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppShader = (OShaderRef*)duk_to_pointer(ctx, -1);
                if (ppShader)
                {
                    delete ppShader;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // setNumber()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto name = JS_STRING(0);
                auto value = JS_FLOAT(1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppShader = (OShaderRef*)duk_to_pointer(ctx, -1);
                if (ppShader)
                {
                    (*ppShader)->setFloat(name, value);
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setNumber");

            // setVector2()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto name = JS_STRING(0);
                auto value = JS_VECTOR2(1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppShader = (OShaderRef*)duk_to_pointer(ctx, -1);
                if (ppShader)
                {
                    (*ppShader)->setVector2(name, value);
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setVector2");

            // setVector3()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto name = JS_STRING(0);
                auto value = JS_VECTOR3(1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppShader = (OShaderRef*)duk_to_pointer(ctx, -1);
                if (ppShader)
                {
                    (*ppShader)->setVector3(name, value);
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setVector3");

            // setVector4()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto name = JS_STRING(0);
                auto value = JS_VECTOR4(1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppShader = (OShaderRef*)duk_to_pointer(ctx, -1);
                if (ppShader)
                {
                    (*ppShader)->setVector4(name, value);
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setVector4");

            // setMatrix()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto name = JS_STRING(0);
                auto value = JS_MATRIX(1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppShader = (OShaderRef*)duk_to_pointer(ctx, -1);
                if (ppShader)
                {
                    (*ppShader)->setMatrix(name, value);
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setMatrix");

            // Done with the object
            pShaderPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newShader(ctx, OGetShader(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            duk_put_global_string(ctx, "Shader");
        }

        static void createMusicBindings()
        {
            auto ctx = pContext;

            // Music() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~Music()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    delete ppMusic;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // setVolume(volume)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto volume = JS_FLOAT(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    (*ppMusic)->setVolume(volume);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setVolume");

            // play()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    (*ppMusic)->play(JS_BOOL(0, true));
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    (*ppMusic)->stop();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    (*ppMusic)->pause();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // resume()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    (*ppMusic)->resume();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "resume");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    duk_push_boolean(ctx, (*ppMusic)->isPlaying());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // isPaused()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppMusic = (OMusicRef*)duk_to_pointer(ctx, -1);
                if (ppMusic)
                {
                    duk_push_boolean(ctx, (*ppMusic)->isPaused());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPaused");

            // Done with the object
            pMusicPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newMusic(ctx, OGetMusic(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            duk_put_global_string(ctx, "Music");
        }

        static void createSoundBindings()
        {
            auto ctx = pContext;

            // Sound() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~Sound()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppSound = (OSoundRef*)duk_to_pointer(ctx, -1);
                if (ppSound)
                {
                    delete ppSound;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // play()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto volume = JS_FLOAT(0, 1);
                auto balance = JS_FLOAT(1);
                auto pitch = JS_FLOAT(2, 1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSound = (OSoundRef*)duk_to_pointer(ctx, -1);
                if (ppSound)
                {
                    (*ppSound)->play(volume, balance, pitch);
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSound = (OSoundRef*)duk_to_pointer(ctx, -1);
                if (ppSound)
                {
                    (*ppSound)->stop();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // setMaxInstance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto maxInstances = JS_INT(0, -1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSound = (OSoundRef*)duk_to_pointer(ctx, -1);
                if (ppSound)
                {
                    (*ppSound)->setMaxInstance(maxInstances);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setMaxInstance");

            // createInstance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSound = (OSoundRef*)duk_to_pointer(ctx, -1);
                if (ppSound)
                {
                    newSoundInstance(ctx, (*ppSound)->createInstance());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "createInstance");

            // Done with the object
            pSoundPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newSound(ctx, OGetSound(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            // createFromData(f)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                auto sampleCount = JS_INT(1);
                auto channelCount = JS_INT(2);
                auto sampleRate = JS_INT(3);
                newSound(ctx, OSound::createFromData(reinterpret_cast<const float*>(pBuffer), sampleCount, channelCount, sampleRate));
                return 1;
            }, 4);
            duk_put_prop_string(ctx, -2, "createFromData");

            duk_put_global_string(ctx, "Sound");
        }

        static void createSoundInstanceBindings()
        {
            auto ctx = pContext;

            // SoundInstance() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~SoundInstance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    delete ppSoundInstance;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // play()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->play();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "play");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->pause();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->stop();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    duk_push_boolean(ctx, (*ppSoundInstance)->isPlaying());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // isPaused()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    duk_push_boolean(ctx, (*ppSoundInstance)->isPaused());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPaused");

            // getLoop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    duk_push_boolean(ctx, (*ppSoundInstance)->getLoop());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getLoop");

            // getVolume()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppSoundInstance)->getVolume());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getVolume");

            // getBalance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppSoundInstance)->getBalance());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getBalance");

            // getPitch()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppSoundInstance)->getPitch());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getPitch");

            // setLoop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto loop = JS_BOOL(0, true);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->setLoop(loop);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setLoop");

            // setVolume()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto value = JS_FLOAT(0, 1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->setVolume(value);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setVolume");

            // setPitch()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto value = JS_FLOAT(0, 1);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->setPitch(value);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setPitch");

            // setBalance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto value = JS_FLOAT(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSoundInstance = (OSoundInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSoundInstance)
                {
                    (*ppSoundInstance)->setBalance(value);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setBalance");

            // Done with the object
            pSoundInstancePrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "SoundInstance");
        }

        static void createTiledMapBindings()
        {
            auto ctx = pContext;

            // TiledMap()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR;
            }, 0);
            duk_push_object(ctx);

            // ~TiledMap()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    delete ppTiledMap;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // render()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_object(ctx, 0))
                    {
                        auto rect = JS_iRECT(0);
                        (*ppTiledMap)->render(rect);
                    }
                    else
                    {
                        (*ppTiledMap)->render();
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "render");

            // renderLayer(name/index, rect)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        auto pLayer = (*ppTiledMap)->getLayer(layerName);
                        if (pLayer)
                        {
                            if (duk_is_object(ctx, 1))
                            {
                                auto rect = JS_iRECT(1);
                                (*ppTiledMap)->renderLayer(rect, pLayer);
                            }
                            else
                            {
                                (*ppTiledMap)->renderLayer(pLayer);
                            }
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_UINT(0);
                        if (duk_is_object(ctx, 1))
                        {
                            auto rect = JS_iRECT(1);
                            (*ppTiledMap)->renderLayer(rect, layer);
                        }
                        else
                        {
                            (*ppTiledMap)->renderLayer(layer);
                        }
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "renderLayer");

            // getSize()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    newVector2(ctx, Vector2(
                        (float)(*ppTiledMap)->getWidth(),
                        (float)(*ppTiledMap)->getHeight()));
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getSize");

            // getTileSize
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppTiledMap)->getTileSize());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getTileSize");

            // getLayerCount
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppTiledMap)->getLayerCount());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getLayerCount");

            // setFilter
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    (*ppTiledMap)->setFiltering((onut::sample::Filtering)JS_UINT(0));
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setFilter");

            // getOpacity
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layerName));
                        if (pLayer)
                        {
                            duk_push_number(ctx, (duk_double_t)pLayer->opacity);
                            return 1;
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_INT(0);
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layer));
                        if (pLayer)
                        {
                            duk_push_number(ctx, (duk_double_t)pLayer->opacity);
                            return 1;
                        }
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "getOpacity");

            // setOpacity
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layerName));
                        if (pLayer)
                        {
                            auto newOpacity = JS_FLOAT(1, 1);
                            if (newOpacity != pLayer->opacity)
                            {
                                pLayer->opacity = newOpacity;
                                (*ppTiledMap)->dirtyChunks(pLayer);
                            }
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_INT(0);
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layer));
                        if (pLayer)
                        {
                            auto newOpacity = JS_FLOAT(1, 1);
                            if (newOpacity != pLayer->opacity)
                            {
                                pLayer->opacity = newOpacity;
                                (*ppTiledMap)->dirtyChunks(pLayer);
                            }
                        }
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "setOpacity");

            // getTileAt
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        auto x = JS_INT(1);
                        auto y = JS_INT(2);
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layerName));
                        if (pLayer)
                        {
                            duk_push_uint(ctx, (*ppTiledMap)->getTileAt(pLayer, x, y));
                            return 1;
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_INT(0);
                        auto x = JS_INT(1);
                        auto y = JS_INT(2);
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layer));
                        if (pLayer)
                        {
                            duk_push_uint(ctx, (*ppTiledMap)->getTileAt(pLayer, x, y));
                            return 1;
                        }
                    }
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "getTileAt");

            // setTileAt
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        auto x = JS_INT(1);
                        auto y = JS_INT(2);
                        auto tileId = JS_UINT(3);
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layerName));
                        if (pLayer)
                        {
                            (*ppTiledMap)->setTileAt(pLayer, x, y, tileId);
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_INT(0);
                        auto x = JS_INT(1);
                        auto y = JS_INT(2);
                        auto tileId = JS_UINT(3);
                        auto pLayer = dynamic_cast<OTiledMap::TileLayer*>((*ppTiledMap)->getLayer(layer));
                        if (pLayer)
                        {
                            (*ppTiledMap)->setTileAt(pLayer, x, y, tileId);
                        }
                    }
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "setTileAt");

            // getObjectCount
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        auto pLayer = dynamic_cast<OTiledMap::ObjectLayer*>((*ppTiledMap)->getLayer(layerName));
                        if (pLayer)
                        {
                            duk_push_uint(ctx, (duk_uint_t)pLayer->objectCount);
                            return 1;
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_INT(0);
                        auto pLayer = dynamic_cast<OTiledMap::ObjectLayer*>((*ppTiledMap)->getLayer(layer));
                        if (pLayer)
                        {
                            duk_push_uint(ctx, (duk_uint_t)pLayer->objectCount);
                            return 1;
                        }
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "getObjectCount");

            // getObject
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    OTiledMap::ObjectLayer* pLayer = nullptr;
                    if (duk_is_string(ctx, 0))
                    {
                        auto layerName = JS_STRING(0, "");
                        pLayer = dynamic_cast<OTiledMap::ObjectLayer*>((*ppTiledMap)->getLayer(layerName));
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto layer = JS_INT(0);
                        pLayer = dynamic_cast<OTiledMap::ObjectLayer*>((*ppTiledMap)->getLayer(layer));
                    }
                    if (pLayer)
                    {
                        int index = -1;
                        if (duk_is_string(ctx, 1))
                        {
                            auto name = JS_STRING(1);
                            for (int i = 0; i < (int)pLayer->objectCount; ++i)
                            {
                                if (pLayer->pObjects[i].name == name)
                                {
                                    index = i;
                                    break;
                                }
                            }
                        }
                        else if (duk_is_number(ctx, 1))
                        {
                            index = JS_INT(1, 0);
                        }
                        if (index >= 0 && index < (int)pLayer->objectCount)
                        {
                            const auto& object = pLayer->pObjects[index];
                            duk_push_object(ctx);
                            
                            newVector2(ctx, object.position);
                            duk_put_prop_string(ctx, -2, "position");
                            newVector2(ctx, object.size);
                            duk_put_prop_string(ctx, -2, "size");
                            duk_push_uint(ctx, (duk_uint_t)object.id);
                            duk_put_prop_string(ctx, -2, "id");
                            duk_push_string(ctx, object.name.c_str());
                            duk_put_prop_string(ctx, -2, "name");
                            duk_push_string(ctx, object.type.c_str());
                            duk_put_prop_string(ctx, -2, "type");
                            duk_push_object(ctx);
                            for (const auto& kv : object.properties)
                            {
                                duk_push_string(ctx, kv.second.c_str());
                                duk_put_prop_string(ctx, -2, kv.first.c_str());
                            }
                            duk_put_prop_string(ctx, -2, "properties");
                            return 1;
                        }
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "getObject");

            // Perform 2d collisions on the tiledmap
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        auto pTiles = pTiledMap->getCollisionTiles();
                        if (pTiles)
                        {
                            auto from = JS_VECTOR2(0);
                            auto to = JS_VECTOR2(1);
                            auto size = JS_VECTOR2(2);
                            auto ret = onut::tilesCollision(from, to, size, pTiles, pTiledMap->getWidth(), pTiledMap->getHeight(), pTiledMap->getTileSize());
                            newVector2(ctx, ret);
                            return 1;
                        }
                    }
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "collision");

            // Set a tile to be passable or not
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        auto pTiles = pTiledMap->getCollisionTiles();
                        if (pTiles)
                        {
                            auto x = JS_INT(0, -1);
                            auto y = JS_INT(1, -1);
                            auto passable = JS_BOOL(2, true);
                            if (x >= 0 && x < pTiledMap->getWidth() && y >= 0 && y < pTiledMap->getHeight())
                            {
                                pTiles[y * pTiledMap->getWidth() + x] = passable ? 1.0f : 0.0f;
                                pTiledMap->resetPath();
                            }
                        }
                    }
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "setPassable");

            // Get a tile to be passable or not
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        auto pTiles = pTiledMap->getCollisionTiles();
                        if (pTiles)
                        {
                            auto x = JS_INT(0, -1);
                            auto y = JS_INT(1, -1);
                            if (x >= 0 && x < pTiledMap->getWidth() && y >= 0 && y < pTiledMap->getHeight())
                            {
                                duk_push_boolean(ctx, pTiles[y * pTiledMap->getWidth() + x] == 0.0f ? 0 : 1);
                                return 1;
                            }
                        }
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "getPassable");

            // Set a tile to be passable or not
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        auto pTiles = pTiledMap->getCollisionTiles();
                        if (pTiles)
                        {
                            auto x = JS_INT(0, -1);
                            auto y = JS_INT(1, -1);
                            auto tileCost = JS_FLOAT(2, 1.0f);
                            if (x >= 0 && x < pTiledMap->getWidth() && y >= 0 && y < pTiledMap->getHeight())
                            {
                                pTiles[y * pTiledMap->getWidth() + x] = tileCost;
                                pTiledMap->resetPath();
                            }
                        }
                    }
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "setTileCost");

            // Get a tile to be passable or not
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        auto pTiles = pTiledMap->getCollisionTiles();
                        if (pTiles)
                        {
                            auto x = JS_INT(0, -1);
                            auto y = JS_INT(1, -1);
                            if (x >= 0 && x < pTiledMap->getWidth() && y >= 0 && y < pTiledMap->getHeight())
                            {
                                duk_push_number(ctx, (duk_double_t)pTiles[y * pTiledMap->getWidth() + x]);
                                return 1;
                            }
                        }
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "getTileCost");

            // Get a path
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                if (duk_is_number(ctx, 0) &&
                    duk_is_number(ctx, 1) &&
                    duk_is_number(ctx, 2) &&
                    duk_is_number(ctx, 3))
                {
                    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                    auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                    if (ppTiledMap)
                    {
                        duk_pop(ctx);
                        auto pTiledMap = ppTiledMap->get();
                        if (pTiledMap)
                        {
                            static OTiledMap::Path path;
                            pTiledMap->getPath(Point(JS_INT(0), JS_INT(1)), Point(JS_INT(2), JS_INT(3)), path, JS_INT(4, OTiledMap::PATH_ALLOW_DIAGONAL | OTiledMap::PATH_CROSS_CORNERS));
                            auto arr_idx = duk_push_array(ctx);
                            for (size_t i = 0; i < path.size(); ++i)
                            {
                                const auto& pos = path[i];
                                newVector2(ctx, Vector2((float)pos.x, (float)pos.y));
                                duk_put_prop_index(ctx, arr_idx, i);
                            }
                            return 1;
                        }
                    }
                }
                return 0;
            }, 5);
            duk_put_prop_string(ctx, -2, "getPath");

            // Get a path with cost
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                if (duk_is_number(ctx, 0) &&
                    duk_is_number(ctx, 1) &&
                    duk_is_number(ctx, 2) &&
                    duk_is_number(ctx, 3))
                {
                    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                    auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                    if (ppTiledMap)
                    {
                        duk_pop(ctx);
                        auto pTiledMap = ppTiledMap->get();
                        if (pTiledMap)
                        {
                            auto path = pTiledMap->getPathWithCost(Point(JS_INT(0), JS_INT(1)), Point(JS_INT(2), JS_INT(3)), JS_INT(4, OTiledMap::PATH_ALLOW_DIAGONAL | OTiledMap::PATH_CROSS_CORNERS));
                            duk_push_object(ctx);
                            auto arr_idx = duk_push_array(ctx);
                            for (size_t i = 0; i < path.path.size(); ++i)
                            {
                                const auto& pos = path.path[i];
                                newVector2(ctx, Vector2((float)pos.x, (float)pos.y));
                                duk_put_prop_index(ctx, arr_idx, i);
                            }
                            duk_put_prop_string(ctx, -2, "path");
                            duk_push_number(ctx, path.cost);
                            duk_put_prop_string(ctx, -2, "cost");
                            return 1;
                        }
                    }
                }
                return 0;
            }, 5);
            duk_put_prop_string(ctx, -2, "getPathWithCost");

            // getLayerIndex
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        duk_push_int(ctx, (duk_int_t)pTiledMap->getLayerIndex(JS_STRING(0, "")));
                        return 1;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "getLayerIndex");

            // addLayer
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        pTiledMap->addLayer(JS_STRING(0));
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "addLayer");

            // addTileSet
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto pTiledMap = ppTiledMap->get();
                    if (pTiledMap)
                    {
                        pTiledMap->addTileSet(JS_TEXTURE(0), "");
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "addTileSet");

            // Done with the object
            pTiledMapPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newTiledMap(ctx, OTiledMap::create(JS_INT(0), JS_INT(1), JS_INT(2)));
                return 1;
            }, 3);
            duk_put_prop_string(ctx, -2, "create");

            // create(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newTiledMap(ctx, OGetTiledMap(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            duk_put_global_string(ctx, "TiledMap");
        }

        static void createSpriteAnimBindings()
        {
            auto ctx = pContext;

            // SpriteAnim()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR;
            }, 0);
            duk_push_object(ctx);

            // ~SpriteAnim()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppSpriteAnim = (OSpriteAnimRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnim)
                {
                    delete ppSpriteAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // createInstance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnim = (OSpriteAnimRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnim)
                {
                    newSpriteAnimInstance(ctx, OMake<OSpriteAnimInstance>(*ppSpriteAnim));
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "createInstance");

            // Done with the object
            pSpriteAnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto prev = oGenerateMipmaps;
                oGenerateMipmaps = JS_BOOL(1, true);
                newSpriteAnim(ctx, OGetSpriteAnim(duk_get_string(ctx, 0)));
                oGenerateMipmaps = prev;
                return 1;
            }, 2);
            duk_put_prop_string(ctx, -2, "createFromFile");

            duk_put_global_string(ctx, "SpriteAnim");
        }

        static void createSpriteAnimInstanceBindings()
        {
            auto ctx = pContext;

            // SpriteAnimInstance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR;
            }, 0);
            duk_push_object(ctx);

            // ~SpriteAnimInstance()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    delete ppSpriteAnimInstance;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // play
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto animName = JS_STRING(0);
                auto fps = JS_FLOAT(1, 0.f);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto pSpriteAnimInstance = *ppSpriteAnimInstance;
                    if (((pSpriteAnimInstance->getCurrentAnim() &&
                        pSpriteAnimInstance->getCurrentAnim()->name != animName) || !pSpriteAnimInstance->isPlaying()))
                    {
                        pSpriteAnimInstance->play(animName, fps);
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "play");

            // forcePlay
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto animName = JS_STRING(0);
                auto fps = JS_FLOAT(1, 0.f);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto pSpriteAnimInstance = *ppSpriteAnimInstance;
                    pSpriteAnimInstance->play(animName, fps);
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "forcePlay");

            // playBackward
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto animName = JS_STRING(0);
                auto fps = JS_FLOAT(1, 0.f);
                auto force = JS_BOOL(2, false);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto pSpriteAnimInstance = *ppSpriteAnimInstance;
                    if (force || (pSpriteAnimInstance->getCurrentAnim() &&
                        pSpriteAnimInstance->getCurrentAnim()->name != animName))
                    {
                        pSpriteAnimInstance->playBackward(animName, fps);
                    }
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playBackward");

            // queue
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto animName = JS_STRING(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    (*ppSpriteAnimInstance)->queueAnim(animName);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "queue");

            // stop
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    (*ppSpriteAnimInstance)->stop(false);
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndReset
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    (*ppSpriteAnimInstance)->stop(true);
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndReset");

            // isPlaying
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->isPlaying();
                    duk_push_boolean(ctx, ret);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // getFrameId
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->getFrameId();
                    duk_push_int(ctx, ret);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getFrameId");

            // getSpriteAnim
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->getDefinition();
                    newSpriteAnim(ctx, ret);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getSpriteAnim");

            // getCurrentAnim
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->getCurrentAnim();
                    duk_push_string(ctx, ret->name.c_str());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getCurrentAnim");

            // getTexture
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->getTexture();
                    newTexture(ctx, ret);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getTexture");

            // getUVs
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->getUVs();
                    newVector4(ctx, ret);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getUVs");

            // getOrigin
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    auto ret = (*ppSpriteAnimInstance)->getOrigin();
                    newVector2(ctx, ret);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getOrigin");

            // getSize
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    const auto& size = (*ppSpriteAnimInstance)->getDefinition()->getSize();
                    newVector2(ctx, Vector2((float)size.x, (float)size.y));
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getSize");

            // setSpeed
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto fps = JS_FLOAT(0, 0.f);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    (*ppSpriteAnimInstance)->setSpeed(fps);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setSpeed");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto pUpdater = JS_UPDATER(0);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppSpriteAnimInstance = (OSpriteAnimInstanceRef*)duk_to_pointer(ctx, -1);
                if (ppSpriteAnimInstance)
                {
                    (*ppSpriteAnimInstance)->setUpdater(pUpdater);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pSpriteAnimInstancePrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "SpriteAnimInstance");
        }

        static void createParticleSystemBindings()
        {
            auto ctx = pContext;

            // ParticleSystem()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR;
            }, 0);
            duk_push_object(ctx);

            // ~ParticleSystem()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppParticleSystem = (OParticleSystemRef*)duk_to_pointer(ctx, -1);
                if (ppParticleSystem)
                {
                    delete ppParticleSystem;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // emit()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto position = JS_VECTOR3(0);
                auto dir = JS_VECTOR3(1, Vector3::UnitZ);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppParticleSystem = (OParticleSystemRef*)duk_to_pointer(ctx, -1);
                if (ppParticleSystem)
                {
                    newParticleEmitter(ctx, oParticleSystemManager->emit(*ppParticleSystem, position, dir));
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "emit");

            // Done with the object
            pParticleSystemPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createFromFile(filename)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newParticleSystem(ctx, OGetParticleSystem(duk_get_string(ctx, 0)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFromFile");

            duk_put_global_string(ctx, "ParticleSystem");
        }

        static void createParticleEmitterBindings()
        {
            auto ctx = pContext;

            // ParticleEmitter()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pEmitter = new OEmitterInstance();

                duk_push_this(ctx);
                duk_push_pointer(ctx, pEmitter);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                duk_push_heapptr(ctx, pParticleEmitterPrototype);
                duk_set_prototype(ctx, -2);
                return 0;
            }, 0);
            duk_push_object(ctx);

            // ~ParticleEmitter()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    delete pEmitter;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // setTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto pos = JS_VECTOR3(0);
                auto dir = JS_VECTOR3(1, Vector3::UnitY);
                auto up = JS_VECTOR3(2, Vector3::UnitZ);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    pEmitter->setTransform(pos, dir, up);
                }
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "setTransform");

            // setTransformMatrix()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto matrix = JS_MATRIX(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    pEmitter->setTransform(matrix);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setTransformMatrix");

            // setRenderEnabled()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto enabled = JS_BOOL(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    pEmitter->setRenderEnabled(enabled);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setRenderEnabled");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    pEmitter->stop();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // render()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    pEmitter->render();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "render");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    duk_push_boolean(ctx, pEmitter->isPlaying());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // isAlive()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pEmitter = (OEmitterInstance*)duk_to_pointer(ctx, -1);
                if (pEmitter)
                {
                    duk_push_boolean(ctx, pEmitter->isAlive());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isAlive");

            // Done with the object
            pParticleEmitterPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "ParticleEmitter");
        }

#define JS_THIS_BOOL_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimBool*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createBoolAnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimBool();
                *pAnim = JS_BOOL(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimBool*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto& val = pAnim->get();
                duk_push_boolean(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto val = JS_BOOL(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto from = JS_BOOL(0);
                auto to = JS_BOOL(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto to = JS_BOOL(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto from = JS_BOOL(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimBool::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            bool value = 0;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            if (duk_is_boolean(ctx, -1)) value = duk_to_boolean(ctx, -1) ? true : false;
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimBool::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimBool::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_BOOL_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pBoolAnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "BoolAnim");
        }

#define JS_THIS_NUMBER_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimFloat*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createNumberAnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimFloat();
                if (duk_is_null_or_undefined(ctx, 0))
                {
                    *pAnim = 0;
                }
                else if (duk_is_number(ctx, 0))
                {
                    *pAnim = (float)duk_to_number(ctx, 0);
                }
                else
                {
                    return DUK_RET_SYNTAX_ERROR;
                }

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimFloat*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto& val = pAnim->get();
                duk_push_number(ctx, (duk_double_t)val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto val = JS_FLOAT(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto from = JS_FLOAT(0);
                auto to = JS_FLOAT(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto to = JS_FLOAT(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto from = JS_FLOAT(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimFloat::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            float value = 0;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            if (duk_is_number(ctx, -1)) value = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimFloat::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimFloat::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_NUMBER_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pNumberAnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "NumberAnim");
        }

#define JS_THIS_VECTOR2_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimVector2*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createVector2AnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimVector2();
                *pAnim = JS_VECTOR2(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimVector2*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto& val = pAnim->get();
                newVector2(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto val = JS_VECTOR2(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto from = JS_VECTOR2(0);
                auto to = JS_VECTOR2(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto to = JS_VECTOR2(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto from = JS_VECTOR2(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimVector2::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            Vector2 value;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            value = JS_VECTOR2(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimVector2::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimVector2::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR2_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pVector2AnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "Vector2Anim");
        }

#define JS_THIS_VECTOR3_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimVector3*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createVector3AnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimVector3();
                *pAnim = JS_VECTOR3(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimVector3*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto& val = pAnim->get();
                newVector3(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto val = JS_VECTOR3(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto from = JS_VECTOR3(0);
                auto to = JS_VECTOR3(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto to = JS_VECTOR3(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto from = JS_VECTOR3(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimVector3::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            Vector3 value;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            value = JS_VECTOR3(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimVector3::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimVector3::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR3_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pVector3AnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "Vector3Anim");
        }

#define JS_THIS_VECTOR4_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimVector4*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createVector4AnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimVector4();
                *pAnim = JS_VECTOR4(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimVector4*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto& val = pAnim->get();
                newVector4(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto val = JS_VECTOR4(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto from = JS_VECTOR4(0);
                auto to = JS_VECTOR4(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto to = JS_VECTOR4(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto from = JS_VECTOR4(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimVector4::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            Vector4 value;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            value = JS_VECTOR4(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimVector4::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimVector4::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_VECTOR4_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pVector4AnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "Vector4Anim");
        }

#define JS_THIS_RECT_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimRect*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createRectAnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimRect();
                *pAnim = JS_RECT(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimRect*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto& val = pAnim->get();
                newRect(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto val = JS_RECT(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto from = JS_RECT(0);
                auto to = JS_RECT(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto to = JS_RECT(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto from = JS_RECT(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimRect::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            Rect value;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            value = JS_RECT(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimRect::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimRect::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_RECT_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pRectAnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "RectAnim");
        }

#define JS_THIS_COLOR_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimColor*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createColorAnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimColor();
                *pAnim = JS_COLOR(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimColor*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto& val = pAnim->get();
                newColor(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto val = JS_COLOR(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto from = JS_COLOR(0);
                auto to = JS_COLOR(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto to = JS_COLOR(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto from = JS_COLOR(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimColor::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            Color value;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            value = JS_COLOR(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimColor::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimColor::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pColorAnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "ColorAnim");
        }

#define JS_THIS_MATRIX_ANIM \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
    auto pAnim = (OAnimMatrix*)duk_to_pointer(ctx, -1); \
    if (!pAnim) return 0; \
    duk_pop(ctx)

        static void createMatrixAnimBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pAnim = new OAnimMatrix();
                *pAnim = JS_MATRIX(0);

                auto pUpdater = JS_UPDATER(1);
                if (pUpdater) pAnim->setUpdater(pUpdater);

                duk_push_this(ctx);
                duk_push_pointer(ctx, pAnim);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pAnim = (OAnimMatrix*)duk_to_pointer(ctx, -1);
                if (pAnim)
                {
                    delete pAnim;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // get()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto& val = pAnim->get();
                newMatrix(ctx, val);
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "get");

            // set(value)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto val = JS_MATRIX(0);
                *pAnim = val;
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "set");

            // playSingle(from, to, duration, tween, loop, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto from = JS_MATRIX(0);
                auto to = JS_MATRIX(1);
                auto duration = JS_FLOAT(2);
                auto tween = (onut::Tween)JS_UINT(3, (unsigned int)OTweenLinear);
                auto loop = (onut::LoopType)JS_UINT(4, (unsigned int)ODontLoop);
                auto callback = getFunction(ctx, 5);
                if (callback)
                {
                    pAnim->play(from, to, duration, tween, loop, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->play(from, to, duration, tween, loop);
                }
                return 0;
            }, 6);
            duk_put_prop_string(ctx, -2, "playSingle");

            // queue(to, duration, tween, callback)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto to = JS_MATRIX(0);
                auto duration = JS_FLOAT(1);
                auto tween = (onut::Tween)JS_UINT(2, (unsigned int)OTweenLinear);
                auto callback = getFunction(ctx, 3);
                if (callback)
                {
                    pAnim->queue(to, duration, tween, [ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                else
                {
                    pAnim->queue(to, duration, tween);
                }
                return 0;
            }, 4);
            duk_put_prop_string(ctx, -2, "queue");

            // play(loop)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto loop = (onut::LoopType)JS_UINT(0, (unsigned int)ODontLoop);
                pAnim->play(loop);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "play");

            // stop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                pAnim->stop(false);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stop");

            // stopAndGoToEnd()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                pAnim->stop(true);
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "stopAndGoToEnd");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                pAnim->pause();
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                duk_push_boolean(ctx, pAnim->isPlaying());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // playKeyFrames(from, keyFrames, loop);
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto from = JS_MATRIX(0);
                auto loop = (onut::LoopType)JS_UINT(2, (unsigned int)ODontLoop);
                OAnimMatrix::KeyFrames keyframes;
                if (duk_is_array(ctx, 1))
                {
                    auto len = duk_get_length(ctx, 1);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 1, i);
                        if (duk_is_object(ctx, -1))
                        {
                            Matrix value;
                            float duration = 0;
                            Tween tween = OTweenLinear;
                            duk_get_prop_string(ctx, -1, "value");
                            value = JS_MATRIX(-1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "duration");
                            if (duk_is_number(ctx, -1)) duration = (float)duk_to_number(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "tween");
                            if (duk_is_number(ctx, -1)) tween = (onut::Tween)duk_to_uint(ctx, -1);
                            duk_pop(ctx);
                            duk_get_prop_string(ctx, -1, "callback");
                            auto callback = getFunction(ctx, -1);
                            duk_pop(ctx);
                            if (callback)
                            {
                                OAnimMatrix::KeyFrame keyFrame(value, duration, tween, [ctx, callback]
                                {
                                    if (callback->push(ctx)) callback->call(ctx, 0);
                                });
                                keyframes.push_back(keyFrame);
                            }
                            else
                            {
                                OAnimMatrix::KeyFrame keyFrame(value, duration, tween, nullptr);
                                keyframes.push_back(keyFrame);
                            }
                        }
                        duk_pop(ctx);
                    }
                }
                pAnim->playKeyFrames(from, keyframes, loop);
                return 0;
            }, 3);
            duk_put_prop_string(ctx, -2, "playKeyFrames");

            // setUpdater()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_MATRIX_ANIM;
                auto pUpdater = JS_UPDATER(0);
                pAnim->setUpdater(pUpdater);
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setUpdater");

            // Done with the object
            pMatrixAnimPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "MatrixAnim");
        }

        static void createBinaryFileReaderBindings()
        {
            auto ctx = pContext;

            // BinaryFileReader() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto filename = JS_STRING(0);
                auto foundFilename = oContentManager->findResourceFile(filename);
                if (foundFilename.empty()) return DUK_RET_URI_ERROR;
                FILE* pFile = nullptr;
#if defined(WIN32)
                fopen_s(&pFile, foundFilename.c_str(), "rb");
#else
                pFile = fopen(foundFilename.c_str(), "rb");
#endif
                if (!pFile) return DUK_RET_URI_ERROR;

                duk_push_this(ctx);
                duk_push_pointer(ctx, pFile);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");

                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~BinaryFileReader()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    fclose(pFile);
                    pFile = nullptr;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

#define READ_INT_TYPE(__type__, __fn__) \
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t \
            { \
                duk_push_this(ctx); \
                duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
                auto pFile = (FILE*)duk_to_pointer(ctx, -1); \
                if (pFile) \
                 { \
                    __type__ val; \
                    auto readret = fread(&val, sizeof(__type__), 1, pFile); \
                    (void)readret; \
                    duk_push_number(ctx, (duk_double_t)val); \
                    return 1; \
                } \
                return 0; \
            }, 0); \
            duk_put_prop_string(ctx, -2, #__fn__);
            READ_INT_TYPE(int8_t, readInt8);
            READ_INT_TYPE(int16_t, readInt16);
            READ_INT_TYPE(int32_t, readInt32);
            READ_INT_TYPE(uint8_t, readUInt8);
            READ_INT_TYPE(uint16_t, readUInt16);
            READ_INT_TYPE(uint32_t, readUInt32);
            READ_INT_TYPE(float, readFloat);

            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    uint8_t val;
                    auto readret = fread(&val, sizeof(uint8_t), 1, pFile);
                    (void)readret;
                    duk_push_boolean(ctx, (duk_bool_t)val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readBool");

            // readString
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    std::string val;
                    char c;
                    do
                    {
                        auto readret = fread(&c, sizeof(c), 1, pFile);
                        if (feof(pFile)) break;
                        (void)readret;
                        if (c) val += c;
                    } while (c);
                    duk_push_string(ctx, val.c_str());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readString");

            // Vectors
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    Vector2 val;
                    auto readret = fread(&val, sizeof(float), 2, pFile);
                    (void)readret;
                    newVector2(ctx, val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readVector2");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    Vector3 val;
                    auto readret = fread(&val, sizeof(float), 3, pFile);
                    (void)readret;
                    newVector3(ctx, val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readVector3");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    Vector4 val;
                    auto readret = fread(&val, sizeof(float), 4, pFile);
                    (void)readret;
                    newVector4(ctx, val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readVector4");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    Matrix val;
                    auto readret = fread(&val, sizeof(float), 16, pFile);
                    (void)readret;
                    newMatrix(ctx, val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readMatrix");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    Rect val;
                    auto readret = fread(&val, sizeof(float), 4, pFile);
                    (void)readret;
                    newRect(ctx, val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readRect");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    Color val;
                    auto readret = fread(&val, sizeof(float), 4, pFile);
                    (void)readret;
                    newColor(ctx, val);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "readColor");

            // Done with the object
            pBinaryFileReaderPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "BinaryFileReader");
        }

        static void createBinaryFileWriterBindings()
        {
            auto ctx = pContext;

            // BinaryFileWriter() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto filename = JS_STRING(0);
                auto foundFilename = oContentManager->findResourceFile(filename);
                if (foundFilename.empty())
                {
                    if (oContentManager->getSearchPaths().empty()) foundFilename = filename;
                    else foundFilename = oContentManager->getSearchPaths().front() + "/" + filename;
                }
                FILE* pFile = nullptr;
#if defined(WIN32)
                fopen_s(&pFile, foundFilename.c_str(), "wb");
#else
                pFile = fopen(foundFilename.c_str(), "wb");
#endif
                if (!pFile) return DUK_RET_URI_ERROR;

                duk_push_this(ctx);
                duk_push_pointer(ctx, pFile);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");

                return 0;
            }, 1);
            duk_push_object(ctx);

            // ~BinaryFileWriter()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    fclose(pFile);
                    pFile = nullptr;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

#define WRITE_INT_TYPE(__type__, __fn__) \
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t \
            { \
                duk_push_this(ctx); \
                duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
                auto pFile = (FILE*)duk_to_pointer(ctx, -1); \
                if (pFile) \
                { \
                    __type__ val = (__type__)JS_INT(0); \
                    fwrite(&val, sizeof(__type__), 1, pFile); \
                    return 0; \
                } \
                return 0; \
            }, 1); \
            duk_put_prop_string(ctx, -2, #__fn__);
#define WRITE_UINT_TYPE(__type__, __fn__) \
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t \
            { \
                duk_push_this(ctx); \
                duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
                auto pFile = (FILE*)duk_to_pointer(ctx, -1); \
                if (pFile) \
                { \
                    __type__ val = (__type__)JS_UINT(0); \
                    fwrite(&val, sizeof(__type__), 1, pFile); \
                    return 0; \
                } \
                return 0; \
            }, 1); \
            duk_put_prop_string(ctx, -2, #__fn__);
            WRITE_INT_TYPE(int8_t, writeInt8);
            WRITE_INT_TYPE(int16_t, writeInt16);
            WRITE_INT_TYPE(int32_t, writeInt32);
            WRITE_UINT_TYPE(uint8_t, writeUInt8);
            WRITE_UINT_TYPE(uint16_t, writeUInt16);
            WRITE_UINT_TYPE(uint32_t, writeUInt32);

            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_FLOAT(0);
                    fwrite(&val, sizeof(val), 1, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeFloat");

            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    uint8_t val = JS_BOOL(0) ? 1 : 0;
                    fwrite(&val, sizeof(uint8_t), 1, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeBool");

            // writeString
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    std::string val = JS_STRING(0);
                    fwrite(val.c_str(), 1, val.size() + 1, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeString");

            // Vectors
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_VECTOR2(0);
                    fwrite(&val, sizeof(float), 2, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeVector2");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_VECTOR3(0);
                    fwrite(&val, sizeof(float), 3, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeVector3");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_VECTOR4(0);
                    fwrite(&val, sizeof(float), 4, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeVector4");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_MATRIX(0);
                    fwrite(&val, sizeof(float), 16, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeMatrix");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_RECT(0);
                    fwrite(&val, sizeof(float), 4, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeRect");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto pFile = (FILE*)duk_to_pointer(ctx, -1);
                if (pFile)
                {
                    auto val = JS_COLOR(0);
                    fwrite(&val, sizeof(float), 4, pFile);
                    return 0;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "writeColor");

            // Done with the object
            pBinaryFileWriterPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "BinaryFileWriter");
        }

        static void createVideoPlayerBindings()
        {
            auto ctx = pContext;

            // constructor
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto renderTarget = OTexture::createRenderTarget({1, 1});
                auto ppPlayer = new OVideoPlayerRef(OVideoPlayer::createWithRenderTarget(renderTarget));

                duk_push_this(ctx);
                duk_push_pointer(ctx, ppPlayer);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");
                return 0;
            }, 0);
            duk_push_object(ctx);

            // ~destructor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    delete ppPlayer;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // isPlaying()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    duk_push_boolean(ctx, (*ppPlayer)->isPlaying());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isPlaying");

            // pause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->pause();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "pause");

            // play()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->play();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "play");

            // setLoop()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto loop = JS_BOOL(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->setLoop(loop);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setLoop");

            // setPlayRate()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto playRate = JS_FLOAT(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->setPlayRate((double)playRate);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "playRate");

            // setSource()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto url = JS_STRING(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->setSource(url);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setSource");

            // setVolume()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto volume = JS_FLOAT(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->setVolume(volume);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setVolume");

            // togglePlayPause()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->togglePlayPause();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "togglePlayPause");

            // update()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    (*ppPlayer)->update();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "update");

            // getRenderTarget()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppPlayer = (OVideoPlayerRef*)duk_to_pointer(ctx, -1);
                if (ppPlayer)
                {
                    newTexture(ctx, (*ppPlayer)->getRenderTarget());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getRenderTarget");

            // Done with the object
            pVideoPlayerPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "VideoPlayer");
        }

        static void createVertexBufferBindings()
        {
            auto ctx = pContext;

            // VertexBuffer() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~VertexBuffer()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppVertexBuffer = (OVertexBufferRef*)duk_to_pointer(ctx, -1);
                if (ppVertexBuffer)
                {
                    delete ppVertexBuffer;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // getCount()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppVertexBuffer = (OVertexBufferRef*)duk_to_pointer(ctx, -1);
                if (ppVertexBuffer)
                {
                    duk_push_uint(ctx, (duk_uint_t)((*ppVertexBuffer)->size() / 4));
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getCount");

            // setData()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppVertexBuffer = (OVertexBufferRef*)duk_to_pointer(ctx, -1);
                if (ppVertexBuffer)
                {
                    (*ppVertexBuffer)->setData(pBuffer, bufferSize);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setData");

            // Done with the object
            pVertexBufferPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createStatic(data)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                newVertexBuffer(ctx, OVertexBuffer::createStatic(pBuffer, bufferSize));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createStatic");

            // createDynamic(size)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto count = JS_UINT(0);
                newVertexBuffer(ctx, OVertexBuffer::createDynamic(count * 4));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createDynamic");

            duk_put_global_string(ctx, "VertexBuffer");
        }

        static void createIndexBufferBindings()
        {
            auto ctx = pContext;

            // IndexBuffer() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~IndexBuffer()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppIndexBuffer = (OIndexBufferRef*)duk_to_pointer(ctx, -1);
                if (ppIndexBuffer)
                {
                    delete ppIndexBuffer;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // getCount()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppIndexBuffer = (OIndexBufferRef*)duk_to_pointer(ctx, -1);
                if (ppIndexBuffer)
                {
                    duk_push_uint(ctx, (duk_uint_t)((*ppIndexBuffer)->size() / 2));
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getCount");

            // setData()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppIndexBuffer = (OIndexBufferRef*)duk_to_pointer(ctx, -1);
                if (ppIndexBuffer)
                {
                    (*ppIndexBuffer)->setData(pBuffer, bufferSize);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setData");

            // Done with the object
            pIndexBufferPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // createStatic(data)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_size_t bufferSize;
                auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                newIndexBuffer(ctx, OIndexBuffer::createStatic(pBuffer, bufferSize));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createStatic");

            // createDynamic(size)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto count = JS_UINT(0);
                newIndexBuffer(ctx, OIndexBuffer::createDynamic(count * 2));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createDynamic");

            duk_put_global_string(ctx, "IndexBuffer");
        }

        static void createEntityBindings()
        {
            auto ctx = pContext;

            // Entity() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~IndexBuffer()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    delete ppEntity;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // destroy()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->destroy();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "destroy");

            // getTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    newMatrix(ctx, (*ppEntity)->getLocalTransform());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getTransform");

            // setTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto transform = JS_MATRIX(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->setLocalTransform(transform);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setTransform");

            // getWorldTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    newMatrix(ctx, (*ppEntity)->getWorldTransform());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getWorldTransform");

            // setWorldTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto transform = JS_MATRIX(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->setWorldTransform(transform);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setWorldTransform");

            // add()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto child = JS_ENTITY(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->add(child);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // remove()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto child = JS_ENTITY(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->remove(child);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "remove");

            // removeFromParent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->remove();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "removeFromParent");

            // getParent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    newEntity(ctx, (*ppEntity)->getParent());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getParent");

            // isEnabled()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    duk_push_boolean(ctx, (*ppEntity)->isEnabled() ? 1 : 0);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isEnabled");

            // setEnabled()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto enabled = JS_BOOL(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->setEnabled(enabled);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setEnabled");

            // isVisible()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    duk_push_boolean(ctx, (*ppEntity)->isVisible() ? 1 : 0);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isVisible");

            // setVisible()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto visible = JS_BOOL(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->setVisible(visible);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setVisible");

            // isStatic()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    duk_push_boolean(ctx, (*ppEntity)->isStatic() ? 1 : 0);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isStatic");

            // setStatic()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto _static = JS_BOOL(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->setStatic(_static);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setStatic");

            // getName()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    duk_push_string(ctx, (*ppEntity)->getName().c_str());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getName");

            // setName()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto name = JS_STRING(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    (*ppEntity)->setName(name);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setName");

            // getComponent()
       /*     duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto componentName = JS_STRING(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    auto pComponent = (*ppEntity)->getComponent();
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getComponent");*/

            // getParentComponent()

            // addComponent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    duk_pop(ctx);
                    if (duk_get_prop_string(ctx, 0, "\xff""\xff""data"))
                    {
                        // We passed a component directly
                        (*ppEntity)->addComponent(JS_COMPONENT(0));
                    }
                    else
                    {
                        // We passed a constructor to a component. use it!
                        duk_dup(ctx, 0);
                        duk_new(ctx, 0);
                    //    (*ppEntity)->addComponent(JS_COMPONENT(-1));
                        return 1;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "addComponent");

            // Done with the object
            pEntityPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // create()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newEntity(ctx, OEntity::create());
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "create");

            duk_put_global_string(ctx, "Entity");
        }

        static void addJSComponentBindings(duk_context *ctx)
        {
            // getTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    newMatrix(ctx, (*ppComponent)->getLocalTransform());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getTransform");

            // setTransform(matrix)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto matrix = JS_MATRIX(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    (*ppComponent)->setLocalTransform(matrix);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setTransform");

            // getWorldTransform()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    newMatrix(ctx, (*ppComponent)->getWorldTransform());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getWorldTransform");

            // setWorldTransform(matrix)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto matrix = JS_MATRIX(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    (*ppComponent)->setWorldTransform(matrix);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setWorldTransform");

            // getEntity()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    newEntity(ctx, (*ppComponent)->getEntity());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getEntity");

            // isEnabled()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    duk_push_boolean(ctx, (*ppComponent)->isEnabled() ? 1 : 0);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isEnabled");

            // setEnabled()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto enable = JS_BOOL(0);
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    (*ppComponent)->setEnabled(enable);
                    return 1;
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setEnabled");

            // getComponent()
            // getParentComponent()

            // destroy()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    (*ppComponent)->destroy();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "destroy");
        }

        static void createMathsBinding()
        {
            createVector2Bindings();
            createVector3Bindings();
            createVector4Bindings();
            createRectBindings();
            createColorBindings();
            createMatrixBindings();
        }

        static void createResourceBindings()
        {
            createTextureBindings();
            createModelBindings();
            createFontBindings();
            createShaderBindings();
            createMusicBindings();
            createSoundBindings();
            createSoundInstanceBindings();
            createTiledMapBindings();
            createSpriteAnimBindings();
            createSpriteAnimInstanceBindings();
            createParticleSystemBindings();
            createParticleEmitterBindings();
            createVertexBufferBindings();
            createIndexBufferBindings();
        }

        static void createAnimBindings()
        {
            createBoolAnimBindings();
            createNumberAnimBindings();
            createVector2AnimBindings();
            createVector3AnimBindings();
            createVector4AnimBindings();
            createRectAnimBindings();
            createColorAnimBindings();
            createMatrixAnimBindings();
        }

        static void createObjectBindings()
        {
            createVideoPlayerBindings();
            createEntityBindings();
        }

        static void createFileBindings()
        {
            createBinaryFileReaderBindings();
            createBinaryFileWriterBindings();
        }

        static void createUIBindings()
        {
            auto ctx = pContext;

            // UI() 
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                return DUK_RET_TYPE_ERROR; // No constructor allowed
            }, 1);
            duk_push_object(ctx);

            // ~UI()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    delete ppUIControl;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);

            // getName()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    duk_push_string(ctx, pUIControl->name.c_str());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getName");

            // setName(name)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto nameBefore = pUIControl->name;
                    pUIControl->name = JS_STRING(0, nameBefore.c_str());
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setName");

            // getRect()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    newRect(ctx, pUIControl->rect);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getRect");

            // setRect(rect)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto rectBefore = pUIControl->rect;
                    pUIControl->rect = JS_RECT(0, rectBefore);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setRect");

            // isVisible()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    duk_push_boolean(ctx, (duk_bool_t)pUIControl->isVisible);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isVisible");

            // setVisible(visible)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->isVisible = JS_BOOL(0, pUIControl->isVisible);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setVisible");

            // isEnabled()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    duk_push_boolean(ctx, (duk_bool_t)pUIControl->isEnabled);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isEnabled");

            // setEnabled(enabled)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->isEnabled = JS_BOOL(0, pUIControl->isEnabled);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setEnabled");

            // isClickThrough()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    duk_push_boolean(ctx, (duk_bool_t)pUIControl->isClickThrough);
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "isClickThrough");

            // setClickThrough(enabled)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->isClickThrough = JS_BOOL(0, pUIControl->isClickThrough);
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setClickThrough");

            // getChecked()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUICheckBox = dynamic_cast<OUICheckBox*>(pUIControl);
                    if (pUICheckBox)
                    {
                        duk_push_boolean(ctx, (duk_bool_t)pUICheckBox->getIsChecked());
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getChecked");

            // setChecked(checked)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUICheckBox = dynamic_cast<OUICheckBox*>(pUIControl);
                    if (pUICheckBox)
                    {
                        pUICheckBox->setIsChecked(JS_BOOL(0, pUICheckBox->getIsChecked()));
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setChecked");

            // getTexture()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUICheckBox = dynamic_cast<OUICheckBox*>(pUIControl);
                    if (pUICheckBox)
                    {
                        newTexture(ctx, pUICheckBox->iconComponent.background.image.pTexture);
                        return 1;
                    }
                    auto pUIImage = dynamic_cast<OUIImage*>(pUIControl);
                    if (pUIImage)
                    {
                        newTexture(ctx, pUIImage->scale9Component.image.pTexture);
                        return 1;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        newTexture(ctx, pUIButton->scale9Component.image.pTexture);
                        return 1;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        newTexture(ctx, pUITextField->scale9Component.image.pTexture);
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getTexture");

            // setTexture(texture)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUICheckBox = dynamic_cast<OUICheckBox*>(pUIControl);
                    if (pUICheckBox)
                    {
                        pUICheckBox->iconComponent.background.image.pTexture = JS_TEXTURE(0);
                        return 0;
                    }
                    auto pUIImage = dynamic_cast<OUIImage*>(pUIControl);
                    if (pUIImage)
                    {
                        pUIImage->scale9Component.image.pTexture = JS_TEXTURE(0);
                        return 0;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        pUIButton->scale9Component.image.pTexture = JS_TEXTURE(0);
                        return 0;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        pUITextField->scale9Component.image.pTexture = JS_TEXTURE(0);
                        return 0;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setTexture");

            // getText()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUILabel = dynamic_cast<OUILabel*>(pUIControl);
                    if (pUILabel)
                    {
                        duk_push_string(ctx, pUILabel->textComponent.text.c_str());
                        return 1;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        duk_push_string(ctx, pUIButton->textComponent.text.c_str());
                        return 1;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        duk_push_string(ctx, pUITextField->textComponent.text.c_str());
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getText");

            // setText(text)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUILabel = dynamic_cast<OUILabel*>(pUIControl);
                    if (pUILabel)
                    {
                        pUILabel->textComponent.text = JS_STRING(0);
                        return 0;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        pUIButton->textComponent.text = JS_STRING(0);
                        return 0;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        pUITextField->textComponent.text = JS_STRING(0);
                        return 0;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setText");

            // getFont()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUILabel = dynamic_cast<OUILabel*>(pUIControl);
                    if (pUILabel)
                    {
                        duk_push_string(ctx, pUILabel->textComponent.font.typeFace.c_str());
                        return 1;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        duk_push_string(ctx, pUIButton->textComponent.font.typeFace.c_str());
                        return 1;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        duk_push_string(ctx, pUITextField->textComponent.font.typeFace.c_str());
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getFont");

            // setFont(font)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUILabel = dynamic_cast<OUILabel*>(pUIControl);
                    if (pUILabel)
                    {
                        pUILabel->textComponent.font.typeFace = JS_STRING(0);
                        return 0;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        pUIButton->textComponent.font.typeFace = JS_STRING(0);
                        return 0;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        pUITextField->textComponent.font.typeFace = JS_STRING(0);
                        return 0;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setFont");

            // getColor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUIPanel = dynamic_cast<OUIPanel*>(pUIControl);
                    if (pUIPanel)
                    {
                        newColor(ctx, pUIPanel->color);
                        return 1;
                    }
                    auto pUIImage = dynamic_cast<OUIImage*>(pUIControl);
                    if (pUIImage)
                    {
                        newColor(ctx, pUIImage->scale9Component.image.color);
                        return 1;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        newColor(ctx, pUIButton->scale9Component.image.color);
                        return 1;
                    }
                    auto pUICheckBox = dynamic_cast<OUICheckBox*>(pUIControl);
                    if (pUICheckBox)
                    {
                        newColor(ctx, pUICheckBox->iconComponent.background.image.color);
                        return 1;
                    }
                    auto pUITextBox = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextBox)
                    {
                        newColor(ctx, pUITextBox->scale9Component.image.color);
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getColor");

            // setColor(font)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUIPanel = dynamic_cast<OUIPanel*>(pUIControl);
                    if (pUIPanel)
                    {
                        pUIPanel->color = JS_COLOR(0, pUIPanel->color);
                        return 0;
                    }
                    auto pUIImage = dynamic_cast<OUIImage*>(pUIControl);
                    if (pUIImage)
                    {
                        pUIImage->scale9Component.image.color = JS_COLOR(0, pUIImage->scale9Component.image.color);
                        return 0;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        pUIButton->scale9Component.image.color = JS_COLOR(0, pUIButton->scale9Component.image.color);
                        return 0;
                    }
                    auto pUICheckBox = dynamic_cast<OUICheckBox*>(pUIControl);
                    if (pUICheckBox)
                    {
                        pUICheckBox->iconComponent.background.image.color = JS_COLOR(0, pUICheckBox->iconComponent.background.image.color);
                        return 0;
                    }
                    auto pUITextBox = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextBox)
                    {
                        pUITextBox->scale9Component.image.color = JS_COLOR(0, pUITextBox->scale9Component.image.color);
                        return 0;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setColor");

            // getTextColor()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUILabel = dynamic_cast<OUILabel*>(pUIControl);
                    if (pUILabel)
                    {
                        newColor(ctx, pUILabel->textComponent.font.color);
                        return 1;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        newColor(ctx, pUIButton->textComponent.font.color);
                        return 1;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        newColor(ctx, pUITextField->textComponent.font.color);
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getTextColor");

            // setTextColor(color)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    auto pUILabel = dynamic_cast<OUILabel*>(pUIControl);
                    if (pUILabel)
                    {
                        pUILabel->textComponent.font.color = JS_COLOR(0, pUILabel->textComponent.font.color);
                        return 0;
                    }
                    auto pUIButton = dynamic_cast<OUIButton*>(pUIControl);
                    if (pUIButton)
                    {
                        pUIButton->textComponent.font.color = JS_COLOR(0, pUIButton->textComponent.font.color);
                        return 0;
                    }
                    auto pUITextField = dynamic_cast<OUITextBox*>(pUIControl);
                    if (pUITextField)
                    {
                        pUITextField->textComponent.font.color = JS_COLOR(0, pUITextField->textComponent.font.color);
                        return 0;
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "setTextColor");

            // add(child)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->add(JS_UI(0));
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "add");

            // insert(child, before)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->insert(JS_UI(0), JS_UI(1));
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "insert");

            // remove(child)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->remove(JS_UI(0));
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "remove");

            // removeAll(child)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    pUIControl->removeAll();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "removeAll");

            // getChildCount()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    duk_push_uint(ctx, (duk_uint_t)pUIControl->getChildren().size());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getChildCount");

            // getChild()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    if (duk_is_string(ctx, 0))
                    {
                        auto pChild = pUIControl->getChild(JS_STRING(0), JS_BOOL(1, true));
                        if (pChild)
                        {
                            newUI(ctx, pChild);
                            return 1;
                        }
                    }
                    else if (duk_is_number(ctx, 0))
                    {
                        auto index = JS_UINT(0);
                        if (index < pUIControl->getChildren().size())
                        {
                            newUI(ctx, pUIControl->getChildren()[index]);
                            return 1;
                        }
                    }
                }
                return 0;
            }, 2);
            duk_put_prop_string(ctx, -2, "getChild");

            // getParent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    if (pUIControl->getParent())
                    {
                        newUI(ctx, pUIControl->getParent());
                        return 1;
                    }
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getParent");

            // copy()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1);
                if (ppUIControl)
                {
                    auto pUIControl = ppUIControl->get();
                    newUI(ctx, pUIControl->copy());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "copy");

#define UI_MOUSE_EVENT(__jsname__, __oname__) \
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t \
            { \
                duk_push_this(ctx); \
                duk_get_prop_string(ctx, -1, "\xff""\xff""data"); \
                auto ppUIControl = (OUIControlRef*)duk_to_pointer(ctx, -1); \
                if (ppUIControl) \
                { \
                    auto pUIControl = ppUIControl->get(); \
                    auto callback = getFunction(ctx, 0); \
                    if (callback) \
                    { \
                        pUIControl->__oname__ = [ctx, callback](const OUIControlRef& pControl, const UIMouseEvent& evt) \
                        { \
                            if (callback->push(ctx)) callback->call(ctx, 0); \
                        }; \
                    } \
                    else \
                    { \
                        pUIControl->onClick = nullptr; \
                    } \
                } \
                return 0; \
            }, 1); \
            duk_put_prop_string(ctx, -2, #__jsname__);

            UI_MOUSE_EVENT(setOnClick, onClick);
            UI_MOUSE_EVENT(setOnDoubleClick, onDoubleClick);
            UI_MOUSE_EVENT(setOnMouseMove, onMouseMove);
            UI_MOUSE_EVENT(setOnMouseDown, onMouseDown);
            UI_MOUSE_EVENT(setOnMouseUp, onMouseUp);
            UI_MOUSE_EVENT(setOnMouseEnter, onMouseEnter);
            UI_MOUSE_EVENT(setOnMouseLeave, onMouseLeave);

            UI_MOUSE_EVENT(setOnRightClick, onRightClick);
            UI_MOUSE_EVENT(setOnRightDoubleClick, onRightDoubleClick);
            UI_MOUSE_EVENT(setOnRightMouseDown, onRightMouseDown);
            UI_MOUSE_EVENT(setOnRightMouseUp, onRightMouseUp);

            UI_MOUSE_EVENT(setOnMiddleClick, onMiddleClick);
            UI_MOUSE_EVENT(setOnMiddleDoubleClick, onMiddleDoubleClick);
            UI_MOUSE_EVENT(setOnMiddleMouseDown, onMiddleMouseDown);
            UI_MOUSE_EVENT(setOnMiddleMouseUp, onMiddleMouseUp);

            // Done with the object
            pUIPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            // creates ...
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUIControl::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "create");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUILabel::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createLabel");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUIPanel::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createFrame");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUIImage::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createImage");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUIButton::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createButton");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUITextBox::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createTextField");
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                newUI(ctx, OUICheckBox::create());
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createCheckBox");

            duk_put_global_string(ctx, "UI");
        }

        static void createBindings()
        {
            auto ctx = pContext;

            // Log function
            JS_GLOBAL_FUNCTION_BEGIN
            {
                OLog(JS_STRING(0));
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("print", 1);

            // Exit function
            JS_GLOBAL_FUNCTION_BEGIN
            {
                OQuit();
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("quit", 0);

            // setTimeout
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto callback = getFunction(ctx, 0);
                auto duration = JS_UINT(1, 0);
                if (callback)
                {
                    auto pTimer = new OTimer();
                    pTimer->start(std::chrono::milliseconds(duration), [ctx, pTimer, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                        delete pTimer;
                    });
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("setTimeout", 2);

            // defer
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto callback = getFunction(ctx, 0);
                if (callback)
                {
                    oDispatcher->dispatch([ctx, callback]
                    {
                        if (callback->push(ctx)) callback->call(ctx, 0);
                    });
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("defer", 2);

            // oRenderer
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->clear(JS_COLOR(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("clear", 1);

                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->clearDepth();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("clearDepth", 1);

                JS_INTERFACE_FUNCTION_BEGIN
                {
                    if (oSettings->getIsRetroMode())
                    {
                        newVector2(ctx, Vector2((float)oSettings->getRetroResolution().x, (float)oSettings->getRetroResolution().y));
                    }
                    else
                    {
                        newVector2(ctx, OScreenf);
                    }
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getResolution", 0);

                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->setupFor2D(JS_MATRIX(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setupFor2D", 1);

                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->setupFor3D(
                        JS_VECTOR3(0, { 0, -10, 0 }),
                        JS_VECTOR3(1, Vector3::Zero),
                        JS_VECTOR3(2, Vector3::Up),
                        JS_FLOAT(3, 90.0f)
                    );
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setupFor3D", 4);

                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->draw(JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("draw", 1);

                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->drawIndexed(JS_UINT(0), JS_UINT(1));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawIndexed", 2);

                // Render target
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pTexture = JS_TEXTURE(0);
                    oRenderer->renderStates.renderTarget = pTexture;
                    updateViewport();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setRenderTarget", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pTexture = JS_TEXTURE(0);
                    oRenderer->renderStates.renderTarget.push(pTexture);
                    updateViewport();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushRenderTarget", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.renderTarget.pop();
                    updateViewport();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popRenderTarget", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newTexture(ctx, oRenderer->renderStates.renderTarget.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getRenderTarget", 0);
                // Textures
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pTexture = JS_TEXTURE(0);
                    auto index = JS_UINT(1);
                    if (index < 8)
                        oRenderer->renderStates.textures[index] = pTexture;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setTexture", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pTexture = JS_TEXTURE(0);
                    auto index = JS_UINT(1);
                    if (index < 8)
                        oRenderer->renderStates.textures[index].push(pTexture);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushTexture", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto index = JS_UINT(0);
                    if (index < 8)
                        oRenderer->renderStates.textures[index].pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popTexture", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto index = JS_UINT(0);
                    if (index < 8)
                    {
                        newTexture(ctx, oRenderer->renderStates.textures[index].get());
                        return 1;
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("getTexture", 1);
                // Vertex shader
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pShader = JS_SHADER(0);
                    oRenderer->renderStates.vertexShader = pShader;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setVertexShader", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pShader = JS_SHADER(0);
                    oRenderer->renderStates.vertexShader.push(pShader);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushVertexShader", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.vertexShader.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popVertexShader", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newShader(ctx, oRenderer->renderStates.vertexShader.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getVertexShader", 0);
                // Pixel shader
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pShader = JS_SHADER(0);
                    oRenderer->renderStates.pixelShader = pShader;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setPixelShader", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pShader = JS_SHADER(0);
                    oRenderer->renderStates.pixelShader.push(pShader);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushPixelShader", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.pixelShader.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popPixelShader", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newShader(ctx, oRenderer->renderStates.pixelShader.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getpPixelShader", 0);
                // Vertex buffer
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pVertexBuffer = JS_VERTEX_BUFFER(0);
                    oRenderer->renderStates.vertexBuffer = pVertexBuffer;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setVertexBuffer", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pVertexBuffer = JS_VERTEX_BUFFER(0);
                    oRenderer->renderStates.vertexBuffer.push(pVertexBuffer);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushVertexbuffer", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.vertexBuffer.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popVertexBuffer", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newVertexBuffer(ctx, oRenderer->renderStates.vertexBuffer.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getVertexBuffer", 0);
                // Index buffer
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pIndexBuffer = JS_INDEX_BUFFER(0);
                    oRenderer->renderStates.indexBuffer = pIndexBuffer;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setIndexBuffer", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pIndexBuffer = JS_INDEX_BUFFER(0);
                    oRenderer->renderStates.indexBuffer.push(pIndexBuffer);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushIndexbuffer", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.indexBuffer.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popIndexBuffer", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newIndexBuffer(ctx, oRenderer->renderStates.indexBuffer.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getIndexBuffer", 0);
                // Blend mode
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.blendMode = (onut::BlendMode)JS_UINT(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setBlendMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.blendMode.push((onut::BlendMode)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushBlendMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.blendMode.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popBlendMode", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oRenderer->renderStates.blendMode.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getBlendMode", 0);
                // Filter mode
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.sampleFiltering = (onut::sample::Filtering)JS_UINT(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setFilterMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.sampleFiltering.push((onut::sample::Filtering)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushFilterMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.sampleFiltering.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popFilterMode", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oRenderer->renderStates.sampleFiltering.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getFilterMode", 0);
                // Filter mode
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.sampleAddressMode = (onut::sample::AddressMode)JS_UINT(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setWrapMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.sampleAddressMode.push((onut::sample::AddressMode)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushWrapMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.sampleAddressMode.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popWrapMode", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oRenderer->renderStates.sampleAddressMode.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getWrapMode", 0);
                // Viewport
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto viewport = JS_iRECT(0);
                    oRenderer->renderStates.viewport = viewport;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setViewport", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto viewport = JS_iRECT(0);
                    oRenderer->renderStates.viewport.push(viewport);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushViewport", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.viewport.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popViewport", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto r = oRenderer->renderStates.viewport.get();
                    newRect(ctx, Rect((float)r.left, (float)r.top, (float)(r.right - r.left), (float)(r.bottom - r.top)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getViewport", 0);
                // Scissor
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto scissor = JS_iRECT(0);
                    oRenderer->renderStates.scissor = scissor;
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setScissor", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto scissor = JS_iRECT(0);
                    oRenderer->renderStates.scissor.push(scissor);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushScissor", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.scissor.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popScissor", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto r = oRenderer->renderStates.scissor.get();
                    newRect(ctx, Rect((float)r.left, (float)r.top, (float)(r.right - r.left), (float)(r.bottom - r.top)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getScissor", 0);
                // Scissor Enabled
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.scissorEnabled = JS_BOOL(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setScissorEnabled", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.scissorEnabled.push(JS_BOOL(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushScissorEnabled", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.scissorEnabled.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popScissorEnabled", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, (duk_bool_t)(oRenderer->renderStates.scissorEnabled.get() ? 1 : 0));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getScissorEnabled", 0);
                // Primitive Mode
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.primitiveMode = (onut::PrimitiveMode)JS_UINT(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setPrimitiveMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.primitiveMode.push((onut::PrimitiveMode)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushPrimitiveMode", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.primitiveMode.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popPrimitiveMode", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oRenderer->renderStates.primitiveMode.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getPrimitiveMode", 0);
                // World
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.world = JS_MATRIX(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setWorld", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.world.push(JS_MATRIX(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushWorld", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.world.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popWorld", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newMatrix(ctx, oRenderer->renderStates.world.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getWorld", 0);
                // View Projection Matrix
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.projection = JS_MATRIX(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setProjection", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.projection.push(JS_MATRIX(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushProjection", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.projection.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popProjection", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newMatrix(ctx, oRenderer->renderStates.projection.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getProjection", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.view = JS_MATRIX(0);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setView", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.view.push(JS_MATRIX(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushView", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.view.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popView", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newMatrix(ctx, oRenderer->renderStates.view.get());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getView", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.depthEnabled = JS_BOOL(0, false);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setDepthEnabled", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.depthEnabled.push(JS_BOOL(0, false));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushDepthEnabled", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.depthEnabled.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popDepthEnabled", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, (duk_bool_t)(oRenderer->renderStates.depthEnabled.get() ? 1 : 0));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getDepthEnabled", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.depthWrite = JS_BOOL(0, true);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setDepthWrite", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.depthWrite.push(JS_BOOL(0, true));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushDepthWrite", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.depthWrite.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popDepthWrite", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, (duk_bool_t)(oRenderer->renderStates.depthWrite.get() ? 1 : 0));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getDepthWrite", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.backFaceCull = JS_BOOL(0, false);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setBackFaceCull", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.backFaceCull.push(JS_BOOL(0, false));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushBackFaceCull", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.backFaceCull.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popBackFaceFull", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, (duk_bool_t)(oRenderer->renderStates.backFaceCull.get() ? 1 : 0));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getBackFaceFull", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->setAlphaTestBias(JS_FLOAT(0, 0.3f));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setAlphaTestBias", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->setSun(JS_VECTOR3(1, Vector3(0, 0, 1)), JS_COLOR(1, Color::Black));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setSun", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->setLight(JS_INT(0, 0), JS_VECTOR3(1, Vector3::Zero), JS_FLOAT(2, 0.0f), JS_COLOR(3, Color::Black));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setLight", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->setAmbient(JS_COLOR(0, Color::Black));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setAmbient", 1);
            }
            JS_INTERFACE_END("Renderer");

            // oSpriteBatch
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto matrix = JS_MATRIX(0);
                    auto shader = JS_SHADER(1);
                    oSpriteBatch->begin(matrix);
                    if (shader)
                    {
                        oRenderer->renderStates.pixelShader = shader;
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("begin", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->end();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("end", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawRect(JS_TEXTURE(0), JS_RECT(1), JS_COLOR(2));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawRect", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawInclinedRect(JS_TEXTURE(0), JS_RECT(1), JS_FLOAT(2), JS_COLOR(3));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawInclinedRect", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawRectWithColors(JS_TEXTURE(0), JS_RECT(1), {JS_COLOR(2), JS_COLOR(3), JS_COLOR(4), JS_COLOR(5)});
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawRectWithColors", 6);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawRectWithUVs(JS_TEXTURE(0), JS_RECT(1), JS_VECTOR4(2), JS_COLOR(3));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawRectWithUVs", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->draw4Corner(JS_TEXTURE(0), JS_RECT(1), JS_COLOR(2));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("draw4Corner", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawRectScaled9(JS_TEXTURE(0), JS_RECT(1), JS_VECTOR4(2), JS_COLOR(3));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawRectScaled9", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawRectScaled9RepeatCenters(JS_TEXTURE(0), JS_RECT(1), JS_VECTOR4(2), JS_COLOR(3));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawRectScaled9RepeatCenters", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawSprite(JS_TEXTURE(0), JS_VECTOR2(1), JS_COLOR(2), JS_FLOAT(3, 0.0f), JS_FLOAT(4, 1.0f), JS_VECTOR2(5, Vector2(.5f, .5f)));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawSprite", 6);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawSpriteWithUVs(JS_TEXTURE(0), JS_VECTOR2(1), JS_VECTOR4(2), JS_COLOR(3), JS_FLOAT(4, 0.0f), JS_FLOAT(5, 1.0f), JS_VECTOR2(6, Vector2(.5f, .5f)));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawSpriteWithUVs", 7);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pSpriteAnimInstance = JS_SPRITE_ANIM_INSTANCE(0);
                    if (pSpriteAnimInstance)
                    {
                        oSpriteBatch->drawSpriteWithUVs(pSpriteAnimInstance->getTexture(), JS_VECTOR2(1), pSpriteAnimInstance->getUVs(), JS_COLOR(2), JS_FLOAT(3, 0.0f), JS_FLOAT(4, 1.0f), pSpriteAnimInstance->getOrigin());
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawSpriteAnim", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawSprite(JS_TEXTURE(0), JS_MATRIX(1), JS_VECTOR2(3, Vector2::One), JS_COLOR(2), JS_VECTOR2(4, Vector2(.5f, .5f)));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawTransformedSprite", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pSpriteAnimInstance = JS_SPRITE_ANIM_INSTANCE(0);
                    if (pSpriteAnimInstance)
                    {
                        oSpriteBatch->drawSpriteWithUVs(pSpriteAnimInstance->getTexture(), JS_MATRIX(1), JS_VECTOR2(3, Vector2::One), pSpriteAnimInstance->getUVs(), JS_COLOR(2), pSpriteAnimInstance->getOrigin());
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawTransformedSpriteAnim", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawBeam(JS_TEXTURE(0), JS_VECTOR2(1), JS_VECTOR2(2), JS_FLOAT(3, 1.f), JS_COLOR(4), JS_FLOAT(5, 0.f), JS_FLOAT(6, 1.f));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawBeam", 7);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->drawCross(JS_VECTOR2(0), JS_FLOAT(1, 10.0f), JS_COLOR(2));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawCross", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pFont = JS_FONT(0);
                    if (pFont) pFont->draw(JS_STRING(1), JS_VECTOR2(2), JS_VECTOR2(3, OTopLeft), JS_COLOR(4));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawText", 5);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pFont = JS_FONT(0);
                    if (pFont) pFont->drawOutlined(JS_STRING(1), JS_VECTOR2(2), JS_VECTOR2(3, OTopLeft), JS_COLOR(4), JS_COLOR(5, Color(0, 0, 0, .75f)), JS_FLOAT(6, 2.0f));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawOutlinedText", 7);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pFont = JS_FONT(0);
                    if (pFont) pFont->drawOutlined(JS_STRING(1), JS_VECTOR2(2), JS_VECTOR2(3, OTopLeft), JS_COLOR(4), JS_COLOR(5, Color(0, 0, 0, .75f)), JS_FLOAT(6, 2.0f), false);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawPrettyOutlinedText", 7);
            }
            JS_INTERFACE_END("SpriteBatch");

            // oPrimitiveBatch
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto type = (onut::PrimitiveMode)JS_UINT(0);
                    auto texture = JS_TEXTURE(1);
                    auto transform = JS_MATRIX(2);
                    oPrimitiveBatch->begin(type, texture, transform);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("begin", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oPrimitiveBatch->end();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("end", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto position = JS_VECTOR2(0);
                    auto color = JS_COLOR(1);
                    auto texCoord = JS_VECTOR2(2);
                    oPrimitiveBatch->draw(position, color, texCoord);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("draw", 3);
            }
            JS_INTERFACE_END("PrimitiveBatch");

            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    setSeed((unsigned int)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setSeed", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    randomizeSeed();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("randomizeSeed", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    int mod = JS_INT(0);
                    duk_double_t val = (duk_double_t)(randi() % mod);
                    duk_push_number(ctx, val);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getNext", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    int min = JS_INT(0);
                    int max = JS_INT(1);
                    if (duk_is_number(ctx, 1)) duk_push_number(ctx, (duk_double_t)randi(min, max));
                    else duk_push_number(ctx, (duk_double_t)randi(min));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randInt", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto chances = JS_FLOAT(0, .5f);
                    duk_push_boolean(ctx, randb(chances));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randBool", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto min = JS_FLOAT(0);
                    auto max = JS_FLOAT(1);
                    if (duk_is_number(ctx, 1)) duk_push_number(ctx, (duk_double_t)randf(min, max));
                    else duk_push_number(ctx, (duk_double_t)randf(min));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randNumber", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto min = JS_VECTOR2(0);
                    auto max = JS_VECTOR2(1);
                    if (duk_is_object(ctx, 1)) newVector2(ctx, rand2f(min, max));
                    else newVector2(ctx, rand2f(min));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randVector2", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto min = JS_VECTOR3(0);
                    auto max = JS_VECTOR3(1);
                    if (duk_is_object(ctx, 1)) newVector3(ctx, rand3f(min, max));
                    else newVector3(ctx, rand3f(min));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randVector3", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto min = JS_VECTOR4(0);
                    auto max = JS_VECTOR4(1);
                    if (duk_is_object(ctx, 1)) newVector4(ctx, rand4f(min, max));
                    else newVector4(ctx, rand4f(min));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randVector4", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto center = JS_VECTOR2(0);
                    auto radius = JS_FLOAT(1);
                    newVector2(ctx, randCircle(center, radius));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randCircle", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto center = JS_VECTOR2(0);
                    auto radius = JS_FLOAT(1);
                    newVector2(ctx, randCircleEdge(center, radius));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randCircleEdge", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    if (duk_is_number(ctx, 0))
                    {
                        newColor(ctx, randc(JS_FLOAT(0, 1.0f)));
                    }
                    else if (duk_is_object(ctx, 0)) 
                    {
                        if (duk_is_object(ctx, 1)) 
                        {
                            newColor(ctx, randc(JS_COLOR(0), JS_COLOR(1)));
                        }
                        else if (duk_is_number(ctx, 1))
                        {
                            newColor(ctx, randc(JS_COLOR(0), JS_FLOAT(1)));
                        }
                        else
                        {
                            newColor(ctx, randc(JS_COLOR(0)));
                        }
                    }
                    else
                    {
                        newColor(ctx, randc());
                    }
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("randColor", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    if (duk_is_array(ctx, 0))
                    {
                        Palette palette;
                        auto len = duk_get_length(ctx, 0);
                        for (decltype(len) i = 0; i < len; ++i)
                        {
                            duk_get_prop_index(ctx, 0, i);
                            palette.push_back(JS_COLOR(-1));
                            duk_pop(ctx);
                        }
                        newColor(ctx, randc(palette, JS_FLOAT(1)));
                        return 1;
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("randPalette", 2);
            }
            JS_INTERFACE_END("Random");

            // oInput
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    if (oSettings->getIsRetroMode())
                    {
                        auto realRes = oRenderer->getTrueResolution();
                        auto retroRes = oSettings->getRetroResolution();
                        auto rect = ORectSmartFit(Rect(0, 0, (float)realRes.x, (float)realRes.y), Vector2((float)retroRes.x, (float)retroRes.y));
                        auto mousePos = oInput->mousePosf;
                        mousePos.x -= rect.x;
                        mousePos.y -= rect.y;
                        mousePos.x /= rect.z;
                        mousePos.y /= rect.w;
                        mousePos.x *= (float)retroRes.x;
                        mousePos.y *= (float)retroRes.y;
                        newVector2(ctx, mousePos);
                    }
                    else
                    {
                        newVector2(ctx, oInput->mousePosf);
                    }
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getMousePos", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, oInput->isStateDown((onut::Input::State)JS_UINT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isDown", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, oInput->isStateUp((onut::Input::State)JS_UINT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isUp", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, oInput->isStateJustDown((onut::Input::State)JS_UINT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isJustDown", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, oInput->isStateJustUp((onut::Input::State)JS_UINT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isJustUp", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oInput->getStateValue((onut::Input::State)JS_UINT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getValue", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto visible = JS_BOOL(0);
                    oInput->setMouseVisible(visible);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setMouseVisible", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    if (duk_is_null_or_undefined(ctx, 0))
                    {
                        oInput->unsetMouseIcon();
                    }
                    else
                    {
                        auto filename = JS_STRING(0);
                        auto x = JS_INT(1);
                        auto y = JS_INT(2);
                        oInput->setMouseIcon(filename, Point(x, y));
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setMouseIcon", 3);
            }
            JS_INTERFACE_END("Input");

            // GamePad
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_boolean(ctx, OGetGamePad(JS_INT(0))->isConnected());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isConnected", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto index = JS_INT(0);
                    auto button = (onut::GamePad::Button)JS_UINT(1);
                    duk_push_boolean(ctx, OGamePadPressed(button, index));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isDown", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto index = JS_INT(0);
                    auto button = (onut::GamePad::Button)JS_UINT(1);
                    duk_push_boolean(ctx, !OGamePadPressed(button, index));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isUp", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto index = JS_INT(0);
                    auto button = (onut::GamePad::Button)JS_UINT(1);
                    duk_push_boolean(ctx, OGamePadJustPressed(button, index));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isJustDown", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto index = JS_INT(0);
                    auto button = (onut::GamePad::Button)JS_UINT(1);
                    duk_push_boolean(ctx, OGamePadJustReleased(button, index));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("isJustUp", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newVector2(ctx, OGetGamePadLeftThumb(JS_INT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getLeftThumb", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    newVector2(ctx, OGetGamePadRightThumb(JS_INT(0)));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getRightThumb", 1);
            }
            JS_INTERFACE_END("GamePad");

            // Http
#define JS_HTTP_ARGUMENTS(__index__) \
    OHttp::Arguments arguments; \
    if (duk_is_object(ctx, __index__)) \
    { \
        auto str = duk_json_encode(ctx, __index__); \
        Json::Reader reader; \
        Json::Value root; \
        if (reader.parse(str, root)) \
        { \
            auto members = root.getMemberNames(); \
            for (auto& member : members) \
            { \
                arguments.push_back({member, root[member].asString()}); \
            } \
        } \
    }
            static uint32_t uniqueHTTPRequestID = 0;
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto url = JS_STRING(0);
                    JS_HTTP_ARGUMENTS(1);
                    auto onErrorFn = duk_get_heapptr(ctx, 2);
                    auto ret = oHttp->getString(url, arguments, [onErrorFn, ctx](long code, std::string message)
                    {
                        if (onErrorFn)
                        {
                            duk_push_heapptr(ctx, onErrorFn);
                            duk_push_uint(ctx, (duk_uint_t)code);
                            duk_push_string(ctx, message.c_str());
                            duk_call(ctx, 2);
                            duk_pop(ctx);
                        }
                    });

                    duk_push_string(ctx, ret.c_str());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getString", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto url = JS_STRING(0);
                    JS_HTTP_ARGUMENTS(1);
                    auto requestId = uniqueHTTPRequestID++;
                    std::string keyOK;
                    std::string keyFAIL;
                    if (duk_is_function(ctx, 2))
                    {
                        keyOK = "httpOK" + std::to_string(requestId);
                        duk_push_global_stash(ctx);
                        duk_dup(ctx, 2);
                        duk_put_prop_string(ctx, -2, keyOK.c_str());
                    }
                    if (duk_is_function(ctx, 3))
                    {
                        keyFAIL = "httpFAIL" + std::to_string(requestId);
                        duk_push_global_stash(ctx);
                        duk_dup(ctx, 3);
                        duk_put_prop_string(ctx, -2, keyFAIL.c_str());
                    }
                    oHttp->getStringAsync(url, arguments,
                                               [requestId, ctx, keyOK, keyFAIL](std::string ret)
                    {
                        duk_push_global_stash(ctx);
                        if (duk_get_prop_string(ctx, -1, keyOK.c_str()))
                        {
                            duk_push_string(ctx, ret.c_str());
                            duk_call(ctx, 1);
                            duk_pop(ctx);
                        }
                        if (!keyOK.empty()) duk_del_prop_string(ctx, -1, keyOK.c_str());
                        if (!keyFAIL.empty()) duk_del_prop_string(ctx, -1, keyFAIL.c_str());
                        duk_pop(ctx);
                    },
                        [requestId, ctx, keyOK, keyFAIL](long code, std::string message)
                    {
                        duk_push_global_stash(ctx);
                        if (duk_get_prop_string(ctx, -1, keyFAIL.c_str()))
                        {
                            duk_push_uint(ctx, (duk_uint_t)code);
                            duk_push_string(ctx, message.c_str());
                            duk_call(ctx, 2);
                            duk_pop(ctx);
                        }
                        if (!keyOK.empty()) duk_del_prop_string(ctx, -1, keyOK.c_str());
                        if (!keyFAIL.empty()) duk_del_prop_string(ctx, -1, keyFAIL.c_str());
                        duk_pop(ctx);
                    });
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("getStringAsync", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto url = JS_STRING(0);
                    JS_HTTP_ARGUMENTS(1);
                    auto onErrorFn = duk_get_heapptr(ctx, 2);
                    auto ret = oHttp->post(url, arguments, [onErrorFn, ctx](long code, std::string message)
                    {
                        if (onErrorFn)
                        {
                            duk_push_heapptr(ctx, onErrorFn);
                            duk_push_uint(ctx, (duk_uint_t)code);
                            duk_push_string(ctx, message.c_str());
                            duk_call(ctx, 2);
                            duk_pop(ctx);
                        }
                    });

                    duk_push_string(ctx, ret.c_str());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("post", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto url = JS_STRING(0);
                    JS_HTTP_ARGUMENTS(1);
                    auto requestId = uniqueHTTPRequestID++;
                    std::string keyOK;
                    std::string keyFAIL;
                    if (duk_is_function(ctx, 2))
                    {
                        keyOK = "httpOK" + std::to_string(requestId);
                        duk_push_global_stash(ctx);
                        duk_dup(ctx, 2);
                        duk_put_prop_string(ctx, -2, keyOK.c_str());
                    }
                    if (duk_is_function(ctx, 3))
                    {
                        keyFAIL = "httpFAIL" + std::to_string(requestId);
                        duk_push_global_stash(ctx);
                        duk_dup(ctx, 3);
                        duk_put_prop_string(ctx, -2, keyFAIL.c_str());
                    }
                    oHttp->getStringAsync(url, arguments,
                                               [requestId, ctx, keyOK, keyFAIL](std::string ret)
                    {
                        duk_push_global_stash(ctx);
                        if (duk_get_prop_string(ctx, -1, keyOK.c_str()))
                        {
                            duk_push_string(ctx, ret.c_str());
                            duk_call(ctx, 1);
                            duk_pop(ctx);
                        }
                        if (!keyOK.empty()) duk_del_prop_string(ctx, -1, keyOK.c_str());
                        if (!keyFAIL.empty()) duk_del_prop_string(ctx, -1, keyFAIL.c_str());
                        duk_pop(ctx);
                    },
                        [requestId, ctx, keyOK, keyFAIL](long code, std::string message)
                    {
                        duk_push_global_stash(ctx);
                        if (duk_get_prop_string(ctx, -1, keyFAIL.c_str()))
                        {
                            duk_push_uint(ctx, (duk_uint_t)code);
                            duk_push_string(ctx, message.c_str());
                            duk_call(ctx, 2);
                            duk_pop(ctx);
                        }
                        if (!keyOK.empty()) duk_del_prop_string(ctx, -1, keyOK.c_str());
                        if (!keyFAIL.empty()) duk_del_prop_string(ctx, -1, keyFAIL.c_str());
                        duk_pop(ctx);
                    });
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("postAsync", 4);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto url = JS_STRING(0);
                    JS_HTTP_ARGUMENTS(1);
                    auto onErrorFn = duk_get_heapptr(ctx, 2);
                    auto ret = OHTTPGetTexture(url, arguments, [onErrorFn, ctx](long code, std::string message)
                    {
                        if (onErrorFn)
                        {
                            duk_push_heapptr(ctx, onErrorFn);
                            duk_push_uint(ctx, (duk_uint_t)code);
                            duk_push_string(ctx, message.c_str());
                            duk_call(ctx, 2);
                            duk_pop(ctx);
                        }
                    });
                    newTexture(ctx, ret);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getTexture", 3);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto url = JS_STRING(0);
                    JS_HTTP_ARGUMENTS(1);
                    auto requestId = uniqueHTTPRequestID++;
                    std::string keyOK;
                    std::string keyFAIL;
                    if (duk_is_function(ctx, 2))
                    {
                        keyOK = "httpOK" + std::to_string(requestId);
                        duk_push_global_stash(ctx);
                        duk_dup(ctx, 2);
                        duk_put_prop_string(ctx, -2, keyOK.c_str());
                    }
                    if (duk_is_function(ctx, 3))
                    {
                        keyFAIL = "httpFAIL" + std::to_string(requestId);
                        duk_push_global_stash(ctx);
                        duk_dup(ctx, 3);
                        duk_put_prop_string(ctx, -2, keyFAIL.c_str());
                    }
                    OHTTPGetTextureAsync(url, arguments,
                                         [requestId, ctx, keyOK, keyFAIL](OTextureRef ret)
                    {
                        duk_push_global_stash(ctx);
                        if (duk_get_prop_string(ctx, -1, keyOK.c_str()))
                        {
                            newTexture(ctx, ret);
                            duk_call(ctx, 1);
                            duk_pop(ctx);
                        }
                        if (!keyOK.empty()) duk_del_prop_string(ctx, -1, keyOK.c_str());
                        if (!keyFAIL.empty()) duk_del_prop_string(ctx, -1, keyFAIL.c_str());
                        duk_pop(ctx);
                    },
                        [requestId, ctx, keyOK, keyFAIL](long code, std::string message)
                    {
                        duk_push_global_stash(ctx);
                        if (duk_get_prop_string(ctx, -1, keyFAIL.c_str()))
                        {
                            duk_push_uint(ctx, (duk_uint_t)code);
                            duk_push_string(ctx, message.c_str());
                            duk_call(ctx, 2);
                            duk_pop(ctx);
                        }
                        if (!keyOK.empty()) duk_del_prop_string(ctx, -1, keyOK.c_str());
                        if (!keyFAIL.empty()) duk_del_prop_string(ctx, -1, keyFAIL.c_str());
                        duk_pop(ctx);
                    });
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("getTextureAsync", 4);
            }
            JS_INTERFACE_END("Http");

            // FileSystem
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto filename = JS_STRING(0);
                    auto foundFilename = oContentManager->findResourceFile(filename);
                    if (foundFilename.empty()) return DUK_RET_URI_ERROR;

                    remove(foundFilename.c_str());

                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("delete", 1);
            }
            JS_INTERFACE_END("FileSystem");

            // Crypto
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto str = JS_STRING(0);
                    auto seed = JS_UINT(1, 0);
                    duk_push_uint(ctx, OHash(str, seed));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("hash", 2);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto str = JS_STRING(0);
                    duk_push_string(ctx, OSha1(str).c_str());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("sha1", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto str = JS_STRING(0);
                    duk_push_boolean(ctx, OValidateEmail(str));
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("validateEmail", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    if (duk_is_string(ctx, 0))
                    {
                        auto str = JS_STRING(0);
                        duk_push_string(ctx, OEncodeBase64(reinterpret_cast<const uint8_t*>(str), strlen(str)).c_str());
                        return 1;
                    }
                    else if (duk_is_buffer(ctx, 0))
                    {
                        duk_size_t bufferSize;
                        auto pBuffer = duk_get_buffer_data(ctx, 0, &bufferSize);
                        duk_push_string(ctx, OEncodeBase64(reinterpret_cast<const uint8_t*>(pBuffer), bufferSize).c_str());
                        return 1;
                    }
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("encodeBase64", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto str = JS_STRING(0);
                    auto out = ODecodeBase64(str);
                    auto pOut = duk_push_buffer(ctx, out.size(), 0);
                    memcpy(pOut, out.data(), out.size());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("decodeBase64", 1);
            }
            JS_INTERFACE_END("Cryptography");

            // Timing
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oTiming->getDeltaTime());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getDeltaTime", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oTiming->getRenderDeltaTime());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getRenderDeltaTime", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_number(ctx, (duk_double_t)oTiming->getTotalElapsed());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getTotalElapsed", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    duk_push_int(ctx, oTiming->getFPS());
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getFPS", 0);
            }
            JS_INTERFACE_END("Timing");

            // Resources
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto prev = oGenerateMipmaps;
                oGenerateMipmaps = JS_BOOL(1, true);
                auto pTex = OGetTexture(JS_STRING(0));
                oGenerateMipmaps = prev;
                if (pTex)
                {
                    newTexture(ctx, pTex);
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            JS_GLOBAL_FUNCTION_END("getTexture", 2);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newModel(ctx, OGetModel(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getModel", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newFont(ctx, OGetFont(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getFont", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newShader(ctx, OGetShader(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getShader", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newMusic(ctx, OGetMusic(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getMusic", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto music = OGetMusic(JS_STRING(0));
                if (music)
                {
                    music->play(JS_BOOL(1, true));
                    newMusic(ctx, music);
                    return 1;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("playMusic", 2);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newSound(ctx, OGetSound(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getSound", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto resName = JS_STRING(0);
                if (onut::getExtension(resName) == "CUE")
                {
                    if (oContentManager->isResourceLoaded(resName))
                    {
                        oContentManager->removeResource(OGetSoundCue(resName));
                    }
                    OGetSoundCue(resName);
                    return 0;
                }
                else
                {
                    if (oContentManager->isResourceLoaded(resName))
                    {
                        oContentManager->removeResource(OGetSound(resName));
                    }
                    newSound(ctx, OGetSound(resName));
                    return 1;
                }
            }
            JS_GLOBAL_FUNCTION_END("getFreshSound", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                OPlaySound(JS_STRING(0), JS_FLOAT(1, 1), JS_FLOAT(2), JS_FLOAT(3, 1));
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("playSound", 4);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                if (duk_is_array(ctx, 0))
                {
                    std::vector<std::string> sounds;
                    auto len = duk_get_length(ctx, 0);
                    for (decltype(len) i = 0; i < len; ++i)
                    {
                        duk_get_prop_index(ctx, 0, i);
                        sounds.push_back(duk_to_string(ctx, -1));
                        duk_pop(ctx);
                    }
                    OPlayRandomSound(sounds, JS_FLOAT(1, 1), JS_FLOAT(2), JS_FLOAT(3, 1));
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("playRandomSound", 4);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                OPlaySoundCue(JS_STRING(0), JS_FLOAT(1, 1), JS_FLOAT(2), JS_FLOAT(3, 1));
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("playSoundCue", 4);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto sound = OGetSound(JS_STRING(0));
                if (sound)
                {
                    newSoundInstance(ctx, sound->createInstance());
                    return 1;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("createSoundInstance", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto sound = JS_SOUND(0);
                if (sound)
                {
                    newSoundInstance(ctx, sound->createInstance());
                    return 1;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("playSoundInstance", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newTiledMap(ctx, OGetTiledMap(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getTiledMap", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto resName = JS_STRING(0);
                if (oContentManager->isResourceLoaded(resName))
                {
                    oContentManager->removeResource(OGetTiledMap(resName));
                }
                newTiledMap(ctx, OGetTiledMap(resName));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getFreshTiledMap", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto spriteAnim = OGetSpriteAnim(JS_STRING(0));
                if (spriteAnim)
                {
                    newSpriteAnimInstance(ctx, OMake<OSpriteAnimInstance>(spriteAnim));
                    return 1;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("createSpriteAnimInstance", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto spriteAnim = OGetSpriteAnim(JS_STRING(0));
                if (spriteAnim)
                {
                    auto pRet = OMake<OSpriteAnimInstance>(spriteAnim);
                    auto pUpdater = JS_UPDATER(3);
                    if (pUpdater) pRet->setUpdater(pUpdater);
                    pRet->play(JS_STRING(1), 0.0f, JS_FLOAT(2));
                    newSpriteAnimInstance(ctx, pRet);
                    return 1;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("playSpriteAnim", 4);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newParticleSystem(ctx, OGetParticleSystem(JS_STRING(0)));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getParticleSystem", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto resName = JS_STRING(0);
                if (oContentManager->isResourceLoaded(resName))
                {
                    oContentManager->removeResource(OGetParticleSystem(resName));
                }
                newParticleSystem(ctx, OGetParticleSystem(resName));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getFreshParticleSystem", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto pPfx = JS_PARTICLE_SYSTEM(0);
                auto pos = JS_VECTOR3(1);
                auto dir = JS_VECTOR3(2, Vector3::UnitZ);
                if (pPfx)
                {
                    newParticleEmitter(ctx, oParticleSystemManager->emit(pPfx, pos, dir));
                    return 1;
                }
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("emitParticles", 3);

            // UIs
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto UIfilename = JS_STRING(0);
                if (UImap.find(UIfilename) == UImap.end())
                {
                    UImap[UIfilename] = OUIControl::createFromFile(UIfilename);
                    oUI->add(UImap[UIfilename]);
                }
                newUI(ctx, UImap[UIfilename]);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("loadUI", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto UIfilename = JS_STRING(0);
                if (UImap.find(UIfilename) == UImap.end()) return 0; // Not loaded
                oUI->remove(UImap[UIfilename]);
                UImap.erase(UImap.find(UIfilename));
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("unloadUI", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto pUIControl = OFindUI(JS_STRING(0));
                if (pUIControl)
                {
                    newUI(ctx, pUIControl);
                    return 1;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("findUI", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                oUIContext->useNavigation = JS_BOOL(0, false);
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("setUINavigation", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                newUI(ctx, oUI);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("getRootUI", 0);

            // Some enums
#define JS_ENUM(__name__, __val__) duk_push_uint(ctx, (duk_uint_t)__val__); duk_put_prop_string(ctx, -2, __name__)
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("OPAQUE", OBlendOpaque);
                JS_ENUM("ALPHA", OBlendAlpha);
                JS_ENUM("ADD", OBlendAdd);
                JS_ENUM("PREMULTIPLIED", OBlendPreMultiplied);
                JS_ENUM("MULTIPLY", OBlendMultiply);
                JS_ENUM("FORCE_WRITE", OBlendForceWrite);
            }
            JS_INTERFACE_END("BlendMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("NEAREST", OFilterNearest);
                JS_ENUM("LINEAR", OFilterLinear);
            }
            JS_INTERFACE_END("FilterMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("WRAP", onut::sample::AddressMode::Wrap);
                JS_ENUM("CLAMP", onut::sample::AddressMode::Clamp);
            }
            JS_INTERFACE_END("WrapMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("POINT_LIST", OPrimitivePointList);
                JS_ENUM("LINE_LIST", OPrimitiveLineList);
                JS_ENUM("LINE_STRIP", OPrimitiveLineStrip);
                JS_ENUM("TRIANGLE_LIST", OPrimitiveTriangleList);
                JS_ENUM("TRIANGLE_STRIP", OPrimitiveTriangleStrip);
            }
            JS_INTERFACE_END("PrimitiveMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("ESCAPE", onut::Input::State::KeyEscape);
                JS_ENUM("_1", onut::Input::State::Key1);
                JS_ENUM("_2", onut::Input::State::Key2);
                JS_ENUM("_3", onut::Input::State::Key3);
                JS_ENUM("_4", onut::Input::State::Key4);
                JS_ENUM("_5", onut::Input::State::Key5);
                JS_ENUM("_6", onut::Input::State::Key6);
                JS_ENUM("_7", onut::Input::State::Key7);
                JS_ENUM("_8", onut::Input::State::Key8);
                JS_ENUM("_9", onut::Input::State::Key9);
                JS_ENUM("_0", onut::Input::State::Key0);
                JS_ENUM("MINUS", onut::Input::State::KeyMinus); 
                JS_ENUM("EQUALS", onut::Input::State::KeyEquals);
                JS_ENUM("BACKSPACE", onut::Input::State::KeyBackspace); 
                JS_ENUM("TAB", onut::Input::State::KeyTab);
                JS_ENUM("Q", onut::Input::State::KeyQ);
                JS_ENUM("W", onut::Input::State::KeyW);
                JS_ENUM("E", onut::Input::State::KeyE);
                JS_ENUM("R", onut::Input::State::KeyR);
                JS_ENUM("T", onut::Input::State::KeyT);
                JS_ENUM("Y", onut::Input::State::KeyY);
                JS_ENUM("U", onut::Input::State::KeyU);
                JS_ENUM("I", onut::Input::State::KeyI);
                JS_ENUM("O", onut::Input::State::KeyO);
                JS_ENUM("P", onut::Input::State::KeyP);
                JS_ENUM("LEFT_BRACKET", onut::Input::State::KeyLeftBracket);
                JS_ENUM("RIGHT_BRACKET", onut::Input::State::KeyRightBracket);
                JS_ENUM("ENTER", onut::Input::State::KeyEnter); 
                JS_ENUM("LEFT_CONTROL", onut::Input::State::KeyLeftControl);
                JS_ENUM("A", onut::Input::State::KeyA);
                JS_ENUM("S", onut::Input::State::KeyS);
                JS_ENUM("D", onut::Input::State::KeyD);
                JS_ENUM("F", onut::Input::State::KeyF);
                JS_ENUM("G", onut::Input::State::KeyG);
                JS_ENUM("H", onut::Input::State::KeyH);
                JS_ENUM("J", onut::Input::State::KeyJ);
                JS_ENUM("K", onut::Input::State::KeyK);
                JS_ENUM("L", onut::Input::State::KeyL);
                JS_ENUM("SEMI_COLON", onut::Input::State::KeySemiColon);
                JS_ENUM("APOSTROPHE", onut::Input::State::KeyApostrophe);
                JS_ENUM("GRAVE", onut::Input::State::KeyGrave);    
                JS_ENUM("LEFT_SHIFT", onut::Input::State::KeyLeftShift);
                JS_ENUM("BACL_SLASH", onut::Input::State::KeyBackslash);
                JS_ENUM("Z", onut::Input::State::KeyZ);
                JS_ENUM("X", onut::Input::State::KeyX);
                JS_ENUM("C", onut::Input::State::KeyC);
                JS_ENUM("V", onut::Input::State::KeyV);
                JS_ENUM("B", onut::Input::State::KeyB);
                JS_ENUM("N", onut::Input::State::KeyN);
                JS_ENUM("M", onut::Input::State::KeyM);
                JS_ENUM("COMMA", onut::Input::State::KeyComma);
                JS_ENUM("PERIOD", onut::Input::State::KeyPeriod); 
                JS_ENUM("SLASH", onut::Input::State::KeySlash); 
                JS_ENUM("RIGHT_SHIFT", onut::Input::State::KeyRightShift);
                JS_ENUM("MULTIPLY", onut::Input::State::KeyMultiply); 
                JS_ENUM("LEFT_ALT", onut::Input::State::KeyLeftAlt); 
                JS_ENUM("SPACE_BAR", onut::Input::State::KeySpaceBar);
                JS_ENUM("CAPS_LOCK", onut::Input::State::KeyCapsLock);
                JS_ENUM("F1", onut::Input::State::KeyF1);
                JS_ENUM("F2", onut::Input::State::KeyF2);
                JS_ENUM("F3", onut::Input::State::KeyF3);
                JS_ENUM("F4", onut::Input::State::KeyF4);
                JS_ENUM("F5", onut::Input::State::KeyF5);
                JS_ENUM("F6", onut::Input::State::KeyF6);
                JS_ENUM("F7", onut::Input::State::KeyF7);
                JS_ENUM("F8", onut::Input::State::KeyF8);
                JS_ENUM("F9", onut::Input::State::KeyF9);
                JS_ENUM("F10", onut::Input::State::KeyF10);
                JS_ENUM("NUM_LOCK", onut::Input::State::KeyNumLock);
                JS_ENUM("SCROLL_LOCK", onut::Input::State::KeyScrollLock); 
                JS_ENUM("NUM_PAD_7", onut::Input::State::KeyNumPad7);
                JS_ENUM("NUM_PAD_8", onut::Input::State::KeyNumPad8);
                JS_ENUM("NUM_PAD_9", onut::Input::State::KeyNumPad9);
                JS_ENUM("NUM_PAD_MINUS", onut::Input::State::KeyNumPadMinus); 
                JS_ENUM("NUM_PAD_4", onut::Input::State::KeyNumPad4);
                JS_ENUM("NUM_PAD_5", onut::Input::State::KeyNumPad5);
                JS_ENUM("NUM_PAD_6", onut::Input::State::KeyNumPad6);
                JS_ENUM("NUM_PAD_ADD", onut::Input::State::KeyNumPadAdd); 
                JS_ENUM("NUM_PAD_1", onut::Input::State::KeyNumPad1);
                JS_ENUM("NUM_PAD_2", onut::Input::State::KeyNumPad2);
                JS_ENUM("NUM_PAD_3", onut::Input::State::KeyNumPad3);
                JS_ENUM("NUM_PAD_0", onut::Input::State::KeyNumPad0);
                JS_ENUM("NUM_PAD_PERIOD", onut::Input::State::KeyNumPadPeriod); 
                JS_ENUM("OEM102", onut::Input::State::KeyOEM102); 
                JS_ENUM("F11", onut::Input::State::KeyF11);
                JS_ENUM("F12", onut::Input::State::KeyF12);
                JS_ENUM("F13", onut::Input::State::KeyF13); 
                JS_ENUM("F14", onut::Input::State::KeyF14); 
                JS_ENUM("F15", onut::Input::State::KeyF15); 
                JS_ENUM("KANA", onut::Input::State::KeyKana); 
                JS_ENUM("ABNT_C1", onut::Input::State::KeyAbntC1); 
                JS_ENUM("CONVERT", onut::Input::State::KeyConvert); 
                JS_ENUM("NO_CONVERT", onut::Input::State::KeyNoConvert); 
                JS_ENUM("YEN", onut::Input::State::KeyYen); 
                JS_ENUM("ABNT_C2", onut::Input::State::KeyAbntC2); 
                JS_ENUM("NUM_PAD_EQUALS", onut::Input::State::KeyNumPadEquals); 
                JS_ENUM("PREVIOUS_TRACK", onut::Input::State::KeyPreviousTrack); 
                JS_ENUM("AT", onut::Input::State::KeyAt); 
                JS_ENUM("COLON", onut::Input::State::KeyColon); 
                JS_ENUM("UNDERLINE", onut::Input::State::KeyUnderline); 
                JS_ENUM("KANJI", onut::Input::State::KeyKanji); 
                JS_ENUM("STOP", onut::Input::State::KeyStop); 
                JS_ENUM("AX", onut::Input::State::KeyAx); 
                JS_ENUM("UNLABELED", onut::Input::State::KeyUnlabeled); 
                JS_ENUM("NEXT_TRACK", onut::Input::State::KeyNextTrack); 
                JS_ENUM("NUM_PAD_ENTER", onut::Input::State::KeyNumPadEnter); 
                JS_ENUM("RIGHT_CONTROL", onut::Input::State::KeyRightControl);
                JS_ENUM("MUTE", onut::Input::State::KeyMute); 
                JS_ENUM("CALCULATOR", onut::Input::State::KeyCalculator); 
                JS_ENUM("PLAY_PAUSE", onut::Input::State::KeyPlayPause); 
                JS_ENUM("MEDIA_STOP", onut::Input::State::KeyMediaStop); 
                JS_ENUM("VOLUME_DOWN", onut::Input::State::KeyVolumeDown); 
                JS_ENUM("VOLUME_UP", onut::Input::State::KeyVolumeUp); 
                JS_ENUM("WEB_HOME", onut::Input::State::KeyWebHome); 
                JS_ENUM("NUM_PAD_COMMA", onut::Input::State::KeyNumPadComma);
                JS_ENUM("NUM_PAD_DIVIDE", onut::Input::State::KeyNumPadDivide); 
                JS_ENUM("SYSRQ", onut::Input::State::Key_SYSRQ);
                JS_ENUM("RIGHT_ALT", onut::Input::State::KeyRightAlt); 
                JS_ENUM("ALT_CAR", onut::Input::State::KeyAltCar); 
                JS_ENUM("PAUSE", onut::Input::State::KeyPause); 
                JS_ENUM("HOME", onut::Input::State::KeyHome); 
                JS_ENUM("UP", onut::Input::State::KeyUp); 
                JS_ENUM("PAGE_UP", onut::Input::State::KeyPageUp); 
                JS_ENUM("LEFT", onut::Input::State::KeyLeft); 
                JS_ENUM("RIGHT", onut::Input::State::KeyRight); 
                JS_ENUM("END", onut::Input::State::KeyEnd); 
                JS_ENUM("DOWN", onut::Input::State::KeyDown); 
                JS_ENUM("PAGE_DOWN", onut::Input::State::KeyPageDown); 
                JS_ENUM("INSERT", onut::Input::State::KeyInsert); 
                JS_ENUM("DELETE", onut::Input::State::KeyDelete); 
                JS_ENUM("LEFT_WINDOWS", onut::Input::State::KeyLeftWindows); 
                JS_ENUM("RIGHT_WINDOWS", onut::Input::State::KeyRightWindows); 
                JS_ENUM("APP_MENU", onut::Input::State::KeyAppMenu); 
                JS_ENUM("POWER", onut::Input::State::KeyPower); 
                JS_ENUM("SLEEP", onut::Input::State::KeySleep); 
                JS_ENUM("WAKE", onut::Input::State::KeyWake); 
                JS_ENUM("WEB_SEARCH", onut::Input::State::KeyWebSearch); 
                JS_ENUM("WEB_FAVORITES", onut::Input::State::KeyWebFavorites); 
                JS_ENUM("WEB_REFRESH", onut::Input::State::KeyWebRefresh); 
                JS_ENUM("WEB_STOP", onut::Input::State::KeyWebStop); 
                JS_ENUM("WEB_FORWARD", onut::Input::State::KeyWebForward); 
                JS_ENUM("WEB_BACK", onut::Input::State::KeyWebBack); 
                JS_ENUM("MY_COMPUTER", onut::Input::State::KeyMyComputer); 
                JS_ENUM("MAIL_L", onut::Input::State::KeyMailL); 
                JS_ENUM("MEDIA_SELECT", onut::Input::State::KeyMediaSelect); 
                JS_ENUM("CIRCOMFLEX", onut::Input::State::KeyCircomflex);
                JS_ENUM("MOUSE_1", onut::Input::State::Mouse1);
                JS_ENUM("MOUSE_2", onut::Input::State::Mouse2);
                JS_ENUM("MOUSE_3", onut::Input::State::Mouse3);
                JS_ENUM("MOUSE_4", onut::Input::State::Mouse4);
                JS_ENUM("MOUSE_X", onut::Input::State::MouseX);
                JS_ENUM("MOUSE_Y", onut::Input::State::MouseY);
                JS_ENUM("MOUSE_Z", onut::Input::State::MouseZ);
                JS_ENUM("MOUSE_W", onut::Input::State::MouseW);
                JS_ENUM("XARCADE_LEFT_PADDLE", onut::Input::State::XArcadeLeftPaddle);
                JS_ENUM("XARCADE_RIGHT_PADDLE", onut::Input::State::XArcadeRightPaddle);
                JS_ENUM("XARCADE_1_PLAYER", onut::Input::State::XArcade1Player);
                JS_ENUM("XARCADE_2_PLAYER", onut::Input::State::XArcade2Player);
                JS_ENUM("XARCADE_LJOY_LEFT", onut::Input::State::XArcadeLJoyLeft);
                JS_ENUM("XARCADE_LJOY_RIGHT", onut::Input::State::XArcadeLJoyRight);
                JS_ENUM("XARCADE_LJOY_UP", onut::Input::State::XArcadeLJoyUp);
                JS_ENUM("XARCADE_LJOY_DOWN", onut::Input::State::XArcadeLJoyDown);
                JS_ENUM("XARCADE_RJOY_LEFT", onut::Input::State::XArcadeRJoyLeft);
                JS_ENUM("XARCADE_RJOY_RIGHT", onut::Input::State::XArcadeRJoyRight);
                JS_ENUM("XARCADE_RJOY_UP", onut::Input::State::XArcadeRJoyUp);
                JS_ENUM("XARCADE_RJOY_DOWN", onut::Input::State::XArcadeRJoyDown);
                JS_ENUM("XARCADE_LBUTTON_1", onut::Input::State::XArcadeLButton1);
                JS_ENUM("XARCADE_LBUTTON_2", onut::Input::State::XArcadeLButton2);
                JS_ENUM("XARCADE_LBUTTON_3", onut::Input::State::XArcadeLButton3);
                JS_ENUM("XARCADE_LBUTTON_4", onut::Input::State::XArcadeLButton4);
                JS_ENUM("XARCADE_LBUTTON_5", onut::Input::State::XArcadeLButton5);
                JS_ENUM("XARCADE_LBUTTON_6", onut::Input::State::XArcadeLButton6);
                JS_ENUM("XARCADE_LBUTTON_7", onut::Input::State::XArcadeLButton7);
                JS_ENUM("XARCADE_LBUTTON_8", onut::Input::State::XArcadeLButton8);
                JS_ENUM("XARCADE_RBUTTON_1", onut::Input::State::XArcadeRButton1);
                JS_ENUM("XARCADE_RBUTTON_2", onut::Input::State::XArcadeRButton2);
                JS_ENUM("XARCADE_RBUTTON_3", onut::Input::State::XArcadeRButton3);
                JS_ENUM("XARCADE_RBUTTON_4", onut::Input::State::XArcadeRButton4);
                JS_ENUM("XARCADE_RBUTTON_5", onut::Input::State::XArcadeRButton5);
                JS_ENUM("XARCADE_RBUTTON_6", onut::Input::State::XArcadeRButton6);
                JS_ENUM("XARCADE_RBUTTON_7", onut::Input::State::XArcadeRButton7);
                JS_ENUM("XARCADE_RBUTTON_8", onut::Input::State::XArcadeRButton8);
            }
            JS_INTERFACE_END("Key");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("A", onut::GamePad::A);
                JS_ENUM("B", onut::GamePad::B);
                JS_ENUM("X", onut::GamePad::X);
                JS_ENUM("Y", onut::GamePad::Y);
                JS_ENUM("DPAD_UP", onut::GamePad::DPadUp);
                JS_ENUM("DPAD_DOWN", onut::GamePad::DPadDown);
                JS_ENUM("DPAD_LEFT", onut::GamePad::DPadLeft);
                JS_ENUM("DPAD_RIGHT", onut::GamePad::DPadRight);
                JS_ENUM("LEFT_TRIGGER", onut::GamePad::LeftTrigger);
                JS_ENUM("RIGHT_TRIGGER", onut::GamePad::RightTrigger);
                JS_ENUM("LEFT_BUMPER", onut::GamePad::LeftBumper);
                JS_ENUM("RIGHT_BUMPER", onut::GamePad::RightBumper);
                JS_ENUM("LEFT_THUMBSTICK", onut::GamePad::LeftThumbStick);
                JS_ENUM("RIGHT_THUMBSTICK", onut::GamePad::RightThumbStick);
                JS_ENUM("START", onut::GamePad::Start);
                JS_ENUM("BACK", onut::GamePad::Back);
                JS_ENUM("LEFT_THUMBSTICK_LEFT", onut::GamePad::LeftThumbStickLeft);
                JS_ENUM("LEFT_THUMBSTICK_RIGHT", onut::GamePad::LeftThumbStickRight);
                JS_ENUM("LEFT_THUMBSTICK_UP", onut::GamePad::LeftThumbStickUp);
                JS_ENUM("LEFT_THUMBSTICK_DOWN", onut::GamePad::LeftThumbStickDown);
                JS_ENUM("RIGHT_THUMBSTICK_LEFT", onut::GamePad::RightThumbStickLeft);
                JS_ENUM("RIGHT_THUMBSTICK_RIGHT", onut::GamePad::RightThumbStickRight);
                JS_ENUM("RIGHT_THUMBSTICK_UP", onut::GamePad::RightThumbStickUp);
                JS_ENUM("RIGHT_THUMBSTICK_DOWN", onut::GamePad::RightThumbStickDown);
            }
            JS_INTERFACE_END("Button");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("NONE", OTweenNone);
                JS_ENUM("LINEAR", OTweenLinear);
                JS_ENUM("EASE_IN", OTweenEaseIn);
                JS_ENUM("EASE_OUT", OTweenEaseOut);
                JS_ENUM("EASE_BOTH", OTweenEaseBoth);
                JS_ENUM("BOUNCE_IN", OTweenBounceIn);
                JS_ENUM("BOUNCE_OUT", OTweenBounceOut);
                JS_ENUM("SPRING_IN", OTweenSpringIn);
                JS_ENUM("SPRING_OUT", OTweenSpringOut);
            }
            JS_INTERFACE_END("Tween");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("NONE", ODontLoop);
                JS_ENUM("LOOP", OLoop);
                JS_ENUM("PING_PONG", OPingPong);
                JS_ENUM("PING_PONG_LOOP", OPingPongLoop);
            }
            JS_INTERFACE_END("Loop");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("UNKONWN", 0);
                JS_ENUM("WINDOWS", 1);
                JS_ENUM("OSX", 2);
                JS_ENUM("LINUX", 3);
            }
            JS_INTERFACE_END("Platform");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("STRAIGHT", 0);
                JS_ENUM("ALLOW_DIAGONAL", OTiledMap::PATH_ALLOW_DIAGONAL);
                JS_ENUM("CROSS_CORNERS", OTiledMap::PATH_ALLOW_DIAGONAL | OTiledMap::PATH_CROSS_CORNERS);
            }
            JS_INTERFACE_END("PathType");
            JS_INTERFACE_BEGIN();
            {
                JS_ENUM("X", Axis::X);
                JS_ENUM("Y", Axis::Y);
                JS_ENUM("Z", Axis::Z);
            }
            JS_INTERFACE_END("Axis");

            // System
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
#if defined(__unix__)
    #if defined(__APPLE__)
                    duk_push_int(ctx, 2);
    #else
                    duk_push_int(ctx, 3);
    #endif
#elif defined(WIN32)
                    duk_push_int(ctx, 1);
#else
                    duk_push_int(ctx, 0);
#endif
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getPlatform", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSettings->setBorderlessFullscreen(JS_BOOL(0, true));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setFullscreen", 1);
            }
            JS_INTERFACE_END("System");


            // Component bullshit
       /*     JS_GLOBAL_FUNCTION_BEGIN
            {
                auto componentName = JS_STRING(0);
                if (duk_get_global_string(ctx, componentName))
                {
                    auto pJSComponentType = OMake<JSComponentType>();

                    // Grab constructor heap pointer
                    pJSComponentType->pConstructor = duk_get_heapptr(ctx, -1);

                    // Grab it's prototype heap pointer
                    duk_get_prototype(ctx, -1);
                    pJSComponentType->pPrototype = duk_get_heapptr(ctx, -3);

                    // Add component prototype to it
                    addJSComponentBindings(ctx);

                    // Check it's callbacks
                    if (duk_get_prop_string(ctx, -3, "onCreate"))
                    {
                        pJSComponentType->p_onCreate = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onUpdate"))
                    {
                        pJSComponentType->flags |= OComponent::FLAG_UPDATABLE;
                        pJSComponentType->p_onUpdate = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onRender"))
                    {
                        pJSComponentType->flags |= OComponent::FLAG_RENDERABLE;
                        pJSComponentType->p_onRender = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -1, "onRender2d"))
                    {
                        pJSComponentType->flags |= OComponent::FLAG_RENDERABLE_2D;
                        pJSComponentType->p_onRender2d = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onMessage"))
                    {
                        pJSComponentType->p_onMessage = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onMessageBroadcast"))
                    {
                        pJSComponentType->flags |= OComponent::FLAG_BROADCAST_LISTENER;
                        pJSComponentType->p_onMessageBroadcast = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onAddChild"))
                    {
                        pJSComponentType->p_onAddChild = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onRemoveChild"))
                    {
                        pJSComponentType->p_onRemoveChild = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onTriggerEnter"))
                    {
                        pJSComponentType->p_onTriggerEnter = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onTriggerLeave"))
                    {
                        pJSComponentType->p_onTriggerLeave = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onEnable"))
                    {
                        pJSComponentType->p_onEnable = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onDisable"))
                    {
                        pJSComponentType->p_onDisable = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }
                    if (duk_get_prop_string(ctx, -3, "onDestroy"))
                    {
                        pJSComponentType->p_onDestroy = duk_get_heapptr(ctx, -3);
                        duk_pop(ctx);
                    }

                    // Register it
                    jsComponentTypesByName[componentName] = pJSComponentType;
                    jsComponentTypesByPointer[pJSComponentType->pConstructor] = pJSComponentType;
                }
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("registerComponent", 1);
            */
            createUpdaterBindings();
            createMathsBinding();
            createResourceBindings();
            createAnimBindings();
            createObjectBindings();
            createUIBindings();
            createFileBindings();

            createImguiBindings();
        }

        static void logJSStack(duk_context* ctx, std::string log)
        {
            // .stack, .fileName, and .lineNumber
            if (duk_is_error(ctx, -1))
            {
                /* Accessing .stack might cause an error to be thrown, so wrap this
                * access in a duk_safe_call() if it matters.
                */
                duk_get_prop_string(ctx, -1, "stack");
                std::string msg = duk_safe_to_string(ctx, -1);
                auto lines = onut::splitString(msg, '\n');
                for (size_t i = 0; i < lines.size() && i < 8; ++i)
                {
                    log += lines[i] + "\n";
                }
                duk_pop(ctx);
            }
            else
            {
                /* Non-Error value, coerce safely to string. */
                log += duk_safe_to_string(ctx, -1);
            }

            OLog(log);
        }

        static bool evalScripts()
        {
            // Search for all scripts
            auto& searchPaths = oContentManager->getSearchPaths();
            std::set<std::string> scriptFilenames;
            std::string mainJS;
            for (auto& searchPath : searchPaths)
            {
                auto ret = onut::findAllFiles(searchPath, "js");
                for (auto& filename : ret)
                {
                    if (onut::toUpper(onut::getFilename(filename)) == "MAIN.JS")
                    {
                        mainJS = filename;
                        continue;
                    }
                    scriptFilenames.insert(filename);
                }
            }

            // Execute them
            for (auto& filename : scriptFilenames)
            {
                FILE* pFile = fopen(filename.c_str(), "rb");
                fseek(pFile, 0, SEEK_END);
                auto len = ftell(pFile);
                fseek(pFile, 0, SEEK_SET);
                char* str = new char[len + 1];
                fread(str, 1, len, pFile);
                str[len] = '\0';
                fclose(pFile);
                if (duk_peval_string(pContext, str) != 0)
                {
                    logJSStack(pContext, onut::getFilename(filename) + std::string(", eval failed: "));
                    duk_pop(pContext);
                    delete[] str;
                    return false;
                }
                delete[] str;
                duk_pop(pContext);
            }

            if (!mainJS.empty())
            {
                FILE* pFile = fopen(mainJS.c_str(), "rb");
                fseek(pFile, 0, SEEK_END);
                auto len = ftell(pFile);
                fseek(pFile, 0, SEEK_SET);
                char* str = new char[len + 1];
                fread(str, 1, len, pFile);
                str[len] = '\0';
                fclose(pFile);
                if (duk_peval_string(pContext, str) != 0)
                {
                    logJSStack(pContext, onut::getFilename(mainJS) + std::string(", eval failed: "));
                    duk_pop(pContext);
                    delete[] str;
                    return false;
                }
                delete[] str;
                duk_pop(pContext);
            }

            return true;
        }

        void update(float dt)
        {
            if (pUpdatePtr)
            {
                duk_push_heapptr(pContext, pUpdatePtr);
                duk_push_number(pContext, (duk_double_t)dt);
                if (duk_pcall(pContext, 1) != 0)
                {
                    logJSStack(pContext, "update, call failed: ");
                    pUpdatePtr = nullptr;
                }
                duk_pop(pContext);
            }

            // Garbage collect
            duk_gc(pContext, 0);
        }

        void render()
        {
            if (pRenderPtr)
            {
                duk_push_heapptr(pContext, pRenderPtr);
                if (duk_pcall(pContext, 0) != 0)
                {
                    logJSStack(pContext, "render, call failed: ");
                    pRenderPtr = nullptr;
                }
                duk_pop(pContext);
                if (oSpriteBatch->isInBatch()) oSpriteBatch->end(); // Maybe JS crashed in a middle of a batch
            }
        }

        void renderUI()
        {
            if (pRenderUIPtr)
            {
                duk_push_heapptr(pContext, pRenderUIPtr);
                if (duk_pcall(pContext, 0) != 0)
                {
                    logJSStack(pContext, "renderUI, call failed: ");
                    pRenderUIPtr = nullptr;
                }
                duk_pop(pContext);
            }
        }
    }
}

// Onut
#include <onut/ContentManager.h>
#include <onut/Curve.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/Files.h>
#include <onut/Font.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

// Private includes
#include "JSBindings.h"

// Third party
#include "duktape/duktape.h"

// STL
#include <set>


namespace onut
{
    namespace js
    {
        void createBindings();
        void evalScripts();

        duk_context* pContext = nullptr;

        // Global functions head pointers
        void* pUpdatePtr = nullptr;
        void* pRenderPtr = nullptr;

        // Prototypes head pointers
        void* pVector2Prototype = nullptr;
        void* pVector3Prototype = nullptr;
        void* pVector4Prototype = nullptr;
        void* pRectPrototype = nullptr;
        void* pColorPrototype = nullptr;
        void* pMatrixPrototype = nullptr;
        void* pTexturePrototype = nullptr;
        void* pFontPrototype = nullptr;

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

        static void newVector2(duk_context* ctx, const Vector2& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_heapptr(ctx, pVector2Prototype);
            duk_set_prototype(ctx, -2);
        }

        static void newVector3(duk_context* ctx, const Vector3& val)
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

        static void newVector4(duk_context* ctx, const Vector4& val)
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

        static void newRect(duk_context* ctx, const Rect& val)
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

        static void newColor(duk_context* ctx, const Color& val)
        {
            duk_push_object(ctx);
            duk_push_number(ctx, val.x);
            duk_put_prop_string(ctx, -2, "r");
            duk_push_number(ctx, val.y);
            duk_put_prop_string(ctx, -2, "g");
            duk_push_number(ctx, val.z);
            duk_put_prop_string(ctx, -2, "b");
            duk_push_number(ctx, val.w);
            duk_put_prop_string(ctx, -2, "a");
            duk_push_heapptr(ctx, pColorPrototype);
            duk_set_prototype(ctx, -2);
        }

        static void newMatrix(duk_context* ctx, const Matrix& val)
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

        static Vector2 getVector2(duk_context *ctx, duk_idx_t index, const Vector2& default = Vector2::Zero)
        {
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(x, index);
                FLOAT_PROP(y, index);
                return Vector2(x, y);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, 0);
                return Vector2(s, s);
            }
            return default;
        }

        static Point getPoint(duk_context *ctx, duk_idx_t index, const Point& default = Point(0, 0))
        {
            if (duk_is_object(ctx, index))
            {
                INT_PROP(x, index);
                INT_PROP(y, index);
                return Point(x, y);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = duk_to_int(ctx, 0);
                return Point(s, s);
            }
            return default;
        }

        static Vector3 getVector3(duk_context *ctx, duk_idx_t index, const Vector3& default = Vector3::Zero)
        {
            if (duk_is_object(ctx, index))
            {
                FLOAT_PROP(x, index);
                FLOAT_PROP(y, index);
                FLOAT_PROP(z, index);
                return Vector3(x, y, z);
            }
            else if (duk_is_number(ctx, index))
            {
                auto s = (float)duk_to_number(ctx, 0);
                return Vector3(s, s, s);
            }
            return default;
        }

        static Vector4 getVector4(duk_context *ctx, duk_idx_t index, const Vector4& default = Vector4::Zero)
        {
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
                auto s = (float)duk_to_number(ctx, 0);
                return Vector4(s, s, s, s);
            }
            return default;
        }

        static Vector4 getRect(duk_context *ctx, duk_idx_t index, const Vector4& default = Vector4::Zero)
        {
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
                auto s = (float)duk_to_number(ctx, 0);
                return Rect(s, s, s, s);
            }
            return default;
        }

        static Color getColor(duk_context *ctx, duk_idx_t index, const Color& default = Color::White)
        {
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
                auto s = (float)duk_to_number(ctx, 0);
                return Color(s, s, s, s);
            }
            return default;
        }

        static Matrix getMatrix(duk_context *ctx, duk_idx_t index, const Matrix& default = Matrix::Identity)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
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

        static bool getBool(duk_context *ctx, duk_idx_t index, bool default = false)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            return duk_to_boolean(ctx, index) == 1 ? true : false;
        }

        static float getFloat(duk_context *ctx, duk_idx_t index, float default = 0.0f)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            return (float)duk_to_number(ctx, index);
        }

        static unsigned int getUInt(duk_context *ctx, duk_idx_t index, unsigned int default = 0)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            return duk_to_uint(ctx, index);
        }

        static const char* getString(duk_context *ctx, duk_idx_t index, const char* default = "")
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            return duk_to_string(ctx, index);
        }

        static OTextureRef getTexture(duk_context *ctx, duk_idx_t index)
        {
            if (duk_is_null_or_undefined(ctx, index)) return nullptr;

            duk_get_prop_string(ctx, index, "\xff""\xff""data");
            auto ppTexture = (OTextureRef*)(duk_to_pointer(ctx, -1));
            duk_pop(ctx);

            if (ppTexture)
            {
                return *ppTexture;
            }
            else
            {
                return nullptr;
            }
        }

        static OFontRef getFont(duk_context *ctx, duk_idx_t index)
        {
            if (duk_is_null_or_undefined(ctx, index)) return nullptr;

            duk_get_prop_string(ctx, index, "\xff""\xff""data");
            auto ppFont = (OFontRef*)(duk_to_pointer(ctx, -1));
            duk_pop(ctx);

            if (ppFont)
            {
                return *ppFont;
            }
            else
            {
                return nullptr;
            }
        }

#define JS_GLOBAL_FUNCTION_BEGIN duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
#define JS_GLOBAL_FUNCTION_PROPS_BEGIN(__argcnt__) , __argcnt__)
#define JS_GLOBAL_FUNCTION_PROPS_END(__name__) duk_put_global_string(ctx, __name__)
#define JS_GLOBAL_FUNCTION_END(__name__, __argcnt__) , __argcnt__); duk_put_global_string(ctx, __name__)

#define JS_INTERFACE_BEGIN() duk_push_object(ctx)
#define JS_INTERFACE_END(__name__) duk_put_global_string(ctx, __name__)
#define JS_INTERFACE_FUNCTION_BEGIN duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
#define JS_INTERFACE_FUNCTION_END(__name__, __argcnt__) , __argcnt__); duk_put_prop_string(ctx, 0, __name__)

#define JS_OBJECT_BEGIN() duk_push_object(ctx)
#define JS_OBJECT_END(__name__) duk_put_prop_string(ctx, -2, __name__)
#define JS_ADD_FLOAT_PROP(__name__, __val__) duk_push_number(ctx, (duk_double_t)(__val__)); duk_put_prop_string(ctx, -2, __name__)
#define JS_ADD_DATA_PROP(__pointer__) duk_push_pointer(ctx, __pointer__); duk_put_prop_string(ctx, -2, "\xff""\xff""data")
#define JS_ADD_VECTOR2_PROP(__name__, __val__) \
    JS_OBJECT_BEGIN(); \
    JS_ADD_FLOAT_PROP("x", __val__.x); \
    JS_ADD_FLOAT_PROP("y", __val__.y); \
    JS_OBJECT_END(__name__);
#define JS_BUILD_MATRIX_OBJECT(__matrix__) \
    JS_OBJECT_BEGIN(); \
    JS_ADD_FLOAT_PROP("_11", __matrix__._11); \
    JS_ADD_FLOAT_PROP("_12", __matrix__._12); \
    JS_ADD_FLOAT_PROP("_13", __matrix__._13); \
    JS_ADD_FLOAT_PROP("_14", __matrix__._14); \
    JS_ADD_FLOAT_PROP("_21", __matrix__._21); \
    JS_ADD_FLOAT_PROP("_22", __matrix__._22); \
    JS_ADD_FLOAT_PROP("_23", __matrix__._23); \
    JS_ADD_FLOAT_PROP("_24", __matrix__._24); \
    JS_ADD_FLOAT_PROP("_31", __matrix__._31); \
    JS_ADD_FLOAT_PROP("_32", __matrix__._32); \
    JS_ADD_FLOAT_PROP("_33", __matrix__._33); \
    JS_ADD_FLOAT_PROP("_34", __matrix__._34); \
    JS_ADD_FLOAT_PROP("_41", __matrix__._41); \
    JS_ADD_FLOAT_PROP("_42", __matrix__._42); \
    JS_ADD_FLOAT_PROP("_43", __matrix__._43); \
    JS_ADD_FLOAT_PROP("_44", __matrix__._44)
#define JS_BUILD_COLOR_OBJECT(__color__) \
    JS_OBJECT_BEGIN(); \
    JS_ADD_FLOAT_PROP("r", __color__.x); \
    JS_ADD_FLOAT_PROP("g", __color__.y); \
    JS_ADD_FLOAT_PROP("b", __color__.z); \
    JS_ADD_FLOAT_PROP("a", __color__.w);
#define JS_BUILD_VECTOR2_OBJECT(__val__) \
    JS_OBJECT_BEGIN(); \
    JS_ADD_FLOAT_PROP("x", __val__.x); \
    JS_ADD_FLOAT_PROP("y", __val__.y); \

#define JS_TEXTURE(__index__) getTexture(ctx, __index__)
#define JS_FONT(__index__) getFont(ctx, __index__)
#define JS_RECT(...) getRect(ctx, __VA_ARGS__)
#define JS_VECTOR2(...) getVector2(ctx, __VA_ARGS__)
#define JS_VECTOR3(...) getVector3(ctx, __VA_ARGS__)
#define JS_VECTOR4(...) getVector4(ctx, __VA_ARGS__)
#define JS_COLOR(...) getColor(ctx, __VA_ARGS__)
#define JS_BOOL(...) getBool(ctx, __VA_ARGS__)
#define JS_FLOAT(...) (float)getFloat(ctx, __VA_ARGS__)
#define JS_STRING(...) getString(ctx, __VA_ARGS__)
#define JS_UINT(...) getUInt(ctx, __VA_ARGS__)
#define JS_MATRIX(...) getMatrix(ctx, __VA_ARGS__)

        void init()
        {
            pContext = duk_create_heap_default();

            createBindings();
            evalScripts();

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
        }

        void shutdown()
        {
            duk_destroy_heap(pContext);
            pContext = nullptr;
        }

        void createVector2Bindings()
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
                newVector2(ctx, Vector2::Max(v2, v2));
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

        void createVector3Bindings()
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
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR3;
                duk_push_number(ctx, (duk_double_t)v.Dot(other));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "dot");

            // cross(other)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto other = getVector2(ctx, 0);
                JS_THIS_VECTOR3;
                newVector2(ctx, v.Cross(other));
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
                newVector3(ctx, Vector3::Max(v2, v2));
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

        void createVector4Bindings()
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
                newVector4(ctx, Vector4::Max(v2, v2));
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

        void createRectBindings()
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

            // Done with the object
            pRectPrototype = duk_get_heapptr(ctx, -1);
            duk_put_prop_string(ctx, -2, "prototype");

            duk_put_global_string(ctx, "Rect");
        }

        void createColorBindings()
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
                newColor(ctx, Color(v.x / other.x, v.y / other.y, v.z / other.z, v.w / other.w));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "div");

            // toVector3
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                newVector3(ctx, Vector3(v.x, v.y, v.z));
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "toVector3");

            // toVector4
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                JS_THIS_COLOR;
                newVector4(ctx, Vector4(v.x, v.y, v.z, v.w));
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
            pVector4Prototype = duk_get_heapptr(ctx, -1);
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

        void createMatrixBindings()
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
                newMatrix(ctx, Matrix::CreateRotationX(DirectX::XMConvertToRadians(degrees)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRotationX");

            // createRotationY(degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto degrees = JS_FLOAT(0);
                newMatrix(ctx, Matrix::CreateRotationY(DirectX::XMConvertToRadians(degrees)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRotationY");

            // createRotationZ(degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto degrees = JS_FLOAT(0);
                newMatrix(ctx, Matrix::CreateRotationZ(DirectX::XMConvertToRadians(degrees)));
                return 1;
            }, 1);
            duk_put_prop_string(ctx, -2, "createRotationZ");

            // createFromAxisAngle(axis, degrees)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto axis = getVector3(ctx, 0);
                auto degrees = JS_FLOAT(1);
                newMatrix(ctx, Matrix::CreateFromAxisAngle(axis, DirectX::XMConvertToRadians(degrees)));
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

            // Constants
            newMatrix(ctx, Matrix::Identity);
            duk_put_prop_string(ctx, -2, "IDENTITY");

            duk_put_global_string(ctx, "Matrix");
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
        
        static void newFont(duk_context* ctx, const OFontRef& pFont)
        {
            duk_push_object(ctx);
            auto ppFont = new OFontRef(pFont);
            duk_push_pointer(ctx, ppFont);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_heapptr(ctx, pFontPrototype);
            duk_set_prototype(ctx, -2);
        }

        void createTextureBindings()
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
                auto saturation = getFloat(ctx, 0, 0);
                auto sepiaAmount = getFloat(ctx, 0, .75f);
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

        void createFontBindings()
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

        void createMathsBinding()
        {
            createVector2Bindings();
            createVector3Bindings();
            createVector4Bindings();
            createRectBindings();
            createColorBindings();
            createMatrixBindings();
        }

        void createResourceBindings()
        {
            createTextureBindings();
            createFontBindings();
        }
        
        void createTiledMapBindings()
        {
            auto ctx = pContext;

            // TiledMap()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pTiledMap = OGetTiledMap(JS_STRING(0));
                if (!pTiledMap) return DUK_ERR_INTERNAL_ERROR;

                auto ppTiledMap = new OTiledMapRef(pTiledMap);
                duk_push_this(ctx);
                duk_push_pointer(ctx, ppTiledMap);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");

                return 0;
            }, 1);
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
                    (*ppTiledMap)->render();
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "render");

            // renderLayer(name)
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_pop(ctx);
                    auto layerName = duk_to_string(ctx, 0);
                    if (layerName)
                    {
                        auto pLayer = (*ppTiledMap)->getLayer(layerName);
                        if (pLayer)
                        {
                            (*ppTiledMap)->renderLayer(pLayer);
                        }
                    }
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "renderLayer");

            // getWidth
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppTiledMap)->getWidth());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getWidth");

            // getHeight
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppTiledMap = (OTiledMapRef*)duk_to_pointer(ctx, -1);
                if (ppTiledMap)
                {
                    duk_push_number(ctx, (duk_double_t)(*ppTiledMap)->getHeight());
                    return 1;
                }
                return 0;
            }, 0);
            duk_put_prop_string(ctx, -2, "getHeight");

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

            // Done with the object
            duk_put_prop_string(ctx, -2, "prototype");
            duk_put_global_string(ctx, "TiledMap");
        }

        void addComponentPrototype(duk_context *ctx)
        {
            // ~Component()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_get_prop_string(ctx, 0, "\xff""\xff""data");
                auto ppComponent = (OComponentRef*)duk_to_pointer(ctx, -1);
                if (ppComponent)
                {
                    delete ppComponent;
                    duk_pop(ctx);
                    duk_push_pointer(ctx, nullptr);
                    duk_put_prop_string(ctx, 0, "\xff""\xff""data");
                }
                return 0;
            }, 1);
            duk_set_finalizer(ctx, -2);
        }

        void createTiledMapComponentPrototype()
        {
            auto ctx = pContext;
            duk_push_object(ctx);

            // TiledMapComponent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                if (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;

                auto pTiledMap = OGetTiledMap(JS_STRING(0));
                if (!pTiledMap) return DUK_ERR_INTERNAL_ERROR;

                auto ppTiledMap = new OTiledMapRef(pTiledMap);
                duk_push_this(ctx);
                duk_push_pointer(ctx, ppTiledMap);
                duk_put_prop_string(ctx, -2, "\xff""\xff""data");

                return 0;
            }, 1);
            duk_push_object(ctx);

            addComponentPrototype(ctx);
        }
        
        void createEntityPrototype()
        {
            auto ctx = pContext;

            duk_push_object(ctx);

            // ~Entity()
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

            // getComponent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                    int tmp;
                    tmp = 5;
                }
                return 1;
            }, 0);
            duk_put_prop_string(ctx, -2, "getComponent");

            // addComponent()
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                duk_push_this(ctx);
                duk_get_prop_string(ctx, -1, "\xff""\xff""data");
                auto ppEntity = (OEntityRef*)duk_to_pointer(ctx, -1);
                if (ppEntity)
                {
                }
                return 0;
            }, 1);
            duk_put_prop_string(ctx, -2, "addComponent");

            // Done with the object
            duk_put_global_string(ctx, "\xff""\xff""Entity");
        }

        void pushEntity(duk_context *ctx, const OEntityRef& pEntity)
        {
            auto ppEntity = new OEntityRef(pEntity);

            duk_push_object(ctx);
            duk_push_pointer(ctx, ppEntity);
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            
            duk_get_global_string(ctx, "\xff""\xff""Entity");
            duk_set_prototype(ctx, -2);
        }

        void createEntityFactoryBindings()
        {
            auto ctx = pContext;
            duk_push_object(ctx);

            // create
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto position = JS_VECTOR3(0, Vector3::Zero);
                auto pEntity = OEntity::create();
                pEntity->setLocalTransform(Matrix::CreateTranslation(position));
                pushEntity(ctx, pEntity);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, 0, "create");

            // createTiledMap
            duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
            {
                auto pEntity = OCreateTiledMapEntity(JS_STRING(0));
                pushEntity(ctx, pEntity);
                return 1;
            }, 1);
            duk_put_prop_string(ctx, 0, "createTiledMap");

            duk_put_global_string(ctx, "EntityFactory");
        }

        void createBindings()
        {
            auto ctx = pContext;

            // Log function
            JS_GLOBAL_FUNCTION_BEGIN
            {
                OLog(JS_STRING(0));
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("print", 1);

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
                    JS_BUILD_VECTOR2_OBJECT(OScreenf);
                    return 1;
                }
                JS_INTERFACE_FUNCTION_END("getResolution", 0);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto pTexture = JS_TEXTURE(0);
                    oRenderer->renderStates.renderTarget.push(pTexture);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("pushRenderTarget", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->renderStates.renderTarget.pop();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("popRenderTarget", 0);
            }
            JS_INTERFACE_END("Renderer");

            // oSpriteBatch
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    auto matrix = JS_MATRIX(0);
                    oSpriteBatch->begin(matrix);
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("begin", 1);
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
                    oSpriteBatch->drawSprite(JS_TEXTURE(0), JS_MATRIX(1), JS_VECTOR2(3, Vector2::One), JS_COLOR(2), JS_VECTOR2(4, Vector2(.5f, .5f)));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("drawTransformedSprite", 5);
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
                    oSpriteBatch->changeBlendMode((onut::BlendMode)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setBlend", 1);
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->changeFiltering((onut::sample::Filtering)JS_UINT(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("setFilter", 1);
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

            createTiledMapBindings();

            // Some enums
            JS_INTERFACE_BEGIN();
            {
                JS_ADD_FLOAT_PROP("OPAQUE", (float)OBlendOpaque);
                JS_ADD_FLOAT_PROP("ALPHA", (float)OBlendAlpha);
                JS_ADD_FLOAT_PROP("ADD", (float)OBlendAdd);
                JS_ADD_FLOAT_PROP("PREMULTIPLIED", (float)OBlendPreMultiplied);
                JS_ADD_FLOAT_PROP("MULTIPLY", (float)OBlendMultiply);
                JS_ADD_FLOAT_PROP("FORCE_WRITE", (float)OBlendForceWrite);
            }
            JS_INTERFACE_END("BlendMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ADD_FLOAT_PROP("NEAREST", (float)OFilterNearest);
                JS_ADD_FLOAT_PROP("LINEAR", (float)OFilterLinear);
            }
            JS_INTERFACE_END("FilterMode");

            createMathsBinding();
            createResourceBindings();
            createEntityPrototype();
            createEntityFactoryBindings();
            createTiledMapComponentPrototype();
        }

        void evalScripts()
        {
            // Search for all scripts
            auto& searchPaths = oContentManager->getSearchPaths();
            std::set<std::string> scriptFilenames;
            for (auto& searchPath : searchPaths)
            {
                auto ret = onut::findAllFiles(searchPath, "js");
                for (auto& filename : ret)
                {
                    scriptFilenames.insert(filename);
                }
            }

            // Execute them
            for (auto& filename : scriptFilenames)
            {
                if (duk_peval_file(pContext, filename.c_str()) != 0)
                {
                    OLog(std::string("eval failed: ") + duk_safe_to_string(pContext, -1));
                }
                duk_pop(pContext);
            }
        }

        void update(float dt)
        {
            if (pUpdatePtr)
            {
                duk_push_heapptr(pContext, pUpdatePtr);
                duk_push_number(pContext, (duk_double_t)dt);
                if (duk_pcall(pContext, 1) != 0)
                {
                    OLog(std::string("call failed: ") + duk_safe_to_string(pContext, -1));
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
                    OLog(std::string("call failed: ") + duk_safe_to_string(pContext, -1));
                    pRenderPtr = nullptr;
                }
                duk_pop(pContext);
                if (oSpriteBatch->isInBatch()) oSpriteBatch->end(); // Maybe JS crashed in a middle of a batch
            }
        }
    }
}

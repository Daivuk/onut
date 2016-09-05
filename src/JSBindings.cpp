// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

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
        void* pUpdatePtr = nullptr;
        void* pRenderPtr = nullptr;

        // Helpers
#define FLOAT_PROP(__name__, __index__) \
            duk_get_prop_string(ctx, __index__, #__name__); \
            auto __name__ = (float)duk_to_number(ctx, -1); \
            duk_pop(ctx);

#define UINT_PROP(__name__, __index__) \
            duk_get_prop_string(ctx, __index__, #__name__); \
            auto __name__ = duk_to_uint(ctx, -1); \
            duk_pop(ctx);

#define STRING_PROP(__name__, __index__) \
            duk_get_prop_string(ctx, __index__, #__name__); \
            auto __name__ = duk_to_string(ctx, -1); \
            duk_pop(ctx);

        static Rect getRect(duk_context *ctx, duk_idx_t index, const Rect& default = {0, 0, 100, 100})
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(x, index);
            FLOAT_PROP(y, index);
            FLOAT_PROP(w, index);
            FLOAT_PROP(h, index);
            return Rect(x, y, w, h);
        }

        static Vector2 getVector2(duk_context *ctx, duk_idx_t index, const Vector2& default = Vector2::Zero)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(x, index);
            FLOAT_PROP(y, index);
            return Vector2(x, y);
        }

        static Vector3 getVector3(duk_context *ctx, duk_idx_t index, const Vector3& default = Vector3::Zero)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(x, index);
            FLOAT_PROP(y, index);
            FLOAT_PROP(z, index);
            return Vector3(x, y, z);
        }

        static Vector4 getVector4(duk_context *ctx, duk_idx_t index, const Vector4& default = Vector4::Zero)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(x, index);
            FLOAT_PROP(y, index);
            FLOAT_PROP(z, index);
            FLOAT_PROP(w, index);
            return Vector4(x, y, z, w);
        }

        static Matrix getMatrix(duk_context *ctx, duk_idx_t index, const Matrix& default = Matrix::Identity)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(_11, index); FLOAT_PROP(_12, index); FLOAT_PROP(_13, index); FLOAT_PROP(_14, index);
            FLOAT_PROP(_21, index); FLOAT_PROP(_22, index); FLOAT_PROP(_23, index); FLOAT_PROP(_24, index);
            FLOAT_PROP(_31, index); FLOAT_PROP(_32, index); FLOAT_PROP(_33, index); FLOAT_PROP(_34, index);
            FLOAT_PROP(_41, index); FLOAT_PROP(_42, index); FLOAT_PROP(_43, index); FLOAT_PROP(_44, index);
            return Matrix(_11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44);
        }

        static Color getColor(duk_context *ctx, duk_idx_t index, const Color& default = Color::White)
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(r, index);
            FLOAT_PROP(g, index);
            FLOAT_PROP(b, index);
            FLOAT_PROP(a, index);
            return Color(r, g, b, a);
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

            OTexture* pTexture = nullptr;

            duk_get_prop_string(ctx, index, "\xff""\xff""data");
            pTexture = (OTexture*)(duk_to_pointer(ctx, -1));
            duk_pop(ctx);

            if (pTexture)
            {
                return ((OTexture*)pTexture)->shared_from_this();
            }
            else
            {
                return nullptr;
            }
        }

        void init()
        {
            pContext = duk_create_heap_default();

            createBindings();
            evalScripts();

            // Grab global Update and Render pointers if present
            duk_get_global_string(pContext, "update");
            pUpdatePtr = duk_get_heapptr(pContext, 0);
            duk_pop(pContext);

            duk_get_global_string(pContext, "render");
            pRenderPtr = duk_get_heapptr(pContext, 0);
            duk_pop(pContext);
        }

        void shutdown()
        {
            duk_destroy_heap(pContext);
            pContext = nullptr;
        }

        void createBindings()
        {
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

#define JS_TEXTURE(__index__) getTexture(ctx, __index__)
#define JS_RECT(...) getRect(ctx, __VA_ARGS__)
#define JS_VECTOR2(...) getVector2(ctx, __VA_ARGS__)
#define JS_VECTOR3(...) getVector3(ctx, __VA_ARGS__)
#define JS_VECTOR4(...) getVector4(ctx, __VA_ARGS__)
#define JS_COLOR(...) getColor(ctx, __VA_ARGS__)
#define JS_FLOAT(...) (float)getFloat(ctx, __VA_ARGS__)
#define JS_STRING(...) getString(ctx, __VA_ARGS__)
#define JS_UINT(...) getUInt(ctx, __VA_ARGS__)
#define JS_MATRIX(...) getMatrix(ctx, __VA_ARGS__)

            auto ctx = pContext;

            // Log function
            JS_GLOBAL_FUNCTION_BEGIN
            {
                OLog(JS_STRING(0));
                return 0;
            }
            JS_GLOBAL_FUNCTION_END("print", 1);

            // Maths
            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("x", JS_FLOAT(0));
                JS_ADD_FLOAT_PROP("y", JS_FLOAT(1));
                JS_ADD_FLOAT_PROP("w", JS_FLOAT(2));
                JS_ADD_FLOAT_PROP("h", JS_FLOAT(3));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("Rect", 4);

            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("x", JS_FLOAT(0));
                JS_ADD_FLOAT_PROP("y", JS_FLOAT(1));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("Vector2", 2);

            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("x", JS_FLOAT(0));
                JS_ADD_FLOAT_PROP("y", JS_FLOAT(1));
                JS_ADD_FLOAT_PROP("z", JS_FLOAT(2));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("Vector3", 3);

            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("x", JS_FLOAT(0));
                JS_ADD_FLOAT_PROP("y", JS_FLOAT(1));
                JS_ADD_FLOAT_PROP("z", JS_FLOAT(2));
                JS_ADD_FLOAT_PROP("w", JS_FLOAT(3));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("Vector4", 4);

            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto color = Color::fromHexRGB(JS_UINT(0));
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("r", color.x);
                JS_ADD_FLOAT_PROP("g", color.y);
                JS_ADD_FLOAT_PROP("b", color.z);
                JS_ADD_FLOAT_PROP("a", color.w);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("ColorHexRGB", 1);

            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto color = Color::fromHexRGBA(JS_UINT(0));
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("r", color.x);
                JS_ADD_FLOAT_PROP("g", color.y);
                JS_ADD_FLOAT_PROP("b", color.z);
                JS_ADD_FLOAT_PROP("a", color.w);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("ColorHexRGBA", 1);

            // Colors
            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("r", JS_FLOAT(0));
                JS_ADD_FLOAT_PROP("g", JS_FLOAT(1));
                JS_ADD_FLOAT_PROP("b", JS_FLOAT(2));
                JS_ADD_FLOAT_PROP("a", JS_FLOAT(3));
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("Color", 4);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_BUILD_COLOR_OBJECT(Color::White);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("whiteColor", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_BUILD_COLOR_OBJECT(Color::Black);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("blackColor", 1);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_BUILD_COLOR_OBJECT(Color::Transparent);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("transparentColor", 1);

            // Matrices
            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_BUILD_MATRIX_OBJECT(Matrix::Identity);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("identityMatrix", 0);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto matrix = JS_MATRIX(0);
                matrix *= Matrix::CreateRotationX(DirectX::XMConvertToRadians(JS_FLOAT(1)));
                JS_BUILD_MATRIX_OBJECT(matrix);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("rotateMatrixAroundX", 2);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto matrix = JS_MATRIX(0);
                matrix *= Matrix::CreateRotationY(DirectX::XMConvertToRadians(JS_FLOAT(1)));
                JS_BUILD_MATRIX_OBJECT(matrix);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("rotateMatrixAroundY", 2);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto matrix = JS_MATRIX(0);
                matrix *= Matrix::CreateRotationZ(DirectX::XMConvertToRadians(JS_FLOAT(1)));
                JS_BUILD_MATRIX_OBJECT(matrix);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("rotateMatrixAroundZ", 2);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto matrix = JS_MATRIX(0);
                auto scaleX = JS_FLOAT(1);
                auto scaleY = JS_FLOAT(2);
                auto scaleZ = JS_FLOAT(3);
                matrix *= Matrix::CreateScale(scaleX, scaleY, scaleZ);
                JS_BUILD_MATRIX_OBJECT(matrix);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("scaleMatrix", 4);
            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto matrix = JS_MATRIX(0);
                auto scaleX = JS_FLOAT(1);
                auto scaleY = JS_FLOAT(2);
                auto scaleZ = JS_FLOAT(3);
                matrix *= Matrix::CreateTranslation(scaleX, scaleY, scaleZ);
                JS_BUILD_MATRIX_OBJECT(matrix);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("translateMatrix", 4);

            // oRenderer
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oRenderer->clear(JS_COLOR(0));
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("clear", 1);
            }
            JS_INTERFACE_END("Renderer");

            // oSpriteBatch
            JS_INTERFACE_BEGIN();
            {
                JS_INTERFACE_FUNCTION_BEGIN
                {
                    oSpriteBatch->begin();
                    return 0;
                }
                JS_INTERFACE_FUNCTION_END("begin", 0);
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
            }
            JS_INTERFACE_END("SpriteBatch");

            JS_GLOBAL_FUNCTION_BEGIN
            {
                auto pTexture = OGetTexture(JS_STRING(0));
                if (!pTexture) return 0;
                JS_OBJECT_BEGIN();
                JS_ADD_DATA_PROP(pTexture.get());
                JS_ADD_FLOAT_PROP("w", pTexture->getSize().x);
                JS_ADD_FLOAT_PROP("h", pTexture->getSize().y);
                return 1;
            }
            JS_GLOBAL_FUNCTION_END("Texture", 1);

            // Some enums
            JS_INTERFACE_BEGIN();
            {
                JS_ADD_FLOAT_PROP("Opaque", (float)OBlendOpaque);
                JS_ADD_FLOAT_PROP("Alpha", (float)OBlendAlpha);
                JS_ADD_FLOAT_PROP("Add", (float)OBlendAdd);
                JS_ADD_FLOAT_PROP("PreMultiplied", (float)OBlendPreMultiplied);
                JS_ADD_FLOAT_PROP("Multiply", (float)OBlendMultiply);
                JS_ADD_FLOAT_PROP("ForceWrite", (float)OBlendForceWrite);
            }
            JS_INTERFACE_END("BlendMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ADD_FLOAT_PROP("Nearest", (float)OFilterNearest);
                JS_ADD_FLOAT_PROP("Linear", (float)OFilterLinear);
            }
            JS_INTERFACE_END("FilterMode");
            JS_INTERFACE_BEGIN();
            {
                JS_ADD_VECTOR2_PROP("TopLeft", Vector2(OTopLeft));
                JS_ADD_VECTOR2_PROP("Top", Vector2(OTop));
                JS_ADD_VECTOR2_PROP("TopRight", Vector2(OTopRight));
                JS_ADD_VECTOR2_PROP("Left", Vector2(OLeft));
                JS_ADD_VECTOR2_PROP("Center", Vector2(OCenter));
                JS_ADD_VECTOR2_PROP("Right", Vector2(ORight));
                JS_ADD_VECTOR2_PROP("BottomLeft", Vector2(OBottomLeft));
                JS_ADD_VECTOR2_PROP("Bottom", Vector2(OBottom));
                JS_ADD_VECTOR2_PROP("BottomRight", Vector2(OBottomRight));
            }
            JS_INTERFACE_END("Align");
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

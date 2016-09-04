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

        static Rect getRect(duk_context *ctx, duk_idx_t index, const Rect& default = {0, 0, 100, 100})
        {
            if (duk_is_null_or_undefined(ctx, index)) return default;
            FLOAT_PROP(x, index);
            FLOAT_PROP(y, index);
            FLOAT_PROP(w, index);
            FLOAT_PROP(h, index);
            return Rect(x, y, w, h);
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
#define JS_ADD_FLOAT_PROP(__name__, __val__) duk_push_number(ctx, (duk_double_t)(__val__)); duk_put_prop_string(ctx, -2, __name__)
#define JS_ADD_DATA_PROP(__pointer__) duk_push_pointer(ctx, __pointer__); duk_put_prop_string(ctx, -2, "\xff""\xff""data")

#define JS_OBJECT_END(__name__) duk_put_prop_string(ctx, -2, __name__)

#define JS_TEXTURE(__index__) getTexture(ctx, __index__)
#define JS_RECT(__index__) getRect(ctx, __index__)
#define JS_COLOR(__index__) getColor(ctx, __index__)
#define JS_FLOAT(__index__) (float)duk_get_number(ctx, __index__)
#define JS_STRING(__index__) duk_get_string(ctx, __index__)
#define JS_UINT(__index__) duk_get_uint(ctx, __index__)

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

            JS_GLOBAL_FUNCTION_BEGIN
            {
                JS_OBJECT_BEGIN();
                JS_ADD_FLOAT_PROP("r", JS_FLOAT(0));
                JS_ADD_FLOAT_PROP("g", JS_FLOAT(1));
                JS_ADD_FLOAT_PROP("b", JS_FLOAT(2));
                JS_ADD_FLOAT_PROP("a", JS_FLOAT(3));
                return 1;
            }
            JS_GLOBAL_FUNCTION_PROPS_BEGIN(4);
            {
                JS_OBJECT_BEGIN();
                {
                    JS_ADD_FLOAT_PROP("r", 1);
                    JS_ADD_FLOAT_PROP("g", 1);
                    JS_ADD_FLOAT_PROP("b", 1);
                    JS_ADD_FLOAT_PROP("a", 1);
                }
                JS_OBJECT_END("White");
                JS_OBJECT_BEGIN();
                {
                    JS_ADD_FLOAT_PROP("r", 0);
                    JS_ADD_FLOAT_PROP("g", 0);
                    JS_ADD_FLOAT_PROP("b", 0);
                    JS_ADD_FLOAT_PROP("a", 1);
                }
                JS_OBJECT_END("Black");
                JS_OBJECT_BEGIN();
                {
                    JS_ADD_FLOAT_PROP("r", 0);
                    JS_ADD_FLOAT_PROP("g", 0);
                    JS_ADD_FLOAT_PROP("b", 0);
                    JS_ADD_FLOAT_PROP("a", 0);
                }
                JS_OBJECT_END("Transparent");
            }
            JS_GLOBAL_FUNCTION_PROPS_END("Color");

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
            }
        }
    }
}

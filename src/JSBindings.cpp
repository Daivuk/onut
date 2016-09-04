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

        static Rect getRect(duk_context *ctx, duk_idx_t index)
        {
            FLOAT_PROP(x, index);
            FLOAT_PROP(y, index);
            FLOAT_PROP(w, index);
            FLOAT_PROP(h, index);
            return Rect(x, y, w, h);
        }

        static Color getColor(duk_context *ctx, duk_idx_t index)
        {
            FLOAT_PROP(r, index);
            FLOAT_PROP(g, index);
            FLOAT_PROP(b, index);
            FLOAT_PROP(a, index);
            return Color(r, g, b, a);
        }

        static OTextureRef getTexture(duk_context *ctx, duk_idx_t index)
        {
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

        // Global
        duk_ret_t js_print(duk_context *ctx)
        {
            auto arg = duk_require_string(ctx, 0);
            OLog(arg);
            return 0;
        }

        duk_ret_t js_Rect_ctor(duk_context *ctx)
        {
            auto x = duk_get_number(ctx, 0);
            auto y = duk_get_number(ctx, 1);
            auto w = duk_get_number(ctx, 2);
            auto h = duk_get_number(ctx, 3);
            duk_push_object(ctx);
            duk_push_number(ctx, x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_number(ctx, w);
            duk_put_prop_string(ctx, -2, "w");
            duk_push_number(ctx, h);
            duk_put_prop_string(ctx, -2, "h");
            return 1;
        }

        duk_ret_t js_Color_ctor(duk_context *ctx)
        {
            auto r = duk_get_number(ctx, 0);
            auto g = duk_get_number(ctx, 1);
            auto b = duk_get_number(ctx, 2);
            auto a = duk_get_number(ctx, 3);
            duk_push_object(ctx);
            duk_push_number(ctx, r);
            duk_put_prop_string(ctx, -2, "r");
            duk_push_number(ctx, g);
            duk_put_prop_string(ctx, -2, "g");
            duk_push_number(ctx, b);
            duk_put_prop_string(ctx, -2, "b");
            duk_push_number(ctx, a);
            duk_put_prop_string(ctx, -2, "a");
            return 1;
        }

        duk_ret_t js_Vector2_ctor(duk_context *ctx)
        {
            auto x = duk_get_number(ctx, 0);
            auto y = duk_get_number(ctx, 1);
            duk_push_object(ctx);
            duk_push_number(ctx, x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, y);
            duk_put_prop_string(ctx, -2, "y");
            return 1;
        }

        duk_ret_t js_Vector3_ctor(duk_context *ctx)
        {
            auto x = duk_get_number(ctx, 0);
            auto y = duk_get_number(ctx, 1);
            auto z = duk_get_number(ctx, 2);
            duk_push_object(ctx);
            duk_push_number(ctx, x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_number(ctx, z);
            duk_put_prop_string(ctx, -2, "z");
            return 1;
        }

        duk_ret_t js_Vector4_ctor(duk_context *ctx)
        {
            auto x = duk_get_number(ctx, 0);
            auto y = duk_get_number(ctx, 1);
            auto z = duk_get_number(ctx, 2);
            auto w = duk_get_number(ctx, 3);
            duk_push_object(ctx);
            duk_push_number(ctx, x);
            duk_put_prop_string(ctx, -2, "x");
            duk_push_number(ctx, y);
            duk_put_prop_string(ctx, -2, "y");
            duk_push_number(ctx, z);
            duk_put_prop_string(ctx, -2, "z");
            duk_push_number(ctx, w);
            duk_put_prop_string(ctx, -2, "w");
            return 1;
        }

        // Renderer
        duk_ret_t js_Renderer_clear(duk_context *ctx)
        {
            oRenderer->clear(getColor(ctx, 0));
            return 0;
        }

        // Spritebatch
        duk_ret_t js_SpriteBatch_begin(duk_context *ctx)
        {
            oSpriteBatch->begin();
            return 0;
        }

        duk_ret_t js_SpriteBatch_end(duk_context *ctx)
        {
            oSpriteBatch->end();
            return 0;
        }

        duk_ret_t js_SpriteBatch_drawRect(duk_context *ctx)
        {
            oSpriteBatch->drawRect(nullptr, getRect(ctx, 0), getColor(ctx, 1));
            return 0;
        }

        duk_ret_t js_SpriteBatch_drawTexturedRect(duk_context *ctx)
        {
            oSpriteBatch->drawRect(getTexture(ctx, 0), getRect(ctx, 1), getColor(ctx, 2));
            return 0;
        }

        // Texture
        duk_ret_t js_Texture(duk_context *ctx)
        {
            auto arg = duk_require_string(ctx, 0);
            auto pTexture = OGetTexture(arg);
            if (!pTexture) return 0;

            duk_push_object(ctx);
            duk_push_pointer(ctx, pTexture.get());
            duk_put_prop_string(ctx, -2, "\xff""\xff""data");
            duk_push_int(ctx, pTexture->getSize().x);
            duk_put_prop_string(ctx, -2, "w");
            duk_push_int(ctx, pTexture->getSize().y);
            duk_put_prop_string(ctx, -2, "h");

            return 1;
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
            // Log function
            duk_push_c_function(pContext, js_print, 1);
            duk_put_global_string(pContext, "print");

            // Maths
            duk_push_c_function(pContext, js_Rect_ctor, 4);
            duk_put_global_string(pContext, "Rect");
            duk_push_c_function(pContext, js_Vector2_ctor, 2);
            duk_put_global_string(pContext, "Vector2");
            duk_push_c_function(pContext, js_Vector3_ctor, 3);
            duk_put_global_string(pContext, "Vector3");
            duk_push_c_function(pContext, js_Vector4_ctor, 4);
            duk_put_global_string(pContext, "Vector4");
            duk_push_c_function(pContext, js_Color_ctor, 4);
            {
                duk_push_object(pContext);
                {
                    duk_push_number(pContext, 1);
                    duk_put_prop_string(pContext, -2, "r");
                    duk_push_number(pContext, 1);
                    duk_put_prop_string(pContext, -2, "g");
                    duk_push_number(pContext, 1);
                    duk_put_prop_string(pContext, -2, "b");
                    duk_push_number(pContext, 1);
                    duk_put_prop_string(pContext, -2, "a");
                }
                duk_put_prop_string(pContext, -2, "White");
                duk_push_object(pContext);
                {
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "r");
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "g");
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "b");
                    duk_push_number(pContext, 1);
                    duk_put_prop_string(pContext, -2, "a");
                }
                duk_put_prop_string(pContext, -2, "Black");
                duk_push_object(pContext);
                {
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "r");
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "g");
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "b");
                    duk_push_number(pContext, 0);
                    duk_put_prop_string(pContext, -2, "a");
                }
                duk_put_prop_string(pContext, -2, "Transparent");
            }
            duk_put_global_string(pContext, "Color");

            // oRenderer
            duk_push_object(pContext);
            {
                duk_push_c_function(pContext, js_Renderer_clear, 1);
                duk_put_prop_string(pContext, 0, "clear");
            }
            duk_put_global_string(pContext, "Renderer");

            // oSpriteBatch
            duk_push_object(pContext);
            {
                duk_push_c_function(pContext, js_SpriteBatch_begin, 0);
                duk_put_prop_string(pContext, 0, "begin");

                duk_push_c_function(pContext, js_SpriteBatch_end, 0);
                duk_put_prop_string(pContext, 0, "end");

                duk_push_c_function(pContext, js_SpriteBatch_drawRect, 2);
                duk_put_prop_string(pContext, 0, "drawRect");

                duk_push_c_function(pContext, js_SpriteBatch_drawTexturedRect, 3);
                duk_put_prop_string(pContext, 0, "drawTexturedRect");
            }
            duk_put_global_string(pContext, "SpriteBatch");

            // OTexture
            duk_push_c_function(pContext, js_Texture, 1);
            duk_put_global_string(pContext, "Texture");
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

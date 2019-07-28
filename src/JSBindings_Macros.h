// Onut
#include <onut/Anim.h>
#include <onut/Component.h>
#include <onut/ComponentFactory.h>
#include <onut/ContentManager.h>
#include <onut/Crypto.h>
#include <onut/Curve.h>
#include <onut/Dispatcher.h>
#include <onut/Entity.h>
#include <onut/EntityFactory.h>
#include <onut/Files.h>
#include <onut/Font.h>
#include <onut/GamePad.h>
#include <onut/Http.h>
#include <onut/Images.h>
#include <onut/IndexBuffer.h>
#include <onut/Input.h>
#include <onut/Log.h>
#include <onut/Model.h>
#include <onut/Music.h>
#include <onut/onut.h>
#include <onut/ParticleSystem.h>
#include <onut/ParticleSystemManager.h>
#include <onut/PrimitiveBatch.h>
#include <onut/PrimitiveMode.h>
#include <onut/Random.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/Sound.h>
#include <onut/SpriteAnim.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>
#include <onut/Timing.h>
#include <onut/UICheckBox.h>
#include <onut/UIButton.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIPanel.h>
#include <onut/UIImage.h>
#include <onut/UILabel.h>
#include <onut/UITextBox.h>
#include <onut/VertexBuffer.h>
#include <onut/VideoPlayer.h>
#include <onut/Window.h>

// Third party
#include <duktape/duktape.h>

namespace onut
{
    namespace js
    {
        extern duk_context* pContext;

        template<typename Ttype>
        std::shared_ptr<Ttype> getRefType(duk_context *ctx, duk_idx_t index)
        {
            if (index >= duk_get_top(ctx)) return nullptr;
            if (duk_is_null_or_undefined(ctx, index)) return nullptr;

            duk_get_prop_string(ctx, index, "\xff""\xff""data");
            auto pp = (std::shared_ptr<Ttype>*)(duk_to_pointer(ctx, -1));
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

        template<typename Tresource>
        std::shared_ptr<Tresource> getResource(duk_context *ctx, duk_idx_t index)
        {
            if (index >= duk_get_top(ctx)) return nullptr;
            if (duk_is_null_or_undefined(ctx, index)) return nullptr;

            if (duk_is_string(ctx, index))
            {
                return oContentManager->getResourceAs<Tresource>(duk_to_string(ctx, index));
            }
            else
            {
                duk_get_prop_string(ctx, index, "\xff""\xff""data");
                auto pp = (std::shared_ptr<Tresource>*)(duk_to_pointer(ctx, -1));
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
        }

        class Function
        {
        public:
            Function(duk_context* ctx, int index) : m_ctx(ctx)
            {
                duk_push_global_stash(m_ctx);
                duk_dup(m_ctx, index < 0 ? index - 1 : index);
                m_name = "fn" + std::to_string(reinterpret_cast<uintptr_t>(this));
                duk_put_prop_string(m_ctx, -2, m_name.c_str());
            }

            bool push(duk_context* ctx)
            {
                duk_push_global_stash(ctx);
                if (duk_get_prop_string(ctx, -1, m_name.c_str()))
                {
                    return true;
                }
                duk_pop(ctx);
                return false;
            }

            void call(duk_context* ctx, duk_idx_t argCount)
            {
                duk_call(ctx, argCount);
                duk_pop(ctx); // Pop result
                duk_pop(ctx); // Pop global stash
            }

            ~Function()
            {
                if (!m_name.empty() && pContext)
                {
                    duk_push_global_stash(m_ctx);
                    duk_del_prop_string(m_ctx, -1, m_name.c_str());
                    duk_pop(m_ctx);
                }
            }

        private:
            duk_context* m_ctx;
            std::string m_name;
        };
        using FunctionRef = std::shared_ptr<Function>;

        FunctionRef getFunction(duk_context* ctx, int index);
        void newVector2(duk_context* ctx, const Vector2& val);
        void newVector3(duk_context* ctx, const Vector3& val);
        void newVector4(duk_context* ctx, const Vector4& val);
        void newRect(duk_context* ctx, const Rect& val);
        void newColor(duk_context* ctx, const Color& val);
        void newMatrix(duk_context* ctx, const Matrix& val);
        Vector2 getVector2(duk_context *ctx, duk_idx_t index, const Vector2& in_default = Vector2::Zero);
        Point getPoint(duk_context *ctx, duk_idx_t index, const Point& in_default = Point(0, 0));
        Vector3 getVector3(duk_context *ctx, duk_idx_t index, const Vector3& in_default = Vector3::Zero);
        Vector4 getVector4(duk_context *ctx, duk_idx_t index, const Vector4& in_default = Vector4::Zero);
        Vector4 getRect(duk_context *ctx, duk_idx_t index, const Vector4& in_default = Vector4::Zero);
        iRect getiRect(duk_context *ctx, duk_idx_t index, const iRect& in_default = { 0, 0, 1, 1 });
        Color getColor(duk_context *ctx, duk_idx_t index, const Color& in_default = Color::White);
        Matrix getMatrix(duk_context *ctx, duk_idx_t index, const Matrix& in_default = Matrix::Identity);
        bool getBool(duk_context *ctx, duk_idx_t index, bool in_default = false);
        float getFloat(duk_context *ctx, duk_idx_t index, float in_default = 0.0f);
        unsigned int getUInt(duk_context *ctx, duk_idx_t index, unsigned int in_default = 0);
        int getInt(duk_context *ctx, duk_idx_t index, int in_default = 0);
        const char* getString(duk_context *ctx, duk_idx_t index, const char* in_default = "");
        OUIControlRef getUI(duk_context *ctx, duk_idx_t index, const OUIControlRef& in_default = nullptr);
        OSpriteAnimInstanceRef getSpriteAnimInstance(duk_context *ctx, duk_idx_t index);
        OEmitterInstance* getParticleEmitter(duk_context *ctx, duk_idx_t index);

#define JS_GLOBAL_FUNCTION_BEGIN duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
#define JS_GLOBAL_FUNCTION_END(__name__, __argcnt__) , __argcnt__); duk_put_global_string(ctx, __name__)

#define JS_INTERFACE_BEGIN() duk_push_object(ctx)
#define JS_INTERFACE_END(__name__) duk_put_global_string(ctx, __name__)
#define JS_INTERFACE_FUNCTION_BEGIN duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
#define JS_INTERFACE_FUNCTION_END(__name__, __argcnt__) , __argcnt__); duk_put_prop_string(ctx, 0, __name__)

#define JS_BOOL(...) getBool(ctx, __VA_ARGS__)
#define JS_FLOAT(...) (float)getFloat(ctx, __VA_ARGS__)
#define JS_STRING(...) getString(ctx, __VA_ARGS__)
#define JS_UINT(...) getUInt(ctx, __VA_ARGS__)
#define JS_INT(...) getInt(ctx, __VA_ARGS__)

#define JS_RECT(...) getRect(ctx, __VA_ARGS__)
#define JS_iRECT(...) getiRect(ctx, __VA_ARGS__)
#define JS_VECTOR2(...) getVector2(ctx, __VA_ARGS__)
#define JS_VECTOR3(...) getVector3(ctx, __VA_ARGS__)
#define JS_VECTOR4(...) getVector4(ctx, __VA_ARGS__)
#define JS_COLOR(...) getColor(ctx, __VA_ARGS__)
#define JS_MATRIX(...) getMatrix(ctx, __VA_ARGS__)

#define JS_TEXTURE(__index__) getResource<OTexture>(ctx, __index__)
#define JS_FONT(__index__) getResource<OFont>(ctx, __index__)
#define JS_SHADER(__index__) getResource<OShader>(ctx, __index__)
#define JS_SOUND(__index__) getResource<OSound>(ctx, __index__)
#define JS_MODEL(__index__) getResource<OModel>(ctx, __index__)
#define JS_SPRITE_ANIM(__index__) getResource<OSpriteAnim>(ctx, __index__)
#define JS_SPRITE_ANIM_INSTANCE(__index__) getSpriteAnimInstance(ctx, __index__)
#define JS_PARTICLE_SYSTEM(__index__) getResource<OParticleSystem>(ctx, __index__)
#define JS_PARTICLE_EMITTER(__index__) getParticleEmitter(ctx, __index__)
#define JS_VERTEX_BUFFER(__index__) getRefType<OVertexBuffer>(ctx, __index__)
#define JS_INDEX_BUFFER(__index__) getRefType<OIndexBuffer>(ctx, __index__)
#define JS_UPDATER(__index__) getRefType<OUpdater>(ctx, __index__)

#define JS_ENTITY(__index__) getRefType<OEntity>(ctx, __index__)
#define JS_COMPONENT(__index__) getRefType<OComponent>(ctx, __index__)

#define JS_UI(...) getUI(ctx, __VA_ARGS__)
    }
}

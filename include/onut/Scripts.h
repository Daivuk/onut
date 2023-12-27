#ifndef SCRIPTS_H_INCLUDED
#define SCRIPTS_H_INCLUDED

#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>
#include <onut/Rect.h>
#include <onut/iRect.h>
#include <onut/Color.h>
#include <onut/Matrix.h>
#include <onut/Point.h>

#include <duktape/duktape.h>

#include <functional>


namespace onut
{
    namespace js
    {
        // Set this in initSettings() to let you setup your game specific javascript bindings
        extern std::function<void(duk_context*)> createUserBindings;

        extern void* pVector2Prototype;
        extern void* pVector3Prototype;
        extern void* pVector4Prototype;
        extern void* pRectPrototype;
        extern void* pColorPrototype;

        void newVector2(duk_context* ctx, const Vector2& val);
        void newVector3(duk_context* ctx, const Vector3& val);
        void newVector4(duk_context* ctx, const Vector4& val);
        void newRect(duk_context* ctx, const Rect& val);
        void newIRect(duk_context* ctx, const iRect& val);
        void newColor(duk_context* ctx, const Color& val);
        void newMatrix(duk_context* ctx, const Matrix& val);

        Vector2 getVector2(duk_context *ctx, duk_idx_t index, const Vector2& in_default);
        Point getPoint(duk_context *ctx, duk_idx_t index, const Point& in_default);
        Vector3 getVector3(duk_context *ctx, duk_idx_t index, const Vector3& in_default);
        Vector4 getVector4(duk_context *ctx, duk_idx_t index, const Vector4& in_default);
        Vector4 getRect(duk_context *ctx, duk_idx_t index, const Vector4& in_default);
        iRect getiRect(duk_context *ctx, duk_idx_t index, const iRect& in_default);
        Color getColor(duk_context *ctx, duk_idx_t index, const Color& in_default);
        Matrix getMatrix(duk_context *ctx, duk_idx_t index, const Matrix& in_default);
    }
}


#endif

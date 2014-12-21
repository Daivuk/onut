#pragma once
#include <functional>

namespace onut {
    class ICallback {
    public:
        virtual void call() = 0;
    };

    template<typename Tfn, typename ... Targs>
    class Callback : public ICallback {
    public:
        Callback(Tfn callback, Targs... args) :
            m_bind(callback, args...)  {
        }

        void call() override {
            m_bind();
        }

    private:
        decltype(std::bind(std::declval<Tfn>(), std::declval<Targs>()...))  m_bind;
    };
}

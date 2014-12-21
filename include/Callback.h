#pragma once
#include <functional>

namespace onut {
    class ICallback {
    public:
        virtual void call() = 0;
    };

    template<typename ... Targs>
    class Callback : public ICallback {
    public:
        Callback(Targs... args) :
            m_bind(args...)  {
        }

        void call() override {
            m_bind();
        }

    private:
        decltype(std::bind(std::declval<Targs>()...))  m_bind;
    };
}

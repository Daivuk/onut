#ifndef JSBINDINGS_H_INCLUDED
#define JSBINDINGS_H_INCLUDED


namespace onut
{
    namespace js
    {
        void init();
        void shutdown();
        void update(float dt);
        void render();
        void renderUI();
    }
}

#endif

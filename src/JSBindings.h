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
        void onWriteChar(char c);
        void onWriteFunc(int f);
        void onWriteString(const std::string &s);
    }
}

#endif

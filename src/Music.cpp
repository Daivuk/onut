// Onut
#include <onut/ContentManager.h>
#include <onut/Music.h>

namespace onut
{
    Music::Music()
    {
    }

    Music::~Music()
    {
    }
}

OMusicRef OGetMusic(const std::string& name)
{
    return oContentManager->getResourceAs<OMusic>(name);
}

void OPlayMusic(const std::string& name, bool loop)
{
    auto pMusic = OGetMusic(name);
    if (pMusic) pMusic->play(loop);
}

void OStopMusic(const std::string& name)
{
    auto pMusic = OGetMusic(name);
    if (pMusic) pMusic->stop();
}

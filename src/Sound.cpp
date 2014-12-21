#include "Sound.h"
#include "Audio.h"
#include "StringUtils.h"
using namespace DirectX;

extern AudioEngine* g_pAudioEngine;

namespace onut {
    Sound* Sound::createFromFile(const std::string& filename) {
        auto pRet = new Sound();
        pRet->m_pSound = new SoundEffect(g_pAudioEngine, utf8ToUtf16(filename).c_str());
        return pRet;
    }

    Sound::~Sound() {
        delete m_pSound;
        m_pSound = nullptr;
    }

    void Sound::play() {
        m_pSound->Play();
    }

    void Sound::stop() {
    }
}

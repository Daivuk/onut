// Onut
#include <onut/AudioEngine.h>
#include <onut/ContentManager.h>
#include <onut/Sound.h>
#include <onut/Strings.h>
#include <onut/Random.h>

// Third partyes
#include <tinyxml2/tinyxml2.h>

// STL
#include <cassert>

namespace onut
{
    OSoundRef Sound::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        enum class WavChunks
        {
            RiffHeader = 0x46464952,
            WavRiff = 0x54651475,
            Format = 0x020746d66,
            LabeledText = 0x478747C6,
            Instrumentation = 0x478747C6,
            Sample = 0x6C706D73,
            Fact = 0x47361666,
            Data = 0x61746164,
            Junk = 0x4b4e554a,
        };

        enum class WavFormat
        {
            PulseCodeModulation = 0x01,
            IEEEFloatingPoint = 0x03,
            ALaw = 0x06,
            MuLaw = 0x07,
            IMAADPCM = 0x11,
            YamahaITUG723ADPCM = 0x16,
            GSM610 = 0x31,
            ITUG721ADPCM = 0x40,
            MPEG = 0x50,
            Extensible = 0xFFFE
        };

        FILE* pFic;
#if defined(WIN32)
        fopen_s(&pFic, filename.c_str(), "rb");
#else
        pFic = fopen(filename.c_str(), "rb");
#endif
        assert(pFic);

        int32_t chunkid = 0;
        int32_t formatsize;
        WavFormat format;
        int16_t channels;
        int channelcount;
        int32_t samplerate;
        int32_t bitspersecond;
        int16_t formatblockalign;
        int16_t bitdepth;

        int32_t headerid;
        int32_t memsize;
        int32_t riffstyle;
        
        int32_t datasize;

        int sampleCount;
        float* pBuffer = nullptr;

        bool datachunk = false;
        while (!datachunk)
        {
            fread(&chunkid, 4, 1, pFic);
            switch ((WavChunks)chunkid)
            {
                case WavChunks::Format:
                {
                    fread(&formatsize, 4, 1, pFic);
                    int16_t format16;
                    fread(&format16, 2, 1, pFic);
                    format = (WavFormat)format16;
                    fread(&channels, 2, 1, pFic);
                    channelcount = (int)channels;
                    fread(&samplerate, 4, 1, pFic);
                    fread(&bitspersecond, 4, 1, pFic);
                    fread(&formatblockalign, 2, 1, pFic);
                    fread(&bitdepth, 2, 1, pFic);
                    if (formatsize == 18)
                    {
                        int16_t extradata;
                        fread(&extradata, 2, 1, pFic);
                        fseek(pFic, (long)extradata, SEEK_CUR);
                    }
                    break;
                }
                case WavChunks::RiffHeader:
                {
                    headerid = chunkid;
                    fread(&memsize, 4, 1, pFic);
                    fread(&riffstyle, 4, 1, pFic);
                    break;
                }
                case WavChunks::Data:
                {
                    datachunk = true;
                    fread(&datasize, 4, 1, pFic);
                    uint8_t* pData = new uint8_t[datasize];
                    fread(pData, bitdepth / 8, datasize / (bitdepth / 8), pFic);

                    sampleCount = (int)datasize / ((int)bitdepth / 8) / channelcount;

                    int len = sampleCount * channelcount;
                    pBuffer = new float[len];

                    switch (bitdepth)
                    {
                        case 8:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                uint8_t sampleU8 = *(uint8_t*)(pData + i);
                                pBuffer[i] = ((float)sampleU8 - 128.0f) / (float)std::numeric_limits<int8_t>::max();
                            }
                            break;
                        }
                        case 16:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                int16_t sample16 = *(int16_t*)(pData + (i * 2));
                                pBuffer[i] = (float)sample16 / (float)std::numeric_limits<int16_t>::max();
                            }
                            break;
                        }
                        case 24:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                int8_t b0 = pData[i * 3 + 0];
                                int8_t b1 = pData[i * 3 + 1];
                                int8_t b2 = pData[i * 3 + 2];
                                int32_t sample24 = b0 | (b1 << 8) | (b2 << 16);
                                pBuffer[i] = (float)sample24 / 8388608.0f;
                            }
                            break;
                        }
                        case 32:
                        {
                            memcpy(pBuffer, pData, sizeof(float) * len);
                            break;
                        }
                        default:
                            assert(false);
                    }

                    delete[] pData;
                    break;
                }
                default:
                {
                    int32_t skipsize;
                    fread(&skipsize, 4, 1, pFic);
                    fseek(pFic, (long)skipsize, SEEK_CUR);
                    break;
                }
            }
        }

        fclose(pFic);

        if (!pBuffer) return nullptr;
        auto pRet = createFromData(pBuffer, sampleCount, channelcount, samplerate, pContentManager);
        delete[] pBuffer;
        return pRet;
    }

    OSoundRef Sound::createFromData(const float* pSamples, int sampleCount, int channelCount, int samplerate, const OContentManagerRef& pContentManager)
    {
        auto engineFreq = oAudioEngine->getSampleRate();
        auto engineChannels = oAudioEngine->getChannels();
        auto pRet = std::make_shared<OSound>();

        pRet->m_bufferSampleCount = sampleCount;
        pRet->m_pBuffer = new float[sampleCount * engineChannels];

        switch (engineChannels)
        {
            case 1:
            {
                switch (channelCount)
                {
                    case 1:
                    {
                        memcpy(pRet->m_pBuffer, pSamples, sizeof(float) * sampleCount);
                        break;
                    }
                    case 2:
                    {
                        for (auto i = 0; i < sampleCount; ++i)
                        {
                            pRet->m_pBuffer[i] = (pSamples[i * 2 + 0] + pSamples[i * 2 + 1]) * 0.5f;
                        }
                        break;
                    }
                    default:
                        assert(false);
                        break;
                }
                break;
            }
            case 2:
            {
                switch (channelCount)
                {
                    case 1:
                    {
                        for (auto i = 0; i < sampleCount; ++i)
                        {
                            pRet->m_pBuffer[i * 2 + 0] = pSamples[i];
                            pRet->m_pBuffer[i * 2 + 1] = pSamples[i];
                        }
                        break;
                    }
                    case 2:
                    {
                        memcpy(pRet->m_pBuffer, pSamples, sizeof(float) * sampleCount * 2);
                        break;
                    }
                    default:
                        assert(false);
                        break;
                }
                break;
            }
            default:
                assert(false);
                break;
        }

        // Resample if the wave is not the same sample rate as the audio engine
        if (samplerate != oAudioEngine->getSampleRate())
        {
            // TODO...
        }

        return pRet;
    }

    Sound::~Sound()
    {
        if (m_pBuffer) delete[] m_pBuffer;
    }

    void Sound::play(float volume, float balance, float pitch)
    {
        if (m_pBuffer)
        {
            auto pInstance = createInstance();
            pInstance->setVolume(volume);
            pInstance->setBalance(balance);
            pInstance->setPitch(pitch);
            m_instances.push_back(pInstance);
            pInstance->play();
        }
    }

    void Sound::stop()
    {
        for (auto& pInst : m_instances)
        {
            pInst->stop();
        }
        m_instances.clear();
    }

    OSoundInstanceRef Sound::createInstance()
    {
        auto pInstance = std::make_shared<SoundInstance>();
        pInstance->m_pSound = OThis;
        return pInstance;
    }
    
    SoundInstance::SoundInstance()
    {
        m_loop = false;
        m_volume = 1.f;
        m_balance = 0.f;
        m_pitch = 1.f;
        m_offset = 0;
    }

    SoundInstance::~SoundInstance()
    {
    }

    void SoundInstance::setLoop(bool loop)
    {
        m_loop = loop;
    }

    void SoundInstance::setVolume(float volume)
    {
        m_volume = volume;
    }

    void SoundInstance::setBalance(float balance)
    {
        if (balance < -1) balance = -1;
        if (balance > 1) balance = 1;
        m_balance = balance;
    }

    void SoundInstance::setPitch(float pitch)
    {
        if (pitch < (1.f / 1024.0f)) pitch = (1.f / 1024.0f);
        if (pitch > 2.f) pitch = 2.f;
        m_pitch = pitch;
    }

    void SoundInstance::play()
    {
        if (isPlaying()) return;
        m_isPaused = false;
        oAudioEngine->addInstance(OThis);
    }

    void SoundInstance::pause()
    {
        oAudioEngine->removeInstance(OThis);
        m_isPaused = true;
    }

    void SoundInstance::stop()
    {
        if (!m_isPaused)
        {
            oAudioEngine->removeInstance(OThis);
            m_isPaused = true;
            m_offset = 0;
        }
    }

    bool SoundInstance::isPlaying() const
    {
        return !m_isPaused;
    }

    bool SoundInstance::isPaused() const
    {
        return m_isPaused;
    }

    bool SoundInstance::getLoop() const
    {
        return m_loop;
    }

    float SoundInstance::getVolume() const
    {
        return m_volume;
    }

    float SoundInstance::getBalance() const
    {
        return m_balance;
    }

    float SoundInstance::getPitch() const
    {
        return m_pitch;
    }

    bool SoundInstance::progress(int frameCount, int sampleRate, int channelCount, float* pOut)
    {
        auto pSoundPtr = m_pSound.get();
        int offset = m_offset;
        auto pSoundBuffer = pSoundPtr->m_pBuffer + offset * channelCount;
        float volume = m_volume;
        int bufferSampleCount = pSoundPtr->m_bufferSampleCount;
        bool loop = m_loop;
        float balance = m_balance;
        float leftVolume = std::min(1.0f, -balance + 1.0f) * volume;
        float rightVolume = std::min(1.0f, balance + 1.0f) * volume;
        float pitch = m_pitch;

        int fi;
        int len = std::min(frameCount, (int)((float)(bufferSampleCount - offset) / pitch));
        switch (channelCount)
        {
            case 1:
            {
                for (int i = 0; i < len; ++i)
                {
                    fi = (int)((float)i * pitch);
                    pOut[i] += pSoundBuffer[fi] * volume;
                }
                break;
            }
            case 2:
            {
                for (int i = 0; i < len * 2; i += 2)
                {
                    fi = (int)((float)i * pitch);
                    pOut[i + 0] += pSoundBuffer[fi + 0] * leftVolume;
                    pOut[i + 1] += pSoundBuffer[fi + 1] * rightVolume;
                }
                break;
            }
            default:
                assert(false);
        }

        offset += (int)((float)len * pitch);
        m_offset = offset;
        if (offset >= bufferSampleCount)
        {
            if (loop)
            {
                m_offset = 0;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    OSoundCueRef SoundCue::createFromFile(const std::string& filename, const OContentManagerRef& in_pContentManager)
    {
        OContentManagerRef pContentManager = in_pContentManager;
        if (!pContentManager) pContentManager = oContentManager;
        auto pSoundCue = std::make_shared<OSoundCue>();

        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename.c_str());
        assert(!doc.Error());
        auto pXmlCue = doc.FirstChildElement("cue");
        assert(pXmlCue);

        for (auto pXmlPlay = pXmlCue->FirstChildElement("play"); pXmlPlay; pXmlPlay = pXmlPlay->NextSiblingElement("play"))
        {
            Play play;
            for (auto pXmlSound = pXmlPlay->FirstChildElement("sound"); pXmlSound; pXmlSound = pXmlSound->NextSiblingElement("sound"))
            {
                Sound sound;
                sound.pSound = oContentManager->getResourceAs<OSound>(pXmlSound->Attribute("name"));
                pXmlSound->QueryFloatAttribute("balance", &sound.balance);
                pXmlSound->QueryFloatAttribute("volume", &sound.volume);
                pXmlSound->QueryFloatAttribute("pitch", &sound.pitch);
                float chance = 1;
                pXmlSound->QueryFloatAttribute("chance", &chance);
                play.totalChances += chance;
                sound.chance = play.totalChances;
                pXmlSound->QueryFloatAttribute("delay", &sound.delay);
                play.sounds.push_back(sound);

            }
            pSoundCue->m_plays.push_back(play);
        }

        return pSoundCue;
    }

    void SoundCue::play(float volume, float balance, float pitch)
    {
        for (auto& play : m_plays)
        {
            auto rnd = onut::randf(play.totalChances);
            for (auto& sound : play.sounds)
            {
                if (rnd <= sound.chance)
                {
                    if (sound.pSound)
                    {
                        if (sound.delay != 0)
                        {
                            play.playTimer.start(sound.delay, [=]
                            {
                                sound.pSound->play(sound.volume * volume,
                                                   sound.balance + balance,
                                                   sound.pitch * pitch);
                            });
                        }
                        else
                        {
                            sound.pSound->play(sound.volume * volume,
                                                sound.balance += balance,
                                                sound.pitch *= pitch);
                        }
                    }
                    break;
                }   
            }
        }
    }
}

OSoundRef OGetSound(const std::string& name)
{
    return oContentManager->getResourceAs<OSound>(name);
}

void OPlaySound(const std::string& name, float volume, float balance, float pitch)
{
    auto pSound = OGetSound(name);
    if (pSound)
    {
        pSound->play(volume, balance, pitch);
    }
}

void OPlayRandomSound(const std::vector<std::string>& sounds, float volume, float balance, float pitch)
{
    OPlaySound(ORandVector(sounds), volume, balance, pitch);
}

OSoundInstanceRef OCreateSoundInstance(const std::string& name)
{
    auto pSound = OGetSound(name);
    if (pSound)
    {
        return pSound->createInstance();
    }
    return nullptr;
}

OSoundCueRef OGetSoundCue(const std::string& name)
{
    return oContentManager->getResourceAs<OSoundCue>(name);
}

void OPlaySoundCue(const std::string& name, float volume, float balance, float pitch)
{
    auto pSoundCue = OGetSoundCue(name);
    if (pSoundCue)
    {
        pSoundCue->play(volume, balance, pitch);
    }
}

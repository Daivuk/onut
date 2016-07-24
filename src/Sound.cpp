// Onut
#include <onut/ContentManager.h>
#include <onut/Sound.h>
#include <onut/Strings.h>
#include <onut/Random.h>

// Private
#include "AudioEngine.h"

// Third partyes
#include <tinyxml2/tinyxml2.h>

namespace onut
{
    OSoundRef Sound::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::make_shared<OSound>();

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
        fopen_s(&pFic, filename.c_str(), "rb");
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

                    pRet->m_bufferSampleCount = (int)datasize / ((int)bitdepth / 8) / channelcount;
                    pRet->m_channelCount = channelcount;

                    int len = pRet->m_bufferSampleCount * channelcount;
                    pRet->m_pBuffer = new float[len];

                    switch (bitdepth)
                    {
                        case 8:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                int8_t sample16 = *(int8_t*)(pData + (i * 2));
                                pRet->m_pBuffer[i] = (float)sample16 / (float)std::numeric_limits<int8_t>::max();
                            }
                            break;
                        }
                        case 16:
                        {
                            for (int i = 0; i < len; ++i)
                            {
                                int16_t sample16 = *(int16_t*)(pData + (i * 2));
                                pRet->m_pBuffer[i] = (float)sample16 / (float)std::numeric_limits<int16_t>::max();
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
                                pRet->m_pBuffer[i] = (float)sample24 / 8388608.0f;
                            }
                            break;
                        }
                        case 32:
                        {
                            memcpy(pRet->m_pBuffer, pData, sizeof(float) * len);
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

        // Resample if the wave is not the same sample rate as the audio engine
        if (samplerate != oAudioEngine->getSampleRate())
        {
            // TODO...
        }

        fclose(pFic);

        return pRet;
    }

    OSoundRef Sound::createFromData(float* pSamples, int sampleCount, int channelCount, int samplerate, const OContentManagerRef& pContentManager)
    {
        auto pRet = std::make_shared<OSound>();

        pRet->m_bufferSampleCount = sampleCount;
        pRet->m_channelCount = channelCount;

        int len = pRet->m_bufferSampleCount * channelCount;
        pRet->m_pBuffer = new float[len];
        memcpy(pRet->m_pBuffer, pSamples, sizeof(float) * len);

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

    SoundInstance::~SoundInstance()
    {
        stop();
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

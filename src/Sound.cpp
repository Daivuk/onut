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
#include <cmath>

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

        int frameCount;
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

                    frameCount = (int)datasize / ((int)bitdepth / 8) / channelcount;

                    int len = frameCount * channelcount;
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
                                uint8_t b0 = pData[i * 3 + 0];
                                uint8_t b1 = pData[i * 3 + 1];
                                uint8_t b2 = pData[i * 3 + 2];
                                int32_t sample24;
                                if (b2 & 0x80)
                                {
                                    sample24 = (0xff << 24) | ((int32_t)b2 << 16) | ((int32_t)b1 << 8) | ((int32_t)b0 << 0);
                                }
                                else
                                {
                                    sample24 = ((int32_t)b2 << 16) | ((int32_t)b1 << 8) | ((int32_t)b0 << 0);
                                }
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
        auto pRet = createFromData(pBuffer, frameCount, channelcount, samplerate, pContentManager);
        delete[] pBuffer;
        return pRet;
    }

    // Some magic found here: http://paulbourke.net/miscellaneous/interpolation/
    //
    // Tension: 1 is high, 0 normal, -1 is low
    // Bias: 0 is even,
    // positive is towards first segment,
    // negative towards the other
    static float resamplingHermite(
        float y0, float y1,
        float y2, float y3,
        float mu,
        float tension = 0.0f,
        float bias = 0.0f)
    {
        float m0, m1, mu2, mu3;
        float a0, a1, a2, a3;

        mu2 = mu * mu;
        mu3 = mu2 * mu;
        m0 = (y1 - y0)*(1 + bias)*(1 - tension) / 2;
        m0 += (y2 - y1)*(1 - bias)*(1 - tension) / 2;
        m1 = (y2 - y1)*(1 + bias)*(1 - tension) / 2;
        m1 += (y3 - y2)*(1 - bias)*(1 - tension) / 2;
        a0 = 2 * mu3 - 3 * mu2 + 1;
        a1 = mu3 - 2 * mu2 + mu;
        a2 = mu3 - mu2;
        a3 = -2 * mu3 + 3 * mu2;

        return (a0*y1 + a1*m0 + a2*m1 + a3*y2);
    }

    OSoundRef Sound::createFromData(const float* pSamples, int frameCount, int channelCount, int samplerate, const OContentManagerRef& pContentManager)
    {
        auto engineFreq = oAudioEngine->getSampleRate();
        auto engineChannels = oAudioEngine->getChannels();
        auto pRet = std::make_shared<OSound>();

        pRet->m_frameCount = frameCount;
        pRet->m_channelCount = channelCount;

        auto len = frameCount * channelCount;

        // Resample if the wave is not the same sample rate as the audio engine
        if (samplerate != oAudioEngine->getSampleRate())
        {
            float t = 0.0f;
            float progress = (float)samplerate / (float)engineFreq;
            int index = 0;
            int oldIndex = 0;
            int newFrameCount = (int)ceilf((float)frameCount / progress);

            pRet->m_pBuffer = new float[newFrameCount * pRet->m_channelCount];
            auto pOut = pRet->m_pBuffer;

            float* pSamplings = new float[4 * channelCount];
            memset(pSamplings, 0, sizeof(float) * 4 * channelCount);

            while (index < newFrameCount)
            {
                for (int i = 0; i < channelCount; ++i)
                {
                    pOut[index * channelCount + i] = resamplingHermite(
                        pSamplings[0 * channelCount + i],
                        pSamplings[1 * channelCount + i],
                        pSamplings[2 * channelCount + i],
                        pSamplings[3 * channelCount + i], t);
                }

                // Progress to the next sample
                t += progress;
                while (t >= 1.0f)
                {
                    for (int i = 0; i < channelCount; ++i)
                    {
                        pSamplings[0 * channelCount + i] = pSamplings[1 * channelCount + i];
                        pSamplings[1 * channelCount + i] = pSamplings[2 * channelCount + i];
                        pSamplings[2 * channelCount + i] = pSamplings[3 * channelCount + i];
                        pSamplings[3 * channelCount + i] = pSamples[oldIndex * channelCount + i];
                    }
                    ++oldIndex;
                    if (oldIndex >= frameCount) oldIndex = frameCount - 1;
                    t -= 1.0f;
                }
                ++index;
            }

            delete[] pSamplings;

            pRet->m_frameCount = newFrameCount;
        }
        else
        {
            pRet->m_pBuffer = new float[len];
            memcpy(pRet->m_pBuffer, pSamples, sizeof(float) * len);
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
        auto pSoundBuffer = pSoundPtr->m_pBuffer;
        int bufferFrameCount = pSoundPtr->m_frameCount;
        int bufferChannelCount = pSoundPtr->m_channelCount;

        int offset = m_offset;
        float volume = m_volume;
        bool loop = m_loop;
        float balance = m_balance;
        float pitch = m_pitch;

        // Calculate panning
        float leftVolume = std::min(1.0f, -balance + 1.0f) * volume;
        float rightVolume = std::min(1.0f, balance + 1.0f) * volume;

        if (channelCount == 1)
        {
            float avg;
            float invChannelCount = 1.0f / (float)bufferChannelCount;
            float sample1;
            float sample2;
            float percent;
            float offsetF = (float)offset;
            for (int i = 0; i < frameCount; ++i)
            {
                avg = 0.0f;
                for (int channel = 0; channel < bufferChannelCount; ++channel)
                {
                    sample1 = pSoundBuffer[(int)offsetF * bufferChannelCount + channel];
                    if (loop)
                    {
                        sample2 = pSoundBuffer[(((int)offsetF + 1) % bufferFrameCount) * bufferChannelCount + channel];
                    }
                    else
                    {
                        if ((int)offsetF + 1 >= bufferFrameCount)
                            sample2 = 0.0f;
                        else
                            sample2 = pSoundBuffer[((int)offsetF + 1) * bufferChannelCount + channel];
                    }
                    percent = offsetF - (float)(int)offsetF;
                    avg += sample1 + (sample2 - sample1) * percent;
                }
                pOut[i] += avg * invChannelCount;
                offsetF += pitch;
                if ((int)offsetF >= bufferFrameCount)
                {
                    if (loop) offsetF -= (float)bufferFrameCount;
                    else
                    {
                        m_offset = bufferFrameCount;
                        return false; // End of buffer, stop playing
                    }
                }
            }
            m_offset = (int)offsetF;
            return true;
        }
        else // 2 or more
        {
            if (bufferChannelCount == 1)
            {
                // Split equally in stereo channels
                float sample;
                float sample1;
                float sample2;
                float offsetF = (float)offset;
                float percent;
                for (int i = 0; i < frameCount; ++i)
                {
                    sample1 = pSoundBuffer[(int)offsetF];
                    if (loop)
                    {
                        sample2 = pSoundBuffer[((int)offsetF + 1) % bufferFrameCount];
                    }
                    else
                    {
                        if ((int)offsetF + 1 >= bufferFrameCount)
                            sample2 = 0.0f;
                        else
                            sample2 = pSoundBuffer[(int)offsetF + 1];
                    }
                    percent = offsetF - (float)(int)offsetF;
                    sample = sample1 + (sample2 - sample1) * percent;
                    pOut[i * channelCount + 0] += sample * leftVolume;
                    pOut[i * channelCount + 1] += sample * rightVolume;
                    offsetF += pitch;
                    if ((int)offsetF >= bufferFrameCount)
                    {
                        if (loop) offsetF -= (float)bufferFrameCount;
                        else
                        {
                            m_offset = bufferFrameCount;
                            return false; // End of buffer, stop playing
                        }
                    }
                }
                m_offset = (int)offsetF;
                return true;
            }
            else
            {
                float sample1;
                float sample2;
                float offsetF = (float)offset;
                float percent;
                for (int i = 0; i < frameCount; ++i)
                {
                    sample1 = pSoundBuffer[(int)offsetF * bufferChannelCount + 0];
                    if (loop)
                    {
                        sample2 = pSoundBuffer[(((int)offsetF + 1) % bufferFrameCount) * bufferChannelCount + 0];
                    }
                    else
                    {
                        if ((int)offsetF + 1 >= bufferFrameCount)
                            sample2 = 0.0f;
                        else
                            sample2 = pSoundBuffer[((int)offsetF + 1) * bufferChannelCount + 0];
                    }
                    percent = offsetF - (float)(int)offsetF;
                    pOut[i * channelCount + 0] += sample1 + (sample2 - sample1) * percent;

                    sample1 = pSoundBuffer[(int)offsetF * bufferChannelCount + 1];
                    if (loop)
                    {
                        sample2 = pSoundBuffer[(((int)offsetF + 1) % bufferFrameCount) * bufferChannelCount + 1];
                    }
                    else
                    {
                        if ((int)offsetF + 1 >= bufferFrameCount)
                            sample2 = 0.0f;
                        else
                            sample2 = pSoundBuffer[((int)offsetF + 1) * bufferChannelCount + 1];
                    }
                    percent = offsetF - (float)(int)offsetF;
                    pOut[i * channelCount + 1] += sample1 + (sample2 - sample1) * percent;

                    offsetF += pitch;
                    if ((int)offsetF >= bufferFrameCount)
                    {
                        if (loop) offsetF -= (float)bufferFrameCount;
                        else
                        {
                            m_offset = bufferFrameCount;
                            return false; // End of buffer, stop playing
                        }
                    }
                }
                m_offset = (int)offsetF;
                return true;
            }
        }

        // Unreachable code path
        return false;
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

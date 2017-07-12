// Oak Nut include
#include <onut/Curve.h>
#include <onut/Font.h>
#include <onut/Input.h>
#include <onut/Log.h>
#include <onut/Music.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Sound.h>

// STL
#include <cmath>

OSoundInstanceRef pLoopingSound;
OMusicRef pMusic;
OSoundRef pNotes[8];

static const double NOTE_FREQUENCIES[8] = {
    261.63, // C4
    293.66, // D4
    329.63, // E4
    349.23, // F4
    392.00, // G4
    440.00, // A4
    493.88, // B4
    523.25, // C5
};

void initSettings()
{
    oSettings->setGameName("Sounds Sample");
    oSettings->setIsResizableWindow(true);
}

void init()
{
    pMusic = OGetMusic("music.ogg");

    pLoopingSound = OCreateSoundInstance("looping.wav");
    pLoopingSound->setLoop(true);

    // Create sounds from custom data
    float* pSampleBuffer = new float[50000];
    for (int note = 0; note < 8; ++note)
    {
        for (int i = 0; i < 50000; ++i)
        {
            // Create a sine wave of a standard C4 note
            pSampleBuffer[i] = (float)std::sin((double)i / 44100.0 * NOTE_FREQUENCIES[note] * 3.1415926535897932384626433832795 * 2.0);

            // Fade out
            float fadeOut = (float)((double)(50000 - i) / 50000.0);
            fadeOut = std::pow(fadeOut, 4.0f);
            pSampleBuffer[i] *= fadeOut;
        }
        pNotes[note] = OSound::createFromData(pSampleBuffer, 50000, 1, 44100);
    }
    delete[] pSampleBuffer;
}

void update()
{
    float volume = 1.f;
    float balance = 0.f;
    float pitch = 1.0f;

    if (OInputPressed(OKeyLeft)) balance -= 1.f;
    if (OInputPressed(OKeyRight)) balance += 1.f;
    if (OInputPressed(OKeyUp)) pitch = 2.f;
    if (OInputPressed(OKeyDown)) pitch = 0.5f;

    if (OInputJustPressed(OKeyF1))
    {
        OPlaySound("48000_8.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF2))
    {
        OPlaySound("48000_16.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF3))
    {
        OPlaySound("48000_24.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF4))
    {
        OPlaySound("48000_32.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF5))
    {
        OPlaySound("44100_32.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF6))
    {
        OPlaySound("22050_32.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF7))
    {
        OPlaySound("11025_32.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF8))
    {
        OPlaySound("8000_32.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKeyF9))
    {
        OPlaySound("48000_32_2ch.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKey1))
    {
        OPlaySound("sound1.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKey4))
    {
        OPlayRandomSound({"sound1.wav", "sound2.wav", "sound3.wav", "sound4.wav", "sound5.wav"}, 
                         volume, balance, pitch);
    }
    if (OInputJustPressed(OKey5))
    {
        OPlaySound("sound5.wav", volume, balance, pitch);
    }
    if (OInputJustPressed(OKey6))
    {
        if (pLoopingSound->isPlaying())
        {
            pLoopingSound->pause();
        }
        else
        {
            pLoopingSound->setBalance(balance);
            pLoopingSound->setPitch(pitch);
            pLoopingSound->play();
        }
    }
    if (OInputJustPressed(OKey7))
    {
        OPlaySoundCue("soundCue.cue", volume, balance, pitch);
    }
    if (OInputJustPressed(OKey9))
    {
        if (pMusic->isPlaying())
        {
            OLog("Stop Music");
            pMusic->stop();
        }
        else
        {
            OLog("Start Music");
            pMusic->play();
        }
    }
    if (OInputJustPressed(OKeySpaceBar))
    {
        if (!pMusic->isPaused()) pMusic->pause();
        else pMusic->resume();
    }
    if (OInputJustPressed(OKeyQ)) pNotes[0]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyW)) pNotes[1]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyE)) pNotes[2]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyR)) pNotes[3]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyT)) pNotes[4]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyY)) pNotes[5]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyU)) pNotes[6]->play(volume, balance, pitch);
    if (OInputJustPressed(OKeyI)) pNotes[7]->play(volume, balance, pitch);
}

void render()
{
    // Clear to black
    oRenderer->clear({0, 0, 0, 1});

    // Draw info
    auto pFont = OGetFont("font.fnt");

    oSpriteBatch->begin();

    pFont->draw("Press ^990F1^999 to play 48000 hz, 8 bits", { 400, 10});
    pFont->draw("Press ^990F2^999 to play 48000 hz, 16 bits", { 400, 30});
    pFont->draw("Press ^990F3^999 to play 48000 hz, 24 bits", { 400, 50 });
    pFont->draw("Press ^990F4^999 to play 48000 hz, 32 bits", { 400, 70 });
    pFont->draw("Press ^990F5^999 to play 44100 hz, 32 bits", { 400, 90 });
    pFont->draw("Press ^990F6^999 to play 22050 hz, 32 bits", { 400, 110 });
    pFont->draw("Press ^990F7^999 to play 11025 hz, 32 bits", { 400, 130 });
    pFont->draw("Press ^990F8^999 to play 8000 hz, 32 bits", { 400, 150 });
    pFont->draw("Press ^990F9^999 to play 48000 hz, 32 bits, 2 channels", { 400, 170 });

    pFont->draw("Press ^9901^999 to play sound1", {10, 10});
    pFont->draw("Press ^9904^999 to play a random sound", {10, 70});
    pFont->draw("Press ^9905^999 to play sound5", {10, 90});
    if (pLoopingSound->isPlaying())
    {
        pFont->draw("Press ^9906^999 to start/stop looping sound (^099Playing^999)", {10, 110});
    }
    else
    {
        pFont->draw("Press ^9906^999 to start/stop looping sound", {10, 110});
    }
    pFont->draw("Press ^9907^999 to play cue file", {10, 130});
    pFont->draw("Press ^990qwertyui^999 to do music", {10, 150});
    pFont->draw("Press ^9909^999 to play/stop music (^990SpaceBar^999 to pause/resume)", {10, 170});

    pFont->draw("Hold ^990Left Arrow^999 to on left channel", { 10, OScreenHf - 50 });
    pFont->draw("Hold ^990Right Arrow^999 to on right channel", { 10, OScreenHf - 30 });

    pFont->draw("Hold ^990Up Arrow^999 to play at 200% speed", { 10, OScreenHf - 100 });
    pFont->draw("Hold ^990Down Arrow^999 to play at 50% speed", { 10, OScreenHf - 80 });

    oSpriteBatch->end();
}

void postRender()
{
}

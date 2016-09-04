// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Curve.h>
#include <onut/Font.h>
#include <onut/Input.h>
#include <onut/Log.h>
#include <onut/Music.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Sound.h>

void init();
void update();
void render();

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

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Sounds Sample");
    ORun(init, update, render);
}

void init()
{
    pMusic = OGetMusic("music.mp3");

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
            fadeOut = std::powf(fadeOut, 4);
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

    if (OInputPressed(OKeyLeft)) balance -= 1.f;
    if (OInputPressed(OKeyRight)) balance += 1.f;

    if (OInputJustPressed(OKey1))
    {
        OPlaySound("sound1.wav", volume, balance);
    }
    if (OInputJustPressed(OKey2))
    {
        OPlaySound("sound1.wav", volume, balance, .5f);
    }
    if (OInputJustPressed(OKey3))
    {
        OPlaySound("sound1.wav", volume, balance, 2.f);
    }
    if (OInputJustPressed(OKey4))
    {
        OPlayRandomSound({"sound1.wav", "sound2.wav", "sound3.wav", "sound4.wav", "sound5.wav"}, 
                         volume, balance);
    }
    if (OInputJustPressed(OKey5))
    {
        OPlaySound("sound5.wav", volume, balance);
    }
    if (OInputJustPressed(OKey6))
    {
        if (pLoopingSound->isPlaying())
        {
            pLoopingSound->pause();
        }
        else
        {
            pLoopingSound->play();
        }
    }
    if (OInputJustPressed(OKey7))
    {
        OPlaySoundCue("soundCue.cue", volume, balance);
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
    if (OInputJustPressed(OKeyQ)) pNotes[0]->play(volume, balance);
    if (OInputJustPressed(OKeyW)) pNotes[1]->play(volume, balance);
    if (OInputJustPressed(OKeyE)) pNotes[2]->play(volume, balance);
    if (OInputJustPressed(OKeyR)) pNotes[3]->play(volume, balance);
    if (OInputJustPressed(OKeyT)) pNotes[4]->play(volume, balance);
    if (OInputJustPressed(OKeyY)) pNotes[5]->play(volume, balance);
    if (OInputJustPressed(OKeyU)) pNotes[6]->play(volume, balance);
    if (OInputJustPressed(OKeyI)) pNotes[7]->play(volume, balance);
}

void render()
{
    // Clear to black
    oRenderer->clear({0, 0, 0, 1});

    // Draw info
    auto pFont = OGetFont("font.fnt");

    oSpriteBatch->begin();

    pFont->draw("Press ^9901^999 to play sound1 at 100% speed", {10, 10});
    pFont->draw("Press ^9902^999 to play sound1 at 50% speed", {10, 30});
    pFont->draw("Press ^9903^999 to play sound1 at 200% speed", {10, 50});
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
    pFont->draw("Press ^9909^999 to play/stop music", {10, 170});

    pFont->draw("Hold ^990Left Arrow^999 to on left channel", {10, OScreenHf - 50});
    pFont->draw("Hold ^990Right Arrow^999 to on right channel", {10, OScreenHf - 30});

    oSpriteBatch->end();
}

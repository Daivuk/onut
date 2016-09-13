var font = getFont("font.fnt");
var music = getMusic("music.mp3");
/*
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
*/
function update()
{
    var volume = 1;
    var balance = 0;

/*
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
    }*/
    if (Input.isJustDown(Key._9))
    {
        if (music.isPlaying())
        {
            print("Stop Music");
            music.stop();
        }
        else
        {
            print("Start Music");
            music.play();
        }
    }
 /*   if (OInputJustPressed(OKeyQ)) pNotes[0]->play(volume, balance);
    if (OInputJustPressed(OKeyW)) pNotes[1]->play(volume, balance);
    if (OInputJustPressed(OKeyE)) pNotes[2]->play(volume, balance);
    if (OInputJustPressed(OKeyR)) pNotes[3]->play(volume, balance);
    if (OInputJustPressed(OKeyT)) pNotes[4]->play(volume, balance);
    if (OInputJustPressed(OKeyY)) pNotes[5]->play(volume, balance);
    if (OInputJustPressed(OKeyU)) pNotes[6]->play(volume, balance);
    if (OInputJustPressed(OKeyI)) pNotes[7]->play(volume, balance);
    */
}

function render()
{
    // Clear to black
    Renderer.clear(Color.BLACK);

    SpriteBatch.begin();

    SpriteBatch.drawText(font, "Press ^9901^999 to play sound1 at 100% speed", new Vector2(10, 10));
    SpriteBatch.drawText(font, "Press ^9902^999 to play sound1 at 50% speed", new Vector2(10, 30));
    SpriteBatch.drawText(font, "Press ^9903^999 to play sound1 at 200% speed", new Vector2(10, 50));
    SpriteBatch.drawText(font, "Press ^9904^999 to play a random sound", new Vector2(10, 70));
    SpriteBatch.drawText(font, "Press ^9905^999 to play sound5", new Vector2(10, 90));
/*    if (pLoopingSound->isPlaying())
    {
        SpriteBatch.drawText(font, "Press ^9906^999 to start/stop looping sound (^099Playing^999)", {10, 110});
    }
    else
    {
        SpriteBatch.drawText(font, "Press ^9906^999 to start/stop looping sound", {10, 110});
    }*/
    SpriteBatch.drawText(font, "Press ^9907^999 to play cue file", new Vector2(10, 130));
    SpriteBatch.drawText(font, "Press ^990qwertyui^999 to do music", new Vector2(10, 150));
    SpriteBatch.drawText(font, "Press ^9909^999 to play/stop music", new Vector2(10, 170));

    var screenH = Renderer.getResolution().y;
    SpriteBatch.drawText(font, "Hold ^990Left Arrow^999 to on left channel", new Vector2(10, screenH - 50));
    SpriteBatch.drawText(font, "Hold ^990Right Arrow^999 to on right channel", new Vector2(10, screenH - 30));

    SpriteBatch.end();
}

var font = getFont("font.fnt");
var music = getMusic("music.ogg");
var sound5 = getSound("sound5.wav");
var loopingSound = createSoundInstance("looping.wav");
loopingSound.setLoop(true);

var notes = [];

var NOTE_FREQUENCIES = [
    261.63, // C4
    293.66, // D4
    329.63, // E4
    349.23, // F4
    392.00, // G4
    440.00, // A4
    493.88, // B4
    523.25, // C5
];

// Create sounds from custom data.
// Not recommanded to do in Javascript. This is extremely slow.
var sampleBuffer = new Float32Array(25000);
for (var note = 0; note < 8; ++note)
{
    var noteFreq = NOTE_FREQUENCIES[note];
    var period = 1 / 44100 * Math.PI * 2 * noteFreq;
    for (var i = 0; i < 25000; ++i)
    {
        // Create a sine wave of a standard C4 note
        var val = Math.sin(i * period);

        // Fade out
        var fadeOut = (25000 - i) / 25000;
        fadeOut = Math.pow(fadeOut, 4);

        sampleBuffer[i] = val * fadeOut;
    }
    notes[note] = Sound.createFromData(sampleBuffer, 25000, 1, 44100);
}
sampleBuffer = null; // Free memory

function update() {
    var volume = 1;
    var balance = 0;

    if (Input.isDown(Key.LEFT)) balance -= 1;
    if (Input.isDown(Key.RIGHT)) balance += 1;

    if (Input.isJustDown(Key._1)) {
        playSound("sound1.wav", volume, balance);
    }
    if (Input.isJustDown(Key._2)) {
        playSound("sound1.wav", volume, balance, .5);
    }
    if (Input.isJustDown(Key._3)) {
        playSound("sound1.wav", volume, balance, 2);
    }
    if (Input.isJustDown(Key._4)) {
        playRandomSound(["sound1.wav", "sound2.wav", "sound3.wav", "sound4.wav", "sound5.wav"],
            volume, balance);
    }
    if (Input.isJustDown(Key._5)) {
        // Play using the resource directly
        sound5.play(volume, balance);
    }
    if (Input.isJustDown(Key._6))
    {
        if (loopingSound.isPlaying())
        {
            loopingSound.pause();
        }
        else
        {
            loopingSound.play();
        }
    }
    if (Input.isJustDown(Key._7)) {
        playSoundCue("soundCue.cue", volume, balance);
    }
    if (Input.isJustDown(Key._9)) {
        if (music.isPlaying()) {
            print("Stop Music");
            music.stop();
        }
        else {
            print("Start Music");
            music.play();
        }
    }
    if (Input.isJustDown(Key.SPACE_BAR))
    {
        if (music.isPaused())
        {
            music.resume();
        }
        else
        {
            music.pause();
        }
    }
    if (Input.isJustDown(Key.Q)) notes[0].play(volume, balance);
    if (Input.isJustDown(Key.W)) notes[1].play(volume, balance);
    if (Input.isJustDown(Key.E)) notes[2].play(volume, balance);
    if (Input.isJustDown(Key.R)) notes[3].play(volume, balance);
    if (Input.isJustDown(Key.T)) notes[4].play(volume, balance);
    if (Input.isJustDown(Key.Y)) notes[5].play(volume, balance);
    if (Input.isJustDown(Key.U)) notes[6].play(volume, balance);
    if (Input.isJustDown(Key.I)) notes[7].play(volume, balance);
}

function render() {
    // Clear to black
    Renderer.clear(Color.BLACK);

    SpriteBatch.begin();

    SpriteBatch.drawText(font, "Press ^9901^999 to play sound1 at 100% speed", new Vector2(10, 10));
    SpriteBatch.drawText(font, "Press ^9902^999 to play sound1 at 50% speed", new Vector2(10, 30));
    SpriteBatch.drawText(font, "Press ^9903^999 to play sound1 at 200% speed", new Vector2(10, 50));
    SpriteBatch.drawText(font, "Press ^9904^999 to play a random sound", new Vector2(10, 70));
    SpriteBatch.drawText(font, "Press ^9905^999 to play sound5", new Vector2(10, 90));
    if (loopingSound.isPlaying())
    {
        SpriteBatch.drawText(font, "Press ^9906^999 to start/stop looping sound (^099Playing^999)", new Vector2(10, 110));
    }
    else
    {
        SpriteBatch.drawText(font, "Press ^9906^999 to start/stop looping sound", new Vector2(10, 110));
    }
    SpriteBatch.drawText(font, "Press ^9907^999 to play cue file", new Vector2(10, 130));
    SpriteBatch.drawText(font, "Press ^990qwertyui^999 to do music", new Vector2(10, 150));
    SpriteBatch.drawText(font, "Press ^9909^999 to play/stop music", new Vector2(10, 170));

    var screenH = Renderer.getResolution().y;
    SpriteBatch.drawText(font, "Hold ^990Left Arrow^999 to on left channel", new Vector2(10, screenH - 50));
    SpriteBatch.drawText(font, "Hold ^990Right Arrow^999 to on right channel", new Vector2(10, screenH - 30));

    SpriteBatch.end();
}

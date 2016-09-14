var nut = getTexture("onutLogo.png");
var frame = getTexture("frameSmall.png");

var angleAnim1 = new NumberAnim();
var angleAnim2 = new NumberAnim();
var bounceAnim = new Vector2Anim();
var frameAnim = new RectAnim();
var callbackAnim = new Vector2Anim();
var batchTransform = new MatrixAnim();

function playBounceSound() {
    playSound("bounce.wav", .5);
}

// Loop
angleAnim1.playSingle(0, 360, 2, Tween.LINEAR, Loop.LOOP);

// Pingpong loop
angleAnim2.playSingle(0, 720, 2, Tween.EASE_BOTH, Loop.PING_PONG_LOOP);

// Bounce
bounceAnim.playSingle(new Vector2(0, 0), new Vector2(0, 200), 2, Tween.BOUNCE_OUT, Loop.LOOP);

// Keyframes
frameAnim.playKeyFrames(new Rect(32, 192, 128, 128), [
    { value: new Rect(32, 192, 128, 400), duration: 2, tween: Tween.EASE_BOTH },
    { value: new Rect(32, 192, 192, 400), duration: 1.5, tween: Tween.BOUNCE_OUT },
    { value: new Rect(32, 192, 128, 128), duration: 1.5, tween: Tween.BOUNCE_OUT },
], Loop.LOOP);

// Queuing + Callbacks
callbackAnim.set(Vector2.ZERO);
callbackAnim.queue(new Vector2(256, 0), 1.5, Tween.EASE_IN, playBounceSound);
callbackAnim.queue(Vector2.ZERO, 1.5, Tween.EASE_IN, playBounceSound);
callbackAnim.play(Loop.LOOP);

function update(dt) {
    // Create some crazy screen animation using a Matrix transform.
    if (Input.isJustDown(Key.SPACE_BAR)) {
        var screen = Renderer.getResolution();
        batchTransform.playKeyFrames(Matrix.IDENTITY, [
            {
                value: Matrix.createTranslation(new Vector3(-screen.x, -screen.y, 0)).mul(
                    Matrix.createRotationZ(45).mul(
                        Matrix.createTranslation(new Vector3(screen.x, screen.y, 0)))),
                duration: 1, tween: Tween.LINEAR
            },
            {
                value: Matrix.createTranslation(new Vector3(-screen.x, -screen.y, 0)).mul(
                    Matrix.createRotationZ(-45).mul(
                        Matrix.createTranslation(new Vector3(screen.x, screen.y, 0)))),
                duration: 2, tween: Tween.LINEAR
            },
            {
                value: Matrix.IDENTITY, duration: 1, tween: Tween.LINEAR
            }
        ]);
    }
}

function render() {
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    // Draw sprites
    SpriteBatch.begin(batchTransform.get());
    SpriteBatch.drawSprite(nut, new Vector2(128, 128), Color.WHITE, angleAnim1.get());
    SpriteBatch.drawSprite(nut, new Vector2(256, 128), Color.WHITE, angleAnim2.get());
    SpriteBatch.drawSprite(nut, new Vector2(384, 128).add(bounceAnim.get()), Color.WHITE);
    SpriteBatch.drawRectScaled9RepeatCenters(frame, frameAnim.get(), new Vector4(48, 48, 48, 48));
    SpriteBatch.drawSprite(nut, new Vector2(512, 256).add(callbackAnim.get()));
    SpriteBatch.end();
}

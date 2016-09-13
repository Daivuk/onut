var gamepads = [
    getTexture("gamepad0.png"),
    getTexture("gamepad1.png"),
    getTexture("gamepad2.png"),
    getTexture("gamepad3.png")
];
var gamepad = getTexture("gamepad.png");
var lthumb = getTexture("lthumb.png");
var rthumb = getTexture("rthumb.png");
var lthumbPress = getTexture("lthumbPress.png");
var rthumbPress = getTexture("rthumbPress.png");
var rb = getTexture("rb.png");
var lb = getTexture("lb.png");
var back = getTexture("back.png");
var start = getTexture("start.png");
var a = getTexture("a.png");
var b = getTexture("b.png");
var x = getTexture("x.png");
var y = getTexture("y.png");
var dleft = getTexture("dleft.png");
var dright = getTexture("dright.png");
var dup = getTexture("dup.png");
var ddown = getTexture("ddown.png");
var lt = getTexture("lt.png");
var rt = getTexture("rt.png");

function drawController(index, position) {
    if (!GamePad.isConnected(index)) {
        SpriteBatch.drawSprite(gamepad, position, new Color(.5));
    }
    else {
        SpriteBatch.drawSprite(gamepads[index], position);
        SpriteBatch.drawSprite(lthumb, GamePad.getLeftThumb(index).mul(12).add(position));
        SpriteBatch.drawSprite(rthumb, GamePad.getRightThumb(index).mul(12).add(position));
        if (GamePad.isDown(index, Button.LEFT_THUMBSTICK)) SpriteBatch.drawSprite(lthumbPress, GamePad.getLeftThumb(index).mul(12).add(position));
        if (GamePad.isDown(index, Button.RIGHT_THUMBSTICK)) SpriteBatch.drawSprite(rthumbPress, GamePad.getRightThumb(index).mul(12).add(position));
        if (GamePad.isDown(index, Button.RIGHT_BUMPER)) SpriteBatch.drawSprite(rb, position);
        if (GamePad.isDown(index, Button.LEFT_BUMPER)) SpriteBatch.drawSprite(lb, position);
        if (GamePad.isDown(index, Button.BACK)) SpriteBatch.drawSprite(back, position);
        if (GamePad.isDown(index, Button.START)) SpriteBatch.drawSprite(start, position);
        if (GamePad.isDown(index, Button.A)) SpriteBatch.drawSprite(a, position);
        if (GamePad.isDown(index, Button.B)) SpriteBatch.drawSprite(b, position);
        if (GamePad.isDown(index, Button.X)) SpriteBatch.drawSprite(x, position);
        if (GamePad.isDown(index, Button.Y)) SpriteBatch.drawSprite(y, position);
        if (GamePad.isDown(index, Button.DPAD_LEFT)) SpriteBatch.drawSprite(dleft, position);
        if (GamePad.isDown(index, Button.DPAD_RIGHT)) SpriteBatch.drawSprite(dright, position);
        if (GamePad.isDown(index, Button.DPAD_UP)) SpriteBatch.drawSprite(dup, position);
        if (GamePad.isDown(index, Button.DPAD_DOWN)) SpriteBatch.drawSprite(ddown, position);
        if (GamePad.isDown(index, Button.LEFT_TRIGGER)) SpriteBatch.drawSprite(lt, position);
        if (GamePad.isDown(index, Button.RIGHT_TRIGGER)) SpriteBatch.drawSprite(rt, position);
    }
}

function render() {
    // Clear
    Renderer.clear(Color.BLACK);

    // Begin a batch
    SpriteBatch.begin();

    var screenResolution = Renderer.getResolution();
    drawController(0, new Vector2(screenResolution.x * .3, screenResolution.y * .3));
    drawController(1, new Vector2(screenResolution.x * .7, screenResolution.y * .3));
    drawController(2, new Vector2(screenResolution.x * .3, screenResolution.y * .7));
    drawController(3, new Vector2(screenResolution.x * .7, screenResolution.y * .7));

    // End and flush the batch
    SpriteBatch.end();
}

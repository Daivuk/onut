var font = Font("main.fnt");
var fontBig = Font("mainBig.fnt");

function render() {
    // Clear
    Renderer.clear(Color(.35, .35, .35, 1));

    // Begin a batch
    SpriteBatch.begin();

    // Normal text
    SpriteBatch.drawText(font, "Lorem ipsum dolor sit amet.", Vector2(10, 10));

    // Bigger font
    SpriteBatch.drawText(fontBig, "CONSECTETUR ADIPISCING ELIT", Vector2(10, 100));

    // Playing with colors
    // ^RGB, where RGB ranged from 0-9
    SpriteBatch.drawText(font, "^933Sed ^993do ^393eiusmod ^399tempor ^339incididunt ^939ut ^111labore ^222et ^444dolore ^666magna ^888aliqua", Vector2(10, 200));

    // Alignment
    var screenResolution = Renderer.getResolution();
    SpriteBatch.drawText(font, "Left aligned", Vector2(10, 300), Align.Left);
    SpriteBatch.drawText(font, "Center aligned", Vector2(screenResolution.x / 2, 300), Align.Center);
    SpriteBatch.drawText(font, "Right aligned", Vector2(screenResolution.x - 10, 300), Align.Right);

    // Cheap outline
    SpriteBatch.drawOutlinedText(font, "Duis aute irure dolor in reprehenderit - Cheap oulines", Vector2(10, 400));

    // Pretty outline
    SpriteBatch.drawPrettyOutlinedText(font, "Duis aute irure dolor in reprehenderit - Better outlines", Vector2(10, 450));


    // End the batch
    SpriteBatch.end();
}

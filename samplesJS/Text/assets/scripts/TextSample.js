var font = getFont("main.fnt");
var fontBig = getFont("mainBig.fnt");

function render() {
    // Clear
    Renderer.clear(new Color(.35, .35, .35, 1));

    // Begin a batch
    SpriteBatch.begin();

    // Normal text
    SpriteBatch.drawText(font, "Lorem ipsum dolor sit amet.", new Vector2(10, 10));

    // Bigger font
    SpriteBatch.drawText(fontBig, "CONSECTETUR ADIPISCING ELIT", new Vector2(10, 100));

    // Playing with colors
    // ^RGB, where RGB ranged from 0-9
    SpriteBatch.drawText(font, "^933Sed ^993do ^393eiusmod ^399tempor ^339incididunt ^939ut ^111labore ^222et ^444dolore ^666magna ^888aliqua", new Vector2(10, 200));

    // Alignment
    var screenResolution = Renderer.getResolution();
    SpriteBatch.drawText(font, "Left aligned", new Vector2(10, 300), Vector2.LEFT);
    SpriteBatch.drawText(font, "Center aligned", new Vector2(screenResolution.x / 2, 300), Vector2.CENTER);
    SpriteBatch.drawText(font, "Right aligned", new Vector2(screenResolution.x - 10, 300), Vector2.RIGHT);

    // Cheap outline
    SpriteBatch.drawOutlinedText(font, "Duis aute irure dolor in reprehenderit - Cheap oulines", new Vector2(10, 400));

    // Pretty outline
    SpriteBatch.drawPrettyOutlinedText(font, "Duis aute irure dolor in reprehenderit - Better outlines", new Vector2(10, 450));

    // End the batch
    SpriteBatch.end();
}

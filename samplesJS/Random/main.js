function render()
{
    Renderer.clear(Color.BLACK);

    PrimitiveBatch.begin(PrimitiveMode.POINT_LIST);

    if (Random.randBool()) PrimitiveBatch.draw(new Vector2(10, 25), new Color(0, 1, 0, 1)); // 50%
    if (Random.randBool(.25)) PrimitiveBatch.draw(new Vector2(20, 25), new Color(0, 1, 0, 1)); // 25%

    for (var i = 0; i < 100; ++i)
    {
        PrimitiveBatch.draw(new Vector2(Random.randInt(10) * 10, 5), new Color(1, 1, 0, 1));
        PrimitiveBatch.draw(new Vector2(Random.randInt(5, 10) * 10, 10), new Color(1, 1, 0, 1));
        PrimitiveBatch.draw(new Vector2(Random.randInt(100), 15), new Color(0, 1, 1, 1));
        PrimitiveBatch.draw(new Vector2(Random.randInt(50, 100), 20), new Color(0, 1, 1, 1));
        PrimitiveBatch.draw(Random.randVector2(new Vector2(25, 25)).add(new Vector2(0, 30)), new Color(1, 0, 1, 1));
        PrimitiveBatch.draw(Random.randVector2(new Vector2(10, 10), new Vector2(25, 25)).add(new Vector2(30, 30)), new Color(1, 0, 1, 1));
        PrimitiveBatch.draw(Random.randCircle(new Vector2(50, 100), 25), new Color(1, 1, 1, 1));
        PrimitiveBatch.draw(Random.randCircleEdge(new Vector2(150, 100), 25), new Color(1, 1, 1, 1));
    }

    PrimitiveBatch.end();

    SpriteBatch.begin();

    SpriteBatch.drawRect(null, new Rect(0, 150, 20, 12), Random.randColor());
    SpriteBatch.drawRect(null, new Rect(21, 150, 20, 12), Random.randColor(.5));
    SpriteBatch.drawRect(null, new Rect(42, 150, 20, 12), Random.randColor(new Color(1, 0, 1)));
    SpriteBatch.drawRect(null, new Rect(63, 150, 20, 12), Random.randColor(new Color(0, 1, 0), new Color(1, 0, 0)));
    SpriteBatch.drawRect(null, new Rect(63, 150, 20, 12), Random.randColor(new Color(0, 1, 0), new Color(1, 0, 0)));

    var palette = [
        Color.fromHexRGB(0x43A08E),
        Color.fromHexRGB(0xB3E0CB),
        Color.fromHexRGB(0xEE5351),
        Color.fromHexRGB(0xD19A8E),
        Color.fromHexRGB(0xDBC9A3)
    ];
    var x = 0;
    for (var i = 0; i < palette.length; ++i)
    {
        SpriteBatch.drawRect(null, new Rect(x, 170, 20, 12), palette[i]);
        x += 21;
    }
    SpriteBatch.drawRect(null, new Rect(0, 183, 20, 12), Random.randPalette(palette));
    SpriteBatch.drawRect(null, new Rect(21, 183, 20, 12), Random.randPalette(palette, .5));

    SpriteBatch.end();
}

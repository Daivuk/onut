var onutLogo = getTexture("onutLogo.png");

function render()
{
    Renderer.clear(Color.BLACK);

    // Lines
    PrimitiveBatch.begin(PrimitiveMode.LINE_STRIP);
    PrimitiveBatch.draw(new Vector2(10, 10), new Color(1, 0, 0, 1));
    PrimitiveBatch.draw(new Vector2(100, 100), new Color(1, 1, 0, 1));
    PrimitiveBatch.draw(new Vector2(75, 200), new Color(0, 1, 0, 1));
    PrimitiveBatch.draw(new Vector2(25, 50), new Color(0, 0, 1, 1));
    PrimitiveBatch.end();

    // Points
    PrimitiveBatch.begin(PrimitiveMode.POINT_LIST);
    PrimitiveBatch.draw(new Vector2(110, 10), new Color(1, 0, 0, 1));
    PrimitiveBatch.draw(new Vector2(110, 50), new Color(1, 1, 0, 1));
    PrimitiveBatch.draw(new Vector2(150, 50), new Color(0, 1, 0, 1));
    PrimitiveBatch.draw(new Vector2(150, 10), new Color(0, 0, 1, 1));
    PrimitiveBatch.end();

    // Triangles
    PrimitiveBatch.begin(PrimitiveMode.TRIANGLE_LIST);
    PrimitiveBatch.draw(new Vector2(250, 10), new Color(1, 0, 0, 1));
    PrimitiveBatch.draw(new Vector2(200, 50), new Color(1, 1, 0, 1));
    PrimitiveBatch.draw(new Vector2(300, 50), new Color(0, 1, 0, 1));
    PrimitiveBatch.draw(new Vector2(250, 110), new Color(0, 0, 1, 1));
    PrimitiveBatch.draw(new Vector2(200, 150), new Color(1, 0, 1, 1));
    PrimitiveBatch.draw(new Vector2(300, 150), new Color(0, 1, 1, 1));
    PrimitiveBatch.end();

    // Textured
    PrimitiveBatch.begin(PrimitiveMode.TRIANGLE_LIST, onutLogo);
    PrimitiveBatch.draw(new Vector2(350, 10), Color.WHITE, new Vector2(.5, 0));
    PrimitiveBatch.draw(new Vector2(300, 100), Color.WHITE, new Vector2(0, 1));
    PrimitiveBatch.draw(new Vector2(400, 100), Color.WHITE, new Vector2(1, 1));
    PrimitiveBatch.end();
}


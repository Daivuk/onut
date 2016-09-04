var nutTexture = Texture("onutLogo.png");
//var font = Font("font.fnt");

function update(dt)
{
}

function render()
{
    Renderer.clear(Color.Black);

    SpriteBatch.begin();

    // Draw a pink rect top left
    SpriteBatch.drawRect(Rect(0, 0, 100, 100), Color(1, 0, .5, 1));

    // Draw a textured rectangle
    SpriteBatch.drawTexturedRect(nutTexture, Rect(128, 128, nutTexture.w, nutTexture.h), Color.White);

    //font.print("Hello World!", 10, 200, 1, 1, 1, 1);
    SpriteBatch.end();
}

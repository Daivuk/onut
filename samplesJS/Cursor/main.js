var cursorRect1 = new Rect(0, 0, 100, 100);
var cursorRect2 = new Rect(100, 0, 100, 100);
var cursorRect3 = new Rect(200, 0, 100, 100);
var cursorRect4 = new Rect(300, 0, 100, 100);
var cursorRect5 = new Rect(100, 200, 200, 100);

function update(dt)
{
    var mousePos = Input.getMousePos();

    if (cursorRect1.contains(mousePos))
    {
        Input.setMouseIcon("green.png", 15, 10);
    }
    else if (cursorRect2.contains(mousePos))
    {
        Input.setMouseIcon("sword.png", 8, 8);
    }
    else if (cursorRect3.contains(mousePos))
    {
        Input.setMouseIcon("hand.png", 11, 11);
    }
    else if (cursorRect4.contains(mousePos))
    {
        Input.setMouseIcon("lemon.png", 8, 10);
    }
    else
    {
        Input.setMouseIcon(null);
    }

    if (cursorRect5.contains(mousePos))
    {
        Input.setMouseVisible(false);
    }
    else
    {
        Input.setMouseVisible(true);
    }
}

function render()
{
    Renderer.clear(Color.BLACK);

    SpriteBatch.begin();
    SpriteBatch.drawRect(null, cursorRect1, new Color(.2, 0, 0, .2));
    SpriteBatch.drawRect(null, cursorRect2, new Color(0, .2, 0, .2));
    SpriteBatch.drawRect(null, cursorRect3, new Color(0, 0, .2, .2));
    SpriteBatch.drawRect(null, cursorRect4, new Color(.2, .2, 0, .2));
    SpriteBatch.drawRect(null, cursorRect5, new Color(.2, 0, .2, .2));
    SpriteBatch.end();
}

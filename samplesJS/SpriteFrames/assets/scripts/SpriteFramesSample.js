var font = getFont("font.fnt");

var baltPng = getTexture("balt.png");
var spriteAnim = createSpriteAnimInstance("baltAnims.spriteanim");

var baltPngASE = getTexture("hero.png");
var spriteAnimASE = createSpriteAnimInstance("hero.json");

function update(dt)
{
    if (Input.isJustDown(Key._1))
    {
        spriteAnim.play("idle");
        spriteAnimASE.play("idle");
    }
    if (Input.isJustDown(Key._2))
    {
        spriteAnim.forcePlay("run");
        spriteAnimASE.forcePlay("walk");
    }
    if (Input.isJustDown(Key._3))
    {
        spriteAnim.forcePlay("run", 2);
        spriteAnimASE.forcePlay("walk", 2);
    }
    if (Input.isJustDown(Key._4))
    {
        spriteAnim.play("attack");
        spriteAnimASE.play("attack_fliph");
    }
}

function render()
{
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));

    // Draw sprites
    SpriteBatch.begin(Matrix.createScale(2));
    Renderer.setFilterMode(FilterMode.NEAREST);
    SpriteBatch.drawSprite(baltPng, new Vector2(4, 8), Color.WHITE, 0, 1, Vector2.TOP_LEFT);
    SpriteBatch.drawSprite(baltPngASE, new Vector2(4, 200), Color.WHITE, 0, 1, Vector2.TOP_LEFT);

    // Sprite the animation
    SpriteBatch.drawSpriteAnim(spriteAnim, new Vector2(256, 64), Color.WHITE, 0, 2);
    SpriteBatch.drawSpriteAnim(spriteAnimASE, new Vector2(256, 204), Color.WHITE, 0, 2);

    // Draw the origin
    SpriteBatch.drawCross(new Vector2(256, 64), 5, new Color(1, 1, 0, 1));
    SpriteBatch.drawCross(new Vector2(256, 204), 5, new Color(1, 1, 0, 1));
    SpriteBatch.end();

    SpriteBatch.begin();
    SpriteBatch.drawText(font, "Sheet", new Vector2(8, 8), Vector2.LEFT);
    SpriteBatch.drawText(font, "Press ^9901^999 to play ^991idle^999 anim", new Vector2(8, 300), Vector2.LEFT);
    SpriteBatch.drawText(font, "Press ^9902^999 to play ^991run^999 anim", new Vector2(8, 320), Vector2.LEFT);
    SpriteBatch.drawText(font, "Press ^9903^999 to play ^991run fast^999 anim", new Vector2(8, 340), Vector2.LEFT);
    SpriteBatch.drawText(font, "Press ^9904^999 to play ^991attack^999 anim", new Vector2(8, 360), Vector2.LEFT);
    SpriteBatch.end();
}

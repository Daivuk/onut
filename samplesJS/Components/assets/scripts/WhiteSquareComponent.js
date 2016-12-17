"component";

var size = 64;

function onRender2d() {
    var transform = getTransform();
    SpriteBatch.drawSprite(null, transform, size);
}

/*
function WhiteSquareComponent() {
    this.size = 64;
}
WhiteSquareComponent.prototype.onRender2d = function() {
    var entity = this.getEntity();
    var transform = entity.getTransform();
    SpriteBatch.drawSprite(null, transform, this.size);
}
registerComponent("WhiteSquareComponent");

function RotatingComponent() {}
RotatingComponent.prototype.onUpdate = function(dt) {
    var entity = this.getEntity();
    var transform = entity.getTransform();
    transfrom = Matrix.createRotationZ(dt * 90).mul(transform);
    entity.setTransform(transform);
}
registerComponent("RotatingComponent");

function BlinkComponent() {
    var blink = 0;
}
BlinkComponent.prototype.onUpdate = function(dt) {
    blink += dt * 2;
    if (blink >= 1) blink -= 1;
    getEntity().setVisible(blink < .5);
}
registerComponent("BlinkComponent");

function DeactivateComponent() {
    var blink = 0;
}
DeactivateComponent.prototype.onUpdate = function(dt) {
    blink += dt;
    if (blink >= 1) blink -= 1;
    var rotatingComponent = getEntity().getComponent("RotatingComponent");
    rotatingComponent.setEnabled(blink < .5);
}
registerComponent("DeactivateComponent");

var screen = Renderer.getResolution();

// Static square
var entity = Entity.create();
entity.setStatic(true); // This square will not update and his rendering will potentially be batched.
entity.addComponent(WhiteSquareComponent);
entity.setTransform(Matrix.createTranslation(new Vector3(screen.x / 4, screen.y / 4, 0)));
*/
// Rotating square
//entity = Entity.create();
//entity.addComponent("WhiteSquareComponent");

/*
    // Rotating square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(OScreenCenterf));
    // Smaller square orbiting this one
    {
        auto pSmallSquare = OEntity::create();
        auto pWhiteSquareComponent = pSmallSquare->addComponent<WhiteSquareComponent>();
        pWhiteSquareComponent->setSize(32.0f);
        pSmallSquare->setLocalTransform(Matrix::CreateTranslation(Vector2(64.0f, 0)));
        pSquare->add(pSmallSquare);
    }

    // Blinking square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<BlinkingComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(OScreenCenterf * 1.5f));

    // Intermittent rotating square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->addComponent<DeactivateComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(Vector2(OScreenCenterXf / 2.0f, OScreenCenterYf * 1.5f)));

    // All combined
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->addComponent<BlinkingComponent>();
    pSquare->addComponent<DeactivateComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(Vector2(OScreenCenterXf * 1.5f, OScreenCenterYf / 2.0f)));
*/

function render()
{
    // Clear
    Renderer.clear(Color.fromHexRGB(0x1d232d));
}

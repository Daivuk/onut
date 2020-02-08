var showGBuffer = false
var aoEnabled = true
var angleAnim = new NumberAnim(0)
angleAnim.playSingle(0, 360, 5, Tween.LINEAR, Loop.LOOP)

function update(dt)
{
    if (Input.isJustDown(Key.G)) showGBuffer = !showGBuffer
    if (Input.isJustDown(Key.A)) aoEnabled = !aoEnabled
}

function render()
{
    // Clear buffers and setup a 3D view and 
    Renderer.clear(new Color(0, 0, 0, 1))
    Renderer.clearDepth()
    Renderer.setupFor3D(new Vector3(0, 0, 0.5), new Vector3(1, 1, 0.5), Vector3.UNIT_Z, 70)

    // Deferred
    // We could have batched the models here, but we are testing the deferred ordering.
    // See the ModelBatch sample for batching.
    Deferred.begin()

    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(0, 0, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(1, 0, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(2, 0, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(0, 1, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(1, 1, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(2, 1, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(0, 2, 0)))
    Deferred.addSolid(getModel("floor.model"), Matrix.createTranslation(new Vector3(1, 2, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(0, 0, 0)))
    Deferred.addSolid(getModel("lightWell.model"), Matrix.createTranslation(new Vector3(1, 0, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(2, 0, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(0, 1, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(1, 1, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(2, 1, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(0, 2, 0)))
    Deferred.addSolid(getModel("ceiling.model"), Matrix.createTranslation(new Vector3(1, 2, 0)))
    Deferred.addSolid(getModel("stairs.model"), Matrix.createTranslation(new Vector3(.5, 2.5, 0)))
    Deferred.addSolid(getModel("wall.model"), Matrix.createTranslation(new Vector3(-.5, 2.5, 0)))
    Deferred.addSolid(getModel("wall.model"), Matrix.createRotationZ(-90).mul(Matrix.createTranslation(new Vector3(1.5, 2.5, 0))))
    Deferred.addSolid(getModel("wall.model"), Matrix.createTranslation(new Vector3(1.5, 1.5, 0)))
    Deferred.addSolid(getModel("wall.model"), Matrix.createRotationZ(-90).mul(Matrix.createTranslation(new Vector3(2.5, 1.5, 0))))
    Deferred.addSolid(getModel("wall.model"), Matrix.createRotationZ(-90).mul(Matrix.createTranslation(new Vector3(2.5, 0.5, 0))))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(-.5, 2.5, 0)))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(.5, 2.5, 0)))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(1.5, 2.5, 0)))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(1.5, 1.5, 0)))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(2.5, 1.5, 0)))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(2.5, 0.5, 0)))
    Deferred.addSolid(getModel("column.model"), Matrix.createTranslation(new Vector3(2.5, -.5, 0)))

    Deferred.addAlphaTest(getModel("alphaTest.model"), Matrix.createRotationZ(angleAnim.get()).mul(
        Matrix.createTranslation(new Vector3(1.0, 0.5, 0.5))));
    
    Deferred.addTransparent(getModel("blend.model"), Matrix.createRotationZ(angleAnim.get()).mul(
        Matrix.createTranslation(new Vector3(2.0, 0.5, 0.5))));
    Deferred.addTransparent(getModel("blend.model"), Matrix.createRotationZ(angleAnim.get()).mul(
        Matrix.createTranslation(new Vector3(0.8, 3.5, 0.5))));
    Deferred.addTransparent(getModel("blend.model"), Matrix.createRotationZ(angleAnim.get()).mul(
        Matrix.createTranslation(new Vector3(0.8 / 2, 3.5 / 2, 0.35))));
    Deferred.addTransparent(getModel("blend.model"), Matrix.createRotationZ(angleAnim.get()).mul(
        Matrix.createTranslation(new Vector3(1.0, 0.5, 0.5))));

    Deferred.addOmni(new Vector3(1, 5.0, 1.5), 3.0, new Color(0.5, 1.0, 1.5))
    Deferred.addOmni(new Vector3(1, 0, 1.1), 1.3, new Color(0.5, 1.0, 1.5))
    Deferred.addOmni(new Vector3(1, 2, 0.5), 1.5, new Color(1.5, 1.0, 0.5))

    Deferred.end(new Color(0.05, 0.075, 0.1),
                 aoEnabled, 0.15, 2.0)

    // Debug stuff
    SpriteBatch.begin()
    var res = Renderer.getResolution()
    if (showGBuffer)
    {
        SpriteBatch.drawRect(Deferred.getAlbedo(), new Rect(0, 0, res.x / 2.0, res.y / 2.0))
        SpriteBatch.drawRect(Deferred.getNormal(), new Rect(res.x / 2.0, 0, res.x / 2.0, res.y / 2.0))
        SpriteBatch.drawRect(Deferred.getDepth(), new Rect(0, res.y / 2.0, res.x / 2.0, res.y / 2.0))
        SpriteBatch.drawRect(Deferred.getMaterial(), new Rect(res.x / 2.0, res.y / 2.0, res.x / 2.0, res.y / 2.0))
    }
    SpriteBatch.drawText(getFont("font.fnt"), "Press G to toggle gbuffer", new Vector2(0, 20), Vector2.TOP_LEFT, new Color(1, 1, 0))
    SpriteBatch.drawText(getFont("font.fnt"), "Press A to toggle SSAO", new Vector2(0, 35), Vector2.TOP_LEFT, new Color(1, 1, 0))
    SpriteBatch.end();
}

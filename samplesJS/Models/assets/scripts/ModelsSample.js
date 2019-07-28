var models = [
    getModel("teapot.model"),
    getModel("barn.model"),
    getModel("box.model")
]
models[2].setTexture(0, getTexture("diffuse.png")); // Swaping texture example

var angleAnim = new NumberAnim()
angleAnim.playSingle(0, -360, 8, Tween.LINEAR, Loop.LOOP)

function render()
{
    Renderer.setupFor3D(
        new Vector3(8, 8, 15),
        new Vector3(0, 0, 5),
        Vector3.UNIT_Z,
        90
    )

    Renderer.clear(new Color(.5, .5, .5, 1))
    Renderer.clearDepth();

    models[0].render(
        Matrix.createRotationZ(angleAnim.get())
    );
    models[1].render(
        Matrix.createRotationZ(angleAnim.get()).mul(
            Matrix.createTranslation(new Vector3(-5, -30, 0))
        )
    );

    var transform =           Matrix.createTranslation(new Vector3(7.527910, 19.933542, 7.614748))
    transform = transform.mul(Matrix.createRotationZ(angleAnim.get()))
    transform = transform.mul(Matrix.createTranslation(new Vector3(-30, -5, 0)))
    models[2].render(transform);
}

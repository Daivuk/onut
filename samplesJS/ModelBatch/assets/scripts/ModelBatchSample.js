var scene = Model.createFromBatch([
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(0, 0, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(1, 0, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(2, 0, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(0, 1, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(1, 1, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(2, 1, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(0, 2, 0)) },
    { model: getModel("floor.model"), transform: Matrix.createTranslation(new Vector3(1, 2, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(0, 0, 0)) },
    { model: getModel("lightWell.model"), transform: Matrix.createTranslation(new Vector3(1, 0, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(2, 0, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(0, 1, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(1, 1, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(2, 1, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(0, 2, 0)) },
    { model: getModel("ceiling.model"), transform: Matrix.createTranslation(new Vector3(1, 2, 0)) },
    { model: getModel("stairs.model"), transform: Matrix.createTranslation(new Vector3(.5, 2.5, 0)) },
    { model: getModel("wall.model"), transform: Matrix.createTranslation(new Vector3(-.5, 2.5, 0)) },
    { model: getModel("wall.model"), transform: Matrix.createRotationZ(-90).mul(Matrix.createTranslation(new Vector3(1.5, 2.5, 0))) },
    { model: getModel("wall.model"), transform: Matrix.createTranslation(new Vector3(1.5, 1.5, 0)) },
    { model: getModel("wall.model"), transform: Matrix.createRotationZ(-90).mul(Matrix.createTranslation(new Vector3(2.5, 1.5, 0))) },
    { model: getModel("wall.model"), transform: Matrix.createRotationZ(-90).mul(Matrix.createTranslation(new Vector3(2.5, 0.5, 0))) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(-.5, 2.5, 0)) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(.5, 2.5, 0)) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(1.5, 2.5, 0)) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(1.5, 1.5, 0)) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(2.5, 1.5, 0)) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(2.5, 0.5, 0)) },
    { model: getModel("column.model"), transform: Matrix.createTranslation(new Vector3(2.5, -.5, 0)) }
])

Renderer.setSun(Vector3.UNIT_Z, Color.BLACK);
Renderer.setAmbient(new Color(0.05, 0.075, 0.1));
Renderer.setLight(0, new Vector3(1, 5.0, 1.5), 3.0, new Color(0.5, 1.0, 1.5)); // Stairs
Renderer.setLight(1, new Vector3(1, 0, 1.1), 1.3, new Color(0.5, 1.0, 1.5)); // Light well
Renderer.setLight(2, new Vector3(1, 2, 0.5), 1.5, new Color(1.5, 1.0, 0.5)); // Torch

function render()
{
    Renderer.setupFor3D(
        new Vector3(0, 0, 0.5),
        new Vector3(1, 1, 0.5),
        Vector3.UNIT_Z,
        90
    )

    Renderer.clear(new Color(0, 0, 0, 1))
    Renderer.clearDepth();

    scene.render(Matrix.IDENTITY);
}

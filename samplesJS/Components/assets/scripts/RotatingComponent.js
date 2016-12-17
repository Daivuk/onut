"component";

function onUpdate(dt) {
    var transform = getTransform();
    transform = Matrix.createRotationZ(dt * 90).mul(transform);
    setTransform(transform);
}

function _onCreate(entity, component)
{
    component.myClearColor = new Color(1, 0, 1, 1);
}

function _onEnable(entity, component)
{
    entity.camera2D.clearColor = component.myClearColor;
}

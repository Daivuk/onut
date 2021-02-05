// DO NOT EDIT - THIS FILE IS GENERATED

// Entity
declare class Entity {
    name: string;

    getPosition(): Vector3;
    getWorldPosition(): Vector3;
    setPosition(position: Vector3);

    getTransform(): Matrix;
    getWorldTransform(): Matrix;
    setTransform(transform: Matrix);

    getComponent(name: string): object;
    addComponent(name: string): object;
    removeComponent(name: string);
    disableComponent(name: string);
    enableComponent(name: string);

    add(child: Entity);
    insert(before: Entity, child: Entity);
    remove(child: Entity);
    removeAll();
    getChildCount(): number;
    getChild(index: number): Entity;
    getChild(name: string, deepSearch: boolean = true): UEntityI;
    getParent(): Entity;

    copy(): Entity;

    // Quick access to default components
    camera2D: Camera2D;
    spriteAnimRenderer: SpriteAnimRenderer;
    spriteRenderer: SpriteRenderer;
    tiledMapRenderer: TiledMapRenderer;
}
declare function findEntity(name: string): Entity;
declare function destroyEntity(entity: Entity);

// Scene
declare function loadScene(name: string);

// Components
declare class Camera2D
{
    zoom: number;
    clearScreen: boolean;
    clearColor: Color;
    origin: Vector2;
}

declare class SpriteAnimRenderer
{
    spriteAnim: SpriteAnim;
    anim: string;
    color: Color;
    smooth: boolean;
    blendMode: int;
}

declare class SpriteRenderer
{
    texture: Texture;
    color: Color;
    origin: Vector2;
    smooth: boolean;
    blendMode: int;
}

declare class TiledMapRenderer
{
    tiledMap: TiledMap;
    smooth: boolean;
    blendMode: int;
}


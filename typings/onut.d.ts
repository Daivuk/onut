interface Color {
    r: number;
    g: number;
    b: number;
    a: number;
}
declare function Color(r: number, g: number, b: number, a: number): Color;
declare function ColorHexRGB(hex: number): Color;
declare function ColorHexRGBA(hex: number): Color;
declare function whiteColor(): Color;
declare function blackColor(): Color;
declare function transparentColor(): Color;

interface Rect {
    x: number;
    y: number;
    w: number;
    h: number;
}
declare function Rect(x: number, y: number, w: number, h: number): Rect;

interface Vector2 {
    x: number;
    y: number;
}
declare function Vector2(x: number, y: number): Vector2;

interface Vector3 {
    x: number;
    y: number;
    z: number;
}
declare function Vector3(x: number, y: number, z: number): Vector3;

interface Vector4 {
    x: number;
    y: number;
    z: number;
    w: number;
}
declare function Vector4(x: number, y: number, z: number, w: number): Vector4;

interface Matrix {
    _11: number; _12: number; _13: number; _14: number;
    _21: number; _22: number; _23: number; _24: number;
    _31: number; _32: number; _33: number; _34: number;
    _41: number; _42: number; _43: number; _44: number;
}
declare function Matrix(
    _11: number, _12: number, _13: number, _14: number,
    _21: number, _22: number, _23: number, _24: number,
    _31: number, _32: number, _33: number, _34: number,
    _41: number, _42: number, _43: number, _44: number): Matrix;
declare function identityMatrix(): Matrix;
declare function rotateMatrixAroundX(matrix: Matrix, angle: number): Matrix;
declare function rotateMatrixAroundY(matrix: Matrix, angle: number): Matrix;
declare function rotateMatrixAroundZ(matrix: Matrix, angle: number): Matrix;
declare function scaleMatrix(matrix: Matrix, x: number, y: number, z: number): Matrix;
declare function translateMatrix(matrix: Matrix, x: number, y: number, z: number): Matrix;

// Resources
interface Texture {
    w: number;
    h: number;
}
declare function Texture(name: string): Texture;
interface Font { }
declare function Font(name: string): Font;

// Renderer
interface Renderer {
    clear(color: Color);
    getResolution(): Vector2;
}
declare var Renderer: Renderer;

// Spritebatch
interface SpriteBatch {
    begin();
    end();

    drawRect(texture: Texture, rect: Rect, color: Color);
    drawinclinedRect(texture: Texture, rect: Rect, incliinedRatio: number, color: Color);
    drawRectWithColors(texture: Texture, rect: Rect, topLeft: Color, bottomLeft: Color, bottomRight: Color, topRight: Color);
    drawRectWithUVs(texture: Texture, rect: Rect, uvs: Vector4, color: Color);
    draw4Corner(texture: Texture, rect: Rect, color: Color);
    drawRectScaled9(texture: Texture, rect: Rect, padding: Vector4, color: Color);
    drawRectScaled9RepeatCenters(texture: Texture, rect: Rect, padding: Vector4, color: Color);
    drawSprite(texture: Texture, position: Vector2, color: Color, rotation: number, scale: number, origin: Vector2);
    drawTransformedSprite(texture: Texture, transform: Matrix, color: Color, scale: Vector2, origin: Vector2);
    drawBeam(texture: Texture, from: Vector2, to: Vector2, size: Number, color: Color, uOffset: number, uScale: number);
    drawCross(position: Vector2, size: number, color: Color);

    drawText(font: Font, text: string, position: Vector2, align: Vector2, color: Color);
    drawOutlinedText(font: Font, text: string, position: Vector2, align: Vector2, color: Color, outlineColor: Color, outlineSize: number);
    drawPrettyOutlinedText(font: Font, text: string, position: Vector2, align: Vector2, color: Color, outlineColor: Color, outlineSize: number);

    setBlend(blendMode: Number);
    setFilter(filterMode: Number);
}
declare var SpriteBatch: SpriteBatch;

// Blend mode
interface BlendMode {
    Opaque: number;
    Alpha: number;
    Add: number;
    PreMultiplied: number;
    Multiply: number;
    ForceWrite: number;
}
declare var BlendMode: BlendMode;

// filtering
interface FilterMode {
    Nearest: number;
    Linear: number;
}
declare var FilterMode: FilterMode;

// Alignment
interface Align {
    TopLeft: Vector2;
    Top: Vector2;
    TopRight: Vector2;
    Left: Vector2;
    Center: Vector2;
    Right: Vector2;
    BottomLeft: Vector2;
    Bottom: Vector2;
    BottomRight: Vector2;
}
declare var Align: Align;

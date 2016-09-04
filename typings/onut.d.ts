interface Color {
    r: number;
    g: number;
    b: number;
    a: number;
}
declare function Color(r: number, g: number, b: number, a: number): Color;
declare function ColorHexRGB(hex: number): Color;
declare function ColorHexRGBA(hex: number): Color;

interface Rect {
    x: number;
    y: number;
    w: number;
    h: number;
}
declare function Rect(x: number, y: number, w: number, h: number): Rect;

interface Texture {
    w: number;
    h: number;
}
declare function Texture(name: string): Texture;

interface Renderer {
    clear(color: Color);
}
declare var Renderer: Renderer;

interface SpriteBatch {
    begin();
    end();
    drawRect(texture: Texture, rect: Rect, color: Color);
    drawinclinedRect(texture: Texture, rect: Rect, incliinedRatio: number, color: Color);
    drawRectWithColors(texture: Texture, rect: Rect, topLeft: Color, bottomLeft: Color, bottomRight: Color, topRight: Color);
}
declare var SpriteBatch: SpriteBatch;

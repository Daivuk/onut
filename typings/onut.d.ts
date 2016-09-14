// Maths
declare class Vector2 {
    static distance(v1: Vector2, v2: Vector2): number;
    static distanceSquared(v1: Vector2, v2: Vector2): number;
    static min(v1: Vector2, v2: Vector2): Vector2;
    static max(v1: Vector2, v2: Vector2): Vector2;
    static lerp(from: Vector2, to: Vector2, t: number): Vector2;
    static smoothStep(from: Vector2, to: Vector2, t: number): Vector2;
    static barycentric(v1: Vector2, v2: Vector2, v3: Vector2, f: number, g: number): Vector2;
    static catmullRom(v1: Vector2, v2: Vector2, v3: Vector2, v4: Vector2, t: number): Vector2;
    static hermite(v1: Vector2, t1: Vector2, v2: Vector2, t2: Vector2, t: number): Vector2;
    static bezier(p1: Vector2, p2: Vector2, p3: Vector2, p4: Vector2, t: number): Vector2;
    static reflect(ivec: Vector2, nvec: Vector2): Vector2;
    static refract(ivec: Vector2, nvec: Vector2, refractionIndex: number): Vector2;

    static ZERO: Vector2;
    static ONE: Vector2;
    static UNIT_X: Vector2;
    static UNIT_Y: Vector2;

    static TOP_LEFT: Vector2;
    static TOP: Vector2;
    static TOP_RIGHT: Vector2;
    static LEFT: Vector2;
    static CENTER: Vector2;
    static RIGHT: Vector2;
    static BOTTOM_LEFT: Vector2;
    static BOTTOM: Vector2;
    static BOTTOM_RIGHT: Vector2;

    /** (0, 0) */
    constructor();
    /** (s, s) */
    constructor(s: number);
    /** (x, y) */
    constructor(x: number, y: number);
    constructor(other: Vector2);

    isEqual(other: Vector2): boolean;
    add(other: Vector2): Vector2;
    sub(other: Vector2): Vector2;
    mul(other: Vector2): Vector2;
    div(other: Vector2): Vector2;
    length(): number;
    lengthSquared(): number;
    dot(other: Vector2): number;
    cross(other: Vector2): Vector2;
    clamp(min: Vector2, max: Vector2): Vector2;
    normalize(): Vector2;

    x: number;
    y: number;
}

declare class Vector3 {
    static distance(v1: Vector3, v2: Vector3): number;
    static distanceSquared(v1: Vector3, v2: Vector3): number;
    static min(v1: Vector3, v2: Vector3): Vector3;
    static max(v1: Vector3, v2: Vector3): Vector3;
    static lerp(from: Vector3, to: Vector3, t: number): Vector3;
    static smoothStep(from: Vector3, to: Vector3, t: number): Vector3;
    static barycentric(v1: Vector3, v2: Vector3, v3: Vector3, f: number, g: number): Vector3;
    static catmullRom(v1: Vector3, v2: Vector3, v3: Vector3, v4: Vector3, t: number): Vector3;
    static hermite(v1: Vector3, t1: Vector3, v2: Vector3, t2: Vector3, t: number): Vector3;
    static bezier(p1: Vector3, p2: Vector3, p3: Vector3, p4: Vector3, t: number): Vector3;
    static reflect(ivec: Vector3, nvec: Vector3): Vector3;
    static refract(ivec: Vector3, nvec: Vector3, refractionIndex: number): Vector3;

    static ZERO: Vector3;
    static ONE: Vector3;
    static UNIT_X: Vector3;
    static UNIT_Y: Vector3;
    static UNIT_Z: Vector3;

    /** (0, 0, 0) */
    constructor();
    /** (s, s, s) */
    constructor(s: number);
    /** (x, y, 0) */
    constructor(x: number, y: number);
    /** (x, y, z) */
    constructor(x: number, y: number, z: number);
    /** (other.x, other.y, 0) */
    constructor(other: Vector2);
    /** (other.x, other.y, z) */
    constructor(other: Vector2, z: number);
    constructor(other: Vector3);

    isEqual(other: Vector3): boolean;
    add(other: Vector3): Vector3;
    sub(other: Vector3): Vector3;
    mul(other: Vector3): Vector3;
    div(other: Vector3): Vector3;
    length(): number;
    lengthSquared(): number;
    dot(other: Vector3): number;
    cross(other: Vector3): Vector3;
    clamp(min: Vector3, max: Vector3): Vector3;
    normalize(): Vector3;

    x: number;
    y: number;
    z: number;
}

declare class Vector4 {
    static distance(v1: Vector4, v2: Vector4): number;
    static distanceSquared(v1: Vector4, v2: Vector4): number;
    static min(v1: Vector4, v2: Vector4): Vector4;
    static max(v1: Vector4, v2: Vector4): Vector4;
    static lerp(from: Vector4, to: Vector4, t: number): Vector4;
    static smoothStep(from: Vector4, to: Vector4, t: number): Vector4;
    static barycentric(v1: Vector4, v2: Vector4, v3: Vector4, f: number, g: number): Vector4;
    static catmullRom(v1: Vector4, v2: Vector4, v3: Vector4, v4: Vector4, t: number): Vector4;
    static hermite(v1: Vector4, t1: Vector4, v2: Vector4, t2: Vector4, t: number): Vector4;
    static bezier(p1: Vector4, p2: Vector4, p3: Vector4, p4: Vector4, t: number): Vector4;
    static reflect(ivec: Vector4, nvec: Vector4): Vector4;
    static refract(ivec: Vector4, nvec: Vector4, refractionIndex: number): Vector4;

    static ZERO: Vector4;
    static ONE: Vector4;
    static UNIT_X: Vector4;
    static UNIT_Y: Vector4;
    static UNIT_Z: Vector4;
    static UNIT_W: Vector4;

    /** (0, 0, 0, 0) */
    constructor();
    /** (s, s, s, s) */
    constructor(s: number);
    /** (x, y, 0, 0) */
    constructor(x: number, y: number);
    /** (x, y, z, 0) */
    constructor(x: number, y: number, z: number);
    /** (x, y, z, w) */
    constructor(x: number, y: number, z: number, w: number);
    /** (other.x, other.y, 0, 0) */
    constructor(other: Vector2);
    /** (other.x, other.y, z, 0) */
    constructor(other: Vector2, z: number);
    /** (v1.x, v1.y, v2.x, v2.y) */
    constructor(v1: Vector2, v2: Vector2);
    /** (other.x, other.y, other.z, 0) */
    constructor(other: Vector3);
    /** (other.x, other.y, other.z, w) */
    constructor(other: Vector3, w: number);
    constructor(other: Vector4);

    isEqual(other: Vector4): boolean;
    add(other: Vector4): Vector4;
    sub(other: Vector4): Vector4;
    mul(other: Vector4): Vector4;
    div(other: Vector4): Vector4;
    length(): number;
    lengthSquared(): number;
    dot(other: Vector4): number;
    cross(other: Vector4): Vector4;
    clamp(min: Vector4, max: Vector4): Vector4;
    normalize(): Vector4;

    x: number;
    y: number;
    z: number;
    w: number;
}

declare class Rect {
    /** (0, 0, 0, 0) */
    constructor();
    /** (x, y, w, h) */
    constructor(x: number, y: number, w: number, h: number);
    /** (position.x, position.y, size.x, size.y) */
    constructor(position: Vector2, size: Vector2);
    constructor(other: Rect);

    isEqual(other: Rect): boolean;
    contains(p: Vector2): boolean;
    grow(by: number): Rect;
    distance(v1: Vector2): number;

    x: number;
    y: number;
    w: number;
    h: number;
}

declare class Color {
    static modulate(c1: Color, c2: Color): Color;
    static lerp(from: Color, to: Color, t: number): Color;
    static fromHexRGB(hex: number): Color;
    static fromHexRGBA(hex: number): Color;

    static BLACK: Color;
    static WHITE: Color;
    static TRANSPARENT: Color;
    static TRANSPARENT_WHITE: Color;

    /** (0, 0, 0, 0) */
    constructor();
    /** (s, s, s, s) */
    constructor(s: number);
    /** (r, g, b, 1) */
    constructor(r: number, g: number, b: number);
    /** (r, g, b, a) */
    constructor(r: number, g: number, b: number, a: number);
    constructor(other: Color);

    isEqual(other: Color): boolean;
    add(other: Color): Vector4;
    sub(other: Color): Vector4;
    mul(other: Color): Vector4;
    div(other: Color): Vector4;
    toVector3(): Vector3;
    toVector4(): Vector4;
    negate(): Color;
    saturate(): Color;
    premultiply(): Color;
    adjustSaturation(): Color;
    adjustContrast(): Color;

    r: number;
    g: number;
    b: number;
    a: number;
}

declare class Matrix {
    static lerp(from: Matrix, to: Matrix, t: number): Matrix;
    static createBillboard(object: Vector3, cameraPosition: Vector3, cameraUp: Vector3): Matrix;
    static createConstrainedBillboard(object: Vector3, cameraPosition: Vector3, rotateAxis: Vector3): Matrix;
    static createTranslation(position: Vector3): Matrix;
    static createScale(scales: Vector3): Matrix;
    static createRotationX(degrees: number): Matrix;
    static createRotationY(degrees: number): Matrix;
    static createRotationZ(degrees: number): Matrix;
    static createFromAxisAngle(axis: Vector3, degrees: number): Matrix;
    static createPerspectiveFieldOfView(fov: number, aspectRatio: number, nearPlane: number, farPlane: number): Matrix;
    static createPerspective(width: number, height: number, nearPlane: number, farPlane: number): Matrix;
    static createPerspectiveOffCenter(left: number, right: number, bottom: number, top: number, nearPlane: number, farPlane: number): Matrix;
    static createOrthographic(width: number, height: number, zNearPlane: number, zFarPlane: number): Matrix;
    static createOrthographicOffCenter(left: number, right: number, bottom: number, top: number, zNearPlane: number, zFarPlane: number): Matrix;
    static createLookAt(position: Vector3, target: Vector3, up: Vector3): Matrix;
    static createWorld(position: Vector3, forward: Vector3, up: Vector3): Matrix;
    static createFromYawPitchRoll(yaw: number, pitch: number, roll: number): Matrix;

    static IDENTITY: Matrix;

    /** Identity */
    constructor();
    constructor(right: Vector3, front: Vector3, up: Vector3);
    constructor(row0: Vector4, row1: Vector4, row2: Vector4, row3: Vector4);
    constructor(
        _11: number, _12: number, _13: number, _14: number,
        _21: number, _22: number, _23: number, _24: number,
        _31: number, _32: number, _33: number, _34: number,
        _41: number, _42: number, _43: number, _44: number);

    isEqual(other: Matrix): boolean;
    add(other: Color): Matrix;
    sub(other: Color): Matrix;
    mul(other: Color): Matrix;
    mul(s: number): Matrix;
    div(other: Color): Matrix;
    div(s: number): Matrix;
    translation(): Vector3;
    right(): Vector3;
    axisX(): Vector3;
    left(): Vector3;
    front(): Vector3;
    axisY(): Vector3;
    back(): Vector3;
    up(): Vector3;
    axisZ(): Vector3;
    down(): Vector3;
    transpose(): Matrix;
    invert(): Matrix;
    determinant(): number;
}

// Texture
declare class Texture {
    static createFromFile(filename: string): Texture;
    static createDynamic(size: Vector2): Texture;
    static createFromData(data: ArrayBuffer, size: Vector2): Texture;
    static createRenderTarget(size: Vector2): Texture;
    static createScreenRenderTarget(): Texture;

    getSize(): Vector2;
    isRenderTarget(): boolean;
    isDynamic(): boolean;
    clearRenderTarget();
    blur(amount: number);
    sepia(tone: Vector3, saturation: number, sepiaAmount: number);
    crt();
    cartoon(tone: Vector3);
    vignette(amount: number);
    resizeTarget(size: Vector2);
    setData(data: ArrayBuffer);
}
/** Same as Texture.createFromFile */
declare function getTexture(filename: string): Texture;

// Font
declare class Font {
    static createFromFile(filename: string): Font;

    measure(text: string): Vector2;
}
/** Same as Font.createFromFile */
declare function getFont(filename: string): Font;

// Shader
declare class Shader {
    static createFromFile(filename: string): Shader;

    setNumber(name: string, value: number);
    setVector2(name: string, value: Vector2);
    setVector3(name: string, value: Vector3);
    setVector4(name: string, value: Vector4);
    setMatrix(name: string, value: Matrix);
}
/** Same as Shader.createFromFile */
declare function getShader(filename: string): Shader;

// Music
declare class Music {
    static createFromFile(filename: string): Music;

    setVolume(volume: number);
    play();
    stop();
    pause();
    resume();
    isPlayer(): boolean;
}
/** Same as Music.createFromFile */
declare function getMusic(filename: string): Music;

// Sound
declare class Sound {
    static createFromFile(filename: string): Sound;
    static createFromData(samples: ArrayBuffer, sampleCount: number, channelCount: number, sampleRate: number): Sound;

    play(volume: number, balance: number, pitch: number);
    stop();
    setMaxInstance(maxInstance: number);
    createInstance(): SoundInstance;
}
/** Same as Music.createFromFile */
declare function getSound(filename: string): Sound;
declare function playSound(filename: string, volume: number, balance: number, pitch: number);
declare function playSoundCue(filename: string, volume: number, balance: number, pitch: number);

// SoundInstance
declare class SoundInstance {
    play();
    pause();
    stop();
    isPlaying(): boolean;
    isPaused(): boolean;
    getLoop(): boolean;
    setLoop(loop: boolean);
    getVolume(): number;
    setVolume(volume: number);
    getBalance(): number;
    setBalance(balance: number);
    getPitch(): number;
    setPitch(pitch: number);
}
/** Same as sound.createInstance */
declare function createSoundInstance(filename: string): SoundInstance;

// TiledMap
declare class TiledMap {
    static createFromFile(filename: string): TiledMap;

    getSize(): Vector2;
    getTileSize(): number;
    setFilter(filterMode: FilterMode);
    getTileAt(layerIndex: number, x: number, y: number): number;
    getTileAt(layerName: string, x: number, y: number): number;
    setTileAt(layerIndex: number, x: number, y: number, tile: number);
    setTileAt(layerName: string, x: number, y: number, tile: number);

    render();
    render(rect: Rect);

    renderLayer(layerIndex: number);
    renderLayer(layerIndex: number, rect: Rect);
    renderLayer(layerName: string);
    renderLayer(layerName: string, rect: Rect);
}
/** Same as TiledMap.createFromFile */
declare function getTiledMap(filename: string): TiledMap;

// Sprite Anim
declare class SpriteAnim {
    static createFromFile(filename: string): SpriteAnim;

    createInstance(): SpriteAnimInstance;
}
/** Same as TiledMap.createFromFile */
declare function getTiledMap(filename: string): SpriteAnim;

// SpriteAnimInstance
declare class SpriteAnimInstance {
    play(animName: String);
    play(animName: string, fps: number);
    playBackward(animName: String);
    playBackward(animName: String, fps: number);
    queue(animName: String);
    stop();
    stopAndReset();
    isPlaying(): boolean;
    getFrameId(): number;
    getSpriteAnim(): SpriteAnim;
    getCurrentAnim(): string;
    setFPS(fps: number);
    getTexture(): Texture;
    getUVs(): Vector4;
    getOrigin(): Vector2;
}
/** Same as spriteAnim.createInstance */
declare function createSpriteAnimInstance(filename: string): SpriteAnimInstance;
/** Same as spriteAnim.createInstance then calling play on the returned instance */
declare function playSpriteAnim(filename: string, animName: string): SpriteAnimInstance;

// Renderer
declare namespace Renderer {
    function clear(color: Color);
    function getResolution(): Vector2;

    function setRenderTarget(renderTarget: Texture);
    function pushRenderTarget(renderTarget: Texture);
    function popRenderTarget();

    function setTexture(texture: Texture, index: number);
    function pushTexture(texture: Texture, index: number);
    function popTexture(index: number);

    function setVertexShader(shader: Shader, index: number);
    function pushVertexShader(shader: Shader, index: number);
    function popVertexShader(index: number);

    function setPixelShader(shader: Shader, index: number);
    function pushPixelShader(shader: Shader, index: number);
    function popPixelShader(index: number);
}

// Spritebatch
declare namespace SpriteBatch {
    function begin();
    function begin(transform: Matrix);
    function end();

    function drawRect(texture: Texture, rect: Rect, color: Color);
    function drawinclinedRect(texture: Texture, rect: Rect, incliinedRatio: number, color: Color);
    function drawRectWithColors(texture: Texture, rect: Rect, topLeft: Color, bottomLeft: Color, bottomRight: Color, topRight: Color);
    function drawRectWithUVs(texture: Texture, rect: Rect, uvs: Vector4, color: Color);
    function draw4Corner(texture: Texture, rect: Rect, color: Color);
    function drawRectScaled9(texture: Texture, rect: Rect, padding: Vector4, color: Color);
    function drawRectScaled9RepeatCenters(texture: Texture, rect: Rect, padding: Vector4, color: Color);
    function drawSprite(texture: Texture, position: Vector2, color: Color, rotation: number, scale: number, origin: Vector2);
    function drawSpriteAnim(spriteAnim: SpriteAnimInstance, position: Vector2, color: Color, rotation: number, scale: number);
    function drawTransformedSprite(texture: Texture, transform: Matrix, color: Color, scale: Vector2, origin: Vector2);
    function drawTransformedSpriteAnim(spriteAnim: SpriteAnimInstance, transform: Matrix, color: Color, scale: Vector2);
    function drawBeam(texture: Texture, from: Vector2, to: Vector2, size: Number, color: Color, uOffset: number, uScale: number);
    function drawCross(position: Vector2, size: number, color: Color);

    function drawText(font: Font, text: string, position: Vector2, align: Vector2, color: Color);
    function drawOutlinedText(font: Font, text: string, position: Vector2, align: Vector2, color: Color, outlineColor: Color, outlineSize: number);
    function drawPrettyOutlinedText(font: Font, text: string, position: Vector2, align: Vector2, color: Color, outlineColor: Color, outlineSize: number);

    function setBlend(blendMode: Number);
    function setFilter(filterMode: Number);
}

// PrimitiveBatch
declare namespace PrimitiveBatch {
    function begin(primitiveMode: PrimitiveMode);
    function begin(primitiveMode: PrimitiveMode, texture: Texture);
    function begin(primitiveMode: PrimitiveMode, texture: Texture, transform: Matrix);
    function end();
    function draw(position: Vector2, color: Color, texCoord: Vector2);
}

// Input
declare namespace Input {
    function getMousePos(): Vector2;
    function isDown(key: Key): boolean;
    function isUp(key: Key): boolean;
    function isJustDown(key: Key): boolean;
    function isJustUp(key: Key): boolean;
    function getValue(key: Key): number;
}

// GamePad
declare namespace GamePad {
    function isConnected(index: number): boolean;
    function getLeftThumb(index: number): Vector2;
    function getRightThumb(index: number): Vector2;
    function isDown(index: number, button: Button): boolean;
    function isUp(index: number, button: Button): boolean;
    function isJustDown(index: number, button: Button): boolean;
    function isJustUp(index: number, button: Button): boolean;
}

// HTTP
declare namespace Http {
    function post(url: string, arguments: any, onErrorFn: (code: number, message: string) => void): string;
    function postAsync(url: string, arguments: any, onSuccessFn: (body: string) => void, onErrorFn: (code: number, message: string) => void);
    function getString(url: string, arguments: any, onErrorFn: (code: number, message: string) => void): string;
    function getStringAsync(url: string, arguments: any, onSuccessFn: (body: string) => void, onErrorFn: (code: number, message: string) => void);
    function get(url: string, arguments: any, onErrorFn: (code: number, message: string) => void): ArrayBuffer;
    function getAsync(url: string, arguments: any, onSuccessFn: (body: ArrayBuffer) => void, onErrorFn: (code: number, message: string) => void);
    function getTexture(url: string, arguments: any, onErrorFn: (code: number, message: string) => void): Texture;
    function getTextureAsync(url: string, arguments: any, onSuccessFn: (texture: Texture) => void, onErrorFn: (code: number, message: string) => void);
}

// Crypto
declare namespace Cryptography {
    function hash(text: string, seed: number): number;
    function sha1(text: string): string;
    function validateEmail(email: string): boolean;
    function encodeBase64(text: string): string;
    function encodeBase64(buffer: ArrayBuffer): string;
    function decodeBase64(base64: string): ArrayBuffer;
}

// Blend mode
declare enum BlendMode {
    OPAQUE,
    ALPHA,
    ADD,
    PREMULTIPLIED,
    MULTIPLY,
    FORCE_WRITE
}

// filtering
declare enum FilterMode {
    NEAREST,
    LINEAR
}

// Primitive mode
declare enum PrimitiveMode {
    POINT_LIST,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE_LIST,
    TRIANGLE_STRIP
}

// Input keys/mouse
declare enum Key {
    ESCAPE,
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,
    MINUS, EQUALS, BACKSPACE, TAB,
    Q, W, E, R, T, Y, U, I, O, P,
    LEFT_BRACKET, RIGHT_BRACKET, ENTER, LEFT_CONTROL,
    A, S, D, F, G, H, J, K, L,
    SEMI_COLON, APOSTROPHE, GRAVE, LEFT_SHIFT, BACKSLASH,
    Z, X, C, V, B, N, M,
    COMMA, PERIOD, SLASH, RIGHT_SHIFT, MULTIPLY,
    LEFT_ALT, SPACE_BAR, CAPS_LOCK,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
    NUM_LOCK, SCROLL_LOCK,
    NUM_PAD_7, NUM_PAD_8, NUM_PAD_9, NUM_PAD_MINUS,
    NUM_PAD_4, NUM_PAD_5, NUM_PAD_6, NUM_PAD_ADD,
    NUM_PAD_1, NUM_PAD_2, NUM_PAD_3,
    NUM_PAD_0, NUM_PAD_PERIOD,
    OEM102, F11, F12, F13, F14, F15,
    KANA, ABNT_C1, CONVERT, NO_CONVERT, YEN, ABNT_C2,
    NUM_PAD_EQUALS, PREVIOUS_TRACK, AT, COLON, UNDERLINE,
    KANJI, STOP, AX, UNLABELED, NEXT_TRACK, NUM_PAD_ENTER,
    RIGHT_CONTROL, MUTE, CALCULATOR, PLAY_PAUSE, MEDIA_STOP,
    VOLUME_DOWN, VOLUME_UP, WEB_HOME,
    NUM_PAD_COMMA, NUM_PAD_DIVICE, SYSRQ, RIGHT_ALT, ALT_CAR,
    PAUSE, HOME, UP, PAGE_UP, LEFT, RIGHT, END, DOWN, PAGE_DOWN,
    INSERT, DELETE, LEFT_WINDOWS, RIGHT_WINDOWS, APP_MENU,
    POWER, SLEEP, WAKE,
    WEB_SEARCH, WEB_FAVORITES, WEB_REFRESH, WEB_STOP,
    WEB_FORWARD, WEB_BACK,
    MY_COMPUTER, MAIL_L, MEDIA_SELECT,
    CIRCOMFLEX,
    MOUSE_1, MOUSE_2, MOUSE_3, MOUSE_4, MOUSE_X, MOUSE_Y, MOUSE_Z
}

declare enum Button {
    A, B, X, Y,
    DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAR_RIGHT,
    LEFT_TRIGGER, LEFT_BUMPER,
    RIGHT_TRIGGER, RIGHT_BUMPER,
    LEFT_THUMBSTICK,
    RIGHT_THUMBSTICK,
    START, BACK,
    LEFT_THUMBSTICK_LEFT,
    LEFT_THUMBSTICK_RIGHT,
    LEFT_THUMBSTICK_UP,
    LEFT_THUMBSTICK_DOWN,
    RIGHT_THUMBSTICK_LEFT,
    RIGHT_THUMBSTICK_RIGHT,
    RIGHT_THUMBSTICK_UP,
    RIGHT_THUMBSTICK_DOWN
}





// TO REMOVE

// Entity
interface Entity {
    getComponent(type): Component;
    addComponent(type): Component;
}

// Components
interface Component {
    getEntity(): Entity;
}

interface TiledMapComponent extends Component {
    setTiledMap(tiledMap: TiledMap);
    getTiledMap(): TiledMap;
    getPassable(mapPos: Vector2): boolean;
    setPassable(mapPos: Vector2, passable: boolean);
}

// Entity factory
interface EntityFactory {
    create(position: Vector3): Entity;
    createCollider2D(size: Vector2, position: Vector2): Entity;
    createSound(filename: string, position: Vector2): Entity;
    createSpriteAnim(filename: string, position: Vector2, defaultAnim: string): Entity;
    createSprite(filename: string, position: Vector2): Entity;
    createText(font: Font, text: string, position: Vector2): Entity;
    createTiledMap(filename: string): Entity;
}
declare var EntityFactory: EntityFactory;

// Scene
interface Scene {
    load(filename: string);
    findEntity(name: string): Entity;
}
declare var Scene: Scene;

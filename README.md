![Logo](logo_small.png)
# Oak Nut engine.
Commonly called "onut". Game engine focused on rapid development. Aimed at Game Jams.

## Quick javascript samples

### Load image and draw a sprite
```javascript
var texture = getTexture("image.png");

function render() {
    // Begin a batch
    SpriteBatch.begin();
    
    // Draw a sprite at screen coordinates x=200, y=100 from top left corner.
    SpriteBatch.drawSprite(texture, new Vector2(200, 100));
    
    // End and flush the batch
    SpriteBatch.end();
}
```

### Animate
```javascript
function update(dt) {
    spriteAngle += dt * 45; // Will rotate 45 degree per second
}

function render() {
    ...
    
    // Draw a sprite at with an animated angle
    SpriteBatch.drawSprite(texture, new Vector2(200, 100), Color.WHITE, spriteAngle);
    
    ...
}
```

See `onut/samplesJS/` folders for more detailed samples.

## Overview

### List of jam projects using it:

* Ottawa Game Jam 2015 - [Cannon Fodder Commander ](https://www.youtube.com/watch?v=Jac9r32uIv0)
* Global Game Jam 2016 - [Fire Whisperers ](https://www.youtube.com/watch?v=SWgFVMk5f2Q)
* Ottawa Game Jam 2016 - [Spy Satellite Showdown ](https://www.youtube.com/watch?v=NHyGlVm2ICA)
* Linux Game Jam 2017 - [Retro Game War ](https://daivuk.itch.io/retro-island-war)

### See subsection on how to compile and run on each platforms

* [Windows](#windows)
* [OS X](#os-x)
* [Linux](#linux)
* [Raspberry PI](#raspberry-pi)

## Windows
Use cmake to compile the engine, JSStandAlone and samples.
```bash
cmake .
```

Open onut.sln solution into visual studio. Build/Run projects.

To compile in release mode, set `CMAKE_BUILD_TYPE` to `Release`:
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```

### JavaScript Game
#### Compile the executable
1. Clone onut somewhere on your PC.
2. Generate using cmake.
3. Build the JSStandAlone project in release.
4. Copy the executable to your game's folder.

#### Setting up your JavaScript project
Recommended to use Visual Studio Code.
1. Create a folder for your game somehere on your PC
2. Copy `[onut path]/jsconfig.json`, `[onut path]/typings/onut.d.ts` to YourGame path.
3. Create a `settings.txt` file. Refer to samples to see what can be put in there
4. Create assets and javascript files
5. `main.js` will always be the last JavaScript file executed, use it to initialize stuff.

#### main.js
```javascript
function update(dt) {
    // Update your game here
}

function render() {
}
```

### C++ game

#### Recommended folder structure
Before you start. Please take note of the following folder structure. It is highly recommended for minimal setup time.
The default asset search paths are configured for this structure. _But it easy to add more using `oContentManager->addSearchPath`._

* `YourGame\`
  * `assets\` Put game assets in here
    * ...
  * `src\` Your game source code (.h and .cpp)
  * .. projects files or your CMakeLists.txt

#### main.cpp
Make sure to define those 5 functions. Otherwise you will get unresolved errors.
```cpp
void initSettings()
{
}

void init()
{
}

void update()
{
}

void render()
{
}

void postRender()
{
}
```
Look at samples to see what can be done here.

## OS X
Use cmake to compile the engine, JSStandAlone and samples.
```bash
cmake .
make
```

To compile only JSStandAlone:
```bash
cmake .
make JSStandAlone
```

To compile any of the C++ samples only:
```bash
cmake .
make DungeonSample
```

To compile in release mode, set `CMAKE_BUILD_TYPE` to `Release`:
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```

Some third parties might need to be installed. SDL2 and CURL. Follow online instructions on how to install them using brew. You will also need to install brew if not installed. SDL2 might give you a little bit trouble creating symlink. Google around there are instructions how to get those working.

To run a javascript sample on OSX, simply type this in command line:
```bash
./JSStandAlone/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If no argument are passed, current directory will be used.

## Linux
Use cmake to compile the engine, JSStandAlone and samples.
```bash
cmake .
make
```

To compile only JSStandAlone:
```bash
cmake .
make JSStandAlone
```

To compile any of the C++ samples only:
```bash
cmake .
make DungeonSample
```

To compile in release mode, set `CMAKE_BUILD_TYPE` to `Release`:
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```

Some third parties might need to be installed. OpenGL, Glew, SDL2 and CURL. Follow online instructions on how to install them using apt-get.

To run a javascript sample on Linux, simply type this in command line:
```bash
./JSStandAlone/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If no argument are passed, current directory will be used.

## Raspberry PI
Use cmake to compile the engine, JSStandAlone and samples.
Make sure to define RPI to 1. It uses different frameworks than standard Linux.
```bash
cmake -DRPI=1 .
make
```

To compile only JSStandAlone:
```bash
cmake -DRPI=1 .
make JSStandAlone
```

To compile any of the C++ samples only:
```bash
cmake -DRPI=1 .
make DungeonSample
```

To compile in release mode, set `CMAKE_BUILD_TYPE` to `Release`:
```bash
cmake -DRPI=1 -DCMAKE_BUILD_TYPE=Release .
```

Some third parties might need to be installed. CURL. Follow online instructions on how to install them using apt-get.

To run a javascript sample on Raspberry PI, simply type this in command line:
```bash
./JSStandAlone/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If no argument are passed, current directory will be used.

### Supported features
Here is a table of features with support per platform and availability to the javascript bindings. For a full list of features, check the samples/ and samplesJS/.

| Feature | Windows | OS X | Linux | Raspberry PI | Javascript Bindings |
|:-------:|:-------:|:----:|:-----:|:------------:|:-:|
| Multiplayer | :x: | :x: | :x: | :x: | :x:
| System Info | :x: | :x: | :x: | :x: | partial
| Cloud | _disabled_ | :x: | :x: | :x: | :x:
| File utils | :white_check_mark: | :x: | :x: | :x: | :x:
| File Dialogs | :white_check_mark: | :x: | :x: | :x: | :x:
| Music | :white_check_mark: | :x: | :x: | :x: | :white_check_mark:
| Video Player | :white_check_mark: | :x: | :x: | :x: | :white_check_mark:
| Effects | :white_check_mark: | :x: | :x: | :x: | :white_check_mark:
| Shaders | :white_check_mark: | :x: | :x: | :x: | :white_check_mark:
| Game Pads | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x: | :white_check_mark:
| Native Cursor | :white_check_mark: | :x: | :x: | :x: | :white_check_mark:
| Mouse | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x: | :white_check_mark:
| Audio Stream | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| Curves | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| Entity/Components | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| Box2D | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| Path Finding | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| XML Parser | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| Thread Pool | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| CSV | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| Images | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| TCP Sockets | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :x:
| UI | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | partial
| Settings | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | partial
| Random | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | partial
| CMake | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | n/a
| Log | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Maths | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Anims | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Textures | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| File IO | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Bitmap Fonts | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Sounds | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| TiledMaps | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Sprite Anims | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Particles | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Vertex Buffers | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Renderer | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| SpriteBatch | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| PrimitiveBatch | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Keyboard | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Http | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Basic Hash | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Sha1 | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:
| Base64 | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark:

## Mentions
_Logo design by Pixilabs Software Inc._

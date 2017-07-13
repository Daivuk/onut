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
* Ottawa Game Jam 2017 - [Sheep Dog Heroes ]()

### See subsection on how to compile and run on each platforms

* [Windows](#windows)
* [OS X](#os-x)
* [Linux](#linux)

### Release history

* Onut 1.0 - **Bark** - Feb 6, 2016
* Onut 2.0 - **Root** (The Cleanup release) - Apr 3, 2016
* Onut 3.0 - **Leaf** (The JavaScript release) - _tbd_
* Onut 4.0 - **Colony** (The Multiplayer release) - _tbd_

## Windows
Use cmake to compile the engine, JSStandAlone and samples.
```bash
cmake -DONUT_BUILD_SAMPLES=ON -DONUT_BUILD_STANDALONE=ON .
```

Open onut.sln solution into visual studio. Build/Run projects.

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
3. Create a `settings.txt` file. Refer to samples to see what can be put in there.
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
cmake -DONUT_BUILD_SAMPLES=ON -DONUT_BUILD_STANDALONE=ON .
make
```

To compile only JSStandAlone:
```bash
make JSStandAlone
```

To compile any of the C++ samples only:
```bash
make DungeonSample
```

To build in release, set the `CMAKE_BUILD_TYPE` flag to `Release` like so:
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```

Some third parties might need to be installed. SDL2 and CURL. Follow online instructions on how to install them using brew. You will also need to install brew if not installed. SDL2 might give you a little bit trouble creating symlink. Google around there are instructions how to get those working.

To run a javascript sample on OSX, simply type this in command line:
```bash
./JSStandAlone/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If you screw this up, onut will load **all** javascript files on your PC. Hilarity ensue.

## Linux
Use cmake to compile the engine, JSStandAlone and samples.
```bash
cmake -DONUT_BUILD_SAMPLES=ON -DONUT_BUILD_STANDALONE=ON .
make
```

To compile only JSStandAlone:
```bash
make JSStandAlone
```

To compile any of the C++ samples only:
```bash
make DungeonSample
```

To build in release, set the `CMAKE_BUILD_TYPE` flag to `Release` like so:
```bash
cmake -DCMAKE_BUILD_TYPE=Release .
```

Some third parties might need to be installed. OpenGL, Glew, SDL2 and CURL. Follow online instructions on how to install them using apt-get.

To run a javascript sample on Linux, simply type this in command line:
```bash
./JSStandAlone/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If you screw this up, onut will load **all** javascript files on your PC. Hilarity ensue.

### Supported features
Here is a table of features with support per platform and availability to the javascript bindings. For a full list of features, check the samples/ and samplesJS/.

| Feature           | Windows    | OS X | Linux | Javascript Bindings |
|:-----------------:|:----------:|:----:|:-----:|:-------------------:|
| Anims             | ✓          | ✓    | ✓     | ✓                   |
| Audio Stream      | ✓          | ✓    | ✓     | _unplanned_         |
| Base64            | ✓          | ✓    | ✓     | ✓                   |
| Bitmap Fonts      | ✓          | ✓    | ✓     | ✓                   |
| Box2D             | ✓          | ✓    | ✓     |                     |
| Cloud             | _disabled_ |      |       |                     |
| CMake             | ✓          | ✓    | ✓     | n/a                 |
| CSV               | ✓          | ✓    | ✓     |                     |
| Effects           | ✓          | ✓    | ✓     | ✓                   |
| Entity/Components | ✓          | ✓    | ✓     |                     |
| File Dialogs      | ✓          |      |       | _unplanned_         |
| File IO           | ✓          | ✓    | ✓     | ✓                   |
| File utils        | ✓          |      |       | _unplanned_         |
| Game Pads         | ✓          | ✓    | ✓     | ✓                   |
| Hash              | ✓          | ✓    | ✓     | ✓                   |
| Http              | ✓          | ✓    | ✓     | ✓                   |
| Images            | ✓          | ✓    | ✓     | _unplanned_         |
| Keyboard          | ✓          | ✓    | ✓     | ✓                   |
| Log               | ✓          | ✓    | ✓     | ✓                   |
| Maths             | ✓          | ✓    | ✓     | ✓                   |
| Mouse             | ✓          | ✓    | ✓     | ✓                   |
| Multiplayer       |            |      |       |                     |
| Music             | ✓          | ✓    | ✓     | ✓                   |
| Native Cursor     | ✓          | ✓    | ✓     | ✓                   |
| Path Finding      | ✓          | ✓    | ✓     | ✓                   |
| Particles         | ✓          | ✓    | ✓     | ✓                   |
| PrimitiveBatch    | ✓          | ✓    | ✓     | ✓                   |
| Random            | ✓          | ✓    | ✓     | ✓                   |
| Renderer          | ✓          | ✓    | ✓     | ✓                   |
| Settings          | ✓          | ✓    | ✓     | partial             |
| Sha1              | ✓          | ✓    | ✓     | ✓                   |
| Shaders           | ✓          | ✓    | ✓     | ✓                   |
| Sounds            | ✓          | ✓    | ✓     | ✓                   |
| Sprite Anims      | ✓          | ✓    | ✓     | ✓                   |
| SpriteBatch       | ✓          | ✓    | ✓     | ✓                   |
| System Info       | n/a        | n/a  | n/a   | ✓                   |
| TCP Sockets       | ✓          | ✓    | ✓     |                     |
| Textures          | ✓          | ✓    | ✓     | ✓                   |
| TiledMaps         | ✓          | ✓    | ✓     | ✓                   |
| Thread Pool       | ✓          | ✓    | ✓     | n/a                 |
| UI                | ✓          | ✓    | ✓     | partial             |
| Vertex Buffers    | ✓          | ✓    | ✓     | ✓                   |
| Video Player      | ✓          |      |       | ✓                   |

## Mentions
_Logo design by Pixilabs Software Inc._

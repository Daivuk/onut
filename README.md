![Oak Nut logo](logo_small.png)
# Oak Nut engine.
2D Game engine focused on rapid development. Aimed at Game Jams.

List of jam projects using it:

* Ottawa Game Jam 2015 - [Cannon Fodder Commander ](https://www.youtube.com/watch?v=Jac9r32uIv0)
* Global Game Jam 2016 - [Fire Whisperers ](https://www.youtube.com/watch?v=SWgFVMk5f2Q)
* Ottawa Game Jam 2016 - [Spy Satellite Showdown ](https://www.youtube.com/watch?v=NHyGlVm2ICA)
* Linux Game Jam 2017 - [Retro Game War ](https://daivuk.itch.io/retro-island-war)

Supported platforms:

* Windows desktop (Only tested on Windows 10)
* OS X
* Raspberry PI (Only tested on model 2B)

See subsection on how to compile and run on each platforms:

* [Windows](#windows)
* [OS X](#os-x)
* [Raspberry PI](#raspberry-pi)

## Windows
### Prerequisites
* Visual Studio 2013 for windows Desktop (Express or Community)

### JavaScript Game
#### Compile the executable
1. Clone onut somewhere on your PC.
2. Open the solution `[onut path]/JSStandAlone/JSStandAlone.sln`
3. Build in release.

#### Setting up your JavaScript project
Recommended to use Visual Studio Code.
1. Create a folder for your game somehere on your PC
2. Copy `[onut path]/jsconfig.json`, `[onut path]/typings/onut.d.ts` to YourGame path.
3. Create a `settings.txt` file. Refer to samples to see what can be put in there
4. Copy the JSStandAlone.exe to YourGame path.
5. Create assets and javascript files
6. `main.js` will always be the last JavaScript file executed, use it to initialize stuff.

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
  * `project\`
    * `win\` Windows configuration
      * `YourGame.sln` Your game solution
      * `YourGame.vcxproj` Your game project
      * `YourGame.vcxproj.filters`
  * `src\` Your game source code (.h and .cpp)

#### Setup project
Make sure to follow the recommended folder structure when doing those steps.
1. Clone onut somewhere on your PC.
2. Open Visual Studio and create a new empty win32 solution and project for your game.
2. Add onut project to your solution.
  * Right click your solution -> Add -> Existing project -> `[onut path]\project\win\onut.vcxproj`
4. Add build dependancies for your game to include `onut`.
  * Right click YourGame project -> Build Dependencies -> Project Dependencies -> Check mark `onut`
5. Add references to `onut` from your game.
  * Right click YourGame project -> Add -> References -> Add New Reference -> Check mark `onut`
6. Add include path to your game source and onut includes
  * Right click YourGame project -> Properties -> Configuration Properties -> C/C++ -> General -> Additional Include Directories. Add the following
    * `../../src`
    * `[onut path]/include`
7. Use static runtime libraries. This will make sharing your game .exe easier, and onut is built like that by default.
  * Right click YourGame project -> Properties
  * Top left, in the Configuration dropdown, Select `Debug`.
  * Configuration Properties -> C/C++ -> Code Generation -> Runtime Library. Choose `Multi-threaded Debug (/MTd)`
  * Top left, in the Configuration dropdown, Select `Release`.
  * Configuration Properties -> C/C++ -> Code Generation -> Runtime Library. Choose `Multi-threaded (/MT)`

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

All apps and samples executable will be into subfolder: `[sample_path]/bin/osx/`

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

Some third parties might need to be installed. SDL2 and CURL. Follow online instructions on how to install them using brew. You will also need to install brew if not installed. SDL2 might give you a little bit trouble creating symlink. Google around there are instructions how to get those working.

To run a javascript sample on OSX, simply type this in command line:
```bash
./JSStandAlone/bin/osx/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If no argument are passed, current directory will be used.

## Raspberry PI
Use cmake to compile the engine, JSStandAlone and samples.
Make sure to define RPI to 1. It uses different frameworks than standard Linux.
```bash
cmake -DRPI .
make
```

All apps and samples executable will be into subfolder: `[sample_path]/bin/linux/`

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

To run a javascript sample on Raspberry PI, simply type this in command line:
```bash
./JSStandAlone/bin/linux/JSStandAlone ./samplesJS/Dungeon/
```
The JSStandAlone app takes 1 argument, the path to where your assets/scripts are. If no argument are passed, current directory will be used.

## Samples
See `onut/samples/` and `onut/samplesJS/` folders to learn how to use onut.

Enjoy!

## Mentions
_Logo design by Stéphane Yilmaz_

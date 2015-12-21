#onut
Oak Nut engine.
2D Game engine focused of development rapidity. Aimed at Game Jams.
Jam projects using it:
* OJam 2015 - Cannon Fodder Commander https://www.youtube.com/watch?v=Jac9r32uIv0

###Recommended folder structure
Before you start. Please take note of the following folder structure. It is higly recommanded for minimal setup time.

The default asset search paths are configured for this structure.
You won't have to set working directory.
Default Shaders will be placed correctly into your `assets\shaders\` once compiled.

* `YourGamePath\`
  * `onut\` Clone onut there
  * `assets\` Put game assets in here
    * `fonts\` Font files and their texture pages.
    * `musics\`
    * `pfx\` Particle effects
    * `shaders\`
    * `sounds\`
    * `textures\`
  * `project\`
    * `win\` Windows configuration
      * `YourGame.sln` Your game solution
      * `YourGame.vcxproj` Your game project
      * `YourGame.vcxproj.filters`
  * `src\` Your game source code (.h and .cpp)
  * `bin\`
    * `win\` Output your executable there

###Prerequisites
* Windows 8.1 or higher
* Visual Studio 2013 for windows Desktop (Express or Community)

###Setup project
Make sure to follow the recommanded folder structure when doing those steps.

1. Create a new win32 solution and project for your game.
2. Clone onut inside your main directory. as shown in the `Recommended folder structure`
4. Add onut project to your solution.
  * Right click your solution -> Add -> Existing project -> `YourGamePath\onut\project\win\onut.vcxproj`
6. Add build dependancies for your game to include `onut`.
  * Right click YourGame project -> Build Dependencies -> Project Dependencies -> Check mark `onut`
8. Add references to `onut` from your game.
  * Right click YourGame project -> Add -> References -> Add New Reference -> Check mark `onut`
9. Add include path to onut
  * Right click YourGame project -> Properties -> Configuration Properties -> C/C++ -> General -> Additional Include Directories. Add the following:
    * `../../src`
    * `../../onut/include`

###Main
```cpp
// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include "onut.h"

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
  // Run. 
  // Define your own init(), update(), render() calls and pass them as parameters.
  onut::run(init, update, render);
}
```

#onut
Onut engine. My little engine to make quick games

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

##Windows 7 or Windows 8
###Prerequisites
* Visual Studio 2013 for windows Desktop (Express or Community)
* DirectX SDK Jun 2010. **_Must_** install it in default directory: 
  * `C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\`
* DirectX End-User Runtimes (June 2010). Your users will have to install this also. Run this during installation of your game.
* For windows 7 only: You must have Windows 7 SP1. Check in Windows update


###Setup project
Make sure to follow the recommanded folder structure when doing those steps.

1. Create a new win32 solution and project for your game
2. Clone onut inside your main directory.
3. Clone EasyGraphix inside onut. https://github.com/Daivuk/EasyGraphix
4. Add onut and EasyGraphix projects to your solution.
  * Right click your solution -> Add -> Existing project -> `YourGamePath\onut\project\win\onut.vcxproj`
  * Right click your solution -> Add -> Existing project -> `YourGamePath\onut\EasyGraphix\eg\src\dx11\egdx11.vcxproj`
5. Add build dependancies for onut game to include `egdx11`.
  * Right click onut project -> Build Dependencies -> Project Dependencies -> Check mark `egdx11`
6. Add build dependancies for your game to include `onut`.
  * Right click YourGame project -> Build Dependencies -> Project Dependencies -> Check mark `onut`
7. Add references to `egdx11` from `onut`.
  * Right click `onut` project -> Add -> References -> Add New Reference -> Check mark `egdx11`
8. Add references to `onut` from your game.
  * Right click YourGame project -> Add -> References -> Add New Reference -> Check mark `onut`
9. Add include path to onut
  * Right click YourGame project -> Properties -> Configuration Properties -> C/C++ -> General -> Additional Include Directories. Add the following:
    * `../../src`
    * `../../onut/include`
    * `../../onut/EasyGraphix/eg/include`
10. Add C++ preprocessor to YourGame `EASY_GRAPHIX`. Do for both Debug and Release
11. Make sure to enable multi core compilation to increase speed. You might have to disable minimal rebuild **No (/Gm-)**

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

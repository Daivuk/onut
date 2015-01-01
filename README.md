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
* For windows 7 only: You must have Windows 7 SP1. Check in Windows update


###Setup project
Make sure to follow the recommanded folder structure when doing those steps.

1. Create a new win32 solution and project for your game
2. Clone onut inside your main directory.
3. Add onut project to your solution.
  * Right click your solution -> Add -> Existing project -> `YourGamePath\onut\project\win\onut.vcxproj`
4. Add build dependancies for your game to include `onut`.
  * Right click YourGame project -> Build Dependencies -> Project Dependencies -> Check mark `onut`
5. Add references to `onut` from your game.
  * Right click YourGame project -> Add -> References -> Add New Reference -> Check mark `onut`
6. Add include path to onut
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

#onut
##Oak Nut engine.
2D Game engine focused on rapid development. Aimed at Game Jams.

List of jam projects using it:

* Ottawa Jam 2015 - [Cannon Fodder Commander ](https://www.youtube.com/watch?v=Jac9r32uIv0)
* Global Game Jam 2016 - [Fire Whisperers ](https://www.youtube.com/watch?v=SWgFVMk5f2Q)

##Prerequisites
* Windows 8.1 or higher
* Visual Studio 2013 for windows Desktop (Express or Community)

##Recommended folder structure
Before you start. Please take note of the following folder structure. It is highly recommended for minimal setup time.

The default asset search paths are configured for this structure. _But it easy to add more using `oContentManager->addSearchPath`._

* `YourGamePath\`
  * `onut\` Clone onut there
  * `assets\` Put game assets in here
    * ...
  * `project\`
    * `win\` Windows configuration
      * `YourGame.sln` Your game solution
      * `YourGame.vcxproj` Your game project
      * `YourGame.vcxproj.filters`
  * `src\` Your game source code (.h and .cpp)

##Setup project
Make sure to follow the recommended folder structure when doing those steps.

###Step by step
1. Create a new empty win32 solution and project for your game.
2. Clone onut inside your main directory. as shown in the `Recommended folder structure`
3. Add onut project to your solution.<br />
  * Right click your solution -> Add -> Existing project -> `YourGamePath\onut\project\win\onut.vcxproj`
4. Add build dependancies for your game to include `onut`.
  * Right click YourGame project -> Build Dependencies -> Project Dependencies -> Check mark `onut`
5. Add references to `onut` from your game.
  * Right click YourGame project -> Add -> References -> Add New Reference -> Check mark `onut`
6. Add include path to onut
  * Right click YourGame project -> Properties -> Configuration Properties -> C/C++ -> General -> Additional Include Directories. Add the following
    * `../../src`
    * `../../onut/include`
7. Compile statically.
    * Right click YourGame project -> Properties -> Configuration Properties -> C/C++ -> Code Generation -> Runtime Library
        * For Debug, set to `Multi-threaded Debug (/MTd)`
        * For Release, set to `Multi-threaded (/MT)`

##Main
```cpp
// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/onut.h>

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
  // Run. 
  // Define your own init(), update(), render() calls and pass them as parameters.
  onut::run(init, update, render);
}
```

##Samples
See the `onut/samples/Samples.sln` folder to learn how to use onut.

Enjoy!

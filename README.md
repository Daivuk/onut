![Logo](logo_small.png)
# Oak Nut engine.
Commonly called "onut". Game engine focused on rapid development. Aimed at Game Jams.

## Quick javascript sample

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

### Update loop
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

### List of projects using it:

* Global Game Jam 2020 - [Arctic Distress ](https://www.youtube.com/watch?v=LuzXd_CCp3I)

[![Arctic Distress ](https://img.youtube.com/vi/LuzXd_CCp3I/0.jpg)](https://www.youtube.com/watch?v=LuzXd_CCp3I)

* Global Game Jam 2020 - [Click to Repair ](https://globalgamejam.org/2020/games/click-repair-8)

[![Click to Repair ](https://ggj.s3.amazonaws.com/styles/game_content__wide/games/screenshots/2020/02/113468/click_to_repair_02_02_2020_16_02_37.png?itok=9vk0CrIX&timestamp=1580656246)](https://globalgamejam.org/2020/games/click-repair-8)

* Global Game Jam 2019 - [Eagle's Nest ](https://www.youtube.com/watch?v=QHOvnFUddvA)

[![Eagle's Nest ](https://img.youtube.com/vi/QHOvnFUddvA/0.jpg)](https://www.youtube.com/watch?v=QHOvnFUddvA)

* Global Game Jam 2018 - [Messenger Hacker ](https://www.youtube.com/watch?v=cYTdtrwJydo)

[![Messenger Hacker ](https://img.youtube.com/vi/cYTdtrwJydo/0.jpg)](https://www.youtube.com/watch?v=cYTdtrwJydo)

* Ottawa Game Jam 2017 - [Sheep Dog Heroes ](https://twitter.com/Daivuk/status/879082739407966209)

* Linux Game Jam 2017 - [Retro Game War ](https://daivuk.itch.io/retro-island-war)

[![Retro Game War ](https://img.itch.zone/aW1hZ2UvMTI4NDg0LzU5MTE2Ni5qcGc=/794x1000/C4EpSO.jpg)](https://daivuk.itch.io/retro-island-war)

* Ottawa Game Jam 2016 - [Spy Satellite Showdown ](https://www.youtube.com/watch?v=NHyGlVm2ICA)

[![Spy Satellite Showdown ](https://img.youtube.com/vi/NHyGlVm2ICA/0.jpg)](https://www.youtube.com/watch?v=NHyGlVm2ICA)

* Global Game Jam 2016 - [Fire Whisperers ](https://www.youtube.com/watch?v=SWgFVMk5f2Q)

[![Fire Whisperers ](https://img.youtube.com/vi/SWgFVMk5f2Q/0.jpg)](https://www.youtube.com/watch?v=SWgFVMk5f2Q)

* Ottawa Game Jam 2015 - [Cannon Fodder Commander ](https://www.youtube.com/watch?v=Jac9r32uIv0)

[![Cannon Fodder Commander ](https://img.youtube.com/vi/Jac9r32uIv0/0.jpg)](https://www.youtube.com/watch?v=Jac9r32uIv0)

### Release history

* Onut 4.0 - The 3D release - _TBD_
* Onut 3.0 - The JavaScript release - Jul 14, 2017
* Onut 2.0 - The Cleanup release - Apr 3, 2016
* Onut 1.0 - Feb 6, 2016

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
Make sure to define those 6 functions. Otherwise you will get unresolved errors.
```cpp
void initSettings()
{
    // Set Default game settings can be set using oSettings.
}

void init()
{
    // Load your content.
}

void shutdown()
{
    // Cleanup.
}

void update()
{
    // Game logic.
}

void render()
{
    // Game render.
}

void postRender()
{
    // Render more stuff after post process.
}

void renderUI()
{
    // Draw imgui debug UI.
}
```
Look at samples to see what can be done here.

## Specific usages

## SpriteAnim
Sprite anims can be edited in Aseprite and exported as json. Certain flags can be set on frame's properties. Properties are colon "`:`" separated.

i.e.: "`loop:genfliph:next=idle`"

Frame properties and their meaning:

|name|usage|scope|meaning|
|-|-|-|-|
|loop|`loop`|tag|animation will be looped|
|genfliph|`genfliph`|tag|an extra animation will be created with the name tag+"_fliph", flipped horitontally|
|genflipv|`genflipv`|tag|an extra animation will be created with the name tag+"_fliph", flipped vertically|
|next|`next=tagName`|tag|when this animation is finished, it will trigger tagName|
|origin|`origin=x,y`|frame|set frame origin. This will apply to subsequent frames until the next origin.|
|sound|`sound=step.wav`|frame|plays a sound on this frame|
|soundcue|`soundcue=step.cue`|frame|plays a sound cue on this frame|
|vol|`vol=0.5`|frame|set volume for sound [0, 1].|
|pan|`pan=0.0`|frame|set panning for sound [-1, 1].|
|pitch|`pitch=2.0`|frame|set pitch for sound.|

## Mentions
_Logo design by Pixilabs Software Inc._

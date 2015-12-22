#Oak Nut Coding Guidelines
These guidelines are to ensure the uniformity of the engine. They are syntax and coding style. The current code _does not_ respect those guidelines in its current state. Refactoring is necessary.

##4 spaces instead of tabs
4 spaces should used instead of tabs

##Curly braces
**bad**
```cpp
void foo() {
}
```
**good**
```cpp
void food()
{
}
```
However, some exception. Initializer list.

**bad**
```cpp
std::vector<std::string> names = 
{
  "Marc",
  "David",
  "Pierre"
};
```
**good**
```cpp
std::string names[] = {
  "Marc",
  "David",
  "Pierre"
};
```
They are not function, but a continuity of the previous line.

##Comments
There are no strict rules regarding comments. Put them where necessary. Rules for Doxygen has not been set yet. Public headers, you can doxygen them or not.
If a piece of code is very complicated, please put comments. When code is obvious, comments are not necessary, but not prohibited neither!

##auto/decltype
Use `auto` and `decltype` to stay type agnostic as much as possible.

**bad**
```cpp
std::string key = "ENTITY";
std::string name = pPlayer->getName();
std::chrono::duration elapsed = std::chrono::seconds(1);
```
**good**
```cpp
auto key = "ENTITY";
auto name = pPlayer->getName();
auto elapsed = std::chrono::seconds(1);
```

##Hungarian
Hungarian notation should be avoided, but for a few exceptions. Types should be typed def where it make sense, and everything be given a contextual name.

**bad**
```cpp
int nHealth;
uint32_t uId;
```
**good**
```cpp
int health;
uint32_t id;
```
**ideal**
```cpp
using Health = int;
using PlayerID = uint32_t;

Health health;
PlayerID id;
```
Same goes with time values. Do not sufix with **uS**, **mS**, **S**, etc. Instead, use `std::chrono` types to store time values.

**bad**
```cpp
uint64_t elapsedMS = 1000; // 1 second in miliseconds
```
**good**
```cpp
auto elapsed = std::chrono::seconds(1);
```
There are a few exceptions that are explained elsewhere in the guidelines. As examples:
* **m_** for members
* **p** for pointers

##Class members
Public members are only allowed in utility classes. Like `Vector2`, `Color`, etc. Otherwise accessors should be defined and implemented in the source file.

Private members should start with the hungarian **m_** notation.

**bad**
```cpp
class Player
{
public:
    int health;
    Vector2 position;
};
```
**good**
```cpp
class Player
{
public:
    int getHealth() const;
    void setHealth(int health);
    
private:
    int m_health = 100;
    Vector2 m_position;
};
```

Members should be initialized to their default values in the header file if possible. This avoid scenario with multiple constructors and some have forgotten to initialize all members. If members have to be initialized in the constructor, because they are constructed from arguments. They must be initialized in the same order they are declared in the header.

**bad**
```cpp
class Player
{
public:
    Player(const Vector2& startPosition, const Color& color) 
        : m_health(100) // Should be initialized in definition
        , m_name("John Doe") // Should be initialized in definition
        , m_position(startPosition) // Should come after m_color
        , m_color(color)
    {}
    
private:
    std::string m_name;
    int m_health;
    Color m_color;
    Vector2 m_position;
};
```
**good**
```cpp
class Player
{
public:
    Player(const Vector2& startPosition, const Color& color)
        : m_color(color)
        , m_position(startPosition)
    {}
    
private:
    std::string m_name = "John Doe";
    int m_health = 100;
    Color m_color;
    Vector2 m_position;
};
```

##Pointers
All pointers should be prefixed with **p**. Double pointers with **pp** and so on. This makes it easier for the programmer to know if he should be using `.` or `->` to access its members.

**bad**
```cpp
Player* player;
void** data;
```
**good**
```cpp
Player* pPlayer;
void** ppData;
```

##Templates
Use them sparsingly. They are very useful. But once you start, they get everywhere. It makes it hard to newcomers to understand templated the code. Even for experienced programmers. _Currently onut is a template nightmare. It will be fixed in due time._

**bad** Current implementation in onut
```cpp
class Texture {};
class ContentManager
{
    template<typename Tresource>
    Tresource* getResource(const std::string& name) {}
};
```
**good**
```cpp
class Resource {};
class Texture : public Resource {};
class ContentManager
{
    Resource* getResource(const std::string& name) {}
};
```
Having the user to write their type as template argument, or in a cast on the returned value is no extra effort.

##Include guards
`#pragma once` should be used at the top of every header files

**bad**
```cpp
#ifndef _A_H_INCLUDED_
#define _A_H_INCLUDED_

class A {};

#endif
```
**good**
```cpp
#pragma once

class A {};
```

##Namespace
Everything in the engine should be put in the `onut` namespace. If 2 classes are in the same file, they should be placed in different namespace block for clarity.

**bad**
```cpp
class A {};
class B {};
```
**good**
```cpp
namespace onut
{
    class A {};
}

namespace onut
{
    class B {};
}
```
Same goes of its source file.

We allow `using` statement outside of namespaces for simplicity of use of the engine. Each public class should define one at the end of their header. They must start with the letter `O` (for Onut) followed by the class name.
```cpp
#pragma once

namespace onut
{
    class Texture {};
}

using OTexture = onut::Texture;
```

##Using namespace
`using namespace` is prohibited. Even for `std`.

**bad**
```cpp
using namespace std;
string name = "Marc";
```
**good**
```cpp
std::string name = "David";
```

##Enums
All enums should be strongly typed using the `class`keyword. And enum name should start with a capital letter just like a class. It is a type. Same goes with its values.

**bad**
```cpp
enum TWEEN
{
    LINEAR,
    EASE_OUT,
    EASE_IN,
    EASE_BOTH
};
```
**good**
```cpp
enum class Tween
{
    Linear,
    EaseOut,
    EaseIn,
    EaseBoth
};
```
We allow `using` statement for simplicity of use of the engine. Only on the enum type, not on its values.
```cpp
using OTween = onut::Tween;
```

##Constants
Constants should use UpperCamelCase. Avoid using `#define` for constants.

**bad**
```cpp
#define MaxSpeed 5
static const float MAX_ACCEL = 25.f;
```
**good**
```cpp
static const float MaxSpeed = 5.f;
static const float MaxAccel = 25.f;
```
Constants shouldn't be exposed to public API, but reside inside source files. If needed to be exposed, they should be part of an enum.

*... to be continued*

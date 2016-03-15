#Oak Nut Coding Guidelines
These guidelines are to ensure the uniformity of the engine. They are syntax and coding style. The current code _does not_ respect those guidelines in its current state. Refactoring is necessary.

##Anatomy of a header file
```cpp
// Todo
```

##Anatomy of a source file
```cpp
// Todo
```

##Memory management
All public interface in onut use `std::shared_ptr`. Using `OForwardDeclare` macro to generate typedefs.

As example, for a class `FooBar`, you'll get those:

* `onut::FooBar` -> `OFooBar`
* `std::shared_ptr<onut::FooBar>` -> `OFooBarRef`
* `std::weak_ptr<onut::FooBar>` -> `OFooBarWeak`

```cpp
auto pEntity = new OEntity();    // incorrect
auto pEntity = OMake<OEntity>();   // correct
```

##Formatting
4 spaces should used instead of tabs
```cpp
int myFunction( int param1 )    // incorrect
int myFunction(int param1)      // correct
int myFunction( )               // incorrect
int myFunction()                // correct
if ( i == 0 )                   // incorrect
if (i == 0)                     // correct
C = ( 3 + 4 ) * ( 2 + 7 );      // incorrect
C = (3 + 4) * (2 + 7);          // correct
int i=2;                        // incorrect
int i = 2;                      // correct
int i=5+7;                      // incorrect
int i = 5 + 7;                  // correct
for (y>0)                       // incorrect
for (y > 0)                     // correct
f(a , b , c);                   // incorrect
f(a, b, c);                     // correct
for (y = nHeight;y > 0;y--)     // incorrect
for (y = nHeight; y > 0; y--)   // correct
for (y = nHeight; y > 0; y --)  // incorrect
for (y = nHeight; y > 0; y--)   // correct
if(a + b)                       // incorrect
if (a + b)                      // correct
C = 3 + 4 * 2 + 7;              // incorrect
C = (3 + 4) * (2 + 7);          // correct
char *pPtr1;                    // incorrect
char* pPtr1;                    // correct
char *Foo();                    // incorrect
char* foo();                    // correct
const string & s;               // incorrect
const string &s;                // incorrect
const string& s;                // correct
```

##Curly braces
```cpp
void foo() {             // incorrect
}
void foo()
{                        // correct
}
std::vector<std::string> names = 
{                        // incorrect
};
std::string names[] = {  // correct
};
```

##Comments
There are no strict rules regarding comments. Put them where necessary. Rules for Doxygen has not been set yet. Public headers, you can doxygen them or not.
If a piece of code is very complicated, please put comments. When code is obvious, comments are not necessary, but not prohibited neither!

##auto
Use of `auto` is encouraged.

##Hungarian
Hungarian notation should be avoided, but for a few exceptions (pointers, members). Types should be typed def where it make sense, and everything be given a contextual name.
```cpp
int nHealth;    // incorrect
uint32_t uId;   // incorrect
int health;     // correct
uint32_t id;    // correct

using Health = int;
Health health;  // ideal
```
Same goes with time values. Do not sufix with **uS**, **mS**, **S**, etc. Instead, use `std::chrono` types to store time values.
```cpp
uint64_t elapsedMS = 1000;              // incorrect
auto elapsed = std::chrono::seconds(1); // correct
```

##Class members
Public members are only allowed in utility classes. Like `Vector2`, `Color`, etc. Otherwise accessors should be defined and implemented in the source file.

Members should be initialized to their default values in the header file if possible.
```cpp
class A
{
private:
    int health;             // incorrect
    int m_health = 100;     // correct
};
```

##Pointers
All pointers should be prefixed with **p**. Double pointers with **pp** and so on. This makes it easier for the programmer to know if he should be using `.` or `->` to access its members.
```cpp
Player* player;  // incorrect
Player* pPlayer; // correct
void** data;     // incorrect
void** pData;    // incorrect
void** ppData;   // correct
```

##Templates
Use them sparingly. They are very useful, but once you start, they get everywhere. It makes it hard to newcomers to understand templated the code. Even for experienced programmers.

##Include guards
`#pragma once` should be used at the top of every header files.

##Namespace
Everything in the engine should be put in the `onut` namespace. If 2 classes are in the same file, they should be placed in different namespace block for clarity.

##Using namespace
`using namespace` is prohibited. Even for `std`.

##Enums
Enums should be strongly typed using the `class` keyword. Unless it's a special case like a bitmask. Then maybe constants should be used instead.
```cpp
enum TWEEN         // incorrect
{
    LINEAR,
    EASE_OUT,
    EASE_IN,
    EASE_BOTH
};
enum class Tween   // correct
{
    Linear,
    EaseOut,
    EaseIn,
    EaseBoth
};
```
We allow `using` statement for simplicity of use of the engine. Public enums should define one at the end of their header. They must start with `O`.
```cpp
using OEnumName = onut::EnumName;
```

##Constants
Constants should use UPPER_CASE. Avoid using `#define` for constants.
```cpp
#define MAX_SPEED 5                   // incorrect
static const float MAX_SPEED = 25.f;  // correct
```



#Oak Nut Coding Guidelines
These guidelines are to ensure the uniformity of the engine. They are syntax and coding style. The current code _does not_ respect those guidelines in its current state. Refactoring is necessary.

##Anatomy of a header file
```cpp
#pragma once                                // include guard
#include "A.h"                              // onut includes
#include "B.h"                              // alphabetical order
#include "Entity.h"

#include <string>                           // Third party includes, or STD
#include <thirdPartyA.h>                    // alphabetical order
#include <thirdPartyB.h>

namespace onut
{
    class Inventory;                        // Forward declarations
}

namespace onut
{
    class Player final : public Entity      // use final if class is not meant to be inherited
    {
    public:
        using Health = int;                 // sub types
        
        Player();                           // Constructors
        Player(Health health);              // Define multiple constructors instead of init() methods
        virtual ~Player();                  // virtual destructor
        
        Health getHealth() const;           // Accessors
        void setHealth(Health health);
        
        Inventory* getInventory() const;    // Group things together that make sense
        void setInventory(Inventory* pInventory);
        
    protected:
        void update() override;             // use override when overriding.
    
    private:
        void initializeInventory();         // Private methods
    
        Health m_health = 100;              // Members initialized in header where possible
        Inventory* m_pInventory = nullptr;  // use nullptr instead of NULL or 0
        Vector2 m_position;                 // Don't align members with spaces
    };
}
using OPlayer = onut::Player;               // Declare this for classes in the public API
                                            // Extra return at the end
```

##Anatomy of a source file
```cpp
#include "Entity.h"                         // onut includes
#include "Inventory.h"                      // alphabetical order
#include "Player.h"                         // Don't put our class at the top

#include <string>                           // Third party includes, or STD
#include <thirdPartyA.h>                    // alphabetical order
#include <thirdPartyB.h>

namespace onut
{
    static const float MAX_SPEED = 10.f;    // Constants at the top
    
    Player::Player()
        : m_position(500, 500)
    {
        initializeInventory();
    }
    
    Player::Player(Health health)
        : m_health(health)
        , m_position(500, 500)              // Initialize members in the same order they are declared
    {
        initializeInventory();
    }
    
    Player::~Player()
    {
        delete m_pInventory;                // Don't check for null. delete takes care of that
    }
    
    Health Player::getHealth() const
    {
        return m_health;
    }
    
    void Player::setHealth(Health health)
    {
        m_health = health;
    }
    
    Inventory* Player::getInventory() const
    {
        return m_pInventory;
    }
    
    void Player::setInventory(Inventory* pInventory)
    {
        delete m_pIntenvory;
        m_pInventory = pInventory;
    }
    
    void Player::initializeInventory()
    {
        m_pInventory = new Inventory();
    }
}
                                            // Extra return at the end
```

##Memory management
Don't use `std::shared_ptr` and such, as they come with an overhead. Try to pool as much as possible using `OPool` object. The less the memory moves around, the better the engine works.

##4 spaces instead of tabs
4 spaces should used instead of tabs

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
Use of `auto` is encouraged, but shouldn't be used in the public interface. Unless of an utility templated class that requires it.

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

Private members should start with the hungarian **m_** notation.

Members should be initialized to their default values in the header file if possible.

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
Use them sparsingly. They are very useful, but once you start, they get everywhere. It makes it hard to newcomers to understand templated the code. Even for experienced programmers. _Currently onut is a template nightmare. It will be fixed in due time._

##Include guards
`#pragma once` should be used at the top of every header files

##Namespace
Everything in the engine should be put in the `onut` namespace. If 2 classes are in the same file, they should be placed in different namespace block for clarity.

We allow `using` statement outside of namespaces for simplicity of use of the engine. Each public class should define one at the end of their header. They must start with the letter `O` (for Onut) followed by the class name.
```cpp
using OClassName = onut::ClassName;
```

##Using namespace
`using namespace` is prohibited. Even for `std`.

##Enums
All enums should be strongly typed using the `class` keyword. It is a type.
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
We allow `using` statement for simplicity of use of the engine. Public enums should defined one at the end of their header. They must start with `O`.
```cpp
using OEnumName = onut::EnumName;
```

##Constants
Constants should use UPPER_CASE. Avoid using `#define` for constants.
```cpp
#define MAX_SPEED 5                   // incorrect
static const float MAX_SPEED = 25.f;  // correct
```
Constants shouldn't be exposed to public API, but reside inside source files. If needed to be exposed, they should be part of an enum.


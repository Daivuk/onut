cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(DungeonSample)

include_directories(
    ./src
)
    
add_executable(DungeonSample WIN32
    src/Bat.cpp
    src/Bomb.cpp
    src/Chest.cpp
    src/DamageFlasher.cpp
    src/Damager.cpp
    src/Door.cpp
    src/DoorTraverser.cpp
    src/Dungeon.cpp
    src/DungeonSample.cpp
    src/Exit.cpp
    src/Guard.cpp
    src/ItemJump.cpp
    src/Life.cpp
    src/OnRoomResetter.cpp
    src/Player.cpp
    src/PushBack.cpp
    src/SmartRoomCamera.cpp
    src/TreasureHunter.cpp
    src/Vase.cpp
)

target_link_libraries(DungeonSample 
    libonut
)

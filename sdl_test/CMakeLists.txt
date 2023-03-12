cmake_minimum_required(VERSION 3.7)

project(SDL2Test)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)

find_package(SDL2_ttf REQUIRED)

find_package(SDL2_mixer REQUIRED)


add_executable(SDL2Test AStarNode.cpp Bomb.cpp Bonfire.cpp Boss2.cpp BossBody.cpp Button.cpp Buyable.cpp Camera.cpp Chest.cpp Door.cpp EnemyBasic.cpp Enemy.cpp EnemyDash.cpp EnemyHandler.cpp EnemyJump.cpp EnemyShooter.cpp EntityHandler.cpp Explosion.cpp FireMagic.cpp Game.cpp General.cpp main.cpp MovingRect.cpp Npc.cpp Particle.cpp Pickupable.cpp Player.cpp Portal.cpp Rect.cpp SaveState.cpp Shot.cpp TileHandler.cpp)
target_link_libraries(SDL2Test ${SDL2_LIBRARIES} SDL2_mixer SDL2_image SDL2_ttf)

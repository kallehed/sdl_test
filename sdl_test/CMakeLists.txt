cmake_minimum_required(VERSION 3.7)

project(SDL2Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

set(SOURCES AStarNode.cpp Bomb.cpp Bonfire.cpp Boss2.cpp BossBody.cpp Button.cpp Buyable.cpp Camera.cpp Chest.cpp Door.cpp EnemyBasic.cpp Enemy.cpp EnemyDash.cpp EnemyHandler.cpp EnemyJump.cpp EnemyShooter.cpp EntityHandler.cpp Explosion.cpp FireMagic.cpp Game.cpp General.cpp main.cpp MovingRect.cpp Npc.cpp Particle.cpp Pickupable.cpp Player.cpp Portal.cpp Rect.cpp SaveState.cpp Shot.cpp TileHandler.cpp)

# if building shared libs, assume above CMake file has loaded the libraries shared
# otherwise get all the packages from package manager and build executable

if(BUILD_SHARED_LIBS)
	add_library(main SHARED)
	target_sources(main PRIVATE ${SOURCES})

	target_link_libraries(main SDL2 SDL2_image SDL2_mixer SDL2_ttf)
else()
	if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
		set(USE_FLAGS "--preload-file data -s ALLOW_MEMORY_GROWTH=1 --use-preload-plugins -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS=\'\[\"png\"\]\' ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
		set(CMAKE_EXECUTABLE_SUFFIX .html)
	else()
		find_package(SDL2 REQUIRED)
		include_directories(${SDL2_INCLUDE_DIRS})
	
		find_package(SDL2_image REQUIRED)
		find_package(SDL2_ttf REQUIRED)
		find_package(SDL2_mixer REQUIRED)
	endif()
	add_executable(SDL2Test ${SOURCES})
	target_link_libraries(SDL2Test ${SDL2_LIBRARIES} SDL2_mixer SDL2_image SDL2_ttf)
endif()

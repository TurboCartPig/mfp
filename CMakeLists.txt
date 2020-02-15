cmake_minimum_required(VERSION 3.5)
project(MFP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/W4 /w44265 /w44062 /wd4061) # Warnings
	add_compile_options(/utf-8 /arch:AVX2)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang | AppleClang | GNU")
	add_compile_options(-Wall -Wextra -Wunreachable-code -Wpedantic -Wdocumentation)
	add_compile_options(-mtune=generic -march=x86_64 -mavx2)
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang | AppleClang")
		add_compile_options(-Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn)
	endif()
endif()

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW)

add_executable(solar src/solar.cpp)
target_link_libraries(solar sfml-graphics)

add_executable(inter src/inter.cpp)
target_link_libraries(inter sfml-graphics)

add_executable(interab src/interab.cpp)
target_link_libraries(interab sfml-graphics)

add_executable(msi src/midpoint-displacement.cpp)
target_link_libraries(msi sfml-graphics)

add_executable(perlin src/perlin.cpp)
target_link_libraries(perlin sfml-graphics)

add_executable(boids src/boids.cpp)
target_link_libraries(boids sfml-graphics)

add_executable(tetris src/tetris_live.cpp)
target_link_libraries(tetris sfml-graphics)

add_executable(fountain src/fountain.cpp)
target_link_libraries(fountain sfml-graphics)

add_executable(fountain-gpu src/fountain-gpu.cpp)
target_link_libraries(fountain-gpu sfml-window OpenGL::OpenGL GLEW::GLEW)

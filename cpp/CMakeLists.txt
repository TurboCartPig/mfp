cmake_minimum_required(VERSION 3.16)
project(MFP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
message("Release: ${CMAKE_CXX_FLAGS_RELEASE}")

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
            # FIXME: Do I need this?
            /utf-8
            # Turn on warnings
            /W4 /w44265 /w44062 /wd4061
            # Reasonably well supported by now
            /arch:AVX2
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    add_compile_options(
            # Turn on warnings
            -Wall -Wextra -Wunreachable-code -Wpedantic
            -Wdocumentation -Wweak-vtables -Wexit-time-destructors
            -Wglobal-constructors -Wmissing-noreturn
            # Reasonably well supported base platform
            -march=skylake
            # Turn on sanitizers if build type is debug
            $<$<CONFIG:DEBUG>:-fsanitize=undefined,address>
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("Warning: GCC has not been well tested yet; Consider using Clang")
else ()
    message("Warning: Your compiler is not supported!")
endif ()

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glbinding REQUIRED COMPONENTS glbinding)

add_library(common INTERFACE)
target_link_libraries(common INTERFACE sfml-graphics)
target_precompile_headers(common INTERFACE src/common.h)

add_executable(solar src/solar.cpp)
target_link_libraries(solar PRIVATE common)

add_executable(inter src/inter.cpp)
target_link_libraries(inter PRIVATE common)

add_executable(interab src/interab.cpp)
target_link_libraries(interab PRIVATE common)

add_executable(msi src/midpoint-displacement.cpp)
target_link_libraries(msi PRIVATE common)

add_executable(perlin src/perlin.cpp)
target_link_libraries(perlin PRIVATE common)

add_executable(boids src/boids.cpp)
target_link_libraries(boids PRIVATE common)

add_executable(fountain src/fountain.cpp)
target_link_libraries(fountain PRIVATE common)

add_executable(fountain-gpu src/fountain-gpu.cpp)
target_link_libraries(
            fountain-gpu
        PRIVATE
            sfml-window
            OpenGL::GL
            glbinding::glbinding
            glbinding::glbinding-aux
)

add_executable(quat src/quat.cpp)
target_link_libraries(quat PRIVATE sfml-system)

add_executable(billiard src/billiard.cpp)
target_link_libraries(billiard PRIVATE common)

add_executable(binary-ops src/binary-ops.cpp)

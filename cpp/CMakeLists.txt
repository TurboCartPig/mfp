cmake_minimum_required(VERSION 3.16)
project(
  MFP
  VERSION 0.1.0
  DESCRIPTION "Exercises from BMA1020 - Mattematikk for programming"
  LANGUAGES CXX)

# Set the C++ standard to use globally
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup clangd's compile_commands.json and symlink it to the project's root
# directory
if(CONFIG EQUAL DEBUG)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()

# List all the compiler options for internal use
set(PRIVATE_COMPILE_OPTIONS
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/w44061>
    $<$<CXX_COMPILER_ID:MSVC>:/w44062>
    $<$<CXX_COMPILER_ID:MSVC>:/w44265>
    $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wall>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wextra>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wpedantic>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wunreachable-code>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wmissing-noreturn>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-march=skylake>
    $<$<AND:$<CXX_COMPILER_ID:Clang,AppleClang,GNU>,$<CONFIG:DEBUG>>:-fsanitize=undefined,address>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wdocumentation>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wweak-vtables>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wexit-time-destructors>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wglobal-constructors>)

# Find dependencies
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glbinding REQUIRED COMPONENTS glbinding)

# Setup individual exercises as buildable targets
# ******************************************************************

add_executable(solar src/solar.cpp)
target_link_libraries(solar PRIVATE sfml-graphics)
target_compile_options(solar PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(inter src/inter.cpp)
target_link_libraries(inter PRIVATE sfml-graphics)
target_compile_options(inter PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(interab src/interab.cpp)
target_link_libraries(interab PRIVATE sfml-graphics)
target_compile_options(interab PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(mpd src/midpoint-displacement.cpp)
target_link_libraries(mpd PRIVATE sfml-graphics)
target_compile_options(mpd PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(perlin src/perlin.cpp)
target_link_libraries(perlin PRIVATE sfml-graphics)
target_compile_options(perlin PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(boids src/boids.cpp)
target_link_libraries(boids PRIVATE sfml-graphics)
target_compile_options(boids PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(fountain src/fountain.cpp)
target_link_libraries(fountain PRIVATE sfml-graphics)
target_compile_options(fountain PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(fountain-gpu src/fountain-gpu.cpp)
target_link_libraries(
  fountain-gpu PRIVATE sfml-window OpenGL::GL glbinding::glbinding
                       glbinding::glbinding-aux)
target_compile_options(fountain-gpu PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(quat src/quat.cpp)
target_link_libraries(quat PRIVATE sfml-system)
target_compile_options(quat PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(billiard src/billiard.cpp)
target_link_libraries(billiard PRIVATE sfml-graphics)
target_compile_options(billiard PRIVATE ${PRIVATE_COMPILE_OPTIONS})

add_executable(binary-ops src/binary-ops.cpp)
target_compile_options(binary-ops PRIVATE ${PRIVATE_COMPILE_OPTIONS})

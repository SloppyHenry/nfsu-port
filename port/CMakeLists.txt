cmake_minimum_required(VERSION 3.24)
project(nfsu LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

get_filename_component(NFSU_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Spieldaten des Nutzers: Tabellen werden beim Bauen aus dessen Speed.exe erzeugt, nichts davon liegt im Repository
set(NFSU_GAME_DIR "${NFSU_REPO_ROOT}/game/pc" CACHE PATH "Ordner der NFSU-PC-Installation (mit Speed.exe)")
set(NFSU_EXE "${NFSU_GAME_DIR}/Speed.exe")
if(NOT EXISTS "${NFSU_EXE}")
  message(FATAL_ERROR "Speed.exe nicht gefunden: ${NFSU_EXE}\nBitte -DNFSU_GAME_DIR=<NFSU-Installation> angeben.")
endif()
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(NFSU_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(NFSU_GENERATED_SOURCES)
foreach(gen IN ITEMS "bware_tables|base" "joystick_tables|misc")
  string(REPLACE "|" ";" gen "${gen}")
  list(GET gen 0 name)
  list(GET gen 1 dir)
  set(out "${NFSU_GEN_DIR}/game/${dir}/${name}.cpp")
  add_custom_command(OUTPUT "${out}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${NFSU_GEN_DIR}/game/${dir}"
    COMMAND Python3::Interpreter "${NFSU_REPO_ROOT}/tools/gen_${name}.py" "${NFSU_EXE}" "${out}"
    DEPENDS "${NFSU_REPO_ROOT}/tools/gen_${name}.py" "${NFSU_REPO_ROOT}/tools/pe_image.py" "${NFSU_EXE}"
    COMMENT "Erzeuge ${name}.cpp aus Speed.exe")
  list(APPEND NFSU_GENERATED_SOURCES "${out}")
endforeach()

# plattformfreie Basis
set(NFSU_CORE_SOURCES src/core/paths.cpp src/core/hash.cpp src/core/matrix.cpp
    src/formats/jdlz.cpp src/formats/chunk.cpp src/formats/ea_audio.cpp
    src/formats/texture_pack.cpp src/formats/png.cpp src/formats/geometry.cpp
    src/formats/gltf_export.cpp src/formats/scenery.cpp src/formats/track_info.cpp
    src/game/misc/point.cpp src/game/misc/spline.cpp src/game/misc/timer.cpp src/game/misc/joylog.cpp src/game/misc/rumble.cpp
    src/game/misc/queuedfile.cpp src/game/misc/events.cpp
    src/game/misc/joystick.cpp
    src/game/base/bware.cpp src/game/misc/quickspline.cpp src/game/misc/table.cpp src/game/misc/lzcompress.cpp
    src/core/testapi.cpp)
find_package(ZLIB REQUIRED)  # System-zlib des macOS-SDK
add_library(nfsu_core STATIC ${NFSU_CORE_SOURCES} ${NFSU_GENERATED_SOURCES})
target_include_directories(nfsu_core PUBLIC src)
target_link_libraries(nfsu_core PUBLIC ZLIB::ZLIB)
target_compile_definitions(nfsu_core PUBLIC NFSU_DEFAULT_DATA="${NFSU_GAME_DIR}")
set_target_properties(nfsu_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# dylib für die Emulator-Brücke (WP-05)
add_library(nfsu_core_shared SHARED ${NFSU_CORE_SOURCES} ${NFSU_GENERATED_SOURCES})
target_include_directories(nfsu_core_shared PUBLIC src)
target_link_libraries(nfsu_core_shared PUBLIC ZLIB::ZLIB)
target_compile_definitions(nfsu_core_shared PUBLIC NFSU_DEFAULT_DATA="${NFSU_GAME_DIR}")
set_target_properties(nfsu_core_shared PROPERTIES OUTPUT_NAME nfsu_core)

# SDL3-Schicht
find_package(SDL3 REQUIRED CONFIG)
add_library(nfsu_platform STATIC src/platform/window.cpp)
target_include_directories(nfsu_platform PUBLIC src)
target_link_libraries(nfsu_platform PUBLIC nfsu_core SDL3::SDL3)

# Filmabspieler (ADR-002): libavformat/libavcodec/libswresample über pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswresample)
add_library(nfsu_video STATIC src/platform/video.cpp)
target_include_directories(nfsu_video PUBLIC src)
target_link_libraries(nfsu_video PUBLIC SDL3::SDL3 PkgConfig::FFMPEG)

add_executable(nfsu apps/nfsu/main.cpp)
target_link_libraries(nfsu PRIVATE nfsu_platform)

add_executable(nfsu-tool apps/nfsu-tool/main.cpp)
target_link_libraries(nfsu-tool PRIVATE nfsu_core nfsu_video)

enable_testing()
add_executable(nfsu_tests tests/main.cpp tests/test_paths.cpp tests/test_ptr32.cpp tests/test_chunk.cpp tests/test_ea_audio.cpp
    tests/test_texture.cpp tests/test_geometry.cpp tests/test_misc.cpp tests/test_misc2.cpp tests/test_bware.cpp)
target_link_libraries(nfsu_tests PRIVATE nfsu_core)
add_test(NAME nfsu_tests COMMAND nfsu_tests)

# MSL-Shader (ADR-003): nur auf macOS, xcrun metal -> .air -> .metallib
if(APPLE)
  set(NFSU_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/render/shaders)
  set(NFSU_SHADER_SRC world world_variants car filter)
  set(NFSU_SHADER_AIR "")
  foreach(s ${NFSU_SHADER_SRC})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/${s}.air
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
      COMMAND xcrun -sdk macosx metal -c ${NFSU_SHADER_DIR}/${s}.metal -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/${s}.air
      DEPENDS ${NFSU_SHADER_DIR}/${s}.metal ${NFSU_SHADER_DIR}/common.h VERBATIM)
    list(APPEND NFSU_SHADER_AIR ${CMAKE_CURRENT_BINARY_DIR}/shaders/${s}.air)
  endforeach()
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/nfsu.metallib
    COMMAND xcrun -sdk macosx metallib ${NFSU_SHADER_AIR} -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/nfsu.metallib
    DEPENDS ${NFSU_SHADER_AIR} VERBATIM)
  add_custom_target(nfsu_shaders ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shaders/nfsu.metallib)

  # Strecken-Viewer (WP-18)
  add_executable(nfsu-viewer apps/viewer/main.cpp apps/viewer/scene.cpp)
  target_link_libraries(nfsu-viewer PRIVATE nfsu_platform)
  target_compile_definitions(nfsu-viewer PRIVATE NFSU_SHADER_LIB="${CMAKE_CURRENT_BINARY_DIR}/shaders/nfsu.metallib"
                             NFSU_REPO_ROOT="${NFSU_REPO_ROOT}")
  add_dependencies(nfsu-viewer nfsu_shaders)
endif()

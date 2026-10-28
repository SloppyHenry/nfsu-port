cmake_minimum_required(VERSION 3.24)
project(nfsu LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W3 /utf-8 /permissive- /Zc:__cplusplus)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX _USE_MATH_DEFINES)
else()
  add_compile_options(-Wall -Wextra)
endif()
option(NFSU_SANITIZE "AddressSanitizer und UBSan (nicht unter MSVC)" OFF)
if(NFSU_SANITIZE AND NOT MSVC)
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
endif()
# Abhängigkeiten aus dem Netz holen statt vom System (CI, Windows): SDL3, zlib
option(NFSU_FETCH_DEPS "SDL3 und zlib per FetchContent bauen" OFF)

get_filename_component(NFSU_REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Spieldaten des Nutzers: Tabellen werden beim Bauen aus dessen Speed.exe erzeugt, nichts davon liegt im Repository
set(NFSU_GAME_DIR "${NFSU_REPO_ROOT}/game/pc" CACHE PATH "Ordner der NFSU-PC-Installation (mit Speed.exe)")
# NFSU_STUB_TABLES: ohne Speed.exe bauen (nur Kompilierprüfung, Tabellen sind Nullen)
option(NFSU_STUB_TABLES "Tabellen als Platzhalter erzeugen" OFF)
if(NFSU_STUB_TABLES)
  set(NFSU_EXE "STUB")
else()
  set(NFSU_EXE "${NFSU_GAME_DIR}/Speed.exe")
endif()
if(NOT NFSU_STUB_TABLES AND NOT EXISTS "${NFSU_EXE}")
  message(FATAL_ERROR "Speed.exe nicht gefunden: ${NFSU_EXE}\nBitte -DNFSU_GAME_DIR=<NFSU-Installation> angeben (oder -DNFSU_STUB_TABLES=ON nur zum Kompilieren).")
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
    DEPENDS "${NFSU_REPO_ROOT}/tools/gen_${name}.py" "${NFSU_REPO_ROOT}/tools/pe_image.py" $<$<NOT:$<BOOL:${NFSU_STUB_TABLES}>>:${NFSU_EXE}>
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
if(NFSU_FETCH_DEPS)
  include(FetchContent)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # zlibstatic landet auch in der Shared Library
  set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(zlib URL https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz)
  FetchContent_Declare(SDL3 URL https://github.com/libsdl-org/SDL/releases/download/release-3.2.24/SDL3-3.2.24.tar.gz)
  FetchContent_MakeAvailable(zlib SDL3)
  add_library(ZLIB::ZLIB ALIAS zlibstatic)
  target_include_directories(zlibstatic INTERFACE ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
else()
  find_package(ZLIB REQUIRED)
  find_package(SDL3 REQUIRED CONFIG)
endif()
add_library(nfsu_core STATIC ${NFSU_CORE_SOURCES} ${NFSU_GENERATED_SOURCES})
target_include_directories(nfsu_core PUBLIC src)
target_link_libraries(nfsu_core PUBLIC ZLIB::ZLIB)
target_compile_definitions(nfsu_core PUBLIC NFSU_DEFAULT_DATA="${NFSU_GAME_DIR}")
set_target_properties(nfsu_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# dylib für die Emulator-Brücke (WP-05), nur Entwicklungswerkzeug
if(NOT WIN32)
add_library(nfsu_core_shared SHARED ${NFSU_CORE_SOURCES} ${NFSU_GENERATED_SOURCES})
target_include_directories(nfsu_core_shared PUBLIC src)
target_link_libraries(nfsu_core_shared PUBLIC ZLIB::ZLIB)
target_compile_definitions(nfsu_core_shared PUBLIC NFSU_DEFAULT_DATA="${NFSU_GAME_DIR}")
set_target_properties(nfsu_core_shared PROPERTIES OUTPUT_NAME nfsu_core)
endif()

# SDL3-Schicht
add_library(nfsu_platform STATIC src/platform/window.cpp)
target_include_directories(nfsu_platform PUBLIC src)
target_link_libraries(nfsu_platform PUBLIC nfsu_core SDL3::SDL3)

# Filmabspieler (ADR-002): libavformat/libavcodec/libswresample über pkg-config,
# sonst (Windows) aus FFMPEG_ROOT mit include/ und lib/
set(FFMPEG_ROOT "" CACHE PATH "FFmpeg-Entwicklerpaket (include/, lib/), falls kein pkg-config")
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND AND NOT FFMPEG_ROOT)
  pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET libavformat libavcodec libavutil libswresample)
  set(NFSU_FFMPEG PkgConfig::FFMPEG)
else()
  find_path(FFMPEG_INCLUDE libavcodec/avcodec.h HINTS ${FFMPEG_ROOT}/include REQUIRED)
  set(NFSU_FFMPEG)
  foreach(l avformat avcodec avutil swresample)
    find_library(FFMPEG_${l} ${l} HINTS ${FFMPEG_ROOT}/lib REQUIRED)
    list(APPEND NFSU_FFMPEG ${FFMPEG_${l}})
  endforeach()
endif()
add_library(nfsu_video STATIC src/platform/video.cpp)
target_include_directories(nfsu_video PUBLIC src ${FFMPEG_INCLUDE})
target_link_libraries(nfsu_video PUBLIC SDL3::SDL3 ${NFSU_FFMPEG})

add_executable(nfsu apps/nfsu/main.cpp)
target_link_libraries(nfsu PRIVATE nfsu_platform)

add_executable(nfsu-tool apps/nfsu-tool/main.cpp)
target_link_libraries(nfsu-tool PRIVATE nfsu_core nfsu_video)

enable_testing()
add_executable(nfsu_tests tests/main.cpp tests/test_paths.cpp tests/test_ptr32.cpp tests/test_chunk.cpp tests/test_ea_audio.cpp
    tests/test_texture.cpp tests/test_geometry.cpp tests/test_misc.cpp tests/test_misc2.cpp tests/test_bware.cpp)
target_link_libraries(nfsu_tests PRIVATE nfsu_core)
add_test(NAME nfsu_tests COMMAND nfsu_tests)

# MSL-Shader (ADR-003): nur auf macOS, xcrun metal -> .air -> .metallib.
# Andere Plattformen: Viewer kompiliert, findet zur Laufzeit aber noch keine Shader (WP-19).
set(NFSU_SHADER_LIB_PATH "")
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
  set(NFSU_SHADER_LIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/shaders/nfsu.metallib")
endif()

# Strecken-Viewer (WP-18)
add_executable(nfsu-viewer apps/viewer/main.cpp apps/viewer/scene.cpp)
target_link_libraries(nfsu-viewer PRIVATE nfsu_platform)
target_compile_definitions(nfsu-viewer PRIVATE NFSU_SHADER_LIB="${NFSU_SHADER_LIB_PATH}"
                           NFSU_REPO_ROOT="${NFSU_REPO_ROOT}")
if(APPLE)
  add_dependencies(nfsu-viewer nfsu_shaders)
endif()

cmake_minimum_required(VERSION 3.11)

project(PerlinNoise)
set(BINARY_NAME perlin_noise)

# Append cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(include)

# Set sources
file(GLOB_RECURSE SRCS "src/*.cpp")
add_definitions(-g)
add_definitions(-O3)

add_executable(${BINARY_NAME} ${SRCS})

if (WIN32)
    include_directories("${CMAKE_MODULE_PATH}/SFML/window/include")
    set(SFML_LIBRARIES
        "${CMAKE_MODULE_PATH}/SFML/window/lib/flac.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/freetype.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/ogg.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/openal32.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/sfml-audio.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/sfml-system.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/sfml-graphics.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/sfml-network.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/sfml-window.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/vorbis.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/vorbisenc.lib"
        "${CMAKE_MODULE_PATH}/SFML/window/lib/vorbisfile.lib"
    )

    set(SFML_DLLS
        "${CMAKE_MODULE_PATH}/SFML/window/bin/openal32.dll"
        "${CMAKE_MODULE_PATH}/SFML/window/bin/sfml-audio-2.dll"
        "${CMAKE_MODULE_PATH}/SFML/window/bin/sfml-system-2.dll"
        "${CMAKE_MODULE_PATH}/SFML/window/bin/sfml-graphics-2.dll"
        "${CMAKE_MODULE_PATH}/SFML/window/bin/sfml-window-2.dll"
        "${CMAKE_MODULE_PATH}/SFML/window/bin/sfml-network-2.dll"
    )
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    target_link_libraries(${BINARY_NAME} ${SFML_LIBRARIES})
else()
    # Add phtread library
    include_directories("${CMAKE_MODULE_PATH}/SFML/include")
    set(SFML_LIBRARIES
        "${CMAKE_MODULE_PATH}/SFML/linux/libsfml-audio.so.2.5.1"
        "${CMAKE_MODULE_PATH}/SFML/linux/libsfml-system.so.2.5.1"
        "${CMAKE_MODULE_PATH}/SFML/linux/libsfml-network.so.2.5.1"
        "${CMAKE_MODULE_PATH}/SFML/linux/libsfml-window.so.2.5.1"
        "${CMAKE_MODULE_PATH}/SFML/linux/libsfml-graphics.so.2.5.1"
    )
    target_link_libraries(${BINARY_NAME} ${SFML_LIBRARIES})
endif()

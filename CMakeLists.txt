cmake_minimum_required(VERSION 3.28)
project(grafika2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS engine/*.cpp)
add_library(silnik2d STATIC ${ENGINE_SOURCES})
target_link_libraries(silnik2d PUBLIC SFML::Graphics SFML::Window SFML::System)

add_subdirectory(game)     # demo techniczne: gra w stylu Hotline Miami (biblioteka hotline_game + hotline_demo)

# glowny program: sandbox + demo gry w jednym oknie (F1 / F2 / Tab przelaczaja)
add_executable(grafika2d main.cpp
        SandboxApp.cpp
        SandboxApp.h)
target_link_libraries(grafika2d PRIVATE silnik2d hotline_game)
target_include_directories(grafika2d PRIVATE ${CMAKE_SOURCE_DIR})
add_custom_command(TARGET grafika2d POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:grafika2d>/assets
        COMMENT "Kopiowanie tekstur do $<TARGET_FILE_DIR:grafika2d>/assets")

# dokumentacja (opcjonalnie): cmake --build . --target docs
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generowanie dokumentacji Doxygen do docs/html"
            VERBATIM)
endif()

# Demo techniczne: prosta gra w stylu Hotline Miami z teksturami.
# Caly kod gry jest tutaj, silnik (biblioteka silnik2d) o grze nic nie wie.
add_library(hotline_game STATIC
        HotlineGame.cpp
        HotlineGame.h
        HotlinePlayer.cpp
        HotlinePlayer.h
        CharacterSprite.cpp
        CharacterSprite.h
        Enemy.cpp
        Enemy.h
        Bullet.cpp
        Bullet.h
        GameAssets.cpp
        GameAssets.h
)
target_link_libraries(hotline_game PUBLIC silnik2d)
# naglowki silnika dolaczamy jako "engine/..." (od korzenia projektu), wlasne - bez przedrostka
target_include_directories(hotline_game PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
# ostatnia deska ratunku, gdyby program uruchomiono z dziwnego katalogu roboczego
target_compile_definitions(hotline_game PUBLIC ASSETS_SOURCE_DIR="${CMAKE_SOURCE_DIR}/assets")

add_executable(hotline_demo main.cpp)
target_link_libraries(hotline_demo PRIVATE hotline_game)

# po kazdym zbudowaniu kopiuj tekstury obok programu (bin/assets)
add_custom_command(TARGET hotline_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:hotline_demo>/assets
        COMMENT "Kopiowanie tekstur do $<TARGET_FILE_DIR:hotline_demo>/assets")

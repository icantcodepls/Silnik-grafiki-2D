/**
 * @file main.cpp
 * @brief Punkt wejścia: jedno okno, w którym F1 / F2 / Tab przełączają sandbox i demo gry.
 *
 * Uruchomienie:  grafika2d            - start w sandboxie,
 *                grafika2d --game     - start w demie gry.
 */
#include <cstring>

#include "SandboxApp.h"
#include "engine/AppSwitcher.h"
#include "engine/Engine.h"
#include "game/HotlineGame.h"

int main(int argc, char** argv) {
    EngineSettings settings;
    settings.title = "Silnik 2D";

    Engine engine(settings);
    SandboxApp sandbox;
    HotlineGame game;

    AppSwitcher switcher;
    switcher.add("Sandbox", sandbox);   // F1
    switcher.add("Hotline Demo", game); // F2
    if (argc > 1 && std::strcmp(argv[1], "--game") == 0) {
        switcher.setInitial(1);
    }
    return engine.run(switcher);
}

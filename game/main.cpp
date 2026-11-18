#include "HotlineGame.h"
#include "engine/Engine.h"

int main() {
    EngineSettings settings;
    settings.title = "Hotline Demo";
    settings.resolution = {800, 600};

    Engine engine(settings);
    HotlineGame game;
    return engine.run(game);
}

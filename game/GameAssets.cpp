#include "GameAssets.h"

#include <filesystem>
#include <string>
#include <vector>

#include "engine/Logger.h"

namespace fs = std::filesystem;

namespace {
// Gdzie szukac katalogu assets/? Zaleznie od tego, skad uruchomiono program:
//  - "assets"      - katalog roboczy = folder z programem (tam CMake kopiuje assets po budowaniu),
//  - "../assets"   - katalog roboczy = podkatalog (np. bin/ w IDE),
//  - ASSETS_SOURCE_DIR - katalog zrodlowy projektu (wpisany przez CMake) - ostatnia deska ratunku.
std::string findAssetsDir() {
    std::vector<fs::path> candidates = {"assets", "../assets", "../../assets"};
#ifdef ASSETS_SOURCE_DIR
    candidates.emplace_back(ASSETS_SOURCE_DIR);
#endif
    for (const fs::path& dir : candidates) {
        std::error_code ec;
        if (fs::exists(dir / "player_legs.png", ec)) {
            return dir.string();
        }
    }
    return "";
}

// wczytuje teksture albo (po zapisaniu bledu w logu) tworzy rozowa zastepcza o podanym rozmiarze
BitmapId loadOrPlaceholder(BitmapHandler& handler, const std::string& dir, const std::string& file, unsigned int w,
                           unsigned int h, bool& ok) {
    if (!dir.empty()) {
        const BitmapId id = handler.load(dir + "/" + file);
        if (id != INVALID_BITMAP) {
            return id;
        }
    }
    Logger::error("Nie mozna wczytac tekstury '" + file + "' (szukano w: '" +
                  (dir.empty() ? "nie znaleziono katalogu assets" : dir) + "') - uzyto tekstury zastepczej");
    ok = false;
    return handler.create(w, h, Color(255, 0, 255));
}
} // namespace

GameAssets::GameAssets() {
    Logger::init(); // blad wczytania musi trafic do errors.log juz teraz
    const std::string dir = findAssetsDir();
    if (!dir.empty()) {
        Logger::info("Tekstury z katalogu: " + fs::absolute(dir).string());
    }
    bool ok = true;
    playerLegs = loadOrPlaceholder(handler, dir, "player_legs.png", 128, 32, ok);
    playerTorso = loadOrPlaceholder(handler, dir, "player_torso.png", 64, 32, ok);
    enemyLegs = loadOrPlaceholder(handler, dir, "enemy_legs.png", 128, 32, ok);
    enemyTorso = loadOrPlaceholder(handler, dir, "enemy_torso.png", 64, 32, ok);
    allLoaded = ok;
}

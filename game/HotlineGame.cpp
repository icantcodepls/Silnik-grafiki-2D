#include "HotlineGame.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <SFML/Graphics.hpp>

#include "engine/Engine.h"
#include "engine/Geometry.h"
#include "engine/PrimitiveRenderer.h"

using namespace sf;

namespace {
constexpr float SPAWN_MARGIN = 40.f; // przeciwnicy nie stoja przy samej krawedzi
constexpr float MIN_DIST_FROM_PLAYER = 150.f;
constexpr float MIN_DIST_BETWEEN_ENEMIES = 60.f;
constexpr int GRID_STEP = 50;

float distance(Vector2f a, Vector2f b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}
} // namespace

HotlineGame::HotlineGame() : player({0.f, 0.f}, assets) {}

void HotlineGame::onStart(Engine& e) {
    engine = &e;
    size = e.getResolution();
    restart();
}

void HotlineGame::restart() {
    bullets.clear();
    player.setBounds(size);
    player.setPosition({size.x / 2.f, size.y / 2.f});
    spawnEnemies();
    updateTitle();
}

Vector2f HotlineGame::randomPoint() {
    std::uniform_real_distribution<float> dx(SPAWN_MARGIN, static_cast<float>(size.x) - SPAWN_MARGIN);
    std::uniform_real_distribution<float> dy(SPAWN_MARGIN, static_cast<float>(size.y) - SPAWN_MARGIN);
    return {dx(rng), dy(rng)};
}

bool HotlineGame::isFreeSpot(Vector2f p) const {
    if (distance(p, player.getPosition()) < MIN_DIST_FROM_PLAYER) {
        return false;
    }
    return std::none_of(enemies.begin(), enemies.end(),
                        [&](const Enemy& e) { return distance(p, e.getPosition()) < MIN_DIST_BETWEEN_ENEMIES; });
}

// 5 przeciwnikow w losowych miejscach; po 200 nieudanych probach (bardzo maly
// ekran) bierzemy ostatni wylosowany punkt, zeby zawsze bylo ich dokladnie 5
void HotlineGame::spawnEnemies() {
    enemies.clear();
    std::uniform_real_distribution<float> angle(0.f, 2.f * 3.14159265f);
    for (int i = 0; i < ENEMY_COUNT; ++i) {
        Vector2f p = randomPoint();
        for (int attempt = 0; attempt < 200 && !isFreeSpot(p); ++attempt) {
            p = randomPoint();
        }
        enemies.emplace_back(p, angle(rng), assets);
    }
}

void HotlineGame::onEvent(const Event& event) {
    if (const auto* key = event.getIf<Event::KeyPressed>()) {
        if (key->code == Keyboard::Key::Escape) {
            engine->quit();
        } else if (key->code == Keyboard::Key::R) {
            restart();
        }
    }
}

void HotlineGame::update(float dt, const Input& input) {
    player.handleInput(input);
    player.update(dt);
    resolvePlayerCollisions();
    for (Enemy& e : enemies) {
        e.update(dt);
    }

    if (input.isMouseDown(Mouse::Button::Left) && player.tryShoot()) {
        bullets.emplace_back(player.getMuzzlePosition(), player.getAimDirection());
    }
    updateBullets(dt);

    // trafieni przeciwnicy znikaja
    std::erase_if(enemies, [](const Enemy& e) { return !e.isAlive(); });
    if (static_cast<int>(enemies.size()) != shownEnemyCount) {
        updateTitle();
    }
}

// przeciwnicy sa twardzi: gracz nie moze przez nich przejsc (wypychanie z kola)
void HotlineGame::resolvePlayerCollisions() {
    for (const Enemy& e : enemies) {
        const Vector2f d = player.getPosition() - e.getPosition();
        const float dist = std::hypot(d.x, d.y);
        const float minDist = player.getRadius() + e.getRadius();
        if (dist < minDist && dist > 0.f) {
            player.setPosition(e.getPosition() + d / dist * minDist);
        }
    }
}

void HotlineGame::updateBullets(float dt) {
    for (Bullet& b : bullets) {
        b.update(dt);
        for (Enemy& e : enemies) {
            if (e.isAlive() && Geometry::segmentIntersectsCircle(b.getPreviousPosition(), b.getPosition(),
                                                                 e.getPosition(), e.getRadius() + b.getRadius())) {
                e.kill();
                b.setActive(false); // pocisk sie zuzywa
                break;
            }
        }
        const Vector2f p = b.getPosition();
        if (p.x < 0.f || p.y < 0.f || p.x >= static_cast<float>(size.x) || p.y >= static_cast<float>(size.y)) {
            b.setActive(false); // poza ekranem
        }
    }
    std::erase_if(bullets, [](const Bullet& b) { return !b.isActive(); });
}

void HotlineGame::updateTitle() {
    shownEnemyCount = static_cast<int>(enemies.size());
    std::string title =
        "Hotline Demo | Wrogowie: " + std::to_string(shownEnemyCount) + "/" + std::to_string(ENEMY_COUNT);
    title +=
        shownEnemyCount == 0 ? " | Wszyscy wyeliminowani - R: nowa runda" : " | LPM strzal, R restart, F1/Tab sandbox, Esc wyjscie";
    engine->setTitle(title);
}

void HotlineGame::onResize(Vector2u oldSize, Vector2u newSize) {
    // zachowujemy wzgledne polozenie obiektow w nowej rozdzielczosci
    const float kx = static_cast<float>(newSize.x) / static_cast<float>(oldSize.x);
    const float ky = static_cast<float>(newSize.y) / static_cast<float>(oldSize.y);
    size = newSize;
    player.setBounds(newSize);
    player.setPosition({player.getPosition().x * kx, player.getPosition().y * ky});
    for (Enemy& e : enemies) {
        e.setPosition({e.getPosition().x * kx, e.getPosition().y * ky});
    }
    bullets.clear(); // pociski zyja ulamek sekundy
}

void HotlineGame::drawGrid(Image& frame) const {
    const Color gridColor(36, 36, 46);
    for (unsigned int x = 0; x < size.x; x += GRID_STEP) {
        PrimitiveRenderer::renderLine(frame, x, 0, x, size.y - 1, gridColor);
    }
    for (unsigned int y = 0; y < size.y; y += GRID_STEP) {
        PrimitiveRenderer::renderLine(frame, 0, y, size.x - 1, y, gridColor);
    }
}

void HotlineGame::draw(Image& frame) {
    drawGrid(frame);
    for (Enemy& e : enemies) {
        e.draw(frame);
    }
    player.draw(frame);         // gracz razem z czerwonym celownikiem
    for (Bullet& b : bullets) { // pociski na wierzchu - leca wzdluz celownika
        b.draw(frame);
    }

    // licznik pozostalych przeciwnikow w rogu (kropki)
    for (int i = 0; i < static_cast<int>(enemies.size()); ++i) {
        PrimitiveRenderer::fillCircle(frame, 18 + i * 18, 18, 5, Color(255, 170, 40));
    }
}

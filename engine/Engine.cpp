#include "Engine.h"
#include "Logger.h"

namespace {
    const Vector2u MIN_RESOLUTION{400,300};
    const Vector2u MAX_RESOLUTION{1080,1920};
    const array<Vector2u, 6> PRESETS{{{640, 480}, {800, 600}, {1024, 768}, {1280, 720}, {1600, 900}, {1920, 1080}}};
    constexpr float MAX_FRAME_TIME = 0.25f;
    unsigned long long area(Vector2u v) {
        return static_cast<unsigned long long>(v.x) * v.y;
    }
    Vector2u clampResolution(Vector2u r) {
        return {clamp(r.x, MIN_RESOLUTION.x, MAX_RESOLUTION.x), clamp(r.y, MIN_RESOLUTION.y, MAX_RESOLUTION.y)};
    }
}

Engine::Engine(const EngineSettings& settings)
    : settings(settings),
      resolution_(clampResolution(settings.resolution)),
      fullscreen_(settings.fullscreen),
      title_(settings.title) {}

bool Engine::createWindow() {
    if (window.isOpen()) {
        window.close();
    }
    if (fullscreen_) {
        window.create(VideoMode::getDesktopMode(), title_, State::Fullscreen);
    } else {
        window.create(VideoMode(resolution_), title_, Style::Default, State::Windowed);
    }
    if (!window.isOpen()) {
        Logger::error(string("Nie udalo sie utworzyc okna (")+ (fullscreen_ ? "pelny ekran" : "okno"));
        if (fullscreen_) {
            Logger::warning("Powrot do trybu okienkowego");
            fullscreen_ = false;
            return createWindow();
        }
        return false;
    }
    window.setKeyRepeatEnabled(false);
    if (settings.fpsLimit>0) {
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(settings.fpsLimit);
    } else {
        window.setFramerateLimit(settings.fpsLimit);
        window.setVerticalSyncEnabled(settings.vsync);
    }
    input_.clear();
    updateView();
    return true;
}

bool Engine::rebuildFrame() {
    if (!texture.resize(resolution_)) {
        Logger::error(string("Nie mozna utworzyc tekstury "+ to_string(resolution_.x)+"x"+ to_string(resolution_.y)));
        return false;
    }
    blank = Image(resolution_, bgColor);
    frame = blank;
    texture.update(frame);
    sprite.setTexture(texture, true);
    return true;
}

void Engine::updateView() {
    if (!window.isOpen()) return;
    const Vector2f winSize(window.getSize());
    if (winSize.x <= 0.f || winSize.y <= 0.f) return;

    const Vector2f resSize(resolution_);
    const float winRatio = winSize.x / winSize.y;
    const float resRatio = resSize.x / resSize.y;
    float w = 1.f, h = 1.f, left = 0.f, top = 0.f;
    if (winRatio > resRatio) {
        w = resRatio / winRatio;
        left = (1.f - w) / 2.f;
    } else if (winRatio < resRatio) {
        h = winRatio / resRatio;
        top = (1.f - h) / 2.f;
    }
    View view(FloatRect({0.f, 0.f}, resSize));
    view.setViewport(FloatRect({left, top}, {w, h}));
    window.setView(view);
}

void Engine::setResolution(Vector2u resolution) {
    resolution = clampResolution(resolution);
    if (resolution == resolution_) {
        return;
    }
    const Vector2u old = resolution_;
    resolution_ = resolution;
    if (!window.isOpen()) {
        return;
    }
    if (!rebuildFrame()) {
        Logger::warning("Zostaje poprzednia rozdzielczosc");
        resolution_ = old;
        rebuildFrame();
        return;
    }
    if (!fullscreen_) {
        window.setSize(resolution_);
    }
    updateView();
    Logger::info("Rozdzielczosc: " + to_string(resolution_.x) + "x" + to_string(resolution_.y));
    if (app) {
        app->onResize(old, resolution_);
    }
}

void Engine::stepResolution(int direction) {
    if (direction > 0) {
        for (const Vector2u& p : PRESETS) {
            if (area(p) > area(resolution_)) {
                setResolution(p);
                return;
            }
        }
    } else {
        for (auto it = PRESETS.rbegin(); it != PRESETS.rend(); ++it) {
            if (area(*it) < area(resolution_)) {
                setResolution(*it);
                return;
            }
        }
    }
}

void Engine::setFullscreen(bool fullscreen) {
    if (fullscreen == fullscreen_) {
        return;
    }
    fullscreen_ = fullscreen;
    if (!window.isOpen()) {
        return;
    }
    if (!createWindow()) {
        throw runtime_error("Nie mozna ponownie utworzyc okna po zmianie trybu");
    }
    Logger::info(fullscreen_ ? "Tryb: pelny ekran" : "Tryb: okno");
}

void Engine::setTitle(const string title) {
    title_ = title;
    if (window.isOpen()) {
        window.setTitle(title_);
    }
}

void Engine::quit() {
    running = false;
}

Vector2f Engine::toFrameCoords(Vector2i pos) {
    Vector2f p = window.mapPixelToCoords(pos);
    p.x = clamp(p.x, 0.f, static_cast<float>(resolution_.x) - 1.f);
    p.y = clamp(p.y, 0.f, static_cast<float>(resolution_.y) - 1.f);
    return p;
}

Vector2f Engine::mousePosition() {
    if (!window.isOpen()) {
        return {0.f, 0.f};
    }
    return toFrameCoords(Mouse::getPosition(window));
}

void Engine::processInput() {
    while (const optional event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        } else if (event->is<Event::Resized>()) {
            updateView();
        } else if (event->is<Event::FocusLost>()) {
            input_.clear();
        } else if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
            input_.setKey(keyPressed->code, true);
            if (keyPressed->code == Keyboard::Key::F11 || (keyPressed->alt && keyPressed->code == Keyboard::Key::Enter)) {
                toggleFullscreen();
            } else if (keyPressed->code == Keyboard::Key::F9) {
                previousResolution();
            } else if (keyPressed->code == Keyboard::Key::F10) {
                nextResolution();
            }
        } else if (const auto* keyReleased = event->getIf<Event::KeyReleased>()) {
            input_.setKey(keyReleased->code, false);
        } else if (const auto* mousePressed = event->getIf<Event::MouseButtonPressed>()) {
            input_.setMouseButton(mousePressed->button, true);
        } else if (const auto* mouseReleased = event->getIf<Event::MouseButtonReleased>()) {
            input_.setMouseButton(mouseReleased->button, false);
        }
        if (window.isOpen() && app) {
            app->onEvent(*event);
        }
    }
}

void Engine::render() {
    const Color wanted = app->backgroundColor();
    if (wanted != bgColor) {
        bgColor = wanted;
        blank = Image(resolution_, bgColor);
    }
    frame = blank;
    app->draw(frame);
    texture.update(frame);

    window.clear(Color::Black);
    window.draw(sprite);
    window.display();
}

int Engine::run(Application& application) {
    Logger::init();
    try {
        app = &application;
        running = true;
        if (!createWindow()) {
            app = nullptr;
            return 1;
        }
        bgColor = app->backgroundColor();
        if (!rebuildFrame()) {
            throw runtime_error("Nie mozna utworzyc bufora obrazu");
        }
        updateView();
        app->onStart(*this);

        const float step = 1.f / settings.updatePerSecond;
        float accumulator = 0.f;
        Clock clock;                        // czasomierz petli glownej
        while (window.isOpen() && running) {
            accumulator += min(clock.restart().asSeconds(), MAX_FRAME_TIME);

            processInput();
            input_.mouse = mousePosition();

            while (accumulator >= step) {   // logika zawsze ze stalym dt
                app->update(step, input_);
                accumulator -= step;
            }
            render();
        }
        if (window.isOpen()) {
            window.close();
        }
    } catch (const exception& e) {
        Logger::error(string("Blad krytyczny: ") + e.what());
        if (window.isOpen()) {
            window.close();
        }
        app = nullptr;
        return 1;
    }
    app = nullptr;
    return 0;
}
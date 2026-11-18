#include "AppSwitcher.h"

#include "Engine.h"
#include "Logger.h"

void AppSwitcher::add(const std::string& name, Application& app) {
    entries.push_back({name, &app, false, {}});
}

void AppSwitcher::setInitial(std::size_t index) {
    if (!started && index < entries.size()) {
        current = index;
    }
}

void AppSwitcher::activate(std::size_t index) {
    if (index >= entries.size() || !engine) {
        return;
    }
    Entry& e = entries[index];
    const sf::Vector2u size = engine->getResolution();
    if (!e.started) {
        e.started = true;
        e.app->onStart(*engine);
    } else if (e.lastSize != size) {
        e.app->onResize(e.lastSize, size);   // rozdzielczość zmieniła się, gdy aplikacja była nieaktywna
    }
    e.lastSize = size;
    current = index;
    Logger::info("Aktywna aplikacja: " + e.name);
    e.app->onActivate();
}

void AppSwitcher::onStart(Engine& e) {
    engine = &e;
    started = true;
    if (entries.empty()) {
        Logger::error("AppSwitcher: brak zarejestrowanych aplikacji");
        e.quit();
        return;
    }
    activate(current);
}

void AppSwitcher::onEvent(const sf::Event& event) {
    if (entries.empty()) {
        return;
    }
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        using K = sf::Keyboard::Key;
        std::size_t target = current;
        bool change = false;
        if (key->code == K::Tab) {
            target = (current + 1) % entries.size();
            change = true;
        } else if (key->code >= K::F1 && key->code <= K::F3) {
            target = static_cast<std::size_t>(key->code) - static_cast<std::size_t>(K::F1);
            change = true;
        }
        if (change) {
            if (target != current && target < entries.size()) {
                entries[current].app->onDeactivate();
                activate(target);
            }
            return;   // klawisze przełączania nie trafiają do aplikacji
        }
    }
    entries[current].app->onEvent(event);
}

void AppSwitcher::update(float dt, const Input& input) {
    if (!entries.empty()) {
        entries[current].app->update(dt, input);
    }
}

void AppSwitcher::draw(sf::Image& frame) {
    if (!entries.empty()) {
        entries[current].app->draw(frame);
    }
}

void AppSwitcher::onResize(sf::Vector2u oldSize, sf::Vector2u newSize) {
    if (entries.empty()) {
        return;
    }
    entries[current].app->onResize(oldSize, newSize);
    entries[current].lastSize = newSize;
}

sf::Color AppSwitcher::backgroundColor() const {
    return entries.empty() ? sf::Color::Black : entries[current].app->backgroundColor();
}

#include "SandboxApp.h"
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/Engine.h"
#include "engine/Logger.h"
#include "engine/PrimitiveRenderer.h"
#include "engine/Rectangle.h"
#include "engine/Square.h"
void SandboxApp::onStart(Engine &engine) {
    this->engine = &engine;
    clearCanvas(Color::Black);
    cout << "Sterowanie: LPM piksel | L linia | C lamana | G wielokat | S kwadrat | R prostokat | P okrag | E elipsa | F wypelnij" << endl
         << "            0-4 czyszczenie | F9/F10 rozdzielczosc | F11 pelny ekran | Esc wyjscie" << endl
         << "            F1 sandbox | F2 / Tab - demo gry" << endl;
}

void SandboxApp::onActivate() {
    engine->setTitle("Silnik 2D - sandbox | F2/Tab: demo gry");
}

void SandboxApp::clearCanvas(Color color) {
    bgColor = color;
    canvas = Image(engine->getResolution(), color);
}

void SandboxApp::onResize(Vector2u oldSize, Vector2u newSize) {
    Image resized(newSize, bgColor);
    const Vector2i common(static_cast<int>(min(oldSize.x, newSize.x)), static_cast<int>(min(oldSize.y, newSize.y)));
    if (!resized.copy(canvas, {0, 0}, IntRect({0, 0}, common))) {
        Logger::warning("Nie udalo sie przeniesc rysunku do nowej rozdzielczosci");
    }
    canvas = std::move(resized);
}

void SandboxApp::onEvent(const Event& event) {
    if (const auto* mouse = event.getIf<Event::MouseButtonPressed>()) {
        if (mouse->button == Mouse::Button::Left) {
            const Vector2f p = engine->toFrameCoords(mouse->position);
            handleClick(p.x, p.y);
        }
    }
    if (const auto* key = event.getIf<Event::KeyPressed>()) {
        handleKey(key->code);
    }
    if (event.is<Event::Closed>()) {
        cout << endl << "Goodnight!" << endl;
    }
}

void SandboxApp::handleClick(unsigned int x, unsigned int y) {
    switch (mode) {
        case Mode::Line:
            if (hasFirstPoint) {
                segment.setPoints(firstX, firstY, x, y);
                segment.draw(canvas);
                cout << "Narysowano linie miedzy punktami (" << segment.getStartX() << ", " << segment.getStartY() << ") a (" << segment.getEndX() << ", " << segment.getEndY() << ")" << endl;
                mode = Mode::Point;
                hasFirstPoint = false;
            } else {
                firstX = x;
                firstY = y;
                hasFirstPoint = true;
            }
            break;
        case Mode::Polyline:
            segmentedLine.addPoint(Point2D(x, y));
            break;
        case Mode::Polygon:
            if (!polygon.isClosed()) {
                if (polygon.getPointCount() >= 3) {
                    polygon.close();
                }
            }
            polygon.addPoint(Point2D(x, y));
            break;
        default:
            point.setPoint(x, y);
            point.draw(canvas);
            cout << "Umieszczono piksel na pozycji: (" << point.getX() << ", " << point.getY() << ")" << endl;
    }
}

void SandboxApp::toggleLine() {
    if (mode == Mode::Line) {
        mode = Mode::Point;
        hasFirstPoint = false;
        cout << "Wylaczono tryb rysowania linii." << endl;
    } else if (mode == Mode::Point) {
        mode = Mode::Line;
        cout << "Wlaczono tryb rysowania linii." << endl;
    }
}

void SandboxApp::togglePolyline() {
    if (mode == Mode::Polyline) {
        segmentedLine.draw(canvas);
        segmentedLine.clear();
        mode = Mode::Point;
        cout << "Wylaczono tryb rysowania lamanej." << endl;
    } else if (mode == Mode::Point) {
        mode = Mode::Polyline;
        cout << "Wlaczono tryb rysowania lamanej." << endl;
    }
}

void SandboxApp::togglePolygon() {
    if (mode == Mode::Point) {
        mode = Mode::Polygon;
        cout << "Wlaczono tryb rysowania wielokata (G - zakoncz, Backspace - cofnij, Delete - anuluj)." << endl;
    } else if (mode == Mode::Polygon) {
        if (polygon.getPointCount() == 0) {
            mode = Mode::Point;
            cout << "Wylaczono tryb rysowania wielokata." << endl;
        } else if (polygon.canClose()) {
            polygon.draw(canvas);
            cout << "Narysowano wielokat (" << polygon.getPointCount() << " wierzcholkow)." << endl;
            polygon.clear();
            mode = Mode::Point;
        } else if (polygon.getPointCount() < 3) {
            cout << "Wielokat potrzebuje co najmniej 3 punktow." << endl;
        } else {
            cout << "Nie mozna zamknac: krawedz zamykajaca przecielaby wielokat. Cofnij punkt (Backspace)." << endl;
        }
    }
}

void SandboxApp::handleKey(Keyboard::Key code) {
    if (code == Keyboard::Key::Num0) clearCanvas(Color::Black);
    if (code == Keyboard::Key::Num1) clearCanvas(Color::White);
    if (code == Keyboard::Key::Num2) clearCanvas(Color::Red);
    if (code == Keyboard::Key::Num3) clearCanvas(Color::Blue);
    if (code == Keyboard::Key::Num4) clearCanvas(Color::Green);

    if (code == Keyboard::Key::L) toggleLine();
    if (code == Keyboard::Key::C) togglePolyline();
    if (code == Keyboard::Key::G) togglePolygon();

    if (code == Keyboard::Key::Backspace) {
        if (mode == Mode::Polygon) polygon.removeLastPoint();
        if (mode == Mode::Polyline) segmentedLine.removeLastPoint();
    }
    if (code == Keyboard::Key::Delete && mode == Mode::Polygon) {
        polygon.clear();
        mode = Mode::Point;
        cout << "Anulowano wielokat." << endl;
    }

    const Vector2f cursor = engine->mousePosition();
    const unsigned int mx = cursor.x, my = cursor.y;

    //rysowanie kwadratu
    if (code == Keyboard::Key::S) {
        Square square = Square(mx, my, 50, Color::White);
        square.draw(canvas);
    }
    //rysowanie prostakata
    if (code == Keyboard::Key::R) {
        Rectangle rectangle = Rectangle(mx, my, 50, 100, Color::White);
        rectangle.draw(canvas);
    }
    //rysowanie kola
    if (code == Keyboard::Key::P) {
        PrimitiveRenderer::renderCircle(canvas, mx, my, 40, Color::White);
    }
    //rysowanie elipsy
    if (code == Keyboard::Key::E) {
        PrimitiveRenderer::renderElipsis(canvas, mx, my, 40, 60, Color::White);
    }
    //wypełnianie
    if (code == Keyboard::Key::F) {
        PrimitiveRenderer::floodFill(canvas, mx, my, Color::White);
    }

    if (code == Keyboard::Key::Escape) {
        cout << endl << "Bye!" << endl;
        engine->quit();
    }
}

void SandboxApp::drawPreview(Image& frame) {
    const Vector2f cursor = engine->mousePosition();
    const unsigned int mx = cursor.x, my = cursor.y;

    LineSegment rubber;
    rubber.setColor(Color(120, 120, 120));

    switch (mode) {
        case Mode::Line:
            if (hasFirstPoint) {
                rubber.setPoints(firstX, firstY, mx, my);
                rubber.draw(frame);
            }
            break;
        case Mode::Polyline:
            segmentedLine.draw(frame);
            if (segmentedLine.getPointCount() > 0) {
                Point2D last = segmentedLine.getPoints().back();
                rubber.setPoints(last.getX(), last.getY(), mx, my);
                rubber.draw(frame);
            }
            break;
        case Mode::Polygon:
            polygon.draw(frame);
            if (polygon.getPointCount() > 0) {
                Point2D last = polygon.getPoints().back();
                rubber.setPoints(last.getX(), last.getY(), mx, my);
                rubber.setColor(polygon.canAddPoint(Point2D(mx, my)) ? Color(80, 220, 120) : Color::Red);
                rubber.draw(frame);
            }
            break;
        default:
            break;
    }
}

void SandboxApp::draw(Image& frame) {
    frame = canvas;
    drawPreview(frame);
}
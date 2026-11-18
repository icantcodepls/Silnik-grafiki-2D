#include "Point2D.h"

#include <cmath>
#include <SFML/Graphics.hpp>
#include "PrimitiveRenderer.h"

using namespace sf;

namespace {
    // wspolrzedne sa bez znaku: wynik obliczen zaokraglamy i obcinamy do 0 (zamiast zawijac do ~4 mld)
    unsigned int toCoord(double v) {
        return v <= 0.0 ? 0u : static_cast<unsigned int>(std::lround(v));
    }
}

Point2D::Point2D(unsigned int x, unsigned int y) : x(x), y(y) {}

void Point2D::setPoint(unsigned int x, unsigned int y) {
    this->x = x;
    this->y = y;
}

unsigned int Point2D::getX() const {
    return x;
}

unsigned int Point2D::getY() const {
    return y;
}

void Point2D::draw(Image& bufor) {
    PrimitiveRenderer::renderPoint(bufor, x, y, color);
}

void Point2D::translate(int dx, int dy) {
    this->x = toCoord(static_cast<double>(this->x) + dx);
    this->y = toCoord(static_cast<double>(this->y) + dy);
}

void Point2D::scale(unsigned int cx, unsigned int cy, float k) {
    this->x = toCoord(cx + (static_cast<double>(this->x) - cx) * k);
    this->y = toCoord(cy + (static_cast<double>(this->y) - cy) * k);
}

void Point2D::rotate(unsigned int cx, unsigned int cy, float angle) {
    // roznice liczymy na liczbach ze znakiem - przy unsigned "this->x - cx" zawija sie do ~4 mld
    const double dx = static_cast<double>(this->x) - cx;
    const double dy = static_cast<double>(this->y) - cy;
    const double c = std::cos(angle), s = std::sin(angle);
    this->x = toCoord(cx + dx * c - dy * s);
    this->y = toCoord(cy + dx * s + dy * c);
}

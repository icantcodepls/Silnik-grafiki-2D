#include "Rectangle.h"

#include "PrimitiveRenderer.h"

Rectangle::Rectangle(unsigned int x, unsigned int y, unsigned int h, unsigned int w, Color color) {
    origin.setPoint(x, y);
    h/=2;
    w/=2;
    Point2D temp;
    temp.setPoint(x-w, y-h);
    sides.addPoint(temp);
    temp.setPoint(x+w, y-h);
    sides.addPoint(temp);
    temp.setPoint(x+w, y+h);
    sides.addPoint(temp);
    temp.setPoint(x-w, y+h);
    sides.addPoint(temp);
    this->color=color;
    sides.setColor(color);
}


void Rectangle::draw(Image &bufor) {
    sides.draw(bufor);
    PrimitiveRenderer::floodFill(bufor, origin.getX(), origin.getY(), color);
}

void Rectangle::translate(int dx, int dy) {
    origin.translate(dx, dy);
    sides.translate(dx, dy);
}

void Rectangle::rotate(unsigned int x, unsigned int y, float angle) {
    origin.rotate(x, y, angle);
    sides.rotate(x, y, angle);
}

void Rectangle::scale(unsigned int x, unsigned int y, float k) {
    origin.scale(x, y, k);
    sides.scale(x, y, k);
}

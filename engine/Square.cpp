#include "Square.h"

#include "PrimitiveRenderer.h"

Square::Square(unsigned int x, unsigned int y, unsigned int s, Color color) {
    origin.setPoint(x, y);
    s/=2;
    Point2D temp;
    temp.setPoint(x-s, y-s);
    sides.addPoint(temp);
    temp.setPoint(x+s, y-s);
    sides.addPoint(temp);
    temp.setPoint(x+s, y+s);
    sides.addPoint(temp);
    temp.setPoint(x-s, y+s);
    sides.addPoint(temp);
    this->color=color;
    sides.setColor(color);
}

void Square::draw(Image &bufor) {
    sides.draw(bufor);
    PrimitiveRenderer::floodFill(bufor, origin.getX(), origin.getY(), color);

}

void Square::translate(int dx, int dy) {
    origin.translate(dx, dy);
    sides.translate(dx, dy);
}

void Square::rotate(unsigned int x, unsigned int y, float angle) {
    origin.rotate(x, y, angle);
    sides.rotate(x, y, angle);
}

void Square::scale(unsigned int x, unsigned int y, float k) {
    origin.scale(x, y, k);
    sides.scale(x, y, k);
}

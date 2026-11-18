#include "LineSegment.h"
#include "PrimitiveRenderer.h"

using namespace sf;

void LineSegment::setPoints(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    p1.setPoint(x1,y1);
    p2.setPoint(x2,y2);
}

unsigned int LineSegment::getStartX() {
    return p1.getX();
}

unsigned int LineSegment::getStartY() {
    return p1.getY();
}

unsigned int LineSegment::getEndX() {
    return p2.getX();
}

unsigned int LineSegment::getEndY() {
    return p2.getY();
}

void LineSegment::draw(Image& bufor) {
    PrimitiveRenderer::renderLine(bufor,p1.getX(),p1.getY(),p2.getX(),p2.getY(), color);
}
void LineSegment::translate(int dx, int dy) {
    p1.translate(dx,dy);
    p2.translate(dx,dy);
}

void LineSegment::scale(unsigned int x, unsigned int y, float k) {
    p1.scale(x,y,k);
    p2.scale(x,y,k);
}

void LineSegment::rotate(unsigned int x, unsigned int y, float angle) {
    p1.rotate(x,y,angle);
    p2.rotate(x,y,angle);
}
#include "segmentedLine.h"

#include "PrimitiveRenderer.h"

using namespace std;
using namespace sf;

void SegmentedLine::addPoint(Point2D point) {
    points.push_back(point);
}

void SegmentedLine::removeLastPoint() {
    if (!points.empty()) {
        points.pop_back();
    }
}

void SegmentedLine::clear() {
    points.clear();
}

void SegmentedLine::draw(Image& bufor) {
    if (points.empty()) {
        return;
    }
    if (points.size()==1) {
        points.front().setColor(color);
        points.front().draw(bufor);
        return;
    }
    vector<Point2D>::iterator it=points.begin();
    unsigned int x2=it->getX();
    unsigned int y2=it->getY();
    ++it;
    while(it!=points.end()) {
        unsigned int x1=x2;
        unsigned int y1=y2;
        x2=it->getX();
        y2=it->getY();
        PrimitiveRenderer::renderLine(bufor,x1,y1,x2,y2, color);
        ++it;
    }
}

void SegmentedLine::translate(int dx, int dy) {
    for (Point2D& p : points) {
        p.translate(dx,dy);
    }
}

void SegmentedLine::scale(unsigned int x, unsigned int y, float k) {
    for (Point2D& p : points) {
        p.scale(x,y,k);
    }
}

void SegmentedLine::rotate(unsigned int x, unsigned int y, float angle) {
    for (Point2D& p : points) {
        p.rotate(x,y,angle);
    }
}

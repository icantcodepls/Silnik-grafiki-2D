#include "ClosedLine.h"

#include "PrimitiveRenderer.h"

void ClosedLine::draw(Image &bufor) {
    setColor(color);
    SegmentedLine::draw(bufor);
    if (points.size()>2) {
        PrimitiveRenderer::renderLine(bufor,points.back().getX(), points.back().getY(), points.front().getX(), points.front().getY(), color);
    }
}

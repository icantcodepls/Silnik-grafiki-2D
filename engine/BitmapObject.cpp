#include "BitmapObject.h"

#include <cmath>

BitmapObject::BitmapObject(BitmapHandler &handler) : handler(&handler) {
    setName("BitmapoBcect");
}

size_t BitmapObject::addPart(BitmapId bitmap, Vector2u offset, const IntRect &source, const string &name) {
    BitmapPart part;
    part.bitmap = bitmap;
    part.offset = offset;
    part.source = source;
    part.name = name;
    parts.push_back(part);
    return parts.size() - 1;
}

void BitmapObject::draw(Image &bufor) {
    for (const BitmapPart &part : parts) {
        const Image* image = handler->get(part.bitmap);
        if (!image) continue;
        const Vector2f scaledOffset(part.offset.x * scaleFactor, part.offset.y * scaleFactor);
        const float c = cos(rotation), s = sin(rotation);
        // Przesuniecie czesci po obrocie moze byc ujemne - liczymy na float i dopiero sume zamieniamy
        // na wspolrzedna (rzutowanie ujemnego floata na unsigned jest niezdefiniowane).
        const float cx = static_cast<float>(position.x) + scaledOffset.x*c - scaledOffset.y*s;
        const float cy = static_cast<float>(position.y) + scaledOffset.x*s + scaledOffset.y*c;
        const Vector2u center = {static_cast<unsigned int>(max(0L, lround(cx))), static_cast<unsigned int>(max(0L, lround(cy)))};
        BitmapHandler::blitTransformed(*image, bufor, center, rotation + part.rotation, {scaleFactor * part.scale.x, scaleFactor * part.scale.y}, part.source);
    }
}

void BitmapObject::translate(int dx, int dy) {
    position.x += dx;
    position.y += dy;
}

void BitmapObject::scale(float k) {
    scaleFactor = k;
}

void BitmapObject::rotate(float angle) {
    rotation = angle;
}

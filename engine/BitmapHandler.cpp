#include "BitmapHandler.h"

#include <cmath>
#include <cstring>

#include "PrimitiveRenderer.h"

BitmapId BitmapHandler::store(unique_ptr<Image> image) {
    const BitmapId id = nextId++;
    bitmaps[id] = move(image);
    return id;
}

BitmapId BitmapHandler::create(unsigned int w, unsigned int h, Color fill) {
    if (w==0 || h==0) return INVALID_BITMAP;
    return store(make_unique<Image>(Vector2u(w,h), fill));
}

BitmapId BitmapHandler::createFromImage(const Image &image) {
    return store(make_unique<Image>(image));
}

BitmapId BitmapHandler::load(const string &filename) {
    auto image = make_unique<Image>();
    if (!image->loadFromFile(filename)) return INVALID_BITMAP;
    return store(move(image));
}

bool BitmapHandler::save(BitmapId id, const string &filename) {
    const Image* image = get(id);
    if (image==nullptr) return false;
    return image->saveToFile(filename);
}

bool BitmapHandler::destroy(BitmapId id) {
    return bitmaps.erase(id) > 0;
}
void BitmapHandler::destroyAll() {
    bitmaps.clear();
}
bool BitmapHandler::exists(BitmapId id) {
    return bitmaps.find(id) != bitmaps.end();
}

Image *BitmapHandler::get(BitmapId id) {
    const auto it=bitmaps.find(id);
    if (it==bitmaps.end()) return nullptr;
    return it->second.get();
}

Vector2u BitmapHandler::getSize(BitmapId id) {
    const Image* image = get(id);
    if (image==nullptr) return Vector2u(0,0);
    return image->getSize();
}


BitmapId BitmapHandler::copy(BitmapId id) {
    const Image* image = get(id);
    if (image==nullptr) return INVALID_BITMAP;
    return createFromImage(*image);
}

BitmapId BitmapHandler::crop(BitmapId id, const IntRect &rect) {
    const Image* image = get(id);
    if (image==nullptr || rect.size.x<=0 || rect.size.y<=0) return INVALID_BITMAP;
    const BitmapId newBitmap = create(static_cast<unsigned int>(rect.size.x), static_cast<unsigned int>(rect.size.y));
    if (newBitmap==INVALID_BITMAP) return INVALID_BITMAP;
    blit(*image, *get(newBitmap), {0,0}, rect);
    return newBitmap;
}


void BitmapHandler::fill(BitmapId id, Color fill) {
    if (Image *image = get(id)) {
        PrimitiveRenderer::clear(*image, fill);
    }
}

void BitmapHandler::flipHorizontal(BitmapId id) {
    if (Image *image = get(id)) {image->flipHorizontally();}
}

void BitmapHandler::flipVertical(BitmapId id) {
    if (Image *image = get(id)) {image->flipVertically();}
}

namespace {
    // Prostokat zrodlowy przyciety do obrazu o rozmiarze size. Pusty prostokat = caly obraz.
    // Zwraca polozenie i rozmiar PO przycieciu (rozmiar moze byc 0, gdy nic nie zostalo).
    IntRect resolveSourceRect(const IntRect& rect, Vector2u size) {
        IntRect result=rect;
        if (result.size.x <= 0 || result.size.y <= 0) result = IntRect({0, 0}, {static_cast<int>(size.x), static_cast<int>(size.y)});
        const int x0 = max(result.position.x, 0), y0 = max(result.position.y, 0);
        const int x1 = min(result.position.x + result.size.x, static_cast<int>(size.x));
        const int y1 = min(result.position.y + result.size.y, static_cast<int>(size.y));
        return IntRect({x0, y0}, {max(0, x1 - x0), max(0, y1 - y0)});
    }
}

void BitmapHandler::blit(const Image &src, Image &dst, Vector2i dstPos, const IntRect &rect, bool alphaBlend) {
    const Vector2u ss = src.getSize(), ds = dst.getSize();
    if (ss.x==0 || ss.y==0 || ds.x==0 || ds.y==0) return;
    IntRect srcRect = rect;
    if (srcRect.size.x<=0 || srcRect.size.y<=0) srcRect = IntRect({0,0}, {static_cast<int>(ss.x),static_cast<int>(ss.y)});
    const IntRect r = resolveSourceRect(srcRect, ss);          // wycinek przyciety do obrazu zrodlowego
    if (r.size.x<=0 || r.size.y<=0) return;
    // jesli przycieto lewa/gorna czesc wycinka, miejsce docelowe przesuwa sie o tyle samo
    int dx = dstPos.x + (r.position.x - srcRect.position.x);
    int dy = dstPos.y + (r.position.y - srcRect.position.y);
    int sx = r.position.x, sy = r.position.y, w = r.size.x, h = r.size.y;
    if (dx<0) {                                                 // wychodzi poza lewa krawedz celu
        sx-=dx;
        w+=dx;
        dx=0;
    }
    if (dy<0) {                                                 // wychodzi poza gorna krawedz celu
        sy-=dy;
        h+=dy;
        dy=0;
    }
    w=min(w,static_cast<int>(ds.x)-dx);                         // wychodzi poza prawa/dolna krawedz celu
    h=min(h,static_cast<int>(ds.y)-dy);
    if (w<=0 || h<=0) return;
    const uint8_t* sp = src.getPixelsPtr();
    uint8_t* dp = const_cast<uint8_t*>(dst.getPixelsPtr());
    for (int row = 0; row < h; ++row) {
        const uint8_t* srow = sp + 4 * (static_cast<size_t>(sy + row) * ss.x + sx);
        uint8_t* drow = dp + 4 * (static_cast<size_t>(dy + row) * ds.x + dx);
        if (!alphaBlend) {
            memmove(drow, srow, static_cast<size_t>(w) * 4);
        } else {
            for (int col = 0; col < w; ++col, srow += 4, drow += 4) {
                if (srow[3] == 0) continue;
                if (srow[3] == 255) { memcpy(drow, srow, 4); continue; }
                const Color out = PrimitiveRenderer::blend(Color(srow[0], srow[1], srow[2], srow[3]), Color(drow[0], drow[1], drow[2], drow[3]));
                drow[0] = out.r; drow[1] = out.g; drow[2] = out.b; drow[3] = out.a;
            }
        }
    }
}

void BitmapHandler::blitTransformed(const Image& src, Image& dst, Vector2u center, float angle, Vector2f scale, const IntRect& srcRect) {
    const Vector2u ss = src.getSize(), ds = dst.getSize();
    if (ss.x == 0 || ss.y == 0 || ds.x == 0 || ds.y == 0) return;
    const IntRect r = resolveSourceRect(srcRect, ss);
    if (r.size.x <= 0 || r.size.y <= 0 || scale.x == 0.f || scale.y == 0.f) return;

    const float c = std::cos(angle), s = std::sin(angle);
    const float halfW = std::fabs(static_cast<float>(r.size.x) * scale.x) * 0.5f;
    const float halfH = std::fabs(static_cast<float>(r.size.y) * scale.y) * 0.5f;
    const float ex = halfW * std::fabs(c) + halfH * std::fabs(s);   // polowa szerokosci ramki otaczajacej
    const float ey = halfW * std::fabs(s) + halfH * std::fabs(c);
    const int x0 = std::max(0, static_cast<int>(std::floor(center.x - ex)));
    const int x1 = std::min(static_cast<int>(ds.x) - 1, static_cast<int>(std::ceil(center.x + ex)));
    const int y0 = std::max(0, static_cast<int>(std::floor(center.y - ey)));
    const int y1 = std::min(static_cast<int>(ds.y) - 1, static_cast<int>(std::ceil(center.y + ey)));

    const std::uint8_t* sp = src.getPixelsPtr();
    std::uint8_t* dp = const_cast<std::uint8_t*>(dst.getPixelsPtr());
    const float halfSrcW = static_cast<float>(r.size.x) * 0.5f, halfSrcH = static_cast<float>(r.size.y) * 0.5f;

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            const float px = static_cast<float>(x) + 0.5f - center.x;
            const float py = static_cast<float>(y) + 0.5f - center.y;
            const float u = (px * c + py * s) / scale.x + halfSrcW;     // R(-angle) * p
            const float v = (-px * s + py * c) / scale.y + halfSrcH;
            const int iu = static_cast<int>(std::floor(u)), iv = static_cast<int>(std::floor(v));
            if (iu < 0 || iv < 0 || iu >= r.size.x || iv >= r.size.y) continue;
            const std::uint8_t* sPix = sp + 4 * (static_cast<std::size_t>(r.position.y + iv) * ss.x + (r.position.x + iu));
            if (sPix[3] == 0) continue;
            std::uint8_t* dPix = dp + 4 * (static_cast<std::size_t>(y) * ds.x + x);
            if (sPix[3] == 255) {
                std::memcpy(dPix, sPix, 4);
            } else {
                const Color out = PrimitiveRenderer::blend(Color(sPix[0], sPix[1], sPix[2], sPix[3]), Color(dPix[0], dPix[1], dPix[2], dPix[3]));
                dPix[0] = out.r; dPix[1] = out.g; dPix[2] = out.b; dPix[3] = out.a;
            }
        }
    }
}
#include "Polygon.h"

#include <algorithm>
#include <cmath>
#include <SFML/Graphics/Image.hpp>
#include "PrimitiveRenderer.h"

using namespace std;
using namespace sf;

namespace {
    struct V { long long x, y; };

    V toV(const Point2D& p) { return {static_cast<long long>(p.getX()), static_cast<long long>(p.getY())}; }

    long long cross(V a, V b, V c) { return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x); }

    bool onSegment(V a, V b, V p) {
        return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
               min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
    }

    bool segmentsIntersect(V a, V b, V c, V d) {
        const long long d1 = cross(a, b, c), d2 = cross(a, b, d);
        const long long d3 = cross(c, d, a), d4 = cross(c, d, b);
        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
            ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return true;
        if (d1 == 0 && onSegment(a, b, c)) return true;
        if (d2 == 0 && onSegment(a, b, d)) return true;
        if (d3 == 0 && onSegment(c, d, a)) return true;
        if (d4 == 0 && onSegment(c, d, b)) return true;
        return false;
    }

    bool overlapAtSharedVertex(V s, V p, V q) {
        return cross(s, p, q) == 0 && (p.x - s.x) * (q.x - s.x) + (p.y - s.y) * (q.y - s.y) > 0;
    }

    bool isSimple(const vector<Point2D>& points, bool closed) {
        const size_t n = points.size();
        if (n < 2) return true;
        vector<V> p;
        p.reserve(n);
        for (const Point2D& pt : points) p.push_back(toV(pt));

        const size_t edges = closed ? n : n - 1;
        auto from = [&](size_t i) { return p[i]; };
        auto to   = [&](size_t i) { return p[(i + 1) % n]; };

        for (size_t i = 0; i < edges; ++i) {
            if (from(i).x == to(i).x && from(i).y == to(i).y) return false;
        }
        for (size_t i = 0; i < edges; ++i) {
            for (size_t j = i + 1; j < edges; ++j) {
                if (j == i + 1) {
                    if (overlapAtSharedVertex(to(i), from(i), to(j))) return false;
                } else if (closed && i == 0 && j == edges - 1) {
                    if (overlapAtSharedVertex(from(i), to(i), from(j))) return false;
                } else if (segmentsIntersect(from(i), to(i), from(j), to(j))) {
                    return false;
                }
            }
        }
        return true;
    }

    void fillPolygon(Image& img, const vector<Point2D>& pts, Color c) {
        const size_t n = pts.size();
        if (n < 3 || c.a == 0) return;
        const Vector2u size = img.getSize();
        if (size.x == 0 || size.y == 0) return;

        vector<V> v;
        v.reserve(n);
        for (const Point2D& pt : pts) v.push_back(toV(pt));

        long long minY = v[0].y, maxY = v[0].y;
        for (const V& p : v) { minY = std::min(minY, p.y); maxY = std::max(maxY, p.y); }
        minY = std::max(minY, 0LL);
        maxY = std::min(maxY, static_cast<long long>(size.y) - 1);

        vector<double> xs;
        for (long long y = minY; y <= maxY; ++y) {
            xs.clear();
            for (size_t i = 0; i < n; ++i) {
                const V a = v[i], b = v[(i + 1) % n];
                if (a.y == b.y) continue;
                if (y < std::min(a.y, b.y) || y >= std::max(a.y, b.y)) continue;
                const double t = ((y + 0.5) - a.y) / static_cast<double>(b.y - a.y);
                xs.push_back(a.x + t * (b.x - a.x));
            }
            std::sort(xs.begin(), xs.end());
            for (size_t k = 0; k + 1 < xs.size(); k += 2) {
                long long x0 = static_cast<long long>(std::ceil(xs[k] - 0.5));
                long long x1 = static_cast<long long>(std::ceil(xs[k + 1] - 0.5)) - 1;
                x0 = std::max(x0, 0LL);
                x1 = std::min(x1, static_cast<long long>(size.x) - 1);
                for (long long x = x0; x <= x1; ++x) {
                    const Vector2u px(static_cast<unsigned>(x), static_cast<unsigned>(y));
                    img.setPixel(px, c.a == 255 ? c : PrimitiveRenderer::blend(c, img.getPixel(px)));
                }
            }
        }
    }
}

bool Polygon::canAddPoint(Point2D point) const {
    vector<Point2D> candidate = points;
    candidate.push_back(point);
    return isSimple(candidate, closed_);
}

void Polygon::addPoint(Point2D point) {
    if (canAddPoint(point)) {
        SegmentedLine::addPoint(point);
    }
}

bool Polygon::canClose() const {
    return points.size() >= 3 && isSimple(points, true);
}

bool Polygon::close() {
    if (isClosed()) return true;
    if (!canClose()) return false;
    closed_ = true;
    return true;
}

bool Polygon::isClosed() const {
    return closed_ && points.size() >= 3;
}

void Polygon::reopen() {
    closed_ = false;
}

bool Polygon::isNearFirstPoint(Point2D p, unsigned int radius) const {
    if (points.empty()) return false;
    const long long dx = static_cast<long long>(p.getX()) - static_cast<long long>(points.front().getX());
    const long long dy = static_cast<long long>(p.getY()) - static_cast<long long>(points.front().getY());
    const long long r = radius;
    return dx * dx + dy * dy <= r * r;
}

void Polygon::clear() {
    closed_ = false;
    SegmentedLine::clear();
}

void Polygon::removeLastPoint() {
    closed_ = false;
    SegmentedLine::removeLastPoint();
}

void Polygon::draw(Image &bufor) {
    if (!isClosed()) {
        SegmentedLine::draw(bufor);
        return;
    }
    if (filled_) {
        fillPolygon(bufor, points, fillColor_.value_or(color));
    }
    SegmentedLine::draw(bufor);
    const Point2D& last = points.back();
    const Point2D& first = points.front();
    PrimitiveRenderer::renderLine(bufor, last.getX(), last.getY(), first.getX(), first.getY(), color);
}
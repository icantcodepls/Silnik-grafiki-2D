#ifndef GRAFIKA2D_SEGMENTEDLINE_H
#define GRAFIKA2D_SEGMENTEDLINE_H
#include <vector>

#include "LineSegment.h"

using namespace std;
using namespace sf;

/**
 * @file segmentedLine.h
 * @brief Linia łamana (ciąg odcinków).
 */

/**
 * @brief Linia łamana złożona z kolejnych punktów połączonych odcinkami.
 */
class SegmentedLine:public ShapeObject {
protected:
    vector<Point2D> points;   ///< Wierzchołki łamanej w kolejności.
public:
    /// @brief Dodaje wierzchołek na końcu. @param point Nowy punkt.
    void virtual addPoint(Point2D point);
    /// @brief Usuwa ostatni wierzchołek (jeśli istnieje).
    void removeLastPoint();
    /// @brief Usuwa wszystkie wierzchołki.
    void virtual clear();
    /// @brief Zwraca liczbę wierzchołków.
    size_t getPointCount() const {return points.size();}
    /// @brief Zwraca wierzchołki.
    const vector<Point2D>& getPoints() const {return points;}
    /// @brief Rysuje łamaną. @param bufor Obraz docelowy.
    void draw(Image& bufor);
    /// @brief Przesuwa wszystkie wierzchołki. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    void translate(int dx, int dy) override;
    /// @brief Skaluje względem punktu (x, y). @param x X środka. @param y Y środka. @param k Współczynnik.
    void scale(unsigned int x, unsigned int y, float k) override;
    /// @brief Obraca wokół punktu (x, y). @param x X środka. @param y Y środka. @param angle Kąt (rad).
    void rotate(unsigned int x, unsigned int y, float angle) override;
};


#endif //GRAFIKA2D_SEGMENTEDLINE_H
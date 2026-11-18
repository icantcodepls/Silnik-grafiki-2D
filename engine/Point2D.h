#ifndef GRAFIKA2D_POINT2D_H
#define GRAFIKA2D_POINT2D_H
#include "ShapeObject.h"

/**
 * @file Point2D.h
 * @brief Punkt na płaszczyźnie o współrzędnych całkowitych.
 */

/**
 * @brief Punkt 2D (piksel) o współrzędnych nieujemnych.
 *
 * Współrzędne są zaokrąglane do liczb całkowitych, a wartości ujemne
 * (np. po przesunięciu lub obrocie) są obcinane do 0.
 */
class Point2D:public ShapeObject {
    unsigned int x=0, y=0;   ///< Współrzędne punktu (piksele).
public:
    /// @brief Tworzy punkt w (0, 0).
    Point2D() = default;
    /**
     * @brief Tworzy punkt o podanych współrzędnych.
     * @param x Współrzędna pozioma.
     * @param y Współrzędna pionowa.
     */
    Point2D(unsigned int x, unsigned int y);
    /**
     * @brief Ustawia współrzędne punktu.
     * @param x Współrzędna pozioma.
     * @param y Współrzędna pionowa.
     */
    void setPoint(unsigned int x, unsigned int y);
    /// @brief Zwraca współrzędną poziomą.
    unsigned int getX() const;
    /// @brief Zwraca współrzędną pionową.
    unsigned int getY() const;
    /**
     * @brief Rysuje punkt jako pojedynczy piksel (punkty poza obrazem są pomijane).
     * @param bufor Obraz docelowy.
     */
    void draw(Image &bufor) override;
    /**
     * @brief Przesuwa punkt o wektor (wynik mniejszy od 0 jest obcinany do 0).
     * @param dx Przesunięcie poziome.
     * @param dy Przesunięcie pionowe.
     */
    void translate(int dx, int dy) override;
    /**
     * @brief Skaluje punkt względem punktu (x, y) ze współczynnikiem k.
     * @param x Współrzędna X środka skalowania.
     * @param y Współrzędna Y środka skalowania.
     * @param k Współczynnik skali.
     */
    void scale(unsigned int x, unsigned int y, float k) override;
    /**
     * @brief Obraca punkt wokół punktu (x, y).
     * @param x Współrzędna X środka obrotu.
     * @param y Współrzędna Y środka obrotu.
     * @param angle Kąt w radianach (przy osi Y skierowanej w dół dodatni kąt obraca zgodnie z ruchem wskazówek zegara).
     */
    void rotate(unsigned int x, unsigned int y, float angle) override;
};

#endif //GRAFIKA2D_BITMAPHANDLER_H

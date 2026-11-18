#ifndef GRAFIKA2D_PRIMITIVERENDERER_H
#define GRAFIKA2D_PRIMITIVERENDERER_H
#include "SFML/Graphics/Color.hpp"

namespace sf { class Image; }   // wystarczy deklaracja - w naglowku uzywamy tylko referencji

/**
 * @file PrimitiveRenderer.h
 * @brief Rysowanie podstawowych figur bezpośrednio w obrazie (sf::Image).
 */

/**
 * @brief Zestaw statycznych funkcji rysujących piksele, linie, figury i wypełnienia.
 *
 * Wszystkie funkcje przycinają rysunek do granic obrazu, więc można im podawać
 * współrzędne spoza bufora.
 */
class PrimitiveRenderer {
public:
    /// @brief Wypełnia cały obraz kolorem. @param image Obraz. @param color Kolor.
    static void clear(sf::Image& image, sf::Color color = sf::Color::Black);
    /// @brief Rysuje pojedynczy piksel. @param bufor Obraz. @param x X. @param y Y. @param color Kolor.
    static void renderPoint(sf::Image& bufor, unsigned int x, unsigned int y, sf::Color color);
    /// @brief Rysuje odcinek (algorytm Bresenhama). @param bufor Obraz. @param x1 X początku. @param y1 Y początku. @param x2 X końca. @param y2 Y końca. @param color Kolor.
    static void renderLine(sf::Image& bufor, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, sf::Color color);
    /// @brief Rysuje kontur kwadratu. @param bufor Obraz. @param x X rogu. @param y Y rogu. @param a Bok.
    static void renderSquare(sf::Image& bufor, unsigned int x, unsigned int y, unsigned int a);
    /// @brief Rysuje kontur prostokąta. @param bufor Obraz. @param x X rogu. @param y Y rogu. @param a Wysokość. @param b Szerokość.
    static void renderRectangle(sf::Image& bufor, unsigned int x, unsigned int y, unsigned int a, unsigned int b);
    /// @brief Rysuje kontur koła. @param bufor Obraz. @param x X środka. @param y Y środka. @param r Promień. @param color Kolor.
    static void renderCircle(sf::Image& bufor, unsigned int x, unsigned int y, unsigned int r, sf::Color color);
    /// @brief Rysuje kontur elipsy. @param bufor Obraz. @param x X środka. @param y Y środka. @param r1 Półoś pozioma. @param r2 Półoś pionowa. @param color Kolor.
    static void renderElipsis(sf::Image& bufor, unsigned int x, unsigned int y, unsigned int r1, unsigned int r2 , sf::Color color);
    /// @brief Wypełnia kolorem obszar o jednolitym kolorze, zaczynając od punktu. @param bufor Obraz. @param x X startu. @param y Y startu. @param color Nowy kolor.
    static void floodFill(sf::Image& bufor, unsigned int x, unsigned int y, sf::Color color);
    /// @brief Miesza kolor src (z alfa) z kolorem tła dst. @param src Kolor nakładany. @param dst Kolor tła. @return Wynik.
    static sf::Color blend(sf::Color src, sf::Color dst);
    /// @brief Rysuje wypełnione koło; współrzędne ze znakiem, więc może wystawać poza bufor (jest przycinane).
    /// @param bufor Obraz. @param x X środka. @param y Y środka. @param r Promień. @param color Kolor.
    static void fillCircle(sf::Image& bufor, int x, int y, int r, sf::Color color);
};

#endif //GRAFIKA2D_PRIMITIVERENDERER_H
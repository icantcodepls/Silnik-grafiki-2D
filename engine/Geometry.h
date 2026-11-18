#ifndef GRAFIKA2D_GEOMETRY_H
#define GRAFIKA2D_GEOMETRY_H
#include "SFML/System/Vector2.hpp"

/**
 * @file Geometry.h
 * @brief Drobne funkcje geometryczne używane przez gry (kolizje, zamiana na piksele).
 */

/// @brief Funkcje geometryczne dla współrzędnych zmiennoprzecinkowych.
namespace Geometry {
/**
 * @brief Kwadrat odległości punktu od odcinka.
 * @param p Punkt.
 * @param a Początek odcinka.
 * @param b Koniec odcinka.
 */
float distanceSquaredToSegment(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b);

/**
 * @brief Sprawdza, czy odcinek przecina (lub dotyka) koło.
 * @param a Początek odcinka.
 * @param b Koniec odcinka.
 * @param c Środek koła.
 * @param r Promień koła.
 */
bool segmentIntersectsCircle(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, float r);

/**
 * @brief Zamienia liczbę zmiennoprzecinkową na współrzędną bufora (zaokrągla, wartości ujemne obcina do 0).
 * @param v Wartość.
 */
unsigned int toPixel(float v);
} // namespace Geometry

#endif // GRAFIKA2D_GEOMETRY_H

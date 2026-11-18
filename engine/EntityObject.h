#ifndef GRAFIKA2D_ENTITYOBJECT_H
#define GRAFIKA2D_ENTITYOBJECT_H
#include "SFML/System/Vector2.hpp"
#include "ShapeObject.h"
#include "UpdatableObject.h"

/**
 * @file EntityObject.h
 * @brief Obiekt gry poruszający się w świecie.
 */

/**
 * @brief Obiekt gry poruszający się w świecie: jednocześnie kształt (ShapeObject) i obiekt aktualizowany (UpdatableObject).
 *
 * Pozycja jest zmiennoprzecinkowa, więc ruch jest płynny nawet przy małej prędkości
 * i krótkim kroku czasu. Wspólna baza dla Player, a w grach także dla przeciwników i pocisków.
 *
 * Hierarchia:
 * @code
 * GameObject (dziedziczony wirtualnie)
 *   DrawableObject + TransformableObject = ShapeObject --+
 *   UpdatableObject -------------------------------------+--> EntityObject --> Player
 * @endcode
 */
class EntityObject : public ShapeObject, public UpdatableObject {
protected:
    sf::Vector2f pos{0.f, 0.f}; ///< Środek obiektu (piksele bufora).
    float facing = 0.f;         ///< Kierunek "przodu" w radianach (0 = w prawo, rośnie zgodnie z ruchem wskazówek zegara).
    float radius = 10.f;        ///< Promień bryły kolizji.
public:
    EntityObject() = default;
    /// @brief Tworzy obiekt. @param position Pozycja środka. @param radius Promień kolizji.
    EntityObject(sf::Vector2f position, float radius) : pos(position), radius(radius) {}

    /// @brief Zwraca pozycję środka.
    sf::Vector2f getPosition() const { return pos; }
    /// @brief Ustawia pozycję środka. @param position Nowa pozycja.
    void setPosition(sf::Vector2f position) { pos = position; }
    /// @brief Zwraca kierunek przodu (rad).
    float getFacing() const { return facing; }
    /// @brief Ustawia kierunek przodu. @param angle Kąt w radianach.
    void setFacing(float angle) { facing = angle; }
    /// @brief Zwraca promień kolizji.
    float getRadius() const { return radius; }
    /// @brief Ustawia promień kolizji. @param r Promień.
    void setRadius(float r) { radius = r; }

    /// @brief Przesuwa obiekt. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    void translate(int dx, int dy) override;
    /// @brief Skaluje położenie i promień względem punktu (x, y). @param x X środka. @param y Y środka. @param k Współczynnik.
    void scale(unsigned int x, unsigned int y, float k) override;
    /// @brief Obraca położenie i kierunek wokół punktu (x, y). @param x X środka. @param y Y środka. @param angle Kąt (rad).
    void rotate(unsigned int x, unsigned int y, float angle) override;
};

#endif // GRAFIKA2D_ENTITYOBJECT_H

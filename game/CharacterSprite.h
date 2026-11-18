#ifndef HOTLINE_CHARACTERSPRITE_H
#define HOTLINE_CHARACTERSPRITE_H
#include <SFML/System/Vector2.hpp>

#include "engine/SpriteObject.h"

/**
 * @file CharacterSprite.h
 * @brief Animowana postać złożona z nóg i tułowia.
 */

/**
 * @brief Postać złożona z dwóch części bitmapy (BitmapObject::parts).
 *
 *  - nogi (4 klatki chodu) są obrócone w kierunku RUCHU,
 *  - tułów (spokój / strzał) jest obrócony w kierunku CELOWANIA.
 *
 * To SpriteObject (BitmapObject + AnimatedObject): rysowanie dziedziczy po BitmapObject,
 * a animate() przełącza klatki nóg i błysk z lufy.
 */
class CharacterSprite : public SpriteObject {
public:
    static constexpr int FRAME = 32;      ///< Bok klatki w arkuszu (px).
    static constexpr int LEG_FRAMES = 4;  ///< Liczba klatek chodu nóg.
    static constexpr int TORSO_FRAMES = 2;///< Liczba klatek tułowia (0 = spokój, 1 = strzał).

private:
    size_t legsPart = 0, torsoPart = 0;   ///< Indeksy części: nogi i tułów.
    int legFrame = 0;                     ///< Aktualna klatka nóg.
    float legTimer = 0.f;                 ///< Czas od ostatniej zmiany klatki nóg.
    float flashTimer = 0.f;               ///< Pozostały czas błysku z lufy.
    bool walking = false;                 ///< Czy postać idzie (animuje nogi).
    float stepInterval = 0.10f;           ///< Czas jednej klatki chodu (s).

    /// @brief Ustawia fragmenty bitmap zgodnie z aktualnymi klatkami.
    void updateFrames();

public:
    /**
     * @brief Tworzy postać z dwóch arkuszy tekstur.
     * @param handler Magazyn bitmap (musi żyć dłużej niż postać).
     * @param legs Arkusz nóg (4 klatki 32x32 w rzędzie).
     * @param torso Arkusz tułowia (2 klatki 32x32 w rzędzie).
     * @param scale Skala rysowania.
     */
    CharacterSprite(BitmapHandler& handler, BitmapId legs, BitmapId torso, float scale = 2.f);

    /**
     * @brief Ustawia pozę postaci.
     * @param position Pozycja środka postaci.
     * @param aimAngle Kąt tułowia (celowanie), w radianach.
     * @param legsAngle Kąt nóg (kierunek ruchu), w radianach.
     */
    void setPose(sf::Vector2f position, float aimAngle, float legsAngle);
    /// @brief Włącza lub wyłącza animację chodu. @param isWalking Czy postać idzie.
    void setWalking(bool isWalking) { walking = isWalking; }
    /// @brief Pokazuje na chwilę klatkę "strzał" tułowia (błysk z lufy). @param seconds Czas trwania.
    void showMuzzleFlash(float seconds = 0.07f) { flashTimer = seconds; }

    /// @brief Przesuwa animację nóg i błysk lufy. @param dt Krok czasu (s).
    void animate(float dt) override;
};

#endif // HOTLINE_CHARACTERSPRITE_H

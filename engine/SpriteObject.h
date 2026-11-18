#ifndef GRAFIKA2D_SPRITEOBJECT_H
#define GRAFIKA2D_SPRITEOBJECT_H
#include "AnimatedObject.h"
#include "BitmapObject.h"


/**
 * @file SpriteObject.h
 * @brief Animowany obiekt bitmapowy.
 */

/**
 * @brief Sprite: BitmapObject, który dodatkowo można animować (AnimatedObject).
 *
 * Klasy pochodne implementują animate(), np. zmieniając fragment bitmapy w
 * BitmapPart::source na kolejne klatki animacji.
 */
class SpriteObject:public BitmapObject, public AnimatedObject{
public:
    using BitmapObject::BitmapObject;   // dziedziczy konstruktor SpriteObject(BitmapHandler&)
};


#endif //GRAFIKA2D_SPRITEOBJECT_H
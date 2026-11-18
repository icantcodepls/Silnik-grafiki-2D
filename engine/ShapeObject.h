#ifndef GRAFIKA2D_SHAPEOBJECT_H
#define GRAFIKA2D_SHAPEOBJECT_H
#include "DrawableObject.h"
#include "TransformableObject.h"


/**
 * @file ShapeObject.h
 * @brief Bazowa klasa figur geometrycznych.
 */

/**
 * @brief Figura: obiekt rysowalny (DrawableObject) i przekształcalny (TransformableObject) z kolorem.
 */
class ShapeObject:public DrawableObject, public TransformableObject{
protected:
    Color color=Color::White;   ///< Kolor figury.
public:
    /// @brief Zwraca kolor figury.
    Color getColor(){return color;}
    /// @brief Ustawia kolor figury. @param color Nowy kolor.
    void setColor(Color color){this->color=color;}
};


#endif //GRAFIKA2D_SHAPEOBJECT_H
#ifndef GRAFIKA2D_TRANSFORMABLEOBJECT_H
#define GRAFIKA2D_TRANSFORMABLEOBJECT_H
#include "GameObject.h"


/**
 * @file TransformableObject.h
 * @brief Interfejs obiektów, które można przesuwać, skalować i obracać.
 */

/**
 * @brief Obiekt poddawany przekształceniom geometrycznym.
 */
class TransformableObject:public virtual GameObject {
public:
    /// @brief Przesuwa obiekt. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    virtual void translate(int dx, int dy) = 0;
    /// @brief Skaluje względem punktu (x, y). @param x X środka. @param y Y środka. @param k Współczynnik.
    virtual void scale(unsigned int x, unsigned int y, float k) = 0;
    /// @brief Obraca wokół punktu (x, y). @param x X środka. @param y Y środka. @param angle Kąt w radianach.
    virtual void rotate(unsigned int x, unsigned int y, float angle) = 0;
};


#endif //GRAFIKA2D_TRANSFORMABLEOBJECT_H
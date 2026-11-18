#ifndef GRAFIKA2D_DRAWABLEOBJECT_H
#define GRAFIKA2D_DRAWABLEOBJECT_H
#include "GameObject.h"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Network/SocketHandle.hpp"

using namespace sf;

/**
 * @file DrawableObject.h
 * @brief Interfejs obiektów rysowanych w buforze obrazu.
 */

/**
 * @brief Obiekt, który potrafi się narysować w buforze obrazu i można go ukryć.
 */
class DrawableObject:public virtual GameObject {
    bool visible = true;   ///< Czy obiekt jest widoczny.
public:
    /// @brief Zwraca true, gdy obiekt jest widoczny.
    bool isVisible(){return visible;};
    /// @brief Pokazuje lub ukrywa obiekt. @param visible Nowa widoczność.
    void setVisible(bool visible){this->visible=visible;};
    /// @brief Rysuje obiekt. @param bufor Obraz docelowy.
    virtual void draw(Image& bufor) = 0;
};


#endif //GRAFIKA2D_DRAWABLEOBJECT_H
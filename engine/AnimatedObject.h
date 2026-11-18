#ifndef GRAFIKA2D_ANIMATEDOBJECT_H
#define GRAFIKA2D_ANIMATEDOBJECT_H
#include "GameObject.h"


/**
 * @file AnimatedObject.h
 * @brief Interfejs obiektów animowanych w czasie.
 */

/**
 * @brief Obiekt, który zmienia swój wygląd w czasie (np. klatki animacji).
 */
class AnimatedObject:public virtual GameObject {
public:
    /// @brief Przesuwa animację do przodu. @param dt Czas, który upłynął (s).
    virtual void animate(float dt) = 0;
};


#endif //GRAFIKA2D_ANIMATEDOBJECT_H
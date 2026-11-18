#ifndef GRAFIKA2D_UPDATABLEOBJECT_H
#define GRAFIKA2D_UPDATABLEOBJECT_H
#include "GameObject.h"


/**
 * @file UpdatableObject.h
 * @brief Interfejs obiektów aktualizowanych w czasie.
 */

/**
 * @brief Obiekt, którego stan zmienia się co krok czasu (ruch, logika).
 */
class UpdatableObject:public virtual GameObject{
public:
    /// @brief Aktualizuje obiekt. @param dt Krok czasu w sekundach.
    virtual void update(float dt) = 0;
};


#endif //GRAFIKA2D_UPDATABLEOBJECT_H
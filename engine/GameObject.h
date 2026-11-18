#ifndef GRAFIKA2D_GAMEOBJECT_H
#define GRAFIKA2D_GAMEOBJECT_H
#include <string>

using namespace std;

/**
 * @file GameObject.h
 * @brief Wspólna klasa bazowa wszystkich obiektów silnika.
 */

/**
 * @brief Bazowy obiekt silnika: ma unikalny identyfikator, nazwę i flagę aktywności.
 *
 * Pozostałe interfejsy (DrawableObject, TransformableObject, UpdatableObject,
 * AnimatedObject) dziedziczą po niej wirtualnie, więc obiekt złożony z kilku z nich
 * ma tylko jedną kopię GameObject.
 */
class GameObject {
    unsigned int id;                       ///< Unikalny identyfikator.
    string name = "Default Object";        ///< Nazwa obiektu.
    bool active = true;                    ///< Czy obiekt jest aktywny (nieaktywne można usuwać z gry).

    /// @brief Zwraca kolejny wolny identyfikator.
    static unsigned int nextId();
public:
    /// @brief Tworzy obiekt z nowym identyfikatorem.
    GameObject();
    virtual ~GameObject() = default;
    /// @brief Zwraca identyfikator.
    unsigned int getId() const {return id;}
    /// @brief Zwraca nazwę.
    string getName() const {return name;}
    /// @brief Ustawia nazwę. @param name Nowa nazwa.
    void setName(string name){this->name = name;}
    /// @brief Zwraca true, gdy obiekt jest aktywny.
    bool isActive() const {return active;}
    /// @brief Ustawia aktywność. @param active Nowa wartość.
    void setActive(bool active){this->active = active;}
};


#endif //GRAFIKA2D_GAMEOBJECT_H
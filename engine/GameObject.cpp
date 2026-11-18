#include "GameObject.h"

unsigned int GameObject::nextId() {
    static unsigned int counter = 0;
    return ++counter;
}

GameObject::GameObject() : id(nextId()) {}

#include "EntityVector.h"
#include <algorithm>

// Adds a new entity to the vector
void EntityVector::addEntity(Entity* entity) {
    entities.push_back(entity);
}

// Removes an entity from the vector by its ID
bool EntityVector::removeEntity(const std::string& id) {
    auto it = std::remove_if(entities.begin(), entities.end(),
        [&](Entity* e){ return e->getId() == id; });

    if (it != entities.end()) {
        entities.erase(it, entities.end());
        return true;
    }
    return false;
}

// Returns a pointer to the entity with the given ID, or nullptr if not found
Entity* EntityVector::getEntity(const std::string& id) {
    for (Entity* e : entities) {
        if (e->getId() == id)
            return e;
    }
    return nullptr;
}

// Returns a modifiable reference to the vector of all entity pointers
std::vector<Entity*>& EntityVector::getAllEntities() {
    return entities;
}

// Returns a constant reference to the vector of all entity pointers (read-only)
const std::vector<Entity*>& EntityVector::getAllEntities() const {
    return entities;
}

// Destructor: deletes all entities and clears the vector
EntityVector::~EntityVector() {
    for (Entity* e : entities) {
        delete e;
    }
    entities.clear();
}

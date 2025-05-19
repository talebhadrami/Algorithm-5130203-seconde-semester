#ifndef ENTITYVECTOR_H
#define ENTITYVECTOR_H

#include "Entity.h"
#include <vector>
#include <string>

// A container class that stores and manages a dynamic list of Entity pointers
// Used to manage wallets (or other entities) belonging to a client
class EntityVector {
private:
    std::vector<Entity*> entities;  // Internal dynamic array of pointers to entities
public:
    // Adds a new entity to the list
    void addEntity(Entity* entity);

    // Removes an entity from the list by its ID
    bool removeEntity(const std::string& id);

    // Retrieves a pointer to the entity with the specified ID
    Entity* getEntity(const std::string& id);

    // Returns a reference to the list of all entities (modifiable)
    std::vector<Entity*>& getAllEntities();

    // Returns a const reference to the list of all entities (read-only)
    const std::vector<Entity*>& getAllEntities() const;

    // Destructor: deletes all entities stored in the list
    ~EntityVector();
};

#endif // ENTITYVECTOR_H

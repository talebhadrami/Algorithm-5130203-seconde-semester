#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// Abstract base class representing a general entity with a unique identifier.
// Used as a base for derived classes such as Client, Wallet, and Transaction.
class Entity {
protected:
    std::string id;  // Unique identifier for the entity
public:
    // Constructor that initializes the entity's ID
    Entity(const std::string& id) : id(id) {}

    // Pure virtual method that must be implemented by all derived classes to return the entity ID
    virtual std::string getId() const = 0;

    // Virtual destructor to allow proper cleanup in derived classes
    virtual ~Entity() {}
};

#endif // ENTITY_H

#ifndef CLIENTBST_H
#define CLIENTBST_H

#include "Client.h"
#include <iostream>

// Node class representing each node in the Binary Search Tree (BST) of clients
class ClientNode {
public:
    Client* data;            // Pointer to the client stored in this node
    ClientNode* left;        // Pointer to the left child
    ClientNode* right;       // Pointer to the right child

    ClientNode(Client* client); // Constructor
    ~ClientNode();              // Destructor
};

// Class representing the Binary Search Tree structure for managing clients
class ClientBST {
private:
    ClientNode* root;  // Root node of the BST

    // Helper methods for recursive operations
    ClientNode* insert(ClientNode* node, Client* client);                // Inserts a client into the tree
    ClientNode* remove(ClientNode* node, const std::string& id, bool& success); // Removes a client by ID
    ClientNode* findMin(ClientNode* node);                               // Finds node with minimum value (leftmost)
    ClientNode* removeMin(ClientNode* node);                             // Removes the node with minimum value
    ClientNode* find(ClientNode* node, const std::string& id) const;     // Finds a node by client ID
    void displayInOrder(ClientNode* node) const;                         // In-order traversal display
    void destroyTree(ClientNode* node);                                  // Recursively deletes the tree

public:
    ClientBST();   // Constructor
    ~ClientBST();  // Destructor

    void insert(Client* client);               // Public method to insert a client
    bool remove(const std::string& id);        // Public method to remove a client by ID
    Client* find(const std::string& id) const; // Public method to find a client by ID
    void displayInOrder() const;               // Public method to display all clients in-order

    ClientNode* getRoot() const;               // Getter for the root node
};

#endif // CLIENTBST_H

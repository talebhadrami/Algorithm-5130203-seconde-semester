#include "ClientBST.h"

// Constructor for a tree node that holds a Client pointer
ClientNode::ClientNode(Client* client)
    : data(client), left(nullptr), right(nullptr) {}

// Destructor to delete the client data
ClientNode::~ClientNode() {
    delete data;
}

// Constructor for the binary search tree
ClientBST::ClientBST() : root(nullptr) {}

// Destructor that destroys the entire tree recursively
ClientBST::~ClientBST() {
    destroyTree(root);
}

// Recursively deletes nodes in the tree
void ClientBST::destroyTree(ClientNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// Recursive insert based on client's total balance
ClientNode* ClientBST::insert(ClientNode* node, Client* client) {
    if (!node) return new ClientNode(client);
    if (client->getTotalBalance() < node->data->getTotalBalance())
        node->left = insert(node->left, client);
    else
        node->right = insert(node->right, client);
    return node;
}

// Public insert method
void ClientBST::insert(Client* client) {
    root = insert(root, client);
}

// Finds the node with the smallest balance (leftmost node)
ClientNode* ClientBST::findMin(ClientNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

// Removes the minimum node in the subtree
ClientNode* ClientBST::removeMin(ClientNode* node) {
    if (!node->left) return node->right;
    node->left = removeMin(node->left);
    return node;
}

// Recursively removes a client by ID
ClientNode* ClientBST::remove(ClientNode* node, const std::string& id, bool& success) {
    if (!node) {
        success = false;
        return nullptr;
    }

    if (id == node->data->getId()) {
        success = true;

        // Case: no left child
        if (!node->left) {
            ClientNode* rightChild = node->right;
            node->right = nullptr;
            delete node;
            return rightChild;
        }
        // Case: no right child
        else if (!node->right) {
            ClientNode* leftChild = node->left;
            node->left = nullptr;
            delete node;
            return leftChild;
        }

        // Case: two children – find the smallest in the right subtree
        ClientNode* minNode = findMin(node->right);
        Client* tempData = node->data;
        node->data = minNode->data;
        minNode->data = tempData;

        node->right = remove(node->right, minNode->data->getId(), success);
        return node;
    }
    else if (id < node->data->getId())
        node->left = remove(node->left, id, success);
    else
        node->right = remove(node->right, id, success);

    return node;
}

// Public method to remove a client by ID
bool ClientBST::remove(const std::string& id) {
    bool success = false;
    root = remove(root, id, success);
    return success;
}

// Recursively searches for a client node by ID
ClientNode* ClientBST::find(ClientNode* node, const std::string& id) const {
    if (!node) return nullptr;
    if (id == node->data->getId()) return node;
    if (id < node->data->getId()) return find(node->left, id);
    return find(node->right, id);
}

// Public method to find a client by ID
Client* ClientBST::find(const std::string& id) const {
    ClientNode* node = find(root, id);
    return node ? node->data : nullptr;
}

// Recursively displays the tree in-order (left → root → right)
void ClientBST::displayInOrder(ClientNode* node) const {
    if (!node) return;
    displayInOrder(node->left);
    std::cout << "Client ID: " << node->data->getId()
              << ", Total Balance: " << node->data->getTotalBalance() << std::endl;
    displayInOrder(node->right);
}

// Public method to trigger in-order display
void ClientBST::displayInOrder() const {
    displayInOrder(root);
}

// Returns the root of the tree
ClientNode* ClientBST::getRoot() const {
    return root;
}

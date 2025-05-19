#include "TransactionList.h"

// Constructor for a transaction node (doubly linked list node)
TransactionNode::TransactionNode(Transaction* data)
    : data(data), prev(nullptr), next(nullptr) {}

// Destructor deletes the transaction object
TransactionNode::~TransactionNode() {
    delete data;
}

// Constructor initializes an empty transaction list
TransactionList::TransactionList()
    : head(nullptr), tail(nullptr), size(0) {}

// Destructor deletes all nodes in the list
TransactionList::~TransactionList() {
    TransactionNode* current = head;
    while (current != nullptr) {
        TransactionNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// Adds a transaction to the end of the list
void TransactionList::addTransaction(Transaction* tx) {
    TransactionNode* newNode = new TransactionNode(tx);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

// Removes a transaction by its ID from the list
bool TransactionList::removeTransaction(const std::string& id) {
    TransactionNode* current = head;
    while (current != nullptr) {
        if (current->data->getId() == id) {
            // Update links to skip the current node
            if (current->prev) current->prev->next = current->next;
            else head = current->next;

            if (current->next) current->next->prev = current->prev;
            else tail = current->prev;

            delete current;
            size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Retrieves a transaction by its ID
Transaction* TransactionList::getTransaction(const std::string& id) {
    TransactionNode* current = head;
    while (current != nullptr) {
        if (current->data->getId() == id)
            return current->data;
        current = current->next;
    }
    return nullptr;
}

// Displays all transactions in order from head to tail
void TransactionList::displayTransactions() const {
    TransactionNode* current = head;
    while (current) {
        std::cout << current->data->getDetails() << std::endl;
        current = current->next;
    }
}

// Returns the head pointer of the list
TransactionNode* TransactionList::getHead() const {
    return head;
}

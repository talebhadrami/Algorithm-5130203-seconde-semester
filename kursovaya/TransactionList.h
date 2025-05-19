#ifndef TRANSACTIONLIST_H
#define TRANSACTIONLIST_H

#include "Transaction.h"
#include <iostream>

// Node class for doubly linked list, stores a pointer to a Transaction
class TransactionNode {
public:
    Transaction* data;          // Pointer to the transaction data
    TransactionNode* prev;      // Pointer to previous node
    TransactionNode* next;      // Pointer to next node

    TransactionNode(Transaction* data); // Constructor
    ~TransactionNode();                  // Destructor
};

// Class implementing a doubly linked list of transactions
class TransactionList {
private:
    TransactionNode* head;  // Pointer to the head of the list
    TransactionNode* tail;  // Pointer to the tail of the list
    int size;               // Number of transactions in the list

public:
    TransactionList();      // Constructor initializes empty list
    ~TransactionList();     // Destructor cleans up all nodes

    void addTransaction(Transaction* tx);        // Adds a transaction to the list
    bool removeTransaction(const std::string& id); // Removes a transaction by ID
    Transaction* getTransaction(const std::string& id); // Retrieves a transaction by ID
    void displayTransactions() const;             // Prints all transactions

    TransactionNode* getHead() const;              // Returns pointer to head node
};

#endif // TRANSACTIONLIST_H

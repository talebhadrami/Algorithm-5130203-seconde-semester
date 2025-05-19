#include "Wallet.h"

// Constructor initializes wallet with ID, owner ID, and initial balance
Wallet::Wallet(const std::string& id, const std::string& ownerId, double balance)
    : Entity(id), ownerId(ownerId), balance(balance) {}

// Adds the specified amount to the wallet balance if positive
void Wallet::deposit(double amount) {
    if (amount > 0) balance += amount;
}

// Withdraws the specified amount if it is positive and sufficient balance exists
// Returns true if successful, false otherwise
bool Wallet::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}

// Returns the current balance in the wallet
double Wallet::getBalance() const {
    return balance;
}

// Returns the wallet ID
std::string Wallet::getId() const {
    return id;
}

// Returns the ID of the wallet owner (client)
std::string Wallet::getOwnerId() const {
    return ownerId;
}

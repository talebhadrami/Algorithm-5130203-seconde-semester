#ifndef WALLET_H
#define WALLET_H

#include "Entity.h"
#include <string>

// Class representing a Wallet, which stores funds and belongs to a client
class Wallet : public Entity {
private:
    double balance;          // Current balance in the wallet
    std::string ownerId;     // ID of the wallet's owner (client)

public:
    // Constructor to initialize wallet ID, owner ID, and starting balance
    Wallet(const std::string& id, const std::string& ownerId, double balance);

    // Deposits the specified amount into the wallet
    void deposit(double amount);

    // Withdraws the specified amount from the wallet if possible
    // Returns true if successful, false otherwise
    bool withdraw(double amount);

    // Returns the current wallet balance
    double getBalance() const;

    // Returns the wallet ID (overrides Entity's pure virtual method)
    std::string getId() const override;

    // Returns the owner (client) ID
    std::string getOwnerId() const;
};

#endif // WALLET_H

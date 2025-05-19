#ifndef CLIENT_H
#define CLIENT_H

#include "Entity.h"
#include "EntityVector.h"
#include "Wallet.h"
#include <string>

// Abstract base class representing a generic client
class Client : public Entity {
protected:
    std::string name;            // Client name
    EntityVector wallets;        // Collection of wallets owned by the client
public:
    Client(const std::string& id, const std::string& name);
    virtual ~Client();

    void addWallet(Wallet* wallet);         // Adds a wallet to the client
    double getTotalBalance() const;         // Calculates the total balance from all wallets

    virtual double calculateCommission(double amount) const = 0;   // Commission rate (pure virtual)
    virtual double getMaxTransactionLimit() const = 0;             // Transaction limit (pure virtual)

    std::string getId() const override;     // Returns client ID
    std::string getName() const;            // Returns client name
    const EntityVector& getWallets() const; // Returns reference to all client wallets
};

// Represents a standard client with the highest commission rate and lowest transaction limit
class StandardClient : public Client {
public:
    StandardClient(const std::string& id, const std::string& name);
    double calculateCommission(double amount) const override;      // 5% commission
    double getMaxTransactionLimit() const override;                // $1000 limit
};

// Represents a gold-level client with low commission and high transaction limit
class GoldClient : public Client {
public:
    GoldClient(const std::string& id, const std::string& name);
    double calculateCommission(double amount) const override;      // 1% commission
    double getMaxTransactionLimit() const override;                // $10000 limit
};

// Represents a platinum-level client with medium commission and limit
class PlatinumClient : public Client {
public:
    PlatinumClient(const std::string& id, const std::string& name);
    double calculateCommission(double amount) const override;      // 2% commission
    double getMaxTransactionLimit() const override;                // $5000 limit
};

#endif // CLIENT_H

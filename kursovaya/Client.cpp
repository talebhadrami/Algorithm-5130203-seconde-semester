#include "Client.h"

Client::Client(const std::string& id, const std::string& name)
    : Entity(id), name(name) {}

Client::~Client() {
    // Wallets are destroyed by EntityVector destructor
}

void Client::addWallet(Wallet* wallet) {
    wallets.addEntity(wallet);
}

double Client::getTotalBalance() const {
    double total = 0.0;
    const auto& walletEntities = wallets.getAllEntities();
    for (Entity* e : walletEntities) {
        Wallet* w = dynamic_cast<Wallet*>(e);
        if (w) total += w->getBalance();
    }
    return total;
}

std::string Client::getId() const {
    return id;
}

std::string Client::getName() const {
    return name;
}

const EntityVector& Client::getWallets() const {
    return wallets;
}

// ----------- StandardClient implementation -----------

StandardClient::StandardClient(const std::string& id, const std::string& name)
    : Client(id, name) {}

double StandardClient::calculateCommission(double amount) const {
    return amount * 0.05;
}

double StandardClient::getMaxTransactionLimit() const {
    return 1000.0;
}

// ----------- GoldClient implementation -----------

GoldClient::GoldClient(const std::string& id, const std::string& name)
    : Client(id, name) {}

double GoldClient::calculateCommission(double amount) const {
    return amount * 0.01;
}

double GoldClient::getMaxTransactionLimit() const {
    return 10000.0;
}

// ----------- PlatinumClient implementation -----------

PlatinumClient::PlatinumClient(const std::string& id, const std::string& name)
    : Client(id, name) {}

double PlatinumClient::calculateCommission(double amount) const {
    return amount * 0.02;
}

double PlatinumClient::getMaxTransactionLimit() const {
    return 5000.0;
}

#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "ClientBST.h"
#include "TransactionList.h"
#include "Wallet.h"
#include <string>
#include <unordered_map>

class Blockchain {
private:
    ClientBST clients;
    TransactionList transactions;
    std::unordered_map<std::string, Wallet*> walletIndex;

public:
    Blockchain();
    ~Blockchain();

    void addClient(Client* client);
    bool processTransaction(Transaction* tx);
    void displayClients() const;
    void displayTransactions() const;

    bool saveClientsToFile(const std::string& filename) const;
    bool loadClientsFromFile(const std::string& filename);
    bool saveTransactionsToFile(const std::string& filename) const;
    bool loadTransactionsFromFile(const std::string& filename);

    Wallet* findWalletById(const std::string& walletId) const;

    ClientNode* getRoot() const;

    // Nouvelle méthode pour indexer un wallet
    void indexWallet(Wallet* wallet);
};

#endif // BLOCKCHAIN_H

#include "Blockchain.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <functional>

Blockchain::Blockchain() {}

Blockchain::~Blockchain() {}

void Blockchain::addClient(Client* client) {
    clients.insert(client);
    const auto& wallets = client->getWallets().getAllEntities();
    for (Entity* e : wallets) {
        Wallet* w = dynamic_cast<Wallet*>(e);
        if (w)
            walletIndex[w->getId()] = w;
    }
}

Wallet* Blockchain::findWalletById(const std::string& walletId) const {
    auto it = walletIndex.find(walletId);
    if (it != walletIndex.end())
        return it->second;
    return nullptr;
}

bool Blockchain::processTransaction(Transaction* tx) {
    Wallet* senderWallet = findWalletById(tx->getSenderWalletId());
    Wallet* recipientWallet = findWalletById(tx->getRecipientWalletId());

    if (!senderWallet || !recipientWallet) {
        std::cerr << "Sender or recipient wallet not found.\n";
        return false;
    }

    Client* senderClient = nullptr;

    std::function<ClientNode*(ClientNode*, Wallet*)> findClientNodeByWallet = [&](ClientNode* node, Wallet* w) -> ClientNode* {
        if (!node) return nullptr;
        const auto& ws = node->data->getWallets().getAllEntities();
        for (Entity* e : ws) {
            if (e->getId() == w->getId())
                return node;
        }
        ClientNode* found = findClientNodeByWallet(node->left, w);
        if (found)
            return found;
        return findClientNodeByWallet(node->right, w);
    };

    ClientNode* senderNode = findClientNodeByWallet(clients.getRoot(), senderWallet);
    if (!senderNode) {
        std::cerr << "Sender client not found.\n";
        return false;
    }
    senderClient = senderNode->data;

    double amount = tx->getAmount();
    double commission = tx->getCommission();

    if (amount > senderClient->getMaxTransactionLimit()) {
        std::cerr << "Transaction amount exceeds sender's limit.\n";
        return false;
    }

    if (senderWallet->getBalance() < amount + commission) {
        std::cerr << "Insufficient funds in sender's wallet.\n";
        return false;
    }

    if (!senderWallet->withdraw(amount + commission)) {
        std::cerr << "Withdrawal failed.\n";
        return false;
    }
    recipientWallet->deposit(amount);

    transactions.addTransaction(tx);

    return true;
}

void Blockchain::displayClients() const {
    clients.displayInOrder();
}

void Blockchain::displayTransactions() const {
    transactions.displayTransactions();
}

bool Blockchain::saveClientsToFile(const std::string& filename) const {
    FILE* file = fopen(filename.c_str(), "w");
    if (!file) return false;

    std::function<void(ClientNode*)> saveNode = [&](ClientNode* node) {
        if (!node) return;

        saveNode(node->left);

        std::string clientType = "Standard";
        if (dynamic_cast<GoldClient*>(node->data)) clientType = "Gold";
        else if (dynamic_cast<PlatinumClient*>(node->data)) clientType = "Platinum";

        fprintf(file, "%s;%s;%s\n", node->data->getId().c_str(), node->data->getName().c_str(), clientType.c_str());

        const auto& wallets = node->data->getWallets().getAllEntities();
        for (Entity* e : wallets) {
            Wallet* w = dynamic_cast<Wallet*>(e);
            if (w) {
                fprintf(file, "W;%s;%.2f\n", w->getId().c_str(), w->getBalance());
            }
        }

        saveNode(node->right);
    };

    saveNode(clients.getRoot());

    fclose(file);
    return true;
}

bool Blockchain::loadClientsFromFile(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) return false;

    char line[256];
    Client* currentClient = nullptr;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] != 'W') {
            char id[50], name[50], type[20];
            sscanf(line, "%[^;];%[^;];%s", id, name, type);

            if (strcmp(type, "Gold") == 0)
                currentClient = new GoldClient(id, name);
            else if (strcmp(type, "Platinum") == 0)
                currentClient = new PlatinumClient(id, name);
            else
                currentClient = new StandardClient(id, name);

            addClient(currentClient);
        } else {
            char wid[50];
            double balance;
            sscanf(line, "W;%[^;];%lf", wid, &balance);
            if (currentClient) {
                Wallet* wallet = new Wallet(wid, currentClient->getId(), balance);
                currentClient->addWallet(wallet);
                // Mettre à jour l'index wallet
                walletIndex[wallet->getId()] = wallet;
            }
        }
    }

    fclose(file);
    return true;
}

bool Blockchain::saveTransactionsToFile(const std::string& filename) const {
    FILE* file = fopen(filename.c_str(), "w");
    if (!file) return false;

    TransactionNode* current = transactions.getHead();
    while (current) {
        Transaction* tx = current->data;
        fprintf(file, "%s;%s;%s;%.2f;%.2f\n", tx->getId().c_str(), tx->getSenderWalletId().c_str(),
                tx->getRecipientWalletId().c_str(), tx->getAmount(), tx->getCommission());
        current = current->next;
    }

    fclose(file);
    return true;
}

bool Blockchain::loadTransactionsFromFile(const std::string& filename) {
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) return false;

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char id[50], sender[50], recipient[50];
        double amount, commission;
        sscanf(line, "%[^;];%[^;];%[^;];%lf;%lf", id, sender, recipient, &amount, &commission);
        Transaction* tx = new Transaction(id, sender, recipient, amount, TxType::TRANSFER, commission);
        transactions.addTransaction(tx);
    }

    fclose(file);
    return true;
}

ClientNode* Blockchain::getRoot() const {
    return clients.getRoot();
}

// Implémentation de la nouvelle méthode indexWallet
void Blockchain::indexWallet(Wallet* wallet) {
    walletIndex[wallet->getId()] = wallet;
}

#include <iostream>
#include <string>
#include <functional>
#include "Blockchain.h"
#include "Client.h"
#include "Wallet.h"
#include "Transaction.h"

void showMenu() {
    std::cout << "\n=== Blockchain Menu ===\n";
    std::cout << "1. Add a client\n";
    std::cout << "2. Display all clients\n";
    std::cout << "3. Make a transaction\n";
    std::cout << "4. Display all transactions\n";
    std::cout << "5. Save data\n";
    std::cout << "6. Load data\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    Blockchain blockchain;
    bool running = true;

    while (running) {
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string id, name, type;
                std::cout << "Client ID: ";
                std::getline(std::cin, id);
                std::cout << "Client name: ";
                std::getline(std::cin, name);
                std::cout << "Client type (Standard, Gold, Platinum): ";
                std::getline(std::cin, type);

                Client* client = nullptr;
                if (type == "Gold")
                    client = new GoldClient(id, name);
                else if (type == "Platinum")
                    client = new PlatinumClient(id, name);
                else
                    client = new StandardClient(id, name);

                blockchain.addClient(client);

                int walletCount;
                std::cout << "How many wallets to add for this client? ";
                std::cin >> walletCount;
                std::cin.ignore();

                for (int i = 0; i < walletCount; ++i) {
                    std::string walletId;
                    double balance;

                    std::cout << "Wallet #" << (i + 1) << " ID: ";
                    std::getline(std::cin, walletId);

                    std::cout << "Initial balance: ";
                    std::cin >> balance;
                    std::cin.ignore();

                    Wallet* wallet = new Wallet(walletId, client->getId(), balance);
                    client->addWallet(wallet);

                    // Important : indexer ce wallet dans la blockchain
                    blockchain.indexWallet(wallet);
                }

                std::cout << "Client and wallets added.\n";
                break;
            }
            case 2:
                blockchain.displayClients();
                break;
            case 3: {
                std::string txId, senderWalletId, recipientWalletId;
                double amount;

                std::cout << "Transaction ID: ";
                std::getline(std::cin, txId);
                std::cout << "Sender wallet ID: ";
                std::getline(std::cin, senderWalletId);
                std::cout << "Recipient wallet ID: ";
                std::getline(std::cin, recipientWalletId);
                std::cout << "Amount: ";
                std::cin >> amount;
                std::cin.ignore();

                Wallet* senderWallet = blockchain.findWalletById(senderWalletId);
                if (!senderWallet) {
                    std::cout << "Sender wallet not found.\n";
                    break;
                }

                Client* senderClient = nullptr;
                std::function<ClientNode*(ClientNode*, Wallet*)> findClientNodeByWallet = [&](ClientNode* node, Wallet* w) -> ClientNode* {
                    if (!node) return nullptr;
                    const auto& ws = node->data->getWallets().getAllEntities();
                    for (Entity* e : ws) {
                        if (e->getId() == w->getId()) return node;
                    }
                    ClientNode* found = findClientNodeByWallet(node->left, w);
                    if (found) return found;
                    return findClientNodeByWallet(node->right, w);
                };

                ClientNode* senderNode = findClientNodeByWallet(blockchain.getRoot(), senderWallet);
                if (!senderNode) {
                    std::cout << "Sender client not found.\n";
                    break;
                }
                senderClient = senderNode->data;

                double commission = senderClient->calculateCommission(amount);

                Transaction* tx = new Transaction(txId, senderWalletId, recipientWalletId, amount, TxType::TRANSFER, commission);
                if (blockchain.processTransaction(tx))
                    std::cout << "Transaction successful.\n";
                else
                    std::cout << "Transaction failed.\n";
                break;
            }
            case 4:
                blockchain.displayTransactions();
                break;
            case 5:
                if (blockchain.saveClientsToFile("Clients.txt") && blockchain.saveTransactionsToFile("Blockchain_transactions.txt"))
                    std::cout << "Data saved successfully.\n";
                else
                    std::cout << "Error saving data.\n";
                break;
            case 6:
                if (blockchain.loadClientsFromFile("Clients.txt") && blockchain.loadTransactionsFromFile("Blockchain_transactions.txt"))
                    std::cout << "Data loaded successfully.\n";
                else
                    std::cout << "Error loading data.\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}

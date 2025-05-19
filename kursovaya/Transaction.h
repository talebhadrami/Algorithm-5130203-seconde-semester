#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Entity.h"
#include <string>

// Enumeration for transaction types (currently only TRANSFER supported)
enum class TxType { TRANSFER };

// Class representing a financial transaction between two wallets
class Transaction : public Entity {
private:
    std::string senderWalletId;       // ID of the sender's wallet
    std::string recipientWalletId;    // ID of the recipient's wallet
    double amount;                    // Amount of money transferred
    TxType type;                      // Type of transaction
    double commission;                // Commission charged for the transaction

public:
    // Constructor to initialize all transaction details
    Transaction(const std::string& id, const std::string& senderWalletId, const std::string& recipientWalletId,
                double amount, TxType type, double commission);

    // Returns the transaction ID
    std::string getId() const override;

    // Returns a formatted string with transaction details
    std::string getDetails() const;

    // Returns the amount transferred
    double getAmount() const;

    // Returns the commission for the transaction
    double getCommission() const;

    // Returns the sender wallet ID
    std::string getSenderWalletId() const;

    // Returns the recipient wallet ID
    std::string getRecipientWalletId() const;
};

#endif // TRANSACTION_H

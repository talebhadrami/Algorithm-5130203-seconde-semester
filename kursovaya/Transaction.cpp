#include "Transaction.h"

// Constructor for Transaction: initializes all attributes
Transaction::Transaction(const std::string& id, const std::string& senderWalletId, const std::string& recipientWalletId,
                         double amount, TxType type, double commission)
    : Entity(id), senderWalletId(senderWalletId), recipientWalletId(recipientWalletId),
      amount(amount), type(type), commission(commission) {}

// Returns the transaction ID
std::string Transaction::getId() const {
    return id;
}

// Returns a formatted string containing all details of the transaction
std::string Transaction::getDetails() const {
    return "Transaction " + id + ": from " + senderWalletId + " to " + recipientWalletId +
           ", amount " + std::to_string(amount) + ", commission " + std::to_string(commission);
}

// Returns the transaction amount
double Transaction::getAmount() const {
    return amount;
}

// Returns the transaction commission
double Transaction::getCommission() const {
    return commission;
}

// Returns the sender wallet ID
std::string Transaction::getSenderWalletId() const {
    return senderWalletId;
}

// Returns the recipient wallet ID
std::string Transaction::getRecipientWalletId() const {
    return recipientWalletId;
}

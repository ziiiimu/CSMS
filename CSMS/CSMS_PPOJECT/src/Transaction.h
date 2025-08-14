// ===== Transaction.h =====
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Product.h"
#include "Customer.h"
#include <vector>
#include <ctime>

/**
 * @brief Enumeration for payment methods
 */
enum class PaymentMethod {
    CASH,
    CREDIT_CARD,
    DEBIT_CARD,
    MOBILE_PAYMENT,
    LOYALTY_POINTS,
    GIFT_CARD
};

/**
 * @brief Enumeration for transaction status
 */
enum class TransactionStatus {
    PENDING,
    COMPLETED,
    CANCELLED,
    REFUNDED,
    PARTIALLY_REFUNDED
};

/**
 * @brief Class representing an item in a transaction
 */
class TransactionItem {
public:
    Product* product;
    double quantity;      // For bulk products, this can be fractional
    double unitPrice;     // Price at time of purchase
    double discount;      // Discount applied to this item
    double subtotal;      // Final price for this item
    std::string notes;    // Special notes for this item

    TransactionItem(Product* prod, double qty, double discount = 0.0, const std::string& notes = "");
    
    void calculateSubtotal();
    void displayItem() const;
};

/**
 * @brief Class representing a complete transaction
 */
class Transaction {
private:
    static int nextTransactionId;
    
    int transactionId;
    std::vector<TransactionItem> items;
    Customer* customer;
    
    double subtotal;
    double tax;
    double totalDiscount;
    double loyaltyPointsUsed;
    double loyaltyPointsEarned;
    double finalTotal;
    
    PaymentMethod paymentMethod;
    TransactionStatus status;
    std::time_t timestamp;
    std::string cashierId;
    std::string notes;

public:
    Transaction(Customer* customer = nullptr, const std::string& cashierId = "");
    
    // Item management
    bool addItem(Product* product, double quantity, double discount = 0.0, const std::string& notes = "");
    bool removeItem(int itemIndex);
    void clearItems();
    
    // Transaction processing
    void calculateTotals(double taxRate = 0.08);
    bool processPayment(PaymentMethod method, double amountPaid = 0.0);
    bool applyLoyaltyPoints(double points);
    void finalizeTransaction();
    
    // Getters
    int getId() const { return transactionId; }
    const std::vector<TransactionItem>& getItems() const { return items; }
    Customer* getCustomer() const { return customer; }
    double getSubtotal() const { return subtotal; }
    double getTax() const { return tax; }
    double getTotalDiscount() const { return totalDiscount; }
    double getFinalTotal() const { return finalTotal; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    TransactionStatus getStatus() const { return status; }
    std::time_t getTimestamp() const { return timestamp; }
    std::string getCashierId() const { return cashierId; }
    double getLoyaltyPointsUsed() const { return loyaltyPointsUsed; }
    double getLoyaltyPointsEarned() const { return loyaltyPointsEarned; }
    
    // Setters
    void setCustomer(Customer* customer) { this->customer = customer; }
    void setCashierId(const std::string& id) { cashierId = id; }
    void setNotes(const std::string& notes) { this->notes = notes; }
    
    // Utility methods
    void printReceipt() const;
    void printDetailedReceipt() const;
    std::string getPaymentMethodString() const;
    std::string getStatusString() const;
    
    // Refund operations
    bool processRefund(double amount = -1.0);  // -1 means full refund
    bool processPartialRefund(int itemIndex, double refundAmount);
};

#endif // TRANSACTION_H
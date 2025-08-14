// ===== Transaction.cpp =====
#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

int Transaction::nextTransactionId = 10001;

// TransactionItem implementation
TransactionItem::TransactionItem(Product* prod, double qty, double discount, const std::string& notes)
    : product(prod), quantity(qty), discount(discount), notes(notes) {
    
    if (product) {
        unitPrice = product->calculateSellingPrice();
        calculateSubtotal();
    } else {
        unitPrice = 0.0;
        subtotal = 0.0;
    }
}

void TransactionItem::calculateSubtotal() {
    if (product) {
        // For bulk products, use special pricing
        BulkProduct* bulkProduct = dynamic_cast<BulkProduct*>(product);
        if (bulkProduct) {
            subtotal = bulkProduct->calculatePriceForQuantity(quantity);
        } else {
            subtotal = unitPrice * quantity;
        }
        
        // Apply discount
        subtotal *= (1.0 - discount);
    }
}

void TransactionItem::displayItem() const {
    if (product) {
        std::cout << product->getName();
        if (quantity != 1.0) {
            std::cout << " x" << std::fixed << std::setprecision(2) << quantity;
        }
        std::cout << " @ $" << std::fixed << std::setprecision(2) << unitPrice;
        if (discount > 0) {
            std::cout << " (" << (discount * 100) << "% off)";
        }
        std::cout << " = $" << std::fixed << std::setprecision(2) << subtotal;
        if (!notes.empty()) {
            std::cout << " [" << notes << "]";
        }
        std::cout << std::endl;
    }
}

// Transaction implementation
Transaction::Transaction(Customer* customer, const std::string& cashierId)
    : transactionId(nextTransactionId++), customer(customer), subtotal(0.0), tax(0.0),
      totalDiscount(0.0), loyaltyPointsUsed(0.0), loyaltyPointsEarned(0.0), finalTotal(0.0),
      paymentMethod(PaymentMethod::CASH), status(TransactionStatus::PENDING),
      cashierId(cashierId) {
    
    timestamp = std::time(nullptr);
}

bool Transaction::addItem(Product* product, double quantity, double discount, const std::string& notes) {
    if (!product || !product->getIsActive() || quantity <= 0) {
        return false;
    }
    
    // Check stock availability
    if (product->getCurrentStock() < static_cast<int>(std::ceil(quantity))) {
        std::cout << "Insufficient stock for " << product->getName() 
                  << ". Available: " << product->getCurrentStock() << std::endl;
        return false;
    }
    
    // For bulk products, check minimum quantity
    BulkProduct* bulkProduct = dynamic_cast<BulkProduct*>(product);
    if (bulkProduct && quantity < bulkProduct->getMinimumQuantity()) {
        std::cout << "Minimum quantity for " << product->getName() 
                  << " is " << bulkProduct->getMinimumQuantity() 
                  << " " << bulkProduct->getUnit() << std::endl;
        return false;
    }
    
    items.push_back(TransactionItem(product, quantity, discount, notes));
    return true;
}

bool Transaction::removeItem(int itemIndex) {
    if (itemIndex >= 0 && itemIndex < static_cast<int>(items.size())) {
        items.erase(items.begin() + itemIndex);
        return true;
    }
    return false;
}

void Transaction::clearItems() {
    items.clear();
}

void Transaction::calculateTotals(double taxRate) {
    subtotal = 0.0;
    totalDiscount = 0.0;
    
    for (const auto& item : items) {
        subtotal += item.subtotal;
        if (item.discount > 0 && item.product) {
            double originalPrice = item.product->calculateSellingPrice() * item.quantity;
            totalDiscount += (originalPrice - item.subtotal);
        }
    }
    
    // Apply customer discount
    if (customer) {
        double customerDiscount = subtotal * customer->getDiscountRate();
        totalDiscount += customerDiscount;
        subtotal -= customerDiscount;
    }
    
    // Apply loyalty points discount
    subtotal -= loyaltyPointsUsed;
    
    // Calculate tax
    tax = subtotal * taxRate;
    
    // Calculate final total
    finalTotal = subtotal + tax;
    
    // Calculate loyalty points earned
    if (customer) {
        loyaltyPointsEarned = finalTotal * 0.01; // 1% base rate
        if (customer->getType() == CustomerType::PREMIUM) {
            loyaltyPointsEarned *= 1.5;
        } else if (customer->getType() == CustomerType::VIP) {
            loyaltyPointsEarned *= 2.0;
        }
    }
}

bool Transaction::processPayment(PaymentMethod method, double amountPaid) {
    if (finalTotal <= 0) {
        return false;
    }
    
    paymentMethod = method;
    
    // For cash payments, check if enough money was provided
    if (method == PaymentMethod::CASH && amountPaid < finalTotal) {
        std::cout << "Insufficient payment. Required: $" << std::fixed << std::setprecision(2) 
                  << finalTotal << ", Provided: $" << amountPaid << std::endl;
        return false;
    }
    
    return true;
}

bool Transaction::applyLoyaltyPoints(double points) {
    if (!customer || customer->getLoyaltyPoints() < points) {
        return false;
    }
    
    loyaltyPointsUsed = points;
    return true;
}

void Transaction::finalizeTransaction() {
    if (status != TransactionStatus::PENDING) {
        return;
    }
    
    // Reduce stock for all items
    for (const auto& item : items) {
        if (item.product) {
            item.product->reduceStock(static_cast<int>(std::ceil(item.quantity)));
        }
    }
    
    // Update customer data
    if (customer) {
        customer->addPurchase(finalTotal);
        if (loyaltyPointsUsed > 0) {
            customer->redeemLoyaltyPoints(loyaltyPointsUsed);
        }
        customer->addLoyaltyPoints(loyaltyPointsEarned);
    }
    
    status = TransactionStatus::COMPLETED;
}

std::string Transaction::getPaymentMethodString() const {
    switch (paymentMethod) {
        case PaymentMethod::CASH: return "Cash";
        case PaymentMethod::CREDIT_CARD: return "Credit Card";
        case PaymentMethod::DEBIT_CARD: return "Debit Card";
        case PaymentMethod::MOBILE_PAYMENT: return "Mobile Payment";
        case PaymentMethod::LOYALTY_POINTS: return "Loyalty Points";
        case PaymentMethod::GIFT_CARD: return "Gift Card";
        default: return "Unknown";
    }
}

std::string Transaction::getStatusString() const {
    switch (status) {
        case TransactionStatus::PENDING: return "Pending";
        case TransactionStatus::COMPLETED: return "Completed";
        case TransactionStatus::CANCELLED: return "Cancelled";
        case TransactionStatus::REFUNDED: return "Refunded";
        case TransactionStatus::PARTIALLY_REFUNDED: return "Partially Refunded";
        default: return "Unknown";
    }
}

void Transaction::printReceipt() const {
    std::cout << "\n" << std::string(40, '=') << std::endl;
    std::cout << "           CONVENIENCE STORE           " << std::endl;
    std::cout << "               RECEIPT                 " << std::endl;
    std::cout << std::string(40, '=') << std::endl;
    
    std::cout << "Transaction ID: " << transactionId << std::endl;
    std::cout << "Date: " << std::ctime(&timestamp);
    std::cout << "Cashier: " << cashierId << std::endl;
    
    if (customer) {
        std::cout << "Customer: " << customer->getFullName() 
                  << " (" << customer->getTypeString() << ")" << std::endl;
    }
    
    std::cout << std::string(40, '-') << std::endl;
    
    for (const auto& item : items) {
        item.displayItem();
    }
    
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << std::endl;
    
    if (totalDiscount > 0) {
        std::cout << "Discount: -$" << std::fixed << std::setprecision(2) << totalDiscount << std::endl;
    }
    
    if (loyaltyPointsUsed > 0) {
        std::cout << "Loyalty Points Used: -$" << std::fixed << std::setprecision(2) 
                  << loyaltyPointsUsed << std::endl;
    }
    
    std::cout << "Tax: $" << std::fixed << std::setprecision(2) << tax << std::endl;
    std::cout << "TOTAL: $" << std::fixed << std::setprecision(2) << finalTotal << std::endl;
    
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "Payment Method: " << getPaymentMethodString() << std::endl;
    std::cout << "Status: " << getStatusString() << std::endl;
    
    if (customer && loyaltyPointsEarned > 0) {
        std::cout << "Loyalty Points Earned: " << std::fixed << std::setprecision(2) 
                  << loyaltyPointsEarned << std::endl;
        std::cout << "Total Loyalty Points: " << std::fixed << std::setprecision(2) 
                  << customer->getLoyaltyPoints() << std::endl;
    }
    
    std::cout << std::string(40, '=') << std::endl;
    std::cout << "    Thank you for shopping with us!    " << std::endl;
    std::cout << std::string(40, '=') << std::endl << std::endl;
}

bool Transaction::processRefund(double amount) {
    if (status != TransactionStatus::COMPLETED) {
        return false;
    }
    
    if (amount == -1.0) {
        amount = finalTotal; // Full refund
    }
    
    if (amount > finalTotal) {
        return false;
    }
    
    // Return items to stock
    for (const auto& item : items) {
        if (item.product) {
            item.product->addStock(static_cast<int>(std::ceil(item.quantity)));
        }
    }
    
    // Update customer data
    if (customer) {
        customer->addPurchase(-amount); // Subtract from total spent
        if (loyaltyPointsEarned > 0) {
            customer->redeemLoyaltyPoints(loyaltyPointsEarned); // Remove earned points
        }
    }
    
    if (amount >= finalTotal) {
        status = TransactionStatus::REFUNDED;
    } else {
        status = TransactionStatus::PARTIALLY_REFUNDED;
    }
    
    return true;
}

void Transaction::printDetailedReceipt() const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "           DETAILED TRANSACTION RECEIPT        " << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "Transaction ID: " << transactionId << std::endl;
    std::cout << "Date & Time: " << std::ctime(&timestamp);
    std::cout << "Cashier: " << cashierId << std::endl;
    std::cout << "Status: " << getStatusString() << std::endl;
    
    if (customer) {
        std::cout << "\nCustomer Information:" << std::endl;
        std::cout << "  Name: " << customer->getFullName() << std::endl;
        std::cout << "  Type: " << customer->getTypeString() << std::endl;
        std::cout << "  ID: " << customer->getId() << std::endl;
        std::cout << "  Discount Rate: " << (customer->getDiscountRate() * 100) << "%" << std::endl;
    }
    
    std::cout << "\n" << std::string(50, '-') << std::endl;
    std::cout << "ITEMS PURCHASED:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];
        std::cout << (i + 1) << ". ";
        item.displayItem();
        
        if (!item.notes.empty()) {
            std::cout << "    Note: " << item.notes << std::endl;
        }
    }
    
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "FINANCIAL BREAKDOWN:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    double itemTotal = 0.0;
    for (const auto& item : items) {
        itemTotal += item.subtotal;
    }
    
    std::cout << "Items Subtotal: $" << std::fixed << std::setprecision(2) << itemTotal << std::endl;
    
    if (totalDiscount > 0) {
        std::cout << "Total Discounts: -$" << std::fixed << std::setprecision(2) << totalDiscount << std::endl;
        std::cout << "After Discounts: $" << std::fixed << std::setprecision(2) << (itemTotal - totalDiscount) << std::endl;
    }
    
    if (loyaltyPointsUsed > 0) {
        std::cout << "Loyalty Points Used: -$" << std::fixed << std::setprecision(2) << loyaltyPointsUsed << std::endl;
    }
    
    std::cout << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << std::endl;
    std::cout << "Tax (8%): $" << std::fixed << std::setprecision(2) << tax << std::endl;
    std::cout << "FINAL TOTAL: $" << std::fixed << std::setprecision(2) << finalTotal << std::endl;
    
    std::cout << "\n" << std::string(50, '-') << std::endl;
    std::cout << "PAYMENT INFORMATION:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Payment Method: " << getPaymentMethodString() << std::endl;
    std::cout << "Amount Paid: $" << std::fixed << std::setprecision(2) << finalTotal << std::endl;
    
    if (customer && loyaltyPointsEarned > 0) {
        std::cout << "\nLOYALTY PROGRAM:" << std::endl;
        std::cout << "Points Earned: " << std::fixed << std::setprecision(2) << loyaltyPointsEarned << std::endl;
        std::cout << "Current Points Balance: " << std::fixed << std::setprecision(2) << customer->getLoyaltyPoints() << std::endl;
    }
    
    if (!notes.empty()) {
        std::cout << "\nTransaction Notes: " << notes << std::endl;
    }
    
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "    Thank you for shopping with us!    " << std::endl;
    std::cout << "         Please come again!           " << std::endl;
    std::cout << std::string(50, '=') << std::endl << std::endl;
}
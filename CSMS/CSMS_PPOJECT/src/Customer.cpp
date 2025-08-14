// ===== Customer.cpp =====
#include "Customer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>

int CustomerDatabase::nextCustomerId = 1001;

Customer::Customer(const std::string& id, const std::string& fName, const std::string& lName,
                   const std::string& email, const std::string& phone, CustomerType type)
    : customerId(id), firstName(fName), lastName(lName), email(email), phone(phone),
      type(type), totalSpent(0.0), transactionCount(0), loyaltyPoints(0.0), isActive(true) {
    
    // Set membership date (simplified)
    membershipDate = "2025-08-14"; // Current date placeholder
}

void Customer::addPurchase(double amount) {
    totalSpent += amount;
    transactionCount++;
    
    // Add loyalty points based on customer type
    double pointsMultiplier = 1.0;
    switch (type) {
        case CustomerType::PREMIUM: pointsMultiplier = 1.5; break;
        case CustomerType::VIP: pointsMultiplier = 2.0; break;
        case CustomerType::EMPLOYEE: pointsMultiplier = 3.0; break;
        default: pointsMultiplier = 1.0; break;
    }
    
    addLoyaltyPoints(amount * 0.01 * pointsMultiplier); // 1% base rate
}

double Customer::getDiscountRate() const {
    switch (type) {
        case CustomerType::PREMIUM: return 0.05; // 5% discount
        case CustomerType::VIP: return 0.10;     // 10% discount
        case CustomerType::EMPLOYEE: return 0.15; // 15% discount
        default: return 0.0;                     // No discount
    }
}

void Customer::addLoyaltyPoints(double points) {
    loyaltyPoints += points;
}

bool Customer::redeemLoyaltyPoints(double points) {
    if (loyaltyPoints >= points) {
        loyaltyPoints -= points;
        return true;
    }
    return false;
}

std::string Customer::getTypeString() const {
    switch (type) {
        case CustomerType::REGULAR: return "Regular";
        case CustomerType::PREMIUM: return "Premium";
        case CustomerType::VIP: return "VIP";
        case CustomerType::EMPLOYEE: return "Employee";
        default: return "Unknown";
    }
}

bool Customer::isEligibleForUpgrade() const {
    if (type == CustomerType::REGULAR && totalSpent >= 500.0) {
        return true;
    }
    if (type == CustomerType::PREMIUM && totalSpent >= 2000.0) {
        return true;
    }
    return false;
}

void Customer::displayInfo() const {
    std::cout << "\n========== Customer Information ==========\n";
    std::cout << "ID: " << customerId << "\n";
    std::cout << "Name: " << getFullName() << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "Phone: " << phone << "\n";
    std::cout << "Type: " << getTypeString() << "\n";
    std::cout << "Total Spent: $" << std::fixed << std::setprecision(2) << totalSpent << "\n";
    std::cout << "Transaction Count: " << transactionCount << "\n";
    std::cout << "Loyalty Points: " << std::fixed << std::setprecision(2) << loyaltyPoints << "\n";
    std::cout << "Discount Rate: " << (getDiscountRate() * 100) << "%\n";
    std::cout << "Member Since: " << membershipDate << "\n";
    std::cout << "Status: " << (isActive ? "Active" : "Inactive") << "\n";
    
    if (isEligibleForUpgrade()) {
        std::cout << "  Eligible for membership upgrade!\n";
    }
    
    std::cout << "==========================================\n";
}

// CustomerDatabase implementation
CustomerDatabase::~CustomerDatabase() {
    for (auto& pair : customers) {
        delete pair.second;
    }
}

Customer* CustomerDatabase::addCustomer(const std::string& firstName, const std::string& lastName,
                                       const std::string& email, const std::string& phone,
                                       CustomerType type) {
    std::string customerId = "C" + std::to_string(nextCustomerId++);
    Customer* customer = new Customer(customerId, firstName, lastName, email, phone, type);
    customers[customerId] = customer;
    return customer;
}

Customer* CustomerDatabase::findCustomer(const std::string& customerId) {
    auto it = customers.find(customerId);
    return (it != customers.end()) ? it->second : nullptr;
}

Customer* CustomerDatabase::findCustomerByEmail(const std::string& email) {
    for (const auto& pair : customers) {
        if (pair.second->getEmail() == email) {
            return pair.second;
        }
    }
    return nullptr;
}

Customer* CustomerDatabase::findCustomerByPhone(const std::string& phone) {
    for (const auto& pair : customers) {
        if (pair.second->getPhone() == phone) {
            return pair.second;
        }
    }
    return nullptr;
}

std::vector<Customer*> CustomerDatabase::getCustomersByType(CustomerType type) {
    std::vector<Customer*> result;
    for (const auto& pair : customers) {
        if (pair.second->getType() == type) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<Customer*> CustomerDatabase::getTopCustomers(int count) {
    std::vector<Customer*> allCustomers;
    for (const auto& pair : customers) {
        allCustomers.push_back(pair.second);
    }
    
    std::sort(allCustomers.begin(), allCustomers.end(),
              [](const Customer* a, const Customer* b) {
                  return a->getTotalSpent() > b->getTotalSpent();
              });
    
    if (count > static_cast<int>(allCustomers.size())) {
        count = allCustomers.size();
    }
    
    return std::vector<Customer*>(allCustomers.begin(), allCustomers.begin() + count);
}

void CustomerDatabase::displayAllCustomers() const {
    std::cout << "\n========== All Customers ==========\n";
    for (const auto& pair : customers) {
        const Customer* customer = pair.second;
        std::cout << "ID: " << customer->getId() 
                  << " | Name: " << customer->getFullName()
                  << " | Type: " << customer->getTypeString()
                  << " | Total Spent: $" << std::fixed << std::setprecision(2) << customer->getTotalSpent()
                  << " | Points: " << customer->getLoyaltyPoints() << "\n";
    }
    std::cout << "===================================\n\n";
}

int CustomerDatabase::getTotalCustomerCount() const {
    return customers.size();
}

double CustomerDatabase::getTotalCustomerSpending() const {
    double total = 0.0;
    for (const auto& pair : customers) {
        total += pair.second->getTotalSpent();
    }
    return total;
}

void CustomerDatabase::displayCustomerStatistics()  {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "              CUSTOMER STATISTICS           " << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    int totalCustomers = customers.size();
    double totalSpending = getTotalCustomerSpending();
    
    std::cout << "Total Customers: " << totalCustomers << std::endl;
    std::cout << "Total Customer Spending: $" << std::fixed << std::setprecision(2) 
              << totalSpending << std::endl;
    
    if (totalCustomers > 0) {
        std::cout << "Average Spending per Customer: $" << std::fixed << std::setprecision(2) 
                  << (totalSpending / totalCustomers) << std::endl;
    }
    
    // Customer type distribution
    auto regularCustomers = getCustomersByType(CustomerType::REGULAR);
    auto premiumCustomers = getCustomersByType(CustomerType::PREMIUM);
    auto vipCustomers = getCustomersByType(CustomerType::VIP);
    auto employeeCustomers = getCustomersByType(CustomerType::EMPLOYEE);
    
    std::cout << "\nCustomer Type Distribution:" << std::endl;
    std::cout << "  Regular: " << regularCustomers.size() << std::endl;
    std::cout << "  Premium: " << premiumCustomers.size() << std::endl;
    std::cout << "  VIP: " << vipCustomers.size() << std::endl;
    std::cout << "  Employee: " << employeeCustomers.size() << std::endl;
    
    // Top customers
    auto topCustomers = getTopCustomers(3);
    std::cout << "\nTop 3 Customers:" << std::endl;
    for (size_t i = 0; i < topCustomers.size(); ++i) {
        const Customer* customer = topCustomers[i];
        std::cout << "  " << (i + 1) << ". " << customer->getFullName() 
                  << " - $" << std::fixed << std::setprecision(2) << customer->getTotalSpent() << std::endl;
    }
    
    std::cout << std::string(60, '=') << std::endl << std::endl;
}

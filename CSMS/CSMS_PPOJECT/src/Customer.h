
// ===== Customer.h =====
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief Enumeration for customer types
 */
enum class CustomerType {
    REGULAR,
    PREMIUM,
    VIP,
    EMPLOYEE
};

/**
 * @brief Class representing a customer
 */
class Customer {
private:
    std::string customerId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    CustomerType type;
    double totalSpent;
    int transactionCount;
    double loyaltyPoints;
    std::string membershipDate;
    bool isActive;

public:
    Customer(const std::string& id, const std::string& fName, const std::string& lName,
             const std::string& email = "", const std::string& phone = "",
             CustomerType type = CustomerType::REGULAR);

    // Getters
    std::string getId() const { return customerId; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const { return firstName + " " + lastName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    CustomerType getType() const { return type; }
    double getTotalSpent() const { return totalSpent; }
    int getTransactionCount() const { return transactionCount; }
    double getLoyaltyPoints() const { return loyaltyPoints; }
    std::string getMembershipDate() const { return membershipDate; }
    bool getIsActive() const { return isActive; }

    // Setters
    void setEmail(const std::string& email) { this->email = email; }
    void setPhone(const std::string& phone) { this->phone = phone; }
    void setType(CustomerType type) { this->type = type; }
    void setIsActive(bool active) { isActive = active; }

    // Business methods
    void addPurchase(double amount);
    double getDiscountRate() const;
    void addLoyaltyPoints(double points);
    bool redeemLoyaltyPoints(double points);
    
    // Utility methods
    std::string getTypeString() const;
    void displayInfo() const;
    bool isEligibleForUpgrade() const;
};

/**
 * @brief Customer database management
 */
class CustomerDatabase {
private:
    std::map<std::string, Customer*> customers;
    static int nextCustomerId;

public:
    ~CustomerDatabase();
    
    Customer* addCustomer(const std::string& firstName, const std::string& lastName,
                         const std::string& email = "", const std::string& phone = "",
                         CustomerType type = CustomerType::REGULAR);
    
    Customer* findCustomer(const std::string& customerId);
    Customer* findCustomerByEmail(const std::string& email);
    Customer* findCustomerByPhone(const std::string& phone);
    
    std::vector<Customer*> getCustomersByType(CustomerType type);
    std::vector<Customer*> getTopCustomers(int count = 10);
    
    void displayAllCustomers() const;
    void displayCustomerStatistics() ;
    
    int getTotalCustomerCount() const;
    double getTotalCustomerSpending() const;
};

#endif // CUSTOMER_H
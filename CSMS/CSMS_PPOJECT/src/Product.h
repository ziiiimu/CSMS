// ===== Product.h =====
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

/**
 * @brief Enumeration for product categories
 */
enum class ProductCategory {
    BEVERAGES,
    SNACKS,
    DAIRY,
    BAKERY,
    HOUSEHOLD,
    ELECTRONICS,
    HEALTH_BEAUTY,
    OTHER
};

/**
 * @brief Base class for all products in the store
 */
class Product {
protected:
    std::string productId;
    std::string name;
    std::string description;
    double basePrice;
    int currentStock;
    int minStockLevel;
    int maxStockLevel;
    ProductCategory category;
    std::string supplier;
    std::string barcode;
    double costPrice;        // Purchase price from supplier
    bool isActive;           // Whether product is currently being sold
    std::vector<std::string> tags;  // Search tags for the product

public:
    /**
     * @brief Constructor for Product
     */
    Product(const std::string& id, const std::string& name, const std::string& desc,
            double price, double cost, int stock, ProductCategory cat,
            const std::string& supplier = "", int minStock = 10, int maxStock = 1000);

    // Virtual destructor for proper inheritance
    virtual ~Product() = default;

    // Pure virtual methods that derived classes must implement
    virtual double calculateSellingPrice() const = 0;
    virtual std::string getProductType() const = 0;
    virtual void displayDetailedInfo() const;

    // Getters
    std::string getId() const { return productId; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    double getBasePrice() const { return basePrice; }
    double getCostPrice() const { return costPrice; }
    int getCurrentStock() const { return currentStock; }
    int getMinStockLevel() const { return minStockLevel; }
    int getMaxStockLevel() const { return maxStockLevel; }
    ProductCategory getCategory() const { return category; }
    std::string getSupplier() const { return supplier; }
    std::string getBarcode() const { return barcode; }
    bool getIsActive() const { return isActive; }
    const std::vector<std::string>& getTags() const { return tags; }

    // Setters
    void setBasePrice(double price) { basePrice = price; }
    void setCostPrice(double cost) { costPrice = cost; }
    void setMinStockLevel(int minStock) { minStockLevel = minStock; }
    void setMaxStockLevel(int maxStock) { maxStockLevel = maxStock; }
    void setIsActive(bool active) { isActive = active; }
    void setDescription(const std::string& desc) { description = desc; }

    // Stock management
    bool reduceStock(int quantity);
    void addStock(int quantity);
    bool isLowStock() const;
    bool isOverstocked() const;
    int getRestockRecommendation() const;

    // Business logic
    double calculateProfitMargin() const;
    double getTotalInventoryValue() const;
    double getTotalInventoryCost() const;
    
    // Tag management
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;

    // Utility methods
    std::string categoryToString() const;
    static ProductCategory stringToCategory(const std::string& categoryStr);
};

/**
 * @brief Regular product with standard pricing
 */
class RegularProduct : public Product {
private:
    double markupPercentage;  // Markup percentage over cost price

public:
    RegularProduct(const std::string& id, const std::string& name, const std::string& desc,
                   double price, double cost, int stock, ProductCategory cat,
                   const std::string& supplier = "", double markup = 0.3,
                   int minStock = 10, int maxStock = 1000);

    double calculateSellingPrice() const override;
    std::string getProductType() const override { return "Regular"; }
    
    double getMarkupPercentage() const { return markupPercentage; }
    void setMarkupPercentage(double markup) { markupPercentage = markup; }
};

/**
 * @brief Perishable product with expiration dates
 */
class PerishableProduct : public Product {
private:
    std::string expirationDate;
    int shelfLifeDays;
    double discountRate;  // Discount rate when near expiration

public:
    PerishableProduct(const std::string& id, const std::string& name, const std::string& desc,
                      double price, double cost, int stock, ProductCategory cat,
                      const std::string& expDate, int shelfLife,
                      const std::string& supplier = "", double discount = 0.2,
                      int minStock = 5, int maxStock = 500);

    double calculateSellingPrice() const override;
    std::string getProductType() const override { return "Perishable"; }
    void displayDetailedInfo() const override;
    
    bool isNearExpiration() const;
    int getDaysUntilExpiration() const;
    
    // Getters and setters
    std::string getExpirationDate() const { return expirationDate; }
    int getShelfLifeDays() const { return shelfLifeDays; }
    double getDiscountRate() const { return discountRate; }
    void setExpirationDate(const std::string& date) { expirationDate = date; }
    void setDiscountRate(double rate) { discountRate = rate; }
};

/**
 * @brief Bulk product sold by weight or volume
 */
class BulkProduct : public Product {
private:
    std::string unit;  // kg, lbs, liters, etc.
    double pricePerUnit;
    double minimumQuantity;

public:
    BulkProduct(const std::string& id, const std::string& name, const std::string& desc,
                double pricePerUnit, double cost, int stock, ProductCategory cat,
                const std::string& unit, double minQty = 0.1,
                const std::string& supplier = "", int minStock = 10, int maxStock = 1000);

    double calculateSellingPrice() const override;
    double calculatePriceForQuantity(double quantity) const;
    std::string getProductType() const override { return "Bulk"; }
    void displayDetailedInfo() const override;
    
    // Getters
    std::string getUnit() const { return unit; }
    double getPricePerUnit() const { return pricePerUnit; }
    double getMinimumQuantity() const { return minimumQuantity; }
    
    // Setters
    void setPricePerUnit(double price) { pricePerUnit = price; }
    void setMinimumQuantity(double minQty) { minimumQuantity = minQty; }
};

#endif // PRODUCT_H

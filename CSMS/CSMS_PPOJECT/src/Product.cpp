
// ===== Product.cpp =====
#include "Product.h"
#include <algorithm>
#include <sstream>
#include <ctime>

// Product base class implementation
Product::Product(const std::string &id, const std::string &name, const std::string &desc,
                 double price, double cost, int stock, ProductCategory cat,
                 const std::string &supplier, int minStock, int maxStock)
    : productId(id), name(name), description(desc), basePrice(price), costPrice(cost),
      currentStock(stock), minStockLevel(minStock), maxStockLevel(maxStock),
      category(cat), supplier(supplier), isActive(true)
{

    // Generate a simple barcode (in real system, this would be more sophisticated)
    barcode = "BAR" + id;
}

bool Product::reduceStock(int quantity)
{
    if (currentStock >= quantity && quantity > 0)
    {
        currentStock -= quantity;
        return true;
    }
    return false;
}

void Product::addStock(int quantity)
{
    if (quantity > 0)
    {
        currentStock += quantity;
        if (currentStock > maxStockLevel)
        {
            currentStock = maxStockLevel;
        }
    }
}

bool Product::isLowStock() const
{
    return currentStock <= minStockLevel;
}

bool Product::isOverstocked() const
{
    return currentStock >= maxStockLevel * 0.9;
}

int Product::getRestockRecommendation() const
{
    if (isLowStock())
    {
        return maxStockLevel - currentStock;
    }
    return 0;
}

double Product::calculateProfitMargin() const
{
    if (costPrice == 0)
        return 0;
    return ((calculateSellingPrice() - costPrice) / costPrice) * 100;
}

double Product::getTotalInventoryValue() const
{
    return calculateSellingPrice() * currentStock;
}

double Product::getTotalInventoryCost() const
{
    return costPrice * currentStock;
}

void Product::addTag(const std::string &tag)
{
    if (std::find(tags.begin(), tags.end(), tag) == tags.end())
    {
        tags.push_back(tag);
    }
}

void Product::removeTag(const std::string &tag)
{
    tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
}

bool Product::hasTag(const std::string &tag) const
{
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

std::string Product::categoryToString() const
{
    switch (category)
    {
    case ProductCategory::BEVERAGES:
        return "Beverages";
    case ProductCategory::SNACKS:
        return "Snacks";
    case ProductCategory::DAIRY:
        return "Dairy";
    case ProductCategory::BAKERY:
        return "Bakery";
    case ProductCategory::HOUSEHOLD:
        return "Household";
    case ProductCategory::ELECTRONICS:
        return "Electronics";
    case ProductCategory::HEALTH_BEAUTY:
        return "Health & Beauty";
    default:
        return "Other";
    }
}

ProductCategory Product::stringToCategory(const std::string &categoryStr)
{
    if (categoryStr == "Beverages")
        return ProductCategory::BEVERAGES;
    if (categoryStr == "Snacks")
        return ProductCategory::SNACKS;
    if (categoryStr == "Dairy")
        return ProductCategory::DAIRY;
    if (categoryStr == "Bakery")
        return ProductCategory::BAKERY;
    if (categoryStr == "Household")
        return ProductCategory::HOUSEHOLD;
    if (categoryStr == "Electronics")
        return ProductCategory::ELECTRONICS;
    if (categoryStr == "Health & Beauty")
        return ProductCategory::HEALTH_BEAUTY;
    return ProductCategory::OTHER;
}

void Product::displayDetailedInfo() const
{
    std::cout << "\n========== Product Details ==========\n";
    std::cout << "ID: " << productId << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Description: " << description << "\n";
    std::cout << "Category: " << categoryToString() << "\n";
    std::cout << "Type: " << getProductType() << "\n";
    std::cout << "Selling Price: $" << std::fixed << std::setprecision(2) << calculateSellingPrice() << "\n";
    std::cout << "Cost Price: $" << std::fixed << std::setprecision(2) << costPrice << "\n";
    std::cout << "Profit Margin: " << std::fixed << std::setprecision(1) << calculateProfitMargin() << "%\n";
    std::cout << "Current Stock: " << currentStock << "\n";
    std::cout << "Min Stock Level: " << minStockLevel << "\n";
    std::cout << "Max Stock Level: " << maxStockLevel << "\n";
    std::cout << "Supplier: " << supplier << "\n";
    std::cout << "Barcode: " << barcode << "\n";
    std::cout << "Status: " << (isActive ? "Active" : "Inactive") << "\n";

    if (isLowStock())
    {
        std::cout << "   LOW STOCK ALERT! Restock recommended: " << getRestockRecommendation() << " units\n";
    }

    if (!tags.empty())
    {
        std::cout << "Tags: ";
        for (size_t i = 0; i < tags.size(); ++i)
        {
            std::cout << tags[i];
            if (i < tags.size() - 1)
                std::cout << ", ";
        }
        std::cout << "\n";
    }

    std::cout << "====================================\n";
}

// RegularProduct implementation
RegularProduct::RegularProduct(const std::string &id, const std::string &name, const std::string &desc,
                               double price, double cost, int stock, ProductCategory cat,
                               const std::string &supplier, double markup,
                               int minStock, int maxStock)
    : Product(id, name, desc, price, cost, stock, cat, supplier, minStock, maxStock),
      markupPercentage(markup) {}

double RegularProduct::calculateSellingPrice() const
{
    return costPrice * (1.0 + markupPercentage);
}

// PerishableProduct implementation
PerishableProduct::PerishableProduct(const std::string &id, const std::string &name, const std::string &desc,
                                     double price, double cost, int stock, ProductCategory cat,
                                     const std::string &expDate, int shelfLife,
                                     const std::string &supplier, double discount,
                                     int minStock, int maxStock)
    : Product(id, name, desc, price, cost, stock, cat, supplier, minStock, maxStock),
      expirationDate(expDate), shelfLifeDays(shelfLife), discountRate(discount) {}

double PerishableProduct::calculateSellingPrice() const
{
    double price = basePrice;
    if (isNearExpiration())
    {
        price *= (1.0 - discountRate);
    }
    return price;
}

bool PerishableProduct::isNearExpiration() const
{
    int daysLeft = getDaysUntilExpiration();
    return daysLeft <= (shelfLifeDays * 0.2); // Within 20% of shelf life
}

int PerishableProduct::getDaysUntilExpiration() const
{
    // Simplified implementation - in real system would use proper date parsing
    // For demo purposes, assume expiration date format is "YYYY-MM-DD"
    return 5; // Placeholder
}

void PerishableProduct::displayDetailedInfo() const
{
    Product::displayDetailedInfo();
    std::cout << "Expiration Date: " << expirationDate << "\n";
    std::cout << "Shelf Life: " << shelfLifeDays << " days\n";
    std::cout << "Days Until Expiration: " << getDaysUntilExpiration() << "\n";

    if (isNearExpiration())
    {
        std::cout << "  NEAR EXPIRATION! " << (discountRate * 100) << "% discount applied\n";
    }

    std::cout << "====================================\n";
}

// BulkProduct implementation
BulkProduct::BulkProduct(const std::string &id, const std::string &name, const std::string &desc,
                         double pricePerUnit, double cost, int stock, ProductCategory cat,
                         const std::string &unit, double minQty,
                         const std::string &supplier, int minStock, int maxStock)
    : Product(id, name, desc, pricePerUnit, cost, stock, cat, supplier, minStock, maxStock),
      unit(unit), pricePerUnit(pricePerUnit), minimumQuantity(minQty) {}

double BulkProduct::calculateSellingPrice() const
{
    return pricePerUnit;
}

double BulkProduct::calculatePriceForQuantity(double quantity) const
{
    if (quantity < minimumQuantity)
    {
        quantity = minimumQuantity;
    }
    return pricePerUnit * quantity;
}

void BulkProduct::displayDetailedInfo() const
{
    Product::displayDetailedInfo();
    std::cout << "Unit: " << unit << "\n";
    std::cout << "Price per " << unit << ": $" << std::fixed << std::setprecision(2) << pricePerUnit << "\n";
    std::cout << "Minimum Quantity: " << minimumQuantity << " " << unit << "\n";
    std::cout << "====================================\n";
}
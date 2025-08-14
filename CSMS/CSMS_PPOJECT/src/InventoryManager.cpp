// ===== InventoryManager.cpp =====
#include "InventoryManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>

InventoryManager::~InventoryManager() {
    for (auto& pair : products) {
        delete pair.second;
    }
}

bool InventoryManager::addProduct(Product* product) {
    if (!product || products.find(product->getId()) != products.end()) {
        return false;
    }
    
    products[product->getId()] = product;
    updateCategoryMapping(product);
    updateSupplierMapping(product);
    return true;
}

bool InventoryManager::removeProduct(const std::string& productId) {
    auto it = products.find(productId);
    if (it == products.end()) {
        return false;
    }
    
    Product* product = it->second;
    removeCategoryMapping(product);
    removeSupplierMapping(product);
    
    delete product;
    products.erase(it);
    return true;
}

Product* InventoryManager::findProduct(const std::string& productId) {
    auto it = products.find(productId);
    return (it != products.end()) ? it->second : nullptr;
}

std::vector<Product*> InventoryManager::findProductsByName(const std::string& name) {
    std::vector<Product*> result;
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    for (const auto& pair : products) {
        std::string productName = pair.second->getName();
        std::transform(productName.begin(), productName.end(), productName.begin(), ::tolower);
        
        if (productName.find(lowerName) != std::string::npos) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

std::vector<Product*> InventoryManager::findProductsByTag(const std::string& tag) {
    std::vector<Product*> result;
    
    for (const auto& pair : products) {
        if (pair.second->hasTag(tag)) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

std::vector<Product*> InventoryManager::getProductsByCategory(ProductCategory category) const {
    auto it = productsByCategory.find(category);
    return (it != productsByCategory.end()) ? it->second : std::vector<Product*>();
}

std::vector<Product*> InventoryManager::getProductsBySupplier(const std::string& supplier) const {
    auto it = productsBySupplier.find(supplier);
    return (it != productsBySupplier.end()) ? it->second : std::vector<Product*>();
}

std::vector<std::string> InventoryManager::getAllSuppliers() const {
    std::vector<std::string> suppliers;
    for (const auto& pair : productsBySupplier) {
        suppliers.push_back(pair.first);
    }
    return suppliers;
}

std::vector<Product*> InventoryManager::getLowStockProducts() const {
    std::vector<Product*> result;
    
    for (const auto& pair : products) {
        if (pair.second->isLowStock() && pair.second->getIsActive()) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

std::vector<Product*> InventoryManager::getOverstockedProducts() const {
    std::vector<Product*> result;
    
    for (const auto& pair : products) {
        if (pair.second->isOverstocked() && pair.second->getIsActive()) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

std::vector<Product*> InventoryManager::getOutOfStockProducts() const {
    std::vector<Product*> result;
    
    for (const auto& pair : products) {
        if (pair.second->getCurrentStock() == 0 && pair.second->getIsActive()) {
            result.push_back(pair.second);
        }
    }
    
    return result;
}

double InventoryManager::getTotalInventoryValue() const {
    double total = 0.0;
    for (const auto& pair : products) {
        if (pair.second->getIsActive()) {
            total += pair.second->getTotalInventoryValue();
        }
    }
    return total;
}

double InventoryManager::getTotalInventoryCost() const {
    double total = 0.0;
    for (const auto& pair : products) {
        if (pair.second->getIsActive()) {
            total += pair.second->getTotalInventoryCost();
        }
    }
    return total;
}

double InventoryManager::getTotalPotentialProfit() const {
    return getTotalInventoryValue() - getTotalInventoryCost();
}

double InventoryManager::getCategoryValue(ProductCategory category) const {
    double total = 0.0;
    auto products = getProductsByCategory(category);
    
    for (Product* product : products) {
        if (product->getIsActive()) {
            total += product->getTotalInventoryValue();
        }
    }
    
    return total;
}

void InventoryManager::generateInventoryReport() const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                INVENTORY REPORT                " << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    std::cout << "Total Products: " << getTotalProductCount() << std::endl;
    std::cout << "Active Products: " << getActiveProductCount() << std::endl;
    std::cout << "Total Inventory Value: $" << std::fixed << std::setprecision(2) 
              << getTotalInventoryValue() << std::endl;
    std::cout << "Total Inventory Cost: $" << std::fixed << std::setprecision(2) 
              << getTotalInventoryCost() << std::endl;
    std::cout << "Potential Profit: $" << std::fixed << std::setprecision(2) 
              << getTotalPotentialProfit() << std::endl;
    
    auto lowStockProducts = getLowStockProducts();
    auto outOfStockProducts = getOutOfStockProducts();
    auto overstockedProducts = getOverstockedProducts();
    
    std::cout << "\nStock Status:" << std::endl;
    std::cout << "  Low Stock Items: " << lowStockProducts.size() << std::endl;
    std::cout << "  Out of Stock Items: " << outOfStockProducts.size() << std::endl;
    std::cout << "  Overstocked Items: " << overstockedProducts.size() << std::endl;
    
    std::cout << std::string(60, '=') << std::endl << std::endl;
}

void InventoryManager::generateLowStockReport() const {
    auto lowStockProducts = getLowStockProducts();
    auto outOfStockProducts = getOutOfStockProducts();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                LOW STOCK REPORT                " << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    if (outOfStockProducts.empty() && lowStockProducts.empty()) {
        std::cout << "  All products are adequately stocked!" << std::endl;
    } else {
        if (!outOfStockProducts.empty()) {
            std::cout << "\n  OUT OF STOCK (" << outOfStockProducts.size() << " items):" << std::endl;
            for (Product* product : outOfStockProducts) {
                std::cout << "  " << product->getId() << " - " << product->getName() 
                          << " (Restock: " << product->getRestockRecommendation() << ")" << std::endl;
            }
        }
        
        if (!lowStockProducts.empty()) {
            std::cout << "\n   LOW STOCK (" << lowStockProducts.size() << " items):" << std::endl;
            for (Product* product : lowStockProducts) {
                std::cout << "  " << product->getId() << " - " << product->getName() 
                          << " (Current: " << product->getCurrentStock() 
                          << ", Min: " << product->getMinStockLevel()
                          << ", Restock: " << product->getRestockRecommendation() << ")" << std::endl;
            }
        }
    }
    
    std::cout << std::string(60, '=') << std::endl << std::endl;
}

void InventoryManager::updateCategoryMapping(Product* product) {
    productsByCategory[product->getCategory()].push_back(product);
}

void InventoryManager::updateSupplierMapping(Product* product) {
    if (!product->getSupplier().empty()) {
        productsBySupplier[product->getSupplier()].push_back(product);
    }
}

void InventoryManager::removeCategoryMapping(Product* product) {
    auto& categoryProducts = productsByCategory[product->getCategory()];
    categoryProducts.erase(std::remove(categoryProducts.begin(), categoryProducts.end(), product), 
                          categoryProducts.end());
}

void InventoryManager::removeSupplierMapping(Product* product) {
    if (!product->getSupplier().empty()) {
        auto& supplierProducts = productsBySupplier[product->getSupplier()];
        supplierProducts.erase(std::remove(supplierProducts.begin(), supplierProducts.end(), product), 
                              supplierProducts.end());
    }
}

int InventoryManager::getTotalProductCount() const {
    return products.size();
}

int InventoryManager::getActiveProductCount() const {
    int count = 0;
    for (const auto& pair : products) {
        if (pair.second->getIsActive()) {
            count++;
        }
    }
    return count;
}

void InventoryManager::displayAllProducts() const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                ALL PRODUCTS                " << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    if (products.empty()) {
        std::cout << "No products in inventory." << std::endl;
    } else {
        for (const auto& pair : products) {
            const Product* product = pair.second;
            std::cout << "ID: " << product->getId() 
                      << " | Name: " << product->getName()
                      << " | Price: $" << std::fixed << std::setprecision(2) << product->calculateSellingPrice()
                      << " | Stock: " << product->getCurrentStock()
                      << " | Category: " << product->categoryToString();
            
            if (product->isLowStock()) {
                std::cout << " [LOW STOCK]";
            }
            if (!product->getIsActive()) {
                std::cout << " [INACTIVE]";
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::string(60, '=') << std::endl << std::endl;
}

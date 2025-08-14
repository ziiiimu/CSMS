// ===== InventoryManager.h =====
#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "Product.h"
#include <map>
#include <vector>
#include <string>

/**
 * @brief Advanced inventory management system
 */
class InventoryManager {
private:
    std::map<std::string, Product*> products;
    std::map<ProductCategory, std::vector<Product*>> productsByCategory;
    std::map<std::string, std::vector<Product*>> productsBySupplier;
    
public:
    ~InventoryManager();
    
    // Product management
    bool addProduct(Product* product);
    bool removeProduct(const std::string& productId);
    Product* findProduct(const std::string& productId);
    std::vector<Product*> findProductsByName(const std::string& name);
    std::vector<Product*> findProductsByTag(const std::string& tag);
    
    // Category and supplier management
    std::vector<Product*> getProductsByCategory(ProductCategory category) const; 
    std::vector<Product*> getProductsBySupplier(const std::string& supplier) const;
    std::vector<std::string> getAllSuppliers() const;
    
    // Stock management
    std::vector<Product*> getLowStockProducts() const;
    std::vector<Product*> getOverstockedProducts() const;
    std::vector<Product*> getOutOfStockProducts() const;
    
    // Financial calculations
    double getTotalInventoryValue() const;
    double getTotalInventoryCost() const;
    double getTotalPotentialProfit() const;
    double getCategoryValue(ProductCategory category) const;
    
    // Reports and analytics
    void generateInventoryReport() const;
    void generateLowStockReport() const;
    void generateCategoryReport() const;
    void generateSupplierReport() const;
    void generateProfitabilityReport() const;
    
    // Bulk operations
    void updateAllPrices(double percentageChange);
    void updateCategoryPrices(ProductCategory category, double percentageChange);
    void deactivateExpiredProducts();
    
    // Display methods
    void displayAllProducts() const;
    void displayProductsByCategory(ProductCategory category) const;
    void displayLowStockAlert() const;
    
    // Utility methods
    int getTotalProductCount() const;
    int getActiveProductCount() const;
    std::vector<Product*> searchProducts(const std::string& searchTerm) const;
    
private:
    void updateCategoryMapping(Product* product);
    void updateSupplierMapping(Product* product);
    void removeCategoryMapping(Product* product);
    void removeSupplierMapping(Product* product);
};

#endif // INVENTORY_MANAGER_H
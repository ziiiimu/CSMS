// ===== Main.cpp =====
#include "Product.h"
#include "Customer.h"
#include "Transaction.h"
#include "InventoryManager.h"
#include <iostream>
#include <string>
#include <memory>

/**
 * @brief Main application class for the Convenience Store Management System
 */
class ConvenienceStoreApp
{
private:
    InventoryManager inventory;
    CustomerDatabase customerDB;
    std::vector<Transaction *> transactions;
    std::string currentCashierId;

public:
    ConvenienceStoreApp() : currentCashierId("CASHIER001")
    {
        initializeTestData();
    }

    ~ConvenienceStoreApp()
    {
        for (auto *transaction : transactions)
        {
            delete transaction;
        }
    }

    void run()
    {
        std::cout << "  Welcome to Advanced Convenience Store Management System!" << std::endl;

        int choice;
        do
        {
            displayMainMenu();
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                handleInventoryMenu();
                break;
            case 2:
                handleCustomerMenu();
                break;
            case 3:
                handleSalesMenu();
                break;
            case 4:
                handleReportsMenu();
                break;
            case 5:
                handleSettingsMenu();
                break;
            case 0:
                std::cout << "Thank you for using CSMS!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
            }
        } while (choice != 0);
    }

private:
    void displayMainMenu()
    {
        std::cout << "\n"
                  << std::string(50, '=') << std::endl;
        std::cout << "    CONVENIENCE STORE MANAGEMENT SYSTEM    " << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1.   Inventory Management" << std::endl;
        std::cout << "2.   Customer Management" << std::endl;
        std::cout << "3.   Sales & Transactions" << std::endl;
        std::cout << "4.   Reports & Analytics" << std::endl;
        std::cout << "5.   Settings" << std::endl;
        std::cout << "0.   Exit" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Choose an option: ";
    }

    void initializeTestData()
    {
        // Add sample products
        inventory.addProduct(new RegularProduct("P001", "Coca Cola 330ml", "Classic Coca Cola can",
                                                2.50, 1.20, 50, ProductCategory::BEVERAGES, "Coca Cola Co", 0.3));

        inventory.addProduct(new RegularProduct("P002", "Lay's Chips Original", "Crispy potato chips",
                                                3.00, 1.50, 30, ProductCategory::SNACKS, "Frito-Lay", 0.25));

        inventory.addProduct(new PerishableProduct("P003", "Fresh Milk 1L", "Whole milk",
                                                   4.00, 2.50, 15, ProductCategory::DAIRY, "2025-08-20", 7, "Dairy Farm"));

        inventory.addProduct(new BulkProduct("P004", "Rice Premium", "Premium jasmine rice",
                                             2.50, 1.80, 100, ProductCategory::OTHER, "kg", 0.5, "Rice Supplier"));

        inventory.addProduct(new RegularProduct("P005", "Chocolate Bar", "Dark chocolate bar",
                                                2.00, 1.00, 8, ProductCategory::SNACKS, "Chocolate Co", 0.4));

        // Add sample customers
        customerDB.addCustomer("John", "Doe", "john.doe@email.com", "+1234567890", CustomerType::REGULAR);
        customerDB.addCustomer("Jane", "Smith", "jane.smith@email.com", "+1234567891", CustomerType::PREMIUM);
        customerDB.addCustomer("Bob", "Johnson", "bob.johnson@email.com", "+1234567892", CustomerType::VIP);
    }

    void handleInventoryMenu()
    {
        int choice;
        do
        {
            std::cout << "\n--- INVENTORY MANAGEMENT ---" << std::endl;
            std::cout << "1. View All Products" << std::endl;
            std::cout << "2. Add New Product" << std::endl;
            std::cout << "3. Search Products" << std::endl;
            std::cout << "4. Update Stock" << std::endl;
            std::cout << "5. Low Stock Alert" << std::endl;
            std::cout << "6. Inventory Reports" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                inventory.displayAllProducts();
                break;
            case 2:
                addNewProduct();
                break;
            case 3:
                searchProducts();
                break;
            case 4:
                updateStock();
                break;
            case 5:
                inventory.generateLowStockReport();
                break;
            case 6:
                inventory.generateInventoryReport();
                break;
            }
        } while (choice != 0);
    }

    void addNewProduct()
    {
        std::string id, name, desc, supplier;
        double price, cost;
        int stock, minStock, maxStock, productType;

        std::cout << "\n--- ADD NEW PRODUCT ---" << std::endl;
        std::cout << "Product ID: ";
        std::cin >> id;

        if (inventory.findProduct(id))
        {
            std::cout << "Product with ID " << id << " already exists!" << std::endl;
            return;
        }

        std::cin.ignore();
        std::cout << "Product Name: ";
        std::getline(std::cin, name);
        std::cout << "Description: ";
        std::getline(std::cin, desc);
        std::cout << "Selling Price: $";
        std::cin >> price;
        std::cout << "Cost Price: $";
        std::cin >> cost;
        std::cout << "Initial Stock: ";
        std::cin >> stock;
        std::cout << "Minimum Stock Level: ";
        std::cin >> minStock;
        std::cout << "Maximum Stock Level: ";
        std::cin >> maxStock;

        std::cin.ignore();
        std::cout << "Supplier: ";
        std::getline(std::cin, supplier);

        std::cout << "\nProduct Type:" << std::endl;
        std::cout << "1. Regular Product" << std::endl;
        std::cout << "2. Perishable Product" << std::endl;
        std::cout << "3. Bulk Product" << std::endl;
        std::cout << "Choose type: ";
        std::cin >> productType;

        std::cout << "\nCategory:" << std::endl;
        std::cout << "1. Beverages" << std::endl;
        std::cout << "2. Snacks" << std::endl;
        std::cout << "3. Dairy" << std::endl;
        std::cout << "4. Bakery" << std::endl;
        std::cout << "5. Household" << std::endl;
        std::cout << "6. Electronics" << std::endl;
        std::cout << "7. Health & Beauty" << std::endl;
        std::cout << "8. Other" << std::endl;

        int categoryChoice;
        std::cout << "Choose category: ";
        std::cin >> categoryChoice;

        ProductCategory category = static_cast<ProductCategory>(categoryChoice - 1);
        Product *newProduct = nullptr;

        switch (productType)
        {
        case 1:
        {
            double markup;
            std::cout << "Markup Percentage (e.g., 0.3 for 30%): ";
            std::cin >> markup;
            newProduct = new RegularProduct(id, name, desc, price, cost, stock,
                                            category, supplier, markup, minStock, maxStock);
            break;
        }
        case 2:
        {
            std::string expDate;
            int shelfLife;
            double discount;
            std::cin.ignore();
            std::cout << "Expiration Date (YYYY-MM-DD): ";
            std::getline(std::cin, expDate);
            std::cout << "Shelf Life (days): ";
            std::cin >> shelfLife;
            std::cout << "Near-expiration Discount Rate (e.g., 0.2 for 20%): ";
            std::cin >> discount;
            newProduct = new PerishableProduct(id, name, desc, price, cost, stock,
                                               category, expDate, shelfLife, supplier,
                                               discount, minStock, maxStock);
            break;
        }
        case 3:
        {
            std::string unit;
            double minQty;
            std::cin.ignore();
            std::cout << "Unit (kg, lbs, liters, etc.): ";
            std::getline(std::cin, unit);
            std::cout << "Minimum Quantity: ";
            std::cin >> minQty;
            newProduct = new BulkProduct(id, name, desc, price, cost, stock,
                                         category, unit, minQty, supplier, minStock, maxStock);
            break;
        }
        default:
            std::cout << "Invalid product type!" << std::endl;
            return;
        }

        if (newProduct && inventory.addProduct(newProduct))
        {
            std::cout << "  Product added successfully!" << std::endl;
        }
        else
        {
            std::cout << "  Failed to add product!" << std::endl;
            delete newProduct;
        }
    }

    void searchProducts()
    {
        std::string searchTerm;
        std::cout << "\nEnter search term (name or tag): ";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);

        auto nameResults = inventory.findProductsByName(searchTerm);
        auto tagResults = inventory.findProductsByTag(searchTerm);

        std::cout << "\n--- SEARCH RESULTS ---" << std::endl;

        if (!nameResults.empty())
        {
            std::cout << "Products matching name:" << std::endl;
            for (Product *product : nameResults)
            {
                std::cout << "  " << product->getId() << " - " << product->getName()
                          << " ($" << std::fixed << std::setprecision(2)
                          << product->calculateSellingPrice() << ")" << std::endl;
            }
        }

        if (!tagResults.empty())
        {
            std::cout << "Products matching tag:" << std::endl;
            for (Product *product : tagResults)
            {
                std::cout << "  " << product->getId() << " - " << product->getName()
                          << " ($" << std::fixed << std::setprecision(2)
                          << product->calculateSellingPrice() << ")" << std::endl;
            }
        }

        if (nameResults.empty() && tagResults.empty())
        {
            std::cout << "No products found matching: " << searchTerm << std::endl;
        }
    }

    void updateStock()
    {
        std::string productId;
        int quantity;
        char operation;

        std::cout << "\nProduct ID: ";
        std::cin >> productId;

        Product *product = inventory.findProduct(productId);
        if (!product)
        {
            std::cout << "Product not found!" << std::endl;
            return;
        }

        std::cout << "Current Stock: " << product->getCurrentStock() << std::endl;
        std::cout << "Operation (+ to add, - to remove): ";
        std::cin >> operation;
        std::cout << "Quantity: ";
        std::cin >> quantity;

        if (operation == '+')
        {
            product->addStock(quantity);
            std::cout << "  Stock added! New stock: " << product->getCurrentStock() << std::endl;
        }
        else if (operation == '-')
        {
            if (product->reduceStock(quantity))
            {
                std::cout << "  Stock reduced! New stock: " << product->getCurrentStock() << std::endl;
            }
            else
            {
                std::cout << "  Insufficient stock!" << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid operation!" << std::endl;
        }
    }

    void handleCustomerMenu()
    {
        int choice;
        do
        {
            std::cout << "\n--- CUSTOMER MANAGEMENT ---" << std::endl;
            std::cout << "1. View All Customers" << std::endl;
            std::cout << "2. Add New Customer" << std::endl;
            std::cout << "3. Search Customer" << std::endl;
            std::cout << "4. Customer Details" << std::endl;
            std::cout << "5. Customer Statistics" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                customerDB.displayAllCustomers();
                break;
            case 2:
                addNewCustomer();
                break;
            case 3:
                searchCustomer();
                break;
            case 4:
                viewCustomerDetails();
                break;
            case 5:
                customerDB.displayCustomerStatistics();
                break;
            }
        } while (choice != 0);
    }

    void addNewCustomer()
    {
        std::string firstName, lastName, email, phone;
        int typeChoice;

        std::cout << "\n--- ADD NEW CUSTOMER ---" << std::endl;
        std::cin.ignore();
        std::cout << "First Name: ";
        std::getline(std::cin, firstName);
        std::cout << "Last Name: ";
        std::getline(std::cin, lastName);
        std::cout << "Email: ";
        std::getline(std::cin, email);
        std::cout << "Phone: ";
        std::getline(std::cin, phone);

        std::cout << "\nCustomer Type:" << std::endl;
        std::cout << "1. Regular" << std::endl;
        std::cout << "2. Premium" << std::endl;
        std::cout << "3. VIP" << std::endl;
        std::cout << "4. Employee" << std::endl;
        std::cout << "Choose type: ";
        std::cin >> typeChoice;

        CustomerType type = static_cast<CustomerType>(typeChoice - 1);
        Customer *customer = customerDB.addCustomer(firstName, lastName, email, phone, type);

        if (customer)
        {
            std::cout << "  Customer added successfully! ID: " << customer->getId() << std::endl;
        }
        else
        {
            std::cout << "  Failed to add customer!" << std::endl;
        }
    }

    void searchCustomer()
    {
        std::string searchTerm;
        int searchType;

        std::cout << "\nSearch by:" << std::endl;
        std::cout << "1. Customer ID" << std::endl;
        std::cout << "2. Email" << std::endl;
        std::cout << "3. Phone" << std::endl;
        std::cout << "Choose search type: ";
        std::cin >> searchType;

        std::cin.ignore();
        std::cout << "Enter search term: ";
        std::getline(std::cin, searchTerm);

        Customer *customer = nullptr;

        switch (searchType)
        {
        case 1:
            customer = customerDB.findCustomer(searchTerm);
            break;
        case 2:
            customer = customerDB.findCustomerByEmail(searchTerm);
            break;
        case 3:
            customer = customerDB.findCustomerByPhone(searchTerm);
            break;
        default:
            std::cout << "Invalid search type!" << std::endl;
            return;
        }

        if (customer)
        {
            customer->displayInfo();
        }
        else
        {
            std::cout << "Customer not found!" << std::endl;
        }
    }

    void viewCustomerDetails()
    {
        std::string customerId;
        std::cout << "\nEnter Customer ID: ";
        std::cin >> customerId;

        Customer *customer = customerDB.findCustomer(customerId);
        if (customer)
        {
            customer->displayInfo();
        }
        else
        {
            std::cout << "Customer not found!" << std::endl;
        }
    }

    void handleSalesMenu()
    {
        int choice;
        do
        {
            std::cout << "\n--- SALES & TRANSACTIONS ---" << std::endl;
            std::cout << "1. New Transaction" << std::endl;
            std::cout << "2. View Transaction History" << std::endl;
            std::cout << "3. Process Refund" << std::endl;
            std::cout << "4. Transaction Details" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                processNewTransaction();
                break;
            case 2:
                viewTransactionHistory();
                break;
            case 3:
                processRefund();
                break;
            case 4:
                viewTransactionDetails();
                break;
            }
        } while (choice != 0);
    }

    void processNewTransaction()
    {
        std::cout << "\n--- NEW TRANSACTION ---" << std::endl;

        // Check if customer wants to provide customer info
        char hasCustomer;
        std::cout << "Is this for a registered customer? (y/n): ";
        std::cin >> hasCustomer;

        Customer *customer = nullptr;
        if (hasCustomer == 'y' || hasCustomer == 'Y')
        {
            std::string customerId;
            std::cout << "Enter Customer ID: ";
            std::cin >> customerId;
            customer = customerDB.findCustomer(customerId);

            if (!customer)
            {
                std::cout << "Customer not found! Proceeding without customer..." << std::endl;
            }
            else
            {
                std::cout << "Customer: " << customer->getFullName()
                          << " (" << customer->getTypeString() << ")" << std::endl;
            }
        }

        Transaction *transaction = new Transaction(customer, currentCashierId);

        // Add items to transaction
        std::string productId;
        while (true)
        {
            std::cout << "\nEnter Product ID (or 'done' to finish): ";
            std::cin >> productId;

            if (productId == "done")
                break;

            Product *product = inventory.findProduct(productId);
            if (!product)
            {
                std::cout << "Product not found!" << std::endl;
                continue;
            }

            if (!product->getIsActive())
            {
                std::cout << "Product is not active!" << std::endl;
                continue;
            }

            std::cout << "Product: " << product->getName()
                      << " ($" << std::fixed << std::setprecision(2)
                      << product->calculateSellingPrice() << ")" << std::endl;
            std::cout << "Available Stock: " << product->getCurrentStock() << std::endl;

            double quantity;
            std::cout << "Quantity: ";
            std::cin >> quantity;

            double discount = 0.0;
            char applyDiscount;
            std::cout << "Apply manual discount? (y/n): ";
            std::cin >> applyDiscount;

            if (applyDiscount == 'y' || applyDiscount == 'Y')
            {
                std::cout << "Discount percentage (0.1 for 10%): ";
                std::cin >> discount;
            }

            if (transaction->addItem(product, quantity, discount))
            {
                std::cout << "  Item added to transaction!" << std::endl;
            }
            else
            {
                std::cout << "  Failed to add item!" << std::endl;
            }
        }

        if (transaction->getItems().empty())
        {
            std::cout << "No items in transaction. Cancelling..." << std::endl;
            delete transaction;
            return;
        }

        // Apply loyalty points if customer exists
        if (customer && customer->getLoyaltyPoints() > 0)
        {
            char useLoyalty;
            std::cout << "\nCustomer has " << customer->getLoyaltyPoints()
                      << " loyalty points. Use them? (y/n): ";
            std::cin >> useLoyalty;

            if (useLoyalty == 'y' || useLoyalty == 'Y')
            {
                double pointsToUse;
                std::cout << "Points to use (max " << customer->getLoyaltyPoints() << "): ";
                std::cin >> pointsToUse;

                if (pointsToUse > 0 && pointsToUse <= customer->getLoyaltyPoints())
                {
                    transaction->applyLoyaltyPoints(pointsToUse);
                }
            }
        }

        // Calculate totals
        transaction->calculateTotals(0.08); // 8% tax rate

        // Show transaction summary
        std::cout << "\n--- TRANSACTION SUMMARY ---" << std::endl;
        std::cout << "Subtotal: $" << std::fixed << std::setprecision(2)
                  << transaction->getSubtotal() << std::endl;
        std::cout << "Tax: $" << std::fixed << std::setprecision(2)
                  << transaction->getTax() << std::endl;
        std::cout << "Total: $" << std::fixed << std::setprecision(2)
                  << transaction->getFinalTotal() << std::endl;

        // Process payment
        std::cout << "\nPayment Method:" << std::endl;
        std::cout << "1. Cash" << std::endl;
        std::cout << "2. Credit Card" << std::endl;
        std::cout << "3. Debit Card" << std::endl;
        std::cout << "4. Mobile Payment" << std::endl;
        std::cout << "Choose payment method: ";

        int paymentChoice;
        std::cin >> paymentChoice;

        PaymentMethod method = static_cast<PaymentMethod>(paymentChoice - 1);

        double amountPaid = 0.0;
        if (method == PaymentMethod::CASH)
        {
            std::cout << "Amount paid: $";
            std::cin >> amountPaid;
        }
        else
        {
            amountPaid = transaction->getFinalTotal();
        }

        if (transaction->processPayment(method, amountPaid))
        {
            transaction->finalizeTransaction();
            transactions.push_back(transaction);

            // Print receipt
            transaction->printReceipt();

            // Show change for cash payments
            if (method == PaymentMethod::CASH && amountPaid > transaction->getFinalTotal())
            {
                std::cout << "Change: $" << std::fixed << std::setprecision(2)
                          << (amountPaid - transaction->getFinalTotal()) << std::endl;
            }

            std::cout << "  Transaction completed successfully!" << std::endl;
        }
        else
        {
            std::cout << "  Payment failed!" << std::endl;
            delete transaction;
        }
    }

    void viewTransactionHistory()
    {
        std::cout << "\n--- TRANSACTION HISTORY ---" << std::endl;

        if (transactions.empty())
        {
            std::cout << "No transactions found." << std::endl;
            return;
        }

        for (const auto *transaction : transactions)
        {
            std::cout << "ID: " << transaction->getId()
                      << " | Total: $" << std::fixed << std::setprecision(2)
                      << transaction->getFinalTotal()
                      << " | Payment: " << transaction->getPaymentMethodString()
                      << " | Status: " << transaction->getStatusString();

            if (transaction->getCustomer())
            {
                std::cout << " | Customer: " << transaction->getCustomer()->getFullName();
            }

            std::time_t timestamp = transaction->getTimestamp();
            std::cout << " | Time: " << std::ctime(&timestamp);
        }
    }

    void processRefund()
    {
        int transactionId;
        std::cout << "\nEnter Transaction ID: ";
        std::cin >> transactionId;

        Transaction *transaction = nullptr;
        for (auto *t : transactions)
        {
            if (t->getId() == transactionId)
            {
                transaction = t;
                break;
            }
        }

        if (!transaction)
        {
            std::cout << "Transaction not found!" << std::endl;
            return;
        }

        if (transaction->getStatus() != TransactionStatus::COMPLETED)
        {
            std::cout << "Can only refund completed transactions!" << std::endl;
            return;
        }

        std::cout << "Transaction Total: $" << std::fixed << std::setprecision(2)
                  << transaction->getFinalTotal() << std::endl;

        char fullRefund;
        std::cout << "Full refund? (y/n): ";
        std::cin >> fullRefund;

        if (fullRefund == 'y' || fullRefund == 'Y')
        {
            if (transaction->processRefund())
            {
                std::cout << "  Full refund processed successfully!" << std::endl;
            }
            else
            {
                std::cout << "  Refund failed!" << std::endl;
            }
        }
        else
        {
            double refundAmount;
            std::cout << "Refund amount: $";
            std::cin >> refundAmount;

            if (transaction->processRefund(refundAmount))
            {
                std::cout << "  Partial refund processed successfully!" << std::endl;
            }
            else
            {
                std::cout << "  Refund failed!" << std::endl;
            }
        }
    }

    void viewTransactionDetails()
    {
        int transactionId;
        std::cout << "\nEnter Transaction ID: ";
        std::cin >> transactionId;

        Transaction *transaction = nullptr;
        for (auto *t : transactions)
        {
            if (t->getId() == transactionId)
            {
                transaction = t;
                break;
            }
        }

        if (transaction)
        {
            transaction->printDetailedReceipt();
        }
        else
        {
            std::cout << "Transaction not found!" << std::endl;
        }
    }

    void handleReportsMenu()
    {
        int choice;
        do
        {
            std::cout << "\n--- REPORTS & ANALYTICS ---" << std::endl;
            std::cout << "1. Inventory Report" << std::endl;
            std::cout << "2. Sales Report" << std::endl;
            std::cout << "3. Customer Analytics" << std::endl;
            std::cout << "4. Low Stock Alert" << std::endl;
            std::cout << "5. Financial Summary" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                inventory.generateInventoryReport();
                break;
            case 2:
                generateSalesReport();
                break;
            case 3:
                generateCustomerAnalytics();
                break;
            case 4:
                inventory.generateLowStockReport();
                break;
            case 5:
                generateFinancialSummary();
                break;
            }
        } while (choice != 0);
    }

    void generateSalesReport()
    {
        std::cout << "\n"
                  << std::string(60, '=') << std::endl;
        std::cout << "                SALES REPORT                " << std::endl;
        std::cout << std::string(60, '=') << std::endl;

        if (transactions.empty())
        {
            std::cout << "No transactions to report." << std::endl;
            std::cout << std::string(60, '=') << std::endl
                      << std::endl;
            return;
        }

        double totalSales = 0.0;
        double totalTax = 0.0;
        int completedTransactions = 0;
        int refundedTransactions = 0;

        for (const auto *transaction : transactions)
        {
            if (transaction->getStatus() == TransactionStatus::COMPLETED)
            {
                totalSales += transaction->getFinalTotal();
                totalTax += transaction->getTax();
                completedTransactions++;
            }
            else if (transaction->getStatus() == TransactionStatus::REFUNDED ||
                     transaction->getStatus() == TransactionStatus::PARTIALLY_REFUNDED)
            {
                refundedTransactions++;
            }
        }

        std::cout << "Total Transactions: " << transactions.size() << std::endl;
        std::cout << "Completed Transactions: " << completedTransactions << std::endl;
        std::cout << "Refunded Transactions: " << refundedTransactions << std::endl;
        std::cout << "Total Sales: $" << std::fixed << std::setprecision(2) << totalSales << std::endl;
        std::cout << "Total Tax Collected: $" << std::fixed << std::setprecision(2) << totalTax << std::endl;

        if (completedTransactions > 0)
        {
            std::cout << "Average Transaction: $" << std::fixed << std::setprecision(2)
                      << (totalSales / completedTransactions) << std::endl;
        }

        std::cout << std::string(60, '=') << std::endl
                  << std::endl;
    }

    void generateCustomerAnalytics()
    {
        auto topCustomers = customerDB.getTopCustomers(5);

        std::cout << "\n"
                  << std::string(60, '=') << std::endl;
        std::cout << "              CUSTOMER ANALYTICS             " << std::endl;
        std::cout << std::string(60, '=') << std::endl;

        std::cout << "Total Customers: " << customerDB.getTotalCustomerCount() << std::endl;
        std::cout << "Total Customer Spending: $" << std::fixed << std::setprecision(2)
                  << customerDB.getTotalCustomerSpending() << std::endl;

        std::cout << "\nTop 5 Customers by Spending:" << std::endl;
        for (size_t i = 0; i < topCustomers.size(); ++i)
        {
            const Customer *customer = topCustomers[i];
            std::cout << (i + 1) << ". " << customer->getFullName()
                      << " - $" << std::fixed << std::setprecision(2) << customer->getTotalSpent()
                      << " (" << customer->getTransactionCount() << " transactions)" << std::endl;
        }

        // Customer type distribution
        std::cout << "\nCustomer Type Distribution:" << std::endl;
        auto regularCustomers = customerDB.getCustomersByType(CustomerType::REGULAR);
        auto premiumCustomers = customerDB.getCustomersByType(CustomerType::PREMIUM);
        auto vipCustomers = customerDB.getCustomersByType(CustomerType::VIP);
        auto employeeCustomers = customerDB.getCustomersByType(CustomerType::EMPLOYEE);

        std::cout << "Regular: " << regularCustomers.size() << std::endl;
        std::cout << "Premium: " << premiumCustomers.size() << std::endl;
        std::cout << "VIP: " << vipCustomers.size() << std::endl;
        std::cout << "Employee: " << employeeCustomers.size() << std::endl;

        std::cout << std::string(60, '=') << std::endl
                  << std::endl;
    }

    void generateFinancialSummary()
    {
        std::cout << "\n"
                  << std::string(60, '=') << std::endl;
        std::cout << "              FINANCIAL SUMMARY             " << std::endl;
        std::cout << std::string(60, '=') << std::endl;

        double totalInventoryValue = inventory.getTotalInventoryValue();
        double totalInventoryCost = inventory.getTotalInventoryCost();
        double potentialProfit = inventory.getTotalPotentialProfit();

        double totalSales = 0.0;
        for (const auto *transaction : transactions)
        {
            if (transaction->getStatus() == TransactionStatus::COMPLETED)
            {
                totalSales += transaction->getFinalTotal();
            }
        }

        std::cout << "INVENTORY:" << std::endl;
        std::cout << "Total Inventory Value: $" << std::fixed << std::setprecision(2)
                  << totalInventoryValue << std::endl;
        std::cout << "Total Inventory Cost: $" << std::fixed << std::setprecision(2)
                  << totalInventoryCost << std::endl;
        std::cout << "Potential Profit: $" << std::fixed << std::setprecision(2)
                  << potentialProfit << std::endl;

        if (totalInventoryCost > 0)
        {
            double profitMargin = (potentialProfit / totalInventoryCost) * 100;
            std::cout << "Profit Margin: " << std::fixed << std::setprecision(1)
                      << profitMargin << "%" << std::endl;
        }

        std::cout << "\nSALES:" << std::endl;
        std::cout << "Total Sales Revenue: $" << std::fixed << std::setprecision(2)
                  << totalSales << std::endl;

        std::cout << "\nCUSTOMERS:" << std::endl;
        std::cout << "Total Customer Spending: $" << std::fixed << std::setprecision(2)
                  << customerDB.getTotalCustomerSpending() << std::endl;

        std::cout << std::string(60, '=') << std::endl
                  << std::endl;
    }

    void handleSettingsMenu()
    {
        int choice;
        do
        {
            std::cout << "\n--- SETTINGS ---" << std::endl;
            std::cout << "1. Change Cashier ID" << std::endl;
            std::cout << "2. System Information" << std::endl;
            std::cout << "3. Data Management" << std::endl;
            std::cout << "0. Back to Main Menu" << std::endl;
            std::cout << "Choose an option: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                changeCashierId();
                break;
            case 2:
                showSystemInfo();
                break;
            case 3:
                handleDataManagement();
                break;
            }
        } while (choice != 0);
    }

    void changeCashierId()
    {
        std::cout << "\nCurrent Cashier ID: " << currentCashierId << std::endl;
        std::cout << "Enter new Cashier ID: ";
        std::cin >> currentCashierId;
        std::cout << "  Cashier ID updated to: " << currentCashierId << std::endl;
    }

    void showSystemInfo()
    {
        std::cout << "\n--- SYSTEM INFORMATION ---" << std::endl;
        std::cout << "System: Advanced Convenience Store Management System" << std::endl;
        std::cout << "Version: 2.0" << std::endl;
        std::cout << "Current Cashier: " << currentCashierId << std::endl;
        std::cout << "Products in System: " << inventory.getTotalProductCount() << std::endl;
        std::cout << "Customers in System: " << customerDB.getTotalCustomerCount() << std::endl;
        std::cout << "Total Transactions: " << transactions.size() << std::endl;
    }

    void handleDataManagement()
    {
        std::cout << "\n--- DATA MANAGEMENT ---" << std::endl;
        std::cout << "1. Export Data (Placeholder)" << std::endl;
        std::cout << "2. Import Data (Placeholder)" << std::endl;
        std::cout << "3. Backup System (Placeholder)" << std::endl;
        std::cout << "Note: Data management features would be implemented" << std::endl;
        std::cout << "with file I/O operations in a complete system." << std::endl;
    }
};

int main()
{
    try
    {
        ConvenienceStoreApp app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
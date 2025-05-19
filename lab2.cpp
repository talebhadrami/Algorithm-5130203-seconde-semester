#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <limits>
#include <cmath>
#include <map>

using namespace std;

// ==============================================
// Warehouse and Product Management System
// ==============================================

// Enum for warehouse types
enum class WarehouseType {
    CENTER,
    WEST,
    EAST
};

// Function to convert WarehouseType to string
string warehouseTypeToString(WarehouseType type) {
    switch (type) {
        case WarehouseType::CENTER: return "Center";
        case WarehouseType::WEST: return "West";
        case WarehouseType::EAST: return "East";
        default: return "Unknown";
    }
}

class Product {
private:
    string barcode;
    string description;
    double price;
    int quantity;
    double transportLongitude;
    double transportLatitude;

    // Helper function to generate random barcode
    string generateBarcode() {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> thousand_dist(1, 9);
        static uniform_int_distribution<> random_dist(0, 999999);
        
        int random_thousand = thousand_dist(gen) * 1000;
        int random_num = random_dist(gen);
        
        char buf[14];
        snprintf(buf, sizeof(buf), "460%04d%06d", random_thousand, random_num);
        return string(buf);
    }

public:
    // Constructors
    Product() : barcode(generateBarcode()), description(""), price(0.0), quantity(0), 
                transportLongitude(0.0), transportLatitude(0.0) {}

    Product(string desc, double pr, int qty, double tLong, double tLat) : 
        barcode(generateBarcode()), description(desc), price(pr), quantity(qty),
        transportLongitude(tLong), transportLatitude(tLat) {
        // Validate coordinates
        if (transportLongitude < 19 || transportLongitude > 169 || 
            transportLatitude < 41 || transportLatitude > 82) {
            throw invalid_argument("Coordinates must be within Russia (longitude 19-169, latitude 41-82)");
        }
    }

    // Copy constructor
    Product(const Product& other) : 
        barcode(other.barcode), description(other.description), price(other.price),
        quantity(other.quantity), transportLongitude(other.transportLongitude),
        transportLatitude(other.transportLatitude) {}

    // Getters
    string getBarcode() const { return barcode; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    double getTransportLongitude() const { return transportLongitude; }
    double getTransportLatitude() const { return transportLatitude; }

    // Setters with validation
    void setDescription(const string& desc) { 
        if (desc.length() > 50) {
            throw invalid_argument("Description must be 50 characters or less");
        }
        description = desc; 
    }

    void setPrice(double pr) { 
        if (pr < 0) {
            throw invalid_argument("Price cannot be negative");
        }
        price = pr; 
    }

    void setQuantity(int qty) { 
        if (qty < 0) {
            throw invalid_argument("Quantity cannot be negative");
        }
        quantity = qty; 
    }

    void setTransportCoordinates(double tLong, double tLat) {
        if (tLong < 19 || tLong > 169 || tLat < 41 || tLat > 82) {
            throw invalid_argument("Coordinates must be within Russia (longitude 19-169, latitude 41-82)");
        }
        transportLongitude = tLong;
        transportLatitude = tLat;
    }

    // Print product information
    void print() const {
        cout << "Barcode: " << barcode << "\n"
             << "Description: " << description << "\n"
             << "Price: " << fixed << setprecision(2) << price << " RUB\n"
             << "Quantity: " << quantity << "\n"
             << "Transport Coordinates: (" << transportLongitude << ", " << transportLatitude << ")\n";
    }
};

class Warehouse {
private:
    string id;
    WarehouseType type;
    double longitude;
    double latitude;
    int maxCapacity;
    int totalStock;
    vector<Product> products;
    static int warehouseCounter; // For generating unique IDs

    // Helper function to generate warehouse ID
    string generateId() {
        char buf[5];
        snprintf(buf, sizeof(buf), "W%03d", 100 + warehouseCounter++);
        return string(buf);
    }

public:
    // Constructors
    Warehouse() : id(generateId()), type(WarehouseType::CENTER), longitude(0.0), 
                  latitude(0.0), maxCapacity(0), totalStock(0) {}

    Warehouse(WarehouseType t, double lon, double lat, int cap) : 
        id(generateId()), type(t), longitude(lon), latitude(lat), 
        maxCapacity(cap), totalStock(0) {
        // Validate coordinates
        if (longitude < 19 || longitude > 169 || latitude < 41 || latitude > 82) {
            throw invalid_argument("Coordinates must be within Russia (longitude 19-169, latitude 41-82)");
        }
        if (cap <= 0) {
            throw invalid_argument("Capacity must be positive");
        }
    }

    // Copy constructor
    Warehouse(const Warehouse& other) : 
        id(other.id), type(other.type), longitude(other.longitude),
        latitude(other.latitude), maxCapacity(other.maxCapacity),
        totalStock(other.totalStock), products(other.products) {}

    // Getters
    string getId() const { return id; }
    WarehouseType getType() const { return type; }
    double getLongitude() const { return longitude; }
    double getLatitude() const { return latitude; }
    int getMaxCapacity() const { return maxCapacity; }
    int getTotalStock() const { return totalStock; }
    const vector<Product>& getProducts() const { return products; }

    // Calculate Manhattan distance to a product
    double calculateDistance(const Product& product) const {
        return abs(latitude - product.getTransportLatitude()) + 
               abs(longitude - product.getTransportLongitude());
    }

    // Add a product to the warehouse
    bool addProduct(const Product& product) {
        if (totalStock + product.getQuantity() > maxCapacity) {
            return false; // Not enough capacity
        }
        
        // Check if product with same barcode already exists
        auto it = find_if(products.begin(), products.end(), 
            [&product](const Product& p) { return p.getBarcode() == product.getBarcode(); });
        
        if (it != products.end()) {
            // Update existing product
            it->setQuantity(it->getQuantity() + product.getQuantity());
        } else {
            // Add new product
            products.push_back(product);
        }
        
        totalStock += product.getQuantity();
        return true;
    }

    // Remove a product from the warehouse
    bool removeProduct(const string& barcode, int quantity) {
        auto it = find_if(products.begin(), products.end(), 
            [&barcode](const Product& p) { return p.getBarcode() == barcode; });
        
        if (it == products.end()) {
            return false; // Product not found
        }
        
        if (quantity > it->getQuantity()) {
            return false; // Not enough quantity to remove
        }
        
        if (quantity == it->getQuantity()) {
            // Remove the product entirely
            totalStock -= it->getQuantity();
            products.erase(it);
        } else {
            // Reduce the quantity
            it->setQuantity(it->getQuantity() - quantity);
            totalStock -= quantity;
        }
        
        return true;
    }

    // Find products by description (case insensitive partial match)
    vector<Product> findProductsByDescription(const string& desc) const {
        vector<Product> result;
        string lowerDesc = desc;
        transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(), ::tolower);
        
        for (const auto& product : products) {
            string productDesc = product.getDescription();
            transform(productDesc.begin(), productDesc.end(), productDesc.begin(), ::tolower);
            
            if (productDesc.find(lowerDesc) != string::npos) {
                result.push_back(product);
            }
        }
        
        return result;
    }

    // Print warehouse information
    void print() const {
        cout << "Warehouse ID: " << id << "\n"
             << "Type: " << warehouseTypeToString(type) << "\n"
             << "Coordinates: (" << longitude << ", " << latitude << ")\n"
             << "Capacity: " << totalStock << "/" << maxCapacity << "\n"
             << "Products stored: " << products.size() << "\n";
    }

    // Print all products in the warehouse
    void printProducts() const {
        if (products.empty()) {
            cout << "No products in this warehouse.\n";
            return;
        }
        
        cout << "Products in warehouse " << id << ":\n";
        for (const auto& product : products) {
            product.print();
            cout << "--------------------\n";
        }
    }
};

// Initialize static counter
int Warehouse::warehouseCounter = 0;

// Function to find the best warehouse for a product
Warehouse* findBestWarehouse(vector<Warehouse>& warehouses, const Product& product) {
    if (warehouses.empty()) {
        return nullptr;
    }
    
    Warehouse* best = &warehouses[0];
    double minDistance = best->calculateDistance(product);
    
    for (size_t i = 1; i < warehouses.size(); ++i) {
        double distance = warehouses[i].calculateDistance(product);
        if (distance < minDistance) {
            minDistance = distance;
            best = &warehouses[i];
        }
    }
    
    return best;
}

// Function to display the warehouse main menu
void displayWarehouseMenu() {
    cout << "\nWarehouse Management System\n"
         << "1. Add a product\n"
         << "2. List all products in a warehouse\n"
         << "3. Search for products by description\n"
         << "4. Remove a product\n"
         << "5. Display warehouse information\n"
         << "6. Switch to Polynomial System\n"
         << "7. Exit\n"
         << "Enter your choice: ";
}

// ==============================================
// Polynomial Class Implementation
// ==============================================

class Polynomial {
private:
    vector<double> coefficients; // Coefficients a0, a1, a2, ..., an
    vector<int> exponents;      // Exponents 0, 1, 2, ..., n
    int degree;                 // Degree of the polynomial (n)

    // Helper function to normalize the polynomial (remove terms with zero coefficients)
    void normalize() {
        for (int i = coefficients.size() - 1; i >= 0; --i) {
            if (coefficients[i] != 0.0) {
                degree = i;
                break;
            }
        }
        
        // Remove terms with zero coefficients (except the constant term)
        for (size_t i = 0; i < coefficients.size(); ) {
            if (i != 0 && coefficients[i] == 0.0) {
                coefficients.erase(coefficients.begin() + i);
                exponents.erase(exponents.begin() + i);
            } else {
                ++i;
            }
        }
    }

public:
    // Constructors
    Polynomial() : degree(-1) {} // Zero polynomial
    
    Polynomial(int deg) : degree(deg) {
        if (deg < 0) {
            throw invalid_argument("Degree cannot be negative");
        }
        coefficients.resize(deg + 1, 0.0);
        exponents.resize(deg + 1);
        for (int i = 0; i <= deg; ++i) {
            exponents[i] = i;
        }
    }
    
    Polynomial(const vector<double>& coeffs) {
        if (coeffs.empty()) {
            degree = -1;
            return;
        }
        
        coefficients = coeffs;
        exponents.resize(coeffs.size());
        for (size_t i = 0; i < coeffs.size(); ++i) {
            exponents[i] = i;
        }
        degree = coefficients.size() - 1;
        normalize();
    }
    
    // Copy constructor
    Polynomial(const Polynomial& other) : 
        coefficients(other.coefficients), exponents(other.exponents), degree(other.degree) {}

    // Getters
    int getDegree() const { return degree; }
    const vector<double>& getCoefficients() const { return coefficients; }
    const vector<int>& getExponents() const { return exponents; }

    // Set coefficient for a specific term
    void setCoefficient(int exponent, double value) {
        if (exponent < 0) {
            throw invalid_argument("Exponent cannot be negative");
        }
        
        if (exponent > degree) {
            // Resize the polynomial to accommodate higher degree
            coefficients.resize(exponent + 1, 0.0);
            exponents.resize(exponent + 1);
            for (int i = degree + 1; i <= exponent; ++i) {
                exponents[i] = i;
            }
            degree = exponent;
        }
        
        coefficients[exponent] = value;
        normalize();
    }

    // Evaluate the polynomial for a given x
    double evaluate(double x) const {
        if (degree == -1) return 0.0; // Zero polynomial
        
        double result = 0.0;
        for (int i = 0; i <= degree; ++i) {
            if (coefficients[i] != 0.0) {
                result += coefficients[i] * pow(x, i);
            }
        }
        return result;
    }

    // Print the polynomial
    void print() const {
        if (degree == -1) {
            cout << "0";
            return;
        }
        
        bool firstTerm = true;
        for (int i = degree; i >= 0; --i) {
            if (coefficients[i] == 0.0 && i != 0) continue;
            
            if (!firstTerm) {
                cout << (coefficients[i] >= 0 ? " + " : " - ");
            }
            
            double absCoeff = abs(coefficients[i]);
            if (i == 0 || absCoeff != 1.0) {
                if (firstTerm) {
                    cout << coefficients[i];
                } else {
                    cout << absCoeff;
                }
            } else if (coefficients[i] == -1.0 && !firstTerm) {
                cout << "-";
            }
            
            if (i > 0) {
                cout << "x";
                if (i > 1) {
                    cout << "^" << i;
                }
            }
            
            firstTerm = false;
        }
        cout << "\n";
    }

    // Polynomial addition
    Polynomial operator+(const Polynomial& other) const {
        int maxDegree = max(degree, other.degree);
        Polynomial result(maxDegree);
        
        for (int i = 0; i <= maxDegree; ++i) {
            double coeff = 0.0;
            if (i <= degree) coeff += coefficients[i];
            if (i <= other.degree) coeff += other.coefficients[i];
            result.setCoefficient(i, coeff);
        }
        
        return result;
    }

    // Polynomial subtraction
    Polynomial operator-(const Polynomial& other) const {
        int maxDegree = max(degree, other.degree);
        Polynomial result(maxDegree);
        
        for (int i = 0; i <= maxDegree; ++i) {
            double coeff = 0.0;
            if (i <= degree) coeff += coefficients[i];
            if (i <= other.degree) coeff -= other.coefficients[i];
            result.setCoefficient(i, coeff);
        }
        
        return result;
    }
};

class VectPolynomial {
private:
    vector<Polynomial> polynomials;

public:
    // Constructors
    VectPolynomial() = default;
    VectPolynomial(const vector<Polynomial>& polys) : polynomials(polys) {}
    VectPolynomial(const VectPolynomial& other) : polynomials(other.polynomials) {}

    // Add a polynomial to the vector
    void addPolynomial(const Polynomial& poly) {
        polynomials.push_back(poly);
    }

    // Get the number of polynomials
    size_t size() const {
        return polynomials.size();
    }

    // Get a polynomial by index
    const Polynomial& getPolynomial(size_t index) const {
        if (index >= polynomials.size()) {
            throw out_of_range("Index out of range");
        }
        return polynomials[index];
    }

    // Get all polynomials
    const vector<Polynomial>& getAllPolynomials() const {
        return polynomials;
    }

    // Print all polynomials
    void printAll() const {
        for (size_t i = 0; i < polynomials.size(); ++i) {
            cout << "Polynomial " << i + 1 << ": ";
            polynomials[i].print();
        }
    }
};

// Global function to process two vectors of polynomials
VectPolynomial processPolynomials(const VectPolynomial& v1, const VectPolynomial& v2) {
    size_t size = min(v1.size(), v2.size());
    VectPolynomial result;
    
    for (size_t i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            // Even index: v1's even with v2's odd (but since i starts at 0, this is opposite)
            // This might need adjustment based on exact requirements
            result.addPolynomial(v1.getPolynomial(i) + v2.getPolynomial(i));
        } else {
            result.addPolynomial(v1.getPolynomial(i) - v2.getPolynomial(i));
        }
    }
    
    return result;
}

// Function to display the polynomial menu
void displayPolynomialMenu() {
    cout << "\nPolynomial Operations Menu\n"
         << "1. Create a new polynomial\n"
         << "2. Display all polynomials\n"
         << "3. Evaluate a polynomial\n"
         << "4. Process two vectors of polynomials\n"
         << "5. Switch to Warehouse System\n"
         << "6. Exit\n"
         << "Enter your choice: ";
}

// ==============================================
// Utility Functions
// ==============================================

// Function to get a valid double input within a range
double getValidDouble(const string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            return value;
        }
        cout << "Invalid input. Please enter a value between " << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Function to get a valid integer input with a minimum value
int getValidInt(const string& prompt, int minVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal) {
            return value;
        }
        cout << "Invalid input. Please enter an integer >= " << minVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Function to get a valid integer input
int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "Invalid input. Please enter an integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Function to get a valid double input
double getValidDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ==============================================
// Main Program
// ==============================================

void runWarehouseSystem() {
    // Create warehouses
    vector<Warehouse> warehouses = {
        Warehouse(WarehouseType::CENTER, 55.75, 37.61, 1000),  // Moscow
        Warehouse(WarehouseType::WEST, 30.31, 59.93, 800),      // Saint Petersburg
        Warehouse(WarehouseType::EAST, 135.05, 48.48, 1200)     // Vladivostok
    };

    int choice;
    do {
        displayWarehouseMenu();
        choice = getValidInt("");
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

        switch (choice) {
            case 1: { // Add a product
                string description;
                double price;
                int quantity;
                double transportLongitude, transportLatitude;

                cout << "Enter product description (max 50 chars): ";
                getline(cin, description);
                
                price = getValidDouble("Enter product price (RUB): ", 0, numeric_limits<double>::max());
                quantity = getValidInt("Enter product quantity: ", 1);
                transportLongitude = getValidDouble("Enter transport longitude (19-169): ", 19, 169);
                transportLatitude = getValidDouble("Enter transport latitude (41-82): ", 41, 82);

                try {
                    Product newProduct(description, price, quantity, transportLongitude, transportLatitude);
                    Warehouse* bestWarehouse = findBestWarehouse(warehouses, newProduct);
                    
                    if (bestWarehouse && bestWarehouse->addProduct(newProduct)) {
                        cout << "Product added successfully to warehouse " << bestWarehouse->getId() << ".\n";
                    } else {
                        cout << "Failed to add product. Warehouse may be at capacity.\n";
                    }
                } catch (const invalid_argument& e) {
                    cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            
            case 2: { // List products in a warehouse
                cout << "Available warehouses:\n";
                for (size_t i = 0; i < warehouses.size(); ++i) {
                    cout << i + 1 << ". " << warehouses[i].getId() << " (" 
                         << warehouseTypeToString(warehouses[i].getType()) << ")\n";
                }
                
                int warehouseChoice = getValidInt("Select warehouse (1-" + to_string(warehouses.size()) + "): ", 1);
                warehouses[warehouseChoice - 1].printProducts();
                break;
            }
            
            case 3: { // Search products by description
                string searchTerm;
                cout << "Enter search term: ";
                getline(cin, searchTerm);
                
                if (searchTerm.empty()) {
                    cout << "Search term cannot be empty.\n";
                    break;
                }
                
                bool found = false;
                for (auto& warehouse : warehouses) {
                    vector<Product> results = warehouse.findProductsByDescription(searchTerm);
                    if (!results.empty()) {
                        found = true;
                        cout << "Found " << results.size() << " product(s) in warehouse " 
                             << warehouse.getId() << ":\n";
                        for (const auto& product : results) {
                            product.print();
                            cout << "--------------------\n";
                        }
                    }
                }
                
                if (!found) {
                    cout << "No products found matching '" << searchTerm << "'.\n";
                }
                break;
            }
            
            case 4: { // Remove a product
                string barcode;
                cout << "Enter product barcode to remove: ";
                getline(cin, barcode);
                
                if (barcode.length() != 13) {
                    cout << "Invalid barcode length. Must be 13 characters.\n";
                    break;
                }
                
                int quantity = getValidInt("Enter quantity to remove: ", 1);
                bool removed = false;
                
                for (auto& warehouse : warehouses) {
                    if (warehouse.removeProduct(barcode, quantity)) {
                        cout << "Successfully removed " << quantity << " of product " << barcode 
                             << " from warehouse " << warehouse.getId() << ".\n";
                        removed = true;
                        break;
                    }
                }
                
                if (!removed) {
                    cout << "Product not found or insufficient quantity in any warehouse.\n";
                }
                break;
            }
            
            case 5: { // Display warehouse information
                for (auto& warehouse : warehouses) {
                    warehouse.print();
                    cout << "--------------------\n";
                }
                break;
            }
            
            case 6: // Switch to Polynomial System
                return;
                
            case 7: // Exit
                cout << "Exiting program.\n";
                exit(0);
                
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

void runPolynomialSystem() {
    VectPolynomial v1, v2;
    int choice;
    
    do {
        displayPolynomialMenu();
        choice = getValidInt("");
        
        switch (choice) {
            case 1: { // Create a new polynomial
                int degree = getValidInt("Enter the degree of the polynomial: ");
                if (degree < 0) {
                    cout << "Degree must be non-negative.\n";
                    break;
                }
                
                Polynomial poly(degree);
                for (int i = 0; i <= degree; ++i) {
                    double coeff = getValidDouble("Enter coefficient for x^" + to_string(i) + ": ");
                    poly.setCoefficient(i, coeff);
                }
                
                int vectorChoice = getValidInt("Add to vector V1 (1) or V2 (2)? ");
                if (vectorChoice == 1) {
                    v1.addPolynomial(poly);
                } else if (vectorChoice == 2) {
                    v2.addPolynomial(poly);
                } else {
                    cout << "Invalid choice. Polynomial not added.\n";
                }
                break;
            }
            
            case 2: { // Display all polynomials
                cout << "\nVector V1 polynomials:\n";
                v1.printAll();
                cout << "\nVector V2 polynomials:\n";
                v2.printAll();
                break;
            }
            
            case 3: { // Evaluate a polynomial
                int vectorChoice = getValidInt("Evaluate polynomial from V1 (1) or V2 (2)? ");
                const VectPolynomial& vec = (vectorChoice == 1) ? v1 : v2;
                
                if (vec.size() == 0) {
                    cout << "No polynomials in the selected vector.\n";
                    break;
                }
                
                cout << "Available polynomials (1-" << vec.size() << "):\n";
                for (size_t i = 0; i < vec.size(); ++i) {
                    cout << i + 1 << ": ";
                    vec.getPolynomial(i).print();
                }
                
                int polyIndex = getValidInt("Select polynomial: ") - 1;
                if (polyIndex < 0 || polyIndex >= vec.size()) {
                    cout << "Invalid selection.\n";
                    break;
                }
                
                double x = getValidDouble("Enter value of x to evaluate: ");
                double result = vec.getPolynomial(polyIndex).evaluate(x);
                cout << "P(" << x << ") = " << result << "\n";
                break;
            }
            
            case 4: { // Process two vectors of polynomials
                if (v1.size() == 0 || v2.size() == 0) {
                    cout << "Both vectors must contain at least one polynomial.\n";
                    break;
                }
                
                VectPolynomial vout = processPolynomials(v1, v2);
                cout << "Resulting polynomials after processing:\n";
                vout.printAll();
                
                // Evaluate all resulting polynomials at x=1
                cout << "\nEvaluating all resulting polynomials at x=1:\n";
                for (size_t i = 0; i < vout.size(); ++i) {
                    double val = vout.getPolynomial(i).evaluate(1.0);
                    cout << "Polynomial " << i + 1 << " at x=1: " << val << "\n";
                }
                break;
            }
            
            case 5: // Switch to Warehouse System
                return;
                
            case 6: // Exit
                cout << "Exiting program.\n";
                exit(0);
                
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

int main() {
    while (true) {
        cout << "\nMain Menu\n"
             << "1. Warehouse Management System\n"
             << "2. Polynomial Operations System\n"
             << "3. Exit\n"
             << "Enter your choice: ";
             
        int choice = getValidInt("");
        
        switch (choice) {
            case 1:
                runWarehouseSystem();
                break;
            case 2:
                runPolynomialSystem();
                break;
            case 3:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
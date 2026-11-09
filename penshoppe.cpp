#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

// ============================================================
// PENSHOPPE BRAND-AFFILIATED E-COMMERCE INVENTORY SYSTEM
// C++17 | DSA Final Laboratory Project
// ============================================================

struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};

// ---------- Input helpers ----------
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt, int minValue = 0) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue) {
            clearInput();
            return value;
        }
        cout << "Invalid input. Please enter a number >= " << minValue << ".\n";
        clearInput();
    }
}

double readDouble(const string& prompt, double minValue = 0.0) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue) {
            clearInput();
            return value;
        }
        cout << "Invalid input. Please enter a value >= " << minValue << ".\n";
        clearInput();
    }
}

string readText(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
        if (value.empty()) cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}

string lowerText(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return value;
}

bool containsIgnoreCase(const string& text, const string& keyword) {
    return lowerText(text).find(lowerText(keyword)) != string::npos;
}

// ---------- Binary Search Tree: product ID index ----------
struct TreeNode {
    int id;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(int value) : id(value), left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    TreeNode* root = nullptr;

    TreeNode* insertNode(TreeNode* node, int id) {
        if (!node) return new TreeNode(id);
        if (id < node->id) node->left = insertNode(node->left, id);
        else if (id > node->id) node->right = insertNode(node->right, id);
        return node;
    }

    bool searchNode(TreeNode* node, int id) const {
        if (!node) return false;
        if (id == node->id) return true;
        return id < node->id ? searchNode(node->left, id)
                             : searchNode(node->right, id);
    }

    void deleteTree(TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    ~ProductBST() { clear(); }

    void clear() {
        deleteTree(root);
        root = nullptr;
    }

    void rebuild(const vector<Product>& products) {
        clear();
        for (const auto& p : products) root = insertNode(root, p.id);
    }

    bool contains(int id) const { return searchNode(root, id); }
};

// ---------- Doubly Linked List: shopping cart ----------
struct CartNode {
    Product product;
    int quantity;
    CartNode* prev;
    CartNode* next;

    CartNode(const Product& p, int q)
        : product(p), quantity(q), prev(nullptr), next(nullptr) {}
};

class ShoppingCart {
private:
    CartNode* head = nullptr;
    CartNode* tail = nullptr;

public:
    ~ShoppingCart() { clear(); }

    void clear() {
        while (head) {
            CartNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    bool empty() const { return head == nullptr; }

    void add(const Product& product, int quantity) {
        for (CartNode* cur = head; cur; cur = cur->next) {
            if (cur->product.id == product.id) {
                cur->quantity += quantity;
                return;
            }
        }
        CartNode* node = new CartNode(product, quantity);
        node->prev = tail;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
    }

    void display() const {
        if (empty()) {
            cout << "\nYour shopping cart is empty.\n";
            return;
        }
        cout << "\n================ SHOPPING CART ================\n";
        cout << left << setw(7) << "ID" << setw(25) << "Product"
             << setw(10) << "Qty" << setw(12) << "Price" << "Subtotal\n";
        cout << string(68, '-') << '\n';
        double total = 0;
        for (CartNode* cur = head; cur; cur = cur->next) {
            double subtotal = cur->product.price * cur->quantity;
            total += subtotal;
            cout << left << setw(7) << cur->product.id
                 << setw(25) << cur->product.name.substr(0, 24)
                 << setw(10) << cur->quantity
                 << "P" << fixed << setprecision(2) << setw(11) << cur->product.price
                 << "P" << subtotal << '\n';
        }
        cout << string(68, '-') << "\nTOTAL: P" << fixed << setprecision(2) << total << '\n';
    }

    template <typename Function>
    void forEach(Function action) const {
        for (CartNode* cur = head; cur; cur = cur->next)
            action(cur->product, cur->quantity);
    }
};

// ---------- Circular Linked List: partner brand showcase ----------
struct BrandNode {
    string brand;
    BrandNode* next;
    explicit BrandNode(const string& value) : brand(value), next(nullptr) {}
};

class BrandShowcase {
private:
    BrandNode* tail = nullptr;

public:
    ~BrandShowcase() { clear(); }

    void add(const string& brand) {
        BrandNode* node = new BrandNode(brand);
        if (!tail) {
            tail = node;
            node->next = node;
        } else {
            node->next = tail->next;
            tail->next = node;
            tail = node;
        }
    }

    void display() const {
        if (!tail) {
            cout << "No partner brands available.\n";
            return;
        }
        cout << "\n========== PARTNER BRAND SHOWCASE ==========\n";
        BrandNode* current = tail->next;
        do {
            cout << "  * " << current->brand << '\n';
            current = current->next;
        } while (current != tail->next);
    }

    void clear() {
        if (!tail) return;
        BrandNode* head = tail->next;
        tail->next = nullptr;
        while (head) {
            BrandNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }
};

// ---------- Stack: undo admin changes ----------
struct UndoNode {
    vector<Product> previousState;
    UndoNode* next;

    explicit UndoNode(const vector<Product>& state)
        : previousState(state), next(nullptr) {}
};

class UndoStack {
private:
    UndoNode* topNode = nullptr;

public:
    ~UndoStack() { clear(); }

    void push(const vector<Product>& state) {
        UndoNode* node = new UndoNode(state);
        node->next = topNode;
        topNode = node;
    }

    bool pop(vector<Product>& state) {
        if (!topNode) return false;
        UndoNode* temp = topNode;
        state = temp->previousState;
        topNode = topNode->next;
        delete temp;
        return true;
    }

    void clear() {
        while (topNode) {
            UndoNode* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }
};

// ---------- Queue: customer checkout ----------
struct Order {
    string customerName;
    vector<pair<int, int>> items; // product ID, quantity
    double total;
};

void printHeader() {
    cout << "\n===============================================================\n";
    cout << "       PENSHOPPE - INVENTORY & E-COMMERCE SYSTEM\n";
    cout << "===============================================================\n";
}

void displayProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "\nNo products in the inventory.\n";
        return;
    }
    cout << "\n======================= PRODUCT CATALOG =======================\n";
    cout << left << setw(7) << "ID" << setw(25) << "Product"
         << setw(16) << "Category" << setw(14) << "Brand"
         << setw(12) << "Price" << "Stock\n";
    cout << string(83, '-') << '\n';
    for (const auto& p : products) {
        cout << left << setw(7) << p.id
             << setw(25) << p.name.substr(0, 24)
             << setw(16) << p.category.substr(0, 15)
             << setw(14) << p.brand.substr(0, 13)
             << "P" << fixed << setprecision(2) << setw(11) << p.price
             << p.stockQuantity << '\n';
    }
    cout << string(83, '-') << '\n';
}

int findProductIndex(const vector<Product>& products, int id) {
    for (size_t i = 0; i < products.size(); ++i)
        if (products[i].id == id) return static_cast<int>(i);
    return -1;
}

// ---------- Lab search: linear keyword search ----------
void searchByKeyword(const vector<Product>& products) {
    string keyword = readText("Enter product keyword: ");
    bool found = false;
    cout << "\nSearch results:\n";
    for (const auto& p : products) {
        if (containsIgnoreCase(p.name, keyword) ||
            containsIgnoreCase(p.category, keyword) ||
            containsIgnoreCase(p.brand, keyword)) {
            cout << "ID: " << p.id << " | " << p.name << " | "
                 << p.brand << " | P" << fixed << setprecision(2)
                 << p.price << " | Stock: " << p.stockQuantity << '\n';
            found = true;
        }
    }
    if (!found) cout << "No matching product found.\n";
}

// ---------- Lab search: binary search over price-sorted copy ----------
void filterByPrice(const vector<Product>& products) {
    double low = readDouble("Minimum price (P): ");
    double high = readDouble("Maximum price (P): ");
    if (low > high) {
        cout << "Minimum price cannot be greater than maximum price.\n";
        return;
    }

    vector<Product> sorted = products;
    sort(sorted.begin(), sorted.end(),
         [](const Product& a, const Product& b) { return a.price < b.price; });

    // Binary search locates the first product whose price is >= low.
    auto first = lower_bound(sorted.begin(), sorted.end(), low,
        [](const Product& p, double value) { return p.price < value; });

    bool found = false;
    cout << "\nProducts priced from P" << low << " to P" << high << ":\n";
    for (auto it = first; it != sorted.end() && it->price <= high; ++it) {
        cout << it->id << " | " << it->name << " | " << it->brand
             << " | P" << fixed << setprecision(2) << it->price
             << " | Stock: " << it->stockQuantity << '\n';
        found = true;
    }
    if (!found) cout << "No products found in this price range.\n";
}

// ---------- Brand filtering ----------
void filterByBrand(const vector<Product>& products) {
    string brand = readText("Enter brand name: ");
    bool found = false;
    for (const auto& p : products) {
        if (containsIgnoreCase(p.brand, brand)) {
            cout << p.id << " | " << p.name << " | " << p.category
                 << " | P" << fixed << setprecision(2) << p.price
                 << " | Stock: " << p.stockQuantity << '\n';
            found = true;
        }
    }
    if (!found) cout << "No products found for that brand.\n";
}

// ---------- Sorting algorithms ----------
void insertionSort(vector<Product>& products, int choice) {
    for (size_t i = 1; i < products.size(); ++i) {
        Product key = products[i];
        int j = static_cast<int>(i) - 1;

        auto shouldMove = [&](const Product& current) {
            if (choice == 1) return current.price > key.price;
            if (choice == 2) return current.stockQuantity > key.stockQuantity;
            return lowerText(current.brand) > lowerText(key.brand);
        };

        while (j >= 0 && shouldMove(products[j])) {
            products[j + 1] = products[j];
            --j;
        }
        products[j + 1] = key;
    }
}

void quickSortByPrice(vector<Product>& products, int low, int high) {
    if (low >= high) return;
    double pivot = products[high].price;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (products[j].price <= pivot) {
            ++i;
            swap(products[i], products[j]);
        }
    }
    swap(products[i + 1], products[high]);
    int pivotIndex = i + 1;
    quickSortByPrice(products, low, pivotIndex - 1);
    quickSortByPrice(products, pivotIndex + 1, high);
}

void sortInventory(vector<Product>& products) {
    if (products.empty()) {
        cout << "Inventory is empty.\n";
        return;
    }
    cout << "\nSORT INVENTORY\n";
    cout << "1. Insertion Sort by Price (ascending)\n";
    cout << "2. Insertion Sort by Stock (ascending)\n";
    cout << "3. Insertion Sort by Brand (A-Z)\n";
    cout << "4. Quick Sort by Price (ascending)\n";
    int choice = readInt("Choose sorting method: ", 1);
    if (choice < 1 || choice > 4) {
        cout << "Invalid sorting option.\n";
        return;
    }

    if (choice == 4)
        quickSortByPrice(products, 0, static_cast<int>(products.size()) - 1);
    else
        insertionSort(products, choice);

    cout << "Inventory sorted successfully.\n";
    displayProducts(products);
}

// ---------- Admin product management ----------
void addProduct(vector<Product>& products, UndoStack& undo, ProductBST& bst) {
    Product p;
    p.id = readInt("Enter unique product ID: ", 1);
    if (findProductIndex(products, p.id) != -1) {
        cout << "That product ID already exists.\n";
        return;
    }
    p.name = readText("Product name: ");
    p.category = readText("Category: ");
    p.brand = readText("Brand: ");
    p.price = readDouble("Price (P): ");
    p.stockQuantity = readInt("Stock quantity: ");

    undo.push(products);
    products.push_back(p);
    bst.rebuild(products);
    cout << "Product added successfully.\n";
}

void updateProduct(vector<Product>& products, UndoStack& undo, ProductBST& bst) {
    int id = readInt("Enter product ID to update: ", 1);
    int index = findProductIndex(products, id);
    if (index == -1) {
        cout << "Product not found.\n";
        return;
    }

    cout << "Updating: " << products[index].name << '\n';
    undo.push(products);
    products[index].name = readText("New product name: ");
    products[index].category = readText("New category: ");
    products[index].brand = readText("New brand: ");
    products[index].price = readDouble("New price (P): ");
    products[index].stockQuantity = readInt("New stock quantity: ");
    bst.rebuild(products);
    cout << "Product updated successfully.\n";
}

void deleteProduct(vector<Product>& products, UndoStack& undo, ProductBST& bst) {
    int id = readInt("Enter product ID to delete: ", 1);
    int index = findProductIndex(products, id);
    if (index == -1) {
        cout << "Product not found.\n";
        return;
    }
    undo.push(products);
    cout << products[index].name << " has been removed.\n";
    products.erase(products.begin() + index);
    bst.rebuild(products);
}

void adminMenu(vector<Product>& products, UndoStack& undo, ProductBST& bst) {
    while (true) {
        cout << "\n============= ADMIN INVENTORY MENU =============\n";
        cout << "1. Add Product\n2. Update Product\n3. Delete Product\n";
        cout << "4. Undo Last Admin Action\n5. Back to Main Menu\n";
        int choice = readInt("Choose an option: ", 1);

        if (choice == 1) addProduct(products, undo, bst);
        else if (choice == 2) updateProduct(products, undo, bst);
        else if (choice == 3) deleteProduct(products, undo, bst);
        else if (choice == 4) {
            if (undo.pop(products)) {
                bst.rebuild(products);
                cout << "Previous inventory state restored.\n";
            } else cout << "There is no admin action to undo.\n";
        }
        else if (choice == 5) break;
        else cout << "Invalid option.\n";
    }
}

// ---------- Cart and checkout ----------
void addToCart(const vector<Product>& products, ShoppingCart& cart) {
    int id = readInt("Enter product ID: ", 1);
    int index = findProductIndex(products, id);
    if (index == -1) {
        cout << "Product not found.\n";
        return;
    }
    int quantity = readInt("Quantity: ", 1);
    if (quantity > products[index].stockQuantity) {
        cout << "Not enough stock. Available: " << products[index].stockQuantity << '\n';
        return;
    }
    cart.add(products[index], quantity);
    cout << "Product added to cart.\n";
}

void checkout(ShoppingCart& cart, vector<Product>& products, queue<Order>& checkoutQueue) {
    if (cart.empty()) {
        cout << "Your cart is empty. Add products before checkout.\n";
        return;
    }

    cart.display();
    string customer = readText("Customer name: ");
    Order order;
    order.customerName = customer;
    order.total = 0.0;

    bool stockValid = true;
    cart.forEach([&](const Product& item, int quantity) {
        int index = findProductIndex(products, item.id);
        if (index == -1 || products[index].stockQuantity < quantity)
            stockValid = false;
    });

    if (!stockValid) {
        cout << "Checkout cannot continue because inventory stock changed.\n";
        return;
    }

    cart.forEach([&](const Product& item, int quantity) {
        int index = findProductIndex(products, item.id);
        products[index].stockQuantity -= quantity;
        order.items.push_back({item.id, quantity});
        order.total += item.price * quantity;
    });

    checkoutQueue.push(order);
    cart.clear();
    cout << "Order placed and added to the checkout queue.\n";
    cout << "Order total: P" << fixed << setprecision(2) << order.total << '\n';
    cout << "Stock quantities have been updated.\n";
}

void processCheckoutQueue(queue<Order>& checkoutQueue) {
    if (checkoutQueue.empty()) {
        cout << "There are no customer orders waiting in the queue.\n";
        return;
    }
    cout << "\n========== PROCESSING CHECKOUT QUEUE (FIFO) ==========\n";
    while (!checkoutQueue.empty()) {
        Order order = checkoutQueue.front();
        checkoutQueue.pop();
        cout << "Processed order for: " << order.customerName
             << " | Total: P" << fixed << setprecision(2) << order.total << '\n';
    }
    cout << "All queued orders have been processed.\n";
}

// ---------- Product ID search using BST ----------
void searchByID(const ProductBST& bst, const vector<Product>& products) {
    int id = readInt("Enter product ID: ", 1);
    if (!bst.contains(id)) {
        cout << "Product ID not found in the BST.\n";
        return;
    }
    int index = findProductIndex(products, id);
    if (index >= 0) {
        const Product& p = products[index];
        cout << "Found: " << p.name << " | " << p.brand << " | "
             << p.category << " | P" << fixed << setprecision(2)
             << p.price << " | Stock: " << p.stockQuantity << '\n';
    }
}

// ---------- Main program ----------
int main() {
    vector<Product> inventory = {
        {1001, "Classic Logo T-Shirt", "Shirts", "Penshoppe", 499.00, 25},
        {1002, "Basic Cotton Tee", "Shirts", "Penshoppe", 399.00, 30},
        {1003, "Denim Straight Pants", "Pants", "Penshoppe", 999.00, 12},
        {1004, "Printed Hoodie", "Jackets", "Penshoppe", 1299.00, 10},
        {2001, "Oxygen Graphic Tee", "Shirts", "Oxygen", 599.00, 18},
        {2002, "Oxygen Cargo Pants", "Pants", "Oxygen", 1199.00, 8},
        {3001, "Memo Casual Blouse", "Tops", "Memo", 799.00, 14},
        {3002, "Memo Wide-Leg Pants", "Pants", "Memo", 1099.00, 9},
        {4001, "Regatta Polo Shirt", "Shirts", "Regatta", 899.00, 16},
        {4002, "Regatta Casual Shorts", "Shorts", "Regatta", 699.00, 20}
    };

    ProductBST productTree;
    productTree.rebuild(inventory);

    ShoppingCart cart;
    BrandShowcase brands;
    brands.add("Penshoppe");
    brands.add("Oxygen");
    brands.add("Memo");
    brands.add("Regatta");

    UndoStack undoStack;
    queue<Order> checkoutQueue;

    bool running = true;
    while (running) {
        printHeader();
        cout << "1. View Complete Product Catalog\n";
        cout << "2. Filter Products by Official Brand\n";
        cout << "3. Search Products (Linear Search / BST by ID)\n";
        cout << "4. Filter Products by Price Range (Binary Search)\n";
        cout << "5. Add Item to Shopping Cart (Doubly Linked List)\n";
        cout << "6. Process Customer Checkout Queue (FIFO)\n";
        cout << "7. Admin Actions / Undo (Stack)\n";
        cout << "8. Display Partner Brand Banner Showcase (Circular List)\n";
        cout << "9. Sort Inventory\n";
        cout << "10. Exit\n";

        int choice = readInt("Enter your choice: ", 1);
        switch (choice) {
            case 1:
                displayProducts(inventory);
                break;
            case 2:
                filterByBrand(inventory);
                break;
            case 3: {
                cout << "\\nSEARCH PRODUCTS\\n";
                cout << "1. Search by Keyword (Linear Search)\\n";
                cout << "2. Search by Product ID (Binary Search Tree)\\n";
                int searchChoice = readInt("Choose search method: ", 1);
                if (searchChoice == 1) searchByKeyword(inventory);
                else if (searchChoice == 2) searchByID(productTree, inventory);
                else cout << "Invalid search option.\\n";
                break;
            }
            case 4:
                filterByPrice(inventory);
                break;
            case 5:
                displayProducts(inventory);
                addToCart(inventory, cart);
                cart.display();
                break;
            case 6:
                checkout(cart, inventory, checkoutQueue);
                processCheckoutQueue(checkoutQueue);
                break;
            case 7:
                adminMenu(inventory, undoStack, productTree);
                break;
            case 8:
                brands.display();
                break;
            case 9:
                sortInventory(inventory);
                break;
            case 10:
                running = false;
                cout << "\nThank you for using the Penshoppe Inventory System!\n";
                break;
            default:
                cout << "Invalid menu choice. Please try again.\n";
        }
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }

    return 0;
}

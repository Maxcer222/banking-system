#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <limits>
#include <sstream>
#include <cctype>
#include <ctime>
#include <filesystem>


using namespace std;

class User {
protected:
    string username;
    string password;
    string avatar;
    

    

public:
    User(string name, string pass) : username(name), password(pass) {  
        generateAvatar();
    }
    string getAvatar() {
        return avatar;
    }


    string getPassword() {
        return password; 
    }

    void generateAvatar() {
        vector<string> avatars = { "blyat", "syka", "naxyi" };
        avatar = avatars[rand() % avatars.size()];
    }

    bool checkPassword(string attempt) {
        return attempt == password;
    }

    virtual void displayProfile() {
        cout << "\nUsername: " << username << endl;
        cout << "Avatar: " << avatar << endl;
    }

    string getUsername() {
        return username; 
    }
    virtual ~User() {}
};

class Buyer : public User {
public:
    Buyer(string name, string pass)
        : User(name, pass) {}
};

class Seller : public User {
public:
    Seller(string name, string pass )
        : User(name, pass) {}
};

class UserManager {
private:
    map<string, User*> users;  

    bool isValidUsername(string username) {
        if (username.length() < 4)
            return false;
        if (!isalpha(username[0]))
            return false;  
        for (char c : username) {
            if (!isalnum(c) && c != '_')
                return false; 
        }
        return true;
    }

    bool isPasswordValid(string password) {
        if (password.length() < 8)
            return false;
        bool hasUpper = false, hasLower = false, hasDigit = false;  
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }
        return hasUpper && hasLower && hasDigit;
    }

    void saveUsers() {
        ofstream file("log.txt");  
        if (!file.is_open()) {
            cout << "Error saving users to file!" << endl;
            return;
        }

        for (auto& pair : users) {  
            User* user = pair.second;
            file << user->getUsername() << " "
                << user->getPassword() << " "  
                << (dynamic_cast<Buyer*>(user) ? "buyer" : "seller") << "\n";  
        }
    }


    void loadUsers() {
        ifstream file("Log.txt");
        if (!file.is_open()) {
            cout << "Failed to open user file\n";
            return;
        }

        string username, password, type;
        while (file >> username >> password >> type) {
            if (username.empty() || password.empty() || type.empty()) {
                continue; 
            }

            try {
                if (type == "buyer") {
                    users[username] = new Buyer(username, password);
                }
                else {
                    users[username] = new Seller(username, password);
                }
            }
            catch (...) {
                cerr << "Failed to create user: " << username << endl;
            }
        }
    }

public:
    UserManager() {
        loadUsers();
    }

    User* login() {
        string username, password;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (users.count(username) && users[username]->checkPassword(password)) {
            cout << "Login successful!\n";
            return users[username];
        }
        cout << "Login failed!\n";
        return 0;
    }

    Buyer* registerBuyer() {
        string username, password;
        cout << "\n--- Buyer Registration ---\n";

        do {
            cout << "Username: ";
            cin >> username;
            if (users.count(username)) {
                cout << "Username already exists!\n";
            }
        } while (!isValidUsername(username) || users.count(username));

        do {
            cout << "Password: ";
            cin >> password;
            if (!isPasswordValid(password)) {
                cout << "Password must contain:\n";
                cout << "- At least 8 characters\n";
                cout << "- Uppercase and lowercase letters\n";
                cout << "- At least one digit\n";
            }
        } while (!isPasswordValid(password));

        Buyer* newBuyer = new Buyer(username, password);
        users[username] = newBuyer;
        saveUsers();

        cout << "Registration complete!\n";
        return newBuyer;
    }

    Seller* registerSeller() {
        string username, password;
        cout << "\n--- Seller Registration ---\n";

        do {
            cout << "Username: ";
            cin >> username;
            if (users.count(username)) {
                cout << "Username already exists!\n";
            }
        } while (!isValidUsername(username) || users.count(username));

        do {
            cout << "Password: ";
            cin >> password;
            if (!isPasswordValid(password)) {
                cout << "Password must contain:\n";
                cout << "- At least 8 characters\n";
                cout << "- Uppercase and lowercase letters\n";
                cout << "- At least one digit\n";
            }
        } while (!isPasswordValid(password));

        Seller* newSeller = new Seller(username, password);
        users[username] = newSeller;
        saveUsers();

        cout << "Registration complete!\n";
        return newSeller;
    }

    ~UserManager() {
        saveUsers();
        for (auto& pair : users) {
            delete pair.second;
        }
    }
};


class Bid {
private:
    string bidderName;
    int itemId;
    double bidAmount;

public:
    Bid(string bidder, int id, double amount) {
        bidderName = bidder;
        itemId = id;
        bidAmount = amount;
    }

    string getBidderName() {
        return bidderName;
    }
    int getItemId() { 
        return itemId;
    }
    double getBidAmount() {
        return bidAmount; 
    }

    void displayBid() {
        cout << "Bidder: " << bidderName << ", Item ID: " << itemId
            << ", Bid: $" <<  bidAmount << endl;
    }
};

class Item {
private:
    int Id;
    string Name;
    string Category;
    double Price;

public:
    Item(int id, string name, string category, double price) {
        Id = id;
        Name = name;
        Category = category;
        Price = price;
    }

    int getId() {
        return Id;
    }
    string getName() {
        return Name;
    }
    string getCategory() {
        return Category;
    }
    double getPrice() {
        return Price;
    }

    void setName(string a) {
        Name = a;
    }
    void setCategory(string b) {
        Category = b;
    }
    void setPrice(double c) {
        Price = c;
    }

    void display() {
        cout << left << setw(5) << Id
            << setw(20) << Name
            << setw(15)<< Category
            << "$" << Price << endl;
    }

    bool operator==(Item other) {
        return Id == other.Id;
    }
};

class Auction {
private:
    vector<Item> items;
    map<int, vector<Bid>> itemBids;
    map<int, double> highestBids;
    map<int, string> currentWinners;
    vector<string> auctionLog;
    bool auctionActive = false;
    int maxBidsTotal = 1;
    int currentBidCount = 0;

public:
    void addItem(Item item) {
        items.push_back(item);
        highestBids[item.getId()] = item.getPrice();
        currentWinners[item.getId()] = "";
        cout << "Item added: ID " << item.getId() << " - " << item.getName() << endl;
    }

    void removeItem(int id) {
        auto it = remove_if(items.begin(), items.end(), [id](Item i) {
            return i.getId() == id;
            }
        );

        if (it != items.end()) {
            items.erase(it, items.end());
            itemBids.erase(id);
            highestBids.erase(id);
            currentWinners.erase(id);
            cout << "Item removed.\n";
        }
        else {
            cout << "Item not found.\n";
        }
    }

    void updateItem(int id, string newName, string newCategory, double newPrice) {
        for (auto& item : items) {
            if (item.getId() == id) {
                item.setName(newName);
                item.setCategory(newCategory);
                item.setPrice(newPrice);
                highestBids[id] = newPrice;
                currentWinners[id] = "";
                cout << "Item updated.\n";
                return;
            }
        }
        cout << "Item not found.\n";
    }

    void listItems() {
        cout << "\n--- Auction items ---\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Category"
            << setw(10) << "Price" << setw(15) << "Current Bid" << setw(20) << "Current Leader" << endl;
        cout << string(80, '-') << endl;

        for (auto& item : items) {
            cout << left << setw(5) << item.getId()
                << setw(20) << item.getName()
                << setw(15) << item.getCategory() << "$" << item.getPrice() <<"       "
                << setw(15) << highestBids[item.getId()] << setw(20) << currentWinners[item.getId()] << endl;
        }
    }

    void filterByPrice(double maxPrice) {
        cout << "\nItems under $" << maxPrice << ":\n";
        for (auto& item : items) {
            if (item.getPrice() <= maxPrice) {
                item.display();
            }
        }
    }

    void recommendCheapestByCategory(string category) {
        auto it = min_element(items.begin(), items.end(), [&](Item a, Item b) {
            if (a.getCategory() != category) return false;
            if (b.getCategory() != category) return true;
            return a.getPrice() < b.getPrice();
            });

        if (it != items.end() && it->getCategory() == category) {
            cout << "Recommended item: ";
            it->display();
        }
        else {
            cout << "No items in category: " << category << endl;
        }
    }

    void startAuction() {
        if (auctionActive) {
            cout << "Auction already active!\n";
            return;
        }
        auctionActive = true;
        currentBidCount = 0;
        auctionLog.push_back("Auction started");
        cout << "Auction started, trading will be closed after   " << maxBidsTotal << " total bids.\n";
    }

    bool isAuctionActive() {
        return auctionActive;
    }

    bool checkAuctionEnd() {
        if (!auctionActive) {
            return false;
        }

        if (currentBidCount >= maxBidsTotal) {
            auctionActive = false;
            determineWinners();
            auctionLog.push_back("Auction ended");
            cout << "Auction ended (reached max bids).\n";
            return true;
        }
        return false;
    }

    void placeBid(string bidderName, int itemId, double bidAmount) {
        if (!isAuctionActive()) {
            cout << "Auction closed\n";
            return;
        }

        auto it = find_if(items.begin(), items.end(), [itemId](Item item) {
            return item.getId() == itemId;
            });

        if (it == items.end()) {
            cout << "Item ID " << itemId << " not found.\n";
            return;
        }

        if (bidAmount <= highestBids[itemId]) {
            cout << "Bid must be higher than current $" <<  highestBids[itemId] << endl;
            return;
        }

        Bid newBid(bidderName, itemId, bidAmount);
        itemBids[itemId].push_back(newBid);
        highestBids[itemId] = bidAmount;
        currentWinners[itemId] = bidderName;
        currentBidCount++;

        string logEntry = bidderName + " bid $" + to_string(bidAmount) + " on item " + to_string(itemId);
        auctionLog.push_back(logEntry);
        cout << "Bid accepted.\n";
    }

    void determineWinners() {
        cout << "\n--- Auction Results --- \n";

        for (auto& pair : currentWinners) {
            int itemId = pair.first;
            string winnerName = pair.second;

            auto it = find_if(items.begin(), items.end(), [itemId](Item item) {
                return item.getId() == itemId;
                });

            if (it != items.end()) {
                if (!winnerName.empty()) {
                    cout << "Item: " << it->getName() << ", Winner: " << winnerName
                        << ", Price: $" <<  highestBids[itemId] << endl;
                }
                else {
                    cout << "Item: " << it->getName() << " - no bids\n";
                }
            }
        }
    }

    void displayAuctionLog() {
        cout << "\n--- Auction Log ---\n";
        for (auto& entry : auctionLog) {
            cout << entry << endl;
        }
    }

    void saveToFile(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error saving items to file!" << endl;
            return;
        }

        for (Item& item : items) {
            file << item.getId() << ";"
                << item.getName() << ";"
                << item.getCategory() << ";"
                << item.getPrice() << "\n";
        }
        cout << "File saved: " << filename << endl;
    }

    void loadFromFile(string filename) {
        items.clear();
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error loading items from file!" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idStr, name, category, priceStr;

            getline(ss, idStr, ';');
            getline(ss, name, ';');
            getline(ss, category, ';');
            getline(ss, priceStr);

            try {
                int id = stoi(idStr);
                double price = stod(priceStr);
                items.emplace_back(id, name, category, price);
                highestBids[id] = price;
                currentWinners[id] = "";
            }
            catch (...) {
                cout << "Error parsing line: " << line << endl;
            }
        }

        cout << "Items loaded from " << filename << endl;
    }
};

int main() {
    Auction auction;
    UserManager userManager;
    User* currentUser = nullptr;
    string itemsFilename = "items.txt";
    auction.loadFromFile(itemsFilename);

    int choice;

    do {
        if (!currentUser) {
            cout << "\n---Main menu---" << endl;
            cout << "1. Login\n2. Register buyer\n3. Register seller\n0. Exit\n";
            cout << "Choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                currentUser = userManager.login();
                break;
            case 2:
                currentUser = userManager.registerBuyer();
                break;
            case 3:
                currentUser = userManager.registerSeller();
                break;
            case 0:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice!\n";
            }
            continue;
        }

        cout << "\n--- Auction Menu ---" << endl;
        cout << "    Username: " << currentUser->getUsername() << endl;
        cout << "    Avatar: " << currentUser->getAvatar() << endl;

        cout << "\n   | General actions |\n";
        cout << "    1. List Items\n";
        cout << "    2. View profile\n";
       

        if (dynamic_cast<Buyer*>(currentUser)) {
            cout << "\n                       | Buyer actions |\n";
            cout << "                        3. Place bid\n";
            cout << "                        4. Filter by price\n";
        }

        if (dynamic_cast<Seller*>(currentUser)) {
            cout << "\n| Seller actions |\n";
            cout << "5. Add item\n";
            cout << "6. Remove item\n";
            cout << "7. Update item\n";
            cout << "8. Start auction\n";
            cout << "9. Auction status\n";
        }

        cout << "\n                                              | Other options |\n";
        cout << "                                               10. Save data\n";
        cout << "                                               11. Load data\n";
        cout << "                                               12. Show winners\n";
        cout << "                                               13. Show log\n";
        cout << "                                               14. Logout\n";
        
       

        cout << "choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 5: {
            int id;
            string name, category;
            double price;
            cout << "Enter item ID: ";
            cin >> id;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Enter price: ";
            cin >> price;
            auction.addItem(Item(id, name, category, price));
            break;
        }
        case 6: {
            int id;
            cout << "Enter item ID to remove: ";
            cin >> id;
            auction.removeItem(id);
            break;
        }
        case 7: {
            int id;
            string name, category;
            double price;
            cout << "Enter item ID to update: ";
            cin >> id;
            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter new category: ";
            getline(cin, category);
            cout << "Enter new price: ";
            cin >> price;
            auction.updateItem(id, name, category, price);
            break;
        }
        case 1:
            auction.listItems();
            break;
        case 3: {
            int id;
            double bid;
            cout << "Enter item ID: ";
            cin >> id;
            cout << "Enter bid amount: ";
            cin >> bid;
            auction.placeBid(currentUser->getUsername(), id, bid);
            break;
        }
        case 10:
            auction.saveToFile(itemsFilename);
            break;
        case 11:
            auction.loadFromFile(itemsFilename);
            break;
        case 4: {
            double maxPrice;
            cout << "Enter maximum price: ";
            cin >> maxPrice;
            auction.filterByPrice(maxPrice);
            break;
        }
        case 2:
            currentUser->displayProfile();
            break;
        case 14:
            cout << "Logged out.\n";
            currentUser = nullptr;
            break;
        case 8:
            auction.startAuction();
            break;
        case 9:
            if (auction.isAuctionActive()) {
                cout << "Auction is active.\n";
            }
            else {
                cout << "Auction is not active.\n";
            }
            break;
        case 12:
            auction.determineWinners();
            break;
        case 13:
            auction.displayAuctionLog();
            break;
        case 0:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option.\n";
        }

        auction.checkAuctionEnd();

    } while (choice != 0);

   
    auction.saveToFile(itemsFilename);
    return 0;
}

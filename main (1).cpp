#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Constants
const int TABLE_SIZE = 1051;  // Size of the hash table
const std::string FILE_NAME = "nflix.csv";  // Input file name

// Structure to hold movie/show information
struct Entry {
    std::string type;
    std::string title;
    std::string director;
    std::string country;
    int year;
    int duration;
};

// Hash Table class
class HashTable {
private:
    // Node to store data and handle collisions
    struct Node {
        Entry data;
        Node* next;
    };

    // Array of linked lists to create buckets
    std::vector<Node*> table;

public:
    HashTable() {
        // Initialize the hash table with null pointers
        table.resize(TABLE_SIZE, nullptr);
    }

    // Hash function using a basic polynomial hash
    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char ch : key) {
            hash = (hash * 31 + ch) % TABLE_SIZE;
        }
        return hash;
    }

    // Insert an entry into the hash table
    void insert(const Entry& entry) {
        int index = hashFunction(entry.title);
        Node* newNode = new Node{entry, nullptr};

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            newNode->next = table[index];
            table[index] = newNode;
        }
    }

    // Lookup an entry based on title and count comparisons
    Entry* lookup(const std::string& title, int& comparisons) {
        int index = hashFunction(title);
        comparisons = 0;

        Node* current = table[index];
        while (current != nullptr) {
            comparisons++;
            if (current->data.title == title) {
                return &current->data;
            }
            current = current->next;
        }

        return nullptr;  // Entry not found
    }
};

// Function to read entries from a file and fill the hash table
void readEntriesFromFile(HashTable& table) {
    std::ifstream inputFile(FILE_NAME);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    Entry entry;
    while (inputFile >> entry.type) {
        inputFile.ignore(1);  // Skip the space
        std::getline(inputFile, entry.title, ',');
        std::getline(inputFile, entry.director, ',');
        std::getline(inputFile, entry.country, ',');
        inputFile >> entry.year;
        inputFile.ignore(1);  // Skip the space
        inputFile >> entry.duration;
        table.insert(entry);
    }

    inputFile.close();
}

int main() {
    HashTable table;
    readEntriesFromFile(table);

    std::cout << "Read " << TABLE_SIZE << " entries." << std::endl;

    while (true) {
        std::string title;
        std::cout << "Enter a movie (or blank to exit): ";
        std::getline(std::cin, title);

        if (title.empty()) {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        int comparisons;
        Entry* result = table.lookup(title, comparisons);

        if (result != nullptr) {
            std::cout << "Comparisons: " << comparisons << std::endl;
            std::cout << "Type: " << result->type << std::endl;
            std::cout << "Movie: " << result->title << std::endl;
            std::cout << "Director: " << result->director << std::endl;
            std::cout << "Country: " << result->country << std::endl;
            std::cout << "Year of Release: " << result->year << std::endl;
            std::cout << "Duration: " << result->duration << " min" << std::endl;
        } else {
            std::cout << "Comparisons: " << comparisons << std::endl;
            std::cout << "Not found in the hash table." << std::endl;
        }
    }

    return 0;
}

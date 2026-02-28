/*
 * CS 300 Project Two - ABCU Course Planner
 * Author: Nathan Vanderpool
 *
 * This program implements a simple course advising tool for ABCU Computer Science department advisors.
 * It loads course data (including course number, title, and variable number of prerequisites) from a CSV file
 * into a Binary Search Tree (BST) for automatic alphanumeric sorting and efficient lookups.
 * The menu allows users to load data, print a sorted list of all courses, or view details and prerequisites for a specific course.
 * Course numbers are handled case-insensitively for user convenience.
 *
 * Data Structure Choice: BST is used because it maintains sorted order for efficient printing of the course list (O(n) in-order traversal)
 * and provides logarithmic time lookups for individual courses and prerequisites, balancing the needs of the advisors.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

// Course structure to hold course information
struct Course {
    string courseNumber;          // Unique course identifier (e.g., "CSCI101"), stored in uppercase
    string title;                 // Full descriptive name of the course
    vector<string> prerequisites; // List of prerequisite course numbers (also uppercase)

    // Default constructor for creating empty courses or with provided values
    Course(string num = "", string t = "") : courseNumber(num), title(t) {}

    // Overloaded less-than operator for BST ordering based on alphanumeric course number comparison
    bool operator<(const Course& other) const {
        return courseNumber < other.courseNumber;
    }
};

// Node structure for the Binary Search Tree
struct Node {
    Course course;  // The course data stored in this node
    Node* left;     // Pointer to left child (smaller course numbers)
    Node* right;    // Pointer to right child (larger course numbers)

    // Constructor to initialize a node with a course and null children
    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class to manage courses in sorted order
class BST {
private:
    Node* root;  // Root node of the tree

    // Recursive helper function to insert a course into the BST
    // Maintains the BST property: left < current < right
    Node* insert(Node* node, const Course& course) {
        // Base case: create a new node if we've reached the insertion point
        if (node == nullptr) {
            return new Node(course);
        }
        // Recurse left if the new course number is smaller
        if (course < node->course) {
            node->left = insert(node->left, course);
        }
        // Recurse right if the new course number is larger
        else if (node->course < course) {
            node->right = insert(node->right, course);
        }
        // If equal, do nothing to avoid duplicates
        return node;
    }

    // Recursive helper function to search for a course by number
    Node* search(Node* node, const string& courseNumber) const {
        // Base case: not found
        if (node == nullptr) return nullptr;
        // Found: return the node
        if (courseNumber == node->course.courseNumber) return node;
        // Recurse left or right based on comparison
        return (courseNumber < node->course.courseNumber) ?
            search(node->left, courseNumber) :
            search(node->right, courseNumber);
    }

    // Recursive helper for in-order traversal to print courses in sorted order
    void inOrder(Node* node) const {
        if (node != nullptr) {
            // Visit left subtree
            inOrder(node->left);
            // Print current course
            cout << node->course.courseNumber << ", " << node->course.title << endl;
            // Visit right subtree
            inOrder(node->right);
        }
    }

    // Recursive helper to destroy the tree and free memory
    void destroy(Node* node) {
        if (node != nullptr) {
            // Recurse on children first (post-order)
            destroy(node->left);
            destroy(node->right);
            // Delete the current node
            delete node;
        }
    }

public:
    // Constructor: initializes an empty tree
    BST() : root(nullptr) {}

    // Destructor: cleans up all nodes to prevent memory leaks
    ~BST() { destroy(root); }

    // Public insert: starts the recursive insertion from root
    void insert(const Course& course) {
        root = insert(root, course);
    }

    // Public find: searches for a course and returns a pointer to it (or nullptr if not found)
    Course* find(const string& courseNumber) const {
        Node* node = search(root, courseNumber);
        return node ? &node->course : nullptr;
    }

    // Public printInOrder: prints all courses in alphanumeric order using in-order traversal
    void printInOrder() const {
        inOrder(root);
    }
};

// Utility function to convert a string to uppercase for case-insensitive handling
string toUpper(string str) {
    // Use std::transform with a lambda to convert each character
    transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return toupper(c); });
    return str;
}

// Function to load courses from a CSV file into the BST
// Expected CSV format: courseNumber,title,prereq1,prereq2,... (variable prereqs, empty fields allowed)
// Skips invalid lines and normalizes course numbers to uppercase
bool loadCourses(const string& filename, BST& tree) {
    ifstream file(filename);
    if (!file.is_open()) {
        // Error handling: file not found or unable to open
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line;
    int lineNum = 0;  // Track line number for better error reporting
    
    while (getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;  // Skip empty lines

        stringstream ss(line);
        string token;
        vector<string> parts;

        // Parse comma-separated values
        while (getline(ss, token, ',')) {
            parts.push_back(token);
        }

        // Validate: at least course number and title required
        if (parts.size() < 2) {
            cout << "Warning: Skipping invalid line " << lineNum << ": too few fields" << endl;
            continue;
        }

        // Normalize to uppercase
        string courseNum = toUpper(parts[0]);
        string title = parts[1];  // Titles are not normalized (may contain mixed case)

        Course course(courseNum, title);

        // Add any additional fields as prerequisites (skip empty ones)
        for (size_t i = 2; i < parts.size(); ++i) {
            string pre = parts[i];
            if (!pre.empty()) {
                course.prerequisites.push_back(toUpper(pre));
            }
        }

        // Insert into the tree
        tree.insert(course);
    }

    file.close();
    return true;
}

// Function to print detailed information for a specific course, including prerequisites
void printCourseInfo(const BST& tree, string courseNum) {
    // Normalize input to uppercase for case-insensitive lookup
    courseNum = toUpper(courseNum);

    Course* course = tree.find(courseNum);
    if (course == nullptr) {
        // Error handling: course not found
        cout << courseNum << " not found." << endl;
        return;
    }

    // Print course number and title
    cout << course->courseNumber << ", " << course->title << endl;
    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "none";
    }
    else {
        // Print prerequisites separated by commas
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) cout << ", ";
        }
    }
    cout << endl;
}

int main() {
    BST courseTree;      // The BST to hold all courses
    bool dataLoaded = false;  // Flag to track if data has been loaded
    string filename;     // User-provided filename
    string choice;       // User menu choice

    // Welcome message matching sample output
    cout << "Welcome to the course planner." << endl << endl;

    // Main menu loop
    while (true) {
        // Display menu options
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;

        if (choice == "1") {
            cout << "Enter the filename (e.g., CS300_ABCU_Advising_Program_Input.csv): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear any leftover input from previous cin >>
            getline(cin, filename);  // Now read the full line with spaces

            if (loadCourses(filename, courseTree)) {
                dataLoaded = true;
                cout << "Data loaded successfully." << endl;
            }
        }

        else if (choice == "2") {
            // Check if data is loaded before printing
            if (!dataLoaded) {
                cout << "Please load data first." << endl;
            }

            else {
                // Print header and sorted list
                cout << "Here is a sample schedule:" << endl << endl;
                courseTree.printInOrder();
                cout << endl;
            }
        }

        else if (choice == "3") {
            // Check if data is loaded before querying
            if (!dataLoaded) {
                cout << "Please load data first." << endl;
            }

            else {
                cout << "What course do you want to know about? ";
                cin >> choice;
                cout << endl;
                printCourseInfo(courseTree, choice);
            }
        }

        else if (choice == "9") {
            // Exit message matching sample
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        else {
            // Invalid input handling matching sample
            cout << choice << " is not a valid option." << endl;
        }

        cout << endl;  // Extra newline for readability between interactions
    }

    return 0;
}

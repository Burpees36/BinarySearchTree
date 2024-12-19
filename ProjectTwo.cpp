//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Hunter Brashears
// Version     : 1.0
// Description : Final Project 
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

// structure for Courses
struct Course {
    string courseNumber;
    string name;
    vector<string> prerequisites;
};

// structure for Node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);


public:
    BinarySearchTree();
    void InOrder();
    void insertCourse(Course course);
    Course searchCourse(string courseNumber);
};

// Constrcutor 
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}


void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

// sorts the tree alphanumerically 
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr (traverse to bottom of tree)
    if (node != nullptr) {
        inOrder(node->left); // recursive call to keep getting smallest node
        cout << node->course.courseNumber << ", " << node->course.name << endl;
        inOrder(node->right); // do the same to the right side
    }
}

void BinarySearchTree::insertCourse(Course course) {
    // if bst is empty, enter as root
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}

void BinarySearchTree::addNode(Node* node, Course course) {
    // if course is smaller than the node, add to left subtree
    if (course.courseNumber < node->course.courseNumber) {
        // if no left node
        if (node->left == nullptr) {
            node->left = new Node(course); // this becomes left node
        }
        else {
            // recurse down the left node
            this->addNode(node->left, course);
        }
    }
    // add to right subtree
    else {
        // if right is empty
        if (node->right == nullptr) {
            // add new course to right
            node->right = new Node(course);
        }
        else {
            // recursion call to traverse down the tree
            addNode(node->right, course);
        }
    }
}


void loadCourse(string csvPath, BinarySearchTree* bst) {
    ifstream file(csvPath);

    // error handleing 
    if (!file.is_open()) {
        cout << "Error: Could not open the file " << csvPath << endl;
    }
    else {
        string line;
        while (getline(file, line)) {
            // create course objecet 
            Course course;
            istringstream stream(line);
            string token;

            // first token to course number
            if (getline(stream, token, ',')) {
                course.courseNumber = token;
            }
            // second token to course name
            if (getline(stream, token, ',')) {
                course.name = token;
            }
            // add any other data as prerequisite
            while (getline(stream, token, ',')) {
                course.prerequisites.push_back(token);
            }
            // add course to tree
            bst->insertCourse(course);
        }
        file.close();
        cout << "Courses loaded successfully." << endl;
    }
}

Course BinarySearchTree::searchCourse(string courseNumber) {
    // start at the root
    Node* current = root;
    // while not empty 
    while (current != nullptr) {
        // if course equals root, return course
        if (courseNumber == current->course.courseNumber) {
            return current->course;
        }
        // if course is smaller than current, traverse left
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            // otherwise traverse right
            current = current->right;
        }
    }
    return Course(); // return empty course if not found
}

// Display User Menu
void displayMenu() {
    cout << "**********************************" << endl;
    cout << "Welcome to the course planner." << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "***********************************" << endl;
    cout << "What would you like to do?" << endl;
    cout << endl;
}


void printCourse(Course course) {
    // print course info
    cout << course.courseNumber << ", " << course.name << endl;
    cout << "Prerequisites: ";
    // if any prerequisites exist, print them
    for (int i = 0; i < course.prerequisites.size(); ++i) {
        cout << course.prerequisites[i];
        // print prerequisite with comma, unless last one in list
        if (i != course.prerequisites.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

 
int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    int choice;
    string csvPath;
   
    while (true) {

        displayMenu();
        cin >> choice;
        // error handleing
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number." << endl;
        }
            
        switch (choice) {
        // load course
        case 1:
            cout << "Enter CSV file name to load: " << endl;
            cin >> csvPath;
            loadCourse(csvPath, bst);
            break;
        // print courses in order
        case 2:
            bst->InOrder();
            break;
        // prompt user for course, and print it 
        case 3: {
            string courseToSearch;
            cout << "Enter a course: ";
            cin >> courseToSearch;
            Course course = bst->searchCourse(courseToSearch);
            if (!course.courseNumber.empty()) {
                printCourse(course);
            }
            else {
                cout << "Course Number " << courseToSearch << " not found." << endl;
            }
            break;
        }
        // exit program
        case 9: 
            cout << "Good bye." << endl;
            return 0;
       
        }
    }
    return 0;
}

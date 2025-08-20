// CS300project2AlbertWaterman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//load data from CSV file and into a binary search tree, displays courses, searching by Id, and prereqs for that course.

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <limits>
using namespace std;
//structure for course information
struct Courses {
	string Id; //ID or key
	string Name; //name of course
	vector <string> prereq; //prereqs for course ID
};
//tree structure
struct Node {
	Courses course; //course data node
	Node* left;
	Node* right;
//default constructor sets up node with a course
	Node(const Courses& aCourse) : course(aCourse) {
		left = nullptr;
		right = nullptr;
	}
};
//class for Binary search tree
class BST {
private:
	Node* root; //pointer for node root

	void addNode(Node* node, Courses course);//ensures new node is in correct position
	void inOrder(Node* node);//traversal order

public:
	BST(); //constructor
	virtual ~BST(); //destructor
	void InOrder(); //method for traversal order
	Courses Search(string Id);//search course ID
	void Insert(Courses course); //Inserts course
};
//default constructor
BST::BST() { //empty tree
	root = nullptr;
}
//destructor
BST::~BST() {//already empty tree so i didnt include cleanup

}
//inserting courses
void BST::Insert(Courses course) {
	// if root equarl to null ptr
	  // root is equal to new node bid
	// else
	  // add Node root and bid
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);//adds course starting from root
	}
}
void BST::addNode(Node* node, Courses course) {
	if (course.Id < node->course.Id) {//less then current node it places left
		if (node->left == nullptr) {//left child null insert bid
			node->left = new Node(course);
		}
		else {
			addNode(node->left, course);//recurse left
		}
	}
	else {
		if (node->right == nullptr) {//if right null insert on right
			node->right = new Node(course);
		}
		else {
			addNode(node->right, course);//recurse right
		}
	}
}
//Traverse tree in order
void BST::InOrder() {
	// call inOrder fuction and pass root 
	this->inOrder(root);
}
void BST::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);//traverse left tree
		cout << node->course.Id << ", " << node->course.Name << endl;//print function for Id and Name

		inOrder(node->right);//traverse right
	}
}
//searching for a course
Courses BST::Search(string Id) {
	// set current node equal to root
	Node* current = root;
	while (current != nullptr) {
		//if the course matches return course
		if (current->course.Id == Id) {
			return current->course;
		}
		else if (Id < current->course.Id) {
			current = current->left;//course less go left
		}
		else {
			current = current->right;//course greater put right
		}
	}
	//returns empty if not found
	Courses course;
	return course;
}
void displayCourse(Courses course) { //displaying courses and including prereq 
	cout << course.Id << ", " << course.Name << endl;
	cout << "Prerequisites: ";
	if (course.prereq.empty()) {
		cout << "None";
	}
	else {
		for (size_t i = 0; i < course.prereq.size(); ++i) {
			if (i > 0) {
				cout << ", ";
			}
			cout << course.prereq[i];
		}
	}
	cout << endl << endl;
	return;
}
void courseLoader(string csvPath, BST* bst) {
	ifstream myFile(csvPath);

	if (!myFile.is_open()) {
		cout << "Error opening file " << csvPath << endl;
		return;
	}

	string line;
	int courseCount = 0;

	// Reads lines in file
	while (getline(myFile, line)) {
		stringstream ss(line);
		string token;
		int count = 0;
		Courses course;
		vector<string> prereqs;

		// Split each line by comma like instructions said to
		while (getline(ss, token, ',')) {
			if (count == 0) {
				course.Id = token;
			}
			else if (count == 1) {
				course.Name = token;
			}
			else {
				prereqs.push_back(token);
			}
			count++;
		}

		// Add all prerequisites to the course
		for (unsigned int i = 0; i < prereqs.size(); ++i) {
			course.prereq.push_back(prereqs.at(i));
		}

		// Insert into the BST
		bst->Insert(course);
		courseCount++;
	}

	cout << courseCount << " courses loaded from \"" << csvPath << "\"." << endl;
}
int main(int argc, char* argv[]) { //menu and main function
	string csvPath, CourseKey;
	switch (argc) { //command line argument
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		CourseKey = argv[2];
		break;
	default:
		csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
		
	}
	BST* bst;
	bst = new BST();
	Courses course;
	int choice = 0;
	while (choice != 9) {//menu loop
		cout << "Welcome to the course planner." << endl;
		cout << endl;
		cout << "  1. Load Data Structure." << endl;
		cout << "  2. Print Course List." << endl;
		cout << "  3. Print course." << endl;
		cout << "  9. Exit" << endl;
		cout << endl;
		cout << "What would you like to do? ";
		
		if (!(cin >> choice)) { // handles non-numeric input error handling
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //using <limits> and https://stackoverflow.com/questions/62849491/how-to-limits-value-user-input-int-max-in-c as a source
			cout << "Invalid input." << endl << endl; // I also used this in another CS class maybe 210 but dont truly remember great tool though.
			cout << endl;
			continue;
		}

		if (choice != 1 && choice != 2 && choice != 3 && choice != 9) { // validates menu options
			cout << choice << " is not a valid option." << endl << endl;
			cout << endl;
			continue;
		}

		switch (choice) {
		case 1:
			courseLoader(csvPath, bst);
			cout << endl;
			break;




		case 2:
			cout << "Here is a sample schedule:" << endl;
			cout << endl;
			bst->InOrder();
			cout << endl;
			break;

		case 3:
			cout << "What course do you want to know about?" << endl;
			cin >> CourseKey;
			for (size_t i = 0; i < CourseKey.length(); ++i) { // added <cctype> and found this code online to change any character to uppercase since all courses are listed as uppercase
				CourseKey[i] = std::toupper(static_cast<unsigned char>(CourseKey[i])); //https://www.geeksforgeeks.org/dsa/convert-alternate-characters-string-upper-case/
			}
			course = bst->Search(CourseKey); //search and display course
			
			if (!course.Id.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course Id " << CourseKey << " not found." << endl;
				cout << endl;
			}
			break;
		case 9:
			cout << "Thank you for using the course planner!" << endl;
			cout << endl;
			break;
		}
	}
	cout << "By Albert Waterman";
			return 0;
}


//Jacob Brents
//CS-300-1025
//10/19/2025
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

//Course structure to hold course information
struct Course {
	string courseName;
	string courseNumber;
	string prerequisites;
};

//Node structure for the binary search tree
struct Node {
	Node* left;
	Node* right;
	Course course;
	Node() {
		left = nullptr;
		right = nullptr;
	}
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

//Binary Search Tree class
class BinarySearchTree {
private:
	Node* root;
	//Functions for adding nodes and in-order traversal
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	//Constructor and public functions
	BinarySearchTree();
	void Insert(Course course);
	void PrintAll();
	Course Search(string courseNumber);
};
	//Add node to the tree, Used recursively in Insert function
	void BinarySearchTree::addNode(Node* node, Course course) {
		//if course number is less than current node's course number, go left
		if (course.courseNumber < node->course.courseNumber) {
			if (node->left == nullptr) {
				node->left = new Node(course);
			}
			//else go left again
			else {
				addNode(node->left, course);
			}
		}
		//else go right
		else {
			if (node->right == nullptr) {
				node->right = new Node(course);
			}
			//else go right again
			else {
				addNode(node->right, course);
			}
		}
	}
	//In-order traversal to print all courses in order
	void BinarySearchTree::inOrder(Node* node) {
		//If not empty, traverse left, print node, traverse right
		if (node != nullptr) {
			inOrder(node->left);
			cout << node->course.courseNumber << ": " << node->course.courseName << " | " << node->course.prerequisites << endl;
			inOrder(node->right);
		}
	}
	//Constructor initializes root to nullptr
	BinarySearchTree::BinarySearchTree() {
		root = nullptr;
	}
	//Insert course into the tree
	void BinarySearchTree::Insert(Course course) {
		//If tree is empty, create root node
		if (root == nullptr) {
			root = new Node(course);
		}
		//Else, add node to the tree
		else {
			addNode(root, course);
		}
	}
	//Print all courses in order
	void BinarySearchTree::PrintAll() {
		//Start in-order traversal from root
		inOrder(root);
	}
	//Search for a course by course number
	Course BinarySearchTree::Search(string courseNumber) {
		//Start from root
		Node* current = root;
		//while current node is not null
		while (current != nullptr) {
			//If course number is less than current node's course number, go left
			if (courseNumber < current->course.courseNumber) {
				current = current->left;
			}
			//Else if course number is greater, go right
			else if (courseNumber > current->course.courseNumber) {
				current = current->right;
			}
			//Else, course found
			else {
				return current->course;
			}
		}
		//Course not found, return empty course
		Course emptyCourse;
		return emptyCourse;
	}



//Global binary search tree instance
BinarySearchTree bst;

//Function to get file name from user
string getFileName() {

	string fileName;
	cout << "Enter the file name: ";
	//I was having issues with geting the fileName after other cin statements, so I added this line to clear the buffer
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, fileName);
	return fileName;



}

//Function to load file and populate the binary search tree
int LoadFile(string fileName) {

	//Open the file
	ifstream inputFile(fileName);

	//Check if file opened successfully
	if (!inputFile) {
		cerr << "Error opening file: " << fileName << endl;
		return 1;
	}

	string line;
	//Read each line from the file
	while (getline(inputFile, line)) {
		//Skip empty lines
		if (line.empty()) {
			continue;
		}
		//Split line by commas
		stringstream ss(line);
		//Temporary storage for tokens
		vector<string> tokens;
		string token;

		//Get each token separated by comma
		while (getline(ss, token, ',')) {
			//push token to temporary vector, I tried to limit the vector usage to make the code a bit faster
			tokens.push_back(token);
		}

		//Check if there are at least 2 main tokens
		if (tokens.size() < 2) {
			cerr << "Invalid line format: " << line << endl;
			continue;
		}

		//If there are at least 2 tokens, create a Course object
		if (tokens.size() >= 2) {
			Course course;
			course.courseNumber = tokens[0];
			course.courseName = tokens[1];

			//If there are more than 2 tokens, concatenate them as prerequisites
			for (int i = 2; i < tokens.size(); ++i) {
				if (i > 2) {
					course.prerequisites += ", ";
				}
				course.prerequisites += tokens[i];
			}
			//Insert course into the binary search tree
			bst.Insert(course);
		}
		else {
				cerr << "Invalid line format: " << line << endl;
				continue;
		}
	}
	//All done, close the file
	inputFile.close();
	cout << "File loaded successfully: " << fileName << endl;

}

//Function to print a specific course
void PrintCourse() {
	string courseNumber;
	cout << "Enter the course number: ";
	cin >> courseNumber;
	//Search for the course in the binary search tree
	Course course = bst.Search(courseNumber);
	//If course found, print details
	if (!course.courseNumber.empty()) {
		cout << course.courseNumber << ": " << course.courseName << " | " << course.prerequisites << endl;
	}
	//Else, print not found message
	else {
		cout << "Course not found: " << courseNumber << endl;
	}
}


//Function to display menu options
void MenuDisplay()
{
	//Display menu options
	cout << "Menu Options:\n";
	cout << "1. Load File\n";
	cout << "2. Print Course List\n";
	cout << "3. Print All\n";
	cout << "9. Exit\n";
}

//Function to handle menu input and call appropriate functions
void MenuInput() {
	int userInput;
	bool done = false;

	while (!done) {
		MenuDisplay();
		//Get user input
		cout << "Enter your choice: ";

		//Input validation
		if(!(cin >> userInput)) {
			//I was having issues with invalid input causing infinite loops, so I added this to clear the buffer
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		//Load file function
		if (userInput == 1) {
			string fileName = getFileName();
			LoadFile(fileName);

			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Print course list function
		else if (userInput == 2) {
			bst.PrintAll();

			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		// Print specific course function
		else if (userInput == 3) {
			PrintCourse();

			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		//Exit program
		else if (userInput == 9) {
			done = true;
		}

		else {
			//Format error message
			cout << userInput << " is not a valid input. Please enter 1, 2, 3, or 9.\n";

			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}


}

int main()
{
	MenuInput();

	return 0;
   
}
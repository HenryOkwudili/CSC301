#include <iostream>
#include <string>
using namespace std;

// Linked list node structure
class Node {
public:
	string data;
	Node* next;
	Node (string value) {
		data = value;
		next = NULL;
	}
};



// Linked list class
class LinkedList {
private:
	Node* head;

public:
	LinkedList() {
		head = NULL;

}

	// Function to add a new node at the beginning of the linked list
	void addAtBeginning(string value) {
		Node* newNode = new Node(value);
		newNode->next = head;
		head = newNode;
	}

// Function to add a new node at the end of the linked list
	// Function to add a new node at the end of the linked list
	void addAtEnd(string value) {
		Node* newNode = new Node(value) ;
		if (head == NULL) {
			head = newNode;
			return;
		}	
		Node* current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}

// Function to add a new node at the middle of the linked list
	void addAtPosition(string value, int position) {
		Node* newNode = new Node(value);
		if (position == 0) {
			newNode->next = head;
			head = newNode;
			return;
		}
		Node* current = head;
		for (int i = 0; i < position - 1 && current != NULL; i++) {
			current = current->next;
		}
		if (current == NULL) {
			cout << "Invalid position\n";
			return;
		}
		newNode->next = current->next;
		current->next = newNode;
	}
	
	// Function to print the linked list
	void printList() {
		Node* current = head;
		while (current != NULL) {
			cout << current->data << " ";
			current = current->next;
		}
		cout << endl;
	}
};

int main() {
	LinkedList list;
	
	// Adding nodes to the beginning
	
	string name, name1, name2, name3, name4, name5, name6;
	cout<<"Input name"<<endl;
	
	getline(cin, name);
	cout<<"Input name"<<endl;

	getline(cin, name1);
	cout<<"Input name"<<endl;

	getline(cin, name2);
	cout<<"Input name"<<endl;

	getline(cin, name3);
	cout<<"Input name"<<endl;

	getline(cin, name4);
	cout<<"Input name"<<endl;

	getline(cin, name5);
	cout<<"Input name"<<endl;

	getline(cin, name6);
	
	list.addAtBeginning(name);
	list.addAtBeginning(name1);
	list.addAtBeginning(name2);
	cout << "Linked list after adding nodes to the beginning: ";
	list.printList();
	
	// Adding nodes to the end
	list.addAtEnd(name3);
	list.addAtEnd(name4);
	list.addAtEnd(name5);
	cout<<"Linked list after adding nodes to the end: ";
	list.printList();
	
	//Adding node to the middle
	list.addAtPosition(name6, 2);
	cout << "Linked list after adding node to the middle: ";
	list.printList();
	
	return 0;
}
	
	
	
	
	

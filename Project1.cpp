#include <iostream>
using namespace std;

const int MAX_SIZE = 5;

class Stack {
	private:
		int arr[MAX_SIZE];
		int top;
		
	public:
		Stack(){
			top = -1;
		}
		
		bool push(int value){
			if (top >= MAX_SIZE - 1){
				cout<<"Stack Overflow\n";
				return false;
			}
			top++;
			arr[top]=value;
			return true;
		}
		
		bool pop(){
			if (top < 0) {
				cout << "Stack Underflow\n";
				return false;
			}
			top--;
			return true;
		}
		
		int peek(){
			if (top < 0){
				cout<< "Stack is empty\n";
				return -1;
			}
			return arr[top];
		}
		
		bool isEmpty(){
			return top < 0;
		}
};

int main(){
	Stack s;
	s.push(5);
	s.push(10);
	s.push(15);
	cout << "Top element: "<< s.peek() << endl;
	s.pop();
	cout << "Top element after pop: "<< s.peek() << endl;
	s.pop();
	s.pop();
	s.pop();
	return 0;
	}

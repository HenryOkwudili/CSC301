#include <iostream>
#include <string>

using namespace std;

struct Node{
	string name;
	string comment;
	Node* left;
	Node* right;
	
	
	Node(string value1, string value2){
		name = value1;
		comment = value2;;
		left=NULL;
		right=NULL;
	}
};

class BinaryTree {
	public:
		Node* root;
		
		
		BinaryTree(){
			root = NULL;
		}
		
		
		void insert(string value1, string value2) {
			root = insertNode(root, value1, value2);
		}
		
		
		void inorderTraversal(){
			cout<<"In-order Traversal: ";
			inorder(root);
			cout<<endl;
		}
		
		void preorderTraversal() {
			cout<<"Pre-order Traversal: ";
			preorder(root);
			cout<<endl;
		}
		
		void postorderTaversal() {
			cout<<"Post-order Traversal: ";
			postorder(root);
			cout<<endl;
		}
		
	private:
		Node* insertNode(Node* node, string value1, string value2) {
			if (node == NULL) {
				return new Node(value1, value2);
			}
			if(value1 < node->name && value2 < node->comment){
				node->left=insertNode(node->left, value1, value2);
			}else{
				node->right = insertNode(node->right, value1, value2);
			}
			return node;
		}
		
		
		void inorder(Node* node) {
			if (node == NULL) {
				return;
			}
			inorder(node->left);
			cout<<node->name<<"";
			cout<<node->comment<<"";
			inorder(node->right);
		}
		
		void preorder(Node* node) {
			if (node == NULL) {
				return;
			}
			cout<<node->name<<" ";
			cout<<node->comment<<"";
			preorder(node->left);
			preorder(node->right);
		}
		
		void postorder(Node* node){
			if(node == NULL) {
				return;
			}
			postorder(node->left);
			postorder(node->right);
			cout<<node->name<< " ";
			cout<<node->comment<<"";
		}
};

int main() {
	BinaryTree tree;
	
	tree.insert("Henry", "Okwudili");
	tree.insert("Basit", "Inaolaji");
	tree.insert("Razaq", "Femi-Sumonu");
	tree.insert("Bare", "Adesokan");
	tree.insert("Daniel", "Onakoya");
	tree.insert("Ifunanya", "Egwuatu");
	tree.insert("Malik", "Memud");
	
	tree.inorderTraversal();
	tree.preorderTraversal();
	tree.postorderTaversal();
}

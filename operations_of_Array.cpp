#include <iostream>
using namespace std;

int insertintoArray(){
	int arr[5];
	
	cout<<"Insert into array: ";
	for (int i = 0; i<5; i++){
		arr[i]=i;
	}
	
	for (int i=0; i<5; i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	return 0;
}

int search(){
	int arr[5]={7, 13, 15, 17, 19};
	int size = 5;
	int value;
	
	cout<<"Enter the element you want to search for: ";
	cin>>value;
	
	for (int i=0; i<size; i++){
		if(arr[i] == value){
			cout<<"Element found at index: "<<i<<"."<<endl;
			return 0;
		}
	}
	
	cout<<"Element not found in the array.";
	return 0;
}

int main(){
	int arr[5]={7, 13, 15, 17, 19};
	int size = 5;
	int index, value;
	
	cout<<"Enter the index of the element you want to update: ";
	cin>>index;
	cout<<"Enter the new value: ";
	cin>>value;
	
	if(index<0||index>=size){
		cout<<"Invalid index."<<endl;
		return 0;
	}
	arr[index] = value;
	
	cout<<"Element updated successfully."<<endl;
	cout<<"New array elements: ";
	for (int i = 0; i<size; i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;
	return 0;
}
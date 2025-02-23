#include <iostream>

int main(){
	int n = 5;
	int arr[n] = {10,6,4,8,2};
	int i, j, min;
	for (i=0; i<n-1; i++){
		min = i;
		
		for (j = i+1; j < n; j++){
		if (arr[j]<arr[min]){
			min = j;
		}
	}
	
	int temp = arr[min];
	arr[min] = arr[i];
	arr[i] = temp;
	cout<<"sorting\n";
	cout<<arr,n<<endl;
	
	
	}
	
}

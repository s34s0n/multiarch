#include <stdio.h>

int recurse(int n);

int main(){
	int key;
	printf("Enter the magic key: ");
	scanf("%d",&key);
	if(key == recurse(8)){
		printf("You have solved the challenge!!\n");
	}
	else{
		printf("Wrong key..... :(\n");
	}
	return 0;
}

int recurse(int n){
	if(n == 1 || n == 0){
		return n;
	}
	else
		return 3*recurse(n-1)+2*recurse(n-2)+1;
}
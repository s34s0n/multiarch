#include <stdio.h>

int magic(char *s);

int main(){
	char s[30];
	printf("Enter flag : ");
	scanf("%s",s);
	if(magic(s)){
		printf("Correct\n");
	}
	else{
		printf("Wrong\n");
	}
}

int magic(char s[]){
	int arr[] = {59, 69, 66, 60, 56, 57, 113, 47, 20, 114, 110, 20, 67, 113, 45, 20, 45, 73, 109, 45, 20, 73, 109, 47, 61, 54};
	for(int i=0 ; i < 26 ; i++){
		if((((int)s[i]^0x56)+11) != arr[i]){
			return 0;
		}
	}
	return 1;
}
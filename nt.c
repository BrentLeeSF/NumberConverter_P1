#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	int i;
	char* arr = malloc(sizeof(char) * argc);

	for(i = 0; i < argc; i++) {
		printf("%s\n",argv[i]);
		if(i >= 1) {
			arr[i-1] = *argv[i];
		}
	}
	printf("Argc: %d\n",argc);
	int j, sLength;
	for(j = 0; j < argc-1; j++) {
		sLength = strlen(arr);
	}
	/*printf("Number is: %d\n",len);
	for(i = 0; i < len; i++) {
		printf()
	}*/



	return 0;
}
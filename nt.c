
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void breakStringUp(char *huh, int index, int argc);
void printNeg(char *huh);

int main(int argc, char *argv[]) {

	if(argc == 1) {
		printf("\n** Please include numbers after the file name **\n\n");
		exit(1);
	}

	/*

	RETURN INVALID NUMBERS!!!!

	*/

	
	int i;
	//char* arr = malloc(sizeof(char) * argc-1);

	for(i = 0; i < argc; i++) {
		printf("ARR: %s\n",argv[i]);
		if(i >= 1) {
			char *newStr = argv[i];
			breakStringUp(newStr, i, argc);
		}
	}
	return 0;
}


void breakStringUp(char *huh, int index, int argc) {
	
	int len = strlen(huh);
	int i;
	printf("Break up: %d\n",argc);

    for(i = 0; i < len; i++) {
    	if(huh[0] == 45) {
    		printNeg(huh);
    		break;
    	}
		printf("%c\n",huh[i]);
	}
}


void printNeg(char *huh) {
	printf("inside Neg %s\n",huh);
}




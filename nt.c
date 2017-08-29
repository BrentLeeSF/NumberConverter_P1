
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

	/*

	RETURN INVALID NUMBERS!!!!
	
	*/

	// get any number and change it to binary and hex
	// get all values and convert to binary, then binary to everything else

void breakStringUp(char *huh);
void printNeg(char *huh);

int main(int argc, const char *argv[]) {

	if(argc == 1) {
		printf("\n** Please include numbers after the file name **\n\n");
		exit(1);
	}
	
	int i, neg, bin, hex, b, r = 0;
	
	for(i = 0; i < argc; i++) {

		if(i >= 1) {

			int len = strlen(argv[i]);
			printf("uhh %s\n", argv[i]);
			char* arr = malloc(sizeof(char) * len);
			strcpy(arr, argv[i]);
			int j;

			for(j = 0; j < len; j++) {
				if(arr[0] == 45) {
					printf("Neg\n");
					neg++;
				}
				if(neg > 0 && arr[1] == 98) {
					b++;
					printf("B\n");
				}
				if(neg > 0 && arr[1] == 114) {
					r++;
					printf("R\n");
				}
				printf("%c\n", arr[j]);
			}
		}
	}
	//breakStringUp(arr);
	return 0;
}


void breakStringUp(char *huh) {
	
	int i, j;
	int arrLength = strlen(huh);
	printf("Whaaaa... %s\n",huh);

    for(i = 0; i < arrLength; i++) {

    	int len = strlen(&huh[i]);
    	printf("LEN %d\n", len);

    	for(j = 0; j < len; j++) {
    		if(huh[0] == 45) {
    			printNeg(huh);
    			break;
    		}
			printf("%c\n",huh[i]);
    	}
    	
	}
}


void printNeg(char *huh) {
	printf("inside Neg %s\n",huh);
}





#include <stdio.h>
#include <string.h>
#include <stdlib.h>

	
// get any number and change it to binary and hex
// get all values and convert to binary, then binary to everything else

void sendToCalculate(char *arr, int neg, int bin, int hex, int b, int r);


int main(int argc, const char *argv[]) {

	if(argc == 1) {
		printf("\n** Please include numbers after the file name **\n\n");
		exit(1);
	}
	
	int i, neg, bin, hex, b, r, zero = 0;
	char *arr;
	
	for(i = 0; i < argc; i++) {

		if(i >= 1) {

			int len = strlen(argv[i]);
			//printf("Length: %d\n",len);
			arr = malloc(sizeof(char) * len);
			strcpy(arr, argv[i]);
			int j;

			for(j = 0; j < len; j++) {

				printf("%c\n", arr[j]);

				if(arr[j] == 45) {
					//printf("Neg\n");
					neg++;
					continue;
				}
				if(len == 2 && neg > 0) {
					if(neg > 0 && arr[1] == 98) {
						b++;
						//printf("B\n");
						continue;
					}
					if(neg > 0 && arr[1] == 114) {
						r++;
						//printf("R\n");
						continue;
					}
				}
				if(len > 2) {
					if(arr[j] == 98) {
						bin++;
						continue;
					}
					if(arr[j] == 120) {
						hex++;
						continue;
					}
					if(arr[j] == 48) {
						zero++;
						continue;
					}
				}
				/* 45 = -, 48-57 = 0-9, 65-70 = A-F, 98 = b, 114 = r */
				if((arr[j] < 45) || (arr[j] == 46 || arr[j] == 47) || (arr[j] > 57 && arr[j] < 65) || (arr[j] > 71 && arr[j] < 98) || (arr[j] > 98 && arr[j] < 114) || arr[j] > 114) {
					printf("INVALID NUMBER! %c\n", arr[j]);
					exit(1);
				}
			}
			printf("\n");
		}
	}
	sendToCalculate(arr, neg, bin, hex, b, r);
	/*

	RETURN INVALID NUMBERS!!!!
	
	*/

	printf("\nNeg %d, Bin %d, Hex %d, -b %d, -r %d, zero %d\n", neg, bin, hex, b, r, zero);

	return 0;
}


void sendToCalculate(char *arr, int neg, int bin, int hex, int b, int r) {
	printf("\nINSIDE: %s, Neg %d, Bin %d, Hex %d, -b %d, -r %d\n", arr, neg, bin, hex, b, r);
	
}





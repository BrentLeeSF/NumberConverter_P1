

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

	
// get any number and change it to binary and hex
// get all values and convert to binary, then binary to everything else

/*
TO GET NUMBERS!!!
Take in numbers and multiply them by their place in numbers (100's, 10's 1's).
Digit Ex. 643 = 6x100 + 4x10 + 3x1 = 643
Binary Ex. 1010000011 -> 1x512 + 0x256 + 1x128 + 0x64 + 0x32 + 0x16 0x8 + 0x4 + 1x2 + 1x1 = 643
Etc
*/


void validateStuff(char *arr, int neg, int bin, int hex, int b, int r);


int main(int argc, char *argv[]) {

	if(argc == 1 || argc == 0) {
		printf("\n** Please include numbers after the file name **\n\n");
		exit(1);
	}
	
	int biNum = 0, bFlag = 0, bIndex = 0, i = 0, hex = 0, one = 0 , neg = 0, rFlag = 0, zero = 0, zeroStart = 0;
	char *arr;
	char *biNumArr = malloc(sizeof(char) * 100);
	
	for(i = 0; i < argc; i++) {

		if(i >= 1) {

			int len = strlen(argv[i]);

			// INVALID LENGTHS
			if(len > 34 || (len > 10 && len%4 == 3) || (len > 10 && len%4 == 1)) {
				printf("The number %s is too long or invalid\n", argv[i]);
				exit(1);
			}
			
			printf("Length: %d\n",len);
			arr = malloc(sizeof(char) * len);
			strcpy(arr, argv[i]);
			printf("Num %s\n", arr);
			int j = 0;
			
			one = 0, neg = 0, zero = 0;


			for(j = 0; j < len; j++) {

				printf("Inside Loop: %c\n",arr[j]);

				// INVALID CHARACTERS
				/* 45 = -, 48-57 = 0-9, 65-70 = A-F, 98 = b, 114 = r */
				if((arr[j] < 45) || (arr[j] == 46 || arr[j] == 47) || (arr[j] > 57 && arr[j] < 65) || (arr[j] > 71 && arr[j] < 98) || (arr[j] > 98 && arr[j] < 114) || (arr[j] < 120 && arr[j] > 114) || (arr[j] > 120)) {
					printf("1 INVALID NUMBER! %c\n", arr[j]);
					exit(1);
				}

				//printf("%c\n", arr[j]);

				// ZERO
				if(arr[j] == 48) {
					// ZERO START
					if(j == 0) {
						zeroStart++;
						continue;
					}
					else {
						zero++;
						continue;
					}
				}
				// ONE
				if(arr[j] == 49) {
					one++;
					continue;
				}

				// BINARY NUMBER
				if(arr[j] == 98 && zeroStart > 0 && j == 1) {
					biNum++;
					bIndex = i;
					continue;
				}
				if(j > 1 && zeroStart > 0 && biNum > 0) {
					if(bIndex+1 == i) {
						
					}
					if(arr[j] == 48) {
						zero++;
						continue;
					}
					if(arr[j] == 49) {
						one++;
						continue;
					}
					else {
						printf("1 INVALID NUMBER! %c\n", arr[j]);
						exit(1);
					}
				}


				// HEX NUMBER
				if(zeroStart > 0 && j == 1 && arr[j] == 120) {
					hex++;
					continue;
				}
				if(hex > 0 && j > 1) {
					if(arr[j] > 64 && arr[j] < 71) {
						printf("Hex Letter son!\n");
					}
				}
				
				// NEGATIVE
				if(arr[j] == 45) {
					if(j == 0) {
						neg++;
						continue;
					}
					else {
						printf("3 INVALID NUMBER! %c\n", arr[j]);
						exit(1);
					}
				}

				// FLAGS -b and -r
				if(len == 2 && neg > 0) {
					if(neg > 0 && arr[j] == 98 && j == 1) {
						bFlag++;
						
						continue;
					}
					if(neg > 0 && arr[j] == 114 && j == 1) {
						rFlag++;
						//printf("R\n");
						continue;
					}
				}
				
			} // end inner loop

			printf("\n");
			
			printf("Length: %d\n", len);
			printf("ONE: %d\n",one);

			if(zeroStart + zero + one == len && len%4 == 0) {
				printf("valid bit %s\n",arr);
				strcat(biNumArr, argv[i]);
			}
		}
	} // end outer for loop

	validateStuff(biNumArr, neg, biNum, hex, bFlag, rFlag);

	printf("\nStrrrr %d, Neg %d, Bin %d, Hex %d, -b %d, -r %d, zero %d\n", (int)biNumArr, neg, biNum, hex, bFlag, rFlag, zero);

	free(biNumArr);
	free(arr);


	return 0;
}


void validateStuff(char *arr, int neg, int biNum, int hex, int bFlag, int rFlag) {
	printf("\nINSIDE: %s, Neg %d, Bin %d, Hex %d, -b %d, -r %d\n", arr, neg, biNum, hex, bFlag, rFlag);
	
}





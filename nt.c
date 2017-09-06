

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


void sendStuff(char *biNumArr, char *rArr, int neg, int zeroB, int hex, int b, int r);


int main(int argc, char *argv[]) {

	if(argc == 1 || argc == 0) {
		printf("\n** Please include numbers after the file name **\n\n");
		exit(1);
	}
	
	int zeroB = 0, bFlag = 0, bIndex = 0, i = 0, hex = 0, one = 0 , neg = 0, rFlag = 0, rIndex = 0, zero = 0, zeroStart = 0;
	char *arr;
	char *biNumArr = malloc(sizeof(char) * 100);
	char *rArr = malloc(sizeof(char) * 100);
	printf("Arr %p, Bi Num %p, R arr %p\n", arr, biNumArr, rArr);
	int bitCount = 0;
	
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

			// NUMBER AFTER -b
			if(i == bIndex+1 && bIndex > 0) {
				bitCount = atoi(argv[i]);
				printf("\nBit Count: %d\n\n",bitCount);
				continue;
			}
			// NUMBERS AFTER -r
			if(i == rIndex+1 && rIndex > 0) {
				strcpy(rArr, argv[i]);
				continue;
			}

			for(j = 0; j < len; j++) {

				printf("Inside Loop: %c\n",arr[j]);

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

				// BINARY NUMBER - 0b start
				if(arr[j] == 98 && zeroStart > 0 && j == 1) {
					bIndex = i;
					zeroB++;
					continue;
				}
				if(j > 1 && zeroStart > 0 && bIndex > 0) {
					if(arr[j] > 49 && arr[j] < 48) {
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
					if(arr[j] < 48 || (arr[j] > 57 && arr[j] < 65) || arr[j] > 70) {
						printf("1 INVALID NUMBER! %c\n", arr[j]);
						exit(1);
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
						bIndex = i;
						continue;
					}
					if(neg > 0 && arr[j] == 114 && j == 1) {
						rFlag++;
						rIndex = i;
						continue;
					}
				}

				// INVALID CHARACTERS
				/* 45 = -, 48-57 = 0-9, 65-70 = A-F, 98 = b, 114 = r */
				if((arr[j] < 45) || (arr[j] == 46 || arr[j] == 47) || (arr[j] > 57 && arr[j] < 65) || (arr[j] > 71 && arr[j] < 98) || (arr[j] > 98 && arr[j] < 114) || (arr[j] < 120 && arr[j] > 114) || (arr[j] > 120)) {
					printf("1 INVALID NUMBER! %c\n", arr[j]);
					exit(1);
				}
				
			} // end inner loop

			printf("\n");
			
			printf("Length: %d\n", len);
			printf("ONE: %d\n",one);

			// binary only 1s and 0s
			if((zeroStart + zero + one == len) && (len%4 == 0)) {
				printf("valid bit %s\n",arr);
				strcpy(biNumArr, argv[i]);
			}
			if((zeroStart + zero + one + zeroB == len)) {
				printf("BI (zero start) valid bit %s\n",arr);
				strcpy(biNumArr, argv[i]);
			}
			
		}
	} // end outer for loop

	printf("\nStrrrr %s, 0b %d, Neg %d, Hex %d, -b %d, -r %d, zero %d\n", biNumArr, zeroB, neg, hex, bFlag, rFlag, zero);

	sendStuff(biNumArr, rArr, neg, zeroB, hex, bFlag, rFlag);

	free(biNumArr);
	free(rArr);
	free(arr);
	

	return 0;
}


void sendStuff(char *biNumArr, char *rArr, int neg, int zeroB, int hex, int bFlag, int rFlag) {

	int i, j;
	int biLen = 0, rLen = 0, rSplit = 0;
	printf("\nFirst %p, %p\n",biNumArr, rArr);

	char *new_R_Arr1 = malloc(sizeof(char) * 100);
	char *finalIHope = malloc(sizeof(char) * 100);

	printf("\nSecond %p, %p\n",new_R_Arr1, finalIHope);

	printf("Zero B %d\n",zeroB);
	if(bFlag > 0) {
		biLen = strlen(biNumArr);
		if(zeroB > 0) {
			for(i = 2; i < biLen; i++) {
				printf("Blaaa %c\n",biNumArr[i]);
			}
		}
	}

	
	if(rFlag > 0) {
		rLen = strlen(rArr);
		printf("R Length %d\n",rLen);
		for(j = 0; j < rLen; j++) {
			if(rArr[j] == 44) {
				printf("\nChar: %c Int: %d, J: %d\n", rArr[j], rArr[j], j);
				rSplit = j;
			}
		}
		printf("Huhhh %s\n", new_R_Arr1);
		memcpy(new_R_Arr1, &rArr[0], rSplit);
		new_R_Arr1[rSplit] = '\0';
		printf("For real? %s\n", new_R_Arr1);
		int leftString = (rLen-1) - rSplit;
		printf("String Left %d\n",leftString);
		memcpy(finalIHope, &rArr[rSplit+1],leftString);
		printf("I hope this Works! %s\n", finalIHope);
	}
	
	printf("\nVALIDATE BiNum: %s, r Array: %s, Neg %d, Hex %d, -b %d, -r %d\n", biNumArr, rArr, neg, hex, bFlag, rFlag);

	free(new_R_Arr1);
	free(finalIHope);
}





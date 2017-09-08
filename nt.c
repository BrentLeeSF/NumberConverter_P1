

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

int checkRegNumSize(char *regNum);
void sendStuff(char *biNumArr, char *rArr, char *regNum, char *hexNum, int neg, int zeroB, int bitCount, int b, int r);
//void numToBinary(char *regNum, char *rArr, char *biNumArr);

int main(int argc, char *argv[]) {

	if(argc == 1 || argc == 0) {
		printf("\n** Please include numbers after the file name **\n\n");
		exit(1);
	}
	
	int zeroB = 0, bFlag = 0, bIndex = 0, i = 0, hex = 0, one = 0 , neg = 0, rFlag = 0, rIndex = 0, zero = 0, zeroStart = 0, num = 0;
	char arr[100];
	char biNumArr[100];
	char rArr[100];
	char regNum[100];
	char hexNum[100];
	//printf("Arr %p, Bi Num %p, R arr %p\n", arr, biNumArr, rArr);
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
			//arr[len];
			strcpy(arr, argv[i]);
			printf("Arg Value %s\n", arr);
			int j = 0;
			
			one = 0, neg = 0, zero = 0;

			// NUMBER AFTER -b
			if(i == bIndex+1 && bIndex > 0) {
				bitCount = atoi(argv[i]);
				//printf("\nBit Count: %d\n\n",bitCount);
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
				// NUMBERS 2-9
				if(arr[j] > 49 && arr[j] < 58) {
					num++;
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
						printf("2 INVALID NUMBER! %c\n", arr[j]);
						exit(1);
					}
					hex++;
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

			//printf("\n");
			
			//printf("Length: %d\n", len);
			//printf("ONE: %d\n",one);

			// BINARY only 1s and 0s
			if((zeroStart + zero + one == len) && (len%4 == 0)) {
				printf("valid bit %s\n",arr);
				strcpy(biNumArr, argv[i]);
			}
			// BINARY 0b start
			else if((zeroStart + zero + one + zeroB == len)) {
				printf("BI (zero start) valid bit %s\n",arr);
				strcpy(biNumArr, argv[i]);
			}
			// REGULAR POSITIVE NUMBER
			else if(zero + one + num == len) {
				printf("Regular Number %s\n",arr);
				strcpy(regNum, argv[i]);
			}
			// REGULAR NEGATIVE NUMBER
			else if(zero + one + num + neg == len) {
				printf("Regular NEGATIVE Number %s\n",arr);
				strcpy(regNum, argv[i]);
			}
			// HEX NUMBER
			else if(zeroStart + zero + one + hex == len) {
				printf("HEX Number %s\n",arr);
				strcpy(hexNum, argv[i]);
			}
		}
	} // end outer for loop

	printf("\nBinary Number %s, Regular Number: %s, Hex Num: %s, 0b %d, Neg %d, -b %d, Bit Count: %d, -r %d, R Numbers: %s, zero %d\n", biNumArr, regNum, hexNum, zeroB, neg, bFlag, bitCount, rFlag, rArr, zero);

	int returnedNum = checkRegNumSize(regNum);

	if(returnedNum < 0) {
		printf("This number is either larger than unsigned int from 0 to 4,294,967,295, or signed int of −2,147,483,648 to 2,147,483,648 %s\n", regNum);
		exit(1);
	}
	sendStuff(biNumArr, rArr, regNum, hexNum, neg, zeroB, bFlag, bitCount, rFlag);



	return 0;
}


void sendStuff(char *biNumArr, char *rArr, char *regNum, char *hexNum, int neg, int zeroB, int bFlag, int bitCount, int rFlag) {

	int i, j;
	int biLen = 0, rLen = 0, rSplit = 0;
	//printf("\nFirst %p, %p\n",biNumArr, rArr);

	char new_R_Arr1[100];
	char finalIHope[100];

	//printf("\nSecond %p, %p\n",new_R_Arr1, finalIHope);

	//printf("Zero B %d\n",zeroB);
	if(zeroB > 0) {
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
	
	printf("\nVALIDATE BiNum: %s, r Array: %s, Regular Number: %s, Hex Num: %s, Neg %d, -b %d, Bit Count: %d, -r %d\n", biNumArr, rArr, regNum, hexNum, neg, bFlag, bitCount, rFlag);

}


int checkRegNumSize(char *regNum) {
	int len = strlen(regNum);
	int i = 0, neg = 0;
	if(regNum[0] == 45) {
		printf("NEGATIVE: %s\n",regNum);

		return 0;
	}

	return -1;
}


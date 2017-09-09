#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 64
#define MAX_BIT_WIDTH 32
//#define MAX_VALUE 4294967295

/* The nt_info struct helps keep track of the conversion process and
   supports a normalize representation.
*/
struct ntInfo {
    char input[MAX_INPUT_LEN];
    int stringWidth;
    int r0;
    int r1;
    unsigned int value;
    int bitCount;
};

/* Initialize an nt_info struct to default and known initial values. */
void ntInfoInit(struct ntInfo *nti) {

    nti->input[0] = '\0';
    nti->stringWidth = MAX_BIT_WIDTH;
    nti->r0 = 0;
    nti->r1 = 0;
    nti->value = 0;
    nti->bitCount = 0;

    return;
}

/* Print an in_info struct for debugging purposes. */
void ntInfoPrint(struct ntInfo *nti) {
    printf("\ninput = %s\n", nti->input);
    printf("String Width = %d\n", nti->stringWidth);
    printf("r0    = %d\n", nti->r0);
    printf("r1    = %d\n", nti->r1);
    printf("value = %u\n", nti->value);
    printf("Bit Count = %u\n\n", nti->bitCount);
    return;
}

/* A helpful usage message. */
void printUsage(void) {
    printf("usage: nt [-b width] [-r start,end] value\n");
    printf("  width is 4, 8, 16, 32 (default)\n");
    printf("  start is 0 to 31, end is 0 to 31\n");
    printf("  value can be binary, hexadecimal, unsigned or signed decimal.\n");
    return;
}

int power(int x, int y) {
    if(y == 0) {
        return 1;
    }
    if(y == 1) {
        return x;
    }
    if(y%2 == 0) {
        return power(x*x, y/2);
    }
    else {
        return power(x*x, y/2) * x;
    }
}


void stringToDigit(char *argv, struct ntInfo *nti, int negFlag) {
    int i = 0, j = 0, sum = 0;
    int len = strlen(argv);
    unsigned int max = 4294967295;
    //printf("valid Num :%s, Length: %d\n", argv, len);

    if(len > 10) {
        printf("too long length\n");
        printUsage();
        exit(-1);
    }
    
    if(negFlag <= 0) {
        for(j = len-1; j >= 0; j--) {
           // printf("argv[j] %d\n", argv[j]);
            /* 48-57 = 0-9 */
            if(argv[j] < 48 || argv[j] > 57) {
                printf("2 too long length\n");
                printUsage();
                exit(1);
            }
            int num = argv[j] - 48;
            int ten = power(10, i);
            //printf("NUM %d, TEN %d\n", num, ten);
            sum += num*ten;
            //printf("Sum: %d\n",sum);
            i++;
        }
    } else if(negFlag > 0) {
        for(j = len-1; j > 0; j--) {
         //   printf("argv[j] %d\n", argv[j]);
            /* 48-57 = 0-9 */
            if(argv[j] < 48 || argv[j] > 57) {
                printf("2 too long length\n");
                printUsage();
                exit(1);
            }
            int num = argv[j] - 48;
            int ten = power(10, i);
           // printf("NUM %d, TEN %d\n", num, ten);
            sum += num*ten;
           // printf("Sum: %d\n",sum);
            i++;
        }
        //printf("1 SUM: %d\n",sum);
        sum = max - (sum-1);
        //printf("SUM %d\n",sum);
    }
    if(sum > max) {
        printf("SUM DONE: %d, Length: %d\n",sum, len);
        printf("TOO BIG SON\n");
        printUsage();
        exit(1);
    }
    nti->value = sum;
}

void splitR(char *argv, struct ntInfo *nti) {

    int rLen = strlen(argv);
    if(rLen > 5) {
        printf("1 INVALID NUMBER! %s\n", argv);
        exit(1);
    }

    int j = 0, rSplit = 0, i = 0;

    for(j = 0; j < rLen; j++) {
        if(argv[j] < 44 || (argv[j] > 44 && argv[j] < 48) || argv[j] > 57) {
            printf("2 INVALID NUMBER! %c\n", argv[j]);
            exit(1);
        }
        if(argv[j] == 44) {
            //printf("\nChar: %c Int: %d, J: %d\n", argv[j], argv[j], j);
            rSplit = j;
        }
    }
    char newRArr1[100];
    memcpy(newRArr1, &argv[0], rSplit);
    newRArr1[rSplit] = '\0';
    nti->r0 = atoi(newRArr1);
    int leftString = (rLen-1) - rSplit;
    char finalIHope[100];
    memcpy(finalIHope, &argv[rSplit+1],leftString);
    nti->r1 = atoi(finalIHope);
}

/* Convert a valid binary string (0b11010) into an unsigned int value. */
void normalizeBinary(char *argv, struct ntInfo *nti) {
    unsigned int v = 0;
    int i;
    char c;
    int b;
    int len = strlen(argv);

    for (i = 2; i < len; i++) {
        /* Left shift v by one to make room for next bit. */
        /* First time through, v == 0, so this does not change v. */
        v = v << 1;

        /* Get integer value of binary character. */
        c = argv[i];
        if (c == '0') {
            b = 0;
        } else if (c == '1') {
            b = 1;
        }
        /* Use logical or to place b in the least significant position of v. */
        v = v | b;
    }
    nti->value = v;
}

/* Validate a binary string value. */
void validateBinary(char *argv) {
    int i;
    bool valid = true;
    int len;
    char c;

    len = strlen(argv);

    /* The input must ahave at least one binary digit. E.g., 0b1. */
    /* Also, the number of binary digits cannot exceed the width. */
    if (len < 3 || len > MAX_BIT_WIDTH + 2) {
        valid = false;
    } else {
        for (i = 0; i < MAX_BIT_WIDTH; i++) {
            c = argv[i + 2];
            /* If at end of input, then break. */
            if (c == '\0') {
                break;
            }
            /* Check if each digit is a valid binary value. */
            if (!(c == '0' || c == '1')) {
                valid = false;
                break;
            }
            //printf("%c", c);
        }
    }
    if (!valid) {
        printf("Invalid binary input: %s\n", argv);
        printUsage();
        exit(-1);
    }
    return;
}

void printBinaryValueCLiteral(struct ntInfo *nti) {
    int i;
    int b;

    printf("0b");
    for (i = nti->stringWidth; i >= 0; i--) {
        
        if(i == nti->stringWidth) {
            b = (nti->value >> (i-1)) & 0b1;
            continue;
        } else {
            b = (nti->value >> i) & 0b1;
            /*if(i%4 == 3) {
                printf(" ");
            }*/
            printf("%d", b);
        }
    }
    printf(" (base 2)\n");
    return;
}

void printBinary(struct ntInfo *nti) {
    int i;
    int b;

    for (i = nti->stringWidth; i >= 0; i--) {
        
        if(i == nti->stringWidth) {
            b = (nti->value >> (i-1)) & 0b1;
            continue;
        } else {
            b = (nti->value >> i) & 0b1;
            if(i%4 == 3 && i < nti->stringWidth-3) {
                printf(" ");
            }
            printf("%d", b);
        }
    }
    printf(" (base 2)\n");
    return;
}

void printSignedAndUnsignedValue(struct ntInfo *nti) {
    printf("%u (base 10 unsigned)\n", nti->value);
    printf("%d (base 10 signed)\n", nti->value);
}



void stringToHex(char *argv, struct ntInfo *nti) {
    int len = strlen(argv);
    int i = 0, j = 0, num = 0, sum = 0;
    unsigned int max = 4294967295;

    for(i = len-1; i > 1; i--) {
        if(argv[i] < 48 || (argv[i] > 57 && argv[i] < 65) || argv[i] > 70) {
            printf("Insufficient arguments.\n");
            printUsage();
            exit(-1);
        }
        //printf("INSIDE HEX %c\n",argv[i]);
        int thisArg = 0;
        if(argv[i] > 47 && argv[i] < 58) {
            thisArg = argv[i] - 48;
        } else if(argv[i] > 64 && argv[i] < 71){
            thisArg = argv[i] - 55;
          //  printf("HUH... %d\n",thisArg);
        }
        num = power(16, j);
        sum += num*thisArg;
        j++;
    }
    if(sum < max) {
        nti->value = sum;
    } else {
        printf("Insufficient arguments.\n");
        printUsage();
        exit(-1);
    }
   // printf("HEX SUM: %d\n",sum);
}


/* Parse the command line arguments. */
/* NOTE: This will need to be modified to support -b and -r. */
void parseArgs(int argc, char **argv, struct ntInfo *nti) {

    if (argc < 2) {
        printf("Insufficient arguments.\n");
        printUsage();
        exit(-1);
    }

    int i = 0, j = 0, bitFlag = 0, binFlag = 0, rFlag = 0, negFlag = 0;

    for(i = 0; i < argc; i++) {

		if(i >= 1) {
			int len = strlen(argv[i]);
			
            // -b # OR -r ##,##
            if(bitFlag > 0 && i == bitFlag+1) {
                if(!(argv[i][0] > 47 && argv[i][0] < 58)) {
                    printf("Insufficient arguments.\n");
                    printUsage();
                    exit(-1);  
                }
                stringToDigit(argv[i], nti, 0);
                int b = atoi(argv[i]);
                nti->bitCount = b;
                continue;
            } else if(rFlag > 0 && i == rFlag+1) {
                if(!(argv[i][0] > 47 && argv[i][0] < 58)) {
                    printf("Insufficient arguments.\n");
                    printUsage();
                    exit(-1);  
                }
                splitR(argv[i], nti);
                continue;
            }

            // FLAGS -b -r
            if(len == 2) {
                if(argv[i][0] == 45) {
                    if(argv[i][1] == 98) {
                        bitFlag = i;
                        continue;
                    }
                    if(argv[i][1] == 114) {
                        rFlag = i;
                        continue;
                    }
                }
            }
            // IF STARTS WITH 0
            if(argv[i][0] == 48) {
                if(argv[i][1] == 98) {
                  //  printf("BINARY\n");
                    validateBinary(argv[i]);
                    normalizeBinary(argv[i], nti);
                    continue;
                } else if(argv[i][1] == 120) {
                    stringToHex(argv[i], nti);
                    continue;
                } else {
                   // printf("validate BINARY 1010101010\n");
                    continue;
                }  
            }
            // neg
            if(argv[i][0] == 45) {
                negFlag++;
                stringToDigit(argv[i], nti, negFlag);
              //  printf("Negative Number\n");
                //ntInfoPrint(nti);
            } else {
             //   printf("VALIDATE POS\n");
                stringToDigit(argv[i], nti, negFlag);
                
               // ntInfoPrint(nti);
            }
		}
	}
    /* Assume the input value is argv[1]. */
   // printf("ARG 1: %s\n", argv[1]);
    strcpy(nti->input, argv[1]);
    
    return;
}

void decimalToBin(struct ntInfo *nti) {
    int num = nti->value;
   // printf("Decimal Value for Binary Conversion: %d ",num);

    int rem = 0;
    while(num != 0) {

        if(num%2 == 0) {
            rem = 0;
        } else {
            rem = 1;
        }
        num = num/2;
        printf("%d",rem);
    }
    printf("\n");
}

void decimalToHex(struct ntInfo *nti) {
    
    unsigned int num = nti->value;
    printf("DEC TO HEX: %u\n",num);
    int rem = 0, index = 7, letter = 0;
    int arr[10];
    while(num != 0) {
        rem = num%16;
        if(rem > 9) {
            letter = rem - 9;
            printf("%c",letter+64);
        } else {
            printf("%d",rem);
        }
        num = num/16;
    }
    printf("\n");
}

void printConversions(struct ntInfo *nti) {
    printBinary(nti);
    printBinaryValueCLiteral(nti);
    decimalToHex(nti);
    printSignedAndUnsignedValue(nti);
    return;
}

int main(int argc, char **argv) {

    struct ntInfo nti;
    ntInfoInit(&nti);

    parseArgs(argc, argv, &nti);

    printConversions(&nti);


    return 0;
}


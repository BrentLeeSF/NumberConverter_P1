#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 64
#define MAX_BIT_WIDTH 32

/* 
1. If negative entry, send to 2s compliment to invert and 
print big number on unsigned. Signed shows negative
2. If negative entry and -b flag, invert it all, print out 
total of binary showing as unsigned. Signed shows orig neg entry.
3. If positie entry and first bit (1010) is a one, then invert 
0101, add a 1, and make negative = 0110 = -6. 
Or 1110 0000 -> 0001 1111 + 1 -> 0010 0000 = -32
*/

/* The nt_info struct helps keep track of the conversion process and
   supports a normalize representation. */
struct ntInfo {
    char input[MAX_INPUT_LEN];
    int stringWidth;
    int r0;
    int r1;
    unsigned int value;
    int bitCount;
    unsigned int newUnsignedValue;
    int newSignedValue;

};

/* Initialize an nt_info struct to default and known initial values. */
void ntInfoInit(struct ntInfo *nti) {
    nti->input[0] = '\0';
    nti->stringWidth = MAX_BIT_WIDTH;
    nti->r0 = 0;
    nti->r1 = 0;
    nti->value = 0;
    nti->bitCount = 0;
    nti->newUnsignedValue = 0;
    nti->newSignedValue = 0;
    return;
}

/* Print an in_info struct for debugging purposes. */
void ntInfoPrint(struct ntInfo *nti) {
    printf("\ninput = %s\n", nti->input);
    printf("String Width = %d\n", nti->stringWidth);
    printf("r0    = %d\n", nti->r0);
    printf("r1    = %d\n", nti->r1);
    printf("value = %u\n", nti->value);
    printf("Bit Count = %u\n", nti->bitCount);
    printf("New Value = %u\n", nti->newUnsignedValue);
    printf("New Value = %d\n", nti->newSignedValue);
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

/* Used get the powers of 2^n, 10^n, and 16^n */
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
    int i = 0, j = 0;
    int len = strlen(argv);
    unsigned int max = 4294967295;
    unsigned int sum = 0;

    if(len > 10) {
        printf("too long length\n");
        printUsage();
        exit(-1);
    }
    
    if(negFlag <= 0) {

        for(j = len-1; j >= 0; j--) {
            /* 48-57 = 0-9 */
            if(argv[j] < 48 || argv[j] > 57) {
                printf("2 too long length\n");
                printUsage();
                exit(1);
            }
            int num = argv[j] - 48;
            int ten = power(10, i);
            sum += num*ten;
            i++;
        }
    } 
    else if(negFlag > 0) {
        for(j = len-1; j > 0; j--) {
            /* 48-57 = 0-9 */
            if(argv[j] < 48 || argv[j] > 57) {
                printUsage();
                exit(1);
            }
            int num = argv[j] - 48;
            int ten = power(10, i);
            sum += num*ten;
            i++;
        }
        sum = max - (sum-1);
    }
    if(sum > max) {
        printUsage();
        exit(1);
    }
    nti->value = sum;
    printf("Value %u\n",nti->value);
}

void splitR(char *argv, struct ntInfo *nti) {

    int rLen = strlen(argv);
    if(rLen > 5) {
        printUsage();
        exit(1);
    }

    int j = 0, rSplit = 0, i = 0;

    for(j = 0; j < rLen; j++) {
        if(argv[j] < 44 || (argv[j] > 44 && argv[j] < 48) || argv[j] > 57) {
            printUsage();
            exit(1);
        }
        if(argv[j] == 44) {
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

    int i = 0, j = 0;
    int b;
    
    int bitLength = nti->bitCount;
    int rStart = nti->r0;
    int rEnd = nti->r1;
    //printf("Start: %d, End: %d\n", rStart, rEnd);
    unsigned int thisValue = 0;
    printf("0b");

    /*if(rStart > 0 && bitLength < 1) {
        printf("inside\n");
        for (i = rStart; i >= rEnd; i--) {
            if(i == rStart) {
                b = (nti->value >> (i-1)) & 0b1;
                continue;
            } else {
                b = (nti->value >> i) & 0b1;
                printf("%d\n", b);
            }
        }
    }*/

    if(bitLength > 0) {
        for (i = bitLength; i >= 0; i--) {
            if(i == bitLength) {
                b = (nti->value >> (i-1)) & 0b1;
                continue;
            } else {
                b = (nti->value >> i) & 0b1;
                printf("%d", b);
            }
        }
    } else {
        for (i = nti->stringWidth; i >= 0; i--) {
            if(i == nti->stringWidth) {
                b = (nti->value >> (i-1)) & 0b1;
                continue;
            } else {
                b = (nti->value >> i) & 0b1;
                printf("%d", b);
            }
        }
    }
    printf(" (base 2)\n");
    return;
}

void twosCompliment(struct ntInfo *nti) {

    int num = nti->newSignedValue;
    int index = 0;
    char thisBin[32];
    int thisDecimal = num;
    //printf("NUM: %d\n",num);

    while(thisDecimal != 0) {
        if((thisDecimal%2) == 0) {
            thisBin[index] = '1';
        } else {
            thisBin[index] = '0';
        }
        thisDecimal /= 2;
        index++;
    }
    thisBin[index] = '\0';
    //printf("I hope this works %s\n",thisBin);

    int j = 0, carry = 1;
    int thisCharLength = strlen(thisBin);
    char finalDecimal[32];
    int finalIndex = 0;

    for(j = 0; j < thisCharLength; j++) {
        if(thisBin[j] == 49 && carry == 1) {
            finalDecimal[j] = '0';
        } else if(thisBin[j] == 48 && carry == 1) {
            finalDecimal[j] = '1';
            carry = 0;
        } else {
            finalDecimal[j] = thisBin[j];
        }
    }
    finalDecimal[thisCharLength] = '\0';
    //printf("\nI hope this works 2 %s\n",finalDecimal);

    int b = 0, a = 0, thisNum = 0, finalSigned = 0;
    int k = thisCharLength-1;
    for(a = thisCharLength-1; a >= 0; a--) {
        b = (int)finalDecimal[k] - 48;
        thisNum = power(2, k);
        finalSigned += thisNum*b;
        //printf("inal Signed %d\n",finalSigned);
        k--;
    }
    nti->newSignedValue = finalSigned*-1;
    printf("FINAL SIGNED %d\n", nti->newSignedValue);
}

void printBinary(struct ntInfo *nti) {
    int fullLength = nti->stringWidth-1;
    int i = 0, num = 0, neg = 0;
    int b;
    int bitLength = nti->bitCount;
    int thisPower = bitLength-1;
    int rEnd = nti->r0;
    int rStart = nti->r1;
    int k = 0;
    unsigned int unsignedValue = 0;
    int signedValue = 0;
    printf("Rend %d, rStart %d, Bits %d, Value: %u\n",rEnd, rStart, bitLength, nti->value);
    
    // -r, no -b
    if(rEnd >0 && bitLength == 0) {
        k = rStart - rEnd;
        for(i = rStart; i >= rEnd; i--) {
            b = (nti->value >> i) & 0b1;
            if(i%4 == 3 && i < rEnd-3) {
                printf(" ");
            }
            printf("%d", b);
            num = power(2, k);
            unsignedValue += num*b;
            signedValue += num*b;
            k--;
            if(b == 1 && i == rStart) {
                neg++;
            }
        }
        nti->newUnsignedValue = unsignedValue;
        nti->newSignedValue = signedValue;
        printf("\n");
        if(neg > 0) {
            twosCompliment(nti);
        }

        // -b no -r
    } else if(bitLength > 0 && rStart == 0) {
        int j = bitLength-1;
        for (i = bitLength; i >= 0; i--) {
            if(i == bitLength) {
                b = (nti->value >> (i-1)) & 0b1;
                continue;
            } else {
                b = (nti->value >> i) & 0b1;
                if(i%4 == 3 && i < bitLength-3) {
                    printf(" ");
                }
                printf("%d", b);
                num = power(2, j);
                unsignedValue += num*b;
                signedValue+= num*b;
                j--;
                if(b == 1 && i == bitLength-1) {
                    neg++;
                }
            }
        }
        printf("\n");

        // -b and -r
    } else if(bitLength > 0 && rStart > 0) {
        
        int smallest = 0;
        int range = rStart - rEnd;
        if(range < bitLength) {
            smallest = range;
        } else { 
            smallest = bitLength;
        }
        int a = smallest;
        for(i = smallest+rEnd; i >= rEnd; i--) {
            b = (nti->value >> i) & 0b1;
            if(i%4 == 3 && i < rEnd-3) {
                printf(" ");
            }
            printf("%d", b);
            num = power(2, a);
            unsignedValue += num*b;
            signedValue += num*b;
            a--;
            if(b == 1 && i == rStart) {
                neg++;
            }
        }
        printf("\n");
    } else {
        int d = nti->stringWidth-1;
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
                num = power(2, d);
                unsignedValue += num*b;
                signedValue += num*b;
                d--;
            }
            if(b == 1 && i == nti->stringWidth-1) {
                neg++;
            }
        }
        printf("\n");
    }
    printf("Signed: %d, Unsigned: %u, neg: %d\n",signedValue,unsignedValue, neg);
    /*

        for (i = bitLength; i >= 0; i--) {
            //printf("INSIDE, I%d\n", i);
            if(rStart > 0) {
                if(rStart <= i || rEnd >= i) {
                    if(i == rStart) {
                        b = (nti->value >> (i-1)) & 0b1;
                        //printf(" Start %d\n",b);
                        continue;
                    } else {
                        b = (nti->value >> i) & 0b1;
                        if(i%4 == 3 && i < rStart-3) {
                            printf(" ");
                        }
                        printf("%d", b);
                        num = power(2, k);
                        thisValue += num*b;
                        signedValue += num*b;
                        k--;
                    }
                }
            } else {
                if(i == bitLength) {
                    b = (nti->value >> (i-1)) & 0b1;
                    continue;
                } else {
                    b = (nti->value >> i) & 0b1;
                    if(i%4 == 3 && i < bitLength-3) {
                        printf(" ");
                    }
                    printf("%d", b);
                    num = power(2, j);
                    thisValue += num*b;
                    signedValue+= num*b;
                    j--;
                }
            }
            
        }*/
    
    /*else {
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
    }*/
    printf(" (base 2)\n");
    //printf("New UNSIGNED Value: %u, New Signed Value: %d\n",thisValue, signedValue);
    //nti->newValue = thisValue;
    
    return;
}

void printSignedAndUnsignedValue(struct ntInfo *nti) {
    int bitLength = nti->bitCount;
    if(bitLength > 0) {
       // printf("%u (base 10 unsigned)\n", nti->newValue);
        printf("%d (base 10 signed)\n", nti->value);
    } else {
        printf("%u (base 10 unsigned)\n", nti->value);
        printf("%d (base 10 unsigned)\n", nti->value);
    }

    
    //printf("%d (base 10 signed)\n", nti->value);
}



void hexStringToDigit(char *argv, struct ntInfo *nti) {
    int len = strlen(argv);
    int i = 0, j = 0, num = 0, sum = 0;
    unsigned int max = 4294967295;


    for(i = len-1; i > 1; i--) {
        if(argv[i] < 48 || (argv[i] > 57 && argv[i] < 65) || argv[i] > 70) {
            printf("Insufficient arguments.\n");
            printUsage();
            exit(-1);
        }

        int thisArg = 0;

        if(argv[i] > 47 && argv[i] < 58) {
            thisArg = argv[i] - 48;
        } else if(argv[i] > 64 && argv[i] < 71){
            thisArg = argv[i] - 55;
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
                    validateBinary(argv[i]);
                    normalizeBinary(argv[i], nti);
                    continue;
                } else if(argv[i][1] == 120) {
                    hexStringToDigit(argv[i], nti);
                    continue;
                }  
            }
            // neg
            if(argv[i][0] == 45) {
                negFlag++;
                stringToDigit(argv[i], nti, negFlag);
            } else {
                stringToDigit(argv[i], nti, negFlag);
            }
		}
	}
    return;
}


void decimalToBin(struct ntInfo *nti) {

    unsigned int num = nti->value;
    int rEnd = nti->r0;
    int rStart = nti->r1;
    int bitLength = nti->bitCount;
    printf("NUM: %u\n",num);
    int i = 0, j = 0;
    char rem = 0;
    char arr[64];

    while(num != 0) {
        if(num%2 == 0) {
            rem = '0';
        } else {
            rem = '1';
        }
        num = num/2;
        arr[i] = rem;
        i++;
    }
    //printf("\n");
    int len = strlen(arr);
    char newNum[64];
    printf("len: %d\n", len);

    for(j = len; j >= 0; j--) {
        if(rEnd > 0 && bitLength == 0) {

        }
        printf("%c", arr[j]);
    }
    printf("\n");
}

void decimalToHex(struct ntInfo *nti) {
    
    unsigned int num = nti->value;
    int rem = 0, index = 7, letter = 0, i = 0, j = 0;
    char arr[16];

    while(num != 0) {
        rem = num%16;
        if(rem > 9) {
            letter = rem - 9;
            arr[i] = letter+64;
            //printf("%c", arr[i]);
        } else {
            arr[i] = rem+48;
           // printf("%c", arr[i]);
        }
        i++;
        num = num/16;
    }
    //printf("\n");

    int bitLength = nti->bitCount;
    if(bitLength > 0) {
        bitLength /= 4;
    } else {
        bitLength = 8;
    }

    int zeros = 0;
    zeros = bitLength - i;
    printf("0x");
    for(j = 0; j < zeros; j++) {
        printf("0");
    }

    for(j = i-1; j >= 0; j--) {
        if(bitLength-1 < j){
            continue;
        } else {
            printf("%c", arr[j]);
        }
    }
    printf(" (base 16) \n");
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


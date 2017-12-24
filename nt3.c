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

/* Takes each number arg, struct, neg flag, and index of bit number
Subtracts 48 from char to get number, uses power to get number representation, 
Determines if it is not a -b number, and uses it as number to be converted.
If too big or too small, prints error message. */
void stringToDigit(char *argv, struct ntInfo *nti, int negFlag, int bitIndex) {

    int i = 0, j = 0, num = 0, ten = 0, end = 0;
    int len = strlen(argv);
    unsigned int max = 4294967295;
    unsigned int min = 2147483648;
    unsigned int sum = 0;
  
    if((len > 10 && negFlag == 0) || (len > 11 && negFlag > 0)) {
        printf("Too long\n");
        printUsage();
        exit(-1);
    }

    if(negFlag > 0) {
        end = 1;
    }

    for(j = len-1; j >= end; j--) {
        /* 48-57 = 0-9 */
        if(argv[j] < 48 || argv[j] > 57) {
            printf("Too long\n");
            printUsage();
            exit(1);
        }
        num = argv[j] - 48;
        ten = power(10, i);
        sum += num*ten;
        i++;
        if(end == 0 && i == 10 && sum < 999999998) {
            printf("Too long\n");
            printUsage();
            exit(1);
        }
    }
    
    if(negFlag > 0 && end == 1) {
        sum = max - (sum-1);
    }

    if(sum > max) {
        printUsage();
        exit(1);
    } else if(negFlag > 0 && sum < min) {
        printUsage();
        exit(1);
    }
    if(bitIndex <= 0) {
        nti->value = sum;
    }
}

/* Takes number after -r as char, determines if valid number, and where comma is
Creates two new char arrays for both range numbers, 
copies from argv[0] until comma into new char array and adds end string and saves number,
does same thing with number after comma, saves as r1 (termination)
*/
void splitR(char *argv, struct ntInfo *nti) {

    int rLen = strlen(argv);
    char newRArr1[100];
    char newRArr2[100];
    int leftString = 0, j = 0, rSplit = 0, i = 0;

    if(rLen > 5) {
        printUsage();
        exit(1);
    }

    for(j = 0; j < rLen; j++) {
        if(argv[j] < 44 || (argv[j] > 44 && argv[j] < 48) || argv[j] > 57) {
            printUsage();
            exit(1);
        }
        if(argv[j] == 44) {
            rSplit = j;
        }
    }
    
    memcpy(newRArr1, &argv[0], rSplit);
    newRArr1[rSplit] = '\0';

    nti->r0 = atoi(newRArr1);

    leftString = (rLen-1) - rSplit;
    memcpy(newRArr2, &argv[rSplit+1],leftString);

    nti->r1 = atoi(newRArr2);
}

/* Convert a valid binary string (0b11010) into an unsigned int value. */
void normalizeBinary(char *argv, struct ntInfo *nti) {

    unsigned int v = 0;
    int i = 0, b = 0;
    char c;
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

    int i = 0;
    bool valid = true;
    int len = strlen(argv);
    char c;


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

void changeStringLength(struct ntInfo *nti) {

    // no -r no -b
    if(nti->r1 <= 0 && nti->bitCount <= 0) {
        nti->r1 = 32;
        nti->r0 = 0;
        nti->stringWidth = 32;
        nti->bitCount = 32;

        // -b no -r
    } else if(nti->bitCount > 0 && nti->r1 <= 0) {
        nti->r1 = nti->bitCount;
        nti->r0 = 0;
        nti->stringWidth = nti->bitCount;

        // -r no -b
    } else if(nti->r1 > 0 && nti->bitCount <= 0) {
        nti->stringWidth = ((nti->r1) +1) - nti->r0;
        nti->bitCount = ((nti->r1) +1) - nti->r0;
        nti->r1 = nti->r1 +1;

        // -b AND -r
    } else if(nti->bitCount > 0 && nti->r1 > 0) {
        nti->stringWidth = nti->bitCount;
        nti->r1 = nti->r1 + 1;
    }
}

/* starts at end of string-1, prints out until beginning of string */
void printBinaryValueCLiteral(struct ntInfo *nti) {

    int i = 0, b = 0;

    printf("0b");

     for (i = (nti->r1 - 1); i >= nti->r0; i--) {
        b = (nti->value >> i) & 0b1;
        printf("%d", b);
    }
    printf(" (base 2)\n");
    return;
}

/* If a binary number starts with a 1, it is a negative number.
It must be converted into negative */
void twosCompliment(struct ntInfo *nti) {

    // loop 1
    int num = nti->newSignedValue;
    int index = 0, j = 0, carry = 1;
    char thisBin[32];
    int thisDecimal = num;

    // loop 2
    int thisCharLength = 0, finalIndex = 0; 
    char finalDecimal[32];

    // loop 3
    int b = 0, a = 0, thisNum = 0, finalSigned = 0, k = 0;


    /* takes number (that is positive), gets binary number, 
    and flips binary number */
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


    thisCharLength = strlen(thisBin);
    k = thisCharLength-1;

    /* Goes through binary array, and adds one to it */
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


    /* Goes though binary in reverse (starting from left), gets the
    binary number, and multiplies by the power to get digit representation.
    When it has */
    for(a = thisCharLength-1; a >= 0; a--) {
        b = (int)finalDecimal[k] - 48;
        thisNum = power(2, k);
        finalSigned += thisNum*b;
        k--;
    }
    nti->newSignedValue = finalSigned*-1;

}


/* Starts at end number, gets binary number, multiplies by end-1 to 2^, 
adds those numbers and assigns as new value. Also, checks if first number 
(going backwards) is a 1. If so, it is negative, so once converted, send to
twosCompliment to get inverse and add one for negative number */
void printBinary(struct ntInfo *nti) {
    
    int i = 0, num = 0, neg = 0, b = 0;
    unsigned int unsignedValue = 0;
    int signedValue = 0;
    int a = nti->bitCount-1;

    for(i = nti->r1-1; i >= nti->r0; i--) {

        b = (nti->value >> i) & 0b1;

        if(i%4 == 3 && i < nti->r1-1) {
            printf(" ");
        }

        printf("%d", b);
        num = power(2, a);

        unsignedValue += num*b;
        signedValue += num*b;
        a--;

        if(b == 1 && i == nti->r1-1 && nti->r1 < 32) {
            neg++;
        }
    }
    nti->newUnsignedValue = unsignedValue;
    nti->newSignedValue = signedValue;

    if(neg > 0) {
        twosCompliment(nti);
    }

    printf(" (base 2)\n");

    return;
}

/* Prints out new signed and unsigned numbers */
void printSignedAndUnsignedValue(struct ntInfo *nti) {
    
    if(nti->newUnsignedValue > 0 || nti->bitCount > 0 || nti->r0 > 0) {
        printf("%u (base 10 unsigned)\n", nti->newUnsignedValue);
    } else {
        printf("%u (base 10 unsigned)\n", nti->value);
    }
    if(nti->newSignedValue != 0 || nti->bitCount > 0 || nti->r0 > 0) {
        printf("%d (base 10 signed)\n", nti->newSignedValue);
    } else {
         printf("%d (base 10 signed)\n", nti->value);
    }  

}


/* Determines ifhex is valid, */
void hexStringToDigit(char *argv, struct ntInfo *nti) {

    int len = strlen(argv);
    int i = 0, j = 0, num = 0, sum = 0, thisArg = 0;
    unsigned int max = 4294967295;

    if(len > 10) {
        printf("Insufficient arguments.\n");
        printUsage();
        exit(-1);
    }

    for(i = len-1; i > 1; i--) {

        if(argv[i] < 48 || (argv[i] > 57 && argv[i] < 65) || argv[i] > 70) {
            printf("Insufficient arguments.\n");
            printUsage();
            exit(-1);
        }

        /* Goes through hex and gets value. Number -48 (0), or
        65 (A) - 55 = 10 */
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

/* takes entire line from terminal. Goes through each arg, 
determines if -b, -r, # after -b, # after -r, hex, binary, pos number,
neg number, and passes it to appropriate function to validate 
what it claims to be. Or, can determine if it's invalid, and prints 
error message */
void parseArgs(int argc, char **argv, struct ntInfo *nti) {

    int i = 0, j = 0, b = 0, len = 0, bitFlag = 0, binFlag = 0, bitIndex = 0, rFlag = 0, negFlag = 0;

    if (argc < 2) {
        printf("Insufficient arguments.\n");
        printUsage();
        exit(-1);
    }

    for(i = 0; i < argc; i++) {

		if(i >= 1) {
			len = strlen(argv[i]);
			
            // NUMBER after -b OR -r Flag
            if(bitFlag > 0 && i == bitFlag+1) {
                if(!(argv[i][0] > 47 && argv[i][0] < 58)) {
                    printf("Insufficient arguments.\n");
                    printUsage();
                    exit(-1);  
                }
                bitIndex = i;
                stringToDigit(argv[i], nti, 0, bitIndex);
                b = atoi(argv[i]);
                nti->bitCount = b;
                bitIndex = 0;
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
                stringToDigit(argv[i], nti, negFlag, bitIndex);
            } else {
                stringToDigit(argv[i], nti, negFlag, bitIndex);
            }
		}
	}

    return;
}

/* Gets either new unsigned or regular unsigned. Continually divides
number by 16 and uses remaineder as hex */
void decimalToHex(struct ntInfo *nti) {
    
    unsigned int num = 0; 
    int rem = 0, index = 7, letter = 0, i = 0, j = 0, zeros = 0;
    int bitLength = nti->bitCount;
    char arr[16];

    if(nti->newUnsignedValue > 0) {
        num = nti->newUnsignedValue;
    } else {
        num = nti->value;
    }

    while(num != 0) {
        rem = num%16;
        if(rem > 9) {
            letter = rem - 9;
            arr[i] = letter+64;
        } else {
            arr[i] = rem+48;
        }
        i++;
        num = num/16;
    }

    if(bitLength > 0) {
        bitLength /= 4;
    } else {
        bitLength = 8;
    }

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
    changeStringLength(&nti);
    printConversions(&nti);

    return 0;
}

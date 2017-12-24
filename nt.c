#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 64
#define MAX_BIT_WIDTH 32

/* 1. If negative entry, send to 2s compliment to invert and 
print big number on unsigned. Signed shows negative
2. If negative entry and -b flag, invert it all, print out 
total of binary showing as unsigned. Signed shows orig neg entry.
3. If positie entry and first bit (1010) is a one, then invert 
0101, add a 1, and make negative = 0110 = -6. 
Or 1110 0000 -> 0001 1111 + 1 -> 0010 0000 = -32 */

/* The nt_info struct helps keep track of the conversion process and
   supports a normalize representation. */
struct nt_info {
    char input[MAX_INPUT_LEN];
    int string_width;
    int r0;
    int r1;
    unsigned int value;
    int bit_count;
    unsigned int new_unsigned_value;
    int new_signed_value;
};

/* Initialize an nt_info struct to default and known initial values. */
void nt_info_init(struct nt_info *nti) {
    nti->input[0] = '\0';
    nti->string_width = MAX_BIT_WIDTH;
    nti->r0 = 0;
    nti->r1 = 0;
    nti->value = 0;
    nti->bit_count = 0;
    nti->new_unsigned_value = 0;
    nti->new_signed_value = 0;
    return;
}

/* Print an in_info struct for debugging purposes. */
void nt_info_print(struct nt_info *nti) {
    printf("\ninput = %s\n", nti->input);
    printf("String Width = %d\n", nti->string_width);
    printf("r0    = %d\n", nti->r0);
    printf("r1    = %d\n", nti->r1);
    printf("value = %u\n", nti->value);
    printf("Bit Count = %u\n", nti->bit_count);
    printf("New Value = %u\n", nti->new_unsigned_value);
    printf("New Value = %d\n", nti->new_signed_value);
    return;
}

/* A helpful usage message. */
void print_usage(void) {
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
    } else {
        return power(x*x, y/2) * x;
    }

}

/* Takes each number arg, struct, neg flag, and index of bit number
Subtracts 48 from char to get number, uses power to get number representation, 
Determines if it is not a -b number, and uses it as number to be converted.
If too big or too small, prints error message. */
void string_to_digit(char *argv, struct nt_info *nti, int neg_flag, int bit_index) {

    int i = 0, j = 0, num = 0, ten = 0, end = 0;
    int len = strlen(argv);
    unsigned int max = 4294967295;
    unsigned int min = 2147483648;
    unsigned int sum = 0;
  
    if((len > 10 && neg_flag == 0) || (len > 11 && neg_flag > 0)) {
        printf("Too long\n");
        print_usage();
        exit(-1);
    }

    if(neg_flag > 0) {
        end = 1;
    }

    for(j = len-1; j >= end; j--) {

        /* 48-57 = 0-9 */
        if(argv[j] < 48 || argv[j] > 57) {
            printf("Too long\n");
            print_usage();
            exit(1);
        }

        num = argv[j] - 48;
        ten = power(10, i);
        sum += num*ten;
        i++;

        if(end == 0 && i == 10 && sum < 999999998) {
            printf("Too long\n");
            print_usage();
            exit(1);
        }

    }
    
    if(neg_flag > 0 && end == 1) {
        sum = max - (sum-1);
    }

    if(sum > max) {
        print_usage();
        exit(1);
    } else if(neg_flag > 0 && sum < min) {
        print_usage();
        exit(1);
    }

    if(bit_index <= 0) {
        nti->value = sum;
    }

}

/* Takes number after -r as char, determines if valid number, and where comma is
Creates two new char arrays for both range numbers, 
copies from argv[0] until comma into new char array and adds end string and saves number,
does same thing with number after comma, saves as r1 (termination) */
void split_r(char *argv, struct nt_info *nti) {

    int r_len = strlen(argv);
    char new_r_arr1[100];
    char new_r_arr2[100];
    int left_string = 0, j = 0, r_split = 0, i = 0;

    if(r_len > 5) {
        print_usage();
        exit(1);
    }

    for(j = 0; j < r_len; j++) {

        if(argv[j] < 44 || (argv[j] > 44 && argv[j] < 48) || argv[j] > 57) {
            print_usage();
            exit(1);
        }

        if(argv[j] == 44) {
            r_split = j;
        }

    }
    
    memcpy(new_r_arr1, &argv[0], r_split);
    new_r_arr1[r_split] = '\0';
    nti->r0 = atoi(new_r_arr1);

    left_string = (r_len-1) - r_split;
    memcpy(new_r_arr2, &argv[r_split+1],left_string);
    nti->r1 = atoi(new_r_arr2);

}

/* Convert a valid binary string (0b11010) into an unsigned int value. */
void normalize_binary(char *argv, struct nt_info *nti) {

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
void validate_binary(char *argv) {

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
        print_usage();
        exit(-1);
    }

    return;
}

void change_string_length(struct nt_info *nti) {

    if(nti->r1 <= 0 && nti->bit_count <= 0) {
        /* no -r no -b */
        nti->r1 = 32;
        nti->r0 = 0;
        nti->string_width = 32;
        nti->bit_count = 32;

    } else if(nti->bit_count > 0 && nti->r1 <= 0) {
        /* -b no -r */
        nti->r1 = nti->bit_count;
        nti->r0 = 0;
        nti->string_width = nti->bit_count;

    } else if(nti->r1 > 0 && nti->bit_count <= 0) {
        /* -r no -b */
        nti->string_width = ((nti->r1) +1) - nti->r0;
        nti->bit_count = ((nti->r1) +1) - nti->r0;
        nti->r1 = nti->r1 +1;

    } else if(nti->bit_count > 0 && nti->r1 > 0) {
        /* -b AND -r */
        nti->string_width = nti->bit_count;
        nti->r1 = nti->r1 + 1;
    }

}

/* starts at end of string-1, prints out until beginning of string */
void print_binary_value_c_literal(struct nt_info *nti) {

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
void twos_compliment(struct nt_info *nti) {

    /* loop 1 */
    int num = nti->new_signed_value;
    int index = 0, j = 0, carry = 1;
    char this_bin[32];
    int this_decimal = num;

    /* loop 2 */
    int this_char_length = 0, final_index = 0; 
    char final_decimal[32];

    /* loop 3 */
    int b = 0, a = 0, this_num = 0, final_signed = 0, k = 0;

    /* takes number (that is positive), gets binary number, 
    and flips binary number */
    while(this_decimal != 0) {

        if((this_decimal%2) == 0) {
            this_bin[index] = '1';
        } else {
            this_bin[index] = '0';
        }

        this_decimal /= 2;
        index++;

    }

    this_bin[index] = '\0';

    this_char_length = strlen(this_bin);
    k = this_char_length-1;

    /* Goes through binary array, and adds one to it */
    for(j = 0; j < this_char_length; j++) {

        if(this_bin[j] == 49 && carry == 1) {
            final_decimal[j] = '0';
        } else if(this_bin[j] == 48 && carry == 1) {
            final_decimal[j] = '1';
            carry = 0;
        } else {
            final_decimal[j] = this_bin[j];
        }

    }

    final_decimal[this_char_length] = '\0';

    /* Goes though binary in reverse (starting from left), gets the
    binary number, and multiplies by the power to get digit representation.
    When it has */
    for(a = this_char_length-1; a >= 0; a--) {

        b = (int)final_decimal[k] - 48;
        this_num = power(2, k);
        final_signed += this_num*b;
        k--;

    }

    nti->new_signed_value = final_signed*-1;

}


/* Starts at end number, gets binary number, multiplies by end-1 to 2^, 
adds those numbers and assigns as new value. Also, checks if first number 
(going backwards) is a 1. If so, it is negative, so once converted, send to
twosCompliment to get inverse and add one for negative number */
void print_binary(struct nt_info *nti) {
    
    int i = 0, num = 0, neg = 0, b = 0;
    unsigned int unsigned_value = 0;
    int signed_value = 0;
    int a = nti->bit_count-1;

    for(i = nti->r1-1; i >= nti->r0; i--) {

        b = (nti->value >> i) & 0b1;

        if(i%4 == 3 && i < nti->r1-1) {
            printf(" ");
        }

        printf("%d", b);
        num = power(2, a);
        unsigned_value += num*b;
        signed_value += num*b;
        a--;

        if(b == 1 && i == nti->r1-1 && nti->r1 < 32) {
            neg++;
        }

    }

    nti->new_unsigned_value = unsigned_value;
    nti->new_signed_value = signed_value;

    if(neg > 0) {
        twos_compliment(nti);
    }

    printf(" (base 2)\n");

    return;
}

/* Prints out new signed and unsigned numbers */
void print_signed_and_unsigned_value(struct nt_info *nti) {
    
    if(nti->new_unsigned_value > 0 || nti->bit_count > 0 || nti->r0 > 0) {
        printf("%u (base 10 unsigned)\n", nti->new_unsigned_value);
    } else {
        printf("%u (base 10 unsigned)\n", nti->value);
    }
    if(nti->new_signed_value != 0 || nti->bit_count > 0 || nti->r0 > 0) {
        printf("%d (base 10 signed)\n", nti->new_signed_value);
    } else {
         printf("%d (base 10 signed)\n", nti->value);
    }  

}


/* Determines ifhex is valid, */
void hex_string_to_digit(char *argv, struct nt_info *nti) {

    int len = strlen(argv);
    int i = 0, j = 0, num = 0, sum = 0, this_arg = 0;
    unsigned int max = 4294967295;

    if(len > 10) {
        printf("Insufficient arguments.\n");
        print_usage();
        exit(-1);
    }

    for(i = len-1; i > 1; i--) {

        if(argv[i] < 48 || (argv[i] > 57 && argv[i] < 65) || argv[i] > 70) {
            printf("Insufficient arguments.\n");
            print_usage();
            exit(-1);
        }

        /* Goes through hex and gets value. Number -48 (0), or
        65 (A) - 55 = 10 */
        if(argv[i] > 47 && argv[i] < 58) {
            this_arg = argv[i] - 48;
        } else if(argv[i] > 64 && argv[i] < 71){
            this_arg = argv[i] - 55;
        }

        num = power(16, j);
        sum += num*this_arg;
        j++;

    }

    if(sum < max) {
        nti->value = sum;
    } else {
        printf("Insufficient arguments.\n");
        print_usage();
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
void parse_args(int argc, char **argv, struct nt_info *nti) {

    int i = 0, j = 0, b = 0, len = 0, bit_flag = 0, bin_flag = 0, bit_index = 0, r_flag = 0, neg_flag = 0;

    if (argc < 2) {
        printf("Insufficient arguments.\n");
        print_usage();
        exit(-1);
    }

    for(i = 0; i < argc; i++) {

        if(i >= 1) {

            len = strlen(argv[i]);
            
            if(bit_flag > 0 && i == bit_flag+1) {

                /* NUMBER AFTER -b Flag */
                if(!(argv[i][0] > 47 && argv[i][0] < 58)) {
                    printf("Insufficient arguments.\n");
                    print_usage();
                    exit(-1);  
                }

                bit_index = i;
                string_to_digit(argv[i], nti, 0, bit_index);
                b = atoi(argv[i]);
                nti->bit_count = b;
                bit_index = 0;
                continue;

            } else if(r_flag > 0 && i == r_flag+1) {

                /* NUMBER AFTER -r Flag */
                if(!(argv[i][0] > 47 && argv[i][0] < 58)) {
                    printf("Insufficient arguments.\n");
                    print_usage();
                    exit(-1);  
                }

                split_r(argv[i], nti);
                continue;
            }

            /* -b -r FLAGS */
            if(len == 2) {

                if(argv[i][0] == 45) {

                    if(argv[i][1] == 98) {
                        bit_flag = i;
                        continue;
                    }

                    if(argv[i][1] == 114) {
                        r_flag = i;
                        continue;
                    }
                    
                }

            }

            /* STARTS WITH 0 */
            if(argv[i][0] == 48) {

                if(argv[i][1] == 98) {
                    validate_binary(argv[i]);
                    normalize_binary(argv[i], nti);
                    continue;

                } else if(argv[i][1] == 120) {
                    hex_string_to_digit(argv[i], nti);
                    continue;
                }  

            }

            /* STARTS WITH negative sign */
            if(argv[i][0] == 45) {
                neg_flag++;
                string_to_digit(argv[i], nti, neg_flag, bit_index);
            } else {
                string_to_digit(argv[i], nti, neg_flag, bit_index);
            }

        }

    }

    return;
}

/* Gets either new unsigned or regular unsigned. Continually divides
number by 16 and uses remaineder as hex */
void decimal_to_hex(struct nt_info *nti) {
    
    unsigned int num = 0; 
    int rem = 0, index = 7, letter = 0, i = 0, j = 0, zeros = 0;
    int bit_length = nti->bit_count;
    char arr[16];

    if(nti->new_unsigned_value > 0) {
        num = nti->new_unsigned_value;
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

    if(bit_length > 0) {
        bit_length /= 4;
    } else {
        bit_length = 8;
    }

    zeros = bit_length - i;

    printf("0x");

    for(j = 0; j < zeros; j++) {
        printf("0");
    }

    for(j = i-1; j >= 0; j--) {

        if(bit_length-1 < j){
            continue;
        } else {
            printf("%c", arr[j]);
        }

    }

    printf(" (base 16) \n");

}

void print_conversions(struct nt_info *nti) {
    print_binary(nti);
    print_binary_value_c_literal(nti);
    decimal_to_hex(nti);
    print_signed_and_unsigned_value(nti);
    return;
}

int main(int argc, char **argv) {

    struct nt_info nti;
    nt_info_init(&nti);

    parse_args(argc, argv, &nti);
    change_string_length(&nti);

    print_conversions(&nti);

    return 0;

}



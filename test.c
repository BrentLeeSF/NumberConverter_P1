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
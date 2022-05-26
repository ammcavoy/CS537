BEGIN {
    for (i = 1000; i > 0; i--) {
        printf("target%d: target%d\n", i, i - 1);
        printf("\techo Bulding target%d\n", i);
        printf("\n");
    }

    printf("target0:\n");
    printf("\techo Building target0\n");
    printf("\n");
}

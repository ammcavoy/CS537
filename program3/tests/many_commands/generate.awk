BEGIN {
    printf("target:\n");

    for (i = 1; i <= 1000; i++) {
        printf("\techo Running command %d\n", i);
    }
}

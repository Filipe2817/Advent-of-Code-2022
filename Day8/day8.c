#include <stdio.h>
#include <stdlib.h>

// ===================================================== UTILS ====================================================== //



// ===================================================== PART 1 ===================================================== //

void part1(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        
    }

    free(line);
    printf("Solution for Part 1: %s\n", result);
}

// ===================================================== PART 2 ===================================================== //
/*
void part2(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {

    }

    free(line);
    printf("Solution for Part 2: %s\n", result);
}
*/
// ================================================================================================================== //

int main() {
    FILE *fptr = NULL;
    fptr = fopen("input.txt", "r");

    if (fptr == NULL) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    part1(fptr);
    rewind(fptr);
    //part2(fptr);

    fclose(fptr);
    return 0;
}

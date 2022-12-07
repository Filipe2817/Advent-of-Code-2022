#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

typedef struct array {
    int *array;
    int used;
    int size;
} Array;

Array *createArray(size_t initialSize) {
    Array *a = malloc(sizeof(struct array));
    a->array = malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
    return a;
}

void insertArray(Array *a, int element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

void freeArray(Array *a) {
    free(a->array);
    free(a);
}

int str_to_int(const char *str) {
    char *endptr;
    errno = 0;

    long long_var = strtol(str, &endptr, 10);
    //   out of range   , extra junk at end,  no conversion at all
    if (errno == ERANGE || *endptr != '\0' || str == endptr) {
        perror("Something went wrong converting string to integer!\n");
        exit(EXIT_FAILURE);
    }

    // Needed when `int` and `long` have different ranges
    #if LONG_MIN < INT_MIN || LONG_MAX > INT_MAX
    if (long_var < INT_MIN || long_var > INT_MAX) {
        errno = ERANGE;
        perror("String value is out of range for type integer\n");
        exit(EXIT_FAILURE);
    }
    #endif

    return (int)long_var;
}

int compare(const void *a, const void *b) {
    return (*(int *)b - *(int *)a); // b - a because we want descending order
}

void parse(char *filename, Array *array) {
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    int sum = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        if (*line == '\n') {
            insertArray(array, sum);
            sum = 0;
        } else {
            line[strcspn(line, "\n")] = 0;
            sum += str_to_int(line);
        }
    }
    free(line);
    fclose(fptr);
}

int main() {
    size_t size = 32;
    Array *a = createArray(size);

    parse("input.txt", a);
    qsort(a->array, a->used, sizeof(int), compare);
    int sum3 = a->array[0] + a->array[1] + a->array[2];

    printf("Solution for Part 1: %d\n", a->array[0]);
    printf("Solution for Part 2: %d\n", sum3);

    freeArray(a);
    return 0;
}

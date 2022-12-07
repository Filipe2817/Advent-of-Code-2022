#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

// ===================================================== UTILS ====================================================== //

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

void get_section_ids(char *line, int ids[]) {
    int index = 0;
    char *line_copy = NULL, *save = NULL;
    char *token = NULL;

    line_copy = save = strdup(line);
    assert(line_copy != NULL);

    while ((token = strsep(&line_copy, "-,")) != NULL)
        ids[index++] = str_to_int(token);

    free(save);
}

// ===================================================== PART 1 ===================================================== //

int check_if_fully_contains(int ids[]) {
    int result = 0;

    if ((ids[0] <= ids[2] && ids[1] >= ids[3]) || (ids[0] >= ids[2] && ids[1] <= ids[3]))
        result = 1;

    return result;
}

void part1(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    int ids[4];
    int fully_contained_ranges = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        get_section_ids(line, ids);
        fully_contained_ranges += check_if_fully_contains(ids);
    }

    free(line);
    printf("Solution for Part 1: %d\n", fully_contained_ranges);
}

// ===================================================== PART 2 ===================================================== //

int check_if_overlaps(int ids[]) { // Consider it overlaps and check if it doesn't
    int result = 1;

    if ((ids[0] < ids[2] && ids[1] < ids[2]) || (ids[0] > ids[3] && ids[1] > ids[3]))
        result = 0;

    return result;
}

void part2(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    int ids[4];
    int overlaped_ranges = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        get_section_ids(line, ids);
        overlaped_ranges += check_if_overlaps(ids);
    }

    free(line);
    printf("Solution for Part 2: %d\n", overlaped_ranges);
}

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
    part2(fptr);

    fclose(fptr);
    return 0;
}

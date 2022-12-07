#include <stdio.h>
#include <stdlib.h>

// ===================================================== UTILS ====================================================== //

typedef struct rose_tree {
    char *value;
    RT **children;
    int num_children;
} RT;

RT *new_tree(char *value, int num_children) {
    RT *t = malloc(sizeof(struct rose_tree));
    t->value = strdup(value);
    t->num_children = num_children;
    t->children = malloc(num_children * sizeof(struct rose_tree *));
    return t;
}

void add_child(RT *t, RT *child) {
    t->children[t->num_children] = child;
    t->num_children++;
}

void print_tree(RT *t, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("%s\n", t->value);
    for (int i = 0; i < t->num_children; i++) {
        print_tree(t->children[i], indent + 2);
    }
}

// ===================================================== PART 1 ===================================================== //

void part1(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    int sum_sizes = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        if (*line == '$')

    }

    free(line);
    printf("Solution for Part 1: %d\n", sum_sizes);
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

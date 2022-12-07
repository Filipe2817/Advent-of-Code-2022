#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

// ===================================================== UTILS ====================================================== //

#define STACK_SIZE 60

typedef struct stack {
    char data[STACK_SIZE];
    int top;
} Stack;

void init_stack(Stack *s) {
    s->top = -1;
}

int is_empty(Stack *s) {
    return (s->top == -1);
}

int is_full(Stack *s) {
    return (s->top == STACK_SIZE-1);
}

void push(Stack *s, char elem) {
    if (is_full(s)) {
        printf("Error: stack is full\n");
        return;
    }

    s->data[++s->top] = elem;
}

char pop(Stack *s) {
    if (is_empty(s)) {
        printf("Error: stack is empty\n");
        return 0;
    }

    char elem = s->data[s->top--];

    return elem;
}

char peek(Stack *s) {
    if (is_empty(s)) {
        printf("Error: stack is empty\n");
        return 0;
    }

    return s->data[s->top];
}

void revert_stack(Stack *s) {
    int i, j;
    char tmp;

    for (i = 0, j = s->top; i < j; i++, j--) {
        tmp = s->data[i];
        s->data[i] = s->data[j];
        s->data[j] = tmp;
    }
}

void print_stack(Stack *s) {
    printf("Stack: ");
    for (int i = 0; i <= s->top; i++) {
        printf("%c ", s->data[i]);
    }
    printf("\n");
}

// ===================================================== PART 1 ===================================================== //

void insert_crates_in_stacks(char *line, Stack *stacks, int line_length) {
    char chunk[4];
    int index = 0;

    while (line_length > 0) {
        strncpy(chunk, line, 4); // Maybe I should put \0 at the end but I'm not reading the array anyways
        if (!isspace(chunk[1]))
            push(&stacks[index], chunk[1]);
        index++;
        line_length -= 4;
        line += 4;
    }
}

void get_rearrangement_procedure(char *line, int instruction[]) {
    int index = 0;
    char *line_copy = NULL, *save = NULL;
    
    line_copy = save = strdup(line);
    assert(line_copy != NULL);    

    while (*line_copy) {
        if (isdigit(*line_copy)) {
            long val = strtol(line_copy, &line_copy, 10);
            assert(val <= INT_MAX);
            instruction[index++] = (int)val;
        } else
            line_copy++;
    }

    instruction[1]--; // 1st stack is on index 0
    instruction[2]--;

    free(save);
}

void move_crates(int instruction[], Stack *stacks) {
    char elem;

    for (int i = 0; i < instruction[0]; i++) {
        elem = pop(&stacks[instruction[1]]);
        push(&stacks[instruction[2]], elem);
    }
}

void part1(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    nread = getline(&line, &len, fptr);
    fseek(fptr, 0, SEEK_SET); // Move the file pointer to the start

    int line_length = strlen(line);
    int number_of_stacks = line_length / 4; // each crate occupies 4 chars counting with spaces and \n    
    Stack stacks[number_of_stacks];
    int instruction[3];

    for (int i = 0; i < number_of_stacks; i++)
        init_stack(&stacks[i]);

    while ((nread = getline(&line, &len, fptr)) != -1 && *line != '\n') {
        if (isdigit(line[1]))
            continue;
        insert_crates_in_stacks(line, stacks, line_length);
    }

    for (int i = 0; i < number_of_stacks; i++)
        revert_stack(&stacks[i]);

    while ((nread = getline(&line, &len, fptr)) != -1) {
        get_rearrangement_procedure(line, instruction);
        move_crates(instruction, stacks);
    }

    char *result = malloc(16 * sizeof(char));
    char *p = result;

    for (int i = 0; i < number_of_stacks; i++)
        p += sprintf(p, "%c", peek(&stacks[i]));

    free(line);
    printf("Solution for Part 1: %s\n", result);
    free(result);
}

// ===================================================== PART 2 ===================================================== //

void move_crates_9001(int instruction[], Stack *stacks) {
    int i;
    int n_elems = instruction[0];
    char elems[n_elems];

    for (i = 0; i < n_elems; i++)
        elems[i] = pop(&stacks[instruction[1]]);

    for (i = n_elems - 1; i >= 0; i--)
        push(&stacks[instruction[2]], elems[i]);
}

void part2(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    nread = getline(&line, &len, fptr);
    fseek(fptr, 0, SEEK_SET);

    int line_length = strlen(line);
    int number_of_stacks = line_length / 4; 
    Stack stacks[number_of_stacks];
    int instruction[3];

    for (int i = 0; i < number_of_stacks; i++)
        init_stack(&stacks[i]);

    while ((nread = getline(&line, &len, fptr)) != -1 && *line != '\n') {
        if (isdigit(line[1]))
            continue;
        insert_crates_in_stacks(line, stacks, line_length); // part 1
    }

    for (int i = 0; i < number_of_stacks; i++)
        revert_stack(&stacks[i]);

    while ((nread = getline(&line, &len, fptr)) != -1) {
        get_rearrangement_procedure(line, instruction); // part 1
        move_crates_9001(instruction, stacks);
    }

    char *result = malloc(16 * sizeof(char));
    char *p = result;

    for (int i = 0; i < number_of_stacks; i++)
        p += sprintf(p, "%c", peek(&stacks[i]));

    free(line);
    printf("Solution for Part 2: %s\n", result);
    free(result);
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

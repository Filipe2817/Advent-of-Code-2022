#include <stdio.h>
#include <stdlib.h>

// ===================================================== UTILS ====================================================== //

typedef struct queue {
    char *chars;
    int front;
    int rear;
    int capacity;
} Queue;

Queue *create_queue(int capacity) {
    Queue *q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    q->capacity = capacity;
    q->chars = malloc(capacity * sizeof(char));
    return q;
}

int is_empty(Queue *q) {
    int r = 0;

    if (q->front == -1 && q->rear == -1)
        r = 1;

    return r;
}

int is_full(Queue *q) {
    int r = 0;

    if (((q->rear + 1) % q->capacity) == q->front)
        r = 1;

    return r;
}

void enqueue(Queue *q, char c) {
    if (is_full(q)) {
        printf("Error: Queue is full!\n");
        return;
    }

    if (is_empty(q))
        q->front = 0;

    q->rear = (q->rear + 1) % q->capacity;
    q->chars[q->rear] = c;
}

void dequeue(Queue *q) { // Dequeue adapted to the problem
    if (is_empty(q)) {
        printf("Error: Queue is empty!\n");
        return;
    }

    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front = (q->front + 1) % q->capacity;
}

int has_repeated_chars(Queue *q) {
    int r = 0;
    int position;
    int seen_chars[26] = {0};

    for (int i = 0; i < q->capacity && r != 1; i++) {
        position = q->chars[i] - 'a';

        if (seen_chars[position])
            r = 1;
        else
            seen_chars[position] = 1;
    }
    
    return r;
}

// ===================================================== PART 1 ===================================================== //

void part1(FILE *fptr) {
    char c;
    int index = 0;
    Queue *q = create_queue(4);

    while (!is_full(q) && (c = getc(fptr)) != '\n') {
        enqueue(q, c);
        index++;
    }
    
    while ((c = getc(fptr)) != '\n') {
        if (!has_repeated_chars(q))
            break;
        
        dequeue(q);
        enqueue(q, c);
        index++;
    }

    printf("Solution for Part 1: %d\n", index);
}

// ===================================================== PART 2 ===================================================== //

void part2(FILE *fptr) { // same as part 1 changing the capacity of the queue
    char c;
    int index = 0;
    Queue *q = create_queue(14);

    while (!is_full(q) && (c = getc(fptr)) != '\n') {
        enqueue(q, c);
        index++;
    }
    
    while ((c = getc(fptr)) != '\n') {
        if (!has_repeated_chars(q))
            break;
        
        dequeue(q);
        enqueue(q, c);
        index++;
    }

    printf("Solution for Part 2: %d\n", index);
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

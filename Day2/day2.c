#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ===================================================== PART 1 ===================================================== //

int get_round_points1(int opponent, int me) {
    int outcome_points[] = {3, 6, 0};
    int value, points;

    value = (me - opponent) % 3;
    points = outcome_points[value] + me - 2;

    return points;
}

void part1(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    int opponent, me;
    int total_points = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        opponent = *line - 'A';
        me = line[2] - 'X' + 3;
        total_points += get_round_points1(opponent, me);
    }

    free(line);
    printf("Solution for Part 1: %d\n", total_points);
}

// ===================================================== PART 2 ===================================================== //

int get_round_points2(int opponent, int outcome) {
    int shape_points[] = {1, 2, 3};
    int value, points;

    value = (opponent + outcome) % 3;
    points = shape_points[value] + ((outcome - 2) * 3);

    return points;
}

void part2(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    char opponent, outcome;
    int total_points = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        opponent = *line - 'A';
        outcome = line[2] - 'X' + 2;
        total_points += get_round_points2(opponent, outcome);
    }

    free(line);
    printf("Solution for Part 2: %d\n", total_points);
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

/*

Win Cases -> (A, Y) | (B, Z) | (C, X) => 6 points
Draw Cases -> (A, X) | (B, Y) | (C, Z) => 3 points
Loss Cases -> (A, Z) | (B, X) | (C, Y) => 0 points

X => 1 point
Y => 2 points
Z => 3 points

===================== PART 1 =====================

OFFSET TO DRAW => 0
OFFSET TO WIN => +1
OFFSET TO LOSE => +2

A = 0       X = 3 => 1 point
B = 1       Y = 4 => 2 points
C = 2       Z = 5 => 3 points

Note: Using X, Y and Z being 0, 1, 2 plus 3 to avoid negative numbers

The strategy is subtracting the my shape position by the opponent's shape position to get the offset between shapes and limit 
that value to the range of outcomes [0, 2] by using the remainder of the integer division by 3 (number of outcomes)

(A, Y) -> 4 - 0 => 4 % 3 = 1
(B, Z) -> 5 - 1 => 4 % 3 = 1
(C, X) -> 3 - 2 => 1 % 3 = 1

(A, X) -> 3 - 0 => 3 % 3 = 0 
(B, Y) -> 4 - 1 => 3 % 3 = 0
(C, Z) -> 5 - 2 => 3 % 3 = 0

(A, Z) -> 5 - 0 => 5 % 3 = 2
(B, X) -> 3 - 1 => 2 % 3 = 2
(C, Y) -> 4 - 2 => 2 % 3 = 2

0 -> DRAW | 1 -> WIN | 2 -> LOSS

int outcome[] = {3, 6, 0};

POINTS = outcome[value] + (my_shape - 2)

===================== PART 2 =====================

Now we know the outcome. Adding the offset of the outcome to the opponents shape position and limiting that value to the range
of shapes [0, 2] we get the position of the shape we have to pick

OFFSET TO DRAW => 0           DRAW = Y = 3 points
OFFSET TO WIN => +1           WIN = Z = 6 points
OFFSET TO LOSE => +2          LOSS = X = 0 points

Choose the X, Y and Z values based on offset of each outcome
Changing the values of X, Y and Z to something similar but ordered is going to help in the parsing phase 

A = 0       X = 2 = (0 + 2) % 3 => 2 
B = 1       Y = 0 = (1 + 2) % 3 => 3
C = 2       Z = 1 = (2 + 2) % 3 => 4


(A, Y) -> 0 + 3 => 3 % 3 = 0
(B, Y) -> 1 + 3 => 4 % 3 = 1
(C, Y) -> 2 + 3 => 5 % 3 = 2

(A, Z) -> 0 + 4 => 4 % 3 = 1
(B, Z) -> 1 + 4 => 5 % 3 = 2
(C, Z) -> 2 + 4 => 6 % 3 = 0

(A, X) -> 0 + 2 => 2 % 3 = 2
(B, X) -> 1 + 2 => 3 % 3 = 0
(C, X) -> 2 + 2 => 4 % 3 = 1

0 -> ROCK | 1-> PAPER | 2 -> SCISSORS

int shape[] = {1, 2, 3};

POINTS = shape[value] + ((outcome - 2) * 3)

*/
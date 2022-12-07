#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int get_common_item_priority(int seen_chars[]) {
    int i;
    
    for (i = 0; i < 53 && seen_chars[i] != 3; i++);
    
    return i + 1;
}

void get_rucksack_items(char *rucksack, int seen_chars[]) {
    int priority;
    int previous_seen_chars[53];
    memcpy(previous_seen_chars, seen_chars, sizeof(previous_seen_chars));

    for (int i = 0; rucksack[i]; i++) {
        if (islower(rucksack[i]))
            priority = rucksack[i] - 'a';
        else
            priority = rucksack[i] - 'A' + 26;

        if (seen_chars[priority] - previous_seen_chars[priority] == 0)
            seen_chars[priority]++;
    }
}

int parse(char *filename) {
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    char *fst_comp = NULL, *snd_comp = NULL;
    int total_priority = 0, elf_number = 1;
    int seen_chars[53] = { 0 };
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;

        if (elf_number == 3) { // new group of elfs found
            get_rucksack_items(line, seen_chars);
            total_priority += get_common_item_priority(seen_chars);
            elf_number = 1;
            memset(seen_chars, 0, sizeof(seen_chars));
        } else {
            get_rucksack_items(line, seen_chars);
            elf_number++;
        }
    }
    free(line);
    fclose(fptr);

    return total_priority;
}

int main() {
    int total_priority = parse("input.txt");

    printf("Solution for Part 2: %d\n", total_priority);

    return 0;
}
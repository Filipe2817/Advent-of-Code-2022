#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void split_rucksack(char *line, char **fst_comp, char **snd_comp) {
    int len_to_split = strlen(line) / 2;

    char *first_comp = malloc(len_to_split + 1);  // Can't change the *fst_comp pointer
    memcpy(first_comp, line, len_to_split);
    first_comp[len_to_split] = 0;

    *fst_comp = first_comp;
    *snd_comp = line + len_to_split;
}

int get_common_item_priority(char *fst_comp, char *snd_comp) {
    bool seen_chars[53] = { false };
    int i, priority;
    bool found = false;

    for (i = 0; fst_comp[i]; i++) {
        if (islower(fst_comp[i]))
            priority = fst_comp[i] - 'a';
        else
            priority = fst_comp[i] - 'A' + 26;

        seen_chars[priority] = true;
    }

    for (i = 0; snd_comp[i] && !found; i++) {
        if (islower(snd_comp[i]))
            priority = snd_comp[i] - 'a';
        else
            priority = snd_comp[i] - 'A' + 26;

        if (seen_chars[priority])
            found = true;
    }

    return priority + 1;
}

int parse(char *filename) {
    FILE *fptr = NULL;
    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        perror("Error!");
        exit(EXIT_FAILURE);
    }

    char *fst_comp = NULL, *snd_comp = NULL;
    int total_priority = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    while ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        split_rucksack(line, &fst_comp, &snd_comp);
        total_priority += get_common_item_priority(fst_comp, snd_comp);
        free(fst_comp);
    }
    free(line);
    fclose(fptr);

    return total_priority;
}

int main() {
    int total_priority = parse("input.txt");

    printf("Solution for Part 1: %d\n", total_priority);

    return 0;
}
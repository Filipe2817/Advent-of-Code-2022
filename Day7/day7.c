#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

// ===================================================== UTILS ====================================================== //

typedef struct node {
    char *name;
    int size;
    bool is_dir;
    struct node *first_child;
    struct node *next_child;
    struct node *parent;
} Node;

Node *create_node(char *name, int size, bool value) {
    Node *new = malloc(sizeof(struct node));
    assert(new != NULL);

    new->name = strdup(name);
    assert(new->name != NULL);

    new->size = size;
    new->is_dir = value;
    new->first_child = NULL;
    new->next_child = NULL;
    new->parent = NULL;

    return new;
}

void append_child(Node *RT, char *name, int size, bool value) {
    Node *new = create_node(name, size, value);
    new->parent = RT;

    Node *temp = RT;
    while (temp != NULL) {
        temp->size += size;
        temp = temp->parent;
    }

    if (RT->first_child != NULL) {
        Node *temp_next = RT->first_child;

        while (temp_next->next_child)
            temp_next = temp_next->next_child;

        temp_next->next_child = new;
    } else {
        RT->first_child = new;
    }
}

void free_tree(Node *RT) {
    Node *temp_child = RT->first_child;

    while (temp_child != NULL) {
        Node *temp_next = temp_child->next_child;
        free_tree(temp_child);
        temp_child = temp_next;
    }

    free(RT->name);
    free(RT);
}

void print_content(Node *RT, int depth) {
    for (int i = 0; i < depth * 2; i++) printf(" ");
    if (RT->is_dir)
        printf("- %s (dir, size=%d)\n", RT->name, RT->size);
    else
        printf("- %s (file, size=%d)\n", RT->name, RT->size); 
}

void print_tree(Node *RT, int depth) {
    if (depth == 0) 
        print_content(RT, depth);
    
    // Depth First Traversal to output the subdirectories right below the main directory
    Node *temp_child = RT->first_child;
    while (temp_child) {
        print_content(temp_child, depth + 1);
        print_tree(temp_child, depth + 1);
        temp_child = temp_child->next_child;
    }
}

// ===================================================== PART 1 ===================================================== //

void change_directory(char *cmd, int cmd_len, Node **current_dir) {
    char arg[cmd_len - 4]; //cmd_len - 5 + 1
    memset(arg, 0, (cmd_len - 4) * sizeof(char));
    strncpy(arg, cmd + 5, cmd_len - 5);

    if (!strcmp(arg, ".."))
        *current_dir = (*current_dir)->parent;
    else {
        *current_dir = (*current_dir)->first_child;
        while (strcmp((*current_dir)->name, arg))
            *current_dir = (*current_dir)->next_child;
    }
}        

void insert_directory_content(char *content, Node *current_dir) {
    char name[256]; // max file/directory name size in Linux is 255 chars
    int size = 0;

    if (isdigit(*content)) {
        sscanf(content, "%d %s", &size, name);
        append_child(current_dir, name, size, false);
    } else {
        strcpy(name, content + 4);
        append_child(current_dir, name, size, true);
    }
}

int find_directories_size_up_to(Node *root, int limit) {
    if (!root) return 0;
    if (!root->is_dir) return 0;

    static int sum = 0;
    if (root->size <= limit)
        sum += root->size;

    Node *temp = root->first_child;
    while (temp != NULL) {
        find_directories_size_up_to(temp, limit);
        temp = temp->next_child;
    }

    return sum;
}

void part1(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    int cmd_len;
    Node *root, *current_dir;

    if ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        cmd_len = strlen(line);
        char *arg = line + 5;
        root = create_node(arg, 0, true);
        current_dir = root;
    }
    
    while ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        if (*line == '$') {
            cmd_len = strlen(line);
            if (cmd_len == 4) // ls command
                continue;
            change_directory(line, cmd_len, &current_dir);
        } else
            insert_directory_content(line, current_dir);
    }

    int sum_sizes = 0;
    int limit = 100000;
    sum_sizes += find_directories_size_up_to(root, limit);
    print_tree(root, 0);

    free(line);
    free_tree(root);
    printf("Solution for Part 1: %d\n", sum_sizes);
}

// ===================================================== PART 2 ===================================================== //

int find_smallest_directory_to_free(Node *root, int need_to_free) {
    if (!root) return 0;
    if (!root->is_dir) return 0;

    static int dir_size = 0;
    if (root->size >= need_to_free && (dir_size == 0 || dir_size > root->size))
        dir_size = root->size;

    Node *temp = root->first_child;
    while (temp != NULL) {
        find_smallest_directory_to_free(temp, need_to_free);
        temp = temp->next_child;
    }

    return dir_size;
}

void part2(FILE *fptr) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;

    int cmd_len;
    Node *root, *current_dir;

    if ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        cmd_len = strlen(line);
        char *arg = line + 5;
        root = create_node(arg, 0, true);
        current_dir = root;
    }
    
    while ((nread = getline(&line, &len, fptr)) != -1) {
        line[strcspn(line, "\n")] = 0;
        if (*line == '$') {
            cmd_len = strlen(line);
            if (cmd_len == 4) // ls command
                continue;
            change_directory(line, cmd_len, &current_dir); // part 1
        } else
            insert_directory_content(line, current_dir); // part 1
    }

    int need_to_free = 30000000 - (70000000 - root->size);
    int smallest_dir = find_smallest_directory_to_free(root, need_to_free);

    free(line);
    free_tree(root);
    printf("Solution for Part 2: %d\n", smallest_dir);
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

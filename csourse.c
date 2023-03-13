// Author: Denys Skira
// Date of last modification: 13/03/2023
// Coursework 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_TYPES_SIZE 3
typedef char* String;

int getCharacterCountPerLine(const String line);
int getCommentCountByLine(const char* const line);
void findDeclaredVariables(char line[], const String data_types[],
                           String** var_array_name, int* var_array_len);
void addVariableNameToArray(String** var_arr, int* var_arr_len,
                            const String beg_var_name, int var_name_len);
void findVariableReferences(const char string[], String* var_arr,
                            int** var_count_arr, int var_arr_len,
                            int* var_count_arr_len);
void addVariablesToFile(String filename, String* var_array_name,
                        int* var_array_count, int var_count);

int main(int args, String* argv) {
    if (args != 2) {
        return 1;
    }

    FILE* input_file = NULL;
    FILE* output_file = NULL;

    char buffer[256];

    int character_count = 0;
    int line_count = 0;
    int comment_count = 0;
    int var_count = 0;

    const String data_types[DATA_TYPES_SIZE] = {"int ", "double ", "char "};
    String* var_array_name = NULL;
    int* var_array_count = NULL;
    int var_count_temp = 0;

    input_file = fopen(argv[1], "r");

    if (input_file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), input_file) != NULL) {
        character_count += getCharacterCountPerLine(buffer);
        line_count++;
        comment_count += getCommentCountByLine(buffer);

        findDeclaredVariables(buffer, data_types, &var_array_name, &var_count);
        findVariableReferences(buffer, var_array_name, &var_array_count,
                               var_count, &var_count_temp);
    }

    printf("Total number of lines: %d\n", line_count);
    printf("Number of characters: %d\n", character_count);
    printf("Number of comments: %d\n", comment_count);
    printf("Total number of variables: %d\n", var_count);

    addVariablesToFile(argv[1], var_array_name, var_array_count, var_count);
}

// Function takes a string and counts the number of characters in it.
// It ignores newline, tab, and space characters.
// It returns the number of characters.
int getCharacterCountPerLine(const String line) {
    static const String ignore = "\n\t ";
    static bool isIgnored[256] = {false};

    // Build lookup table
    for (int i = 0; ignore[i]; i++) {
        isIgnored[ignore[i]] = true;
    }

    int count = 0;
    int length = strlen(line);

    for (int i = 0; i < length; i++) {
        if (!isIgnored[line[i]]) {
            count++;
        }
    }

    return count;
}

// Function takes a string and counts the number of comments in it.
// It searches for "//" in the string and counts them.
// It returns the number of comments.
int getCommentCountByLine(const char* const line) {
    int count = 0;
    String commentStart = strstr(line, "//");
    while (commentStart != NULL) {
        commentStart = strstr(commentStart + 1, "//");
        count++;
    }
    return count;
}

// Function takes a string, an array of data types, an array of variable names,
// and a variable to keep track of the number of variables.
// It searches for variable declarations in the string and adds them to the
// array of variable names.
// It ignores variables declared inside a function call or inside quotes. It
// does not add variables if there is a comment on the same line.
// It modifies the array of variable names and the number of variables through
// pointers.

void findDeclaredVariables(char line[], const String data_types[],
                           String** var_array_name, int* var_array_len) {
    String string = NULL;
    String var_name_temp = NULL;
    int var_name_length;
    const String var_type = NULL;

    if (strstr(line, "//") != NULL) {
        return;
    }

    for (int i = 0; i < DATA_TYPES_SIZE; i++) {
        string = line;

        while (string = strstr(string, data_types[i])) {
            string += strlen(data_types[i]);

            if (string[0] == '*') {
                string = strchr(string, ' ') + 1;
            }

            var_name_temp = string;
            string = strchr(string, ' ') + 1;

            var_name_length = string - var_name_temp - 1;

            if (string[0] != '(' && '\"') {
                addVariableNameToArray(var_array_name, var_array_len,
                                       var_name_temp, var_name_length);
            }
        }
    }
}

// Function takes an array of variable names, the length of the array, a string
// containing the name of a variable, and the length of the variable name.
// It adds the variable name to the array of variable names and increments the
// length of the array. It allocates memory for the new variable name and the
// updated array of variable names.

void addVariableNameToArray(String** var_array_names, int* var_arr_len,
                            const String temp_var_name, int var_name_len) {
    // Dynamically reallocates the memory allocated for
    // the first element in the var_array_names array. New size of the
    // allocated memory block is calculated by multiplying the current size of
    // the block (var_arr_len[0]) by the size of a String object
    // (sizeof(String)), and adding space for one more String object (the +1 in
    // the expression).
    String* temp =
        realloc(var_array_names[0], (var_arr_len[0] + 1) * sizeof(String));

    if (temp == NULL) {
        return;
    }
    var_array_names[0] = temp;

    // Dynamically allocates memory on the heap for a
    // character array of size (var_name_len + 1) and assigns the address of the
    // allocated memory block to a pointer variable temp_var of type String.

    // The (var_name_len + 1) expression calculates the size of the memory block
    // to be allocated in bytes. It is var_name_len + 1 because the size of the
    // character array needs to be large enough to store var_name_len characters
    // plus a null terminator character '\0' at the end of the array.
    String temp_var = (String)malloc((var_name_len + 1) * sizeof(char));
    if (temp_var == NULL) {
        return;
    }

    memcpy(temp_var, temp_var_name, var_name_len);
    temp_var[var_name_len] = '\0';

    (var_array_names[0])[(var_arr_len[0])++] = temp_var;
}

// Function takes a string, an array of variable names, an array of variable
// counts, the length of the array of variable names, and a variable to keep
// track of the length of the array of variable counts.
// It searches for variable references in the string and increments the count of
// the corresponding variable in the array of variable counts. It modifies the
// array of variable counts and the length of the array through pointers.

void findVariableReferences(const char string[], String* var_arr,
                            int** var_count_arr, int var_arr_len,
                            int* var_count_arr_len) {
    String token = NULL;
    int size;

    if (var_arr_len != (var_count_arr_len[0])) {
        int* temp = malloc(var_arr_len * sizeof(int));

        if (!temp) {
            return;
        }

        memset(temp, -1, var_arr_len * sizeof(int));

        if (var_count_arr && var_count_arr[0]) {
            memcpy(temp, *var_count_arr, (*var_count_arr_len) * sizeof(int));
            free(*var_count_arr);
        }

        (var_count_arr[0]) = temp;
        (var_count_arr_len[0]) = var_arr_len;
    }

    for (int i = 0; i < var_arr_len; i++) {
        size_t var_len = strlen(var_arr[i]);
        size_t token_len = var_len + 3;
        String token = malloc(token_len);
        snprintf(token, token_len, " %s ", var_arr[i]);

        const char* tmp = strstr(string, token);
        while (tmp) {
            (var_count_arr[0])[i]++;
            tmp = strstr(tmp + strlen(token), token);
        }

        token[0] = '\t';

        tmp = strstr(string, token);
        while (tmp) {
            (var_count_arr[0])[i]++;
            tmp = strstr(tmp + strlen(token), token);
        }

        free(token);
    }
}

// Function takes a filename, an array of variable names, an array of variable
// counts, and the length of the array of variable counts.
// It writes the variable names and their counts to a file with the same name as
// the input file but with the ".s" extension.

void addVariablesToFile(String filename, String* var_array_name,
                        int* var_array_count, int var_count) {
    char* new_filename = malloc(strlen(filename) + 1);
    strncpy(new_filename, filename, strlen(filename) - 2);
    new_filename[strlen(filename) - 2] = '\0';
    strcat(new_filename, ".s");

    FILE* file = fopen(new_filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s for writing\n",
                new_filename);
        exit(1);
    }

    for (int i = 0; i < var_count; i++) {
        fprintf(file, "%s\t%d\n", var_array_name[i], var_array_count[i]);
    }
    free(new_filename);
    fclose(file);
}

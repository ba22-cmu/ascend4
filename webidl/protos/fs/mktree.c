#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void create_directory(const char *path) {
    if (mkdir(path, 0755) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }
}

void create_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "fred\n");  // Write "fred" to the file
    fclose(file);
}

int main() {
    // Define the directory and file structure
    const char *dir_a = "a";
    const char *dir_b = "a/b";
    const char *file_1 = "a/b/1";

    // Create directory a
    create_directory(dir_a);
    
    // Create directory a/b
    create_directory(dir_b);

    // Create file a/b/1 containing "fred"
    create_file(file_1);

    printf("Directory and file created:\n");
    printf("%s/\n", dir_a);
    printf("  %s/\n", dir_b);
    printf("    %s (file)\n", "1");

    return 0;
}


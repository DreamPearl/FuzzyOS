// list directory
#include <stdio.h>
#include <string.h>

void process(char filename[]) {
    printf("file: %s\n", filename);

    FILE *handler = fopen(filename, "r");
    if(handler==NULL) {
        printf("file open: failed: %s\n", filename);
        return;
    }
    puts("content: \n");
    char buffer[80];
    for (size_t i = 0; i < 20; i++) {
        if (!fgets(buffer, sizeof(buffer), handler)) {
            break;
        }
        puts(buffer);
        putchar('\n');
    }
    fclose(handler);
}

int print_usage() {
    printf("Usage: cat <filename>");
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return print_usage();
    }
    char *filename = argv[1];
    process(filename);
    return 0;
}
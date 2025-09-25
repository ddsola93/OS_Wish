#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For possibly future needs like fork/exec

// Function prototypes
void print_error_and_exit(const char *msg, int status_code);
void interactive_mode();
void batch_mode(const char *filename);
void shell_loop(FILE *input_stream);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // No arguments: Interactive Mode
        interactive_mode();
    } else if (argc == 2) {
        // One argument: Batch Mode
        batch_mode(argv[1]);
    } else {
        // More than one argument: Error
        print_error_and_exit("Usage: wish [batch_file]", 1);
    }

    return 0;
}

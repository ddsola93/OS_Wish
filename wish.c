// <stdio.h>: Input/output functions (printf, fgets, etc.)
#include <stdio.h>
// <stdlib.h>: Memory management, process control, conversions (exit, malloc, free)
#include <stdlib.h>
// <string.h>: String manipulation (strcpy, strcmp, strtok)
#include <string.h>
// <ctype.h>: Character handling (isspace, isalpha, etc.)
#include <ctype.h>
// <unistd.h>: POSIX API (fork, exec, access)
#include <unistd.h>
// <sys/wait.h>: Process control (wait, waitpid)
#include <sys/wait.h>



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
// Input validation ensures that the command entered by the user is in a proper format and has the correct number of arguments.
int is_valid_command(char *command) {
    // Skip empty commands or commands with only whitespace
    while (*command != '\0') {
        if (!isspace(*command)) {
            return 1; // Valid command
        }
        command++;
    }
    return 0; // Invalid (empty/whitespace only)
}


void print_error_and_exit(const char *msg, int status_code)
{
}

void interactive_mode()
{
}

void batch_mode(const char *filename)
{
}

void shell_loop(FILE *input_stream)
{
    // Skip empty or whitespace-only lines
if (!is_valid_command(line)) {
    fprintf(stderr, "Error: Empty or invalid command\n");
    continue; // Go to next command
}
// Command Execution Errors
void execute_command(char **args) {
    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        fprintf(stderr, "Error: Fork failed\n");
        return;
    }
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "Error: Command not found: %s\n", args[0]);
            exit(1); // Exit child if exec fails
        }
    } else {
        // Parent process waits for child
        int status;
        waitpid(pid, &status, 0);
    }
}
// File Handling Errors
if (!file) {
    print_error_and_exit("Error: Cannot open file", 1);
}

}

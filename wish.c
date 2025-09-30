#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
}

// returns 0 if success, 1 if failure
int fork_process(char *command, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0){
        perror("fork failed");
        return 1
    }
    else if (pid == 0) {
        // Child: call execvp to replace image with new program
        execvp(command, argv);
        perror("execvp failed");
        exit(1);
    }
    else {
        // Parent: wait for child to finish
    }
    
}

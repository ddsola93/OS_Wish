#include <stdio.h>

#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 100
#define MAX_LINE 1024

char error_message[30] = "An error has occurred\n";
char *path_dirs[100];
int path_count = 0;

#include <unistd.h>

#include <sys/wait.h>

#include <fcntl.h>





#define MAX_ARGS 100

#define MAX_LINE 1024





char error_message[30] = "An error has occurred\n";

char *path_dirs[100];

int path_count = 0;





// Function prototypes

void print_error_and_exit(const char *msg, int status_code);

void interactive_mode();

void batch_mode(const char *filename);
void shell_loop(FILE *input_stream, int interactive);
void parse_and_execute(char *line);
void execute_command(char **args, int argc, char *outfile);
char *find_executable(char *command);
void builtin_cd(char **args, int argc);
void builtin_path(char **args, int argc);

int main(int argc, char *argv[]) {
    // Default path is /bin
    path_dirs[path_count++] = strdup("/bin");

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

void print_error_and_exit(const char *msg, int status_code) {
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(status_code);
}

void interactive_mode() {
    shell_loop(stdin, 1);
}

void batch_mode(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        print_error_and_exit("batch file open failed", 1);
    }
    shell_loop(file, 0);
    fclose(file);
}

void shell_loop(FILE *input_stream, int interactive) {
    char *line = NULL;
    size_t len = 0;

    while (1) {
        if (interactive) {
            printf("wish> ");
            fflush(stdout);
        }

        if (getline(&line, &len, input_stream) == -1) {
            break;  // EOF
        }

        parse_and_execute(line);
    }
    free(line);
    exit(0);
}

void parse_and_execute(char *line) {
    char *commands[MAX_ARGS];
    int cmd_count = 0;

    // Handle parallel commands separated by "&"
    char *cmd = strtok(line, "&");
    while (cmd != NULL && cmd_count < MAX_ARGS) {
        commands[cmd_count++] = strdup(cmd);
        cmd = strtok(NULL, "&");
    }

    pid_t pids[MAX_ARGS];
    int pid_count = 0;

    for (int i = 0; i < cmd_count; i++) {
        char *tokens[MAX_ARGS];
        int argc = 0;
        char *outfile = NULL;

        // Tokenize command (handle redirection ">")
        char *token = strtok(commands[i], " \t\n");
        while (token != NULL && argc < MAX_ARGS - 1) {
            if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " \t\n");
                if (token == NULL || strtok(NULL, " \t\n") != NULL) {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    return;
                }
                outfile = token;
                break;
            }
            tokens[argc++] = token;
            token = strtok(NULL, " \t\n");
        }
        tokens[argc] = NULL;
        if (argc == 0) continue;

        // Built-ins (no fork)
        if (strcmp(tokens[0], "exit") == 0) {
            if (argc != 1) {
                write(STDERR_FILENO, error_message, strlen(error_message));
            } else {
                exit(0);
            }
        } else if (strcmp(tokens[0], "cd") == 0) {
            builtin_cd(tokens, argc);
        } else if (strcmp(tokens[0], "path") == 0) {
            builtin_path(tokens, argc);
        } else {
            // External command → run in child
            pid_t pid = fork();
            if (pid == 0) {
                execute_command(tokens, argc, outfile);
                exit(0);
            } else if (pid > 0) {
                pids[pid_count++] = pid;
            } else {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }
    }

    // Wait for children (parallel commands)
    for (int i = 0; i < pid_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

void execute_command(char **args, int argc, char *outfile) {
    char *exec_path = find_executable(args[0]);
    if (!exec_path) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    if (outfile != NULL) {
        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd < 0) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);
    }

    execv(exec_path, args);
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}

char *find_executable(char *command) {
    static char fullpath[MAX_LINE];
    for (int i = 0; i < path_count; i++) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path_dirs[i], command);
        if (access(fullpath, X_OK) == 0) {
            return fullpath;
        }
    }
    return NULL;
}

void builtin_cd(char **args, int argc) {
    if (argc != 2) {
        write(STDERR_FILENO, error_message, strlen(error_message));
        return;
    }
    if (chdir(args[1]) != 0) {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
}

void builtin_path(char **args, int argc) {
    path_count = 0;
    for (int i = 1; i < argc; i++) {
        path_dirs[path_count++] = strdup(args[i]);
    }
}





void parse_and_execute(char *line) {

    char *commands[MAX_ARGS];

    int cmd_count = 0;





    // Handle parallel commands separated by "&"

    char *cmd = strtok(line, "&");

    while (cmd != NULL && cmd_count < MAX_ARGS) {

        commands[cmd_count++] = strdup(cmd);

        cmd = strtok(NULL, "&");

    }





    pid_t pids[MAX_ARGS];

    int pid_count = 0;





    for (int i = 0; i < cmd_count; i++) {

        char *tokens[MAX_ARGS];

        int argc = 0;

        char *outfile = NULL;





        // Tokenize command (handle redirection ">")

        char *token = strtok(commands[i], " \t\n");

        while (token != NULL && argc < MAX_ARGS - 1) {

            if (strcmp(token, ">") == 0) {

                token = strtok(NULL, " \t\n");

                if (token == NULL || strtok(NULL, " \t\n") != NULL) {

                    write(STDERR_FILENO, error_message, strlen(error_message));

                    return;

                }

                outfile = token;

                break;

            }

            tokens[argc++] = token;

            token = strtok(NULL, " \t\n");

        }

        tokens[argc] = NULL;

        if (argc == 0) continue;





        // Built-ins (no fork)

        if (strcmp(tokens[0], "exit") == 0) {

            if (argc != 1) {

                write(STDERR_FILENO, error_message, strlen(error_message));

            } else {

                exit(0);

            }

        } else if (strcmp(tokens[0], "cd") == 0) {

            builtin_cd(tokens, argc);

        } else if (strcmp(tokens[0], "path") == 0) {

            builtin_path(tokens, argc);

        } else {

            // External command → run in child

            pid_t pid = fork();

            if (pid == 0) {

                execute_command(tokens, argc, outfile);

                exit(0);

            } else if (pid > 0) {

                pids[pid_count++] = pid;

            } else {

                write(STDERR_FILENO, error_message, strlen(error_message));

            }

        }

    }





    // Wait for children (parallel commands)

    for (int i = 0; i < pid_count; i++) {

        waitpid(pids[i], NULL, 0);

    }

}





void execute_command(char **args, int argc, char *outfile) {

    char *exec_path = find_executable(args[0]);

    if (!exec_path) {

        write(STDERR_FILENO, error_message, strlen(error_message));

        exit(1);

    }





    if (outfile != NULL) {

        int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);

        if (fd < 0) {

            write(STDERR_FILENO, error_message, strlen(error_message));

            exit(1);

        }

        dup2(fd, STDOUT_FILENO);

        dup2(fd, STDERR_FILENO);

        close(fd);

    }





    execv(exec_path, args);

    write(STDERR_FILENO, error_message, strlen(error_message));

    exit(1);

}





char *find_executable(char *command) {

    static char fullpath[MAX_LINE];

    for (int i = 0; i < path_count; i++) {

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path_dirs[i], command);

        if (access(fullpath, X_OK) == 0) {

            return fullpath;

        }

    }

    return NULL;

}





void builtin_cd(char **args, int argc) {

    if (argc != 2) {

        write(STDERR_FILENO, error_message, strlen(error_message));

        return;

    }

    if (chdir(args[1]) != 0) {

        write(STDERR_FILENO, error_message, strlen(error_message));

    }

}





void builtin_path(char **args, int argc) {

    path_count = 0;

    for (int i = 1; i < argc; i++) {

        path_dirs[path_count++] = strdup(args[i]);

    }

}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // create pipes
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // fork process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child process
        // close read end of pipe
        close(fd[0]);

        // redirect stdout to write end of pipe
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        // execute word count command
        execlp("wc", "wc", "-w", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // parent process
        // close write end of pipe
        close(fd[1]);

        // read input from stdin
        char buf[BUFSIZ];
        ssize_t nread;
        while ((nread = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
            // write input to pipe
            if (write(fd[0], buf, nread) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        // close read end of pipe
        close(fd[0]);

        // wait for child process to complete
        int status;
        if (wait(&status) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        // check child process status
        if (WIFEXITED(status)) {
            printf("word count: %d\n", WEXITSTATUS(status));
        } else {
            printf("child process exited abnormally\n");
        }
    }

    return 0;
}
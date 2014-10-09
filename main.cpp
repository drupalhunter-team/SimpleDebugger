#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc < 2)
    {
        printf("Usage: %s <program>\n", basename(argv[0]));
        exit(-1);
    }

    printf("Doing fork()...\n");
    pid_t pid = fork();
    printf("PID is %d\n", pid);

    int exec_error;
    switch (pid)
    {
        case 0: // Child
            printf("Running child %s", argv[1]);
            ptrace(PTRACE_TRACEME, 0, NULL, NULL);
            exec_error = execl(argv[1], NULL);
            printf("Error in exec(): %d", exec_error);
            break;
        case -1: // Error
            perror("fork()");
            exit(-1);
    }

    printf("In the parent...\n");

    int status;
    pid = waitpid(pid, &status, 0);
    int isExited = WIFEXITED(status);
    while (isExited != 1)
    {
        printf("waitpid() status: %x\n", status);
        ptrace(PTRACE_CONT, pid, 0L, 0L);
        pid = waitpid(pid, &status, 0);
        isExited = WIFEXITED(status);
    }
    printf("waitpid() status: %x\n", status);

    return 0;
}

void loadSymbols()
{

}
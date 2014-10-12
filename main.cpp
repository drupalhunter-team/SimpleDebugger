#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>

// MacOS
#include <libgen.h>

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "Symbols.h"

#ifdef __APPLE__
#define TRACE_ME PT_TRACE_ME
#define TRACE_CONTINUE PT_CONTINUE
#elif __linux
#define TRACE_ME PTRACE_TRACEME
#define TRACE_CONTINUE PTRACE_CONT
#endif

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
            ptrace(TRACE_ME, 0, NULL, 0);
            exec_error = execl(argv[1], basename(argv[1]), NULL);
            printf("Error in exec(): %d", exec_error);
            break;
        case -1: // Error
            perror("fork()");
            exit(-1);
    }

    printf("In the parent...\n");

    Symbols *symbols = new Symbols();
    symbols->Load(argv[1]);

    int status;
    pid = waitpid(pid, &status, 0);
    while (WIFEXITED(status) || (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL))
    {
        printf("waitpid() status: %x\n", status);

        ptrace(TRACE_CONTINUE, pid, 0L, 0L);

        pid = waitpid(pid, &status, 0);
    }
    printf("waitpid() status: %x\n", status);

    return 0;
}

// multiprocessing example
#include <stdio.h>
#include <process.h>


int main(int argc,char *argv[]) {
    printf("main process, executing fork()\n");
    int pid = fork();
    printf("fork()=%d\n", pid);
    int am_i_child;
    if(pid == 0) {
        // child process
        am_i_child=1;
        printf("child process: says Hi\n");
        yield(); // force process scheduler
    } else {
        am_i_child=0;
        // parent process
        printf("parent process: says Hi\n");
        printf("will for child process to exit\n");
        waitpid(pid, NULL);
    }
    printf("process exiting. Is current process the child: %s\n", (am_i_child)?"Yes":"No");

    return 0;
}
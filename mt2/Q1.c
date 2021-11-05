#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READEND 0
#define WRITEEND 1

int main(int argc, char* argv[])
{
    /* TODO: Q1
       Implement "ls /dev | xargs | cut -d ' ' -f<a>-<b>"
       where <a> and <b> are from the command line
       The use of system is prohibited.
    */

    int ls_to_xargs[2];
    assert(!pipe(ls_to_xargs));

    if (fork() == 0) {
        assert(dup2(ls_to_xargs[1], STDOUT_FILENO) == STDOUT_FILENO);
        close(ls_to_xargs[1]);
        close(ls_to_xargs[0]);
        execlp("ls", "ls", "/dev", NULL);
        assert(0);
    }

    int xargs_to_cut[2];
    assert(!pipe(xargs_to_cut));
    if (fork() == 0) {
        assert(dup2(ls_to_xargs[0], STDIN_FILENO) == STDIN_FILENO);
        assert(dup2(xargs_to_cut[1], STDOUT_FILENO) == STDOUT_FILENO);

        close(xargs_to_cut[1]);
        close(xargs_to_cut[0]);
        close(ls_to_xargs[1]);
        close(ls_to_xargs[0]);
        execlp("xargs", "xargs", NULL);
        assert(0);
    }
    assert(dup2(xargs_to_cut[0], STDIN_FILENO) == STDIN_FILENO);

    close(ls_to_xargs[1]);
    close(ls_to_xargs[0]);
    close(xargs_to_cut[1]);
    close(xargs_to_cut[0]);
    char cmdbuf[64];
    snprintf(cmdbuf, 64, "-f%s-%s", argv[1], argv[2]);

    execlp("cut", "cut", "-d ", cmdbuf, NULL);

    return 0;
}

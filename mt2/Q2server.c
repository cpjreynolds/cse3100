#include <dirent.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* prints the error that is encountered and terminate the program */
int checkError(int status, int line)
{
    if (status < 0) {
        printf("socket error(%d)-%d: [%s]\n", getpid(), line, strerror(errno));
        exit(-1);
    }
    return status;
}

/* creates a server-side socket that binds to the given port number and listens
 * for TCP connect requests */
int bindAndListen(int port)
{
    int sid = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int status = bind(sid, (struct sockaddr*)&addr, sizeof(addr));
    checkError(status, __LINE__);
    status = listen(sid, 10);
    checkError(status, __LINE__);
    return sid;
}

/* reaps dead children */
void cleanupDeadChildren()
{
    int status = 0;
    pid_t deadChild; // reap the dead children
    do {
        deadChild = waitpid(0, &status, WNOHANG);
        if (deadChild > 0)
            printf("Reaped %d\n", deadChild);
    } while (deadChild > 0);
}

void handle_client(int fd)
{
    if (fork() == 0) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);
        execlp("sqlite3", "sqlite3", "foobar.db", NULL);
    }
    close(fd);
    puts("dispatched client");
}

int main(int argc, char* argv[])
{
    /* Create two sockets: one to receive SQL commands (port 9000) and another
     * for service (port 9001)*/
    int sid = bindAndListen(9000);
    int srv = bindAndListen(9001);

    struct pollfd pfds[2] = {{0}, {0}};

    pfds[0].fd = sid;
    pfds[0].events = POLLIN;
    pfds[1].fd = srv;
    pfds[1].events = POLLIN;

    while (1) {
        /* TODO: Q2.3
         *  Listen for input on both sockets.
         *  If a client connects to port 9000, run a unique instance of SQLite
         * to respond to SQLite queries from the client. Reap dead children
         * using cleanupDeadChildren. The executable for SQLite is sqlite3 that
         * takes as argument a database foobar.db. Note that if you are coding
         * this on your localhosts, you may need to install SQLite using 'sudo
         * apt install sqlite3'. If a client connects to port 9001, compare the
         * received string with '$die!'. If it matches, terminate the server
         * process.
         */
        cleanupDeadChildren();
        int rc = poll(pfds, sizeof(pfds) / sizeof(struct pollfd), 1000);
        if (rc == 0)
            continue;
        if (rc < 0) {
            int ec = errno;
            if (ec == EAGAIN || ec == EINTR) {
                continue;
            }
            else {
                break;
            }
        }
        if (pfds[0].revents & POLLIN) {
            // 9000
            int conn = accept(sid, NULL, NULL);
            printf("accepted a socket: %d\n", conn);
            handle_client(conn);
        }
        if (pfds[1].revents & POLLIN) {
            // 9001
            int conn = accept(srv, NULL, NULL);
            char inbuf[256];
            ssize_t nbytes =
                checkError(recv(conn, inbuf, sizeof(inbuf) - 1, 0), __LINE__);
            inbuf[nbytes] = '\0';
            if (strncmp("\\$die!", inbuf, sizeof(inbuf)) == 0) {
                close(conn);
                break;
            }
            printf("bad command [%s]\n", inbuf);
            close(conn);
        }
    }
    close(srv);
    close(sid);
    cleanupDeadChildren();
    printf("terminated...\n");
    return 0;
}

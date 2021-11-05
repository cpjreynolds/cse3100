#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/* readString reads input from the stdin and returns the \n\0 terminated string
 * If EOF is read, it returns NULL */
char* readString()
{
    int l = 2; // start with enough space to hold the emtpy string (with \n).
    char* s = malloc(l * sizeof(char));
    int i = 0;
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (i == l - 2) {
            s = realloc(s, l * 2 * sizeof(char));
            l *= 2;
        }
        s[i++] = ch;
    }
    if (ch == EOF) {
        free(s);
        return NULL;
    }
    else {
        s[i] = ch;
        s[i + 1] = 0;
        return s;
    }
}

/* prints the error that is encountered and terminate the program */
void checkError(int status, int line)
{
    if (status < 0) {
        printf("socket error(%d)-%d: [%s]\n", getpid(), line, strerror(errno));
        exit(-1);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("usage is: Q2client <hostname>\n");
        return 1;
    }

    /* Connect to the server hosted on <hostname> (read as command line
     * argument) on port 9000. */
    char* hostname = argv[1];
    int sid = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in srv;
    struct hostent* server = gethostbyname(hostname);
    srv.sin_family = AF_INET;
    srv.sin_port = htons(9000);
    memcpy(&srv.sin_addr.s_addr, server->h_addr, server->h_length);
    int status = connect(sid, (struct sockaddr*)&srv, sizeof(srv));
    checkError(status, __LINE__);

    // assert(fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) != -1);

    struct pollfd pfds[2];

    pfds[0].fd = sid;
    pfds[0].events = POLLIN;

    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;

    // if there is a god, I hope they forgive me for writing this code.
    while (1) {
        int rc = poll(pfds, sizeof(pfds) / sizeof(struct pollfd), -1);
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
        if (pfds[1].revents & POLLIN) {
            // have cmd on stdin
            char* instr = readString();
            if (!instr) {
                pfds[1].events = 0;
                instr = ".exit\n";
            }
            size_t inlen = strlen(instr);
            /*
             * validation
             * if (inlen < 2 || (instr[0] != '.' && instr[inlen - 1] != ';')) {
             *     instr = "\n";
             *     inlen = 1;
             * }
             */
            // send it
            ssize_t tx_bytes = 0;
            while (tx_bytes < inlen) {
                ssize_t rc = send(sid, instr, inlen - tx_bytes, 0);
                if (rc < 0) {
                    perror("Q2client: send");
                    exit(1);
                }
                else {
                    tx_bytes += rc;
                }
            }
            free(instr);
        }

        if (pfds[0].revents & POLLIN) {
            ssize_t rx_bytes = 0;
            char rxbuf[64];

            do {
                rx_bytes = recv(sid, rxbuf, sizeof(rxbuf), 0);
                if (rx_bytes < 0) {
                    if (errno == EAGAIN)
                        break;
                    perror("Q2client: recv");
                    exit(1);
                }
                else if (rx_bytes == 0) {
                    // yOu ShOuLdNt UsE gOtO
                    goto end;
                }
                else {
                    write(STDOUT_FILENO, rxbuf, rx_bytes);
                }
            } while (rx_bytes == sizeof(rxbuf));
        }
    }
end:
    return 0;
}

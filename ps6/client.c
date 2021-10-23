#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "command.h"
static ssize_t checkError(ssize_t status, int line)
{
    if (status < 0) {
        printf("socket error(%d)-%d: [%s]\n", getpid(), line, strerror(errno));
        exit(-1);
    }
    else {
        return status;
    }
}

void doLSCommand(int sid);
void doExitCommand(int sid);
void doGETCommand(int sid);
void doPUTCommand(int sid);

int main(int argc, char* argv[])
{
    // Create a socket
    int sid = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in srv;
    struct hostent* server = gethostbyname("localhost");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(8080);
    memcpy(&srv.sin_addr.s_addr, server->h_addr, server->h_length);
    int status = connect(sid, (struct sockaddr*)&srv, sizeof(srv));
    checkError(status, __LINE__);
    int done = 0;
    do {
        char opcode[32];
        scanf("%s", opcode);
        if (strncmp(opcode, "ls", 2) == 0) {
            doLSCommand(sid);
        }
        else if (strncmp(opcode, "get", 3) == 0) {
            doGETCommand(sid);
        }
        else if (strncmp(opcode, "put", 3) == 0) {
            doPUTCommand(sid);
        }
        else if (strncmp(opcode, "exit", 4) == 0) {
            doExitCommand(sid);
            done = 1;
        }
    } while (!done);

    return 0;
}

// eat an entire message on the socket.
#define SLURP(sd, dst)                                                         \
    {                                                                          \
        ssize_t nb = 0;                                                        \
        do {                                                                   \
            nb += checkError(recv(sd, &dst, sizeof(dst), 0), __LINE__);        \
        } while (sizeof(dst) - nb > 0);                                        \
    }

// barf a message out onto the socket.
#define GLORP(sd, src)                                                         \
    {                                                                          \
        ssize_t nb = 0;                                                        \
        do {                                                                   \
            nb += checkError(send(sd, &src, sizeof(src), 0), __LINE__);        \
        } while (sizeof(src) - nb > 0);                                        \
    }

// read a payload and message.
static inline Payload get_payload(int fd, char** buf)
{
    Payload p;
    SLURP(fd, p);

    p.code = ntohl(p.code);
    p.length = ntohl(p.length);

    *buf = calloc(p.length, 1);

    ssize_t nb = 0;
    do {
        nb += checkError(recv(fd, *buf, p.length, 0), __LINE__);
    } while (p.length - nb > 0);

    return p;
}

void doLSCommand(int sid)
{
    /*
      TODO: Send Command c to the server. Then, use recv to read the
      response of the server into Payload p. Following this payload, the
      actual actual message will be sent to the client by the server.
      Payload p has a length field, which is the length of this message.

      You should store the list of files in buf.
     */

    Command c = {0}; // THIS is how to memset a struct on the stack.
    Payload p;
    char* buf;
    c.code = htonl(CC_LS);
    GLORP(sid, c);

    p = get_payload(sid, &buf);

    if (p.code == PL_TXT)
        printf("Got: [\n%s]\n", buf);
    else {
        printf("Unexpected payload: %d\n", p.code);
    }
    free(buf);
}

void doGETCommand(int sid)
{
    Command c = {0};
    Payload p;
    printf("Give a filename:");
    // char fName[256];
    scanf("%s", c.arg);
    c.code = htonl(CC_GET);
    GLORP(sid, c);

    SLURP(sid, p);
    p.code = ntohl(p.code);
    p.length = ntohl(p.length);

    /*
      TODO: Prepare Command c, send it, and recv back into Payload p.


      Something that is important to note: payload p contains the code and
      length as integers in network byte order. You need to use the proper
      function to convert p.length to host byte order. What function would
      you use for this? It's either htonl, htons, ntohl, or ntohs.
     */

    receiveFileOverSocket(sid, c.arg, ".download", p.length);
    printf("transfer done\n");
}

void doPUTCommand(int sid)
{
    Command c = {0};
    Payload p = {0};
    printf("Give a local filename:");
    // char fName[256];
    scanf("%s", c.arg);
    c.code = htonl(CC_PUT);
    GLORP(sid, c);
    p.code = ntohl(PL_FILE);
    p.length = ntohl(getFileSize(c.arg));
    GLORP(sid, p);
    /*
      TODO: Prepare Command c, and send it to the server. Then, prepare the
      Payload p, and send it to the server.

      Be sure c.code, p.length are in network byte order.
     */
    sendFileOverSocket(c.arg, sid);
    printf("transfer done\n");
}

void doExitCommand(int sid)
{
    Command c;
    Payload p;
    int status;
    c.code = htonl(CC_EXIT);
    memset(c.arg, 0, sizeof(c.arg));
    status = send(sid, &c, sizeof(c), 0);
    checkError(status, __LINE__);
}

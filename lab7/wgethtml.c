#include "analyze.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int checkError(int status, int line)
{
    if (status < 0) {
        printf("socket error(%d)-%d: [%s]\n", getpid(), line, strerror(errno));
        exit(-1);
    }
    else {
        return status;
    }
}

char* readResponse(int sid)
{
    size_t sz = 8;
    char* buf = malloc(sizeof(char) * sz);
    int done = 0, received = 0;
    while (!done) {
        int nbb =
            checkError(read(sid, buf + received, sz - received), __LINE__);
        if (nbb > 0)
            received += nbb;
        else if (nbb == 0)
            done = 1;
        if (received == sz) {
            buf = realloc(buf, sz * 2);
            sz *= 2;
        }
    }
    if (received == sz) {
        buf = realloc(buf, sz + 1);
    }
    buf[received] = 0;
    return buf;
}

int main(int argc, char* argv[])
{
    // Create a socket
    if (argc < 2) {
        printf("Usage is: wgethtml <url>\n");
        return 1;
    }
    char host[512];
    int port = 80;
    char url[512];
    analyzeURL(argv[1], host, &port, url);
    printf("[%s] [%d] [%s]\n", host, port, url);

    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* res;
    char port_s[16];
    sprintf(port_s, "%d", port);
    int ret = getaddrinfo(host, port_s, &hints, &res);
    if (ret) {
        printf("%s\n", gai_strerror(ret));
        exit(-1);
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    checkError(sockfd, __LINE__);

    char addr[INET_ADDRSTRLEN];
    struct sockaddr_in* sadr = (struct sockaddr_in*)res->ai_addr;
    fprintf(stderr, "%s:%hu\n",
            inet_ntop(AF_INET, &sadr->sin_addr, addr, res->ai_addrlen),
            ntohs(sadr->sin_port));
    checkError(connect(sockfd, res->ai_addr, res->ai_addrlen), __LINE__);

    char get_msg[1024];
    snprintf(get_msg, 1024, "GET %s\n", url);

    ssize_t nb = send(sockfd, get_msg, strlen(get_msg), 0);
    assert(nb == strlen(get_msg));

    char* resp = readResponse(sockfd);
    printf("%s\n", resp);

    return 0;
}

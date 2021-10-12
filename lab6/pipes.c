#define BUFFER_SIZE 20
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// will eat your socks if message is not null-terminated.
void write_message(char* message, int fd)
{
    // Write message to fd
    while (*message) {
        write(fd, message++, 1);
    }
}

char* read_message(int fd)
{
    size_t bcap = BUFFER_SIZE;
    char* buf = calloc(bcap, 1);
    char* cursor = buf;
    // Read from fd until you hit a newline.
    do {
        if (read(fd, cursor, 1) != 1) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        if (cursor == buf + bcap - 2) {
            buf = realloc(buf, bcap * 2); // invalidates cursor.
            cursor = buf + bcap - 2;      // cursor valid again.
            bcap *= 2;
        }
    } while (*cursor++ != '\n');
    *cursor = '\0';
    return buf;
}

void cs_encrypt(char* message)
{
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == '\n')
            break;
        message[i] += 1;
    }
}

void cs_decrypt(char* message)
{
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] == '\n')
            break;
        message[i] -= 1;
    }
}

int main()
{
    // Set up pipes
    int p_to_c1[2], c1_to_c2[2], c2_to_p[2];
    // TODO: Create Pipes
    pipe(p_to_c1);
    pipe(c1_to_c2);
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child 1
        char* msg;
        // TODO: Close Unneeded Pipes
        close(p_to_c1[1]);
        close(c1_to_c2[0]);

        msg = read_message(p_to_c1[0]);

        printf("Child 1 is Encrypting!\n");
        cs_encrypt(msg);

        write_message(msg, c1_to_c2[1]);
        close(p_to_c1[0]);
        close(c1_to_c2[1]);
        exit(0);
    }
    else {
        pipe(c2_to_p);
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Child 2
            char* msg;
            // TODO: Close Unneeded Pipes
            close(p_to_c1[0]);
            close(p_to_c1[1]);
            close(c1_to_c2[1]);
            close(c2_to_p[0]);

            msg = read_message(c1_to_c2[0]);
            close(c1_to_c2[0]);
            printf("Child 2: %s", msg);

            write_message(msg, c2_to_p[1]);
            close(c2_to_p[1]);
            exit(0);
        }
        else {
            // Parent
            char* msg = "This is a test.\n";
            // TODO: Close Unneeded Pipes
            close(p_to_c1[0]);
            close(c2_to_p[1]);

            printf("Parent: %s", msg);
            write_message(msg, p_to_c1[1]);
            close(p_to_c1[1]);

            msg = read_message(c2_to_p[0]);
            close(c2_to_p[0]);

            cs_decrypt(msg);
            printf("Parent: %s", msg);
            close(c1_to_c2[1]);
            close(c1_to_c2[0]);
            free(msg);
            return 0;
        }
    }
}

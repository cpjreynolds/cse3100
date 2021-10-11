#define BUFFER_SIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
void write_message(char * message, int fd) {
  // Write message to fd
}
char* read_message(int fd) {
  // Read from fd until you hit a newline. 
}

void encrypt(char * message) {
  for(int i=0;i<strlen(message);i++) {
    if(message[i] == '\n') break;
    message[i] += 1;
  }
 
}
void decrypt(char * message) {
  for(int i=0;i<strlen(message);i++) {
    if(message[i] == '\n') break;
    message[i] -= 1;
  }
}



int main() {
  // Set up pipes
  int pTOc1[2], c1TOc2[2], c2TOp[2];
  // Create Pipes
  pid_t pid1 = fork();
  if (pid1 == 0) {
    // Child 1
    char* msg;
    // TODO: Close Unneeded Pipes
    msg = read_message(pTOc1[0]);
    printf("Child 1 is Encrypting!\n");
    encrypt(msg);
    write_message(msg,c1TOc2[1]);
    // TODO: Close Remaining Pipes
    exit(0);
  } else {
    pid_t pid2 = fork();
    if (pid2 ==0) {
      // Child 2
      char* msg;
      // TODO: Close Unneeded Pipes
      msg = read_message(c1TOc2[0]);
      printf("Child 2: %s", msg);
      write_message(msg, c2TOp[1]);
      // TODO: Close Remaining Pipes
      exit(0);
    } else {
      // Parent
      char * msg = "This is a test.\n";
      //TODO: Close Unneeded Pipes
      printf("Parent: %s", msg);
      write_message(msg,pTOc1[1]);
      msg = read_message(c2TOp[0]);
      decrypt(msg);
      printf("Parent: %s", msg);
      // TODO: Close Remaining Pipes and free the msg
      return 0;
    }
    
  }
}

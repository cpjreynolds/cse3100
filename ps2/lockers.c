#include <stdio.h>

char lockerState(int l, int t)
{

  /*
    TODO: This should compute the state of locker l after t students have done their toggling. If the locker is open, return 1. If the locker is closed, return 0.
   */
}



int main(int argc, char* argv[])
{
  int locker;
  while(1){
    printf("Enter locker number: ");
    scanf("%d", &locker);
    if(locker < 0){
      break;
    }else if(lockerState(locker, locker) == 0){
      printf("Closed\n");
    }else{
      printf("Open\n");
    }
  }
  return 0;
}

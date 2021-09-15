#include "calendar.h"
#include <stdio.h>


int main(){
  int month, year;
  do{
    printf("Enter month (1 = jan,..., 12 = dec): ");
    scanf("%d", &month);
    if(month > 0){      
      printf("Enter year: ");
      scanf("%d", &year);
      printf("Number of days in month %d of year %d: %d\n", month, year, numDaysInMonth(month, year));
    }
  }while(month > 0);
}

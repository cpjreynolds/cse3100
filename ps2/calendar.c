#include "calendar.h"
#include <stdio.h>

int feb_days(int year) {
    if ((year % 100 && !(year % 4)) || (!(year % 100) && !(year % 400))) {
        return 29;
    } else {
        return 28;
    }
}

int numDaysInMonth(int month, int year) {
    /*

     */
    switch (month) {
    case JAN:
    case MAR:
    case MAY:
    case JUL:
    case AUG:
    case OCT:
    case DEC:
        return 31;
        /*
         TODO: Add the cases for February through December. Don't forget about
         leap years. For years that are not centuries (i.e. not 1900, 2000), it
         is a leap year if it is disible by 4. For years that are centuries, it
         is a leap year if it is divisible by 400. Return -1 if the month
         doesn't fit any of the cases.
        */
    case APR:
    case JUN:
    case SEP:
    case NOV:
        return 30;
    case FEB:
        return feb_days(year);
    default:
        return -1;
    }
}

int numFirstMondays(int startYear, int endYear, int janFirstWeekday) {
    /*
      TODO: Add a loop here to compute the number of mondays that fall on the
      first of the month between January 1st of startYear and dec 31st of
      endYear. Note that janFirstWeekday is the weekday of january 1st of
      startYear.
     */
    return 0;
}

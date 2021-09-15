#ifndef __CALENDAR_H
#define __CALENDAR_H

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

/*
#define JAN 1
#define DEC 12
*/
// Or.....
enum {
    JAN = 1,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC,
};

int numDaysInMonth(int month, int year);
int numFirstMondays(int startYear, int endYear, int janFirstWeekday);

#endif

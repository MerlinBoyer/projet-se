#ifndef TIME_H
#define TIME_H

struct Time {
  int hours;
  int minutes;
  int seconds;
};

/*
Init the timer 1 and set up an interruption
 */
void init_time(struct Time t);
/*
Return the current time in a structure
 */
struct Time get_time();
/*
Return time in a string
 */
void get_time_str(char * t);
/*
set time
*/
void set_time(struct Time t);

#endif

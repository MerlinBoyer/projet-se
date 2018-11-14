#ifndef TIME_H
#define TIME_H

struct Time {
  int hours;
  int minutes;
  int seconds;
};
  
void init_time(struct Time t);
struct Time get_time();
void get_time_str(char * t);

#endif

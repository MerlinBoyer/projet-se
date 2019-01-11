#ifndef MONITOR_H
#define MONITOR_H

/*
Init the monitor. Init the timer 0, the sensor hall effect and the interruptions.
 */
void init_monitor();

/*
Get the current angle of the board
 */
int get_current_angle();

#endif

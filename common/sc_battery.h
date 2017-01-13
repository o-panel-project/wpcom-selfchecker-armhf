/*
 * battery.h
 */

#ifndef __BATTERY_H__
#define __BATTERY_H__

#define DC_RL	100
#define DC_RH	750
#define BAT_RL	100
#define BAT_RH	510

int sc_get_battery_status(
		char *log_prefix, int *dc_level,
		int *bat1_level,  int *bat1_stat,
		int *bat2_level,  int *bat2_stat);
int sc_get_cradle_battery_level();
#define	CRADLE_BAT_OPEN_ERROR	(-1)
#define	CRADLE_BAT_MODE_ERROR	(-2)

#endif

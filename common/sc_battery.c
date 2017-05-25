#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "common.h"
#include "wpcio.h"
#include "sc_battery.h"

int sc_get_battery_status(
		char *log_prefix, int *dc_level,
		int *bat1_level,  int *bat1_stat,
		int *bat2_level,  int *bat2_stat)
{
	int dc, bat1, bat2;
	int fd, res;

	if (dc_level != NULL)
		*dc_level = 0; 
	if (bat1_level != NULL)
		*bat1_level = 0; 
	if (bat2_level != NULL)
		*bat2_level = 0; 
	if (bat1_stat != NULL)
		*bat1_stat = 0; 
	if (bat2_stat != NULL)
		*bat2_stat = 0; 

	fd = wpcio_open(WPCIO_OPEN_RETRY, log_prefix);
	if (fd < 0) return -1;

	/* DC level */
	if (dc_level != NULL) {
		res = ioctl(fd, WPC_GET_DC_LEVEL, &dc);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get adc value(0), error code = %d\n", res);
			*dc_level = -1;
		} else
			*dc_level = dc * (DC_RL + DC_RH) / DC_RL;
	}
	/* Battery1 */
	if (bat1_level != NULL) {
		res = ioctl(fd, WPC_GET_BAT1_LEVEL, &bat1);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get adc value(1), error code = %d\n", res);
			*bat1_level = -1;
		} else
			*bat1_level = bat1 * (BAT_RL + BAT_RH) / BAT_RL;
	}
	if (bat1_stat != NULL) {
		res = ioctl(fd, WPC_GET_BAT1_CHARGING_STAT, bat1_stat);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get bat1 charging stat, error code = %d\n", res);
			*bat1_stat = -1;
		}
	}
	/* Battery2 */
	if (bat2_level != NULL) {
		res = ioctl(fd, WPC_GET_BAT2_LEVEL, &bat2);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get adc value(2), error code = %d\n", res);
			*bat2_level = -1;
		} else
			*bat2_level = bat2 * (BAT_RL + BAT_RH) / BAT_RL;
	}
	if (bat2_stat != NULL) {
		res = ioctl(fd, WPC_GET_BAT2_CHARGING_STAT, bat2_stat);
		if (res < 0) {
			printf("Could not get bat2 charging stat, error code = %d\n", res);
			*bat2_stat = -1;
		}
	}

	close(fd);
	usleep(WPCIO_CLOSE_GUARD);

	return 0;
}

/*
 * get BatteryCradle level
 * 20170113 wpc
 */
#include <endian.h>
#include "sc_i2c.h"
#define mV_CONVERT_FACTOR   (float)(433.0/103.0)
#define AV_ITEM_COUNT       5
int sc_get_cradle_battery_level(int machine_type)
{
	int fd;
	int conf_reg, conv_reg, adc_val, res;
	float vol;
	int i;
	int bus_no;

	if (machine_type == WPC_BOARD_TYPE_O)
		bus_no = I2C_BUS3;
	else
		bus_no = I2C_BUS2;
	fd = sc_i2c_open(bus_no, I2C_CRADLE_ADC_ADDR);
	if (fd < 0) {
		//printf("%s: i2c-%d, 0x%02x open error\n",
		//		__func__, I2C_BUS2, I2C_CRADLE_ADC_ADDR);
		return CRADLE_BAT_OPEN_ERROR;
	}

#if 0
	/* Config register 0x1 MODE one shot */
	conf_reg = sc_i2c_read(fd, 0x1, I2C_SMBUS_WORD_DATA);
	if (conf_reg < 0) {
		close(fd);
		return -1;
	}
	conf_reg |= 0x8100; /* oneshot mode */
#endif
	conf_reg = be16toh(0x8483); /* OS:1 MODE:0(Continuous-conv) */
	res = sc_i2c_write(fd, 0x1, conf_reg, I2C_SMBUS_WORD_DATA);
	if (res < 0) {
		//printf("%s: Conversion mode write error\n", __func__);
		close(fd);
		return CRADLE_BAT_MODE_ERROR;
	}

	float sum = 0.0, av;
	int min_val = 0x0fff;
	int max_val = 0;
	for (i = 0; i < AV_ITEM_COUNT; i++) {
		usleep(I2C_GUARDTIME_USEC);

		/* Conversion register 0x0 */
		conv_reg = sc_i2c_read(fd, 0x0, I2C_SMBUS_WORD_DATA);
		if (conv_reg < 0) {
			conv_reg = 0;
		}
		adc_val = be16toh(conv_reg) >> 4;   /* upper 12bit */
		if (max_val < adc_val)
			max_val = adc_val;
		if (min_val >= adc_val)
			min_val = adc_val;
		//printf("%s: Conversion reg = %d\n", __func__, adc_val);
		sum += adc_val;
	}
	close(fd);

	/* Conversion */
	av = (sum - max_val - min_val) / (AV_ITEM_COUNT - 2);
	vol = av * mV_CONVERT_FACTOR;

	return (int)vol;    /* mV */
}

int sc_get_battery_status_o(
		char *log_prefix, int *dc_level,
		int *bat1_level,  int *bat1_stat,
		int *bat2_level,  int *bat2_stat)
{
	int dc, bat1;
	float dcf, bat1f;
	int fd, res;

	if (dc_level != NULL)
		*dc_level = 0; 
	if (bat1_level != NULL)
		*bat1_level = 0; 
	if (bat2_level != NULL)
		*bat2_level = 0; 
	if (bat1_stat != NULL)
		*bat1_stat = 0; 
	if (bat2_stat != NULL)
		*bat2_stat = 0; 

	fd = wpcio_open(WPCIO_OPEN_RETRY, log_prefix);
	if (fd < 0) return -1;

	/* DC level */
	if (dc_level != NULL) {
		res = ioctl(fd, WPC_GET_DC_LEVEL, &dc);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get adc value(0), error code = %d\n", res);
			*dc_level = -1;
		} else {
			dcf = (float)dc * ((DC_RL_O + DC_RH_O) / DC_RL_O) * FACTOR_O;
			*dc_level = (int)dcf;
//			printf("dc_level = %d\n", *dc_level);
		}
	}
	/* Battery1 */
	if (bat1_level != NULL) {
		res = ioctl(fd, WPC_GET_BAT1_LEVEL, &bat1);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get adc value(1), error code = %d\n", res);
			*bat1_level = -1;
		} else {
			bat1f = (float)bat1 * ((BAT_RL_O + BAT_RH_O) / BAT_RL_O) * FACTOR_O;
			*bat1_level = (int)bat1f;
//			printf("bat1_level = %d\n", *bat1_level);
		}
	}
	if (bat1_stat != NULL) {
		res = ioctl(fd, WPC_GET_BAT1_CHARGING_STAT, bat1_stat);
		usleep(WPCIO_IOCTL_GUARD);
		if (res < 0) {
			printf("Could not get bat1 charging stat, error code = %d\n", res);
			*bat1_stat = -1;
		}
	}
	/* Battery2 */
	*bat2_level = 0;
	*bat2_stat = 0;

	close(fd);
	usleep(WPCIO_CLOSE_GUARD);

	return 0;
}

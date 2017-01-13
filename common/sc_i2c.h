#include "linux/i2c-dev.h"

#define	I2C_BUS1	1
#define	I2C_BUS2	2
#define I2C_DEVICE_BASE	"/dev/i2c-"
#define I2C_CRADLE_ADC_ADDR	0x48
#define	I2C_CRADLE_ROM_ADDR	0x55
#define I2C_DATA_SIZE	4
#define I2C_GUARDTIME_USEC	10000

int sc_i2c_open(int bus_no, const unsigned char addrs);
int sc_i2c_check_open(int *fd);
int sc_i2c_read(int fd, int daddress, int mode);
int sc_i2c_write(int fd, int daddress, int value, int mode);
int sc_i2c_read_cradle(unsigned char *buf);
int sc_i2c_write_cradle(unsigned char *buf);

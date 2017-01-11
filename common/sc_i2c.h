#include "linux/i2c-dev.h"

#define CRADLE_ADC_ADDR	0x48
#define	CRADLE_ROM_ADDR	0x55
#define I2C_DEVICE_BASE	"/dev/i2c-"
#define I2C_DATA_SIZE	4
#define I2C_GUARDTIME_USEC	10000

int sc_i2c_open(int bus_no, const unsigned char addrs);
int sc_i2c_check_open(int *fd);
int sc_i2c_read(int fd, int daddress, int mode);

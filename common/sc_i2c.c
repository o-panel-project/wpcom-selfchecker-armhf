/*
 * I2C functions
 *    move from common.c
 * 20170111 wpc
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "sc_i2c.h"
#include "common.h"

int sc_i2c_open(int bus_no, const unsigned char addrs)
{
	char devpath[255];
	int fd;

	sprintf(devpath, I2C_DEVICE_BASE"%d", bus_no);
	fd = open(devpath, O_RDWR);
	if (fd < 0) return -errno;

	if (ioctl(fd, I2C_SLAVE, addrs) < 0) {
		debug_printf(3, "Error: Could not set address : %s\n", strerror(errno));
		return -errno;
	}

	return fd;
}

int sc_i2c_check_open(int *fd)
{
	int bus_no = 2;

	if(0<*fd) return *fd;

	if (sc_IsJ4())
		bus_no = 1;

	*fd = sc_i2c_open(bus_no, CRADLE_ROM_ADDR);
	if(*fd<0) return *fd;

	return 0;
}

int sc_i2c_read(int fd, int daddress, int mode)
{
	int res;

	switch (mode) {
	case I2C_SMBUS_BYTE_DATA:
		res = i2c_smbus_read_byte_data(fd, daddress);
		break;
	case I2C_SMBUS_WORD_DATA:
		res = i2c_smbus_read_word_data(fd, daddress);
		break;
	default:
		res = -1;
		break;
	}
	return res;
}

int sc_i2c_write(int fd, int daddress, int value, int mode)
{
	int res;

	switch (mode) {
	case I2C_SMBUS_BYTE_DATA:
		res = i2c_smbus_write_byte_data(fd, daddress, value);
		break;
	case I2C_SMBUS_WORD_DATA:
		res = i2c_smbus_write_word_data(fd, daddress, value);
		break;
	default:
		res = -1;
		break;
	}
	return res;
}

//
//  Cyclic selectin of write data
//

static char *write_data[]={ "aaaa", "bbbb", "1234", "1a2b", "cccc" };
static int write_data_pointer=0;

static char *sc_i2c_pick_write_data()
{
	write_data_pointer++;
	if(4<write_data_pointer) write_data_pointer=0;
	return write_data[write_data_pointer];
}

// i2c read
// 0 on success, 1 on open error, 2 on ioctl failure
int sc_i2c_read_cradle(unsigned char *buf)
{
	int fd=0, i, r;

	bzero(buf, I2C_DATA_SIZE);

	if(sc_i2c_check_open(&fd)<0)
		return 1;

	for(i=0;i<I2C_DATA_SIZE;i++){
		usleep(I2C_GUARDTIME_USEC);
		r = sc_i2c_read(fd, i, I2C_SMBUS_BYTE_DATA);
		if(r<0){
			close(fd);
			return 2;
		}
		buf[i]=r;
	}
	close(fd);
	return 0;
}

// i2c write
// 0 on success, 1 on open error, 2 on ioctl failure
int sc_i2c_write_cradle(unsigned char *buf)
{
	int fd=0, i;
	char *data;

	if(sc_i2c_check_open(&fd)<0)
		return 1;

	data=sc_i2c_pick_write_data();
	memcpy(buf, data, I2C_DATA_SIZE);

	for(i=0;i<I2C_DATA_SIZE;i++){
		usleep(I2C_GUARDTIME_USEC);
		if(sc_i2c_write(fd, i, buf[i], I2C_SMBUS_BYTE_DATA)<0){
			close(fd);
			return 2;
		}
	}
	close(fd);
	return 0;
}


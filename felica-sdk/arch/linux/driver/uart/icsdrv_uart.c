/**
 * \brief    UART Driver (Linux)
 * \date     2008/10/14
 * \author   Copyright 2005,2006,2007,2008 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "DSB"

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"

#include "icsdrv_uart.h"

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/select.h>
#include <termios.h>

/* --------------------------------
 * Prototype Declaration
 * -------------------------------- */

static speed_t icsdrv_uart_convert_to_dev_speed(
    UINT32 speed);

/* --------------------------------
 * Function
 * -------------------------------- */

/* ------------------------
 * Exported
 * ------------------------ */

/**
 * This function opens a port to the device.
 *
 * \param  handle                [OUT] Handle to access the port.
 * \param  port_name              [IN] The port name to open.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_BUSY              Device busy.
 * \retval ICS_ERROR_PERMISSION        Permission denied.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_open(
    ICS_HANDLE* handle,
    const char* port_name)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_open"
    int fd;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(handle, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(port_name, NULL, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_STR(port_name);

    /* open the device */
    fd = open(port_name, O_RDWR);
    if (fd == -1) {
        ICSLOG_ERR_STR(errno, "open()");
        switch (errno) {
        case EBUSY:
            return ICS_ERROR_BUSY;
        case EACCES:
            return ICS_ERROR_PERMISSION;
        default:
            return ICS_ERROR_IO;
        }
    }

    *handle = fd;
    ICSLOG_DBG_INT(*handle);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function closes the port.
 *
 * \param  handle                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_close(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_close"
    int res;
    int fd;
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_INT(handle);

    fd = handle;

    res = close(fd);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "close()");
        return ICS_ERROR_IO;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function writes data to the device.
 *
 * \param  handle                 [IN] The handle to access the port.
 * \param  data                   [IN] A data to write.
 * \param  data_len               [IN] The length of the data.
 * \param  time0                  [IN] The base time for time-out. (ms)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_write(
    ICS_HANDLE handle,
    const UINT8* data,
    UINT32 data_len,
    UINT32 time0,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_write"
    int res;
    int fd;
    fd_set writefds;
    struct timeval tm;
    UINT32 nwritten;
    UINT32 current_time;
    UINT32 rest_timeout;
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_IN_RANGE(handle, 0, FD_SETSIZE - 1, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(data, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_LE(data_len, 0xffff, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_INT(handle);
    ICSLOG_DBG_UINT(data_len);
    ICSLOG_DUMP(data, data_len);
    ICSLOG_DBG_UINT(time0);
    ICSLOG_DBG_UINT(timeout);

    fd = handle;

    nwritten = 0;
    while (nwritten < data_len) {
        rest_timeout = utl_get_rest_timeout(time0, timeout, &current_time);

        FD_ZERO(&writefds);
        FD_SET(fd, &writefds);
        tm.tv_sec = (rest_timeout / 1000);
        tm.tv_usec = ((rest_timeout % 1000) * 1000);

        res = select(fd + 1, NULL, &writefds, NULL, &tm);
        if ((res == -1) && (errno != EINTR)) {
            ICSLOG_ERR_STR(errno, "select()");
            return ICS_ERROR_IO;
        } else if (res == 0) {
            rc = ICS_ERROR_TIMEOUT;
            ICSLOG_ERR_STR(rc, "Timeout at writing.");
            return rc;
        } else if (res == 1) {
            res = write(fd, data + nwritten, data_len - nwritten);
            if (res == -1) {
                ICSLOG_ERR_STR(errno, "write()");
                return ICS_ERROR_IO;
            }
            nwritten += res;
        } else {
            rc = ICS_ERROR_IO;
            ICSLOG_ERR_STR(rc, "Unknown error.");
            return rc;
        }
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function reads data from the device.
 *
 * \param  handle                 [IN] The handle to access the port.
 * \param  min_read_len           [IN] The minimum length of read data.
 * \param  max_read_len           [IN] The maximum length of read data.
 * \param  data                  [OUT] Read data.
 * \param  read_len              [OUT] The length of read data or NULL.
 * \param  time0                  [IN] The base time for time-out. (ms)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_read(
    ICS_HANDLE handle,
    UINT32 min_read_len,
    UINT32 max_read_len,
    UINT8* data,
    UINT32* read_len,
    UINT32 time0,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_read"
    int res;
    int fd;
    fd_set readfds;
    struct timeval tm;
    UINT32 nread;
    UINT32 current_time;
    UINT32 rest_timeout;
    UINT32 rc;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_IN_RANGE(handle, 0, FD_SETSIZE - 1, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_NE(data, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_LE(min_read_len, max_read_len, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_INT(handle);
    ICSLOG_DBG_UINT(max_read_len);
    ICSLOG_DBG_UINT(time0);
    ICSLOG_DBG_UINT(timeout);

    fd = handle;

    nread = 0;
    do {
        rest_timeout = utl_get_rest_timeout(time0, timeout, &current_time);

        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        tm.tv_sec = (rest_timeout / 1000);
        tm.tv_usec = ((rest_timeout % 1000) * 1000);

        res = select(fd + 1, &readfds, NULL, NULL, &tm);
        if ((res == -1) && (errno != EINTR)) {
            ICSLOG_ERR_STR(errno, "select()");
            return ICS_ERROR_IO;
        } else if (res == 0) {
            rc = ICS_ERROR_TIMEOUT;
            ICSLOG_ERR_STR(rc, "Timeout at reading.");
            return rc;
        } else if (res == 1) {
            res = read(fd, data + nread, max_read_len - nread);
            if (res == -1) {
                ICSLOG_ERR_STR(errno, "read()");
                return ICS_ERROR_IO;
            }
            nread += res;
        } else {
            rc = ICS_ERROR_IO;
            ICSLOG_ERR_STR(rc, "Unknown error.");
            return rc;
        }
    } while (nread < min_read_len);

    if (read_len != NULL) {
        *read_len = nread;
    }
    ICSLOG_DBG_UINT(nread);
    ICSLOG_DUMP(data, nread);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function sets the communication speed of the port.
 *
 * \param  handle                 [IN] The handle to access the port.
 * \param  speed                  [IN] The communication speed to set.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid argument.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_set_speed(
    ICS_HANDLE handle,
    UINT32 speed)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_set_speed"
    int res;
    UINT32 rc;
    int fd;
    struct termios termios_attr;
    speed_t dev_speed;
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_INT(handle);
    ICSLOG_DBG_UINT(speed);

    fd = handle;

    dev_speed = icsdrv_uart_convert_to_dev_speed(speed);
    if (dev_speed == B0) {
        rc = ICS_ERROR_INVALID_PARAM;
        ICSLOG_ERR_STR(rc, "Invalid speed.");
        return rc;
    }

    /* set termios attributions */
    res = tcgetattr(fd, &termios_attr);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "tcgetattr()");
        return ICS_ERROR_IO;
    }

    termios_attr.c_iflag &= ~(BRKINT | ICRNL | IGNCR | INLCR | INPCK |
                              PARMRK | ISTRIP | IXOFF | IXON);
    termios_attr.c_iflag |= (IGNBRK | IGNPAR);
    termios_attr.c_oflag &= ~(OPOST);
    termios_attr.c_cflag &= ~(CSTOPB | HUPCL | PARENB);
    termios_attr.c_cflag |= (CLOCAL | CREAD | CS8);
    termios_attr.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON |
                              IEXTEN | ISIG | TOSTOP);
    termios_attr.c_lflag |= NOFLSH;
    termios_attr.c_cc[VMIN] = 1;
    termios_attr.c_cc[VTIME] = 0;

    res = cfsetispeed(&termios_attr, dev_speed);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "cfsetispeed()");
        return ICS_ERROR_IO;
    }
    res = cfsetospeed(&termios_attr, dev_speed);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "cfsetospeed()");
        return ICS_ERROR_IO;
    }

    res = tcsetattr(fd, TCSANOW, &termios_attr);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "tcsetattr()");
        return ICS_ERROR_IO;
    }

    res = tcflush(fd, TCIOFLUSH);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "tcflush()");
        return ICS_ERROR_IO;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function clears the receiving queue of the UART.
 *
 * \param  handle                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid argument.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_clear_rx_queue(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_clear_rx_queue"
    int res;
    int fd;
    fd_set readfds;
    struct timeval tm;
    char buf[32];
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_IN_RANGE(handle, 0, FD_SETSIZE - 1, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_INT(handle);

    fd = handle;

    do {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        tm.tv_sec = 0;
        tm.tv_usec = 0;

        res = select(fd + 1, &readfds, NULL, NULL, &tm);
        if ((res == -1) && (errno != EINTR)) {
            ICSLOG_ERR_STR(errno, "select()");
            return ICS_ERROR_IO;
        } else if (res == 0) {
            break;
        }
        res = read(fd, buf, sizeof(buf));
        ICSLOG_DUMP(buf, res);
    } while (res > 0);

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function waits until all data written to the UART.
 *
 * \param  handle                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid argument.
 * \retval ICS_ERROR_IO                Other error.
 */
UINT32 icsdrv_uart_drain_tx_queue(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_drain_tx_queue"
    int res;
    int fd;
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_INT(handle);

    fd = handle;

    res = tcdrain(fd);
    if (res == -1) {
        ICSLOG_ERR_STR(errno, "tcdrain()");
        return ICS_ERROR_IO;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/* ------------------------
 * Internal
 * ------------------------ */

/**
 * This function converts speed to speed_t.
 *
 * \param  speed                  [IN] Speed.
 *
 * \return Converted speed.
 */
static speed_t icsdrv_uart_convert_to_dev_speed(
    UINT32 speed)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "icsdrv_uart_convert_to_dev_speed"
    ICSLOG_FUNC_BEGIN;

    switch (speed) {
    case 300:
        return B300;
    case 600:
        return B600;
    case 1200:
        return B1200;
    case 2400:
        return B2400;
    case 4800:
        return B4800;
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    case 460800:
        return B460800;
    case 921600:
        return B921600;
    default:
        break;
    }

    ICSLOG_FUNC_END;
    return B0;
}

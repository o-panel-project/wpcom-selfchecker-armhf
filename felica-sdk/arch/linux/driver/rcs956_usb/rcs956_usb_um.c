/**
 * \brief    RC-S956 USB Driver (Linux)
 * \date     2008/10/14
 * \author   Copyright 2007,2008 Sony Corporation
 */

#undef ICSLOG_MODULE
#define ICSLOG_MODULE "DGb"

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <linux/usbdevice_fs.h>

#include "ics_types.h"
#include "ics_error.h"
#include "icslib_chk.h"
#include "icslog.h"
#include "utl.h"

#include "rcs956_usb.h"

/* --------------------------------
 * Constant
 * -------------------------------- */

#define RCS956_VENDOR_ID        0x054c
#define RCS956_PRODUCT_ID       0x02e1

#define RCS956_USB_ENDPOINT_BULK_OUT            0x04
#define RCS956_USB_ENDPOINT_BULK_IN             0x84
#define RCS956_USB_MAX_BULK_OUT_LEN             64
#define RCS956_USB_MAX_BULK_IN_LEN              64

#define RCS956_USB_COMMAND_BUF_LEN              (8 + 265 + 2)

/* --------------------------------
 * Prototype Declaration
 * -------------------------------- */

/* --------------------------------
 * Function
 * -------------------------------- */

/*
 * Coverts CPU-endian byte order to UINT16
 */
static UINT16 rcs956_cpub_to_uint16(const UINT8 x[2])
{
    UINT16 out;

    utl_memcpy(&out, (const void*)x, 2);

    return out;
}

/* ------------------------
 * Macro
 * ------------------------ */

#define RCS956_LB_TO_UINT16(p)          ((p)[0] | ((p)[1] << 8))

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
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_usb_raw_open(
    ICS_HANDLE* handle,
    const char* port_name)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_open"
    int fd;
    int res;
    UINT32 rc;
    UINT8 desc[18 + 9 + 9 + 7 + 7];
    unsigned int ep;
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
            rc = ICS_ERROR_BUSY;
            break;
        case EACCES:
            rc = ICS_ERROR_PERMISSION;
            break;
        default:
            rc = ICS_ERROR_IO;
            break;
        }
        return rc;
    }

    /* read descriptors */
    res = read(fd, desc, sizeof(desc));
    if (res != sizeof(desc)) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "Cannot read Device Descriptor.");
        return rc;
    }
    ICSLOG_DUMP(desc, sizeof(desc));

    /* check the device descriptor */
    if ((rcs956_cpub_to_uint16(desc + 8) != RCS956_VENDOR_ID) ||
        (rcs956_cpub_to_uint16(desc + 10) != RCS956_PRODUCT_ID)) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "Not RCS956 device.");
        return rc;
    }

    /* check the configuration descritor */
    if (RCS956_LB_TO_UINT16(desc + 20) != 32) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "Too large USB descriptors.");
        return rc;
    }

    /* check the endpoint decriptor */
    if ((desc[38] != RCS956_USB_ENDPOINT_BULK_OUT) ||
        (desc[39] != 0x02)) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "Unknown endpoint descriptor.");
        return rc;
    }
    if ((desc[45] != RCS956_USB_ENDPOINT_BULK_IN) ||
        (desc[46] != 0x02)) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "Unknown endpoint descriptor.");
        return rc;
    }

    /* synchronize ep's toggle bits with the device */
    ep = RCS956_USB_ENDPOINT_BULK_OUT;
    res = ioctl(fd, USBDEVFS_CLEAR_HALT, &ep);
    if (res != 0) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "ioctl(USBDEVFS_CLEAR_HALT, BULK_OUT)");
        return rc;
    }

    ep = RCS956_USB_ENDPOINT_BULK_IN;
    res = ioctl(fd, USBDEVFS_CLEAR_HALT, &ep);
    if (res != 0) {
        close(fd);
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "ioctl(USBDEVFS_CLEAR_HALT, BULK_IN)");
        return rc;
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
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_usb_raw_close(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_close"
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

/*
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
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_usb_raw_write(
    ICS_HANDLE handle,
    const UINT8* data,
    UINT32 data_len,
    UINT32 time0,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_write"
    UINT32 rc;
    int res;
    int fd;
    struct usbdevfs_bulktransfer bulktransfer;
    UINT32 nwritten;
    UINT32 write_len;
    UINT32 current_time;
    UINT32 rest_timeout;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(data, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_LE(data_len, RCS956_USB_COMMAND_BUF_LEN,
                     ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_INT(handle);
    ICSLOG_DBG_UINT(data_len);
    ICSLOG_DUMP(data, data_len);
    ICSLOG_DBG_UINT(time0);
    ICSLOG_DBG_UINT(timeout);

    fd = handle;

    nwritten = 0;
    do {
        rest_timeout = utl_get_rest_timeout(time0, timeout, &current_time);
        if (rest_timeout == 0) {
            rc = ICS_ERROR_TIMEOUT;
            ICSLOG_ERR_STR(rc, "Time-out.");
            return rc;
        }

        write_len = (data_len - nwritten);
        if (write_len > RCS956_USB_MAX_BULK_OUT_LEN) {
            write_len = RCS956_USB_MAX_BULK_OUT_LEN;
        }

        bulktransfer.ep = RCS956_USB_ENDPOINT_BULK_OUT;
        bulktransfer.len = write_len;
        bulktransfer.timeout = rest_timeout;
        bulktransfer.data = (void*)(data + nwritten);

        res = ioctl(fd, USBDEVFS_BULK, &bulktransfer);
        if (res < 0) {
            ICSLOG_ERR_STR(errno, "ioctl(USBDEVFS_BULK)");
            if (errno == ETIMEDOUT) {
                rc = ICS_ERROR_TIMEOUT;
            } else {
                rc = ICS_ERROR_IO;
            }
            return rc;
        }
        nwritten += res;
    } while ((nwritten < data_len) ||
             (write_len == RCS956_USB_MAX_BULK_OUT_LEN));

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
 *                                     (NULL means reading the whole data)
 * \param  time0                  [IN] The base time for time-out. (ms)
 * \param  timeout                [IN] Time-out period. (ms)
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_INVALID_PARAM     Invalid parameter.
 * \retval ICS_ERROR_TIMEOUT           Time-out.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_usb_raw_read(
    ICS_HANDLE handle,
    UINT32 min_read_len,
    UINT32 max_read_len,
    UINT8* data,
    UINT32* read_len,
    UINT32 time0,
    UINT32 timeout)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_read"
    UINT32 rc;
    int res;
    int fd;
    struct usbdevfs_bulktransfer bulktransfer;
    UINT32 nread;
    UINT32 n;
    UINT8 buf[RCS956_USB_MAX_BULK_IN_LEN];
    UINT32 current_time;
    UINT32 rest_timeout;
    ICSLOG_FUNC_BEGIN;

    ICSLIB_CHKARG_NE(data, NULL, ICS_ERROR_INVALID_PARAM);
    ICSLIB_CHKARG_LE(min_read_len, max_read_len, ICS_ERROR_INVALID_PARAM);

    ICSLOG_DBG_INT(handle);
    ICSLOG_DBG_UINT(max_read_len);
    ICSLOG_DBG_UINT(time0);
    ICSLOG_DBG_UINT(timeout);

    fd = handle;

    nread = 0;
    do {
        n = (max_read_len - nread);
        if (n > RCS956_USB_MAX_BULK_IN_LEN) {
            n = RCS956_USB_MAX_BULK_IN_LEN;
        }

        rest_timeout = utl_get_rest_timeout(time0, timeout, &current_time);
        if (rest_timeout == 0) {
            rc = ICS_ERROR_TIMEOUT;
            ICSLOG_ERR_STR(rc, "Time-out.");
            return rc;
        }

        bulktransfer.ep = RCS956_USB_ENDPOINT_BULK_IN;
        bulktransfer.len = n;
        bulktransfer.timeout = rest_timeout;
        bulktransfer.data = buf;

        res = ioctl(fd, USBDEVFS_BULK, &bulktransfer);
        if (res < 0) {
            ICSLOG_ERR_STR(errno, "ioctl(USBDEVFS_BULK)");
            if (errno == ETIMEDOUT) {
                rc = ICS_ERROR_TIMEOUT;
            } else {
                rc = ICS_ERROR_IO;
            }
            return rc;
        }
        if (res > n) {
            rc = ICS_ERROR_BUF_OVERFLOW;
            ICSLOG_ERR_STR(rc, "Buffer overflow.");
            return rc;
        }
        utl_memcpy(data + nread, buf, res);

        nread += res;
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
 * This function clears the receiving queue of the USB.
 *
 * \param  handle                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 */
UINT32 rcs956_usb_raw_clear_rx_queue(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_clear_rx_queue"
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_INT(handle);

    /* Do nothing. */

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

/**
 * This function waits until all data written to the USB.
 *
 * \param  handle                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 */
UINT32 rcs956_usb_raw_drain_tx_queue(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_drain_tx_queue"
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_INT(handle);

    /* Do nothing. */

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}


/**
 * This function resets the USB device.
 *
 * \param  handle                 [IN] The handle to access the port.
 *
 * \retval ICS_ERROR_SUCCESS           No error.
 * \retval ICS_ERROR_IO                Other driver error.
 */
UINT32 rcs956_usb_raw_reset(
    ICS_HANDLE handle)
{
#undef ICSLOG_FUNC
#define ICSLOG_FUNC "rcs956_usb_raw_reset"
    int res;
    UINT32 rc;
    int fd;
    ICSLOG_FUNC_BEGIN;

    ICSLOG_DBG_INT(handle);

    fd = handle;

    res = ioctl(fd, USBDEVFS_RESET, NULL);
    if (res != 0) {
        rc = ICS_ERROR_IO;
        ICSLOG_ERR_STR(rc, "ioctl(USBDEVFS_RESET)");
        return rc;
    }

    ICSLOG_FUNC_END;
    return ICS_ERROR_SUCCESS;
}

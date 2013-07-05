/***************************************************************************/
/*                                                                         */
/*                   LINUX SELFCHECK TEST PROGRAM                          */
/*            Linux Selfcheck Test Program for j-panel                     */
/*                                                                         */
/***************************************************************************/
/***************************************************************************/
/* SPECIFICATIONS                                                          */
/*   FILE       = USB_FeliCatest.h                                         */
/*   FUNCTION   = Testing the hardware connectivity of the                 */
/*              = j-panel FeliCa Devices                                   */
/*   DEVELOPERS = VACS                                                     */
/*   ATTRIBUTE                                                             */
/*   HISTORY                                                               */
/*     ID    Date      Name           Description                          */
/*     ----  --------  -------------  -----------------------------------  */
/*                                                                         */
/***************************************************************************/
/*	RCS956												*/
#include	<felica_cc.h>
#include	<rcs956_typea.h>
#include	<utl.h>
#include	<icsdrv.h>
#include	<ics_error.h>
#include	<rcs956_typeb.h>

#ifndef __USB_FELICA_H__

 #define __USB_FELICA_H__

#if defined __cplusplus
	extern "C" {
#endif

/*	FeliCa												*/
#define	USB_FELICA_RECV_SIZE_MAX					128
#define	LOCAL_WORK_SIZE16							16
#define	LOCAL_WORK_SIZE64							64
#define	LOCAL_WORK_SIZE128							128
#define	LOCAL_WORK_SIZE256							256
#define	LOCAL_WORK_SIZE1024							1024
#define	DEVICE_WORK_SIZE128							128
#define	INTERNAL_DEVICE_NO_SIZE						128
#define	USBFS_DEVICES_WORK_SIZE						255
#define	MOUNT_LOG_WORK_SIZE							255
#define	MNT_WR_WORK_SIZE							255

#if	1
#define	USB_FELICA_DISP_CLEAR						1
#endif
/*	50count clear										*/
#define	USB_FELICA_DISP_CLEAR_COUNT					50

#define	USB_FELICA_READ_THD_STATUS_INIT				0
#define	USB_FELICA_READ_THD_STATUS_START			1
#define	USB_FELICA_READ_THD_STATUS_FINISH_REQ		2
#define	USB_FELICA_READ_THD_STATUS_FINISH			99

/*	usbfs parameter										*/
#define	USBFS_MOUNT_COMMAND							"mount"
#define	USBFS_UMOUNT_COMMAND						"umount"
#define	USBFS_MOUNT_OPTION							"-t"
#define	USBFS_UMOUNT_OPTION							"-l"
#define	USBFS_MOUNT_DEVICE1							"usbfs"
#define	USBFS_MOUNT_DEVICE2							"usbfs"
#define	USBFS_MOUNT_DEVICE_POTISON					"/proc/bus/usb"

/*	internal USB mount/umount parameter					*/
#define	INTERNAL_USB_DEVICE1						"/dev"
#define	INTERNAL_USB_DEVICE2						"/sda1"
#define	INTERNAL_USB_DEVICE_POTISON					"/mnt1"
#define	INTERNAL_USB_MOUNT_OPTION1					"-o"
#define	INTERNAL_USB_MOUNT_OPTION2					"exec"

/*	RCS956 usbfs portno get								*/
#define	USBFS_PORT_DEVICES							"/devices"
#define	USBFS_PORT_TARGET_STRING					"Dev#="
#define	USBFS_MANUFACTURER_STRING					"Manufacturer=Sony"

/*	RCS956 usbfs portno									*/
#define	USBFS_DEVICES_START_POINTER					5
#define	USBFS_DEVICES_PORTNO_CHAR_NUMBER			3

/*	may be overwritten by Makefile						*/
#define	RCS956_DEFAULT_PORT_NAME					"/001"
#define	RCS956_DEFAULT_PORT_START_DEVICE_NAME		2
#define	RCS956_DEFAULT_PORT_END_DEVICE_NAME			127
#define	RCS956_DEFAULT_SPEED						115200
/*	300ms												*/
#define	RCS956_DEFAULT_TIMEOUT						300

#define	RCS956_DEFAULT_SYSTEM_CODE					0xffff
#define	RCS956_DEFAULT_POLLING_MAX_RETRY_TIMES		9
#define	RCS956_DEFAULT_POLLING_OPTION				0
#define	RCS956_DEFAULT_POLLING_TIMESLOT				0

#define	MOUNT_LOG_INIT								0
#define	MOUNT_LOG_OK								1
#define	MOUNT_LOG_NG								-1
#define	MOUNT_LOG_ERROR								-2
#define	MOUNT_OK									1
#define	MOUNT_NG									0
#define	UMOUNT_OK									0
#define	UMOUNT_NG									1

#define	RCS956_PORT_NO_GET_INIT						0
#define	RCS956_PORT_NO_GET_NG						-1

#define	RCS956_OPEN_INIT							0
#define	RCS956_OPEN_NG								-1

#define	RCS956_INITIALIZE_OK						0
#define	RCS956_INITIALIZE_NG						-1

#define	RCS956_FINALIZE_OK							0
#define	RCS956_FINALIZE_NG							-1

#define	IDM_STRING_BYTE								8
#define	PUPI_STRING_BYTE							4

#define	MOUNT_LOG_FILE								"/etc/mtab"

/*	init												*/
#define	RCS956_ICS_INIT								99
/*	RCS956 polling success								*/
#define	RCS956_ICS_SUCCESS							1
/*	RCS956 typea polling success						*/
#define	RCS956_TYPEA_ICS_SUCCESS					2
/*	RCS956 typeb polling success						*/
#define	RCS956_TYPEB_ICS_SUCCESS					3
/*	RCS956 polling error								*/
#define	RCS956_ICS_ERROR							4
/*	RCS956 rf_off error									*/
#define	RCS956_RF_OFF_ICS_ERROR						5
/*	RCS956 usbfs mount error							*/
#define	RCS956_USBFS_ERROR							6
/*	RCS956 open error									*/
#define	RCS956_OPEN_ERROR							7
/*	I2C-USB switching error								*/
#define	I2C_USB_SWITCH_ERROR						8
/*	TCS956 portno get error								*/
#define	RCS956_PORT_NO_GET_ERROR					9
/*	I2C-USB auto switching + RCS956 re-opening			*/
#define	RCS956_RE_OPENING_START						10
/*	internal USB memory access permission				*/
#define	INTERNAL_USB_ACCESS_PER						11
/*	internal USB memory access prohibition				*/
#define	INTERNAL_USB_ACCESS_PRO						12
/*	internal USB memory access error					*/
#define	INTERNAL_USB_ACCESS_ERROR					13

/*	RCS956 polling permission							*/
#define	RCS956_POLLING_PERMISSION					1
/*	RCS956 polling prohibition							*/
#define	RCS956_POLLING_PROHIBITION					0

/*	RCS956 polling order FeliCa							*/
#define	RCS956_POLLING_ORDER_FELICA					0
/*	RCS956 polling order TYPEA							*/
#define	RCS956_POLLING_ORDER_TYPEA					1
/*	RCS956 polling order TYPEB							*/
#define	RCS956_POLLING_ORDER_TYPEB					2

#define	RCS956_DISPLAY_ON							1
#define	RCS956_DISPLAY_OFF							0

#define	RCS956_LOGGING_ON							1
#define	RCS956_LOGGING_OFF							0

#define	RETURN_NO_ERROR								0
#define	RETURN_ERROR								-1

#define	USB_FELICA_THREAD_FINISH_COUNTER			20

#define	RCS956_POLLING_OFF							0
#define	RCS956_POLLING_ON							1

#define	RCS956_OPENING_OFF							0
#define	RCS956_OPENING_ON							1

#define	RCS956_RE_OPENING_OFF						0
#define	RCS956_RE_OPENING_ON						1

/*	USB FeliCa logging									*/
/*	0:normal 1:DEBUG									*/
#if	0
	#define	USB_FELICA_LOGGING_DEBUG				99
#endif

#ifdef	USB_FELICA_LOGGING_DEBUG
	/*	DEBUG											*/
	#define	USB_FELICA_USB							"/tmp"
#else
	/*	NORMAL											*/
	#define	USB_FELICA_USB							"/mnt1"
#endif

#ifdef	USB_FELICA_LOGGING_DEBUG
	/*	DEBUG											*/
	#define	USB_FELICA_DIR							"/"
#else
	/*	NORMAL											*/
	#define	USB_FELICA_DIR							"/logging"
#endif

/*	USB FeliCa logging file save cycle(M)				*/
#define	USB_FELICA_LOGGING_CYCLE_FILE_NAME			"/usb_felica_cycle.txt"

#ifdef	USB_FELICA_LOGGING_DEBUG
	/*	DEBUG											*/
	#define	USB_FELICA_LOGGING_FILE_BASE			"usb_felica_log"
	#define	USB_FELICA_LOGGING_FILE_BASE_FIX		"fix_usb_felica_log"
#else
	/*	NORMAL											*/
	/*	usb_felica_logXXXX.log							*/
	#define	USB_FELICA_LOGGING_FILE_BASE			"/usb_felica_log"
	#define	USB_FELICA_LOGGING_FILE_BASE_FIX		"/fix_usb_felica_log"
#endif

/*	XXXX=0001-9999										*/
#define	USB_FELICA_LOGGING_FILE_LOG					".log"

#ifdef	USB_FELICA_LOGGING_DEBUG
	/*	DEBUG											*/
	#define	USB_FELICA_LOGGING_CYCLE_DEF			1
#else
	/*	NORMAL											*/
	/*	60m(1H)											*/
	#define	USB_FELICA_LOGGING_CYCLE_DEF			60
#endif

/*	1m													*/
#define	USB_FELICA_LOGGING_CYCLE_MIN				1
/*	1440m(24H)											*/
#define	USB_FELICA_LOGGING_CYCLE_MAX				1440
#define	USB_FELICA_XXXX_INIT						0
#define	USB_FELICA_XXXX_MIN							1
#define	USB_FELICA_XXXX_MAX							9999
/*	60000ms(60s=1m)										*/
#define	USB_FELICA_LOGGING_TIME						60000
#define	USB_FELICA_LOGGING_TIME_INIT				0
/*	60:M 3600:H											*/
#define	USB_FELICA_LOGGING_TIME_BASE				60
#define	USB_FELICA_LOGGING_WRITE_INIT				0
/*	Display only										*/
#define	USB_FELICA_LOGGING_WRITE_NO					(USB_FELICA_LOGGING_WRITE_INIT + 1)
/*	Display and logging									*/
#define	USB_FELICA_LOGGING_WRITE_OK					(USB_FELICA_LOGGING_WRITE_NO + 1)
#define	USB_FELICA_LOGGING_TIMEOUT_INIT				-1

/*	set logging message									*/
#define	USB_FELICA_SET_LOGGING_MESSAGE_OK			0
#define	USB_FELICA_SET_LOGGING_MESSAGE_NG			-1

#define	USB_FELICA_SET_LOGGING_MESSAGE_NO_POS		0
#define	USB_FELICA_SET_LOGGING_LENGTH_POS			1
#define	USB_FELICA_SET_LOGGING_MESSAGE_POS			2
#define	USB_TYPEA_UID_MESSAGE_START_POS				5
#define	USB_FELICA_SET_LOGGING_MESSAGE_CHAR_LEN		2

#define	USB_FELICA_SET_LOGGING_NOMESSGAE			0
#define	USB_FELICA_SET_DUMMY						2
#define	USB_FELICA_SET_LOGGING_LENGTH_CNT			2

/*	set i2c connecting satus message					*/
#define	USB_FELICA_SET_CONNECT_STATUS_OK			0
#define	USB_FELICA_SET_CONNECT_STATUS_NG			-1
/*	set i2c connecting									*/
#define	USB_FELICA_SET_CONNECT						1
/*	set i2c disconnecting								*/
#define	USB_FELICA_SET_DISCONNECT					2

/*	set usb felica test status message					*/
#define	USB_FELICA_SET_STATUS_OK					0
#define	USB_FELICA_SET_STATUS_NG					-1
/*	usb felica test status normal						*/
#define	USB_FELICA_STATUS_NORMAL					1
/*	usb felica test status abnormal						*/
#define	USB_FELICA_STATUS_ABNORMAL					2

/*	USB FeliCa read thred join							*/
#define	USB_FELICA_READ_THREAD_JOIN_OK				0
#define	USB_FELICA_READ_THREAD_JOIN_NG				-1

/*	I2C-USB switching processing						*/
#define	I2C_USB_SWITCHING_PROCESSING_OK				0
#define	I2C_USB_SWITCHING_PROCESSING_NG				-1

/*	switching process select							*/
#define	I2C_USB_SWITCHING_PROCESS_OPENING			1
#define	I2C_USB_SWITCHING_PROCESS_RE_OPENING		2
#define	I2C_USB_SWITCHING_PROCESS_BOTH_OPENING		3

/*	internal USB memory access							*/
/*	internal USB memory access timer 					*/
#define	INTERNAL_USB_ACCESS_TIME					1000
/*	internal USB memory access permission				*/
#define	INTERNAL_USB_ACCESS_PERMISSION				1
/*	internal USB memory access prohibition				*/
#define	INTERNAL_USB_ACCESS_PROHIBITION				0
/*	internal USB memory access device init				*/
#define	INTERNAL_USB_ACCESS_DEVICE_INIT_NO			255
/*	internal USB memory access devices no check			*/
#define	INTERNAL_USB_ACCESS_DEVICE_FIND				1
#define	INTERNAL_USB_ACCESS_DEVICE_NOTFIND			0
/*	internal USB memory access error flag				*/
#define	INTERNAL_USB_ACCESS_ERROR_OFF				0
#define	INTERNAL_USB_ACCESS_ERROR_ON				1
/*	internal USB memory access open file				*/
#define	INTERNAL_USB_ACCESS_OPEN_FILE_NAME_ACCESS	"/internal_usb_access.txt"

/*	ICS ERROR restart									*/
#define	ICS_ERROR_OFF								0
#define	ICS_ERROR_ON								1

/*	ICS ERROR MESSAGE									*/
#define ICS_ERROR_NOT_SUPPORTED_MESSAGE				"not supported"
#define ICS_ERROR_NOT_IMPLEMENTED_MESSAGE			"not implemented"
#define ICS_ERROR_NOT_INITIALIZED_MESSAGE			"not initialized"
#define ICS_ERROR_NOT_OPENED_MESSAGE				"not opened"
#define ICS_ERROR_ALREADY_OPENED_MESSAGE			"already opened"
#define ICS_ERROR_INVALID_PARAM_MESSAGE				"invalid param"
#define ICS_ERROR_ILLEGAL_MODE_MESSAGE				"illegal mode"
#define ICS_ERROR_FATAL_MESSAGE						"fatal"
#define ICS_ERROR_IO_MESSAGE						"io"
#define ICS_ERROR_NO_RESOURCES_MESSAGE				"no resources"
#define ICS_ERROR_BUSY_MESSAGE						"busy"
#define ICS_ERROR_PERMISSION_MESSAGE				"permisson"
#define ICS_ERROR_TIMEOUT_MESSAGE					"timeout"
#define ICS_ERROR_FRAME_CRC_MESSAGE					"frame crc"
#define ICS_ERROR_INVALID_RESPONSE_MESSAGE			"invalid id"
#define ICS_ERROR_SYNTAX_MESSAGE					"syntax"
#define ICS_ERROR_BUF_OVERFLOW_MESSAGE				"buf overflow"
#define ICS_ERROR_DATA_TRANS_START_MESSAGE			"data trans start"
#define ICS_ERROR_DATA_TRANS_END_MESSAGE			"data trans end"
#define ICS_ERROR_NOT_STARTED_MESSAGE				"not started"
#define ICS_ERROR_ALREADY_STARTED_MESSAGE			"already started"
#define ICS_ERROR_SEQUENCE_MESSAGE					"sequence"
#define ICS_ERROR_DESELECTED_MESSAGE				"deselected"
#define ICS_ERROR_RELEASED_MESSAGE					"released"
#define ICS_ERROR_RF_OFF_MESSAGE					"rf off"
#define ICS_ERROR_NOT_EXIST_MESSAGE					"not exist"
#define ICS_ERROR_ALREADY_EXIST_MESSAGE				"already exist"
#define ICS_ERROR_IGNORE_MESSAGE					"ignore"
#define ICS_ERROR_STATUS_FLAG1_MESSAGE				"status flag1"
#define ICS_ERROR_STATUS_FLAG_MESSAGE				"status flag"
#define ICS_ERROR_SN_OVERFLOW_MESSAGE				"sn overflow"
#define ICS_ERROR_INVALID_DATA_MESSAGE				"invalid id data"
#define ICS_ERROR_DISCONNECTED_MESSAGE				"disconnected"
#define ICS_ERROR_SHUTDOWN_MESSAGE					"shutdown"
#define ICS_ERROR_MANY_ERRORS_MESSAGE				"many errors"
#define ICS_ERROR_NOT_CONNECTED_MESSAGE				"not connected"
#define ICS_ERROR_DEV_BUSY_MESSAGE					"dev busy"
#define ICS_ERROR_DEVICE_MESSAGE					"device"

/*	/mnt1/copy_exe.txt check							*/
#define	COPY_EXE_USB_DEVICE_POTISON					"/mnt1"
#define	COPY_EXE_FILE_NAME							"/copy_exe.txt"
#define	COPY_EXE_HAVING								1
#define	COPY_EXE_ZILCH								0

/*	USB reset retry										*/
#define	USB_RESET_RETRY_INIT						0
#define	USB_RESET_RETRY_MAX							10

#define	USB_RESET_TIMER_INIT						0
/*	USB reset timer 600ms x 12 = 7200ms				*/
#define	USB_RESET_TIMER_MAX							12
#define	USB_RESET_TIMER_EXEC						99

/************RCS956 Test Functions**************/
/*****************************************
	NAME		: RCS956portNoGet
	I/F			: int		RCS956portNoGet(void)
	FUNCTION	: RCS956 usbfs portno get
	ARGUMENTS	: void
	RETURN		: int	portno(2-127,-1)
*****************************************/
int		RCS956portNoGet(void);

/*****************************************
	NAME		: read_devices_usb
	I/F			: int		read_devices_usb(
											char	*file_name,
											char	*check_str,
											char	*check_end
											)
	FUNCTION	: devices file check
	ARGUMENTS	: char	*file_name	mount logging file name
				  char	*check_str		check string
				  char	*check_end		check end string
	RETURN		: int	portno(2-127,-1)
*****************************************/
int		read_devices_usb(
						char	*file_name,
						char	*check_str,
						char	*check_end
						);

/*****************************************
	NAME		: internal_USB_access_devicesno_check
	I/F			: int	internal_USB_access_devicesno_check(void)
	FUNCTION	: internal USB memory access devices no check
	ARGUMENTS	: void
	RETURN		: int	status
*****************************************/
int		internal_USB_access_devicesno_check(void);

/*****************************************
	NAME		: RCS956open
	I/F			: int		RCS956open(int	device_no)
	FUNCTION	: RCS956 open
	ARGUMENTS	: int	device_no
	RETURN		: int	status
*****************************************/
int		RCS956open(int	device_no);

/*****************************************
	NAME		: rcs956initialize
	I/F			: int		rcs956initialize(
											ICS_HW_DEVICE		*dev,
											felica_cc_devf_t	*devf,
											int					device_no
											)
	FUNCTION	: RCS956 initialize
	ARGUMENTS	: ICS_HW_DEVICE		*dev	H/W device id
				  felica_cc_devf_t	*devf	device info
				  int				device_no	deviceno
	RETURN		: int	status
*****************************************/
int		rcs956initialize(
						ICS_HW_DEVICE		*dev,
						felica_cc_devf_t	*devf,
						int					device_no
						);

/*****************************************
	NAME		: read_mount_log_usb
	I/F			: int		read_mount_log_usb(char	*file_name,char	*check_str)
	FUNCTION	: mount logging file check
	ARGUMENTS	: char	*file_name	mount logging file name
				  char	*check_str	mount logging check string
	RETURN		: int status
*****************************************/
int		read_mount_log_usb(char	*file_name,char	*check_str);

/*****************************************
	NAME		: rcs956finalize
	I/F			: int		rcs956finalize(ICS_HW_DEVICE	*dev)
	FUNCTION	: RCS956 finalize
	ARGUMENTS	: ICS_HW_DEVICE		*dev	H/W device id
	RETURN		: int status
*****************************************/
int		rcs956finalize(ICS_HW_DEVICE	*dev);

/*****************************************
  NAME		: USB_FeliCa_resulttextcopy
  I/F		: int	USB_FeliCa_resulttextcopy(void)
  FUNCTION	: USB FeliCa Result Text Copy
  ARGUMENTS	: void
  RETURN	: int status
*****************************************/
int		USB_FeliCa_resulttextcopy(void);

/*****************************************
  NAME		: USB_FeliCa_resulttextread
  I/F		: int		USB_FeliCa_resulttextread(char *revb)
  FUNCTION	: USB FeliCa Result Text Read
  ARGUMENTS	: char *revb
  RETURN	: int status
*****************************************/
int		USB_FeliCa_resulttextread(char	*revb);

/*****************************************
  NAME		: USB FeliCareadthd_status_read
  I/F		: int USB_FeliCareadthd_status_read (void)
  FUNCTION	: FeliCareadthd status read
  ARGUMENTS	: void
  RETURN	: int status
*****************************************/
int		USB_FeliCareadthd_status_read(void);

/*****************************************
  NAME		: USB FeliCareadthd_status_write
  I/F		: int USB_FeliCareadthd_status_write(int write_status)
  FUNCTION	: FeliCareadthd status write
  ARGUMENTS	: int write status
  RETURN	: int status
*****************************************/
int		USB_FeliCareadthd_status_write(int	write_status);

/*****************************************
  NAME		: USB_FeliCaTeatinit
  I/F		: int USB_FeliCaTeatinit ()
  FUNCTION	: FeliCa Test Init
  ARGUMENTS	: void
  RETURN	: int status
*****************************************/
int		USB_FeliCaTeatinit (void);

/*****************************************
  NAME		: USB_FELICA_LOGGING_FILE_CLEAR
  I/F		: int		USB_FELICA_LOGGING_FILE_CLEAR(void)
  FUNCTION	: FeliCa Logging logging file clear
  ARGUMENTS	: void
  RETURN	: int ret
*****************************************/
int		USB_FELICA_LOGGING_FILE_CLEAR(void);

/*****************************************
  NAME		: USB_FELICA_LOGGING_CYCLE
  I/F		: int		USB_FELICA_LOGGING_CYCLE(void)
  FUNCTION	: USB FeliCa Logging file save cycle file Open/read
  ARGUMENTS	: void
  RETURN	: int ret
*****************************************/
int		USB_FELICA_LOGGING_CYCLE(void);

/*****************************************
  NAME		: USB_FELICA_LOGGING_WRITE
  I/F		: int		USB_FELICA_LOGGING_WRITE(void)
  FUNCTION	: USB FeliCa Logging file write
  ARGUMENTS	: char	*log
  RETURN	: int ret
*****************************************/
int		USB_FELICA_LOGGING_WRITE(char	*log);

/*****************************************
  NAME		: USB_FeliCareadthd_join
  I/F		: int		USB_FeliCareadthd_join(void)
  FUNCTION	: USB FeliCa read thred join
  ARGUMENTS	: void
  RETURN	: int status
*****************************************/
int		USB_FeliCareadthd_join(void);

/*****************************************
  NAME		: i2c_usb_switching_request
  I/F		: int		i2c_usb_switching_request(
													int	proc_select,
													int	proc_flg,
													int	polling_flg,
													int	fix_button_flg
												)
  FUNCTION	: I2C-USB switching request
  ARGUMENTS	:	int	proc_select		:switching processing select
				int	proc_flg		:switching processing flg
				int	polling_flg		:polling flg
				int	fix_button_flg	:fix button flg
  RETURN	: int	status
*****************************************/
int		i2c_usb_switching_request(
									int	proc_select,
									int	proc_flg,
									int	polling_flg,
									int	fix_button_flg
								);

#if defined __cplusplus
	}
#endif

#endif

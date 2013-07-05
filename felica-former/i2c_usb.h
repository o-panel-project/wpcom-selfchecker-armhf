#ifndef	__USBSW_H__

	#define	__USBSW_H__

	#if	defined	__cplusplus
		extern	"C"	{
	#endif

/*	define										*/
#define	I2C_USB_SWITCH_OFF						0
#define	I2C_USB_SWITCH_ON						(I2C_USB_SWITCH_OFF + 1)
#define	I2C_USB_SWITCHCONT						(I2C_USB_SWITCH_OFF + 2)

#define	I2C_USB_OFF								0
#define	I2C_USB_ON								(I2C_USB_OFF + 1)
#define	I2C_USB_CONT							(I2C_USB_OFF + 2)

/*	cradle										*/
#define	CRADLE_START							0
#define	CRADLE_STOP								(CRADLE_START + 1)

/*	uslepp										*/
#define	WAIT_1MS								1000
#define	WAIT_50MS								50000
#define	WAIT_100MS								100000
#define	WAIT_200MS								200000
#define	WAIT_300MS								300000
#define	WAIT_400MS								400000
#define	WAIT_500MS								500000
#define	WAIT_600MS								600000
#define	WAIT_700MS								700000
#define	WAIT_800MS								800000
#define	WAIT_900MS								900000
#define	WAIT_1000MS								1000000

/*	PCA9557 write data cnt						*/
#define	I2C_USB_DATA_CNT						3
/*	1.Reg1<=0x00								*/
/*	2.Reg3<=0x00								*/
/*	3.Reg1<=0xXX								*/
#define	I2C_USB_DATA_CNT_REG1_INIT				0
#define	I2C_USB_DATA_CNT_REG3_CONF				1
#define	I2C_USB_DATA_CNT_REG1_OUT				2

/*	PCA9557 write data size						*/
#define	I2C_USB_DATA_SIZE						1

/*	I2C error									*/
#define	I2C_USB_WRITE_ERROR						-1
#define	I2C_USB_MMAP_ERROR						-3
#define	I2C_USB_IOCTL_ERROR						-4
#define	I2C_USB_DEV_OPEN_ERROR					-5
#define	I2C_USB_BUS_ERROR						-16
#define	I2C_USB_ARB_LOST						-22

/*	PCA9557 slave address						*/
#define	I2C_USB_SLAVE_ADDRESS					0x3a

/*	PCA9557 Write data							*/
/*	Command byte								*/
/*	Register 1 (Output Port Register)			*/
#define	I2C_USB_COMMAND_REG01					0x01
/*	Register 3 (Configuration Register)			*/
#define	I2C_USB_COMMAND_REG03					0x03

/*	Data to port reg1 init						*/
#define	I2C_USB_DATA_REG1_INIT					0x00
/*	Data to port reg3 config					*/
#define	I2C_USB_DATA_REG3_CONF					0x00
/*	Data=0xff									*/
#define	I2C_USB_DATA_0xFF						0xff

#if	1
/*	I2C Select		:	00000010B				*/
#define	I2C_USB_DATA_REG1						0x02
/*	USB Fixation	:	00000100B				*/
#define	I2C_USB_DATA_REG1_FIX					0x04
/*	USB 30s			:	00001000B				*/
#define	I2C_USB_DATA_REG1_30SEC					0x08

#else	/*	old									*/
/*	I2C Select		:	00000010B				*/
#define	I2C_USB_DATA_REG1						0x02
/*	USB 30s			:	00000100B				*/
#define	I2C_USB_DATA_REG1_30SEC					0x04
/*	USB 60s			:	00001000B				*/
#define	I2C_USB_DATA_REG1_60SEC					0x08
/*	USB 90s			:	00010000B				*/
#define	I2C_USB_DATA_REG1_90SEC					0x10
/*	USB Fixation	:	00100000B				*/
#define	I2C_USB_DATA_REG1_FIX					0x20

#endif

/* mount device name							*/
#define	MNT_DEV_SDA1							"/dev/sda1"
#define	MNT_DEV_SDB1							"/dev/sdb1"
#define	MNT_DEV_SDC1							"/dev/sdc1"
#define	MNT_DEV_SDD1							"/dev/sdd1"

/* mount/umount command							*/
#define	DEV_MOUNT_COMMAND						"mount"
#define	DEV_UMOUNT_COMMAND						"umount"

#define	TERMINAL_SIDE_NAME_MNT1					"/mnt1"
#define	CRADLE_SIDE_NAME_MNT2					"/mnt2"

#define	DEV_MOUNT_OPTION						"-o exec"
#define	DEV_UMOUNT_OPTION_L						"-l"
#define	DEV_UMOUNT_OPTION_F						"-f"

/* mount/umount log file name					*/
#define	MOUNT_LOG_FILE_HUB						"/etc/mtab"

/* W/R test file name							*/
#define	MNT_WR_TERMINAL_USB_FILE				"/mnt1_mount_loop_test.txt"
#define	MNT_WR_CRADLE_USB_FILE					"/mnt2_mount_loop_test.txt"

/* mount/umount log file result					*/
#define	MOUNT_LOG_INIT_HUB						0
#define	MOUNT_LOG_OK_HUB						1
#define	MOUNT_LOG_NG_HUB						-1
#define	MOUNT_LOG_ERROR_HUB						-2
#define	MOUNT_NG_HUB							0
#define	MOUNT_OK_HUB							1
#define	MOUNT_ERR_HUB							99
#define	UMOUNT_OK_HUB							0
#define	UMOUNT_NG_HUB							1

/* W/R test result								*/
#define	MNT_WR_INIT								0
#define	MNT_WR_OK								1
#define	MNT_WR_NG								-1
#define	MNT_WR_ERROR							-2

/* W/R test str									*/
#define	TERMINAL_SIDE_MNT_RW_STRING				"abcdefghijklmnopqrstuvwxyz1234567890"
#define	CRADLE_SIDE_MNT_RW_STRING				"0987654321zyxwvutsrqponmlkjihgfedcba"

/*	terminal USB mount check timer				*/
#define	TERMINAL_MOUNT_CHECK_TIMER				500
/*	auto mount 1000ms							*/
#define	TERMINAL_MOUNT_TIMER					1000
/*	auto mount 1000ms							*/
#define	CRADLE_MOUNT_TIMER						1000

/*	auto mount start 5000ms						*/
#define	CRADLE_MOUNT_START_TIMER				5000

/*	auto umount 7sec							*/
#define	TERMINAL_UMOUNT_TIMER					7000

/*	auto umount 7sec							*/
#define	CRADLE_UMOUNT_TIMER						7000

/* 100ms										*/
#define	USLEEP_TIME_BASE100						100000
/* 200ms										*/
#define	USLEEP_TIME_BASE200						100000

/*	Electrical discharge waiting 5sec			*/
#define	ELECT_DISCHARGE_WAIT_SEC				5

/*	I2C-USB Switching timer	(600ms)				*/
#define	I2C_USB_SWITHING_TIMER					600

/*	I2C-USB status message						*/
#define	I2C_USB_STATUS_OK_MESSAGE				"USB connecting..."
#define	I2C_USB_STATUS_NG_MESSAGE				"USB disconnecting..."

/*	usbfs mount error							*/
#define	USBFS_MOUNT_NG_MESSAGE					"usbfs mount error"
/*	RCS956 open success							*/
#define	RCS956_OPEN_OK_MESSAGE					"RCS956 open success"
/*	RCS956 open error							*/
#define	RCS956_OPEN_NG_MESSAGE					"RCS956 open error"
/*	RCS956 close success						*/
#define	RCS956_CLOSE_OK_MESSAGE					"RCS956 close success"
/*	RCS956 close error							*/
#define	RCS956_CLOSE_NG_MESSAGE					"RCS956 close error"
/*	I2C-USB switching error						*/
#define	I2C_USB_SWITCHING_NG_MESSAGE			"I2C-USB switching error"
/*	RCS956 portno get error						*/
#define	RCS956_PORT_NO_GET_NG_MESSAGE			"usbfs portno get error"
/*	RCS956 ICS error							*/
#define	RCS956_ICS_ERROR_MESSAGE				"ICS ERRROR"
/*	RCS956 rf off ICS error						*/
#define	RCS956_RF_OFF_ICS_ERROR_MESSAGE			"RF OFF ICS ERRROR"
/*	RCS956 other error							*/
#define	RCS956_ICS_OTHER_ERROR_MESSAGE			"ICS OTHER ERROR"
/*	I2C-USB auto switching + RCS956 re-opening	*/
#define	RCS956_RE_OPENING_START_MESSAGE			"RCS956 re-opening start"
/*	internal USB memory access permission		*/
#define	INTERNAL_USB_ACCESS_PER_MESSAGE			"Internal USB access permit"
/*	internal USB memory access prohibition		*/
#define	INTERNAL_USB_ACCESS_PRO_MESSAGE			"Internal USB access prohibit"
/*	internal USB memory access error			*/
#define	INTERNAL_USB_ACCESS_ERROR_MESSAGE		"It failed in internal USB it access."

/*	I2C-USB auto switching timer max			*/
#define	I2C_USB_AUTO_CNT_MAX					2

/*	1:REG01 Reverse								*/
#if	0
#define	I2C_USB_COMMAND_REG01_REVERSE			99
#endif

/*	1:terminal USB umount						*/
#if	0
#define	I2C_USB_TERMINAL_USB_UMOUNT				99
#endif

/*	I2C-USB fixation button on off switch		*/
#define	I2C_USB_FIX_BUTTON_TRUE					1
#define	I2C_USB_FIX_BUTTON_FALSE				0

/*	I2C-USB	fixation button fix or 30sec select	*/
#define	I2C_USB_FIX_BUTTON_NONE					0
#define	I2C_USB_FIX_BUTTON_FIX					1
#define	I2C_USB_FIX_BUTTON_30SEC				2

/*	USB Hub reset counter max					*/
#define	USB_HUB_RESET_COUNTER_MAX				10

/*	USB Hub Test								*/
#define	USB_HUB_SELECT_NONE						0
#define	USB_HUB_SELECT_TERMINAL_P1				1
#define	USB_HUB_SELECT_TERMINAL_P2				2
#define	USB_HUB_SELECT_CRADLE_P3				3
#define	USB_HUB_SELECT_CRADLE_P4				4

/*	USB BCR Print								*/
#define	I2C_USB_PRNTER_OFF						0
#define	I2C_USB_PRNTER_ON						(I2C_USB_PRNTER_OFF + 1)
#define	I2C_USB_PRNTER_CONT						(I2C_USB_PRNTER_OFF + 2)

#define	I2C_USB_BCR_LINE_MAX					1

#define	I2C_USB_BCR_CHECK_TIMER					1000

#define	I2C_USB_BCR_CLEAR_BUFFER_RETRY_TIMER	(10*1000)		/*	10ms			*/
#define	I2C_USB_BCR_CLEAR_BUFFER_RETRY_CNT		10
#define	I2C_USB_BCR_CLEAR_BUFFER_RESPONS_BYTE	3
#define	I2C_USB_BCR_CLEAR_BUFFER_OK				0
#define	I2C_USB_BCR_CLEAR_BUFFER_NG				-1

#define	I2C_USB_BCR_PRINT_OUT_OK				0
#define	I2C_USB_BCR_PRINT_OUT_NG				-1
#define	I2C_USB_BCR_PRINT_RESULT_OK				0
#define	I2C_USB_BCR_PRINT_RESULT_NG				-1

#define	I2C_USB_BCR_PRINT_SPLIT					16

#define	I2C_USB_LINE_CONNECT_MSG				"[ USB line Connection ]"
#define	I2C_USB_LINE_UNCONNECT_MSG				"[ USB line unconnection ]"
#define	I2C_USB_BCR_PRINT_SUCCESS_MSG			"[ USB BCR Print success ]"
#define	I2C_USB_BCR_PRINT_ERROR_MSG				"[ USB BCR Print error ]"
#define	I2C_USB_BCR_PRINT_OPEN_ERROR_MSG		"[ USB Printer open error ]"
#define	I2C_USB_BCR_PRINT_BUF_CLS_ERROR_MSG		"[ USB Printer buffer clear error ]"
#define	I2C_USB_BCR_PRINT_INIT_ERROR_MSG		"[ USB Printer Initialization error ]"

#define	I2C_USB_BCR_CODE_DISP_TOP_SELECT		0
#define	I2C_USB_BCR_CODE_DISP_BOTTOM_SELECT		-1
#define	I2C_USB_BCR_CODE_DISP_MAX_LINES			50
#define	I2C_USB_BCR_CODE_LINE_FEED_MAX			3

/**
 *	main function
 */
int	i2c_usb_switching(int sub_addr, int write_data, int write_cnt);
int	i2c_usb_hub_switching(int sub_addr, int write_data, int write_cnt);

int	clear_print_buffer_bcr(int fd);
int	print_out_bcr(int fd, unsigned char *buf, int size);
void result_bcr_print(char *msg, int flg);

	#if	defined	__cplusplus
		}
	#endif
#endif

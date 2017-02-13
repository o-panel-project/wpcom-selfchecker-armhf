/***************************************************************************/
/*                                                                         */
/*                   LINUX SELFCHECK TEST PROGRAM                          */
/*            Linux Selfcheck Test Program for j-panel                     */
/*                                                                         */
/***************************************************************************/
/***************************************************************************/
/* SPECIFICATIONS                                                          */
/*   FILE       = PLD_IOacess.h                                            */
/*   FUNCTION   = Access functions to the PLD registers                    */
/*   DEVELOPERS = VACS                                                     */
/*   ATTRIBUTE                                                             */
/*   HISTORY                                                               */
/*     ID    Date      Name           Description                          */
/*     ----  --------  -------------  -----------------------------------  */
/*                                                                         */
/***************************************************************************/
#ifndef __PLDIO_H__

 #define __PLDIO_H__

#if defined __cplusplus
	extern "C" {
#endif


/*  PLD registers are mapped in the starting address	*/
/*  of 0xf0000000 defined as __region_CS1				*/
/*  in kernel/include/asm-frv/mb-regs.h					*/
#define BACKLIGHT 	0xf0000000+0x00080001
#define O_BACKLIGHT_SW		4

#define DIPSWITCH	0xf0000000+0x00080002
#define I_DSW1			0
#define I_DSW2			1
#define I_DSW3			2
#define I_DSW4			3

#define LEDINDICATOR	0xf0000000+0x00080003
#define O_DLED1			0
#define O_DLED2			1
#define O_DLED3			2
#define O_DLED4			3
#define O_STSLED_G		4
#define O_STSLED_Y		5
#define O_STSLED_R		6		//added 0929

#define BATT_CHG_ENABLE	0xf0000000+0x00080004
#define BATT_CHG_ON		0

#define P_ENABLE	0xf0000000+0x00080006
#define SD_PWR			0
#define O_LCD_SWITCH		1

#define RAMBOOT_REG	0xf0000000+0x00080007
#define RESUME_ON		2
#define	RAMBOOT_ON		3

#define PLD_A_CLOCK	0xf0000000+0x00080008
#define AUDIO_CLCK		0

#define HARDWARE_INFO	0xf0000000+0x0008000a
#if 0
#define PLD_VERSION		0
#endif
#define HW_VERSION		0
#define E_TYPE			6
#define GENERATION		7

#define PLDVERSION_INFO_HIGH	0xf0000000+0x0008000e
#define	PLD_VERSION_HIGH	0
#define PLDVERSION_INFO_MID_LOW	0xf0000000+0x0008000f
#define	PLD_VERSION_MID		4
#define	PLD_VERSION_LOW		0

#define	DIP_AUTO_READ_TIMER	500		/* 500ms	*/

#define I_ENABLE	0xf0000000+0x00080009
#define O_LVDS_CONTROL		0

#define SUCCESS			0
#define FAIL			1

/*****************************************
  NAME		: write_PLD
  I/F		: char write_PLD(unsigned long target_add, unsigned long *value);
  FUNCTION	: write to PLD register
  ARGUMENTS	: unsigned long target_add - register address
  			: unsigned long *value - pointer to data storage
  RETURN	: char status
*****************************************/
char write_PLD(unsigned long target_add, unsigned long *value);

/*****************************************
  NAME		: read_PLD
  I/F		: char read_PLD(unsigned long target_add, unsigned long *value);
  FUNCTION	: write to PLD register
  ARGUMENTS	: unsigned long target_add - register address
  			: unsigned long *value - pointer to data storage
  RETURN	: char status
*****************************************/
char read_PLD(unsigned long target_add, unsigned long *value);

/*****************************************
  NAME		: Auto_Read_DIP
  I/F		: void	Auto_Read_DIP(void)
  FUNCTION	: Dip Switch Auto Raed
  ARGUMENTS	: void
  RETURN	: void
*****************************************/
void	Auto_Read_DIP(void);

#if defined __cplusplus
	}
#endif

#endif

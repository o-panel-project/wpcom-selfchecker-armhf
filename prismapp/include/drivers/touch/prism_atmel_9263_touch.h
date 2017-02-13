///////////////////////////////////////////////////////////////////////////////
//
//            Copyright (c) 2010 by Blue Water Embedded, Inc.
//
//  This software is copyrighted by and is the sole property of Blue Water
//  Embedded, Inc. All rights, title, ownership, or other intersts in the
//  software remain the property of Blue Water Embedded, Inc. This software
//  may only be used in accordance with the corresponding license agreement.
//  Any unauthorized use, duplications, transmission, distribution, or
//  disclosure of this software is expressly forbidden.
//
//  This Copyright notice may not be removed or modified without prior
//  written consent of Blue Water Embedded, Inc.
//
//  Blue Water Embedded, Inc. reserves the right to modify this software
//  without notice.
//
//  Blue Water Embedded, Inc.               info@bwembedded.com
//  3847 Pine Grove Ave                     http://www.bwembedded.com
//  Suite A
//  Fort Gratiot, MI 48059
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
//  prism_atmel_9263_touch.h - Touch input driver for Ateml 9263 ek board.
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#if defined(PRISM_TOUCH_SUPPORT)

#ifndef _AT91SAM926X_TOUCH_
#define _AT91SAM926X_TOUCH_

///////////////////////////////////////////////////////////////////////////////
// Delay values- User defined delay values. These values are currently
// in 10ms ticks, which is the timebase used when creating this driver.
// You can adjust these values if needed for your system.
///////////////////////////////////////////////////////////////////////////////

#define TOUCH_SPI_DELAY         2       // Wait time while checking SPI busy?

#if defined(PRISM_FOR_MQX)

#define TOUCH_POLLING_PERIOD    2   // Polling Interval
#define TOUCH_SETTLE_DELAY      5   // How long to settle on a value?
#define TOUCH_RESPONSE_DELAY    5
#define TOUCH_PROMPT_DELAY     500
#define PENIRQ_SETTLE_DELAY    1

#elif defined(PRISM_FOR_THREADX)

#define TOUCH_POLLING_PERIOD    1   // Polling Interval
#define TOUCH_PROMPT_DELAY     10
#define TOUCH_RESPONSE_DELAY    5
#define PENIRQ_SETTLE_DELAY    1

#elif defined(PRISM_FOR_NUCLEUS)

#define TOUCH_POLLING_PERIOD    3   // Polling Interval
#define TOUCH_RESPONSE_DELAY    5
#define TOUCH_PROMPT_DELAY     10
#define PENIRQ_SETTLE_DELAY    1


#elif defined(PRISM_FOR_SMX)

#define TOUCH_POLLING_PERIOD    1   // Polling Interval
#define TOUCH_PROMPT_DELAY     10
#define PENIRQ_SETTLE_DELAY    1

#elif defined(PRISM_FOR_CMX)

#define TOUCH_POLLING_PERIOD    5   // Polling Interval
#define TOUCH_PROMPT_DELAY     20
#define PENIRQ_SETTLE_DELAY    5

#elif defined(PRISM_FOR_INTEGRITY)

#define TOUCH_POLLING_PERIOD    2   // Polling Interval
#define TOUCH_PROMPT_DELAY     10
#define PENIRQ_SETTLE_DELAY    1

#elif defined(PRISM_FOR_UCOS)

#define TOUCH_POLLING_PERIOD    5   // Polling Interval
#define TOUCH_PROMPT_DELAY     2
#define PENIRQ_SETTLE_DELAY    2

#elif defined(PRISM_FOR_ECOS)

#define TOUCH_POLLING_PERIOD    5   // Polling Interval
#define TOUCH_PROMPT_DELAY     2
#define PENIRQ_SETTLE_DELAY    2

#else

// running standalone
#define TOUCH_SETTLE_DELAY      50   // How long to settle on a value?
#define TOUCH_PROMPT_DELAY    2000
#define PENIRQ_SETTLE_DELAY    100

#ifndef PRISM_DELAY
#define PRISM_DELAY(a) \
        { \
            pm_int_t _Count = 0; \
            for (_Count = 0; _Count < (a) * 10; _Count++) \
            { \
                Pm_Framework::ScreenManager()->PaintInvalid(); \
            } \
        }
#endif

#endif

///////////////////////////////////////////////////////////////////////////////
// Miscellaneous definitions used in the driver code...
///////////////////////////////////////////////////////////////////////////////
#if defined (PRISM_FOR_INTEGRITY)
#define IODEV_ADS7843_READ_XY		1
#define IODEV_ADS7843_TOUCH_DOWN 	2
#endif
///////////////////////////////////////////////////////////////////////////////
// when an touch interrupt comes in and the X value is 0x0fff, the
// interrupt was generated because of a release. This value ensures
// that we will always be able to figure out what state we should
// be in and allow us to sync back up if we miss the previous down
// interrupt.
//
// Interrupts are only generated on the touch and release events. Moving
// events after the initial touch do not generate further interrupts;
// therefore, once we detect a touch event, we poll the device for
// the current x/y coordinates until we receive a release event.
//
///////////////////////////////////////////////////////////////////////////////
#define PRISM_X_VAL_NOT_TOUCHED 0x0fff

///////////////////////////////////////////////////////////////////////////////
// this is a timeout period used when we write commands to the SPI bus
///////////////////////////////////////////////////////////////////////////////
#define PRISM_SAM926X_TIMEOUT     5000

///////////////////////////////////////////////////////////////////////////////
// the following pertains to the priority of the interrupt.
// this priority can be set between 0 (lowest) and 7 (highest).
///////////////////////////////////////////////////////////////////////////////
#define PRISM_SAM926X_PRIORITY    0x07

///////////////////////////////////////////////////////////////////////////////
// The following are defines which work with the ATMEL/IAR code. These
// are used to proerly initialize the controller.
///////////////////////////////////////////////////////////////////////////////
#define AT91C_MASTER_CLOCK_FOR_I2S      (100454400UL)
//#define AT91C_MASTER_CLOCK_FOR_I2S      (200454400UL)
#define AT91C_TOUCHSCREEN_SPI_CLK       920000


#define AT91C_SPI_PCS2_DATAFLASH        0xB
#define TS_DLYBS                        (10 << 16)
#define TS_DLYBCT                       (40 << 24)

#if defined(PRISM_AT91SAM9261)
#define TOUCH_CS                        2
#else
#define TOUCH_CS                        3
#endif

// ADS7843 Touch Screen Controller Control Byte bit definitions

#define ADS_CTRL_PD0			(1 << 0)		
#define ADS_CTRL_PD1			(1 << 1)		
#define ADS_CTRL_START			(1 << 7)		
#define ADS_CTRL_SWITCH_SHIFT	 4	    		

#define TC_POLL_X       0x93  // 12 bit, differential mode, left justified
#define TC_POLL_Y       0xd3  // 12 bit, differential mode, left justified
#define TC_POWR_DN      0x90  // Y-switches on, IRQ enable, pwr dwn

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class AtmelTouch : public Pm_Touch_Device
{
    public:

        AtmelTouch(void);

        virtual pm_bool_t Initialize();
        virtual void Run(void);

    private:


        virtual pm_bool_t AverageSample(uint16_t &xRaw, uint16_t &yRaw);
        virtual pm_bool_t DetectTouch(void);
        virtual pm_bool_t OneTouchSample(uint16_t &xRaw, uint16_t &yRaw);

        uint16_t SendTouchCommand(uint8_t Command);

       #if defined(PRISM_TOUCH_SUPPORT_INCLUDE_CALIBRATION)
        virtual void WaitForTouchState(pm_bool_t PenDown);
       #endif 

};

#endif 
#endif 

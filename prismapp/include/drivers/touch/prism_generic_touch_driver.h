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
//  prism_generic_touch_driver.h - example implementation of touch driver
//
//  Author: Kenneth G. Maxwell
//
//  Revision History:
//  Refer to SVN revision history.
// 
///////////////////////////////////////////////////////////////////////////////


#if defined(PRISM_TOUCH_SUPPORT)

#ifndef _PRISM_TOUCH_DRIVER_
#define _PRISM_TOUCH_DRIVER_



#ifdef PRISM_TOUCH_NOISE_LIMIT
#undef PRISM_TOUCH_NOISE_LIMIT
#endif

#define PRISM_TOUCH_NOISE_LIMIT  50  // allowed variation in raw consecutive ATOD


///////////////////////////////////////////////////////////////////////////////
// Delay values- User defined delay values. These values are currently
// in 10ms ticks, which is the timebase used when creating this driver.
// You can adjust these values if needed for your system.
///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_FOR_MQX)

#define TOUCH_POLLING_PERIOD    2   // Polling Interval
#define TOUCH_SETTLE_DELAY      5   // How long to settle on a value?
#define TOUCH_RESPONSE_DELAY    5
#define TOUCH_PROMPT_DELAY     500
#define PENIRQ_SETTLE_DELAY    1

#elif defined(PRISM_FOR_THREADX)

#define TOUCH_POLLING_PERIOD    5   // Polling Interval
#define TOUCH_PROMPT_DELAY     10
#define TOUCH_RESPONSE_DELAY    5
#define PENIRQ_SETTLE_DELAY     5

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
#define TOUCH_PROMPT_DELAY     20
#define PENIRQ_SETTLE_DELAY    5

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
///////////////////////////////////////////////////////////////////////////////

class PrismGenericTouch : public Pm_Touch_Device
{
    public:

        PrismGenericTouch(void);
        virtual pm_bool_t Initialize();
        virtual void Run(void);

    private:

        virtual pm_bool_t DetectTouch(void);
        virtual pm_bool_t OneTouchSample(uint16_t &xRaw, uint16_t &yRaw);
};

#endif 
#endif 

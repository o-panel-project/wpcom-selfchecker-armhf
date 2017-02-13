///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// ptouchdev.hpp - Pure virtual base class for touch devices
//
// Copyright (c) 2010 Blue Water Embedded, Inc.
//              All Rights Reserved.
//
// Unauthorized redistribution of this source code, in whole or part,
// without the express written permission of Blue Water Embedded, Inc.
// is strictly prohibited.
//
// Notes:
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#if defined(PRISM_TOUCH_SUPPORT)

#ifndef PTOUCHDEV_HPP_
#define PTOUCHDEV_HPP_

// special test mode, should not be used for normal operation
//#define PRISM_TOUCH_TEST_MODE


///////////////////////////////////////////////////////////////////////////////
// PRISM_TOUCH_LINEAR_SCALE- Turn this on to use simple interpolation for
// each axis, and comment this out to use a slower but more accurate
// successive approximation algorithm.
///////////////////////////////////////////////////////////////////////////////

#define PRISM_TOUCH_LINEAR_SCALE

///////////////////////////////////////////////////////////////////////////////
// If not using LINEAR_SCALE, this determines when the approximation is
// good enough
///////////////////////////////////////////////////////////////////////////////

#define PRISM_TOUCH_PIXEL_ACCURACY 10

#define PRISM_TOUCH_NOISE_LIMIT  50   

#ifndef PRISM_TOUCH_SAMPLES
//#define PRISM_TOUCH_SAMPLES      8    // how many samples do we average for one
#define PRISM_TOUCH_SAMPLES      1    // how many samples do we average for one
#endif                              // reading?

#ifndef PRISM_TOUCH_POLL_DELAY
#define PRISM_TOUCH_POLL_DELAY   2    // how long does touch task suspend
#endif                              // between check for movement?

#ifndef PRISM_TOUCH_PROMPT_DELAY
#define PRISM_TOUCH_PROMPT_DELAY 100  // timer ticks to delay while prompting
#endif

///////////////////////////////////////////////////////////////////////////////
// TOUCH_STATE
//
// Calibration data and status for touch driver
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint16_t RawXVals[4];
    uint16_t RawYVals[4];
    pm_int_t    ScreenWidth;
    pm_int_t    ScreenHeight;
    pm_int_t    CenterX;
    pm_int_t    CenterY;
    pm_point_t  CurPos;
    pm_point_t  LastPos;
    uint16_t    LastMessage;
    pm_bool_t   PenDown;
    pm_bool_t   SwapAxis;
    pm_bool_t   IsCalibrated;
    pm_bool_t   IsInitialized;
} TOUCH_STATE;

///////////////////////////////////////////////////////////////////////////////
// TOUCH_RAW_POINT
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint16_t RawX;
    uint16_t RawY;
} TOUCH_CALIBRATION_POINT;

///////////////////////////////////////////////////////////////////////////////
// TOUCH_RAW_DATA
//
// Structure for setting the raw calibration data
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    TOUCH_CALIBRATION_POINT UL;
    TOUCH_CALIBRATION_POINT UR;
    TOUCH_CALIBRATION_POINT LR;
    TOUCH_CALIBRATION_POINT LL;
} TOUCH_RAW_DATA;

///////////////////////////////////////////////////////////////////////////////
// indexes into TOUCH_CAL_DATA
///////////////////////////////////////////////////////////////////////////////

#define TC_UL 0
#define TC_UR 1
#define TC_LR 2
#define TC_LL 3

#if defined(PRISM_TOUCH_SUPPORT_INCLUDE_CALIBRATION)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TouchEventPanel : public Pm_Panel
{
    public:
        TouchEventPanel(pm_int_t iLeft, pm_int_t iTop);
        void SetText(char *Event);

    private:
        Pm_ML_Prompt *mpTextWidget;
};
#endif


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Touch_Device : public Pm_Input_Device
{
    public:
        Pm_Touch_Device(void);
        virtual ~Pm_Touch_Device() {}

       #if defined(PRISM_TOUCH_TEST_MODE)
        void TouchTestMode(void);
       #endif

       #if defined(PRISM_TOUCH_SUPPORT_INCLUDE_CALIBRATION)
        virtual pm_bool_t Calibrate(void);
       #endif 
       
       //#if !defined(MINIMAL_TOUCH_DRIVER)
        virtual pm_bool_t GetLastPos(pm_point_t &PutPoint);
       //#endif

        pm_bool_t IsCalibrated(void) const
            { return mState.IsCalibrated; }

        pm_bool_t IsTouched(void) const
            { return mState.PenDown; }

	   #if !defined(MINIMAL_TOUCH_DRIVER)
        virtual pm_bool_t Poll(void);
        virtual void Run(void);
       #else
       	virtual void Run(void) = 0;
       #endif
       
       #if !defined(MINIMAL_TOUCH_DRIVER)
        virtual pm_bool_t SetRawCalibrations(TOUCH_RAW_DATA *pRawVals);
       #endif

    protected:

       #if !defined(MINIMAL_TOUCH_DRIVER)
        virtual pm_bool_t AverageSample(uint16_t &xRaw, uint16_t &yRaw);
        virtual void CheckSendTouchNotify(void);
       #endif

       #if defined(PRISM_TOUCH_SUPPORT_INCLUDE_CALIBRATION)
        virtual Pm_Widget  *CreateCalibrationTarget(void);
        virtual TouchEventPanel *CreateCalibrationPanel(void);
       #endif

       #if !defined(MINIMAL_TOUCH_DRIVER)
        virtual pm_bool_t DetectTouch(void) = 0;

       #if defined(PRISM_TOUCH_LINEAR_SCALE)
        virtual pm_int_t GetScaledXPos(uint16_t RawVal);
        virtual pm_int_t GetScaledYPos(uint16_t RawVal);
       #else
        virtual pm_int_t GetScaledXPos(uint16_t RawX, pm_int_t yVal);
        virtual pm_int_t GetScaledYPos(uint16_t RawY, pm_int_t xVal);
       #endif

        virtual pm_bool_t OneTouchSample(uint16_t &xRaw, uint16_t &yRaw) = 0;
       #endif

        #if defined(PRISM_TOUCH_TEST_MODE)
        virtual pm_bool_t OneRawTouchSample(pm_int_t &Reg0, pm_int_t &Reg1,
            pm_int_t &Reg2, pm_int_t &Reg3) = 0;
        #endif

       #if !defined(MINIMAL_TOUCH_DRIVER)
        virtual pm_point_t ScaleRawData(uint16_t xRaw, uint16_t yRaw);
       #endif

       #if defined(PRISM_TOUCH_SUPPORT_INCLUDE_CALIBRATION)
        virtual void WaitForTouchState(pm_bool_t PenDown);
       #endif 

        TOUCH_STATE mState;
};

///////////////////////////////////////////////////////////////////////////////
Pm_Touch_Device *CreatePrismTouchDevice(void);

#endif  

#endif  


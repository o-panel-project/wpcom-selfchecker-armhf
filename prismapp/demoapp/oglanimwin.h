///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



#ifndef __OGLANIMWIN_H__
#define __OGLANIMWIN_H__

#define SCALE_MAX_LIMIT 2.5     // ratio
#define SCALE_MIN_LIMIT 1.0	    // ratio
#define MIN_DRAG   10           // pixels
#define DRAG_STEP  3            // count
#define ZOOM_SPEED      1.5     // ratio
#define PHYSICAL_WIDTH  305.0
#define PHYSICAL_HEIGHT 235.0
#define DOUBLE_CLICK_THRESHOLD 200

///////////////////////////////////////////////////////////////////////////////
class oglAnimWin : public Pm_Panel
{
    public:

        oglAnimWin(const Pm_Region &Size, uint32_t Style);
        virtual pm_int_t Notify(const pm_event_t &Event);
		void show();
		void hide();

		// Flick api
        void FlickPicture(pm_bitmap_t *pNext, int xShift, int yShift);
		void FlickType(int);
		void AddStatusCurrent();
		void RemoveStatusCurrent();

		// Zoom api
		void ZoomEnable()
		{ mZoomEnable = TRUE; };
		void ZoomDisable()
		{ mZoomEnable = FALSE; };

		// Event Handler
        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);
        pm_int_t OnEventFocusIn(const pm_event_t &Event);
        pm_int_t OnEventFocusOut(const pm_event_t &Event);
        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventPan(const pm_event_t &Event);
        pm_int_t OnEventZoomIn(const pm_event_t &Event);
        pm_int_t OnEventZoomOut(const pm_event_t &Event);
        pm_int_t OnEventMultitouch(const pm_event_t &Event);
        pm_int_t OnEventFlickLeft(const pm_event_t &Event);
        pm_int_t OnEventFlickRight(const pm_event_t &Event);
        pm_int_t OnEventFlickUp(const pm_event_t &Event);
        pm_int_t OnEventFlickDown(const pm_event_t &Event);

		// test
		void AddItem(int);
		void AppQuit() { mParentAlive = FALSE; };

    protected:

		PrismX11PaintEngine *mpEngine;
        pm_int_t mCanvas;
		pm_uint_t mContext;

		// Zoom
        void CheckPanLimits(void);
        void CheckZoomLimits(void);
        void CalcZoomParam(const pm_event_t &Event);
		void ZoomReset()
		{
			mZoomPutPoint.Set(0, 0);
			mZoomRatio = 1.0;
			mZoomImage.Set(0, 0, mClient.Width() - 1, mClient.Height() - 1);
			mpEngine->oxx_AnimWin_zoom_reset(mContext);
		};

        pm_bool_t  mSingleTouch;
 
		pm_int_t   mScreenHRes;
		pm_int_t   mScreenVRes;
        pm_point_t mStartPan;
        pm_point_t mPenDown;
        pm_point_t mZoomPutPoint;
        pm_point_t mZoomCenter;
        pm_int_t   mZoomWidth;
        pm_int_t   mZoomHeight;
		float      mZoomRatio;
		Pm_Region  mZoomImage;
		pm_int_t   mZoomStep;
		pm_int_t   mPenUpTime;
		pm_bool_t  mZoomEnable;
		
	private:
		pm_bool_t  mParentAlive;
};
#endif // __OGLANIMWIN_H__

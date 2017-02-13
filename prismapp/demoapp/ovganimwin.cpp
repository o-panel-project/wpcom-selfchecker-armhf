///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#include "prism_includes.h"
#include "ovganimwin.h"

pm_event_table_entry ovgAnimWinEvents[] = {
    {PM_EVENT_SHOW, PM_EVENT_HANDLER(&ovgAnimWin::OnEventShow)},
    {PM_EVENT_HIDE, PM_EVENT_HANDLER(&ovgAnimWin::OnEventHide)},
    {PM_EVENT_GAINED_FOCUS, PM_EVENT_HANDLER(&ovgAnimWin::OnEventFocusIn)},
    {PM_EVENT_LOST_FOCUS, PM_EVENT_HANDLER(&ovgAnimWin::OnEventFocusOut)},
    {PM_EVENT_PEN_DRAG,   PM_EVENT_HANDLER(&ovgAnimWin::OnEventPan)},
    {PM_EVENT_PENDOWN,    PM_EVENT_HANDLER(&ovgAnimWin::OnEventPenDown)},
    {PM_EVENT_PENUP,      PM_EVENT_HANDLER(&ovgAnimWin::OnEventPenUp)},
    {PM_EVENT_ZOOM_IN,    PM_EVENT_HANDLER(&ovgAnimWin::OnEventZoomIn)},
    {PM_EVENT_ZOOM_OUT,   PM_EVENT_HANDLER(&ovgAnimWin::OnEventZoomOut)},
    {PM_EVENT_MULTITOUCH, PM_EVENT_HANDLER(&ovgAnimWin::OnEventMultitouch)},
    {0, NULL}   /* array terminator */
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  Constructor
//  Create top level object and add all children
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
ovgAnimWin::ovgAnimWin(const Pm_Region &Size, uint32_t Style) : 
    Pm_Panel(Size, Style)
	, mSingleTouch(FALSE)
	, mZoomStep(0)
	, mPenUpTime(0)
	, mZoomEnable(FALSE)
	, mParentAlive(TRUE)
{
	mpEngine = (PrismX11PaintEngine*)Pm_Framework::Engine();
	mContext = mpEngine->oxx_AnimWin_CreateContext(Size);
	mCanvas = mpEngine->CreateCanvas(PM_CANVAS_TYPE_SIMPLE,
		Size.Width(), Size.Height(), Size.Left, Size.Top, -1, NULL);
	mScreenHRes = Pm_Framework::Engine()->GetXRes();
	mScreenVRes = Pm_Framework::Engine()->GetYRes();
	ZoomReset();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::Notify(const pm_event_t &Event)
{
//	printf("ovgAnimWin::Notify() event = %d\n", Event.Type);

    pm_event_table_entry *pEventList = ovgAnimWinEvents;
    while(pEventList->EventType)
    {
        if (pEventList->EventType == Event.Type)
        {
            if (pEventList->Handler == NULL)
            {
                return 0;
            }
            return PM_CALL_EVENT_HANDLER(*this, pEventList->Handler)(Event);
        }
        pEventList++;
    }

    return Pm_Panel::Notify(Event);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::FlickPicture(pm_bitmap_t *pNext, int xShift, int yShift)
{
	mpEngine->oxx_flick_run(mContext, pNext, xShift, yShift);
	ZoomReset();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::AddItem(int show)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::FlickType(int iType)
{
	if (iType == 1)
		mpEngine->oxx_AnimWin_flickType(mContext, PM_ANIMWIN_FLICK_FLIP);
	else if (iType == 2)
		mpEngine->oxx_AnimWin_flickType(mContext, PM_ANIMWIN_FLICK_TURN);
	else
		mpEngine->oxx_AnimWin_flickType(mContext, PM_ANIMWIN_FLICK_CONT);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::AddStatusCurrent()
{
	mpEngine->oxx_AnimWin_AddStatus(mContext, PM_SF_CURRENT);
}
void ovgAnimWin::RemoveStatusCurrent()
{
	mpEngine->oxx_AnimWin_RemoveStatus(mContext, PM_SF_CURRENT);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::CheckPanLimits()
{
	pm_int_t imgWidth = mZoomImage.Width();
	pm_int_t imgHeight = mZoomImage.Height();

	// Right move limit
	if (mZoomPutPoint.x > 0)
	{
		mZoomPutPoint.x = 0;
	}

	// Down move limit
	if (mZoomPutPoint.y > 0)
	{
		mZoomPutPoint.y = 0;
	}

	// Left move limit
	if (PRISM_ABS(mZoomPutPoint.x) > (imgWidth - mClient.Width()))
	{
		mZoomPutPoint.x = mClient.Width() - imgWidth;
	}

	// Up move limit
	if (PRISM_ABS(mZoomPutPoint.y) > (imgHeight - mClient.Height()))
	{
		mZoomPutPoint.y = mClient.Height() - imgHeight;
	}

	// Image geometry reset
	mZoomImage.Set(mZoomPutPoint.x, mZoomPutPoint.y,
					mZoomPutPoint.x + imgWidth - 1,
					mZoomPutPoint.y + imgHeight - 1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::CheckZoomLimits()
{
	pm_int_t imgWidth = mZoomImage.Width();
	pm_int_t imgHeight = mZoomImage.Height();

	// Max zoom
	if (imgWidth > mClient.Width() * SCALE_MAX_LIMIT ||
		imgHeight > mClient.Height() * SCALE_MAX_LIMIT)
	{
		imgWidth = mClient.Width() * SCALE_MAX_LIMIT;
		imgHeight = mClient.Height() * SCALE_MAX_LIMIT;
		mZoomRatio = SCALE_MAX_LIMIT;
	}
	else
	{
		// Min zoom
		if (imgWidth < mClient.Width() * SCALE_MIN_LIMIT ||
			imgHeight < mClient.Height() * SCALE_MIN_LIMIT)
		{
			imgWidth = mClient.Width() * SCALE_MIN_LIMIT;
			imgHeight = mClient.Height() * SCALE_MIN_LIMIT;
			mZoomRatio = SCALE_MIN_LIMIT;
		}
		else
		{
			mZoomRatio = (float)imgWidth / (float)mClient.Width();
		}
	}

	// Left move limit
	if (mZoomPutPoint.x < 0 &&
		PRISM_ABS(mZoomPutPoint.x) > (imgWidth - mClient.Width()))
	{
		mZoomPutPoint.x = mClient.Width() - imgWidth;
	}

	// Right move limit
	if (mZoomPutPoint.x > 0)
		mZoomPutPoint.x = 0;

	// Up move limit
	if (mZoomPutPoint.y < 0 &&
		PRISM_ABS(mZoomPutPoint.y) > (imgHeight - mClient.Height()))
	{
		mZoomPutPoint.y = mClient.Height() - imgHeight;
	}

	// Down move limit
	if (mZoomPutPoint.y > 0)
		mZoomPutPoint.y = 0;

	// Image geometry reset
	mZoomImage.Set(mZoomPutPoint.x, mZoomPutPoint.y,
					mZoomPutPoint.x + imgWidth - 1,
					mZoomPutPoint.y + imgHeight - 1);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::CalcZoomParam(const pm_event_t &Event)
{
	pm_int_t newWidth, newHeight;
	float ratio_w, ratio_h, ratio;

    printf("Zoom> mZoomWidth: %d, mZoomHeight: %d, eWidth: %d, eHeight: %d\n",
        mZoomWidth, mZoomHeight, Event.Payload[0], Event.Payload[1]);

    float ScaleDist;	// move distance (pixel)

    pm_int_t DeltaX = Event.Payload[0];
    DeltaX -= mZoomWidth;	// move distance (mm)

    pm_int_t DeltaY = Event.Payload[1];
    DeltaY -= mZoomHeight;	// move distance (mm)

    if (PRISM_ABS(DeltaX) >= PRISM_ABS(DeltaY))
    {
		ScaleDist = mScreenHRes * DeltaX / PHYSICAL_WIDTH;
		ScaleDist *= ZOOM_SPEED;	// move bias
    }
    else
    {
		ScaleDist = mScreenVRes * DeltaY / PHYSICAL_HEIGHT;
		ScaleDist *= ZOOM_SPEED;	// move bias
    }
	printf("%s() DeltaX %d, DeltaY %d, Dist %f\n",
			__func__, DeltaX, DeltaY, ScaleDist);
	newWidth = mZoomImage.Width() + ScaleDist;
	newHeight = mZoomImage.Height() + ScaleDist;
	ratio_w = (float)newWidth / (float)mZoomImage.Width();
	ratio_h = (float)newHeight / (float)mZoomImage.Height();
	if (ratio_w < ratio_h) {
		ratio = ratio_w;
	} else {
		ratio = ratio_h;
	}

	newWidth = mZoomImage.Width() * ratio;
	newHeight = mZoomImage.Height() * ratio;

	// Image put position
	mZoomPutPoint.x -= (mZoomCenter.x * ratio - mZoomCenter.x);
	mZoomPutPoint.y -= (mZoomCenter.y * ratio - mZoomCenter.y);
	mZoomImage.Set(
		mZoomPutPoint.x,
		mZoomPutPoint.y,
		mZoomPutPoint.x + newWidth - 1,
		mZoomPutPoint.y + newHeight - 1);
	printf("Zoom> ratio %f put (%d, %d)-(%dx%d)\n", ratio,
			mZoomImage.Left, mZoomImage.Top,
			mZoomImage.Width(), mZoomImage.Height());
	CheckZoomLimits();
	printf("Zoom(Adjust)> ratio %f put (%d, %d)-(%dx%d)\n", mZoomRatio,
			mZoomImage.Left, mZoomImage.Top,
			mZoomImage.Width(), mZoomImage.Height());
	return;
}

///////////////////////////////////////////////////////////////////////////////
// Event Handler //////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventShow(const pm_event_t &Event)
{
	printf("%s() ---\n", __FUNCTION__);
	mpEngine->oxx_AnimWin_AddStatus(mContext, PM_SF_VISIBLE);
	mpEngine->oxx_AnimWin_show(mContext);
    return Pm_Panel::OnEventShow(Event);
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventHide(const pm_event_t &Event)
{
	printf("%s() ---\n", __FUNCTION__);
	if (!mParentAlive) return 0;
	mpEngine->oxx_AnimWin_RemoveStatus(mContext, PM_SF_VISIBLE);
	mpEngine->oxx_AnimWin_hide(mContext);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventFocusIn(const pm_event_t &Event)
{
	printf("%s() ---\n", __FUNCTION__);
	mpEngine->oxx_AnimWin_AddStatus(mContext, PM_SF_CURRENT);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventFocusOut(const pm_event_t &Event)
{
	printf("%s() ---\n", __FUNCTION__);
	mpEngine->oxx_AnimWin_RemoveStatus(mContext, PM_SF_CURRENT);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventPenDown(const pm_event_t &Event)
{
	printf("%s() ---\n", __FUNCTION__);

    mSingleTouch = TRUE;
    mStartPan = Event.Point;
    mPenDown = Event.Point;

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventPenUp(const pm_event_t &Event)
{
	printf("%s() - time %d-%d=%d, %d,%f\n",
			__FUNCTION__, Event.Param, mPenUpTime, Event.Param - mPenUpTime,
			mSingleTouch, mZoomRatio);

    mZoomWidth = -1;

    if (mSingleTouch) {
		if (mZoomRatio > 1.0) {
			if ((Event.Param > mPenUpTime) &&
				((Event.Param - mPenUpTime) < DOUBLE_CLICK_THRESHOLD)) {
				printf("%s() - Double click!\n", __FUNCTION__);
				ZoomReset();
				mpEngine->oxx_AnimWin_draw_zoom(
							mContext, mZoomPutPoint, mZoomRatio);
			}
		}
    }
	mPenUpTime = Event.Param;
	mZoomStep = 0;

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventMultitouch(const pm_event_t &Event)
{
	pm_int_t x, y;

    mSingleTouch = FALSE;
    mZoomWidth = -1;
	mZoomStep = 0;

    x = (mPenDown.x + Event.Point.x) / 2;	// prism geometry
    y = (mPenDown.y + Event.Point.y) / 2;
    mZoomCenter.x = x - mClient.Left;		// window relative
    mZoomCenter.y = y - mClient.Top;

	printf("%s() - Zoom center P(%d, %d) W(%d, %d)\n",
			__FUNCTION__, x, y, mZoomCenter.x, mZoomCenter.y);

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventPan(const pm_event_t &Event)
{
	pm_int_t DeltaX, DeltaY;

	mZoomStep = (mZoomStep + 1) % DRAG_STEP;
	if (mZoomStep != 0)
	{
		return 0;
	}

	if (mZoomRatio == 1.0) return 0;

	DeltaX = PRISM_ABS(Event.Point.x - mStartPan.x); 
	DeltaY = PRISM_ABS(Event.Point.y - mStartPan.y);
	if ((DeltaX > MIN_DRAG ||
		 DeltaY > MIN_DRAG) && mSingleTouch)
	{
		mZoomPutPoint.x += Event.Point.x - mStartPan.x;
        mZoomPutPoint.y += Event.Point.y - mStartPan.y;
		CheckPanLimits();
		printf("%s() Drag put (%d, %d)\n",
			__FUNCTION__, mZoomPutPoint.x, mZoomPutPoint.y);
        mStartPan = Event.Point;
       // mSingleTouch = FALSE;
		mpEngine->oxx_AnimWin_draw_zoom(mContext, mZoomPutPoint, mZoomRatio);
	//	mpEngine->ovg_AnimWin_draw_zoom(mContext, mZoomPutPoint, 1.0);
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventZoomIn(const pm_event_t &Event)
{
	if (!mZoomEnable) return 0;

    if (mZoomWidth == -1)
    {
        mZoomWidth = Event.Payload[0];
        mZoomHeight = Event.Payload[1];
		printf("%s(1) - Zoom rect (%d, %d)\n",
			__FUNCTION__, mZoomWidth, mZoomHeight);
    }
    else
    {
		printf("%s(n) - Zoom rect (%d, %d)\n",
			__FUNCTION__, Event.Payload[0], Event.Payload[1]);
        CalcZoomParam(Event);
		mpEngine->oxx_AnimWin_draw_zoom(mContext, mZoomPutPoint, mZoomRatio);
        mZoomWidth = Event.Payload[0];
        mZoomHeight = Event.Payload[1];
    }

	return 0;
}
///////////////////////////////////////////////////////////////////////////////
pm_int_t ovgAnimWin::OnEventZoomOut(const pm_event_t &Event)
{
	if (!mZoomEnable) return 0;

    if (mZoomWidth == -1)
    {
        mZoomWidth = Event.Payload[0];
        mZoomHeight = Event.Payload[1];
		printf("%s(1) - Zoom rect (%d, %d)\n",
			__FUNCTION__, mZoomWidth, mZoomHeight);
    }
    else
    {
		printf("%s(n) - Zoom rect (%d, %d)\n",
			__FUNCTION__, Event.Payload[0], Event.Payload[1]);
        CalcZoomParam(Event);
		mpEngine->oxx_AnimWin_draw_zoom(mContext, mZoomPutPoint, mZoomRatio);
        mZoomWidth = Event.Payload[0];
        mZoomHeight = Event.Payload[1];
    }

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
void ovgAnimWin::show()
{
	printf("ovgAnimWin_%s() ---\n", __FUNCTION__);
	mpEngine->oxx_AnimWin_AddStatus(mContext, PM_SF_VISIBLE);
	mpEngine->oxx_AnimWin_show(mContext);
}
void ovgAnimWin::hide()
{
	printf("ovgAnimWin_%s() ---\n", __FUNCTION__);
	mpEngine->oxx_AnimWin_hide(mContext);
	mpEngine->oxx_AnimWin_RemoveStatus(mContext, PM_SF_VISIBLE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#ifndef __MAINPANEL_H__
#define __MAINPANEL_H__

#include "ovganimwin.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class MainPanel : public Pm_Panel
{
    public:
        MainPanel();
        virtual pm_int_t Notify(const pm_event_t &Event);
		pm_int_t OnEventQuit(const pm_event_t &Event);

        pm_int_t OnEventFlick(const pm_event_t &Event);
        pm_int_t OnEventZoomIn(const pm_event_t &Event);
        pm_int_t OnEventZoomOut(const pm_event_t &Event);

		enum ChildIds {
			IDB_QUIT = 99,
		};

    protected:
        ovgAnimWin *mpPhotoFrame;
        Pm_Prompt *mpGesture;
        Pm_Prompt *mpSpeed;

//        pm_int_t OnEventFlick_m(const pm_event_t &Event);
        pm_int_t OnEventFlickLeft(const pm_event_t &Event);
        pm_int_t OnEventFlickRight(const pm_event_t &Event);
        pm_int_t OnEventFlickUp(const pm_event_t &Event);
        pm_int_t OnEventFlickDown(const pm_event_t &Event);
        void PaintPhotos(void);
        void FlickPicture(int xShift, int yShift);
        void UpdateGestureSpeed(const pm_event_t &Event);

        pm_bitmap_t *mpCurrentPhoto;
        pm_bitmap_t *mpNextPhoto;

        pm_int_t mFlickStepX;
        pm_int_t mFlickStepY;
        pm_int_t mFlickX;
        pm_int_t mFlickY;
        pm_int_t mPhotoIndex;
		pm_int_t mFlickDir;

		pm_bitmap_t *DecodeImage(char *);
//		pm_bool_t DecodeImage(char *);
//		pm_int_t ListingImage(char *);
	private:
		Pm_Text_Btn *mpBtnQuit;
};
#endif // __MAINPANEL_H__


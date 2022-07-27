///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <dirent.h>
#include <unistd.h>
#include "prism_includes.h"
#include "mainpanel.h"

#define NUM_MAX_PHOTOS 10
#define FLICK_STEP_X 32 
#define FLICK_STEP_X_HI 32 
#define FLICK_STEP_X_LO 32
#define FLICK_STEP_Y 25
#define PHOTO_TIMER 1

static int lcd_width = 1024;
static int lcd_height = 600;
static int win_width = 1024;
static int win_height = 600;
static int win_x = 0;
static int win_y = 0;

pm_int_t ArgImageNum;
//char *ArgImageName[NUM_MAX_PHOTOS];
char *pWallpaper = NULL;
char **pImageNameList = NULL;
char *pImageFolder = NULL;

pm_bitmap_t **menuPhoto;
pm_bitmap_t *pWallpaperBitmap = NULL;

///////////////////////////////////////////////////////////////////////////////
pm_event_table_entry MainPanelEvents[] = {

    {PM_EVENT_TIMEOUT, PM_EVENT_HANDLER(&MainPanel::OnEventFlick)},
	{PM_EVENT_ZOOM_IN, PM_EVENT_HANDLER(&MainPanel::OnEventZoomIn)},
	{PM_EVENT_ZOOM_OUT,PM_EVENT_HANDLER(&MainPanel::OnEventZoomOut)},

	{PM_EVENT_ST_PAN_UP,    PM_EVENT_HANDLER(&MainPanel::OnEventFlick)},
	{PM_EVENT_ST_PAN_DOWN,  PM_EVENT_HANDLER(&MainPanel::OnEventFlick)},
	{PM_EVENT_ST_PAN_LEFT,  PM_EVENT_HANDLER(&MainPanel::OnEventFlick)},
	{PM_EVENT_ST_PAN_RIGHT, PM_EVENT_HANDLER(&MainPanel::OnEventFlick)},

	{PM_CEN(MainPanel::IDB_QUIT, PM_EVENT_CLICK),
							PM_EVENT_HANDLER(&MainPanel::OnEventQuit)},

    {0, NULL}   /* array terminator */
};

static pm_int_t ListingImage(char *);
extern int prismmain(int, char**, key_t, const Pm_Region *);

int main(int argc, char *argv[])
{
	pm_int_t i, skip = 0;
	int opt;

	while ((opt = getopt(argc, argv, "b:d:i:x:y:z:")) != -1) {
		switch (opt) {
		case 'b':
			pWallpaper = strdup(optarg);
			break;
		case 'i':
			pImageFolder = strdup(optarg);
			break;
		case 'x':
			win_x = atoi(optarg);
			break;
		case 'y':
			win_y = atoi(optarg);
			break;
		case 'd':
		case 'z':
			printf("%s option %c ignored.\n", argv[0], (char)opt);
			break;
		default:
			printf("Usage: %s -b bg -i jpegdir\n", argv[0]);
			return 0;
		}
	}
	if (pImageFolder == NULL)
		pImageFolder = strdup(".");

	ArgImageNum = ListingImage(pImageFolder);
	if (ArgImageNum == 0) {
		free(pImageFolder);
		if (pWallpaper) free(pWallpaper);
		printf("Usage: %s jpeg not found.\n", argv[0]);
		return 0;
	}
	printf("%d jpeg file found.\n", ArgImageNum);

	menuPhoto = (pm_bitmap_t **) new pm_bitmap_t*[ArgImageNum];

	Display *dpy = XOpenDisplay(NULL);
	if (dpy) {
		int scr = DefaultScreen(dpy);
		lcd_width = DisplayWidth(dpy, scr);
		lcd_height = DisplayHeight(dpy, scr);
		XCloseDisplay(dpy);
	}
	Pm_Region appRect;
	appRect.Set(0, 0, lcd_width - 1, lcd_height - 1);
	prismmain(argc, argv, 0, NULL);//&appRect);

	delete pImageNameList;
	delete menuPhoto;
	free(pImageFolder);
	if (pWallpaper) free(pWallpaper);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PrismAppStart(Pm_Screen_Manager *pScreen)
{
    Pm_Input_Device::ReadyInputDevices();
    MainPanel *pPanel = new MainPanel();
    pScreen->Link(pPanel);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  Constructor
//  Create top level object and add all children
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
MainPanel::MainPanel() : 
    Pm_Panel(PM_BORDER_NONE)
{
    Pm_Region ChildSize;
	pm_int_t i;

	if (pWallpaper)
		pWallpaperBitmap = DecodeImage(pWallpaper);
	for (i = 0; i < ArgImageNum; i++)
		menuPhoto[i] = DecodeImage(pImageNameList[i]);
	win_width = menuPhoto[0]->Width;
	win_height = menuPhoto[0]->Height;

    mSize.Set(0, 0, lcd_width - 1, lcd_height - 1);
    InitializeClientRegion();
    SetColor(PM_CI_NORMAL, CID_BTN_BORDER1_FOC);
    AddStatus(PM_SF_MOVEABLE|PM_SF_SIZEABLE);
	if (pWallpaperBitmap)
		SetWallpaper(pWallpaperBitmap);

	printf("Animation window %dx%d+%d+%d.\n",
			win_width, win_height, win_x, win_y);
    ChildSize.Set(win_x, win_y, win_x + win_width - 1, win_y + win_height - 1);
    mpPhotoFrame = new oglAnimWin(ChildSize, PM_BORDER_NONE);
    mpPhotoFrame->RemoveStatus(PM_SF_MOVEABLE|PM_SF_SIZEABLE);
    mpPhotoFrame->SetColor(PM_CI_NORMAL, CID_BLACK);
    mpPhotoFrame->SetWallpaper(menuPhoto[0]);
	mpPhotoFrame->ZoomEnable();
    Link(mpPhotoFrame);

    mPhotoIndex = 0;
    mFlickDir = 0;
	mpPhotoFrame->show();

	ChildSize.Set(lcd_width -100 -10, lcd_height -50 -10,
			lcd_width -10, lcd_height -10);
	mpBtnQuit = new Pm_Text_Btn(ChildSize, 0, IDB_QUIT);
	mpBtnQuit->AssignText("Quit");
	Link(mpBtnQuit);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::Notify(const pm_event_t &Event)
{
    pm_event_table_entry *pEventList = MainPanelEvents;

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

/*
 * -Event Handlers-
 */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventQuit(const pm_event_t &Event)
{
	mpPhotoFrame->AppQuit();
	return -PM_EVENT_TERMINATE;
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventFlick(const pm_event_t &Event)
{
	static int FlickTypeLR = 0;
	static int FlickTypeUD = 0;

	switch (Event.Type) {
	case PM_EVENT_ST_PAN_LEFT:
		mpPhotoFrame->FlickType(FlickTypeLR);
		OnEventFlickLeft(Event);
		FlickTypeLR = (FlickTypeLR + 1) % 3;
		break;
	case PM_EVENT_ST_PAN_UP:
		mpPhotoFrame->FlickType(FlickTypeUD);
		OnEventFlickUp(Event);
		FlickTypeUD = (FlickTypeUD + 1) % 2;
		break;
	case PM_EVENT_ST_PAN_RIGHT:
		mpPhotoFrame->FlickType(FlickTypeLR);
		OnEventFlickRight(Event);
		FlickTypeLR = (FlickTypeLR + 1) % 3;
		break;
	case PM_EVENT_ST_PAN_DOWN:
		mpPhotoFrame->FlickType(FlickTypeUD);
		OnEventFlickDown(Event);
		FlickTypeUD = (FlickTypeUD + 1) % 2;
		break;
	default:
		break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventFlickLeft(const pm_event_t &Event)
{
	if (Event.Param < 100)
    	FlickPicture(-FLICK_STEP_X_HI, 0);    
	else
    	FlickPicture(-FLICK_STEP_X_LO, 0);    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventFlickRight(const pm_event_t &Event)
{
	if (Event.Param < 100)
    	FlickPicture(FLICK_STEP_X_HI, 0);    
	else
    	FlickPicture(FLICK_STEP_X_LO, 0);    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventFlickUp(const pm_event_t &Event)
{
    FlickPicture(0, -FLICK_STEP_Y);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventFlickDown(const pm_event_t &Event)
{
    FlickPicture(0, FLICK_STEP_Y);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void MainPanel::FlickPicture(int xShift, int yShift)
{
	mPhotoIndex++;
	if (mPhotoIndex >= ArgImageNum) {
    	mPhotoIndex = 0;
	}
	mpPhotoFrame->SetWallpaper(menuPhoto[mPhotoIndex]);
    mpPhotoFrame->FlickPicture(menuPhoto[mPhotoIndex], xShift, yShift);
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventZoomIn(const pm_event_t &Event)
{
    printf("MainPanel::%s() -----\n", __func__);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
pm_int_t MainPanel::OnEventZoomOut(const pm_event_t &Event)
{
    printf("MainPanel::%s() -----\n", __func__);
    return 0;
}

/*
 * -Utilities-
 */

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
pm_bitmap_t *MainPanel::DecodeImage(char *pPathname)
{
	pm_bitmap_t *p;
	Pm_File ImageFile;
	ImageFile.Open(pPathname, PRISM_FILEMODE_RO|PRISM_FILEMODE_BIN);
    if (!ImageFile.IsOpen())
    {
        // unable to open source image
        return NULL;
    }
	Pm_Graphic_Reader *pReader = new Pm_Jpg_Reader(0);
	pReader->SetPalStart(0);
    if (!pReader->ReadImage(&ImageFile))
    {
		delete pReader;
        return NULL;
    }
	pReader->SetSystemPalette(NULL, TRUE_ALPHA_COLORS);
	pReader->SetMode(pReader->GetMode() | PM_GR_MODE_RGB_ORDER);
	pReader->RemapBitmap(FALSE);

	p = pReader->GetBitmapPointer(0);

	delete pReader;
	printf("Decode %s done.\n", pPathname);
	return p;
}

///////////////////////////////////////////////////////////////////////////////
static pm_int_t ListingImage(char *pPathname)
{
	DIR *dir_ptr;
	struct dirent *entry;
	char *ext;
	int count = 0, i, ret;
	char path[256];

	if ((dir_ptr = opendir(pPathname)) == NULL)
		return 0;
	while ((entry = readdir(dir_ptr)) != NULL) {
		ext = strrchr(entry->d_name, '.');
		if (ext != NULL && (strcmp(ext, ".jpg") == 0))
			count++;
	}
	closedir(dir_ptr);
	if (count == 0)
		return 0;
//	printf("%s(): %d jpeg file found.\n", __func__, count);

	pImageNameList = (char **) new char*[count];

	dir_ptr = opendir(pPathname);
	i = 0;
	while ((entry = readdir(dir_ptr)) != NULL) {
		ext = strrchr(entry->d_name, '.');
		if (ext != NULL && (strcmp(ext, ".jpg") == 0)) {
			sprintf(path, "%s/%s", pPathname, entry->d_name);
			pImageNameList[i] = strdup(path);;
			printf("Input image[%d] %s\n", i, pImageNameList[i]);
			i++;
		}
	}
	closedir(dir_ptr);
	return count;
}

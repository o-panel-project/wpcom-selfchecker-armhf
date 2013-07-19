
#ifndef _PRISM_MENU_
#define _PRISM_MENU_

#define PRISM_MENU_CHK_WIDTH 8

#define MENU_GRADIENT_SLICE 32

enum PM_BI_MENUBTN_BMPS {
    PM_BI_MENU_LEFT = 0,    // Normal left icon
    PM_BI_MENU_PUSHED,      // Pushed left icon
    PM_BI_MENU_BACK,        // background
    NUM_MENU_BTN_ICONS
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
enum
{
    PM_CI_MENU_BACK = 0,
    PM_CI_MENU_BACK_SEL,
    PM_CI_MENU_TEXT,
    PM_CI_MENU_TEXT_SEL,
    PM_CI_MENU_GRAD1,
    PM_CI_MENU_GRAD2,
    PM_CI_MENU_SEP1,
    PM_CI_MENU_SEP2,
    MAX_MENU_COLORS
};


///////////////////////////////////////////////////////////////////////////////
struct Pm_Menu_Description
{
    pm_uint_t TextId;
    uint16_t Id;

    uint32_t Style;

    pm_uint_t LeftIconId;       // icon displayed to left of text (or 0)
    pm_uint_t PushedIconId;     
    pm_uint_t NormalIconId; 

    Pm_Menu_Description *pSubMenu;
};

///////////////////////////////////////////////////////////////////////////////
struct Pm_Dyn_Menu_Description
{
    const pm_char_t *pText;
    uint16_t Id;

    uint32_t Style;

    pm_uint_t LeftIconId;       // icon displayed to left of text (or 0)
    pm_uint_t PushedIconId;     // overrides Left icon when pushed (or 0)
    pm_uint_t NormalIconId;     // bitmap behind text area (or 0)

    Pm_Dyn_Menu_Description *pSubMenu;
};

class Pm_Menu_Btn;      

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Menu : public Pm_Panel
{
    public:
        Pm_Menu(void);
        Pm_Menu(Pm_Menu_Description *pDesc, pm_bool_t PopUp = FALSE);
        Pm_Menu(Pm_Dyn_Menu_Description *pDesc, pm_bool_t PopUp = FALSE);

        void AssignMenuDescription(Pm_Menu_Description *pDesc);
        void AssignMenuDescription(Pm_Dyn_Menu_Description *pDesc);
        
        void CloseSiblings(Pm_Menu_Btn *NotMe);
        Pm_Menu_Btn *FindButton(const pm_char_t *pWho);
        Pm_Region GetMinSize(void);
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventGainedFocus(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
        virtual void MenuKeyHandler(pm_int_t Key, int32_t Flags);
       #endif

        virtual void Paint(const Pm_Region &Invalid);
        void SetOwner(Pm_Widget *pWho);

    protected:

        void CommonMenuInit(void);
        void PositionButtons(void);
};




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Menu_Bar : public Pm_Widget
{
    public:
        Pm_Menu_Bar(Pm_Menu_Description *pDesc);
        Pm_Menu_Bar(Pm_Dyn_Menu_Description *pDesc);

        virtual ~Pm_Menu_Bar();

        virtual void CloseSiblings(Pm_Menu_Btn *NotMe);
        virtual void Paint(const Pm_Region &Invalid);
        Pm_Menu_Btn *FindButton(const pm_char_t *pWho);
        virtual pm_bool_t InFlyoverMode(void) {return mIsHover;}
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventShow(const pm_event_t &Event);
        pm_int_t OnEventParentSized(const pm_event_t &Event);
        pm_int_t OnEventLostFocus(const pm_event_t &Event);

       #ifdef PRISM_KEYPAD_SUPPORT
        pm_int_t OnEventKeypadPush(const pm_event_t &Event);
       #endif

        pm_int_t OnEventNewLanguage(const pm_event_t &Event);

        virtual void SetFlyoverMode(pm_bool_t State) {mIsHover = State;}

    protected:
       #ifdef PRISM_KEYPAD_SUPPORT
        virtual void MenuKeyHandler(pm_int_t Key, int32_t Flags);
       #endif

        void CommonMenuBarInit(void);
        virtual void PositionButtons(void);
        virtual void SizeToParent(void);

        pm_bool_t mIsHover;

       #ifdef PRISM_GRADIENT_SUPPORT
        pm_gradient_t mGradient;
       #endif
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Pm_Menu_Btn : public Pm_Text_Widget
{
    friend class Pm_Menu_Bar;

    public:
        Pm_Menu_Btn(Pm_Menu_Description *pDesc);
        Pm_Menu_Btn(Pm_Dyn_Menu_Description *pDesc);

        virtual ~Pm_Menu_Btn();

        virtual void AssignBitmap(pm_uint_t Index, pm_uint_t BitmapId);
        virtual void CloseMenu(void);

        void Disable();
        void Enable();

        virtual Pm_Widget *Find(uint16_t Id, pm_bool_t Recursive = TRUE);
        Pm_Menu_Btn *FindButton(const pm_char_t *pWho);
        virtual Pm_Region GetMinSize(uint16_t Type);
        Pm_Widget *GetOwner(void) {return mpOwner;}
        Pm_Menu *GetSubMenu(void) {return mpSubMenu;}

        virtual pm_bool_t IsDivider(void)
        {
            if (mStyle & PM_BTN_DIVIDER)
            {
                return TRUE;
            }
            return FALSE;
        }

        virtual pm_bool_t IsPushed(void)
        {
            if (mStyle & PM_BTN_PUSHED)
            {
                return TRUE;
            }
            return FALSE;
        }

        pm_bool_t IsSubVisible(void) {return mSubVisible;}
        pm_bool_t IsPointerOver(void) {return mPointerOver;}
        virtual pm_int_t Notify(const pm_event_t &Event);

        pm_int_t OnEventPenDown(const pm_event_t &Event);
        pm_int_t OnEventPenUp(const pm_event_t &Event);
        pm_int_t OnEventHide(const pm_event_t &Event);
        pm_int_t OnEventMouseOver(const pm_event_t &Event);
        pm_int_t OnEventMouseNotOver(const pm_event_t &Event);
        pm_int_t OnEventNewLanguage(const pm_event_t &Event);

        virtual void Paint(const Pm_Region &Invalid);

        virtual void SetPushed(pm_bool_t State);
        virtual void SetOwner(Pm_Widget *pWho);
        void SetSubMenu(Pm_Menu *pMenu);

    protected:

        void AddSubMenu(pm_bool_t Vertical);
        void CommonMenuBtnInit(uint32_t Style, void *pSub);
        void NotifyOwner(void);
        void RemoveSiblingChecks(void);
        virtual void UnselectSiblings(void);

        pm_uint_t mIconTable[NUM_MENU_BTN_ICONS];

        Pm_Menu *mpSubMenu;
        Pm_Widget *mpOwner;
        pm_bool_t mSubVisible;
        pm_bool_t mPointerOver;
};

#endif


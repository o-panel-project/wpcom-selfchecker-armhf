/*  --------------------------------------------------------------------  */
/*  Wireless LAN Environment Measurement Tool   (GUI)                     */
/*  --------------------------------------------------------------------  */
/*  Function    : Reads information from target files and displays        */
/*                        the data graphically.                           */
/*  --------------------------------------------------------------------  */
/*  History     : 2012.02.06 (v1.0) -  First release                      */
/*  --------------------------------------------------------------------  */


// バージョン
#define VER_MAJOR 1
#define VER_MINOR 1


// defined values

#define EVT_MSK   ButtonReleaseMask|ButtonPressMask
#define SB_WIDTH  80        // Button Width
#define SB_HEIGHT 30        // Button Height
#define SB_CURSOR XC_hand1  // Cursor for inside MenuButton

#define   POSX   0
#define   POSY   0
#define   WIDTH  1024
#define   HEIGHT 600
#define   NG     -1
#define   OK     0

// buttons
#define   ENVT_START  0
#define   D_START     1
#define   D_STOP      2
#define   N_START     3
#define   N_STOP      4
#define   TIME_1      5
#define   TIME_2      6
#define   TIME_3      7
#define   D_RESET     8
#define   N_RESET     9
#define   PREVIOUS   10
#define   NEXT       11
// 使用禁止          12
#define   QUIT       13
#define   START_SCAN 14
#define   STOP_SCAN  15

// colors
#define C_BLACK     "black"
#define C_WHITE     "white"
#define C_GRAY      "gray50"
#define C_RED       "red"
#define C_YELLOW    "yellow"
#define C_GREEN     "sea green"

#define LOG_FILE    "results.dat"

#define CH_CANCEL   13
#define CH_SELECTED 14


// -- Include Libraries

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h> 
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>
#include <X11/cursorfont.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wchar.h>
#include <pango/pangoxft.h>

#include "ReadWLAN.h"


//  -- GLOBAL VARIABLES

Display *dpy;
Window   w;
Window   popUp;
Window   win[16];
XftDraw *draw;
XftDraw *popUp_draw;
XftDraw *win_draw[16];
XEvent   event,event2;
int      screen;
GC       gc;
Visual  *visual;
Colormap colmap;
Cursor tempcursor;
XSetWindowAttributes w_attr;
XFontSet fs;
XftColor color_black;
XftColor color_green;
XftColor color_red;
XftColor color_yellow;
PangoFontDescription *fd;
PangoContext *context;
PangoLayout  *layout;


int ntest_time=0,dtest_time=0;
int test_error=0;
int id, run_once =0, isDataOK=0;
int ProcessFlg=0, ErrorButton=0, net_interval =0; 
int network_test=0, dwnstream_test=0, res=0, ConfirmButton=0;

int  TstRunning=0;
char param[5][256];
char *info[22];

#if 1
char *page1[32] = {
		"無線 LAN 環境測定モード",  // 0
		"接続先",        // 1
		"接続状況",      // 2
		"WEP",           // 3
		"チャンネル",    // 4
		"電界強度",      // 5
		"スループット",  // 6
		"エラーレート",  // 7
		"他のチャンネルの電界強度",  // 8
		"スタート",      // 9
		"ストップ",      // 10
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "10",  // 20
		"11",            // 21
		"12",            // 22
		"13",            // 23
		"[チャンネル]",  // 24
		"MAC アドレス",  // 25
		"->",            // 26
		"AP MAC",        // 27
		"WLAN MAC",      // 28
		"スキャン開始",  // 29
		"スキャン停止",  // 30
		"終了"           // 31
	};
char *page2[14] = {
		"無線 LAN 性能測定モード",  // 0
		"ダウンストリームレート",   // 1
		"ネットワークディレイ",     // 2
		"(平均)",                   // 3
		"(最低)",                   // 4
		"(最高)",                   // 5
		"ダウンストリームテスト",   // 6
		"ネットワークテスト",       // 7
		"ダウンストリームテストのリセット",  // 8
		"ネットワークテストのリセット",      // 9
		"<-",                       // 10
		"テスト間隔",               // 11
		"測定結果",                 // 12
		"(現在)"                    // 13
	};
char *judgement[7] = {
		"合格",    // 0
		"不合格",  // 1
		"確認",    // 2
		"接続",    // 3
		"未接続",  // 4
		"使用",    // 5
		"未使用"   // 6
	};
#define MSG_SECOND     "秒"
#define MSG_PROCESSING "処理中"
#define MSG_MEASURING  "測定中"
#define MSG_ERROR      "エラー"
#define MSG_RESULT_FILE_READ_ERROR "測定結果ファイルが読み込めません"
#define MSG_TEST_FILE_READ_ERROR   "テストファイルの読み込みエラー"
#define MSG_DS_TEST_CANNOT_START   "ダウンストリームテストを開始できません"
#define MSG_NW_TEST_CANNOT_START   "ネットワークテストを開始できません"
#define MSG_FAILING                "不合格"
#define MSG_IFUP                   "Initialize …"
#define MSG_NOIP                   "IPアドレスが取得できていません"
#else
char *page1[32] = {
		"WLAN Environmental measurement mode", "destination", "status", "WEP", "channel",
		"field intensity", "throughput", "erorr rate", "other channel field intensity", "start",
		"stop", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",  "11", "12", "13",
		"[channel]", "MAC address", "->", "AP MAC", "WLAN MAC", "scan start", "scan stop", "Quit"
	};
char *page2[14] = {
		"WLAN Performance measurement mode", "down stream rate", "network delay",
		"(average)", "(min)", "(max)", "down stream test", "network test",
		"reset down stream", "reset network test", "<-",
		"test interval", "result of a measurement", "(current)"
	};
char *judgement[7] = {
		"passing", "failing", "confirm", "connect", "unconnect", "use", "unuse"
	};
#define MSG_SECOND     "sec"
#define MSG_PROCESSING "processing"
#define MSG_MEASURING  "measuring"
#define MSG_ERROR      "Error"
#define MSG_RESULT_FILE_READ_ERROR "Reading error of measurement result file."
#define MSG_TEST_FILE_READ_ERROR   "Reading error of test file."
#define MSG_DS_TEST_CANNOT_START   "A downstream test cannot start. "
#define MSG_NW_TEST_CANNOT_START   "A network test cannot start. "
#define MSG_FAILING                "failing"
#endif

#define EXIT_BUTTON_ON "exit_button_on"


//Function Prototypes

void EnvtPage();
void PerformPage();
void UpdateUI(int x, char *data);
void ClearFields();
void ClearWindow();
int  ParseFile();
int  ValidateData(int idNo);
void ResetFlags();
int  CheckButtons();
void ShadeForm(Window x, XftDraw *draw, int dimension[6], char *data, XftColor *color, char *bgColor);
void MakeButton(int win_num, int dimension[6], char *data);
void MakePopUpMsg(char *Title, char *Message);
void MakePopUpMsg2(char *Title, char *Message);
void MakePopUpMsgWithConfirmButton(char *Title, int posTitle, char *Message, int posMessage);
void TimeButton(int btn_num);
void ResetTests(int tstNum);
void StopScanButton(char *c_c1, char* c_c2, char* c_c3);
unsigned long PickColor(char *color);
int  reset_testfile2();
int  reset_testfile3();
int  ReadParamFile();
void removeLF(char *string);
void DrawPangoXftRenderLayout(XftDraw *draw, XftColor *color, PangoLayout *layout, int x, int y, char *str);
void ExecIfip();
void ExecIfipPage();


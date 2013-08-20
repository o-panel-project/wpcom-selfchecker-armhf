/*	--------------------------------------------------------------------------- */
/*	Wireless LAN Environment Measurement Tool	(GUI)			    */
/*	--------------------------------------------------------------------------- */
/*	Function	: Reads information from target files and displays          */
/*                        the data graphically.                     		    */
/*	Author          : Diana Dee Tan (AWS Systems,Inc)           		    */
/*	Contact         : dianadee.tan@awsys-i.com				    */
/*	Last update     : 2006.11.02           					    */
/*	--------------------------------------------------------------------------- */
/*	History	   : 2006.11.22 (v2.0) -  Added Scan Other Channels Function	    */
/*	             2006.11.02 (v1.0) -  First release				    */
/*	--------------------------------------------------------------------------- */


#include "WLANtool.h"
#include "common.h"
#include "wpcio.h"


typedef struct tag_Rect {
	int x;
	int y;
	int w;
	int h;
} Rect;

Rect ds_status_coor = {.x=620, .y= 95, .w=10, .h=10};  // ダウンストリームレートステータス表示座標
Rect nw_status_coor = {.x=620, .y=165, .w=10, .h=10};  // ネットワークディレイステータス表示座標

void DrawStatusIndicator(Display *d, Window w, GC gc, Rect *rect, char *bgColor);

int g_board_type = WPC_BOARD_TYPE_J;

/*************************************************************************************
** Function:     Main Function
** Description:  Controls the Flow of the Program
** Argument:     None
** Return:       None
*************************************************************************************/
int main(int ac, char *av[])
{
	int z=0, a=0, scan_time=0, StartFlag=0;
	int StartButton = 0, NextButton = 0, ScanButton = 0, QuitButton = 0;
	int abc[6]   = {0, 0,  90, 40, 15, 10};
	int reset[6] = {0, 0, 330, 40,  0, 10};
	int coor[6];
	char tmp[3], *msg;
	struct tm *t;
	time_t ltime;
	FILE *fp;
	char fontname[256];
	char command[256];
	int ret;
	
printf("wlantool build(%s,%s)\n",__DATE__,__TIME__);

	signal(SIGINT,  (void *)int_handler);
	signal(SIGHUP,  (void *)int_handler);
	signal(SIGQUIT, (void *)int_handler);
	signal(SIGTERM, (void *)int_handler);
	
	g_board_type = sc_get_board_type();
	printf("wlantool execute on j%d-PANEL\n", g_board_type);

	// 設定ファイル読み込み（先頭５つ）
	if (ReadParamFile() != OK) {
		printf("\nError in reading Config File\n");
		return NG;
	}
	
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "Could not open display\n");
		exit(1);
	}
	
	screen  = DefaultScreen(dpy);
	w       = XCreateSimpleWindow(dpy, RootWindow(dpy,screen),
				POSX, POSY, WIDTH, HEIGHT, 3,
				BlackPixel(dpy,screen),
				WhitePixel(dpy,screen) );
	gc      = XCreateGC(dpy, w, 0, 0);
	visual  = DefaultVisual(dpy, screen);
	colmap  = DefaultColormap(dpy, screen);
	draw    = XftDrawCreate(dpy, w, visual, colmap);
	
	XSelectInput(dpy, w, StructureNotifyMask);
	
	XMapWindow(dpy, w);
	
	// Wait till screen is displayed
	for (;;) {
		XNextEvent(dpy, &event);
		if (event.type == MapNotify) {
			break;
		}
	}
	
	XSetForeground(dpy, gc, BlackPixel(dpy,screen));
	XSetBackground(dpy, gc, WhitePixel(dpy,screen));
	
	// フォント作成
	if (ac == 2) {
		strcpy(fontname, av[1]);
	} else {
		strcpy(fontname, "DFPHSGothic-W7 12");
	}
	fd = pango_font_description_from_string(fontname);
	printf("font name %s\n", pango_font_description_to_filename(fd));
	
	// 色設定
	XftColorAllocName(dpy, visual, colmap, C_BLACK,  &color_black);
	XftColorAllocName(dpy, visual, colmap, C_GREEN,  &color_green);
	XftColorAllocName(dpy, visual, colmap, C_RED,    &color_red);
	XftColorAllocName(dpy, visual, colmap, C_YELLOW, &color_yellow);
	
	// pango layoutの作成
	context = pango_xft_get_context(dpy, screen);   // コンテキスト作成
	layout  = pango_layout_new(context);            // レイアウト作成
	pango_layout_set_font_description(layout, fd);  // フォントの指定
	
	// ifup実行、初期化画面表示
	ExecIfipPage();
	ExecIfip();
	ClearWindow();
	
	// メイン画面表示
	EnvtPage();
	
envt_mode:
	
	// Check Events from Buttons in Environment Page
	for (;;) {
		XCheckWindowEvent(dpy, win[ENVT_START], EVT_MSK, &event);
		if (event.type == ButtonRelease) {
			StartButton = ON;
			event.type  = 0;
			break;
		}
		
		XCheckWindowEvent(dpy, win[NEXT], EVT_MSK, &event2);
		if (event2.type == ButtonRelease) {
			NextButton  = ON;
			event2.type = 0;
			break;
		}
		
		XCheckWindowEvent(dpy, win[START_SCAN], EVT_MSK, &event);
		if (event.type == ButtonRelease) {
			ScanButton = ON;
			event.type = 0;
			break;
		}
		
		if (win[QUIT] != 0) {
			XCheckWindowEvent(dpy, win[QUIT], EVT_MSK, &event);
			if (event.type == ButtonRelease) {
				QuitButton = ON;
				event.type = 0;
				break;
			}
		}
	}
	
	// Start of Environment Mode
	if (StartButton) {
		StartButton = OFF;
		
		// IP取得確認
		if (!IsGetIP()) {
			// 取得できてない画面表示
			MakePopUpMsgWithConfirmButton(MSG_ERROR, 140, MSG_NOIP, 45);
			EnvtPage();
			goto envt_mode;
		}
		
		StartFlag = ON;
		isDataOK = OK;
		MakePopUpMsg(MSG_PROCESSING, MSG_MEASURING);
		XftDrawDestroy(popUp_draw);
		XDestroyWindow(dpy, popUp);
		get_environment_info(0);
		if (test_error == 3) {
			MakePopUpMsg2(MSG_ERROR, MSG_RESULT_FILE_READ_ERROR);
			ClearFields();
			EnvtPage();
			ConfirmButton = OFF;
			test_error=NO_ERROR;
			goto envt_mode;
		}
		
		//display Pop up Msg at least 2 seconds
		if (ParseFile() == NG) {
			ConfirmButton = ErrorButton = ON;
			MakePopUpMsg(MSG_ERROR, MSG_TEST_FILE_READ_ERROR);
			EnvtPage();
			ConfirmButton = OFF;
			goto envt_mode;
		}
		//Display read values graphically
		ClearFields();
		EnvtPage();
		for (z=0; z<=id;z++) {
			res= ValidateData(z);
			printf("\n#%d  %s",z,info[z]);
			UpdateUI(z, info[z]);
		}
		ConfirmButton = ON;
		if (isDataOK == OK) {
			msg = judgement[0];
		} else {
			msg = judgement[1];
		}
		MakePopUpMsg(page2[12], msg);
		
		fp = fopen(param[1],"w");
		time(&ltime);
		t=localtime(&ltime);
		fprintf(fp,"\n\nDate & Time : %02d-%02d-%02d %02d:%02d:%02d	  \t",
				  t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
		fprintf(fp,"\nResults: ");
//		fprintf(fp,msg);
		fprintf(fp,"\n-------------------------\n");
		fprintf(fp,"SSID:%s\n",info[0]);
		fprintf(fp,"ASSOC:%s\n",info[1]);
		fprintf(fp,"WEP:%s\n",info[2]);
		fprintf(fp,"MYCH:%s\n",info[3]);
		fprintf(fp,"MAC:%s\n",info[4]);
		fprintf(fp,"RSSI:%s\n",info[5]);
		fprintf(fp,"THRU:%s\n",info[6]);
		fprintf(fp,"ERATE:%s\n",info[7]);
		fprintf(fp,"CH1:%s\n",info[8]);
		fprintf(fp,"CH2:%s\n",info[9]);
		fprintf(fp,"CH3:%s\n",info[10]);
		fprintf(fp,"CH4:%s\n",info[11]);
		fprintf(fp,"CH5:%s\n",info[12]);
		fprintf(fp,"CH6:%s\n",info[13]);
		fprintf(fp,"CH7:%s\n",info[14]);
		fprintf(fp,"CH8:%s\n",info[15]);
		fprintf(fp,"CH9:%s\n",info[16]);
		fprintf(fp,"CH10:%s\n",info[17]);
		fprintf(fp,"CH11:%s\n",info[18]);
		fprintf(fp,"CH12:%s\n",info[19]);
		fprintf(fp,"CH13:%s\n",info[20]);
		fclose(fp);
		
		//Update screen after Confirmed Button is pressed
		EnvtPage();
		for(z=0; z<=id;z++){
			res= ValidateData(z);
			UpdateUI(z, info[z]);
		}
		ConfirmButton =OFF;
		for (z=0;z<=id;z++) {
			free(info[z]);
		}
		goto envt_mode;
	} //endifStart
	
/*Additional Function*/
	
	if (ScanButton) {
		ScanButton = OFF;
		StopScanButton(C_GRAY, C_GRAY, C_WHITE);
		scan_time = get_time(); 
		for (;;) {
			XCheckWindowEvent(dpy,win[15], EVT_MSK, &event2);  // A1114
			if (event2.type == ButtonRelease) {
				event2.type = 0;
				printf("\nStop Scan\n");
				StopScanButton(C_WHITE, C_WHITE, C_GRAY);
				break;
			}
			
			if (test_error == 3) {
				ConfirmButton = ErrorButton = ON;
				MakePopUpMsg("エラー","Error in Scanning Channels");
				ConfirmButton = OFF;
				ClearFields();
				EnvtPage(); 
				StopScanButton(C_WHITE, C_WHITE, C_GRAY);
				if (StartFlag == ON) {
					StartFlag = OFF;
					ParseFile();
					for (z=0; z<=8; z++) {
						res = ValidateData(z);
						UpdateUI(z, info[z]);
					}
				}
				goto envt_mode;
			}
			
			if ( (diff_time(scan_time,get_time()) >= 1000) && get_environment_info (1) != NG) {
				scan_time=get_time();
				if (ParseFile() == NG) {
					printf("\nERROR READING FILE!!!\n");
				}
				XSetForeground(dpy, gc, WhitePixel(dpy,screen));
				XFillRectangle(dpy, w, gc, 66, 316, 549,199);
				XSetForeground(dpy, gc, BlackPixel(dpy,screen));
				for (z=315; z<515; z+=25) {
					XDrawLine(dpy,w,gc,65,z,615,z);
				}
				XFlush(dpy);
				for (z=9; z<=21; z++) {
					res = ValidateData(z);
					UpdateUI(z, info[z]);
				}
				for (z=0; z<=id; z++) {
					free(info[z]);
				}
			}  // endif
		}  // endfor
		
		XSync(dpy,True);
		goto envt_mode;
	}
	
	if (NextButton) {
		fp = fopen(param[3], "w");  // Initialize testfiles
		fprintf(fp,"0");
		fclose(fp);
		fp = fopen(param[4], "w");
		fprintf(fp,"0,0,0");
		fclose(fp);
		NextButton = 0;
		XftDrawDestroy(win_draw[ENVT_START]);
		XftDrawDestroy(win_draw[NEXT]);
		XftDrawDestroy(win_draw[START_SCAN]);
		XftDrawDestroy(win_draw[STOP_SCAN]);
		if (win_draw[QUIT] != NULL) {
			XftDrawDestroy(win_draw[QUIT]);
		}
		XDestroyWindow(dpy,win[ENVT_START]);
		XDestroyWindow(dpy,win[NEXT]);
		XDestroyWindow(dpy,win[START_SCAN]);
		XDestroyWindow(dpy,win[STOP_SCAN]);
		if (win[QUIT] != 0) {
			XDestroyWindow(dpy,win[QUIT]);
		}
		run_once=0;
		
		// Start of Performance Mode
		ClearWindow();
		PerformPage();
performance_mode:
		for (;;) {
			a=CheckButtons();
			
			// Buttons Pressed. Corresponding functions called.
			if (a==D_START && dwnstream_test==OFF) {
				// IP取得確認
				if (!IsGetIP()) {
					// 取得できてない画面表示
					MakePopUpMsgWithConfirmButton(MSG_ERROR, 140, MSG_NOIP, 45);
					PerformPage();
					goto performance_mode;
				}
				
				dtest_time = get_time();
				ShadeForm(win[D_START], win_draw[D_START], abc, page1[9],  &color_black, C_GRAY);
				ShadeForm(win[D_STOP],  win_draw[D_STOP],  abc, page1[10], &color_black, C_WHITE);
				reset[4] = 35;
				ShadeForm(win[D_RESET], win_draw[D_RESET], reset, page2[8], &color_black, C_GRAY);
				coor[0] = 401; coor[1] = 81; coor[2] = 209; coor[3] = 39;
				coor[4] = 490; coor[5] = 90;
				dwnstream_test=ON;
				dtest_time = get_time();
				start_downstream();
			}
			
			if( (a==D_STOP && dwnstream_test==ON)){
				dwnstream_test=OFF;
				stop_downstream();
				ShadeForm(win[D_STOP],  win_draw[D_STOP],  abc, page1[10], &color_black, C_GRAY);
				ShadeForm(win[D_START], win_draw[D_START], abc, page1[9],  &color_black, C_WHITE);
				reset[4]=35;
				ShadeForm(win[D_RESET], win_draw[D_RESET], reset, page2[8], &color_black, C_WHITE);
			}
			
			if (a==N_START && net_interval!=OFF && network_test==OFF){
				// IP取得確認
				if (!IsGetIP()) {
					// 取得できてない画面表示
					MakePopUpMsgWithConfirmButton(MSG_ERROR, 140, MSG_NOIP, 45);
					PerformPage();
					goto performance_mode;
				}
				
				ShadeForm(win[N_START], win_draw[N_START], abc, page1[9],  &color_black, C_GRAY);
				ShadeForm(win[N_STOP],  win_draw[N_STOP],  abc, page1[10], &color_black, C_WHITE);
				reset[4]=50;
				ShadeForm(win[N_RESET], win_draw[N_RESET], reset,page2[9], &color_black, C_GRAY);
				network_test=ON;
				dtest_time = get_time();
				start_network();
			}
			
			if (a==N_STOP && network_test==ON) {
				network_test=OFF;
				stop_network();
				net_interval=0;
				ShadeForm(win[N_STOP],  win_draw[N_STOP],  abc,   page1[10], &color_black, C_GRAY);
				ShadeForm(win[N_START], win_draw[N_START], abc,   page1[9],  &color_black, C_WHITE);
				reset[4]=50;
				ShadeForm(win[N_RESET], win_draw[N_RESET], reset, page2[9],  &color_black, C_WHITE);
				for (z=5;z<8;z++) {
					sprintf(tmp,"%d" MSG_SECOND,z-4);
					coor[0] = coor[1] = 0; coor[2] = 40; coor[3] = 30;
					coor[4] = 7; coor[5] = 5;
					ShadeForm(win[z], win_draw[z], coor, tmp, &color_black, C_WHITE);
				}
			}
			
//		   Time Interval for Network Test (1s,2s,3s)
			if (a==TIME_1 && network_test == OFF) {
				net_interval=1;
				TimeButton(net_interval);
			}
			if (a==TIME_2 && network_test == OFF) {
				net_interval=2;
				TimeButton(net_interval);
			}
			if (a==TIME_3 && network_test == OFF) {
				net_interval=3;
				TimeButton(net_interval);
			}
			
			if(a==D_RESET && dwnstream_test==OFF) {
				reset_testfile2();
				ResetTests(1);
			}
			if (a==N_RESET && network_test==OFF) {
				reset_testfile3();
				ResetTests(2);
			}
			if (a == PREVIOUS) {
				if (dwnstream_test == 1) {
					stop_downstream();
				}
				if (network_test == 1) {
					stop_network();
				}
				ResetFlags();
				ProcessFlg=0;
				for (z=1; z<11; z++) {
					XftDrawDestroy(win_draw[z]);
					XDestroyWindow(dpy,win[z]);
				}
				ClearWindow();
				break;
			}
			
			//If error occurs while Downstream Test Ongoing
			if (test_error == DOWN_ERROR && dwnstream_test == ON) {
//				printf("\ndownstream Test Error\n");
				test_error=NO_ERROR;
				DrawStatusIndicator(dpy, w, gc, &ds_status_coor, C_WHITE);
				if (TstRunning!=1) {
					stop_downstream();
				}
				dwnstream_test=OFF;
				ConfirmButton=ErrorButton=ON; 
				MakePopUpMsg(MSG_ERROR, MSG_DS_TEST_CANNOT_START);
				PerformPage();
				ConfirmButton =OFF;
				ShadeForm(win[D_STOP],  win_draw[D_STOP],  abc,   page1[10], &color_black, C_GRAY);
				ShadeForm(win[D_START], win_draw[D_START], abc,   page1[9],  &color_black, C_WHITE);
				reset[4]=35;
				ShadeForm(win[D_RESET], win_draw[D_RESET], reset, page2[8],  &color_black, C_WHITE);
				goto performance_mode;
			}
			
			// Read from Downstream File & Update GUI every second
			if (test_error != DOWN_ERROR && dwnstream_test == ON && 
				(diff_time(dtest_time,get_time())) >= 1000) {
				dtest_time	= get_time();
				ProcessFlg=1;
				if (ParseFile() == NG) {
					ProcessFlg=0;
					test_error = DOWN_ERROR;
					goto performance_mode;
				}
				coor[0] = 401; coor[1] = 81; coor[2] = 209; coor[3] = 39;
				coor[4] = 490; coor[5] = 90;
				ShadeForm(w, draw, coor, strcat(info[0]," Kbps"), &color_black, C_WHITE);
				info[0]=NULL;
				free(info[0]);
				
				// ダウンロードストリームテストステータス確認
				{
					int dsfp;
					dsfp = open(FTPTEMPFILE, O_RDONLY|O_NONBLOCK, (mode_t)0777);
					if (dsfp < 0) {
						// ファイルが読めない場合はエラー
						DrawStatusIndicator(dpy, w, gc, &ds_status_coor, C_WHITE);
					} else {
						int ret;
						read(dsfp, &ret, sizeof(ret));
						close(dsfp);
						if (ret == -1) {
							// ファイルに-1が入っていた場合はエラー
							DrawStatusIndicator(dpy, w, gc, &ds_status_coor, C_WHITE);
						} else {
							DrawStatusIndicator(dpy, w, gc, &ds_status_coor, C_GREEN);
						}
					}
				}
			}
			
			// If error occurs while Network Test Ongoing
			if (test_error == NET_ERROR && network_test== ON ) {
//				printf("\nNetwork Test Error\n");
				test_error = NO_ERROR;
#if 0
				if (TstRunning != 2) {
					stop_network();
				}
				network_test =OFF;
				ShadeForm(win[N_STOP],  win_draw[N_STOP],  abc, page1[10], &color_black, C_GRAY);
				ShadeForm(win[N_START], win_draw[N_START], abc, page1[9],  &color_black, C_WHITE);
				reset[4]=50;
				ShadeForm(win[N_RESET], win_draw[N_RESET], reset, page2[9], &color_black, C_WHITE);
				ConfirmButton=ErrorButton=ON;
				MakePopUpMsg(MSG_ERROR, MSG_NW_TEST_CANNOT_START);
				ConfirmButton = OFF;
				for (z=5;z<8;z++) {
					sprintf(tmp,"%d" MSG_SECOND,z-4);
					coor[0] = coor[1] = 0; coor[2] = 40; coor[3] = 30;
					coor[4] = 7; coor[5] = 5;
					ShadeForm(win[z], win_draw[z], coor, tmp, &color_black, C_WHITE);
				}
				goto performance_mode;
#endif
			}
			
			// Read from Network File  & Update GUI every second
			if(network_test == ON && test_error != NET_ERROR &&
				(diff_time(ntest_time,get_time())) >= 1000) {
				ntest_time = get_time();
				ProcessFlg=2;
				if (ParseFile() == NG) {
					test_error = NET_ERROR;
					ProcessFlg=0;
					goto performance_mode;
				}
				coor[0] = 401; coor[1] = 151; coor[2] = 209; coor[3] = 39;
				coor[4] = 490; coor[5] = 160;
				ShadeForm(w, draw, coor, strcat(info[0]," msec"), &color_black, C_WHITE);
				coor[0] = 401; coor[1] = 201; coor[2] = 209; coor[3] = 39;
				coor[4] = 490; coor[5] = 210;
				ShadeForm(w, draw, coor, strcat(info[1]," msec"), &color_black, C_WHITE);
				coor[0] = 401; coor[1] = 251; coor[2] = 209; coor[3] = 39;
				coor[4] = 490; coor[5] = 260;
				ShadeForm(w, draw, coor,strcat(info[2]," msec"), &color_black, C_WHITE);
				info[0]=NULL;
				info[1]=NULL;
				info[2]=NULL;
				free(info[0]);
				free(info[1]);
				free(info[2]);
				
				{
					int nwfp;
					nwfp = open(TESTFILE3, O_RDONLY|O_NONBLOCK, (mode_t)0777);
					if (nwfp < 0) {
						// ファイルが読めない場合はエラー
						DrawStatusIndicator(dpy, w, gc, &nw_status_coor, C_WHITE);
					} else {
						char buf[32];
						read(nwfp, buf, sizeof(buf));
						close(nwfp);
						if (buf[0] == '0') {
							// ファイルに-1が入っていた場合はエラー
							DrawStatusIndicator(dpy, w, gc, &nw_status_coor, C_WHITE);
						} else {
							DrawStatusIndicator(dpy, w, gc, &nw_status_coor, C_GREEN);
						}
					}
				}
			} //endif
		}
		
		//Go back to Environment Mode if <- Button Pressed
		run_once=0;
		EnvtPage();
		goto envt_mode;
	}
	
	// From QuitButton
	ret = ReadOneConfig(21, command);
	if (ret != -1) {
		printf("exec:%s\n", command);
		system(command);
	}
	
	return OK;
}


/**********************************************************************************
** Function:     EnvtPage
** Description:  Draws the Environment Mode GUI
** Argument:     None
** Return:       None
************************************************************************************/
void EnvtPage()
{
	int a,z;
	int xyz[6]={0,0,0,0,0,0};
	int ret;
	char command[256];
	char str[256];
	
	//draw Main Window for Tool
	XDrawRectangle(dpy, w, gc, 5 ,5, 1012, 590);
	
	//draw Border for Window Title
	sprintf(str, "%s (v%d.%d)", page1[0], VER_MAJOR, VER_MINOR);
	DrawPangoXftRenderLayout(draw, &color_black, layout,   15, 15, str);  // 無線 LAN 環境測定モード
	
	XDrawLine(dpy, w, gc, 5, 40, 1016, 40);
	
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45,  60, page1[ 1]);  // 接続先
	DrawPangoXftRenderLayout(draw, &color_black, layout, 410,  90, page1[ 2]);  // 接続状況
	DrawPangoXftRenderLayout(draw, &color_black, layout, 410,  60, page1[ 3]);  // WEP
	DrawPangoXftRenderLayout(draw, &color_black, layout, 410, 120, page1[ 4]);  // チャンネル
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45,  90, page1[27]);  // AP MAC
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45, 120, page1[28]);  // WLAN MAC
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45, 170, page1[ 5]);  // 電界強度
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45, 200, page1[ 6]);  // スループット
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45, 230, page1[ 7]);  // エラーレート
	DrawPangoXftRenderLayout(draw, &color_black, layout,  45, 290, page1[ 8]);  // 他のチャンネルの電界強度
	
	
	//Draw Boxes
	
	XDrawRectangle(dpy, w, gc, 140,  55, 250, 25);  // 接続先
	XDrawRectangle(dpy, w, gc, 500,  55, 210, 25);  // WEP
	XDrawRectangle(dpy, w, gc, 140,  85, 250, 25);  // AP MAC
	XDrawRectangle(dpy, w, gc, 500,  85, 210, 25);  // 接続状況
	XDrawRectangle(dpy, w, gc, 140, 115, 250, 25);  // WLAN MAC
	XDrawRectangle(dpy, w, gc, 500, 115, 210, 25);  // チャンネル
	
	// 電界強度
	XDrawRectangle(dpy, w, gc, 180, 165, 210, 25);
	XDrawRectangle(dpy, w, gc, 410, 165, 305, 25);
	XDrawLine(dpy, w, gc, 450.5, 195, 450.5, 220);
	for (a=450.5; a<677; a+=37.5) {
		XDrawLine(dpy, w, gc, a, 165, a, 190);
	}
	
	// スループット
	XDrawRectangle(dpy, w, gc, 180, 195, 210, 25);
	XDrawRectangle(dpy, w, gc, 410, 195, 305, 25);
	XDrawLine(dpy, w, gc, 450.5, 195, 450.5, 220);
	for (a=450.5; a<677; a+=37.5) {
		XDrawLine(dpy, w, gc, a, 195, a, 220);
	}
	
	// エラーレート
	XDrawRectangle(dpy, w, gc, 180, 225, 210, 25);
	XDrawRectangle(dpy, w, gc, 410, 225, 305, 25);
	XDrawLine(dpy, w, gc, 443, 225, 443, 250);
	for (a=443; a<685; a+=30) {
		XDrawLine(dpy, w, gc, a, 225, a, 250);  //error rate
	}
	
	// 他のチャンネルの電界強度
	XDrawRectangle(dpy, w, gc, 65, 315, 550, 200);
	for (a=315; a<515; a+=25) {
		XDrawLine(dpy, w, gc, 65, a, 615, a);  //inner lines of ScanArea
	}
	a = 89;
	for (z=11; z<25; z++) {
		DrawPangoXftRenderLayout(draw, &color_black, layout, a, 515, page1[z]);
		a += 40;
	}
	
	if (run_once == 0) {
		// 「スタート」ボタン
		xyz[0] = 300; xyz[1] = 550; xyz[2] = 90; xyz[3] = 30;
		xyz[4] =  15; xyz[5] =   5;
		MakeButton(ENVT_START, xyz, page1[9]);
		
		// 「->」ボタン
		xyz[0] = 720; xyz[1] = 550; xyz[2] = 50; xyz[3] = 30;
		xyz[4] =  15; xyz[5] =   5;
		MakeButton(NEXT, xyz, page1[26]);
		
		// 「スキャン開始」ボタン
		xyz[0] = 640; xyz[1] = 380; xyz[2] = 120; xyz[3] = 30;
		xyz[4] =  11; xyz[5] =   5;
		MakeButton(START_SCAN, xyz, page1[29]);
		
		// 「スキャン停止」ボタン
		xyz[0] = 640; xyz[1] = 420; xyz[2] = 120; xyz[3] = 30;
		xyz[4] =  11; xyz[5] =   5;
		MakeButton(STOP_SCAN, xyz, page1[30]);
		
		// 「終了」ボタン
		ret = ReadOneConfig(22, command);
		if (ret != -1) {
			if (strcmp(command,EXIT_BUTTON_ON) == 0) {
				xyz[0] = 914; xyz[1] = 550; xyz[2] = 90; xyz[3] = 30;
				xyz[4] =  30; xyz[5] =   5;
				MakeButton(QUIT, xyz, page1[31]);
			}
		}
		run_once = 1;
	}
	
	xyz[0] =  0; xyz[1] = 0; xyz[2] = 120; xyz[3] = 30;
	xyz[4] = 11; xyz[5] = 5;
	ShadeForm(win[STOP_SCAN], win_draw[STOP_SCAN], xyz, page1[30], &color_black, C_GRAY);  // スキャン停止
}


/***********************************************************************************
** Function:     PerformPage()
** Description:  Draws the Performance Mode GUI
** Argument:     None
** Return:       None
*************************************************************************************/
void PerformPage()
{
	int xyz[6] = {0,0,0,0,0,0};
	
	XDrawRectangle(dpy, w, gc, 5 ,5, 1012, 590);
	// draw Border for Window Title
	
	DrawPangoXftRenderLayout(draw, &color_black, layout, 15, 15, page2[0]);  // 無線 LAN 性能測定モード
	XDrawLine(dpy, w, gc, 5, 40,1016, 40);
	
	// Start Text Modes
	
	DrawPangoXftRenderLayout(draw, &color_black, layout, 120,  90, page2[ 1]);  // ダウンストリームレート
	DrawPangoXftRenderLayout(draw, &color_black, layout, 120, 160, page2[ 2]);  // ネットワークディレイ
	DrawPangoXftRenderLayout(draw, &color_black, layout, 300,  90, page2[13]);  // (現在)
	DrawPangoXftRenderLayout(draw, &color_black, layout, 300, 160, page2[ 3]);  // (平均)
	DrawPangoXftRenderLayout(draw, &color_black, layout, 300, 210, page2[ 4]);  // (最低)
	DrawPangoXftRenderLayout(draw, &color_black, layout, 300, 260, page2[ 5]);  // (最高)
	DrawPangoXftRenderLayout(draw, &color_black, layout,  25, 360, page2[ 6]);  // ダウンストリームテスト
	DrawPangoXftRenderLayout(draw, &color_black, layout, 420, 360, page2[ 7]);  // ネットワークテスト
	DrawPangoXftRenderLayout(draw, &color_black, layout, 430, 415, page2[11]);  // テスト間隔
	
	XDrawRectangle(dpy, w, gc, 400,  80, 210, 40);  // ダウンストリームレート(現在)
	XDrawRectangle(dpy, w, gc, 400, 150, 210, 40);  // ネットワークディレイ(平均)
	XDrawRectangle(dpy, w, gc, 400, 200, 210, 40);  // ネットワークディレイ(最低)
	XDrawRectangle(dpy, w, gc, 400, 250, 210, 40);  // ネットワークディレイ(最高)
	
	// ダウンストリームレートエラー表示
	DrawStatusIndicator(dpy, w, gc, &ds_status_coor, C_WHITE);
	// ネットワークディレイエラー表示
	DrawStatusIndicator(dpy, w, gc, &nw_status_coor, C_WHITE);
	
	XFlush(dpy);
	
	if (run_once == 0) {
		// 「スタート」ボタン
		xyz[0] = 210; xyz[1] = 350; xyz[2] = 90; xyz[3] = 40;
		xyz[4] =  15; xyz[5] =  10;
		MakeButton(1, xyz, page1[9]);
		
		// 「ストップ」ボタン
		xyz[0] = 310; xyz[1] = 350;
		MakeButton(2, xyz, page1[10]);
		xyz[0] =  0; xyz[1] = 0; xyz[2] = 90; xyz[3] = 40;
		xyz[4] = 15; xyz[5] = 10;
		ShadeForm(win[D_STOP], win_draw[D_STOP], xyz ,page1[10], &color_black, C_GRAY);
		
		// 「スタート」ボタン
		xyz[0] = 580; xyz[1] = 350;
		MakeButton(3, xyz, page1[9]);
		
		// 「ストップ」ボタン
		xyz[0] = 680; xyz[1] = 350;
		MakeButton(4, xyz, page1[10]);
		xyz[0] =  0; xyz[1] =  0; xyz[2] = 90; xyz[3] = 40;
		xyz[4] = 15; xyz[5] = 10;
		ShadeForm(win[N_STOP], win_draw[N_STOP], xyz, page1[10], &color_black, C_GRAY);
		
		// 「1秒」ボタン
		xyz[0] = 530; xyz[1] = 410; xyz[2] = 40; xyz[3] = 30;
		xyz[4] =   7; xyz[5] =  5;
		MakeButton(5, xyz, "1" MSG_SECOND);
		
		// 「2秒」ボタン
		xyz[0] = 580; xyz[1] = 410;
		MakeButton(6, xyz, "2" MSG_SECOND);
		
		// 「3秒」ボタン
		xyz[0] = 630; xyz[1] = 410;
		MakeButton(7, xyz, "3" MSG_SECOND);
		
		// 「ダウンストリームテストのリセット」ボタン
		xyz[0] = 60; xyz[1] = 460; xyz[2] = 330; xyz[3] = 40;
		xyz[4] = 35; xyz[5] =  10;
		MakeButton(8, xyz, page2[8]);
		
		// 「ネットワークテストのリセット」ボタン
		xyz[0] = 420; xyz[1] = 460;
		xyz[4] =  50; xyz[5] =  10;
		MakeButton(9, xyz, page2[9]);
		
		// 「<-」ボタン
		xyz[0] = 35; xyz[1] = 550; xyz[2] = 50; xyz[3] = 30;
		xyz[4] = 15; xyz[5] =  5;
		MakeButton(10, xyz, page2[10]);
		
		run_once=1;
	}
}


/*************************************************************************************
** Function:   UpdateUI
** Description: Updates the GUI with the values read from the testfile.
** Argument:    int f - specifies which field to be updated
**              char *data - the data to be displayed in the GUI
** Return:      None
*************************************************************************************/
void UpdateUI(int f, char *data)
{
	int tbuf = 0;
	char *C_bg, *C_fill2;
	XftColor *C_txt, *C_fill;
	int xy[6] = {0,0,0,0,0,0};
	
	switch(f) {
	case 0:  // 接続先
		xy[0] = 141; xy[1] = 56; xy[2] = 249; xy[3] = 24;
		xy[4] = 150; xy[5] = 60;
		if (res == OK) {
			ShadeForm(w, draw, xy, data, &color_green, C_WHITE);
		} else {
			if (strcmp(data,"@NoAP@") == 0) {
				ShadeForm(w, draw, xy, "", &color_black, C_RED);
			} else {
				if(strlen(data) < 33)  isDataOK = OK;
				ShadeForm(w, draw, xy, data, &color_black, C_RED);
			}  // endelse
		}
		break;
		
	case 1:  // 接続状況
		xy[0] = 501; xy[1] = 86; xy[2] = 209; xy[3] = 24;
		xy[4] = 584; xy[5] = 90;
		if (res == OK) {
			if (!strcmp(data,"1")) {
				ShadeForm(w, draw, xy, judgement[3], &color_green, C_WHITE);
			} else {
				ShadeForm(w, draw, xy, judgement[4], &color_black, C_RED);
				isDataOK = NG;
			}
		} else {
			ShadeForm(w, draw, xy, "", &color_red, C_RED);
		}
		break;
		
	case 2:  // WEP
		xy[0] = 501; xy[1] = 56; xy[2] = 209; xy[3] = 24;
		xy[4] = 584; xy[5] = 60;
		if (res == OK) {
			if (!strcmp(data,"1")) {
				ShadeForm(w, draw, xy, judgement[5], &color_green, C_WHITE);
			} else {
				ShadeForm(w, draw, xy, judgement[6], &color_black, C_RED);
				isDataOK = NG;
			}
		} else {
			ShadeForm(w, draw, xy, "", &color_red, C_RED);
		}
		break;
		
	case 3:  // チャンネル
		if (ConfirmButton != ON) {
			strcat(data," ch");
		}
		xy[0] = 501; xy[1]= 116; xy[2] = 209; xy[3] = 24;
		xy[4] = 584; xy[5]= 120;
		if (res == OK) {
			ShadeForm(w, draw, xy, data, &color_green, C_WHITE);
		} else {
			ShadeForm(w, draw, xy, "", &color_red, C_RED);
		}
		break;
	
	case 4:  // AP MAC
		if (res == OK) {
			DrawPangoXftRenderLayout(draw, &color_black, layout, 202, 90, data);
		} else {
			xy[0] = 141; xy[1] = 86; xy[2] = 249; xy[3] = 24;
			xy[4] = 202; xy[5] = 90;
			ShadeForm(w, draw, xy, "", &color_black, C_RED);
		}
		XFlush(dpy);
		break;
	
	case 5:  // WLAN MAC
		if (res == OK) {
			DrawPangoXftRenderLayout(draw, &color_black, layout, 202, 120, data);
		} else {
			xy[0] = 141; xy[1] = 116; xy[2] = 249; xy[3] = 24;
			xy[4] = 202; xy[5] = 120;
			ShadeForm(w, draw, xy, "", &color_black, C_RED);
		}
		XFlush(dpy);
		break;
		
	case 6:  // 電界強度
		if (ConfirmButton != ON) {
			strcat(data," dBm");
		}
		tbuf = abs(atoi(data));
		if (res == OK) {
			if (tbuf >= 94) {  //M1115
				C_txt   = &color_black;
				C_bg    = C_RED;
				C_fill  = &color_red;
				C_fill2 = C_RED;
				isDataOK = NG;
			}
				if (tbuf >= 86 && tbuf <=93) {  // M1115
					C_txt   = &color_black;
					C_bg    = C_YELLOW;
					C_fill  = &color_yellow;
					C_fill2 = C_YELLOW;
				}
				if (tbuf >=20 && tbuf <=85) {  // M1115
					C_txt   = &color_green;
					C_bg    = C_WHITE;
					C_fill  = &color_green;
					C_fill2 = C_GREEN;
				}
			} else {
				if (tbuf < 20){
				tbuf=20;
				C_txt   = &color_green;
				C_bg    = C_WHITE;
				C_fill  = &color_green;
				C_fill2 = C_GREEN;
				isDataOK = OK;
			}
			if (tbuf >100 ) {
				tbuf=100;
				C_txt   = &color_black;
				C_bg    = C_RED;
				C_fill  = &color_red;
				C_fill2 = C_RED;
			}
		}
		xy[0] = 181; xy[1] = 166; xy[2] = 209; xy[3] = 24;
		xy[4] = 265; xy[5] = 170;
		ShadeForm(w, draw, xy, data, C_txt, C_bg);
		xy[0] = xy[4] = 413; xy[1] = xy[5] = 168; xy[2] = (100-tbuf)*3.75; xy[3] = 20;
		ShadeForm(w, draw, xy, "", C_fill, C_fill2);
		break;
		
	case 7:  // スループット
		if (ConfirmButton !=ON) {
			strcat(data," Kbps");
		}
		tbuf = atoi(data);
		if (tbuf > 10000) {
			tbuf = 10000;  // A1123
		}
		if (res == OK) {
			if (tbuf >= 800) {
				C_txt  = &color_green;
				C_bg  = C_WHITE;
				C_fill = &color_green;
				C_fill2 = C_GREEN;
			}  // M115
			if (tbuf >=0 && tbuf < 800) {
				C_txt  = &color_black;
				C_bg   = C_RED;
				C_fill = &color_red;
				C_fill2 = C_RED;
				isDataOK = NG;
			}
		} else {
			if (tbuf < 0 ) {
				tbuf = 0;
			}
			if (tbuf > 10000) {
				tbuf = 10000;
			}
			C_txt  = &color_black;
			C_bg   = C_RED;
			C_fill = &color_red;
			C_fill2 = C_RED;
		}
		xy[0] = 181; xy[1] = 196; xy[2] = 209; xy[3] = 24;
		xy[4] = 265; xy[5] = 200;
		ShadeForm(w, draw, xy, data, C_txt, C_bg); 
		xy[0] = xy[4] = 413; xy[1] = xy[5] = 198; xy[2] = tbuf*0.03; xy[3] = 20;
		ShadeForm(w, draw, xy, "", C_fill, C_fill2);
		break;
		
	case 8:  // エラーレート
		tbuf = atoi(data);
		if (ConfirmButton !=ON ) {
			strcat(data, " %");
		}
		if (res == OK) {
			if (tbuf >= 50) {
				C_txt   = &color_black; 
				C_bg    = C_RED;
				C_fill  = &color_red;
				C_fill2 = C_RED;
				isDataOK = NG;
			}
			if (tbuf >= 20 && tbuf < 50) {
				C_txt   = &color_black;
				C_bg    = C_YELLOW;
				C_fill  = &color_yellow;
				C_fill2 = C_YELLOW;
			}
			if (tbuf < 20) {
				C_txt = C_fill = &color_green;
				C_fill2 = C_GREEN;
				C_bg    = C_WHITE;
			}
		} else {
			if (tbuf > 10 && tbuf <=100) {
				res = OK;
			}
			if (tbuf > 100) {
				tbuf = 100;
			}
			C_txt   = &color_black;
			C_bg    = C_RED;
			C_fill  = &color_red;
			C_fill2 = C_RED;
		}
		xy[0] = 181; xy[1] = 226; xy[2] = 209; xy[3] = 24;
		xy[4] = 265; xy[5] = 230;
		ShadeForm(w, draw, xy,data, C_txt, C_bg); 
		xy[0] = xy[4] = 413; xy[1] = xy[5] = 228; xy[2] = tbuf*3; xy[3] = 20;
		ShadeForm(w, draw, xy,"", C_fill, C_fill2);
		break;
		
	default:  // 他のチャンネルの電界強度
		if ( f >=9 && f < 22) {
			tbuf=abs(atoi(data));
			if (tbuf%2 == 1 ) tbuf+=1;  // debug
			if (res == OK) {
				XFillRectangle(dpy,w,gc,85+(40*(f-9)),515-((100-tbuf)*2.5),15,(100-tbuf)*2.5);
			} else {
				if(tbuf > 100) tbuf = 100;
				if(tbuf < 20) tbuf = 20;
				XFillRectangle(dpy,w,gc,85+(40*(f-9)),515-((100-tbuf)*2.5),15,(100-tbuf)*2.5);
			}
			XFlush(dpy);
		}
		break;
	}
}


/*************************************************************************************
** Function:    ClearFields
** Description: Clears all the fields in the Environment Mode GUI
** Argument:    None
** Return:      None
*************************************************************************************/
void ClearFields()
{
	XSetForeground(dpy, gc, WhitePixel(dpy,screen));

	XFillRectangle(dpy, w, gc, 141, 56, 249,24);  //Target AP
	XFillRectangle(dpy, w, gc, 501, 56, 209,24);  //WEP

	XFillRectangle(dpy, w, gc, 141, 86, 249,24);  //AP MAC
	XFillRectangle(dpy, w, gc, 501, 86, 209,24);  //Association

	XFillRectangle(dpy, w, gc, 141, 116, 249,24);  //WLAN MAC
	XFillRectangle(dpy, w, gc, 501, 116, 209,24);  //Channel

	XFillRectangle(dpy, w, gc, 181, 166, 209,24);
	XFillRectangle(dpy, w, gc, 411, 166, 303,23);
	XFillRectangle(dpy, w, gc, 181, 196, 209,24);
	XFillRectangle(dpy, w, gc, 411, 196, 303,23);
	XFillRectangle(dpy, w, gc, 181, 226, 209,24);
	XFillRectangle(dpy, w, gc, 411, 226, 303,23);

	XFillRectangle(dpy, w, gc, 65, 316, 549,199);

	XSetForeground(dpy, gc, BlackPixel(dpy,screen));
}


/*************************************************************************************
** Function:     ClearWindow()
** Description:  Fills the Window with White
** Argument:     None
** Return:       None
*************************************************************************************/
void ClearWindow()
{
	XSetForeground(dpy, gc, WhitePixel(dpy,screen));
	XFillRectangle(dpy, w, gc, 6, 6, 1011, 589);
	XFlush(dpy);
	XSetForeground(dpy, gc, BlackPixel(dpy,screen));
}


/*************************************************************************************
** Function:    ParseFile
** Description: Reads and Parses the  read data from the Specific test file.
**              Saves the strings in a character array.
** Argument:    None
** Return:      0  OK
**              -1 NG
*************************************************************************************/
int ParseFile()
{
#if 1
	int  fd, bytesRead;
	int  b=0, i=0, c_flg=0;
	char buf[150];
	char filename[256];
	char temp[150];
	
	memset(buf,      0, sizeof(buf));
	memset(filename, 0, sizeof(filename));
	memset(temp,     0, sizeof(temp));
#else
	int  fd, bytesRead;
	char buf[150]="";
	char filename[14]="";
	
	int b=0, i=0, c_flg=0;
	char temp[150]="";
#endif
	
	if (dwnstream_test==1 && ProcessFlg==1) {
		// ダウンストリームテストが開始されていたならば
		strcpy(filename, param[3]);  // param[3] == TESTFILE2[256]
	}
	if (network_test==1 && ProcessFlg==2) {
		// ネットワークディレイテストが開始されていなたならば
		strcpy(filename, param[4]);  // param[4] == TESTFILE3[256]
	}
	if (network_test!=1 && dwnstream_test!=1) {
		// ダウンストリームテスト、ネットワークディレイテスト以外のときならば
		strcpy(filename, param[2]);  // param[2] == TESTFILE1[256]
	}
	
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		if (strcmp(filename,param[3]) == OK) {
			// ダウンストリームテストが開始されていたならば
			test_error = DOWN_ERROR;
		}
		if (strcmp(filename,param[4]) == OK) {
			// ネットワークディレイテストが開始されていなたならば
			printf("\nCannot open file handle\n");
			test_error = NET_ERROR;
		}
		return NG;
	}
	
	flock(fd, LOCK_EX);
	bytesRead = read(fd, buf, sizeof(buf));
//	if (strcmp(filename,param[2]) == OK) {
//		bytesRead = bytesRead - 1; 
//	}
	
	if (bytesRead <= 0) {
		if (strcmp(filename, param[3]) == OK) {
			// ダウンストリームテストが開始されていたならば
			test_error = DOWN_ERROR;
		}
		if (strcmp(filename, param[4]) == OK) {
			// ネットワークディレイテストが開始されていなたならば
			printf("\nfilename=%s,param[3]=%s,param=[4]=%s\n", filename, param[3], param[4]);
			printf("\nCannot read anything\n");
			test_error = NET_ERROR;
		}
		flock(fd, LOCK_UN);
		return NG;
	}
	
	flock(fd, LOCK_UN);
	close(fd);
	id = 0;
	
// Parser
printf("ParseFile:");
	while(1) {
		// 44=0x2c=',', 33=0x21='!', 126=0x7e='~'
		// ','以外の文字？
		if ((buf[i]!=44) && (buf[i]>=33 && buf[i]<=126)) {
			temp[b] = buf[i];
			c_flg = 0;
		}
		
		if(buf[i] == 44) {
			temp[b] = '\0';
			info[id] = (char *)malloc(strlen(temp)+1);
			strcpy(info[id], temp);
printf("(%d)%s, ", id, info[id]);
			memset(temp, 0 , sizeof(temp));
			id++;
			b=0;
			c_flg=1;
		}
		
		if (i == bytesRead) {
			temp[b]= '\0';
			info[id] = (char *)malloc(strlen(temp)+1);
			strcpy(info[id], temp);
printf("(%d)%s\n", id, info[id]);
			break;
		}
		
		if (c_flg != 1) {
			b++;
		}
		i++;
	}  // endwhile
	
	return OK;
}


/*************************************************************************************
** Function:    ValidateData(int idNo)
** Description:  Checks if data is valid( within range), if not change specific data 
** Argument:     int idNo - the specific field to be checked
** Return:       0  OK
**               -1 NG
*************************************************************************************/
int ValidateData(int idNo)
{
   if(idNo == 0){
      if ((strlen(info[idNo]) !=0 ) && (strcmp(info[idNo],"@NoAP@")!=0) &&
          (strlen(info[idNo]) <= 17 )) return OK;
      if  (strlen(info[idNo]) >=33){
         *(info[idNo] + 33) = '\0';
      }
      isDataOK = NG;
      return NG;
   }
   
   if(idNo == 1 || idNo == 2){
      if (atoi(info[idNo]) == 1 || atoi(info[idNo]) == 0 ) return OK;
      isDataOK = NG;
      return NG;
   }
   
   if(idNo == 3){   //Channel
      if ( atoi(info[idNo]) >0 && atoi(info[idNo]) < 14) return OK;
      return NG;
   }         
   
//   if(idNo == 4){   //MAC Address
   if(idNo == 4 || idNo == 5){   //MAC Address
      if ((strlen(info[idNo]) == 17) && 
          (strcmp(info[idNo],"XX:XX:XX:XX:XX:XX")!=0)) return OK;
      return NG;
   }
   
   if(idNo == 6 || (idNo >= 9 && idNo <=21)){
      if ( abs(atoi(info[idNo])) >= 20 && abs(atoi(info[idNo])) <= 100 ) return OK;
      if (idNo == 5) isDataOK = NG;
      return NG;     
   }
   
   if (idNo == 7){
//      if (atoi(info[idNo]) >= 0 && atoi(info[idNo]) <= 10000 ) return OK;
      if (atoi(info[idNo]) >= 0) return OK;
      isDataOK = NG;
      return NG;
   }
   
   if (idNo == 8){
//      if (atoi(info[idNo]) >= 0 && atoi(info[idNo]) <= 10 ) return OK;
      if (atoi(info[idNo]) >= 0 && atoi(info[idNo]) <= 50 ) return OK;
      isDataOK = NG;
      return NG;
   }

   return OK;
}


/*************************************************************************************
** Function:    ResetFlags()
** Description:  Resets all flags used in tests
** Argument:     None
** Return:      None
*************************************************************************************/
void ResetFlags()
{
   dwnstream_test=0;
   network_test=0;
   net_interval=0;
   test_error=0;
   dtest_time =0;
   ntest_time =0;
}


/*************************************************************************************
** Function:    CheckButtons
** Description: Checks and returns the button pressed.
** Argument:     None
** Return:       0  OK
**               -1 NG
*************************************************************************************/
int CheckButtons()
{
   int btn=0;
   XEvent event4;
   
   for(btn=1;btn<11;){
      XCheckWindowEvent(dpy,win[btn], EVT_MSK, &event4 );
      if(event4.type == ButtonRelease){
         event4.type =0;
         XSync(dpy,True);
         return btn;
      }
      btn++;
   }
   return 0;
}


/*************************************************************************************
** Function:    PickColor
** Description: Converts the color (in string) to its pixelmap equivalent
** Argument:    char *color - color to be converted
** Return:      c1.pixel - the unsigned long equivalent of the color.
*************************************************************************************/
unsigned long PickColor(char *color)
{
	Colormap cmap;
	XColor c0, c1;

	cmap = DefaultColormap(dpy, 0);
	XAllocNamedColor(dpy, cmap, color, &c1, &c0);
	return (c1.pixel);
}


/*************************************************************************************
** Function:      MakeButton
** Description:   Makes Buttons
** Argument:      int win_num - counter for the buttons to be made
**                dimension[0] - x coordinate of the button
**                dimension[1] - y coordinate of the button
**                dimension[2] - length of the button
**                dimension[3] - width of the button
**                dimension[4] - x coordinate of the label in the button
**                dimension[5] - y coordinate of the label in the button
** Return:        None
************************************************************************************/
void MakeButton(int win_num, int dimension[6], char *data)
{
	win[win_num] = XCreateSimpleWindow(dpy, w, dimension[0], dimension[1], dimension[2],
						dimension[3], 1,
						BlackPixel(dpy,screen),
						WhitePixel(dpy,screen) );
	XSelectInput(dpy, win[win_num], ButtonReleaseMask|ButtonPressMask);
	tempcursor = XCreateFontCursor(dpy, SB_CURSOR);
	XDefineCursor(dpy, win[win_num], tempcursor);
	w_attr.backing_store = Always;
	XChangeWindowAttributes(dpy, win[win_num],CWBackingStore, &w_attr);
	XMapWindow(dpy, win[win_num]);
	XSelectInput(dpy, w, ExposureMask);
	XMapWindow(dpy, w);
	
	win_draw[win_num] = XftDrawCreate(dpy, win[win_num], visual, colmap);
	DrawPangoXftRenderLayout(win_draw[win_num], &color_black, layout, dimension[4], dimension[5], data);
	XFlush(dpy);
}


/*************************************************************************************
** Function:    MakePopUpMsg
** Description: Makes a Pop Up window with specific message ( status,errors...)
** Argument:    char *Title - title of the pop up window
**              char *Message - message to be displayed in pop-up window
** Return:      None
***********************************************************************************/
void MakePopUpMsg(char *Title, char *Message)
{
	int wxy[6]={0,0,0,0,100,70};
	
	popUp = XCreateSimpleWindow(dpy, w, 280, 190, 350, 150, 3,
		BlackPixel(dpy,screen), WhitePixel(dpy,screen));
	popUp_draw = XftDrawCreate(dpy, popUp, visual, colmap);
	XMapWindow(dpy, popUp);
	
	if (ConfirmButton != ON) {
		DrawPangoXftRenderLayout(popUp_draw, &color_black, layout, 145, 70, Message);
	} else {
		XDrawLine(dpy,popUp,gc,0,30,350,30);
		if (ErrorButton == 1) {
			wxy[4] = 20; wxy[5] = 5;
			ShadeForm(popUp, popUp_draw, wxy, Title, &color_red, C_WHITE);
			wxy[4] = 15; wxy[5] = 50;
			ShadeForm(popUp, popUp_draw, wxy, Message, &color_red, C_WHITE);
		} else {
			wxy[4] = 140; wxy[5] = 5;
			ShadeForm(popUp, popUp_draw, wxy, Title, &color_black, C_WHITE);
			if (strcmp(Message,MSG_FAILING) == 0) {
				printf("\nNOT O K\n");
				wxy[4] = 145; wxy[5] = 50;
				ShadeForm(popUp, popUp_draw, wxy, Message, &color_red, C_WHITE);
			} else {
				printf("\nO K\n");
				wxy[4] = 145; wxy[5] = 50;
				ShadeForm(popUp, popUp_draw, wxy, Message, &color_green, C_WHITE);
			}
		}
	}
	
	// Make Confirm Button
	if( ConfirmButton == ON ){
		win[12] = XCreateSimpleWindow(dpy,popUp,130,100,80,30,1,
			BlackPixel(dpy,screen),WhitePixel(dpy,screen) );
		win_draw[12] = XftDrawCreate(dpy, win[12], visual, colmap);
		XSelectInput(dpy, win[12], ButtonReleaseMask|ButtonPressMask);
		tempcursor = XCreateFontCursor(dpy, SB_CURSOR);
		XDefineCursor( dpy, win[12], tempcursor );
		 w_attr.backing_store = Always;  
		XChangeWindowAttributes(dpy, win[12], CWBackingStore, &w_attr);
		XMapWindow(dpy, win[12]);
		XMapWindow(dpy, popUp);
		DrawPangoXftRenderLayout(win_draw[12], &color_black, layout, 25, 5, judgement[2]);
		XFlush(dpy);
		ErrorButton=0;
		
		for (;;) {
			XCheckWindowEvent(dpy,win[12], EVT_MSK, &event2 );
			if(event2.type == ButtonRelease){
				printf("\nConfirm Button\n");
				event2.type =0;
				break;
			}
		}
		XSync(dpy,True);  //Discard events in queue
		XftDrawDestroy(win_draw[12]);
		XftDrawDestroy(popUp_draw);
		XDestroyWindow(dpy, win[12]);
		XDestroyWindow(dpy, popUp);
	}
	XFlush(dpy);
}


void MakePopUpMsg2(char *Title, char *Message)
{
	popUp = XCreateSimpleWindow(dpy, w, 280, 190, 350, 150, 3,
		BlackPixel(dpy,screen), WhitePixel(dpy,screen));
	popUp_draw = XftDrawCreate(dpy, popUp, visual, colmap);
	XMapWindow(dpy, popUp);
	
	DrawPangoXftRenderLayout(popUp_draw, &color_black, layout, 50, 70, Message);
	XFlush(dpy);
}


void MakePopUpMsgWithConfirmButton(char *Title, int posTitle, char *Message, int posMessage)
{
	int wxy[6]={0,0,0,0,100,70};

	popUp = XCreateSimpleWindow(dpy, w, 280, 190, 350, 150, 3,
		BlackPixel(dpy,screen), WhitePixel(dpy,screen));
	popUp_draw = XftDrawCreate(dpy, popUp, visual, colmap);
	XMapWindow(dpy, popUp);
	
	XDrawLine(dpy,popUp,gc,0,30,350,30);
	wxy[4] = posTitle; wxy[5] = 5;
	ShadeForm(popUp, popUp_draw, wxy, Title, &color_black, C_WHITE);
	wxy[4] = posMessage; wxy[5] = 50;
	ShadeForm(popUp, popUp_draw, wxy, Message, &color_green, C_WHITE);
	
	// Make Confirm Button
		win[12] = XCreateSimpleWindow(dpy,popUp,130,100,80,30,1,
			BlackPixel(dpy,screen),WhitePixel(dpy,screen) );
		win_draw[12] = XftDrawCreate(dpy, win[12], visual, colmap);
		XSelectInput(dpy, win[12], ButtonReleaseMask|ButtonPressMask);
		tempcursor = XCreateFontCursor(dpy, SB_CURSOR);
		XDefineCursor( dpy, win[12], tempcursor );
		 w_attr.backing_store = Always;  
		XChangeWindowAttributes(dpy, win[12], CWBackingStore, &w_attr);
		XMapWindow(dpy, win[12]);
		XMapWindow(dpy, popUp);
		DrawPangoXftRenderLayout(win_draw[12], &color_black, layout, 25, 5, judgement[2]);
		XFlush(dpy);
		ErrorButton=0;
		
		for (;;) {
			XCheckWindowEvent(dpy,win[12], EVT_MSK, &event2 );
			if(event2.type == ButtonRelease){
				printf("\nConfirm Button\n");
				event2.type =0;
				break;
			}
		}
		XSync(dpy,True);  //Discard events in queue
		XftDrawDestroy(win_draw[12]);
		XftDrawDestroy(popUp_draw);
		XDestroyWindow(dpy, win[12]);
		XDestroyWindow(dpy, popUp);
	
	XFlush(dpy);
}


/*************************************************************************************
** Function:     ShadeForm
** Description:  Fills a specific area with a background and writes a colored text
** Argument:     Window x - window to be modified.
**               XftDraw *draw - 
**               dimension[0] - x coordinate of the area to be filled with bg color.
**               dimension[1] - y coordinate of the area to be filled with bg color.
**               dimension[2] - length of the area to be filled with bg color.
**               dimension[3] - width of the area to be filled with bg color.
**               dimension[4] - x coordinate of the text to be draw in reference to 
**                              window x
**               dimension[5] - y coordinate of the text to be draw in reference to 
**                              window y
**               char *data - text to be written
**               XftColor *color - color of the text
**               char *bgColor  - fill color
** Return:       None
************************************************************************************/
void ShadeForm(Window x, XftDraw *draw, int dimension[6], char *data, XftColor *color, char *bgColor)
{
	XSetForeground(dpy, gc, PickColor(bgColor));
	XFillRectangle(dpy, x, gc, dimension[0], dimension[1], dimension[2], dimension[3]);
	DrawPangoXftRenderLayout(draw, color, layout, dimension[4], dimension[5], data);
	XFlush(dpy); 
	XSetForeground(dpy, gc, BlackPixel(dpy,screen));
}


/*************************************************************************************
** Function:     TimeButton
** Description:  Grays out the specific time interval button pressed.
** Argument:     int btn_num - button number
** Return:       none
*************************************************************************************/
void TimeButton(int btn_num)
{
	int first = 0, second = 0;
	int xy[6] = {0, 0, 40, 30, 7, 5};
	char buf[16], buf1[16], btn[16];
	
	if (btn_num == 1) { first = 2; second =3; }
	if (btn_num == 2) { first = 1; second =3; }
	if (btn_num == 3) { first = 1; second =2; }
	
	sprintf(buf,  "%d" MSG_SECOND, first);
	sprintf(buf1, "%d" MSG_SECOND, second);
	sprintf(btn,  "%d" MSG_SECOND, btn_num);
	
	ShadeForm(win[4+btn_num], win_draw[4+btn_num], xy, btn,  &color_black, C_WHITE);
	ShadeForm(win[4+first],   win_draw[4+first],   xy, buf,  &color_black, C_GRAY);
	ShadeForm(win[4+second],  win_draw[4+second],  xy, buf1, &color_black, C_GRAY);
}


/*************************************************************************************
** Function:     ResetTests(int tstNum)
** Description:  Resets the specific test reset to 0
** Argument:     int tstNum - 1 for Downstream Test
**                          others for Network Test
** Return:       None
*************************************************************************************/
void ResetTests(int tstNum)
{
	int reset_btn[6] = {401, 81, 209, 39, 490, 90};
	
	if (tstNum == 1) {  // If Downstream Test
		ShadeForm(w, draw, reset_btn, "0 Kbps", &color_black, C_WHITE);
		DrawStatusIndicator(dpy, w, gc, &ds_status_coor, C_WHITE);
	} else {  // If Network Test
		reset_btn[1] = 151; reset_btn[5] = 160;
		ShadeForm(w, draw, reset_btn,"0 msec", &color_black, C_WHITE);
		reset_btn[1] = 201; reset_btn[5] = 210;
		ShadeForm(w, draw, reset_btn,"0 msec", &color_black, C_WHITE);
		reset_btn[1] = 251; reset_btn[5] = 260;
		ShadeForm(w, draw, reset_btn,"0 msec", &color_black, C_WHITE);
		DrawStatusIndicator(dpy, w, gc, &nw_status_coor, C_WHITE);
	}
}


/*************************************************************************************
** Function:     StopScanButton
** Description:  Fills Start Button with White and Stop Scan with Gray
** Argument:     none
** Return:       none
*************************************************************************************/
void StopScanButton(char *c_c1, char *c_c2, char *c_c3)
{
	int scan_btn[6];
	
	// 「スタート」ボタン
	scan_btn[0] =  0; scan_btn[1] = 0; scan_btn[2] = 90; scan_btn[3] = 30;
	scan_btn[4] = 15; scan_btn[5] = 5;
	ShadeForm(win[ENVT_START], win_draw[ENVT_START], scan_btn,page1[9], &color_black, c_c1);
	
	// 「スキャン開始」ボタン
	scan_btn[0] =  0; scan_btn[1] = 0; scan_btn[2] = 120; scan_btn[3] = 30;
	scan_btn[4] = 11; scan_btn[5] = 5;
	ShadeForm(win[START_SCAN], win_draw[START_SCAN], scan_btn,page1[29], &color_black, c_c2);
	
	// 「スキャン停止」ボタン
	scan_btn[0] =  0; scan_btn[1] = 0; scan_btn[2] = 120; scan_btn[3] = 30;
	scan_btn[4] = 11; scan_btn[5] = 5;
	ShadeForm(win[STOP_SCAN], win_draw[STOP_SCAN], scan_btn,page1[30], &color_black, c_c3);
	
	// 「->」ボタン
	scan_btn[0] =  0; scan_btn[1] = 0; scan_btn[2] = 50; scan_btn[3] = 30;
	scan_btn[4] = 20; scan_btn[5] = 5;
	ShadeForm(win[NEXT], win_draw[NEXT], scan_btn, page1[26], &color_black, c_c1);
	
	// 「終了」ボタン
	if (win[QUIT] != 0) {
		scan_btn[0] =  0; scan_btn[1] = 0; scan_btn[2] = 90; scan_btn[3] = 30;
		scan_btn[4] = 30; scan_btn[5] = 5;
		ShadeForm(win[QUIT], win_draw[QUIT], scan_btn, page1[31], &color_black, c_c1);
	}
}


/*************************************************************************************
** Function:     ResetTests(int tstNum)
** Description:  Resets the specific test reset to 0
** Argument:     int tstNum - 1 for Downstream Test
**                          others for Network Test
** Return:       None
*************************************************************************************/


int	reset_testfile2()
{
	FILE	*fp;

	printf( "[reset_testfile2] start\n" );

	fp = fopen( param[3], "wr" );
	if ( fp == 0 ) {
		printf( "[reset_testfile2] %s cannot open1\n", param[3] );
		return -1;
	}

	fprintf( fp, "0" );
	fclose( fp );
	return 0;
}

/*************************************************************************************
** Function:     ResetTests(int tstNum)
** Description:  Resets the specific test reset to 0
** Argument:     int tstNum - 1 for Downstream Test
**                          others for Network Test
** Return:       None
*************************************************************************************/


int	reset_testfile3()
{
	FILE	*fp;

	printf( "[reset_testfile3] start\n" );

	fp = fopen( param[4], "wr" );
	if ( fp == 0 ) {
		printf( "[reset_testfile3] %s cannot open1\n", param[4]);
		return -1;
	}

	fprintf( fp, "0,0,0" );
	fclose( fp );
	return 0;
}

/*************************************************************************************
** Function:     ReadParamFile
** Description:  Reads parameter file and set specified strings
** Argument:     None
** Return:       None
*************************************************************************************/
int ReadParamFile()
{
	int  x;
	FILE *fp;
	
	fp = fopen(CONFIG_FILE, "r");
	if (fp == 0) {
		printf("configration file cannot open\n");
		return NG;
	}
	
	for (x=0; x<5; x++) {
		fgets(param[x], 256, fp);
		removeLF(param[x]);
	}
	
	fclose(fp);
	
	return OK;
}


void DrawPangoXftRenderLayout(XftDraw *draw, XftColor *color, PangoLayout *layout, int x, int y, char *str)
{
	pango_layout_set_text(layout, str, strlen(str));  // 表示文字列の指定
	pango_xft_render_layout(draw, color, layout, x*1024, y*1024);
}


void ExecIfipPage()
{
	DrawPangoXftRenderLayout(draw, &color_black, layout, 450, 280, MSG_IFUP);
}


void ExecIfip()
{
	char command[256];
	int ret;
	XEvent event;
	
	XCheckWindowEvent(dpy, w, EVT_MSK, &event);
	ret = ReadOneConfig(20, command);
	if (ret != -1) {
		printf("exec:%s\n", command);
		system(command);
	}
}


void DrawStatusIndicator(Display *d, Window w, GC gc, Rect *rect, char *bgColor)
{
#if 0
	XSetForeground(d, gc, PickColor(bgColor));
	XFillArc(d, w, gc, rect->x, rect->y, rect->w, rect->h, 0, 360*64);
	XSetForeground(d, gc, BlackPixel(dpy,screen));
	XDrawArc(d, w, gc, rect->x, rect->y, rect->w, rect->h, 0, 360*64);
#endif
}


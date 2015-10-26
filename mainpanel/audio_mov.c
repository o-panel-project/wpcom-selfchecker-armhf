/*
 *		Audio & Movie Play
 *
 *					2011.6	written by Omoikane Inc.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <alsa/asoundlib.h>
#include "common.h"


static int button_no=0;
static int vol, vol1, vol2, play_mode=0;
static GtkWidget *cb, *b_stop, *b_quit, *hs;
static snd_mixer_elem_t *elem;
static snd_mixer_t *handle;
int audio_pid=0, audio_is_loop=0, movie_is_loop=0;


//
//	Search volume element
//
snd_mixer_elem_t* get_elem(snd_mixer_t* h, char const* name)
{
	snd_mixer_elem_t* se = NULL;
	snd_mixer_elem_t* var = snd_mixer_first_elem(h);
	while (var != NULL) {
		if (strcasecmp(name, snd_mixer_selem_get_name(var)) == 0) {
			se = var;
			break;
		}
		var = snd_mixer_elem_next(var);
	}
	if (se == NULL)
		printf("Could not find mixer name: %s", name);
	return se;
}

static int alsa_setup_j3(int start_up)
{
	if ((elem = get_elem(handle, "DAC1 Digital Fine")) == NULL) {
	   return 8;
	}
	return 0;
}

static int alsa_setup_j4(int start_up)
{
	snd_mixer_elem_t* se = NULL;
	if ((se = get_elem(handle, "PCM")) == NULL) {
	   return 6;
	}
	snd_mixer_selem_set_playback_dB_all(se, 0, 0);	/* 0dB */

	if ((elem = get_elem(handle, "Line DAC")) == NULL) {
	   return 7;
	}
	return 0;
}

static int alsa_setup(int start_up)
{
	int r;
	int l_v, r_v;
	
	if((r=snd_mixer_open(&handle, 0)) < 0) {
		printf("Mixer open error: %s", snd_strerror(r));
		return 1;
	}
	if((r=snd_mixer_attach(handle, "default"))< 0){
		printf("Mixer attach error: %s, %s", "default", snd_strerror(r));
		snd_mixer_close(handle);
		return 2;
	}
	if((r=snd_mixer_selem_register(handle, NULL, NULL)) < 0){
		printf("Mixer register error: %s", snd_strerror(r));
		snd_mixer_close(handle);
		return 3;
	}
	if((r=snd_mixer_load(handle))<0){
		printf("Mixer load error: %s", snd_strerror(r));
		snd_mixer_close(handle);
		return 4;
	}

	if (sc_IsJ4()) {
		r = alsa_setup_j4(start_up);
	} else {
		r = alsa_setup_j3(start_up);
	}
	if (r > 0) {
		g_print("%s():Playback volume not found\n", __func__);
	   return r;
	}
	if(start_up) {
		snd_mixer_selem_get_playback_volume(
				elem, SND_MIXER_SCHN_FRONT_LEFT, &l_v);
		snd_mixer_selem_get_playback_volume(
				elem, SND_MIXER_SCHN_FRONT_RIGHT, &r_v);
		vol = l_v > r_v ? l_v : r_v;
	}

	return 0;
}

#if	1	/*	20110917VACS	*/
//
//	set ALSA volume
//
#if 0
void sc_alsa_set_volume_j4(int v0, int v1)
{
	int		r;
	int		r0, r1;
	int		r2, r3;
	long	pvol0, pvol1;
	long	pvol2, pvol3;

	int x, y;

	/* PCM */
	if ((r = snd_mixer_selem_get_playback_volume(
					elem1, SND_MIXER_SCHN_FRONT_LEFT, &pvol0)) < 0) {
		g_print("PCM Mixer get0 error: %s", snd_strerror(r));
	}
	if ((r = snd_mixer_selem_get_playback_volume(
					elem1, SND_MIXER_SCHN_FRONT_RIGHT, &pvol1)) <0) {
		g_print("PCM Mixer get1 error: %s", snd_strerror(r));
	}
	if ((r = snd_mixer_selem_get_playback_volume(
					elem2, SND_MIXER_SCHN_FRONT_LEFT, &pvol2)) < 0) {
		g_print("Line DAC Mixer get0 error: %s", snd_strerror(r));
	}
	if ((r = snd_mixer_selem_get_playback_volume(
					elem2, SND_MIXER_SCHN_FRONT_RIGHT, &pvol3)) <0) {
		g_print("Line DAC Mixer get1 error: %s", snd_strerror(r));
	}

	if ((!pvol0 && !pvol1) && (!pvol2 && !pvol3) && vol) {
		snd_mixer_close(handle);
//		sync();
		debug_printf(3, "Reopening alsa\n");
		alsa_setup(0);
		gtk_range_set_value(GTK_RANGE(hs), vol);
	}

	debug_printf(3, "set volume (elem:0x%x) (%d, %d) -> (%d, %d)",
			elem1, pvol0, pvol1, v0, v1);
	fflush(0);
	if ((r0 = snd_mixer_selem_set_playback_volume(
					elem1, SND_MIXER_SCHN_FRONT_LEFT, v0)) < 0) {
		g_print("PCM Mixer set0 error: %s", snd_strerror(r0));
	}
	if ((r1 = snd_mixer_selem_set_playback_volume(
					elem1, SND_MIXER_SCHN_FRONT_RIGHT, v1)) < 0) {
		g_print("PCM Mixer set1 error: %s", snd_strerror(r1));
	}
	debug_printf(3, " : (%d, %d)\n", r0, r1);

	debug_printf(3, "set volume (elem:0x%x) (%d, %d) -> (%d, %d)",
			elem2, pvol2, pvol3, v0, v1);
	fflush(0);
	if ((r2 = snd_mixer_selem_set_playback_volume(
					elem2, SND_MIXER_SCHN_FRONT_LEFT, v0)) < 0) {
		g_print("Line DAC Mixer set0 error: %s", snd_strerror(r0));
	}
	if ((r3 = snd_mixer_selem_set_playback_volume(
					elem2, SND_MIXER_SCHN_FRONT_RIGHT, v1)) < 0) {
		printf("Line DAC Mixer set1 error: %s", snd_strerror(r1));
	}
	debug_printf(3, " : (%d, %d)\n", r2, r3);
}
#endif

void
sc_alsa_set_volume(int v0, int v1)
{
int		r, r0, r1;
long	pvol0, pvol1;

	int x, y;

	if((r=snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &pvol0))<0){
		printf("Mixer get0 error: %s", snd_strerror(r));
	}
	if((r=snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, &pvol1))<0){
		printf("Mixer get1 error: %s", snd_strerror(r));
	}
	debug_printf(3, "from (%d, %d) -> ", pvol0, pvol1);
	fflush(0);

	if((!pvol0&&!pvol1)&&vol){
		snd_mixer_close(handle);
//		sync();
		debug_printf(3, "Reopening alsa\n");
		alsa_setup(0);
		gtk_range_set_value(GTK_RANGE(hs), vol);
	}

	debug_printf(3, "set to %d, %d (elem:0x%x) -> ", v0, v1, elem);
	fflush(0);
	if((r0=snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, v0))<0){
		printf("Mixer set0 error: %s", snd_strerror(r0));
	}
	if((r1=snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, v1))<0){
		printf("Mixer set1 error: %s", snd_strerror(r1));
	}
	debug_printf(3, "(%d, %d)\n", r0, r1);
}

void
audio_set_volume(void)
{
	debug_printf(3, "%s():button_no=%d, play_mode=%d\n",
			__func__, button_no, play_mode);

	switch(play_mode%3){
	case 0:
		sc_alsa_set_volume(vol, vol);
		break;
	case 1:	/* Left */
		debug_printf(3, "%s():Left on, Right off\n", __func__);
		sc_alsa_set_volume(vol, 0);
		break;
	case 2:	/* Right */
		debug_printf(3, "%s():Left off, Right on\n", __func__);
		sc_alsa_set_volume(0, vol);
		break;
	}
}
#endif

#if	0	/*	20110917VACS	*/
//
//	set ALSA volume
//
void sc_alsa_set_volume(int v0, int v1)
{
	int r, r0, r1;
	long pvol0, pvol1;
	
	if((r=snd_mixer_selem_get_playback_volume(elem, 0, &pvol0))<0){
		printf("Mixer get error: %s", snd_strerror(4));
	}
	if((r=snd_mixer_selem_get_playback_volume(elem, 0, &pvol1))<0){
		printf("Mixer get error: %s", snd_strerror(4));
	}
//	debug_printf(3, "from (%d, %d) -> ", pvol0, pvol1);
	fflush(0);
	if((!pvol0&&!pvol1)&&vol){
		snd_mixer_close(handle);
		sync();
		debug_printf(3, "Reopening alsa\n");
		alsa_setup(0);
		gtk_range_set_value(GTK_RANGE(hs), vol);
	}
//	debug_printf(3, "set to %d, %d (elem:0x%x) -> ", vol, vol, elem);
	fflush(0);
	if((r0=snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, v0))<0){
		printf("Mixer set error: %s", snd_strerror(r0));
	}
	if((r1=snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, v1))<0){
		printf("Mixer set error: %s", snd_strerror(r1));
	}
	debug_printf(3, "(%d, %d)\n", r0, r1);
}

void audio_set_volume()
{
	int vol_single;
	
	vol_single=min((vol*120)/100, 100);
	switch(play_mode%3){
	case 0:
		sc_alsa_set_volume(vol, vol);
		break;
	case 1:
		sc_alsa_set_volume(0, vol_single);
		break;
	case 2:
		sc_alsa_set_volume(vol_single, 0);
		break;
	}
}
#endif

static void vol_got(GtkWidget *widget, gpointer data)
{
	vol=gtk_range_get_value(GTK_RANGE(widget));
	audio_set_volume();
}

static void loop_got(GtkWidget *widget, gpointer is_loop)
{
	*(int *)is_loop=(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))==TRUE);
}

static void button_press(GtkWidget *widget, gpointer data)
{
	button_no=(int)data;
}

GtkWidget *put_vol(GtkWidget *c, int *check, int *is_loop)
{
	GtkWidget *a, *h0;
	
	a=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);

	/*	20110917VACS	*/
	int volume_max = AUDIO_VOLUME_MAX;
	if (sc_IsJ4())
		volume_max = AUDIO_VOLUME2_MAX;
	hs=gtk_hscale_new_with_range(0, volume_max, 1);

	gtk_range_set_value(GTK_RANGE(hs), vol);
	g_signal_connect(hs, "value-changed", G_CALLBACK(vol_got), (gpointer)0);
	
	cb=gtk_check_button_new_with_label("Loop Play");
	g_signal_connect(cb, "toggled", G_CALLBACK(loop_got), (gpointer)is_loop);
	
	h0=gtk_hbox_new(FALSE, 10);
	
	gtk_container_add(GTK_CONTAINER(h0), cb);
	gtk_container_add(GTK_CONTAINER(h0), hs);
	gtk_container_add(GTK_CONTAINER(a), h0);
	gtk_container_add(GTK_CONTAINER(c), a);
	
	return hs;
}

void audio_fin(int check, int pid)
{
	if(!pid) return;
	if(check && gtk_widget_get_sensitive(b_stop)!=TRUE) return;
	do_killpg(pid);
}

int audio_play(int *pid, int file_no)
{
	char f[SMALL_STR];
	int fdnull, mypid;
	char alsaopt[SMALL_STR];
	
	audio_set_volume();
	switch(*pid=fork()){
	case -1:
		return -1;
	
	case 0:
		mypid = getpid();
		setpgid(mypid, mypid);
		fdnull=open("/dev/null", O_RDWR);
		dup2(fdnull,0);
		strcpy(f, base_path);
		strcat(f, "/data/");
		strcat(f, file_no ? "test_b.wav" : "test_a.wav");
#if 0	/* noisy */
		if (sc_IsJ4())
			strcpy(alsaopt, "alsa:device=hw=0.0");
		else
#endif
			strcpy(alsaopt, "alsa");
		if(audio_is_loop){
			execl("/usr/bin/mplayer", "mplayer", "-ao", alsaopt, "-loop", "0", f, NULL);
		}else{
			execl("/usr/bin/mplayer", "mplayer", "-ao", alsaopt, f, NULL);
		}
		_exit(127);
		break;
	}
	return 0;
}

//
//	menu for 'Audio'
//
#if 1  // vacs,2012/2/29
int audio_main(GtkWidget *table, GtkWidget *bsub)
#else
int audio_main(GtkWidget *p, GtkWidget *bsub)
#endif
{
	int s;
	time_t t_fin=0;
	GtkWidget *v0, *v1, *a0, *a1, *bb, *hs;
	GtkWidget *b0, *b1, *b2, *b3, *b4, *b5;
	GtkTreeStore *store;
	
	play_mode=0;
	if(alsa_setup(1)){
		printf("Could not initialize alsa. Aborted Audio Test.\n");
		return 1;
	}
	
	v0=gtk_vbox_new(FALSE, 10);
	
	b0=gtk_button_new_with_label("Play A STEREO");
	g_signal_connect(b0, "clicked", G_CALLBACK(button_press), (gpointer)10);
	b1=gtk_button_new_with_label("Play A - L");
	g_signal_connect(b1, "clicked", G_CALLBACK(button_press), (gpointer)11);
	b2=gtk_button_new_with_label("Play A - R");
	g_signal_connect(b2, "clicked", G_CALLBACK(button_press), (gpointer)12);
	b3=gtk_button_new_with_label("Play B STEREO");
	g_signal_connect(b3, "clicked", G_CALLBACK(button_press), (gpointer)13);
	b4=gtk_button_new_with_label("Play B - L");
	g_signal_connect(b4, "clicked", G_CALLBACK(button_press), (gpointer)14);
	b5=gtk_button_new_with_label("Play B - R");
	g_signal_connect(b5, "clicked", G_CALLBACK(button_press), (gpointer)15);
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.5);
	v1=gtk_vbox_new(FALSE, 10);
	
	gtk_container_add(GTK_CONTAINER(v1), b0);
	gtk_container_add(GTK_CONTAINER(v1), b1);
	gtk_container_add(GTK_CONTAINER(v1), b2);
	gtk_container_add(GTK_CONTAINER(v1), b3);
	gtk_container_add(GTK_CONTAINER(v1), b4);
	gtk_container_add(GTK_CONTAINER(v1), b5);
	gtk_container_add(GTK_CONTAINER(a1), v1);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	hs=put_vol(v0, NULL, &audio_is_loop);
	
	a0=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	b_stop=gtk_button_new_with_label("Stop");
	g_signal_connect(b_stop, "clicked", G_CALLBACK(button_press), (gpointer)2);
	gtk_container_add(GTK_CONTAINER(a0), b_stop);
	gtk_container_add(GTK_CONTAINER(v0), a0);
	
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(&button_no, bsub, b_quit, sc_bbox1_click_noquit);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		audio_is_loop=1;
		button_no=13;
	}else{
		button_no=0;
	}
	
	if(audio_pid)
		gtk_widget_set_sensitive(v1, FALSE);
	
	if(audio_is_loop)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb), TRUE);
	
	while(1){
		if(audio_pid){
			if(waitpid(audio_pid,&s,WNOHANG) || !have_proc_pid(audio_pid)){
				audio_pid=0;
				gtk_widget_set_sensitive(v1, TRUE);
				gtk_widget_set_sensitive(b_quit, TRUE);
			}
		}
		switch(button_no){
		case 0:  // nothing
			break;
		
		case 1:  // quit
			if(!audio_is_loop) audio_fin(0, audio_pid);
			goto fin;
		
		case 2:  // stop
			audio_fin(0, audio_pid);
			store=GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv_sidemenu)));
			gtk_tree_store_set(store, &iter_audio, COLUMN_LABEL, "Audio", COLUMN_NAME_COLOR, NULL, -1);
			break;
		
		case 10: // play A/B, stereo-L-R
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			play_mode=button_no-10;
			sc_gtk_update();
			gtk_widget_set_sensitive(v1, FALSE);
			if(audio_is_loop){
				store=GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tv_sidemenu)));
				gtk_tree_store_set(store, &iter_audio, COLUMN_LABEL, "Audio", COLUMN_NAME_COLOR, "Orange", -1);
			}else{
				gtk_widget_set_sensitive(b_quit, FALSE);
			}
			audio_play(&audio_pid, play_mode/3);
			break;
		}
		button_no=0;
		sc_gtk_update();
		gdk_threads_leave();
		usleep(80000);
		gdk_threads_enter();
		if(t_fin && t_fin<time(NULL)){
			debug_printf(3, "Loop test timed up.\n");
			audio_fin(0, audio_pid);
			if(audio_pid) waitpid(audio_pid, &s, WNOHANG);
			goto fin;
		}
	}

fin:
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	snd_mixer_close(handle);
	return 0;
}

int movie_play(int *pid)
{
	int fdnull, mypid;
	char f[SMALL_STR];
	char alsaopt[SMALL_STR];
	
	audio_set_volume();
	switch(*pid=fork()){
	case -1:
		return -1;
	
	case 0:
		mypid = getpid();
		setpgid(mypid, mypid);
		fdnull=open("/dev/null",O_RDWR);
		dup2(fdnull,0);
		sprintf(f, "%s/data/sample.mp4", base_path);
		if(movie_is_loop){
			execl("/mnt1/bin/newsc_movie.sh", "newsc_movie.sh", f, "-loop", NULL);
		}else{
			execl("/mnt1/bin/newsc_movie.sh", "newsc_movie.sh", f, NULL);
		}
		_exit(127);
		break;
	}
	return 0;
}

//
//	menu for 'Movie Play'
//
int movie_main(GtkWidget *table, GtkWidget *bsub)
{
	int pid=0, s;
	time_t t_fin=0;
	GtkWidget *v0, *a0, *a1, *bb, *hs, *lb;
	GtkWidget *b0;

	play_mode=0;
	if(alsa_setup(1)){
		printf("Could not intiailize alsa. Aborted Audio Test.\n");
		return 1;
	}
	
	v0=gtk_vbox_new(FALSE, 10);
	lb=gtk_label_new("");
	gtk_widget_set_usize(lb, 500, 300);
	gtk_container_add(GTK_CONTAINER(v0), lb);
	
	b0=gtk_button_new_with_label("Play Movie");
	g_signal_connect(b0, "clicked", G_CALLBACK(button_press), (gpointer)10);
	a1=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	gtk_container_add(GTK_CONTAINER(a1), b0);
	gtk_container_add(GTK_CONTAINER(v0), a1);
	
	movie_is_loop=0;
	hs=put_vol(v0, NULL, &movie_is_loop);
	
	a0=gtk_alignment_new(0.5, 0.5, 0.5, 0.2);
	b_stop=gtk_button_new_with_label("Stop");
	g_signal_connect(b_stop, "clicked", G_CALLBACK(button_press), (gpointer)2);
	gtk_container_add(GTK_CONTAINER(a0), b_stop);
	gtk_container_add(GTK_CONTAINER(v0), a0);
	
	b_quit=gtk_button_new_from_stock("gtk-quit");
	bb=sc_bbox2(&button_no, bsub, b_quit, sc_bbox1_click_noquit);
	gtk_box_pack_start(GTK_BOX(v0), bb, FALSE, FALSE, 0);
	
#if 1  // vacs,2012/2/29
	sc_table_attach2(GTK_TABLE(table), v0);
	gtk_widget_show_all(table);
#else
	gtk_paned_pack2(GTK_PANED(p), v0, TRUE, FALSE);
	gtk_widget_show_all(p);
#endif
	
	if(testtime){
		t_fin=time(NULL)+testtime;
		movie_is_loop=1;
		button_no=10;
	}else{
		button_no=0;
	}
	
	while(1){
		if(pid){
			if(waitpid(pid,&s,WNOHANG) || !have_proc_pid(pid)){
				pid=0;
				gtk_widget_set_sensitive(b0, TRUE);
				gtk_widget_set_sensitive(b_quit, TRUE);
			}
		}
		switch(button_no){
		case 0:  // nothing
			break;
		
		case 1:  // quit
			audio_fin(1, pid);
			goto fin;
		
		case 2:  // stop
			audio_fin(0, pid);
			break;
		
		case 10: // play
			gtk_widget_set_sensitive(b0, FALSE);
			gtk_widget_set_sensitive(b_quit, FALSE);
			sc_gtk_update();
			movie_play(&pid);
			break;
		}
		button_no=0;
		sc_gtk_update();
		gdk_threads_leave();
		usleep(80000);
		gdk_threads_enter();
		if(t_fin && t_fin<time(NULL)){
			debug_printf(3, "Loop test timed up.\n");
			audio_fin(0, pid);
			if(pid) waitpid(pid,&s,WNOHANG);
			goto fin;
		}
	}

fin:
	sc_bbox2_remove(bsub);
	gtk_widget_destroy(v0);
	snd_mixer_close(handle);
	return 0;
}

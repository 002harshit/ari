#include "keymap.h"
#include "miniaudio/miniaudio.h"
#include <X11/XKBlib.h>
#include <X11/extensions/record.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

ma_engine engine;
int is_verbose = 1;
#define ari_log(fmtstr, ...)                                                   \
	if (is_verbose == 1) {                                                     \
		printf(fmtstr, __VA_ARGS__);                                           \
	}

void
ari_play(const char *filename)
{
	ma_engine_play_sound(&engine, filename, NULL);
}

void
ari_add_key(int keycode, int is_down, int is_mouse)
{

	if (is_mouse || keycode > 256)
		return;

	const char *wav = wav_filelist[keycode * 2 + is_down];
	ari_log("file: %s", wav);
	ari_play(wav);
}

int last_key = 0;
void
ari_key_pressed_cb(XPointer arg, XRecordInterceptData *d)
{
	if (d->category != XRecordFromServer)
		return;

	int key = ((unsigned char *)d->data)[1];
	int type = ((unsigned char *)d->data)[0] & 0x7F;
	int repeat = d->data[2] & 1;

	int is_down = type == 2;
	int is_mouse = (key - 8) < 0;

	if (!repeat) {
		ari_add_key(key, is_down, is_mouse);
		ari_log("\ncode:  %02x hex: %02x ", key, key);
	}
	XRecordFreeData(d);
}

void
ari_record()
{
	XRecordRange *rr;
	XRecordClientSpec rcs;
	XRecordContext rc;

	Display *dpy = XOpenDisplay(NULL);
	assert(dpy != NULL && "Unable to open display");

	rr = XRecordAllocRange();
	assert(rr != NULL && "XRecordAllocRange error");

	rr->device_events.first = KeyPress;
	rr->device_events.last = ButtonReleaseMask;
	rcs = XRecordAllClients;

	rc = XRecordCreateContext(dpy, 0, &rcs, 1, &rr, 1);
	assert(rc != 0 && "XRecordCreateContext error");
	XFree(rr);

	int status = XRecordEnableContext(dpy, rc, ari_key_pressed_cb, NULL);
	assert(status != 0 && "XRecordEnableContext error");
}

int
main()
{
	ma_result result;
	result = ma_engine_init(NULL, &engine);
	assert(result == MA_SUCCESS);
	ari_record();
	ma_engine_uninit(&engine);
}

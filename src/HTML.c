/*---------------------------------------------------------
|
|   File:   HTML.c
|
|   Contains: Routines for reading charts
|       and webkit interactions.
|
|
|   Written By:     Elias Keshishoglou
|
|   Copyright :     2014 Elias Keshishoglou all rights reserved.
|
|   This program is free software; you can redistribute it and/or
|   modify it under the terms of the GNU General Public License
|   as published by the Free Software Foundation; either version 2
|   of the License, or (at your option) any later version.
|
|   This program is distributed in the hope that it will be useful,
|   but WITHOUT ANY WARRANTY; without even the implied warranty of
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|   GNU General Public License for more details.
|
|   You should have received a copy of the GNU General Public License
|   along with this program; if not, write to the Free Software
|   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
USA.
|
|
|
|-----------------------------------------------*/

#define HTML_c

/*
 * Place Includes Here
 */

#include <gtk/gtk.h>
#define WebKit2 1

#include "LiveMusicApp.h"
#ifdef GTK_4
#include <webkit/webkit.h>
#else
#include <webkit2/webkit2.h>
#endif
#include "MyWidgets.h"
#include "APlayMidi.h"

#define _GNU_SOURCE
#include <string.h>
#include "Player.h"
#include <unistd.h>
#include "SooperOSC.h"
#include <sys/time.h>
#include "HTML.h"
#include "libgen.h"

/*
 * Place defines and Typedefs here
 */

#define ParseValue "Preset"
#define FileMaxLineLength 250

// It looks like this forces presets 1-5
#define PersistentPresets 1

/*
 * Place Local prototypes here
 */

int Search_in_File(const char *fname, WebLoadPresets *thePresets);
int ScalePage(void);
void SetPatchTitles(theImageButtons *MyButton, char *Text, int Value);
gboolean Play_click_handler(GtkWidget *widget, GdkEvent *event,
                            gpointer user_data);
static void scroll_js_finished_cb(GObject *object, GAsyncResult *result,
                                  gpointer user_data);

gboolean on_RightMenu_clicked(WebKitWebView *page, WebKitContextMenu *menu,
                              char *hitTestResult,
                              gpointer contextMenuCallback);

int DecodeURI(char *s, char *dec);
int ishex(int x);

void HoverLink_cb(WebKitWebView *web_view, gchar *title, gchar *uri,
                  gpointer user_data);

static void DrumFav_cb(gpointer *Data);
static void SaveFav_cb(gpointer *Data);
static void EditChart_cb(gpointer *Data);

/*
 * Place Static variables here
 */

static WebKitWebView *web_view;
theImageButtons PresetButtons[MaxPresetButtons];
theImageButtons SaveWebButton;
theImageButtons DrumLoopButton;
theImageButtons ForwardButton;
theImageButtons BackButton;
theImageButtons SetListButton;
theImageButtons PlayPauseButton;
theImageButtons TapTempoButton;
WebKitContextMenu *MyConMenu;

// GtkWidget *scrolled_window;

GtkWidget *ChartGTKView;
char SetListFileName[FileNameMaxLength];
FILE *SetListFile;
char BasePathName[FileNameMaxLength];
char LastDrumName[FileNameMaxLength];
char LastClickURI[FileNameMaxLength];
int ScrollPosition;
int JavaScrollPosition;
int WaitForCallBack;

// ~/.config/mimeapps.list

/*----------------------------------------------
 * Function:        SavePresetChanges
 *
 * Description: Rewrite the setlist song's HTML file in place, updating
 *              its <meta name="PresetN"...> and <meta name="Tempo"...>
 *              tags to match the currently-assigned web presets
 *              (gMyInfo.WebPresets.thePreset[0..5]) and tempo. Works by
 *              streaming the file line-by-line to a ".Test" temp file,
 *              rewriting any line containing "PresetN"/"Tempo" as it
 *              goes, then renaming the temp file over the original once
 *              done. Refuses to touch `FileName` if it's actually a
 *              directory. Returns 0 on success, 1 if either file
 *              couldn't be opened.
 *
 *              ("Cheesy" per the original comment: it does substring
 *              matching on the raw HTML text rather than proper HTML/
 *              XML parsing, so it depends on the file's meta tags
 *              looking exactly like the ones it writes.)
 *-----------------------------------------------*/
char SavePresetChanges(char *FileName) {
    FILE *InFile;
    FILE *OutFile;
    char FileLine[200];
    tPatchIndex thePatch;
    DIR *directory;

    /* Check to see if this is a directory.
    If it is return, don't try and modify it.
    */
    directory = opendir(FileName);

    if (directory != NULL) {
        closedir(directory);
        return 0;
    }

    /* Open the current html file.
     */
    InFile = fopen(FileName, "r");

    /* Open the output file.
     */
    sprintf(FileLine, "%s.Test", FileName);
    OutFile = fopen(FileLine, "w+");

    if (!OutFile) {
        /* BUG: remove() takes a filename (const char *), not a FILE*.
         * `OutFile` is NULL here anyway (that's why we're in this
         * branch), so this call does nothing useful - it's not
         * actually deleting the ".Test" temp file it looks like it's
         * trying to clean up (that would be `remove(FileLine)`).
         * Left as-is; flagging for a follow-up fix rather than
         * changing behavior in a comments-only pass. */
        remove(OutFile);
        printd(LogError, "Html File Error %x %x\n", InFile, OutFile);
        return (1);
    }

    if (!InFile) {
        printd(LogError, "Html File Error %x %x\n", InFile, OutFile);
        return (1);
    }

    /* Read the infile line by line and replace the meta
    Data with the current settings.
    */
    while (!feof(InFile)) {
        FileLine[0] = 0;

        /* Read a line of file.
         */
        fgets(FileLine, 200, InFile);

        if (strstr(FileLine, "Preset1")) {
            thePatch = gMyInfo.WebPresets.thePreset[0];

            if (thePatch >= 0 && thePatch < Max_Patches)
                sprintf(FileLine, "<meta name=\"Preset1\" content=\"%s\" >\n",
                        gMyInfo.MyPatchInfo[thePatch].Name);
        }

        if (strstr(FileLine, "Preset2")) {
            thePatch = gMyInfo.WebPresets.thePreset[1];

            if (thePatch >= 0 && thePatch < Max_Patches)
                sprintf(FileLine, "<meta name=\"Preset2\" content=\"%s\" >\n",
                        gMyInfo.MyPatchInfo[thePatch].Name);
        }

        if (strstr(FileLine, "Preset3")) {
            thePatch = gMyInfo.WebPresets.thePreset[2];

            if (thePatch >= 0 && thePatch < Max_Patches)
                sprintf(FileLine, "<meta name=\"Preset3\" content=\"%s\" >\n",
                        gMyInfo.MyPatchInfo[thePatch].Name);
        }

        if (strstr(FileLine, "Preset4")) {
            thePatch = gMyInfo.WebPresets.thePreset[3];

            if (thePatch >= 0 && thePatch < Max_Patches)
                sprintf(FileLine, "<meta name=\"Preset4\" content=\"%s\" >\n",
                        gMyInfo.MyPatchInfo[thePatch].Name);
        }

        if (strstr(FileLine, "Preset5")) {
            thePatch = gMyInfo.WebPresets.thePreset[4];

            if (thePatch >= 0 && thePatch < Max_Patches)
                sprintf(FileLine, "<meta name=\"Preset5\" content=\"%s\" >\n",
                        gMyInfo.MyPatchInfo[thePatch].Name);
        }

        if (strstr(FileLine, "Preset6")) {
            thePatch = gMyInfo.WebPresets.thePreset[5];

            if (thePatch >= 0 && thePatch < Max_Patches)
                sprintf(FileLine, "<meta name=\"Preset6\" content=\"%s\" >\n",
                        gMyInfo.MyPatchInfo[thePatch].Name);
        }

        if (strstr(FileLine, "Tempo")) {
            sprintf(FileLine, "<meta name=\"Tempo\" content=\"%d\" >\n",
                    gMyInfo.Tempo);

            if (gMyInfo.Tempo < 60) {
                gMyInfo.Tempo = 60;
            }

            if (gMyInfo.Tempo > 160) {
                gMyInfo.Tempo = 160;
            }
        }

        /* Now write the line back to the output file.
         */
        fprintf(OutFile, "%s", FileLine);
    }

    // Close both files
    fclose(InFile);
    fclose(OutFile);

    sprintf(FileLine, "%s.Test", FileName);

    // Rename the temp file back to the main file.
    rename(FileLine, FileName);

    return (0);
}

/*----------------------------------------------
 * Function:        on_Back_clicked
 *
 * Description: "Back" button handler: saves any preset/tempo changes
 *              made to the currently-displayed song page (via
 *              SavePresetChanges()) before navigating the embedded
 *              WebKit browser back to the previous page. Also resets
 *              the 5 preset button labels back to their generic
 *              defaults, unless PersistentPresets is defined (it isn't
 *              anywhere in this tree, so this reset always runs).
 *
 *-----------------------------------------------*/
void on_Back_clicked(GtkButton *button, gpointer user_data) {
    const gchar *CurrentURI;

    ScrollPosition = 0;
    //  webkit_web_view_set_editable( web_view, FALSE);
    gtk_image_set_from_pixbuf(GTK_IMAGE(BackButton.Image),
                              BackButton.ButtonDownImage);
    /* Get the currently loaded page.
     */
    CurrentURI = webkit_web_view_get_uri(web_view);
    /* &CurrentURI[7] skips over the "file://" scheme prefix (7 chars)
     * to turn the URI into a plain filesystem path for
     * SavePresetChanges(). Assumes the URI is always a local file://
     * URL - would misbehave (wrong offset) if a page were ever loaded
     * from http(s):// instead. */
    SavePresetChanges((char *)&CurrentURI[7]);
    webkit_web_view_go_back(web_view);

#ifndef PersistentPresets
    // Reset the patches
    SetPatchTitles(&PresetButtons[0], "Preset 1", 1);
    SetPatchTitles(&PresetButtons[1], "Preset 2", 2);
    SetPatchTitles(&PresetButtons[2], "Preset 3", 3);
    SetPatchTitles(&PresetButtons[3], "Preset 4", 4);
    SetPatchTitles(&PresetButtons[4], "Preset 5", 5);
    //  SetPatchTitles(&PresetButtons[5], "Preset 6", 6);
#endif
}

/*----------------------------------------------
 * Function:        on_Forward_clicked
 *
 * Description: "Forward" button handler: navigates the embedded
 *              WebKit browser forward. Unlike on_Back_clicked(), does
 *              not save preset changes first (there's nothing to save
 *              going forward - the page being left wasn't just edited).
 *
 *-----------------------------------------------*/
void on_Forward_clicked(GtkButton *button, gpointer user_data) {

    gtk_image_set_from_pixbuf(GTK_IMAGE(ForwardButton.Image),
                              ForwardButton.ButtonDownImage);
    webkit_web_view_go_forward(web_view);
}

#ifndef GTK_4
/*----------------------------------------------
 * Function:        web_view_javascript_finished
 *
 * Description: Generic async callback for
 *              webkit_web_view_evaluate_javascript() calls made
 *              elsewhere in this file. Extracts the JS result as a
 *              string (if it is one) and, when `user_data` was passed
 *              as 1 by the caller that kicked off the evaluation,
 *              interprets that string as the page's current scroll
 *              position (JavaScrollPosition) - this is how
 *              ScrollCtrl() below reads back where the WebView is
 *              scrolled to. `user_data` values other than 1 currently
 *              do nothing with the result; the "1 = scroll position"
 *              convention is entirely defined by the caller(s) that
 *              queue the JS eval, not by this function itself.
 *
 *              Also sets WaitForCallBack = -100 as a "callback has
 *              fired" sentinel that callers can poll for.
 *-----------------------------------------------*/
static void web_view_javascript_finished(GObject *object, GAsyncResult *result,
                                         gpointer user_data) {
    WebKitJavascriptResult *js_result;
    JSCValue *value;
    GError *error = NULL;

    WaitForCallBack = -100;

    //  printf("web_view_javascript_finished\n");
    //    js_result =
    //    webkit_web_view_run_javascript_finish(WEBKIT_WEB_VIEW(object), result,
    //    &error);
    js_result = webkit_web_view_evaluate_javascript_finish(
        WEBKIT_WEB_VIEW(object), result, &error);

    if (!js_result) {
        //      printf ("Error running javascript: %s", error->message);
        g_error_free(error);
        return;
    }

    value = webkit_javascript_result_get_js_value(js_result);

    //  printf(" Return0 %x %s\n", value, value);
    if (jsc_value_is_string(value)) {
        JSCException *exception;
        gchar *str_value;

        str_value = jsc_value_to_string(value);
        exception = jsc_context_get_exception(jsc_value_get_context(value));

        if ((intptr_t)user_data == 1) {
            JavaScrollPosition = atoi(str_value);
            //          printf("Java Scroll %s %d\n", str_value,
            //          JavaScrollPosition);
 }
    }

    webkit_javascript_result_unref(js_result);
}

/*----------------------------------------------
 * Function:        scroll_js_finished_cb
 *
 * Description: Fire-and-forget async callback for the JS that actually
 *              performs a scroll (window.scrollTo(...) or similar,
 *              queued by ScrollCtrl() below). Doesn't need the result
 *              value - it's only used to catch and log a JS execution
 *              error, if any.
 *-----------------------------------------------*/
static void scroll_js_finished_cb(GObject *object, GAsyncResult *result,
                                  gpointer user_data) {
    WebKitJavascriptResult *js_result;
    GError *error = NULL;

    //    js_result =
    //    webkit_web_view_run_javascript_finish(WEBKIT_WEB_VIEW(object), result,
    //    &error);
    js_result = webkit_web_view_evaluate_javascript_finish(
        WEBKIT_WEB_VIEW(object), result, &error);

    if (error != NULL) {
        g_print("Error running scroll script: %s", error->message);
        g_error_free(error);
        return;
    }
}
#endif

/*----------------------------------------------
 * Function:        ScrollCtrl
 *
 * Description: Programmatic scrolling of the loaded song page.
 *              `Amount` is overloaded: either one of the ScrollPgDn/
 *              ScrollPgUp/ScrollEnd/ScrollHome/ScrollKeyDn/ScrollKeyUp
 *              sentinel constants (relative scroll by a fixed pixel
 *              amount), or - if > 20 - taken as an absolute target
 *              scroll position directly. Reads the current scroll
 *              position first via ScrollGetPosition(), applies the
 *              requested change, clamps to >= 0, then injects a
 *              `window.scrollTo(0, N)` JS call into the WebView to
 *              actually move it. The JS runs async; scroll_js_finished_cb()
 *              just logs errors, it doesn't confirm the scroll landed.
 *-----------------------------------------------*/
int ScrollCtrl(float Amount) {
    GtkAdjustment *Adjust;
    gdouble UpperV, VIncrement;
    gdouble Value;
    char Script[500];

    ScrollPosition = ScrollGetPosition();

    if (Amount == ScrollPgDn) {
        ScrollPosition += 200;
    }

    if (Amount == ScrollPgUp) {
        ScrollPosition -= 200;
    }

    if (Amount == ScrollEnd) {
        ScrollPosition = 100000;
    }

    if (Amount == ScrollHome) {
        ScrollPosition = 1;
    }

    if (Amount == ScrollKeyDn) {
        ScrollPosition += 25;
    }

    if (Amount == ScrollKeyUp) {
        ScrollPosition -= 25;
    }

    if (Amount > 20) {
        ScrollPosition = Amount;
    }

    if (ScrollPosition < 0) {
        ScrollPosition = 0;
    }

    if (ScrollPosition != 0) {
        //  sprintf(Script,"window.getSelection().getRangeAt(0).toString()");
        printf("Scroll Control %f %d\n", Amount, ScrollPosition);
        sprintf(Script, "window.scrollTo(%d,%d);", 0, ScrollPosition);
        webkit_web_view_run_javascript(web_view, Script, NULL,
                                       scroll_js_finished_cb, NULL);
        //        webkit_web_view_evaluate_javascript_finish(web_view, Script,
        //        NULL, scroll_js_finished_cb, NULL);
    }
    return (0);
}

/*----------------------------------------------
 * Function:        ScrollGetPosition
 *
 * Description: Read back the WebView's current vertical scroll offset
 *              (window.pageYOffset) by running a JS snippet
 *              asynchronously and waiting for web_view_javascript_finished()
 *              to store the result in JavaScrollPosition.
 *
 *              *** POTENTIAL BUG - WORTH VERIFYING ***
 *              The "wait" below is a plain busy-wait spin loop on
 *              WaitForCallBack, with no call to
 *              g_main_context_iteration()/gtk_main_iteration() (there
 *              are none anywhere in this codebase). WebKit's async JS
 *              callback can only fire when GTK's main loop processes
 *              pending events - and if this function runs on the main
 *              thread (which it appears to, since it drives UI/WebKit
 *              calls directly), that main loop is the very thing
 *              blocked spinning here. That would mean
 *              web_view_javascript_finished() never actually runs
 *              during this wait, WaitForCallBack just counts down to 0
 *              every time (a ~10000-iteration busy-wait doing nothing
 *              useful), and JavaScrollPosition is always stale/whatever
 *              it was from some previous, unrelated call. Flagging this
 *              rather than changing it, since fixing it (e.g. dropping
 *              the busy-wait and making callers handle the result
 *              asynchronously) is a behavioral change beyond the scope
 *              of a comments pass - worth a focused look/test on real
 *              hardware.
 *-----------------------------------------------*/
float ScrollGetPosition(void) {

#ifndef GTK_4

    WaitForCallBack = 10000;

    webkit_web_view_run_javascript(web_view, "window.pageYOffset.toString();",
                                   NULL, web_view_javascript_finished,
                                   (void *)1);
    //    webkit_web_view_evaluate_javascript_finish(web_view,
    //    "window.pageYOffset.toString();",
    //                                   NULL,  web_view_javascript_finished,
    //                                   (void *)1);

    while (--WaitForCallBack > 0)
        ;

    //  printf("Scroll J %d-> S %d  Wait %d\n",
    //         JavaScrollPosition, ScrollPosition, WaitForCallBack);
    return (JavaScrollPosition);
#endif
}

/*----------------------------------------------
 * Function:        ScalePage
 *
 * Description: "Fit to page" zoom: computes a WebKit zoom level that
 *              would make the loaded page's content fit within the
 *              scrolled window's current viewport, then applies the
 *              smaller of the horizontal/vertical scale factors (so
 *              the whole page fits without cropping in either
 *              direction) via webkit_web_view_set_zoom_level().
 *              ScaleH/V = viewport page_size / content upper bound,
 *              i.e. "what fraction of the current zoom would make the
 *              content exactly fill the viewport."
 *
 *              (Previously this was reading `Horiz`/`Vert` completely
 *              uninitialized - they were declared but never assigned
 *              before use, a real bug that made the "doesn't work
 *              well" comment below unsurprising. Fixed to pull the
 *              viewport size from gtk_adjustment_get_page_size(); see
 *              CLEANUP_PROGRESS.md bug log / patch 0002. Still worth
 *              re-testing visually since the fix changes the actual
 *              computed zoom level, not just silences a warning.)
 *
 *-----------------------------------------------*/
int ScalePage(void) {

    gfloat UpperH, UpperV;
    GtkAdjustment *Adjust;
    gint Horiz, Vert;
    gfloat ScaleH, ScaleV;

    Adjust =
        gtk_scrolled_window_get_hadjustment((GtkScrolledWindow *)ChartGTKView);
    UpperH = gtk_adjustment_get_upper(Adjust);
    Horiz = (gint)gtk_adjustment_get_page_size(Adjust);

    Adjust =
        gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)ChartGTKView);
    UpperV = gtk_adjustment_get_upper(Adjust);
    Vert = (gint)gtk_adjustment_get_page_size(Adjust);
    printf("ScalePage %x %f\n", Adjust, UpperV);

    if (UpperH != 0) {
        ScaleH = ((gfloat)Horiz) / (gfloat)UpperH;
    } else {
        ScaleH = 1.0;
    }

    if (UpperV != 0) {
        ScaleV = ((gfloat)Vert) / (gfloat)UpperV;
    } else {
        ScaleV = 1.0;
    }

    webkit_web_view_set_zoom_level(web_view, MIN(ScaleV, ScaleH) + 0.001);
    return (0);
}

/*----------------------------------------------
 * Function:        on_DrumLoopButton_clicked
 *
 * Description: Toggle the drum loop/backing track on or off (via
 *              ToggleMidiLoop()). (The doc here previously said "Scale
 *              the graphics to fit the page" - copy/paste leftover
 *              from the neighboring ScalePage(); corrected.)
 *
 *-----------------------------------------------*/
void on_DrumLoopButton_clicked(GtkWidget *widget, gpointer data) {

    //   webkit_web_view_reload(web_view);
    gtk_image_set_from_pixbuf(GTK_IMAGE(DrumLoopButton.Image),
                              DrumLoopButton.ButtonDownImage);

    ToggleMidiLoop();
    //    ScalePage();
}

unsigned int CurTapTempo;
struct timeval Time0;
/*----------------------------------------------
 * Function:        on_TapTempo_clicked
 *
 * Description: Tap-tempo button handler: on each click, measures the
 *              elapsed time since the previous click and derives a BPM
 *              from it (60,000,000 / elapsed-microseconds), then
 *              rolling-averages that into gMyInfo.Tempo (see the
 *              MinTempoValue/MaxTempoValue-gated averaging below). A
 *              ctrl-click or right-click instead commits the current
 *              running average (CurTapTempo) as the tempo immediately,
 *              without waiting for another tap.
 *
 *              *** LIKELY BUG - WORTH VERIFYING ***
 *              `Time1` is declared as `struct timeval` (seconds +
 *              MICROseconds) but passed to clock_gettime(), which
 *              expects `struct timespec` (seconds + NANOseconds) - a
 *              type mismatch normally caught by
 *              -Wincompatible-pointer-types, which is suppressed
 *              project-wide (see Makefile.am). On this platform the two
 *              structs happen to be the same size, so it doesn't crash,
 *              but clock_gettime() is actually writing NANOseconds into
 *              a field this function then treats as MICROseconds (used
 *              directly in the BPM math, and divided by 1000 as if
 *              converting microseconds to milliseconds). That would
 *              make every BPM calculation here off by roughly 1000x in
 *              its sub-second component, badly corrupting the tap-tempo
 *              result. Flagging rather than fixing in this
 *              comments-only pass, since correcting it (switching
 *              Time0/Time1 to struct timespec and fixing the math
 *              throughout this function) is a behavioral change worth
 *              testing on real hardware.
 *
 *-----------------------------------------------*/
gboolean on_TapTempo_clicked(GtkWidget *widget, GdkEvent *event,
                             gpointer user_data) {
    char String[20];
    struct timeval Time1;
    long long elapsedTime;
    int ReturnVal;
#ifdef GTK_4
    int MyButton = !button(event);
    int MyState = !state(event);
#else
    int MyButton = event->button.button;
    int MyState = event->button.state;
#endif

    ReturnVal = clock_gettime(CLOCK_REALTIME, &Time1);

#if 0
    long NowTime, OldTime;
    // Get the new time
    // ReturnVal = clock_getres(CLOCK_REALTIME, &Time1);
    // printf("TimeRes %ld %ld %d\n", Time1.tv_sec,Time1.tv_usec, ReturnVal);

    NowTime = (long)((((long)(Time1.tv_sec) * 1000000) + ((long)(Time1.tv_usec) / 1000)))  ;
    printf("Now %18ld \n", NowTime);
    OldTime = (long)((((long)(Time0.tv_sec) * 1000000) + ((long)(Time0.tv_usec) / 1000)))  ;
    printf("Old %18ld \n", OldTime);
    printf("Dif %18ld  %d \n", NowTime - OldTime, 60000000 / (NowTime - OldTime));
    printf("msec %18ld %18ld \n", (Time1.tv_sec - Time0.tv_sec), Time1.tv_usec - Time0.tv_usec);

    NowTime = Time1.tv_usec - Time0.tv_usec;

    if (NowTime < 0) {
        NowTime = ((1000000000 + Time1.tv_usec) - Time0.tv_usec);
        printf("Neg %ld\n", NowTime);
    }

    else {
        printf("Nor %ld\n", NowTime);
    }

#endif

    if (Time1.tv_usec - Time0.tv_usec) {
        elapsedTime =
            (long)(60000000 /
                   (((long long)(Time1.tv_sec - Time0.tv_sec) * 1000000) +
                    ((long long)(Time1.tv_usec - Time0.tv_usec) / 1000)));
    } else {
        elapsedTime = 0;
    }

    Time0 = Time1;

    /* If it's a ctrl click or
    a right lick then assign the value.
    */
    if ((MyState & GDK_CONTROL_MASK) || (MyButton == 3)) {
        gMyInfo.Tempo = CurTapTempo;
        g_idle_add(GTKIdel_cb, theMainWindow);
        return (CurTapTempo);
    }

    // Calculate BPM and average with previous value.
    if (elapsedTime > MinTempoValue && elapsedTime < MaxTempoValue) {
        CurTapTempo = (0.65 * CurTapTempo) + (0.35 * elapsedTime);
    }

    sprintf(String, "Tap=%d", CurTapTempo);
    MyImageButtonSetText(&TapTempoButton, String);

    /*
     * Draw the button
     */
    gtk_image_set_from_pixbuf(GTK_IMAGE(TapTempoButton.Image),
                              TapTempoButton.ButtonDownImage);

    return (1);
}

/*----------------------------------------------
 * Function:        on_patch_clicked
 *
 * Description: Click handler for one of the numbered preset buttons
 *              (CPatch = which button, 0-based, from user_data). Two
 *              distinct behaviors depending on the Ctrl key:
 *                - Ctrl+click: opens the patch-picker popup
 *                  (ShowPatchListSelect) to let the user (re)assign
 *                  which patch this preset button points to, rather
 *                  than triggering the patch itself.
 *                - Plain click: looks up which patch this preset
 *                  button is currently assigned to
 *                  (gMyInfo.WebPresets.thePreset[CPatch]), applies it
 *                  via DoPatch(), and redraws the button in its
 *                  "pressed" state.
 *              (Note: Preset indices up to Max_Patches are accepted,
 *              not just < Max_Patches - MyPatchInfo[] is actually
 *              sized to Max_Patches_W_Presets to also hold the special
 *              "preset of a preset" slots, so this isn't an
 *              off-by-one.)
 *
 *-----------------------------------------------*/
gboolean on_patch_clicked(GtkWidget *widget, GdkEvent *event,
                          gpointer user_data) {
    tPatchIndex Preset;
    int CPatch;
    GdkEvent *theEvent;
#ifdef GTK_4
    int MyState = !state(event);
    /* Event status for CTRL key.
     */
    theEvent = gtk_event_controller_get_current_event(widget);
#else
    int MyState = event->button.state;
    /* Event status for CTRL key.
     */
    theEvent = gtk_get_current_event();
#endif

    /* Preset Number.
     */
    CPatch = (intptr_t)user_data;
    printd(LogHTML, "1 In Button Preset %d %d\n", CPatch, theEvent);

    /* Check to make sure the preset value is valid.    */
    if (CPatch >= 0 && CPatch < MaxPresetButtons) {
        Preset = gMyInfo.WebPresets.thePreset[CPatch];
    } else {
        return (false);
    }

    /* Here is where we handle the CTRL Click to assign.
     */
    if (MyState & GDK_CONTROL_MASK) {
        printd(LogHTML, "We have Control Down\n");
        /* FINISH, need to make the assignment in Patch_Popup_CB
         */
        CurrentPreset = CPatch + Max_Patches + 1;
        ShowPatchListSelect(GTK_WINDOW(widget), CPatch);

        return (TRUE);
    }

    printd(LogHTML, "2 In Button Preset %d %d\n", CPatch, Preset);

    /* Make sure the preset is at least in the correct range.
     */
    if (Preset < 0 || Preset > Max_Patches) {
        return (false);
    }

    printd(LogHTML, "3 In Button Preset Name %s\n",
           gMyInfo.MyPatchInfo[Preset].Name);

    /* Execute the patch change.
     */
    DoPatch(&gMyInfo.MyPatchInfo[Preset]);

    /* Redraw the button.   */
    gtk_image_set_from_pixbuf(GTK_IMAGE(PresetButtons[CPatch].Image),
                              PresetButtons[CPatch].ButtonDownImage);

    return (1);
}

/*----------------------------------------------
 * Function:        on_patch__release_handler
 *
 * Description: Button-release counterpart to on_patch_clicked() -
 *              just redraws the preset button back to its "up"
 *              (unpressed) image. Doesn't re-trigger or undo the
 *              patch change itself, that already happened on press.
 *
 *-----------------------------------------------*/
gboolean on_patch__release_handler(GtkWidget *widget, GdkEvent *event,
                                   gpointer user_data) {
    theImageButtons *theButton;
    theButton = (theImageButtons *)user_data;
    //  PatchIndex = LayoutSwitchPatch(user_data, true);

    gtk_image_set_from_pixbuf(
        GTK_IMAGE(PresetButtons[(intptr_t)user_data].Image),
        PresetButtons[(intptr_t)user_data].ButtonUpImage);
    return TRUE; /* stop event propagation */
}

#if 0
/* Disabled (#if 0) old implementation of on_SaveWeb_clicked: toggled
 * WebKit's own in-page rich-text editing mode (editable overlay on the
 * loaded HTML) and, on save, wrote the WebView's title/content
 * straight back to the file. Superseded by the #else branch below,
 * which instead shells out to an external HTML editor - presumably
 * because in-page WebKit editing was unreliable or insufficient for
 * real chart editing. Left here for reference. */
/*----------------------------------------------
 * Function:        on_SaveWeb_clicked
 *
 * Description: Save the changes to the file
 *
 *-----------------------------------------------*/
void on_SaveWeb_clicked(GtkWidget *widget, gpointer data) {
    const gchar *CurrentURI;
    const char *Buffer;
    GString* theBuffer;
    FILE *fp;

    if (webkit_web_view_is_editable(web_view)) {
        MyImageButtonSetText(&SaveWebButton, "Edit");
        webkit_web_view_set_editable(web_view, FALSE);
        return;
    }

    MyImageButtonSetText(&SaveWebButton, "Save");
    webkit_web_view_set_editable(web_view, TRUE);

    /*
     * Draw the button
     */
    gtk_image_set_from_pixbuf(GTK_IMAGE(SaveWebButton.Image),
                              SaveWebButton.ButtonDownImage);


    CurrentURI = webkit_web_view_get_uri(web_view);
    g_print("Save %s\n", CurrentURI);
    Buffer = webkit_web_view_get_title(web_view);

    printd(LogHTML, "Len = %d\n %s\n", (int) strlen(Buffer), Buffer);

    /* Get passed the file://
     */
    CurrentURI += 7;

    if ((fp = fopen(CurrentURI, "w")) == NULL) {
        g_print("File open Failed %s\n", CurrentURI);
        return;
    }

    fwrite(Buffer, strlen(Buffer), 1, fp);
    fclose(fp);
}
#else
/*----------------------------------------------
 * Function:        on_SaveWeb_clicked
 *
 * Description: "Edit"/save button for the current song's HTML file:
 *              shells out to `MusicApps.sh html <path>` (an external
 *              editor launcher script, per the MusicApps.sh
 *              convention used elsewhere in this codebase) rather than
 *              editing in-page. See the #if 0 block above for the
 *              earlier, in-WebView-editing approach this replaced.
 *
 *-----------------------------------------------*/
void on_SaveWeb_clicked(GtkWidget *widget, gpointer data) {
    const gchar *CurrentURI;
    char ExecuteString[200];

    /*
     * Draw the button
     */
    gtk_image_set_from_pixbuf(GTK_IMAGE(SaveWebButton.Image),
                              SaveWebButton.ButtonDownImage);

    // Get the current URI of the viewable file.
    CurrentURI = webkit_web_view_get_uri(web_view);

    // Call the text editor.
    //  sprintf(ExecuteString, "gedit %s &\n",
    //          CurrentURI);
    // sprintf(ExecuteString, "%s %s &\n",
    //         gMyInfo.Apps[HTMLEditor].Name,
    //         &CurrentURI[7]);
    sprintf(ExecuteString, "MusicApps.sh html %s &\n", &CurrentURI[7]);
    printd(LogHTML, "Edit: %s\n", ExecuteString);
    system(ExecuteString);
}
#endif
/*----------------------------------------------
 * Function:        PageLoaded
 *
 * Description: WebKit load-status callback, fired on every navigation
 *              event. Only acts on WEBKIT_LOAD_COMMITTED (note: NOT
 *              WEBKIT_LOAD_FINISHED, despite what the old comment
 *              above the check used to say - COMMITTED fires once the
 *              new URI is known, which is what this function needs).
 *              On a committed load:
 *                - Derives BasePathName (the containing folder) and
 *                  FileName from the URI, for relative-path resolution
 *                  elsewhere.
 *                - Logs the navigation to the history file.
 *                - If it's an .html file: runs Search_in_File() to
 *                  pick up any web-preset/tempo meta tags embedded in
 *                  the page, and updates the status bar unless it's
 *                  the index page.
 *                - If it's .mp3 or .mid: just logs and returns (these
 *                  aren't HTML content to scan for presets).
 *
 *              *** BUG: the zoom reset below is dead code ***
 *              `webkit_web_view_set_zoom_level(web_view, 1)` at the
 *              end of this function can never execute - every branch
 *              of the .mp3/.mid/else `if` chain above it ends in
 *              `return`, including the plain `else` for any URI that
 *              matched none of them. So whatever this line was meant
 *              to do (reset zoom to 100% after every load, most
 *              likely undoing ScalePage()'s zoom-to-fit) simply never
 *              runs. Flagging rather than fixing in this comments
 *              pass - moving the reset earlier would be a real
 *              behavior change worth testing.
 *
 *-----------------------------------------------*/
void PageLoaded(WebKitWebView *web_view, WebKitLoadEvent load_event,
                gpointer user_data) {
    gchar *MyCurrentURI;
    char *Pointer;
    int Loop;
    char FileName[FileNameMaxLength];

    /* Get the URL that was selected.
     */
    MyCurrentURI = (gchar *)webkit_web_view_get_uri(web_view);
    printd(LogHTML, "load_status_cb %s event %d \n", MyCurrentURI, load_event);
    ScrollPosition = 0;

    /* We only care about the completed event.
    WEBKIT_LOAD_STARTED, WEBKIT_LOAD_REDIRECTED,
    WEBKIT_LOAD_COMMITTED, WEBKIT_LOAD_FINISHED
    *** Use WEBKIT_LOAD_FINISHED to reload after ever click.
    */
    if (load_event != WEBKIT_LOAD_COMMITTED) {
        return;
    }

#if 1
    /* We need an absolute path. For files in the current
    song folder add the full path before the name.
    */
    strcpy(BasePathName, MyCurrentURI);

    for (Loop = strlen(BasePathName); Loop >= 0; Loop--)
        if (BasePathName[Loop] == '/') {
            strcpy(FileName, &BasePathName[Loop + 1]);
            BasePathName[Loop + 1] = 0;
            Loop = -1;
        }

    //  printd(LogHTML, "Base Name %s \n", BasePathName);
#endif
    printd(LogHTML, "After Basename %s \n", BasePathName);

#if 0

    if (strcmp("index.html", FileName)) {
        printf("match Basename1 %s \n", FileName);
        //      UpdateStatus(FileName);
    }

#endif

    /* Keep a record
     */
    printd(LogHTML, "Pageloaded Title %s \n",
           webkit_web_view_get_title(web_view));
    WriteToHistory(MyCurrentURI);
    //  Pointer = strstr(MyCurrentURI, ".html");

    /* Let's check to see if this is an HTML file.
     */
    if (strstr(MyCurrentURI, ".html")) {

        /* Search for specifics like web presets */
        Search_in_File(MyCurrentURI, &gMyInfo.WebPresets);

        /* Add the file to the status.
         */
        if (!strstr(MyCurrentURI, "index")) {
            UpdateStatus("---Load---");
        }
    }
    if (strstr(MyCurrentURI, ".mp3")) {
        printd(LogHTML, "*** MP3 file.\n");
        //      UpdateStatus(FileName);

        return;
    }

    else if (strstr(MyCurrentURI, ".mid")) {
        printd(LogHTML, "*** mid file.\n");
        //      UpdateStatus(FileName);
        return;
    }

    else {
        printd(LogHTML, "Not HTML File\n");
        return;
    }

    //  webkit_web_view_set_editable(web_view, FALSE);
    webkit_web_view_set_zoom_level(web_view, 1);
}

/*----------------------------------------------
 * Function:        NavigationPolicy
 *
 * Description: WebKit response-policy-decision callback: for every
 *              resource WebKit is about to load, decides whether
 *              WebKit should render it normally (return FALSE) or
 *              whether this app is going to handle it instead
 *              (webkit_policy_decision_ignore() + return TRUE).
 *
 *              Decision flow:
 *                1. `.rtf` or no extension at all, or `.html`: let
 *                   WebKit render it normally (return TRUE here just
 *                   means "decision made", not "let WebKit handle
 *                   it" - see the note below, this looks like a bug).
 *                2. Otherwise, try delegating to the external
 *                   `MusicApps.sh <ext> <path>` script via system().
 *                   If that script exits 0 (success = "I handled
 *                   it"), tell WebKit to ignore the request and stop.
 *                3. If MusicApps.sh didn't handle it (nonzero exit),
 *                   fall back to handling known media types directly:
 *                   .mp3/.mp4/.webm/.mkv/.mid all get routed to the
 *                   internal player (SetPlayerFile/StartMidiLoop)
 *                   instead of being opened as a web resource.
 *                4. Anything else: return FALSE, let WebKit handle it.
 *
 *              `(theURI + 7)` throughout skips the "file://" scheme
 *              prefix, same assumption as on_Back_clicked() - only
 *              valid for local file:// URIs.
 *
 *              *** LIKELY BUG (pre-existing, not from this pass) ***
 *              Step 1's `.rtf`/no-extension/`.html` branches all
 *              `return (TRUE)` with a comment "Let WebKit handle it" -
 *              but returning TRUE from a WebKitPolicyDecision callback
 *              means "I'm handling this" (matching this function's own
 *              header doc: "Return TRUE means we are handling the
 *              URL"), and without a matching
 *              webkit_policy_decision_ignore()/_use() call, the
 *              decision is left unresolved. The commented-out
 *              `return (FALSE)` right above each of these suggests an
 *              earlier version had this the other way around, and it
 *              got inverted at some point. Flagging rather than
 *              changing, since it's a routing behavior change worth
 *              testing rather than a comments-pass fix.
 *
 *-----------------------------------------------*/
gboolean NavigationPolicy(WebKitWebView *web_view,
                          WebKitPolicyDecision *decision,
                          WebKitPolicyDecisionType decision_type,
                          gpointer user_data) {
    char *theOrgURI;
    char theURI[250];
    char string[1000];
    int Loop;
    char *mimeType;
    char *ext;
    int SysRet;
    char *PageIndex;
    int PageNumber;
    char *FileName;

    // if (decision_type != WEBKIT_POLICY_DECISION_TYPE_RESPONSE) {
    //     return FALSE;
    // }

    printd(LogHTML, "NavigationPolicy\n");
    if (decision == NULL || web_view == NULL) {
        printd(LogHTML, "*** decision Null\n");
        return TRUE;
    }

    WebKitResponsePolicyDecision *responseDecision =
        //    WebKitNavigationPolicyDecision *responseDecision =
        WEBKIT_RESPONSE_POLICY_DECISION(decision);
    WebKitWebResource *mainResource =
        webkit_web_view_get_main_resource(web_view);
    WebKitURIRequest *request =
        webkit_response_policy_decision_get_request(responseDecision);

    if (request == NULL) {
        printd(LogHTML, "*** request Null\n");
        return TRUE;
    }

    char *requestURI = (char *)webkit_uri_request_get_uri(request);

    printd(LogHTML, "*** requestURI web_view %s \n",
           webkit_web_view_get_uri(web_view));

    if (requestURI == NULL) {
        printd(LogHTML, "*** URI Null\n");
        return TRUE;
    }

    // GET LINK NAME !!!!!

    printd(LogHTML, "*** requestURI %s %s\n", requestURI,
           webkit_web_resource_get_uri(mainResource));

    //   webkit_policy_decision_download(decision);
    //   return TRUE;

    theOrgURI = requestURI;
    DecodeURI(theOrgURI, theURI);
    printd(LogHTML, "NavigationPolicy2 %s %s \n", theURI, theOrgURI);

    /* Default: if no '/' is found below, treat the whole URI as the
     * filename rather than leaving FileName uninitialized (it's
     * dereferenced further below in the .mid/.mp3/.mp4 branches).
     */
    FileName = requestURI;

    for (Loop = strlen(requestURI); Loop >= 0; Loop--)
        if (requestURI[Loop] == '/') {
            FileName = &requestURI[Loop + 1];
            Loop = -1;
        }

    /* Fallback: no '/' found in the URI, so there's nothing to split off -
     * treat the whole URI as the filename rather than leaving FileName
     * uninitialized (it's dereferenced further below).
     */
    if (FileName == requestURI + strlen(requestURI) || FileName == NULL) {
        FileName = requestURI;
    }

    /* get the extension of the file.
     */
    ext = strrchr(theURI, '.');

    // This is for the Save Favorites link.
    if (ext != NULL) {
        strcpy(LastClickURI, (theURI + 7));
    }

    /* If it's a web page we want to display,
    Let the WebKit handle it.
    */
    if (strstr(theURI, ".rtf") || (ext == NULL)) {
        printd(LogHTML, "*** theURI is rtf");
        //       return (FALSE);
        return (TRUE);
    }

    if (strstr(theURI, ".html") || (ext == NULL)) {
        printd(LogHTML, "*** theURI is html");
        //       return (FALSE);
        return (TRUE);
    }
    //    return TRUE;

    // Let's check if the MusicApp.sh can handle this.
    sprintf(string, "MusicApps.sh %s \'%s\' \'%s\' ", ext + 1, &theURI[7],
            theURI);

    SysRet = system(string);
    printd(LogInfo, "*** systemcall %d %s\n", SysRet, string);
    printd(LogHTML, "NavigationPolicy Title %s \n",
           webkit_web_view_get_title(web_view));
    //   printd(LogHTML, "NavigationPolicy Title %s \n",
    //   webkit_hit_test_result_get_link_title());

    WriteToHistory(theURI);

    /*
     * This tells webkit we are dealing with it.
     */
    if (!SysRet) {
        webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
        printd(LogInfo, "NavPol return true\n");
        return (TRUE);
    }

    /* If we find an MP3 file then handle it ourselves and tell WebKit
     * not to deal with it.
     */
    if (strstr(theURI, ".mp3")) {
        /*
         * Tell web kit not to o anything with it.
         */
        SetPlayerFile((theURI + 7));
        printd(LogInfo, "Call SetPlayer %s \n", theURI);

        webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
        printd(LogHTML, "*** After systemcall %s\n", SysCallString);
        printf("*** MP3 file. %s\n", FileName);

        UpdateStatus(FileName);

        /*
         * This tells webkit we are dealing with it.
         */
        return (TRUE);
    }

#if 1
    if (strstr(theURI, ".mp4") || strstr(theURI, ".webm") || strstr(theURI, ".mkv")) {
        /*
         * Tell web kit not to o anything with it.
         */
        SetPlayerFile((theURI + 7));
        printd(LogInfo, "Call SetPlayer %s \n", theURI);

        webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
        printd(LogHTML, "*** MP4 After systemcall %s\n", SysCallString);

        UpdateStatus(FileName);

        /*
         * This tells webkit we are dealing with it.
         */
        return (TRUE);
    }
#endif

    if (strstr(theURI, ".mid")) {
        /*
         * Tell web kit not to o anything with it.
         */
        printd(LogInfo, "Midi file %s \n", theURI);
        StartMidiLoop((theURI + 7));

        strcpy(LastDrumName, (theURI + 7));
        webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
        printf("*** Midi file. %s\n", FileName);

        UpdateStatus(FileName);

        /*
         * This tells webkit we are dealing with it.
         */
        return (TRUE);
    }

    // Tell Webkit to handle the URI
    return (FALSE);
}

/*----------------------------------------------
 * Function:        OpenSetListSong
 *
 * Description: Open the Nth song (1-based `SongNumber`) from the
 *              currently-loaded set list HTML file
 *              (SetListFileName). Does a simple manual scan of the
 *              file's raw text for `href=` occurrences (not real
 *              HTML parsing) - the Nth href found is treated as song
 *              N, its target extracted with `strtok(Found, ">")` up
 *              to the closing `>` of the `<a href="...">` tag, and
 *              that becomes the URI loaded into the WebView.
 *              `Found += 6` skips past the literal `href="` (5 chars
 *              for "href=" + 1 for the opening quote) to land on the
 *              actual link target.
 *
 *              *** POSSIBLE BUG - WORTH VERIFYING ***
 *              The resolved path is built as
 *              dirname(set-list-file) + "/." + tokenizer - note the
 *              literal "." before the song filename, which makes the
 *              final path something like ".../SongsDir/.SongName.html"
 *              (a dot-prefixed/hidden filename). This is the only
 *              place in the file that does this; nowhere else writes
 *              set-list entries with a leading dot on the filename
 *              (compare DrumFav_cb/SaveFav_cb below, which write plain
 *              paths). If set-list entries are actually just
 *              "SongName.html" without a leading dot, this would be
 *              looking for a file that doesn't exist and silently fail
 *              to load. Could be `strcat(Copy, "/.")` was meant to be
 *              `strcat(Copy, "/")` (i.e. the "." is a stray typo, not
 *              a real relative-path "./" marker - there'd be no reason
 *              for one here since dirname() already gives an absolute
 *              path). Flagging rather than changing - would need to
 *              see a real set-list file's actual href format to be
 *              sure.
 *
 *-----------------------------------------------*/
void OpenSetListSong(int SongNumber) {
    int Loop;
    char temp[FileMaxLineLength];
    char Copy[FileMaxLineLength];
    char *tokenizer;
    char *Found;
    int Count = 0;
    int SongCount = 0;

    printd(LogHTML, "Made it to OpenSetListSong %d\n", SongNumber);

    if (SongNumber < 1) {
        printd(LogHTML, "Invalid SongNumber %d\n", SongNumber);
        return;
    }

    SetListFile = fopen(&SetListFileName[7], "r");

    if (!SetListFile) {
        printd(LogHTML, "Can't open Setlist file  %s\n", SetListFileName);
        return;
    }

    /*
     * Walk thru the file and find the HREF links.
     */
    while (fgets(temp, FileMaxLineLength - 1, SetListFile) != NULL &&
           (++Count < 150)) {
        temp[FileMaxLineLength - 1] = 0;

        /*
         * Look for the Links and .Count the number.
         */
        Found = strstr(temp, "href=");

        if (Found != NULL) {
            SongCount++;
            Found += 6;
            tokenizer = strtok(Found, ">");
            printd(LogHTML, "Parser found %d %s\n", SongCount, tokenizer);

            /*
             * If the is the SongNumber HREF count then grab the file name.
             */
            if (SongCount == SongNumber) {
                CurrentSetListSong = SongNumber;
                strcpy(Copy, SetListFileName);
                //              dirname(SetListFileName);
                dirname(Copy);
                printd(LogHTML, "After  %s\n", Copy);
                strcat(Copy, "/.");
                strcat(Copy, tokenizer);
                printd(LogHTML, "Final  %s\n", Copy);
#if 1
                webkit_web_view_load_uri(web_view, Copy);
#else
                webkit_web_view_load_html(web_view, Copy, "file:///");
#endif
                break;
            }
        }
    }

    printd(LogHTML, "Leaving OpenSetListSong\n");

    // Close the file if still open.
    if (SetListFile) {
        fclose(SetListFile);
    }
}

/*----------------------------------------------
 * Function:        on_SetList_clicked
 *
 * Description: "Load this page as the set list" button handler: takes
 *              the currently-displayed page's URI and remembers it as
 *              SetListFileName (used later by OpenSetListSong() to
 *              open songs from it). Also updates the SetList button's
 *              own label to show just the base filename (no path, no
 *              ".html" extension - `Length - 5` chops the trailing
 *              5-char ".html" suffix; assumes the URI always ends in
 *              exactly that extension).
 *              (The doc here previously said "Save the changes to the
 *              file" - leftover from SavePresetChanges(); corrected.)
 *
 *-----------------------------------------------*/
void on_SetList_clicked(GtkWidget *widget, gpointer data) {
    const gchar *CurrentURI;
    char *BasePtr;
    int Length;
    int Loop;

    printd(LogHTML, "on_SetList_clicked  1\n");

    CurrentURI = webkit_web_view_get_uri(web_view);
    strcpy(SetListFileName, CurrentURI);
    Length = strlen(SetListFileName);

    for (Loop = Length; Loop; Loop--)
        if (SetListFileName[Loop] == '/') {
            break;
        }

    //  BasePtr = basename(SetListFileName);
    BasePtr = &SetListFileName[Loop + 1];
    Length = strlen(BasePtr);
    BasePtr[Length - 5] = 0;
    MyImageButtonSetText(&SetListButton, BasePtr);
    strcpy(SetListFileName, CurrentURI);
    printd(LogHTML, "on_SetList_clicked  4\n");
}

// /usr/lib/mozilla/plugins
// libevbrowserplugin.so
/*----------------------------------------------
 * Function:        InitHTML
 *
 * Description: One-time setup of the "chart display" side of the UI,
 *              called once from the app's startup path with the
 *              GtkBuilder handle for the main window's Glade layout.
 *              Three main things happen here, in order:
 *                1. Wire up all the fixed on-screen buttons related to
 *                   HTML browsing (preset/patch buttons 1..N, Back,
 *                   Forward, Drum loop, Tap tempo, Edit/SaveWeb,
 *                   SetList, Play/Pause): look each widget up from the
 *                   Glade builder by name, initialize it as a
 *                   theImageButtons (2-state image button), and
 *                   connect its press/release signal handlers. This
 *                   is repetitive by nature - same pattern per button,
 *                   just a different Glade widget name/handler/image
 *                   set each time.
 *                2. Create the embedded WebKitWebView, parent it into
 *                   the "scrolledwindow1" Glade widget (ChartGTKView),
 *                   wire its load-changed/decide-policy/hovering-over-
 *                   link/context-menu signals to PageLoaded/
 *                   NavigationPolicy/HoverLink_cb/on_RightMenu_clicked,
 *                   and load the initial page from gMyInfo.BasePath.
 *                3. Configure WebKitSettings for that view (font size,
 *                   disabled media stream/mediasource, fullscreen
 *                   allowed, page cache disabled) and apply them.
 *
 *              Notes on specific non-obvious bits:
 *              - The Glade widget backing DrumLoopButton is named
 *                "ScaleButton" in the .glade file, not "DrumButton" or
 *                similar - likely a repurposed widget from an earlier
 *                UI iteration whose Glade ID was never renamed to
 *                match its current DrumLoop function. Worth knowing if
 *                editing the .glade layout directly.
 *              - Font sizing has a disabled (#if 0) screen-size-aware
 *                block (12/18/28pt for ScreenSize 0/1/2) - the #else
 *                branch that's actually active just hardcodes 28pt
 *                regardless of screen size.
 *              - The `FileName` URI buffer is built with two
 *                back-to-back strncpy() calls and no explicit null
 *                terminator; it works because strncpy() zero-pads the
 *                remainder of the destination when the source is
 *                shorter than the given length, so termination happens
 *                to fall out of that padding behavior rather than
 *                being done explicitly - fragile if BasePath were ever
 *                as long as the buffer itself.
 *
 *-----------------------------------------------*/
void InitHTML(GtkBuilder *gxml) {
    char FileName[255];
    GtkWidget *Widget;
    int Loop;
    char Buffer[40];
    GtkWidget *EventBox;

    /* Load the buttons and set the callbacks for them.
     */
    CurrentSetListSong = 0;
    printd(LogHTML, "InitHTML\n");

    for (Loop = 0; Loop < MaxPresetButtons - 1; Loop++) {
        sprintf(Buffer, "Patch%d", Loop + 1);
        EventBox =
            GTK_WIDGET(gtk_builder_get_object((GtkBuilder *)gxml, Buffer));
        //      gtk_widget_get_usize(EventBox);

        MyImageButtonInit(&PresetButtons[Loop], EventBox, PatchButtonOnImage,
                          PatchButtonOffImage);
        MyImageButtonSetText2(&PresetButtons[Loop], Loop + 1, Buffer);
        gtk_widget_set_tooltip_text(
            PresetButtons[Loop].EventBox,
            "CTRL-Click to set. This will get saved in the song file.");

        g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                         G_CALLBACK(on_patch_clicked), (gpointer)Loop);

        g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                         G_CALLBACK(on_patch__release_handler), (gpointer)Loop);
    }

    ChartGTKView = GTK_WIDGET(gtk_builder_get_object(gxml, "scrolledwindow1"));

    printd(LogHTML, "ChartGTKView %x\n", ChartGTKView);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ChartGTKView),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    /* Create a new webkit view to display our data.
     */
    web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
#ifdef GTK_4
//    gtk_box_pack_start(GTK_CONTAINER(ChartGTKView), GTK_WIDGET(web_view));
    gtk_widget_set_child(GTK_WIDGET(ChartGTKView), GTK_WIDGET(web_view));
#else
    gtk_container_add(GTK_CONTAINER(ChartGTKView), GTK_WIDGET(web_view));
#endif
    printd(LogHTML, "web_view %x\n", web_view);

    EventBox =
        GTK_WIDGET(gtk_builder_get_object((GtkBuilder *)gxml, "BackButton"));
    MyImageButtonInit(&BackButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&BackButton, "Back");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_Back_clicked), &BackButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(normal_release_handler), &BackButton);

    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "ForwardButton"));
    MyImageButtonInit(&ForwardButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&ForwardButton, "Forward");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_Forward_clicked), &ForwardButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(normal_release_handler), &ForwardButton);

    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "ScaleButton"));
    MyImageButtonInit(&DrumLoopButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&DrumLoopButton, "Drum");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_DrumLoopButton_clicked), &DrumLoopButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(normal_release_handler), &DrumLoopButton);

    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "TapTempo"));
    MyImageButtonInit(&TapTempoButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&TapTempoButton, "Tap");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_TapTempo_clicked), &TapTempoButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(normal_release_handler), &TapTempoButton);
    gtk_widget_set_tooltip_text(
        TapTempoButton.EventBox,
        "CTRL-Click or Right-Click to set metronome. The "
        "new tempo will be saved in the song file.");

    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "SaveWeb"));
    MyImageButtonInit(&SaveWebButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&SaveWebButton, "Edit");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_SaveWeb_clicked), &SaveWebButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(normal_release_handler), &SaveWebButton);
    gtk_widget_set_tooltip_text(SaveWebButton.EventBox, "Open HTML editor.");

    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "SetList"));
    MyImageButtonInit(&SetListButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&SetListButton, "SetList");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(on_SetList_clicked), &SetListButton);
    g_signal_connect(G_OBJECT(EventBox), "button-release-event",
                     G_CALLBACK(normal_release_handler), &SetListButton);
    gtk_widget_set_tooltip_text(
        SetListButton.EventBox,
        "With a valid HTML file open this will make it the default setlist "
        "allowing the patches to switch between charts.");

    EventBox = GTK_WIDGET(gtk_builder_get_object(gxml, "PlayPause"));
    MyImageButtonInit(&PlayPauseButton, EventBox, MainButtonOnImage,
                      MainButtonOffImage);
    MyImageButtonSetText(&PlayPauseButton, "Play");
    g_signal_connect(G_OBJECT(EventBox), "button-press-event",
                     G_CALLBACK(Play_click_handler), &PlayPauseButton);

    /* Register a callback that gets invoked each time that a page is finished
     * downloading */
    g_signal_connect(web_view, "load-changed", G_CALLBACK(PageLoaded), NULL);
    g_signal_connect(web_view, "decide-policy", G_CALLBACK(NavigationPolicy),
                     web_view);
    // Hover in case we use it,
    g_signal_connect(G_OBJECT(web_view), "hovering-over-link",
                     G_CALLBACK(HoverLink_cb), web_view);

    strncpy(FileName, "file://", 7);
    strncpy(&FileName[7], gMyInfo.BasePath, sizeof(FileName) - 7);
    printd(LogHTML, "Path %s %s\n", gMyInfo.BasePath, FileName);
    webkit_web_view_load_uri(web_view, FileName);

    WebKitSettings *settings = webkit_settings_new();
    printd(LogHTML, "settings %x\n", settings);

    g_object_set(G_OBJECT(settings), "enable-page-cache", FALSE, NULL);
    webkit_settings_set_enable_hyperlink_auditing(settings, TRUE);
    webkit_settings_get_enable_tabs_to_links(settings);

#if 0
    if (ScreenSize == 0) {
        g_object_set(G_OBJECT(settings), "default-font-size", 12, NULL);
        g_object_set(G_OBJECT(settings), "default-monospace-font-size", 12, NULL);
    }

    if (ScreenSize == 1) {
        g_object_set(G_OBJECT(settings), "default-font-size", 18, NULL);
        g_object_set(G_OBJECT(settings), "default-monospace-font-size", 18, NULL);
    }

    if (ScreenSize == 2) {
        g_object_set(G_OBJECT(settings), "default-font-size", 28, NULL);
        g_object_set(G_OBJECT(settings), "default-monospace-font-size", 28, NULL);
    }
#else
    g_object_set(G_OBJECT(settings), "default-font-size", 28, NULL);
    g_object_set(G_OBJECT(settings), "default-monospace-font-size", 28, NULL);
#endif

    printd(LogHTML, "Settings for webkit\n");
    webkit_settings_set_enable_media_stream(G_OBJECT(settings), FALSE);
    webkit_settings_set_enable_mediasource(G_OBJECT(settings), FALSE);
    webkit_settings_set_enable_fullscreen(G_OBJECT(settings), TRUE);

#if 0
    webkit_user_content_manager_add_style_sheet
    (WebKitUserContentManager * manager,
     WebKitUserStyleSheet * stylesheet);

    WebKitUserStyleSheet *
    webkit_user_style_sheet_new(const gchar * source,
                                WebKitUserContentInjectedFrames injected_frames,
                                WebKitUserStyleLevel level,
                                const gchar * const * allow_list,
                                const gchar * const * block_list);

    webkit_settings_set_enable_accelerated_2d_canvas(G_OBJECT(settings), TRUE);
    webkit_settings_set_draw_compositing_indicators(G_OBJECT(settings), FALSE);
    g_object_set(settings,
                 "enable-webgl", FALSE,
                 "enable-java", FALSE,
                 "enable-javascript", FALSE,
                 "enable-mock-capture-devices", FALSE,
                 "enable-offline-web-application-cache", FALSE,
                 "enable-page-cache", FALSE,
                 "enable-smooth-scrolling", FALSE,
                 "hardware-acceleration-policy", WEBKIT_HARDWARE_ACCELERATION_POLICY_ALWAYS
                );
#endif

    // Set up the context menu
    g_signal_connect(G_OBJECT(web_view), "context-menu",
                     G_CALLBACK(on_RightMenu_clicked), NULL);

    // if (ButtonSize < 121) {
    //     webkit_web_view_set_zoom_level(web_view, 0.75);
    // }

    /* Apply the result */
    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(web_view), settings);
    gtk_widget_show_all(ChartGTKView);
}

/*----------------------------------------------
 * Function:        HoverLink_cb
 *
 * Description: WebView "hovering-over-link" signal handler. Currently
 *              just logs the hovered link's title/URI - the "Hover in
 *              case we use it" comment at the connect site in
 *              InitHTML() suggests this was wired up for a future
 *              feature (e.g. a status-bar link preview) that was
 *              never built out.
 *-----------------------------------------------*/
void HoverLink_cb(WebKitWebView *web_view, gchar *title, gchar *uri,
                  gpointer user_data) {

    printd(LogHTML, "Hover %s %s\n", title, uri);
}

/*----------------------------------------------
 * Function: on_RightMenu_clicked
 *
 * Description: WebView "context-menu" signal handler - builds the
 *              right-click context menu shown over a song page,
 *              adding three custom items (Drum Fav / Edit Chart /
 *              Save link) on top of WebKit's own defaults, each wired
 *              to its own callback below. Returning FALSE tells
 *              WebKit to go ahead and show the (now-augmented) menu
 *              rather than suppressing it.
 *-----------------------------------------------*/
// https://webkitgtk.org/reference/webkit2gtk/stable/WebKitContextMenuItem.html
// https://developer.gnome.org/gio/stable/GAction.html#GAction-struct
// https://developer.gnome.org/glib/stable/glib-GVariant.html#GVariant
// https://github.com/vain/lariza/issues/54
//

gboolean on_RightMenu_clicked(WebKitWebView *page, WebKitContextMenu *menu,
                              char *hitTestResult,
                              gpointer contextMenuCallback) {
    GSimpleAction *action = NULL;

    WebKitContextMenuItem *MyItem;

    // ---------------------- Drum Fav
    action = g_simple_action_new("Drum Fav", NULL);
    g_signal_connect_swapped(G_OBJECT(action), "activate",
                             G_CALLBACK(DrumFav_cb), page);
    MyItem = webkit_context_menu_item_new_from_gaction((GAction *)action,
                                                       "Drum Fav", NULL);
    g_object_unref(action);

    webkit_context_menu_append(menu, MyItem);
    // ----------------------

    // ---------------------- Edit Chart
    action = g_simple_action_new("Edit Chart", NULL);
    g_signal_connect_swapped(G_OBJECT(action), "activate",
                             G_CALLBACK(EditChart_cb), page);
    MyItem = webkit_context_menu_item_new_from_gaction((GAction *)action,
                                                       "Edit Chart", NULL);
    g_object_unref(action);

    webkit_context_menu_append(menu, MyItem);
    // ----------------------

    // ---------------------- Save link
    action = g_simple_action_new("Save link", NULL);
    g_signal_connect_swapped(G_OBJECT(action), "activate",
                             G_CALLBACK(SaveFav_cb), page);
    MyItem = webkit_context_menu_item_new_from_gaction((GAction *)action,
                                                       "Save link", NULL);
    g_object_unref(action);

    webkit_context_menu_append(menu, MyItem);
    // ----------------------

    return (false);
}

/*----------------------------------------------
 * Function: DrumFav CallBack.
 *
 * Description: "Drum Fav" context menu item handler: appends the
 *              current drum-loop file (LastDrumName, set when a
 *              .mid file was last opened via NavigationPolicy) as an
 *              `<a href>` line to DrumFav.html, a simple growing
 *              bookmarks-style list of favorited drum loops. Uses a
 *              shell `echo ... >>` via system() rather than opening
 *              the file directly in C.
 *-----------------------------------------------*/
static void DrumFav_cb(gpointer *Data) {
    WebKitWebView *thepage;
    char lFileName[FileNameMaxLength * 2];

    thepage = (WebKitWebView *)Data;

    //    sprintf(lFileName, "echo %s >> %s\n", LastDrumName,
    //    GetResourceDir("DrumFav.txt", FileLocUser));
    sprintf(lFileName, "echo '<a href=\"%s\">%s</a>' >> %s\n", LastDrumName,
            LastDrumName, GetResourceDir("DrumFav.html", FileLocUser));

    printd(LogHTML, "DrumFav %s\n", lFileName);
    system(lFileName);

    //  printf("We got this %x %s\n", Data, webkit_web_view_get_uri(thepage));
}

/*----------------------------------------------
 * Function: SaveFav CallBack.
 *
 * Description: "Save link" context menu item handler - same pattern
 *              as DrumFav_cb() above, but appends the last-clicked
 *              link (LastClickURI, captured in NavigationPolicy when
 *              it saw a file extension) to SaveFav.html instead of
 *              DrumFav.html. General-purpose "bookmark this link"
 *              rather than drum-loop-specific.
 *-----------------------------------------------*/
static void SaveFav_cb(gpointer *Data) {
    WebKitWebView *thepage;
    gchar *CurrentURI;
    char lFileName[FileNameMaxLength * 2];

    // Get current Name minus the file://
    //    CurrentURI = webkit_web_view_get_uri(web_view) + 7;

    CurrentURI = LastClickURI;

    sprintf(lFileName, "echo '<a href=\"%s\">%s</a>' >> %s\n", CurrentURI,
            CurrentURI, GetResourceDir("SaveFav.html", FileLocUser));

    printd(LogHTML, "SaveFav %s\n", lFileName);
    system(lFileName);

    //  printf("We got this %x %s\n", Data, webkit_web_view_get_uri(thepage));
}

/*----------------------------------------------
 * Function: EditChart_cb CallBack.
 *
 * Description: "Edit Chart" context menu item handler: launches the
 *              external HTML editor for the currently-displayed page
 *              via `MusicApps.sh html <path>`, same mechanism as
 *              on_SaveWeb_clicked()'s active (#else) implementation
 *              higher up in this file - this is effectively that same
 *              action, reachable from the right-click menu as well as
 *              the toolbar "Edit" button.
 *-----------------------------------------------*/
static void EditChart_cb(gpointer *Data) {
    WebKitWebView *thepage;
    char ExecuteString[FileNameMaxLength];
    const gchar *CurrentURI;

    thepage = (WebKitWebView *)Data;

    // Get the current URI of the viewable file.
    CurrentURI = webkit_web_view_get_uri(thepage);

    sprintf(ExecuteString, "MusicApps.sh html %s &\n", &CurrentURI[7]);
    printd(LogHTML, "Edit: %s\n", ExecuteString);
    system(ExecuteString);

    //  printf("We got this %x %s\n", Data, webkit_web_view_get_uri(thepage));
}

#define ContentTagLen 9 /* strlen("content=\"") - see Search_in_File() */
/*----------------------------------------------
 * Function:        Search_in_File
 *
 * Description: Scans a song's HTML file line-by-line (up to 150
 *              lines) for a fixed set of custom `<meta name="Xxx"
 *              content="...">`-style tags embedded in the page, and
 *              applies each one found:
 *                - PresetN (N=1-9): assigns a patch to preset button N
 *                  via AssignPreset(), stored into thePresets->thePreset[].
 *                - Tempo: sets the song's tempo (60-160 BPM range
 *                  enforced) and updates the status bar.
 *                - LMA_Time: sets gMyInfo.TempoMax (doubled "since we
 *                  flash we need twice as many counts" per the
 *                  original comment - presumably a metronome LED/UI
 *                  flash rate tied to this).
 *                - SetNow: immediately assigns the "current" patch
 *                  (AssignPreset(0, ...)) rather than a numbered preset.
 *                - LoopFile / DrumFile: load an associated looper/drum
 *                  MIDI file for this song (DrumFile[0]==62 is '>' -
 *                  guards against picking up an unclosed/malformed tag
 *                  as a filename; "/dev/null" is treated as "no file").
 *                - IntroCount / BeatsPerMeasure / LoopLength: metronome/
 *                  looper timing parameters, each just parsed with
 *                  atoi() and stored, plus a status bar update.
 *
 *              Same "cheesy" substring-matching approach as
 *              SavePresetChanges() (not real HTML parsing) - and this
 *              is effectively the read-side counterpart to that
 *              function's write-side. Each tag's `Found += (N +
 *              ContentTagLen)` skip amount is: length of the
 *              tag-specific boilerplate after the matched name (the
 *              rest of the attribute name plus `" content="`) + the 9
 *              chars of `content="` itself (ContentTagLen). If you
 *              add a new tag, work out its own skip length the same
 *              way rather than reusing an existing one.
 *
 *              PersistentPresets is never #defined anywhere in this
 *              tree, so both the "clear presets before loading" block
 *              near the top and the "reset unfilled presets to
 *              generic labels" block at the end are always/never
 *              active respectively, consistently (the #ifndef and
 *              #ifdef are complementary) - not a bug, just dead
 *              alternate-behavior code as it stands.
 *
 *              NOTE: `FValue` is declared but never used in this
 *              function - harmless leftover, not fixed here since
 *              this is a comments-only pass (this class of "declared
 *              but unused" warning wasn't in scope for the earlier
 *              bugfix pass either, which only tackled uninitialized-
 *              read and missing-return warnings).
 *
 *-----------------------------------------------*/
int Search_in_File(const char *fname, WebLoadPresets *thePresets) {
    FILE *fp;
    char temp[FileMaxLineLength];
    char Copy[FileMaxLineLength];
    char *Found;
    int Value;
    char *tokenizer;
    char *String;
    int Count = 0;
    char DrumFile[FileNameMaxLength];
    char LoopFile[FileNameMaxLength];
    char StatusString[40];
    float FValue;

    /* Start at zero song sections.
     */
    NumberSongMarks = 0;

    /* Get passed the file://
     */
    fname += 7;

    if ((fp = fopen(fname, "r")) == NULL) {
        return (-1);
    }

#ifndef PersistentPresets
    // Clear the presets before loading new ones.
    for (Count = 0; Count < MaxPresetButtons; Count++) {
        thePresets->thePreset[Count] = PresetInvalid;
    }
#endif

    DrumFile[0] = 0;
    LoopFile[0] = 0;
    Count = 0;

    printd(LogHTML, "Have file %x %s\n", fp, fname);

    while (fgets(temp, FileMaxLineLength - 1, fp) != NULL && (++Count < 150)) {
        temp[FileMaxLineLength - 1] = 0;

        strncpy(Copy, temp, FileMaxLineLength);

        /* Look for Presets.
         */
        Found = strstr(temp, "Preset");

        if (Found != NULL) {
            Value = Found[6] - '1';
            //          printf("We are in Presets [%d]\n", Value);

            /* skip 8  "preset1 "   */
            Found += (8 + ContentTagLen);

            /* Validate the index before using it.
             */
            if (Value >= 0 && Value < 10) {
                thePresets->thePreset[Value] = AssignPreset(Value + 1, Found);
                //              printf("Preset%d %d\n", Value,
                //              thePresets->thePreset[Value]);
                printd(LogHTML, "Preset%d %d\n", Value,
                       thePresets->thePreset[Value]);
                strncpy(temp, Copy, FileMaxLineLength);
            }
        }

        /* Set the Tempo for this tune.
         */
        Found = strstr(temp, "Tempo");

        if (Found != NULL) {
            Found += 7 + ContentTagLen;
            Value = atoi(Found);

            if (Value > 60 && Value < 160) {
                thePresets->theTempo = Value;
            }

            printd(LogInfo, "Tempo %d\n", Value);
            gMyInfo.Tempo = Value;
            sprintf(StatusString, "Tempo %d", Value);
            UpdateStatus(StatusString);
        }

        /* Set the Tempo for this tune.
         */
        Found = strstr(temp, "LMA_Time");

        if (Found != NULL) {
            Found += 10 + ContentTagLen;
            Value = atoi(Found);
            /* Since we flash we need twice as many counts.
             */
            gMyInfo.TempoMax = (2 * Value);
            printd(LogInfo, "Time %d\n", Value);
        }

        /* Set the current patch to this one.
         */
        Found = strstr(temp, "SetNow");

        if (Found != NULL) {
            Found += (7 + ContentTagLen);
            printd(LogInfo, "SetNow %s\n", Found);
            AssignPreset(0, Found);
            strncpy(temp, Copy, FileMaxLineLength);
        }

        /* Get the Loop file patch..
         */
        Found = strstr(temp, "LoopFile");

        if (Found != NULL) {
            Found += 10 + ContentTagLen;
            String = Found;
            tokenizer = strtok(String, "\""); // break up by spaces
            printd(LogHTML, "LoopFile %s\n", tokenizer);

            //          strcpy(gMyInfo.LoopFileName, BasePathName);
            //          strcat(gMyInfo.LoopFileName, tokenizer);

            strcpy(gMyInfo.LoopFileName, tokenizer);

            printd(LogHTML, "LoopFile name %s\n", gMyInfo.LoopFileName);

            if (strlen(gMyInfo.LoopFileName) > 3) {
                MyOSCLoadFile(gMyInfo.LoopFileName);
                strncpy(temp, Copy, FileMaxLineLength);
//                printd(LogHTML, "LoopFile Sending %d\n", strlen(gMyInfo.LoopFileName));
            }
        }

        /* Get the Drum File patch.
         */
        Found = strstr(temp, "DrumFile");

        if (Found != NULL) {
            Found += 10 + ContentTagLen;
            String = Found;
            tokenizer = strtok(String, "\""); // break up by spaces
            printd(LogHTML, "DrumFile %s\n", tokenizer);
            strcpy(DrumFile, tokenizer);
            strncpy(temp, Copy, FileMaxLineLength);

            if (DrumFile[0] == 62) {
                DrumFile[0] = 0;
            }

            if (!strcmp(DrumFile, "/dev/null")) {
                DrumFile[0] = 0;
            }

            if (DrumFile[0] != 0) {
                LoadMidiLoop(DrumFile);
            }
        }

        /* Set the current patch to this one.
         */
        Found = strstr(temp, "IntroCount");

        if (Found != NULL) {
            Found += 12 + ContentTagLen;
            Value = atoi(Found);
            printd(LogHTML, "IntroCount %d\n", Value);
            strncpy(temp, Copy, FileMaxLineLength);
            gMyInfo.CountInBeats = Value;
            sprintf(StatusString, "Intro Count %d", Value);
            UpdateStatus(StatusString);
        }

        /* Set the current patch to this one.
         */
        Found = strstr(temp, "BeatsPerMeasure");

        if (Found != NULL) {
            Found += 17 + ContentTagLen;
            Value = atoi(Found);
            printd(LogHTML, "BeatsPerMeasure %d\n", Value);
            strncpy(temp, Copy, FileMaxLineLength);
            gMyInfo.BeatsPerMeasure = Value;
            sprintf(StatusString, "Beats  %d", Value);
            UpdateStatus(StatusString);
        }

        /* Set the current patch to this one.
         */
        Found = strstr(temp, "LoopLength");

        if (Found != NULL) {
            Found += 12 + ContentTagLen;
            Value = atoi(Found);
            printd(LogHTML, "LoopLength %d\n", Value);
            strncpy(temp, Copy, FileMaxLineLength);
            gMyInfo.LoopRecBeats = Value;
            sprintf(StatusString, "Loop Len  %d", Value);
            UpdateStatus(StatusString);
        }
    }

    // Close the file if still open.
    if (fp) {
        fclose(fp);
    }

    /*
     * Check to see if we have requested a new file for the drum or the looper.
     */
    if (DrumFile[0]) {
#if 0
        sprintf(Copy, "MusicApps.sh DrumFile %s & ", DrumFile);
        printd(LogHTML, "Calling System with %s\n", Copy);
        system(Copy);
#else
        LoadMidiLoop(DrumFile);
        //        StartMidiLoop (DrumFile);
#endif
    }

#ifdef PersistentPresets
    if (thePresets->thePreset[0] == PresetInvalid) {
        SetPatchTitles(&PresetButtons[0], "Preset 1", 1);
    }
    if (thePresets->thePreset[1] == PresetInvalid) {
        SetPatchTitles(&PresetButtons[1], "Preset 2", 2);
    }
    if (thePresets->thePreset[2] == PresetInvalid) {
        SetPatchTitles(&PresetButtons[2], "Preset 3", 3);
    }
    if (thePresets->thePreset[3] == PresetInvalid) {
        SetPatchTitles(&PresetButtons[3], "Preset 4", 4);
    }
    if (thePresets->thePreset[4] == PresetInvalid) {
        SetPatchTitles(&PresetButtons[4], "Preset 5", 5);
    }
#endif

    return (0);
}

/*----------------------------------------------
 * Function:        AssignPreset
 *
 * Description: Resolve a preset's value string (from a Search_in_File()
 *              meta tag) into a patch index, and apply it. `String`
 *              is either a quoted patch name (`"My Patch Name"` -
 *              looked up via FindString() against the patch name
 *              table) or a bare numeric patch index (parsed with
 *              atoi(), clamped to 0 if out of range). `PresetNum` of
 *              0 means "apply this patch immediately" (the "SetNow"
 *              tag case); PresetNum 1-8 instead just labels/assigns
 *              that numbered preset button via SetPatchTitles(),
 *              without switching to it yet. Returns the resolved
 *              patch index (or an invalid value if String didn't
 *              resolve to anything in range - the caller,
 *              Search_in_File(), stores this into
 *              thePresets->thePreset[] regardless, invalid or not).
 *-----------------------------------------------*/
tPatchIndex AssignPreset(int PresetNum, char *String) {
    int Value;
    char *tokenizer;

    printd(LogHTML, "AssignPreset %d %s\n", PresetNum, String);

    /* If we Start with a quote it's a name lookup.
     */
    if (*String == '"') {
        String++;
        tokenizer = strtok(String, "\""); // break up by spaces
        printd(LogHTML, "Token1 [%s]\n", tokenizer);
        Value = FindString(fsPatchNames, tokenizer);

    }

    else {
        Value = atoi(String);
        printd(LogHTML, "***** Assign Value %s %d\n", String, Value);

        if (Value >= Max_Patches) {
            Value = 0;
        }
    }

    printd(LogHTML, "Preset %d %d %s\n", PresetNum, Value,
           gMyInfo.MyPatchInfo[Value].Name);

    if (Value < 0 || Value >= Max_Patches) {
        printd(LogHTML, "AssignPreset Out1 %d\n", Value);
        return (Value);
    }

    // Special Case
    if (PresetNum == 0) {
        DoPatch(&gMyInfo.MyPatchInfo[Value]);
    }

    // If it's a valid Preset number assign it.
    if (PresetNum > 0 && PresetNum < 9) {
        printd(LogHTML, "*********PresetNum Case %d\n", PresetNum);

        SetPatchTitles(&PresetButtons[PresetNum - 1],
                       gMyInfo.MyPatchInfo[Value].Name, PresetNum);
    }

#if 0

    /* If it's a preset button or a set now.    */
    switch (PresetNum) {
    case 0:
        DoPatch(&gMyInfo.MyPatchInfo[Value]);
        break;

    case 1:
        printd(LogHTML, "*********PresetNum Case 1 ");
        SetPatchTitles(&PresetButtons[0], gMyInfo.MyPatchInfo[Value].Name, 1);
        break;

    case 2:
        SetPatchTitles(&PresetButtons[1], gMyInfo.MyPatchInfo[Value].Name, 2);
        break;

    case 3:
        SetPatchTitles(&PresetButtons[2], gMyInfo.MyPatchInfo[Value].Name, 3);
        break;

    case 4:
        SetPatchTitles(&PresetButtons[3], gMyInfo.MyPatchInfo[Value].Name, 4);
        break;

    case 5:
        SetPatchTitles(&PresetButtons[4], gMyInfo.MyPatchInfo[Value].Name, 5);
        break;

    case 6:
        SetPatchTitles(&PresetButtons[5], gMyInfo.MyPatchInfo[Value].Name, 6);
        break;

    default:
        break;
    }

#endif
    printd(LogHTML, "AssignPreset OutEnd %d\n", Value);
    return (Value);
}

/*----------------------------------------------
 * Function:        SetPatchTitles
 *
 * Description: Update one preset button's displayed label/number
 *              (Value = the preset slot number, Text = the patch
 *              name) - thin wrapper around MyImageButtonSetText2().
 *              Called from AssignPreset() when a numbered preset tag
 *              is resolved, and from Search_in_File()'s
 *              PersistentPresets block (never active - see
 *              Search_in_File()'s doc) to reset unfilled presets to a
 *              generic "Preset N" label.
 *-----------------------------------------------*/
void SetPatchTitles(theImageButtons *MyButton, char *Text, int Value) {
    //    char String[PatchNameSize];
    //    int StringLen;

    printd(LogHTML, "SetPatchTitles %x %s\n", MyButton, Text);
    // StringLen=strlen(Text);
    // sprintf(String, "      %03d      \n%*s",
    //         Value,
    //         (15 + StringLen) / 2,
    //         Text);
    MyImageButtonSetText2(MyButton, Value, Text);
}

/* ishex: is character `x` a valid hex digit (0-9, a-f, A-F)? Helper
 * for DecodeURI()'s %XX escape decoding below. */
int ishex(int x) {
    return (x >= '0' && x <= '9') || (x >= 'a' && x <= 'f') ||
           (x >= 'A' && x <= 'F');
}

/* Get rid of URL funky characters, like %20
 *
 * Standard-ish URL-decode: `+` becomes a space, `%XX` becomes the
 * byte with that hex value (via ishex() + sscanf), everything else
 * copies through unchanged. Writes the decoded result into `dec`
 * (caller-supplied buffer, must be at least as large as `s`) and
 * returns the decoded length, or -1 if a `%` wasn't followed by two
 * valid hex digits. Used by NavigationPolicy() to turn a raw request
 * URI into a usable filesystem path before extension/routing checks.
 */
int DecodeURI(char *s, char *dec) {
    char *o;
    char *end = s + strlen(s);
    int c;

    for (o = dec; s <= end; o++) {
        c = *s++;

        if (c == '+') {
            c = ' ';
        } else if (c == '%' && (!ishex(*s++) || !ishex(*s++) ||
                                !sscanf(s - 2, "%2x", &c))) {
            return -1;
        }

        if (dec) {
            *o = c;
        }
    }

    return o - dec;
}

#if 0
/* Disabled (#if 0) - all four functions below use the WebKit1 API
 * (WebKitWebFrame, WebKitViewportAttributes - neither exists in
 * WebKit2/webkit2gtk, which is what this project actually links
 * against per configure.ac). Leftover from a pre-WebKit2 port, kept
 * as reference for the viewport/zoom-on-load behavior they
 * implemented (auto-fit zoom on load, similar in spirit to the live
 * ScalePage()/PageLoaded() zoom handling above) rather than deleted
 * outright. */
void load_finished_cb(WebKitWebView *web_view, WebKitWebFrame *web_frame,
                      gpointer data) {
    printd(LogHTML, "Finished downloading %s\n",
           webkit_web_view_get_uri(web_view));
    ScalePage();
}

/* testing      */

/* Handle the viewport-attributes-recompute-requested signal to override the device width */
static void viewport_recompute_cb(WebKitWebView* web_view,
                                  WebKitViewportAttributes* attributes, GtkWidget* window) {
    gint override_available_width = 700;

    g_object_get(G_OBJECT(attributes), "width", &override_available_width,
                 NULL);
    g_object_set(G_OBJECT(attributes), "available-width",
                 override_available_width, NULL);

    //  g_object_get (G_OBJECT (attributes), "height", &override_available_width, NULL);
    //  g_object_set (G_OBJECT(attributes), "available-height", override_available_width, NULL);
}

/* Handle the viewport-attributes-changed signal to recompute the initial scale factor */
static void viewport_changed_cb(WebKitWebView* web_view,
                                WebKitViewportAttributes* attributes, gpointer data) {
    gfloat initialScale;
    g_object_get(G_OBJECT(attributes), "initial-scale-factor", &initialScale,
                 NULL);
    webkit_web_view_set_zoom_level(web_view, initialScale);
}

/* Handle the notify::valid signal to initialize the zoom level */
static void viewport_valid_changed_cb(WebKitViewportAttributes* attributes,
                                      GParamSpec* pspec, WebKitWebView* web_view) {
    gboolean is_valid;
    g_object_get(attributes, "valid", &is_valid, NULL);

    if (!is_valid) {
        webkit_web_view_set_zoom_level(web_view, 1.0);
    }
}
#endif
#if 0
/* Disabled (#if 0) - another WebKit1-era leftover: an older
 * DownloadRequestcb/PolicyRequestCD pair that appear to be an earlier
 * take on what NavigationPolicy() does now (deciding how to handle
 * downloads/navigation requests, including a PDF-handling branch not
 * present in the current NavigationPolicy()). Superseded by
 * NavigationPolicy() + the decide-policy signal wired up in
 * InitHTML(); kept here as reference rather than deleted. */
gboolean
DownloadRequestcb(WebKitWebView *web_view,
                  WebKitDownload *download,
                  gpointer user_data) {

    printd(LogHTML, "DownloadRequestcb %s\n", download);

}

gboolean
PolicyRequestCD(WebKitWebView* view, WebKitWebFrame* frame,
                WebKitNetworkRequest* request, const char* mime_type,
                WebKitWebPolicyDecision* decision, gpointer data) {
    //    char* type = (char*)data;

    printd(LogHTML, "PolicyRequestCD %s %s\n", mime_type, data);
#if 0

    if (g_str_equal(type, "pdf")) {
        g_assert_cmpstr(mime_type, ==, "application/pdf");
        g_assert(!webkit_web_view_can_show_mime_type(view, mime_type));
    }

    else
        if (g_str_equal(type, "html")) {
            g_assert_cmpstr(mime_type, ==, "text/html");
            g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
        }

        else
            if (g_str_equal(type, "text")) {
                WebKitNetworkResponse* response = webkit_web_frame_get_network_response(frame);
                SoupMessage* message = webkit_network_response_get_message(response);
                char* disposition;

                g_assert(message);
                soup_message_headers_get_content_disposition(message->response_headers,
                        &disposition, NULL);
                g_object_unref(response);

                g_assert_cmpstr(disposition, ==, "attachment");
                g_free(disposition);

                g_assert_cmpstr(mime_type, ==, "text/plain");
                g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
            }

            else
#endif

                if (g_str_equal(mime_type, "audio/mpeg")) {
                    g_assert(webkit_web_view_can_show_mime_type(view, mime_type));
                    printd(LogHTML, "PolicyRequestCD MP3 Found %s \n", mime_type);
                }

    //   g_free(type);

    return FALSE;
}

#endif
#if 0

if (Amount == ScrollPgDn)
    if ((VIncrement + Value) >= UpperV) {
        gtk_adjustment_set_value(Adjust, 0);
        printd(LogHTML, "ScrollDown Rolling Over\n");
    }

    else {
        gtk_adjustment_set_value(Adjust, VIncrement + Value);
    }

if (Amount == ScrollPgUp)
    if ((Value - VIncrement) < 0) {
        gtk_adjustment_set_value(Adjust, 0);
        printd(LogHTML, "ScrollDown Rolling Over\n");
    }

    else {
        gtk_adjustment_set_value(Adjust, Value - VIncrement);
    }

if (Amount >= 0) {
    gtk_adjustment_set_value(Adjust, Amount);

}

gtk_adjustment_changed(Adjust);
gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(ChartGTKView), Adjust);
#endif
#if 0
// Scroll bar fix to add GtkViewport
GtkWidget *viewport;
printf("Adjustment %d %d\n", gtk_widget_get_margin_left(scrolled_window),
       gtk_widget_get_margin_right(scrolled_window));
viewport =
    gtk_viewport_new(gtk_scrolled_window_get_hadjustment(scrolled_window),
                     gtk_scrolled_window_get_vadjustment(scrolled_window));
gtk_container_set_focus_hadjustment(GTK_CONTAINER(viewport),
                                    gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolled_window)));
gtk_container_set_focus_vadjustment(GTK_CONTAINER(viewport),
                                    gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window)));
//      gtk_container_add (GTK_CONTAINER (scrolled_window), viewport);
//  gtk_scrolled_window_set_max_content_width()
gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(scrolled_window), viewport);

#endif

#if 0
GtkAdjustment *Adjust;
gdouble Value;
Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *) ChartGTKView);
gtk_adjustment_set_page_size(Adjust, 100);
Value = gtk_adjustment_get_value(Adjust);
printf("Value %f\n", Value);
return ((float) Value);
#endif

// xdotool windowactivate 100663307 ; xdotool key Page_Up
// web_view
// ChartGTKView
#if 0
Adjust = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *) ChartGTKView);
gtk_adjustment_set_page_size(Adjust, 100);

UpperV = gtk_adjustment_get_upper(Adjust);
gtk_adjustment_set_page_size(Adjust, 100);
gtk_adjustment_set_page_increment(Adjust, UpperV / 4);
VIncrement = gtk_adjustment_get_page_increment(Adjust);
Value = gtk_adjustment_get_value(Adjust);
printd(LogHTML, "In ScrollDown %f  %f %f\n", UpperV, VIncrement, Value);
printf("In ScrollDown %f  %f %f\n", UpperV, VIncrement, Value);
printf("Page Size %f\n", gtk_adjustment_get_value(Adjust));
#endif

#if 0 // Using NewMethod.
/* If we find an MP3 file then handle it ourselves and tell WebKit
 * not to deal with it.
 */

if (strstr(theURI, ".mp3")) {
    /*
     * Tell web kit not to o anything with it.
     */
    SetPlayerFile((theURI + 7));
    printd(LogInfo, "Call SetPlayer %s \n", theURI);

    webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
    printd(LogHTML, "*** MP3 After systemcall %s\n", SysCallString);
    /*
     * This tells webkit we are dealing with it.
     */
    return (true);
}

#ifdef AllowInternalMP4

if (strstr(theURI, ".mp4") || strstr(theURI, ".webm") || strstr(theURI, ".mpg")) {
    /*
     * Tell web kit not to o anything with it.
     */
    SetPlayerFile((theURI + 7));
    printd(LogInfo, "Call SetPlayer %s \n", theURI);

    webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
    printd(LogHTML, "*** MP4 After systemcall %s\n", SysCallString);
    /*
     * This tells webkit we are dealing with it.
     */
    return (true);
}

#else // AllowInternalMP4

if (strstr(theURI, ".mp4") || strstr(theURI, ".webm") || strstr(theURI, ".mpg")) {
    /*
     * Tell web kit not to o anything with it.
     */
    webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));

    //      sprintf(string, "smplayer \'%s\' &", &theURI[7]);
    sprintf(string, "MusicApps.sh  mp4 \'%s\' ", &theURI[7]);
    system(string);
    printd(LogInfo, "***MP4 else systemcall %s\n", string);

    /*
     * This tells webkit we are dealing with it.
     */
    return (true);
}

#endif // AllowInternalMP4

if (strstr(theURI, ".mid") || strstr(theURI, ".med")) {
    /*
     * Tell web kit not to o anything with it.
     */
    webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));

    //      sprintf(string, "%s \'%s\' &", gMyInfo.Apps[MidiPlayer].Name, &theURI[7]);
    sprintf(string, "MusicApps.sh med \'%s\' ", &theURI[7]);
    //      sprintf(string, "muse \'%s\' &", &theURI[7]);
    //      sprintf(string, "/usr/bin/rosegarden \'%s\' &", &theURI[7]);
    SysRet = system(string);
    printd(LogInfo, "***mid systemcall %s\n", string);

    printf("Type %s returns %d\n", ext, SysRet);
    /*
     * This tells webkit we are dealing with it.
     */
    return (true);
}

printf("About to check for mscz [%s]\n", theURI);

if (strstr(theURI, ".mscz")  || strstr(theURI, ".gp") || strstr(theURI, ".ptb")) {

    printf("MusicApps.sh %s \'%s\' \n", ext + 1, &theURI[7]);

    /*
     * Tell web kit not to o anything with it.
     */
    webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));

    //      sprintf(string, "musescore \'%s\' &", &theURI[7]);
    sprintf(string, "MusicApps.sh %s \'%s\' ", ext + 1, &theURI[7]);
    printf("Muse Score [%s]\n", string);
    //      sprintf(string, "%s \'%s\' &",gMyInfo.Apps[MidiPlayer].Name, &theURI[7]);
    SysRet = system(string);
    printd(LogInfo, "*** mscz systemcall %s\n", string);
    printf("Type %s returns %d\n", ext, SysRet);

    /*
     * This tells webkit we are dealing with it.
     */
    if (!SysRet) {
        return (true);
    }
    else {
        return (false);
    }
}


if (strstr(theURI, ".tg")) {
    /*
     * Tell web kit not to o anything with it.
     */
    webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));

    sprintf(string, "%s \'%s\' &", gMyInfo.Apps[TabPlayer].Name, &theURI[7]);
    system(string);
    printd(LogInfo, "***tg systemcall %s\n", string);

    /*
     * This tells webkit we are dealing with it.
     */
    return (true);
}

if (strstr(theURI, ".pdf")) {
    char *PageIndex;
    int PageNumber;

    /*
     * Tell web kit not to do anything with it.
     */
    PageIndex = strstr(theURI, "#page=");

    if (PageIndex) {
        webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
        *PageIndex = 0;
        PageIndex += 6;
        PageNumber = atoi(PageIndex);
        //          sprintf(string, "/usr/bin/okular \'%s\'' --page=%d &", theURI, PageNumber);
        //          sprintf(string, "okular \'%s\' --page=%d &", &theURI[7], PageNumber);
        sprintf(string, "MusicApps.sh pdf \'%s\' --page=%d ", &theURI[7], PageNumber);
    }

    else {
        return (false);
    }

    system(string);
    printd(LogInfo, "***PDF systemcall %s\n", string);

    /*
     * This tells webkit we are dealing with it.
     */
    return (true);
}

#endif // Using NewMethod.

/* Depending on the PDF call there may be a page number, so
we will handle it.
*/
#if 0

if (strstr(theURI, ".pdf")) {
    printf("PDF 1 [%s]\n", theURI);
    PageIndex = strstr(theURI, "#page=");

    if (PageIndex) {
        *PageIndex = 0;
        PageIndex += 6;
        PageNumber = atoi(PageIndex);
        sprintf(string, "MusicApps.sh %s \'%s\' %d", ext + 1, &theURI[7], PageNumber);
    }

    else {
        sprintf(string, "MusicApps.sh %s \'%s\' ", ext + 1, &theURI[7]);
    }

    printf("PDF 2 [%s]\n", string);

    /* Call the bash app and see if there is a handler for the media-type.
    */
    SysRet = system(string);
    printd(LogInfo, "*** PDF systemcall %d %s\n", SysRet, string);

    /*
     * This tells webkit we are dealing with it.
     */
    if (!SysRet) {
        webkit_policy_decision_ignore(WEBKIT_POLICY_DECISION(decision));
        printd(LogInfo, "NavPol return true\n");
        return (true);
    }

    else {
        printd(LogInfo, "NavPol return false\n");
        return (false);
    }
}

#endif

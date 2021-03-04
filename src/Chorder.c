/*---------------------------------------------
 *	Revision Date:	 2014/04/08
 *
 *	Contains:	<Type in description>
 *
 *	Written by:	Elias Kesh
 *
 *	Date:		<Creation Date>
 *
 *	Copyright © 2012 Elias Kesh.  All rights reserved.
 *
 * Parts of the draw fretboard taken from OpenMoko-ChordMaster
 * Authored by Michael 'Mickey' Lauer <mlauer@vanille-media.de>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 *---------------------------------------------*/

/*
 * Includes
 */

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LiveMusicApp.h"

/*
 * Constants
 */

#define MaxDisplayFrets	16
#define MaxNumStrings	TotalMaxStrings
#define MaxNumFrets		24

#if (GTK_MAJOR_VERSION == 2)
#define XOffset		70
#define YOffset		70
#else
#define XOffset		5
#define YOffset		5
#endif

typedef struct {
    /* Relative position of string number, 1 is the first.
     */
    char String;

    /* Finger to use for chord.
     */
    char Finger;

    /* Interval of the note to the chord.
     */
    char Interval;

    /* Fret relative to the lowest from number. Starting with 0
     */
    char Fret;
} thePreChord;

#define EOfscl		-1
#define SN		-2
//	C   C#   D   D#   E   F   F#   G   G#   A   A#   B   C
//	  1    2   3    4   5   6    7    8   9   10   11  12
// Chords and Scales
// 								1	2	3	4	5	6	7	8	9	10	11	12	13	14
char chMajor[] =
{ 0, SN, 4, SN, 7, SN, 11, EOfscl };
char chMinor[] =
{ 0, SN, 3, SN, 7, SN, 10, EOfscl };
char chDominant[] =
{ 0, SN, 4, SN, 7, SN, 10, EOfscl };
char chMajor6[] =
{ 0, SN, 4, SN, 7, 9, SN, EOfscl };
char chMinor6[] =
{ 0, SN, 3, SN, 7, 9, SN, EOfscl };
char chMajor9[] =
{ 0, SN, 4, SN, 7, SN, 11, SN, 14, EOfscl };
char chMinor9[] =
{ 0, SN, 3, SN, 7, SN, 10, SN, 14, EOfscl };
char chDom9[] =
{ 0, SN, 4, SN, 7, SN, 10, SN, 14, EOfscl };
char ch69[] =
{ 0, SN, 4, SN, 7, SN, 10, SN, 14, EOfscl };

char scMajor[] =
{ 0, 2, 4, 5, 7, 9, 11, EOfscl };
char scMinor[] =
{ 0, 2, 3, 5, 7, 8, 10, EOfscl };
char scHemiPent[] =
{ 0, SN, 4, 5, 7, SN, 11, EOfscl };
char scMajPent[] =
{ 0, 2, 4, SN, 7, 9, SN, EOfscl };
char scMinPent[] =
{ 0, SN, 3, 5, 7, SN, 10, EOfscl };
char scSLoc[] =
{ 0, 1, 3, 4, 6, 8, 10, EOfscl };
char scLydian[] =
{ 0, 2, 4, 6, 7, 9, 11, EOfscl };

char scHarMaj[] =
{ 0, 2, 4, 5, 7, 8, 11, EOfscl };
char scHarMin[] =
{ 0, 2, 3, 5, 7, 8, 11, EOfscl };

char scMelMinUp[] =
{ 0, 2, 3, 5, 7, 9, 11, EOfscl };
char scMelMinDown[] =
{ 0, 2, 3, 5, 7, 8, 10, EOfscl };

char scAugmented[] =
{ 0, 3, 4, SN, 7, 8, 11, EOfscl };
char scAcoustic[] =
{ 0, 2, 4, 6, 7, 9, 10, EOfscl };
char scHungMaj1[] =
{ 0, 3, 4, 6, 7, 9, 10, EOfscl };
char scHungMaj2[] =
{ 0, 3, 4, 6, 7, 9, 11, EOfscl };
char scHungMin1[] =
{ 0, 2, 3, 6, 7, 8, 11, EOfscl };
char scHungMin2[] =
{ 0, 2, 3, 6, 7, 9, 10, EOfscl };
char scEnigAs[] =
{ 0, 1, 4, 7, 8, 10, 11, EOfscl };
char scEnigDe[] =
{ 0, 1, 4, 6, 8, 10, 11, EOfscl };
char scNeapolitanMaj[] =
{ 0, 1, 3, 5, 7, 9, 11, EOfscl };
char scNeapolitanMin[] =
{ 0, 1, 3, 5, 7, 8, 11, EOfscl };
char scRomanianMin[] =
{ 0, 2, 3, 6, 9, 10, 11, EOfscl };

// Maj Scale R  M2  M3  P4  P5  M6  M7
// Offset    0,  2,  4,  5,  7,  9, 11

char scByzantine[] =
{ 0, 1, 4, 5, 7, 8, 11, EOfscl };

char scHiroJoshi[] =
{ 0, 2, 3, SN, 7, 8, SN, EOfscl };

char scWholeNote[] =
{ 0, 2, 4, 6, 8, SN, 10, EOfscl };
char scWholeHalf[] =
{ 0, 2, 3, 5, 6, 8, 9, 11, EOfscl };

// Super Locrian
typedef struct {
    thePreChord *PreChord;
    char Name[20];
} PreChordMenu;

typedef struct {
    char *ChordList;
    char Name[20];
} ChordMenu;

ChordMenu myChordMenu[] = {
    { ( void * ) 1, "Fret Board" },
    { chMajor, "Major Chord" },
    { chMinor, "Minor Chord" },
    { chDominant, "Dominant Chord" },
    { chMajor6, "Major 6" },
    { chMinor6, "Minor 6" },
    { chMajor9, "Major 9" },
    { chMinor9, "Minor 9" },
    { chDom9, "Dom 9" },
    { ch69, "Dom 69" },
    { scMajor, "Major Scale" },
    { scMinor, "Minor Scale" },
    { scHemiPent, "Hemi Pent" },
    { scMajPent, "Maj Pent" },

    { scHarMaj, "Maj Harm" },
    { scHarMin, "Min Harm" },

    { scMelMinUp, "Min Harm Up" },
    { scMelMinDown, "Min Harm Down" },

    { scWholeNote, "Whole Tone" },
    { scWholeHalf, "Whole Half" },

    { scSLoc, "S Loc" },
    { scLydian, "Lydian" },
    { scMinPent, "Min Pent" },

    { scAugmented, "Augmented" },
    { scAcoustic, "Acoustic" },
    { scHungMaj1, "Hungarian Maj1" },
    { scHungMaj2, "Hungarian Maj2" },
    { scHungMin1, "Hungarian Min1" },
    { scHungMin2, "Hungarian Min2" },

    { scEnigAs, "Enigmatic Asending" },
    { scEnigDe, "Enigmatic Descending" },
    { scNeapolitanMaj, "Neopolitan Maj" },
    { scNeapolitanMin, "Neopolitan Min" },
    { scRomanianMin, "Romanian Min" },
    { scByzantine, "Byzantine" },
    { scHiroJoshi, "HiroJoshi" },

    { NULL, "" }
};

thePreChord pchMaj[] = {
    /* String  Finger Interval Fret */
    { 1, 1, 1, 0 },
    { 2, 3, 5, 2 },
    { 3, 4, 1, 2 },
    { 4, 2, 3, 1 },
    { -1, -1, -1, -1 }
};
thePreChord pchMin[] = {
    /* String  Finger Interval Fret */
    { 1, 1, 1, 0 },
    { 2, 3, 5, 2 },
    { 3, 4, 1, 2 },
    { 4, 2, 3, 0 },
    { -1, -1, -1, -1 }
};

thePreChord pchMaj7[] = {
    /* String  Finger Interval Fret */
    { 1, 2, 1, 0 },
    { 2, 3, 3, -1 },
    { 3, 3, 7, 1 },
    { 4, 4, 3, 1 },
    { -1, -1, -1, -1 }
};
thePreChord pchMin7[] = {
    /* String  Finger Interval Fret */
    { 1, 2, 1, 0 },
    { 2, 3, 3, -2 },
    { 3, 3, 7, 0 },
    { 4, 4, 3, 0 },
    { -1, -1, -1, -1 }
};

thePreChord pchMaj7O[] = {
    /* String  Finger Interval Fret */
    { 1, 1, 1, 0 },
    { 4, 3, 7, 1 },
    { 5, 4, 3, 3 },
    { -1, -1, -1, -1 }
};

thePreChord pchMin7O[] = {
    /* String  Finger Interval Fret */
    { 1, 1, 1, 0 },
    { 4, 3, 7, 0 },
    { 5, 4, 3, 2 },
    { -1, -1, -1, -1 }
};

thePreChord pchMaj6[] = {
    /* String  Finger Interval Fret */
    { 1, 1, 1, 0 },
    { 2, 3, 3, 2 },
    { 3, 2, 5, 2 },
    { -1, -1, -1, -1 }
};

PreChordMenu myPreChordMenu[] = {
    { pchMaj, "Maj" },
    { pchMin, "Min" },
    { pchMaj7, "Maj7"},
    { pchMin7, "Min7" },
    { pchMaj7O, "Maj7 Open" },
    { pchMin7O, "Min7 Open"},
    { pchMaj6, "Min6"},
    { NULL, "" }
};


char *FlatNotes[12] = {
    "A ", "Bb", "B ", "C ", "Db", "D ", "Eb", "E ", "F ",
    "Gb ", "G ", "Ab"
};

char *SharpNotes[12] = {
    "A ", "A#", "B ", "C ", "C#", "D ", "D#", "E ", "F ",
    "F# ", "G ", "G#"
};
/*
 * Globals
 */
char **NoteNames;
char NumStrings;
int FretOffset;
int StringOffset;
GtkWidget *MyFretArea;
char CurRootNote;
char *CurScale;
thePreChord *CurPreScale;
int DisplayPosition;
int WindowWidth;
typedef struct {
    char Position;
    char ChordNotes[MaxNumStrings][MaxDisplayFrets];
} theChord;

theChord myChord;
int Fwidth, Fheight;

#if (GTK_MAJOR_VERSION == 2)
GdkPixbuf *Fingerboard;
GdkPixbuf *FingerboardDot;
GdkPixbuf *BouncieBall;
GdkPixbuf *BouncieBallBl;
GdkPixbuf *BouncieBallGr;
GdkPixbuf *BouncieBallRed;
GdkPixbuf *BouncieBallYel;
#else
int Cwidth, Cheight;
cairo_surface_t *Fingerboard;
cairo_surface_t *FingerboardDot;
cairo_surface_t *BouncieBall;
cairo_surface_t *BouncieBallBl;
cairo_surface_t *BouncieBallGr;
cairo_surface_t *BouncieBallRed;
cairo_surface_t *BouncieBallYel;
cairo_t *cr;
cairo_surface_t *CSurface;
#endif

/*
 * Local Prototypes
 */
int StringLayout[MaxNumStrings][MaxNumFrets];
void MarkNote ( char Note, char Value );
void SetScale ( char Root, char *Scale );
//static void draw_strings(GtkWidget *area, GdkGC *gcontext);
//static gboolean draw_fretboard(GtkWidget *area, GdkEventExpose *event);
static gboolean draw_fretboard_background ( GtkWidget *area,
        GdkEventExpose *event );
void SetChord ( char Root, thePreChord *myPreChord );


/*--------------------------------------------
 * Function:		SetChordCallBack
 *
 * Description:		The Scale or Chord call back from button press and
 * 				call the drawing routine.
 *---------------------------------------------*/
static void SetChordCallBack ( GtkWidget *widget, gpointer data ) {
    int ScaleIndex;

//	CurScale = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    ScaleIndex = gtk_combo_box_get_active ( GTK_COMBO_BOX ( widget ) );
    CurPreScale = myPreChordMenu[ScaleIndex].PreChord;
    SetChord ( CurRootNote, CurPreScale );
    draw_fretboard_background ( MyFretArea, NULL );
    gdk_window_invalidate_rect ( gtk_widget_get_window ( widget ), NULL, true );

}

/*--------------------------------------------
 * Function:		SetScaleCallBack
 *
 * Description:		The Scale or Chord call back from button press and
 * 				call the drawing routine.
 *---------------------------------------------*/
static void SetScaleCallBack ( GtkWidget *widget, gpointer data ) {
    int ScaleIndex;

//	CurScale = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    ScaleIndex = gtk_combo_box_get_active ( GTK_COMBO_BOX ( widget ) );
    CurScale = myChordMenu[ScaleIndex].ChordList;
    SetScale ( CurRootNote, CurScale );
    draw_fretboard_background ( MyFretArea, NULL );
    gdk_window_invalidate_rect ( gtk_widget_get_window ( widget ), NULL, true );
}

/*--------------------------------------------
 * Function:		SetRootCallBack
 *
 * Description:		Set root note call back from button press and
 * 				call the drawing routine.
 *---------------------------------------------*/
static void SetRootCallBack ( GtkWidget *widget, gpointer data ) {

    CurRootNote = ( char ) gtk_combo_box_get_active ( GTK_COMBO_BOX ( widget ) );
//    CurRootNote = (char *)data;
    SetScale ( CurRootNote, CurScale );
    draw_fretboard_background ( MyFretArea, NULL );
    gdk_window_invalidate_rect ( gtk_widget_get_window ( widget ), NULL, true );
}

/*--------------------------------------------
 * Function:		GetFretPosition
 *
 * Description:		When the fret position is changed by the user.
 *---------------------------------------------*/
static void GetFretPositionCallBack ( GtkWidget *widget, GtkWidget *entry ) {
//    const gchar *PositionText = gtk_entry_get_text(GTK_ENTRY (entry));
//	sscanf((char *)PositionText, "%d", &DisplayPosition);
    DisplayPosition = gtk_combo_box_get_active ( GTK_COMBO_BOX ( widget ) );
    SetScale ( CurRootNote, CurScale );
    draw_fretboard_background ( MyFretArea, NULL );
    gdk_window_invalidate_rect ( gtk_widget_get_window ( widget ), NULL, true );

//		DisplayPosition = atoi(PositionText);
}
/*--------------------------------------------
 * Function:		GetIntervalNote
 *
 * Description:		Given a root note and an interval get the new note
 *---------------------------------------------*/
char GetIntervalNote ( char Starting, char Interval ) {
    char NewInt;

    NewInt = ( Starting + Interval ) % 12;
    return ( NewInt );
}

/*--------------------------------------------
 * Function:		PrintBoard
 *
 * Description:	Print the board to a terminal -- Test
 *---------------------------------------------*/
void PrintBoard() {
    int FretLoop;
    int StringLoop;

    for ( StringLoop = 0; StringLoop < MaxNumStrings; StringLoop++ ) {
        for ( FretLoop = 0; FretLoop < MaxNumFrets; FretLoop++ ) {
            printd ( LogInfo, "| %s ",
                     NoteNames[StringLayout[StringLoop][FretLoop]] );
        }

        printd ( LogInfo, "\n" );
    }

}

/*--------------------------------------------
 * Function:		SetOpenString
 *
 * Description:	Set the open string tuning.
 * ** TODO Make this dynamic based on Preferences
 *---------------------------------------------*/
void SetOpenString ( void ) {
    int Loop;

#if 0
    StringLayout[8][0] = NValueB;
    StringLayout[7][0] = NValueE;
    StringLayout[6][0] = NValueA;
    StringLayout[5][0] = NValueD;
    StringLayout[4][0] = NValueG;
    StringLayout[3][0] = NValueC;
    StringLayout[2][0] = NValueF;
    StringLayout[1][0] = NValueBb;
    StringLayout[0][0] = NValueEb;
#endif

    NumStrings = gMyInfo.NumberOfStrings;

    for ( Loop = 0; Loop < MaxNumStrings; Loop++ ) {
        StringLayout[Loop][0] = gMyInfo.BaseStringName[Loop];
    }
}

/*--------------------------------------------
 * Function:		SetupFretBoard
 *
 * Description:	Based on the open string tuning fill in all the notes
 * 			for every fret on everystring.
 *---------------------------------------------*/
void SetupFretBoard ( void ) {
    int	 FretLoop;
    int	 StringLoop;

    for ( StringLoop = 0; StringLoop < NumStrings; StringLoop++ ) {
        for ( FretLoop = 1; FretLoop < MaxNumFrets; FretLoop++ ) {
            StringLayout[StringLoop][FretLoop] = GetIntervalNote (
                                                 StringLayout[StringLoop][FretLoop - 1], 1 );
        }
    }
}

/*--------------------------------------------
 * Function:		SetChord
 *
 * Description:	Clear the chord structure and fill in the notes we
 * 		want to display.
 *---------------------------------------------*/
void SetChord ( char Root, thePreChord *myPreChord ) {
    int	 Loop;
//    char	NewNote;
    int String;
    int Fret;
    int Value;
    int MaxFret = 0;
    int MaxString = 0;
    int RootIndex = -1;

    bzero ( ( void * ) &myChord, sizeof ( theChord ) );
    myChord.Position = DisplayPosition;

    /* First let's find the max and min string and frets.
     */
    for ( Loop = 0; myPreChord[Loop].Finger != -1; Loop++ ) {
        if ( myPreChord[Loop].Fret > MaxFret ) {
            MaxFret = myPreChord[Loop].Fret;
        }

        if ( myPreChord[Loop].String > MaxString ) {
            MaxString = myPreChord[Loop].String;
        }

        if ( myPreChord[Loop].Interval == 1 && RootIndex == -1 ) {
            RootIndex = Loop;
        }
    }

    printd ( LogInfo, "String %d Fret %d, RootString %d\n", MaxString, MaxFret,
             RootIndex );

    /* Let's find the starting point of the chord.
     */
    Loop = 1;

    for ( String = 2; String < NumStrings && Loop; String++ ) {
        for ( Fret = 0; Fret < ( MaxDisplayFrets - 3 ) && Loop; Fret++ ) {
            if ( StringLayout[String][Fret + myChord.Position] == Root ) {
                Loop = 0;
            }
        }
    }

    printd ( LogInfo, "Starting String %d Fret %d Root %d\n", String, Fret, Root );

//               myChord.ChordNotes[String][Fret] = Value;
    for ( Loop = 0; myPreChord[Loop].Finger != -1; Loop++ ) {
        printd ( LogInfo, "C_String %d, C_Fret %d\n", myPreChord[Loop].String,
                 myPreChord[Loop].Fret );
        myChord.ChordNotes[MaxNumStrings - ( String + myPreChord[Loop].String )][Fret
                + myPreChord[Loop].Fret] = myPreChord[Loop].Interval;
    }

#if 0

    for ( Loop = 0; Scale[Loop] != EOfscl; Loop++ ) {
        NewNote = Scale[Loop];

        if ( NewNote >= 0 ) {
            NewNote = GetIntervalNote ( Root, NewNote );
            MarkNote ( NewNote, Loop + 1 );
        }
    }

#endif
}

/*--------------------------------------------
 * Function:		SetScale
 *
 * Description:	Clear the chord structure and fill in the notes we
 * 		want to display.
 *---------------------------------------------*/
void SetScale ( char Root, char *Scale ) {
    int Loop;
    char NewNote;

    if ( Scale == (void *)1 ) {
        return;
    }

    bzero ( ( void * ) &myChord, sizeof ( theChord ) );
    myChord.Position = DisplayPosition;

    for ( Loop = 0; Scale[Loop] != EOfscl; Loop++ ) {
        NewNote = Scale[Loop];

        if ( NewNote >= 0 ) {
            NewNote = GetIntervalNote ( Root, NewNote );
            MarkNote ( NewNote, Loop + 1 );
        }
    }
}

/*--------------------------------------------
 * Function:		MarkNote
 *
 * Description:	Add the note we want to display into the structure.
 *---------------------------------------------*/
void MarkNote ( char Note, char Value ) {
    int String;
    int Fret;

    for ( String = 0; String < NumStrings; String++ ) {
        for ( Fret = 0; Fret < MaxDisplayFrets; Fret++ ) {
            if ( StringLayout[String][Fret + myChord.Position + 1] == Note ) {
                myChord.ChordNotes[String][Fret] = Value;
            }
        }
    }
}

/*--------------------------------------------
 * Function:		delete_event
 *
 * Description:
 *---------------------------------------------*/
static gboolean delete_event ( GtkWidget *widget, GdkEvent *event, gpointer data ) {
    /* If you return FALSE in the "delete-event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs. */

    g_print ( "delete event occurred\n" );

    /* Change TRUE to FALSE and the main window will be destroyed with
     * a "delete-event". */

    return TRUE;
}

/* Another callback */
static void destroy ( GtkWidget *widget, gpointer data ) {
    gtk_main_quit();
}

#if (GTK_MAJOR_VERSION == 2)
/*--------------------------------------------
 * Function:		ChorderMain
 *
 * Description:	for GTK 20X
 *---------------------------------------------*/
int ChorderMain ( GtkWidget *MainWindow, GtkWidget *window ) {
    /* GtkWidget is the storage type for widgets */
//   GtkWidget *window;
    GtkWidget *button;
    GtkWidget *event_box;
    GtkWidget *rootBox;
    GtkWidget *rootBox1;
    GtkWidget *chdBox;
    GtkWidget *SclBox;
    GtkWidget *BottomBox;
    GtkWidget *vbox;
    GtkWidget *label1, *label2, *label3, *label4;
    GdkPixbuf *pix;
    GError *err = NULL;
    GtkWidget *area = NULL;
//    GdkGC *gc;
//    GdkPixmap *background;
    GtkWidget *RootFixed;
    GtkWidget *RootCombo;
    GtkWidget *ScaleFixed;
    GtkWidget *ScaleCombo;
    GtkWidget *FretFixed;
    GtkWidget *FretCombo;
    gboolean touchscreen_mode = TRUE;
    GtkWidget *PreChordFixed;
    GtkWidget *PreChordCombo;
    char Loop, Buffer[120];
    int width, height;


    printd ( LogInfo, "FretOffset 0\n" );
    /* This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application. */
//   gtk_init (&argc, &argv);
    /* create a new window */
//   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_get_size ( GTK_WIDGET ( MainWindow ), &width, &height );
    NoteNames = FlatNotes;
    SetOpenString();
    FretOffset = 60;
    FretOffset = ( int ) ( ( float ) ( width - 300 ) / ( float ) NumStrings );
    printd ( LogInfo, "FretOffset %d %d %d \n", FretOffset, width, NumStrings );
    printd ( LogInfo, "FretOffset %d %d %d \n", FretOffset, width, NumStrings );
    StringOffset = 30;
    myChord.Position = 2;
    DisplayPosition = 2;
    printd ( LogInfo, "FretOffset 1\n" );
    SetupFretBoard();
    printd ( LogInfo, "FretOffset 2\n" );
    CurRootNote = NValueC;
    CurScale = chMajor;
    WindowWidth = width;
    SetScale ( CurRootNote, CurScale );
    printd ( LogInfo, "FretOffset 3\n" );

    printd ( LogInfo, "Chorder %x\n", window );

    /* Here is the Root Note pulldown.
     * ----------------------------------
     */
    RootFixed = gtk_fixed_new();
    RootCombo = gtk_combo_box_text_new();

    for ( Loop = 0; Loop < 12; Loop++ ) {
        sprintf ( Buffer, "%2s Root", NoteNames[Loop] );
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( RootCombo ), Buffer );
    }

    label1 = gtk_label_new ( "Root Select" );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( RootCombo ), CurRootNote );
    gtk_fixed_put ( GTK_FIXED ( RootFixed ), label1, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( RootFixed ), RootCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( RootCombo ), "changed",
                       G_CALLBACK ( SetRootCallBack ), ( gpointer ) label1 );
//	gtk_settings_set_property_value(RootCombo, "gtk-touchscreen-mode", &touchscreen_mode);
    gtk_widget_set_size_request ( RootCombo, 130, 60 );

    /* Here is the Scale pulldown.
     * ----------------------------------
     */
    ScaleFixed = gtk_fixed_new();
    ScaleCombo = gtk_combo_box_text_new();

    for ( Loop = 0; myChordMenu[Loop].ChordList; Loop++ ) {
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( ScaleCombo ),
                                         myChordMenu[Loop].Name );
    }

    label2 = gtk_label_new ( "Chord Select " );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( ScaleCombo ), 0 );
    gtk_fixed_put ( GTK_FIXED ( ScaleFixed ), label2, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( ScaleFixed ), ScaleCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( ScaleCombo ), "changed",
                       G_CALLBACK ( SetScaleCallBack ), ( gpointer ) label2 );
    gtk_widget_set_size_request ( ScaleCombo, 130, 60 );

    /* Here is the Fret Position pulldown.
     * ----------------------------------
     */
    FretFixed = gtk_fixed_new();
    FretCombo = gtk_combo_box_text_new();

    for ( Loop = 0; Loop < 10; Loop++ ) {
        sprintf ( Buffer, "%02d Pos", Loop );
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( FretCombo ), Buffer );
    }

    label3 = gtk_label_new ( "Fret Position " );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( FretCombo ), DisplayPosition );
    gtk_fixed_put ( GTK_FIXED ( FretFixed ), label3, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( FretFixed ), FretCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( FretCombo ), "changed",
                       G_CALLBACK ( GetFretPositionCallBack ), ( gpointer ) label3 );
    gtk_widget_set_size_request ( FretCombo, 130, 60 );

    /* Here is the Predefined Chord pulldown.
     * ----------------------------------
     */
    PreChordFixed = gtk_fixed_new();
    PreChordCombo = gtk_combo_box_text_new();

    for ( Loop = 0; myPreChordMenu[Loop].PreChord; Loop++ ) {
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( PreChordCombo ),
                                         myPreChordMenu[Loop].Name );
    }

    label4 = gtk_label_new ( "PreChord Select" );
    gtk_fixed_put ( GTK_FIXED ( PreChordFixed ), label4, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( PreChordFixed ), PreChordCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( PreChordCombo ), "changed",
                       G_CALLBACK ( SetChordCallBack ), ( gpointer ) label4 );
    gtk_widget_set_size_request ( PreChordCombo, 130, 60 );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( PreChordFixed ), 2 );

    /* When the window is given the "delete-event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect ( window, "delete-event", G_CALLBACK ( delete_event ), NULL );

    /* Here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete-event" callback. */
    g_signal_connect ( window, "destroy", G_CALLBACK ( destroy ), NULL );

    /* Sets the border width of the window. */
    gtk_container_set_border_width ( GTK_CONTAINER ( window ), 10 );
#if 0
    /* Creates a new button with the label "Hello World". */
    button = gtk_button_new_with_label ( "Quit" );
    /* When the button receives the "clicked" signal, it will call the
     * function hello() passing it NULL as its argument.  The hello()
     * function is defined above. */
    g_signal_connect ( button, "clicked", G_CALLBACK ( hello ), NULL );
    /* This will cause the window to be destroyed by calling
     * gtk_widget_destroy(window) when "clicked".  Again, the destroy
     * signal could come from here, or the window manager. */
    g_signal_connect_swapped ( button, "clicked", G_CALLBACK ( gtk_widget_destroy ),
                               window );

#endif
    event_box = gtk_event_box_new();
    rootBox = gtk_hbox_new ( TRUE, 5 );
//   rootBox1 = gtk_hbox_new(TRUE, 5);
    chdBox = gtk_hbox_new ( TRUE, 5 );
    SclBox = gtk_hbox_new ( TRUE, 5 );
    vbox = gtk_vbox_new ( FALSE, 10 );
    BottomBox = gtk_hbox_new ( FALSE, 10 );

//	Fingerboard = gdk_pixbuf_new_from_file(GetResourceDir("./wood.png",FileLocConfig), &err);
    Fingerboard = gdk_pixbuf_new_from_file ( GetResourceDir ( "RedWood.png", FileLocConfig ),
                  &err );

    FingerboardDot = gdk_pixbuf_new_from_file (
                     GetResourceDir ( "FretboardDot.png", FileLocConfig ), &err );
    BouncieBall = gdk_pixbuf_new_from_file ( GetResourceDir ( "ball.png", FileLocConfig ), &err );
    BouncieBallBl = gdk_pixbuf_new_from_file ( GetResourceDir ( "blueball.png", FileLocConfig ),
                    &err );
    BouncieBallGr = gdk_pixbuf_new_from_file ( GetResourceDir ( "greenball.png", FileLocConfig ),
                    &err );
    BouncieBallRed = gdk_pixbuf_new_from_file ( GetResourceDir ( "redball.png", FileLocConfig ),
                     &err );
    BouncieBallYel = gdk_pixbuf_new_from_file ( GetResourceDir ( "yelball.png", FileLocConfig ),
                     &err );

    MyFretArea = gtk_image_new();
    gtk_widget_set_size_request ( MyFretArea,
                                  FretOffset * ( MaxDisplayFrets + 1 ) + 30,
                                  StringOffset * NumStrings + 30 );

//    gtk_box_set_homogeneous(vbox, FALSE);
    gtk_box_pack_start ( GTK_BOX ( vbox ), MyFretArea, TRUE, TRUE, 5 );
    gtk_box_pack_start ( GTK_BOX ( vbox ), rootBox, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), RootFixed, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), ScaleFixed, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), FretFixed, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), PreChordFixed, FALSE, FALSE, 2 );

    gtk_box_pack_start ( GTK_BOX ( vbox ), BottomBox, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( BottomBox ), button, FALSE, FALSE, 2 );

    /* This packs the button into the window (a gtk container). */
    gtk_container_add ( GTK_CONTAINER ( window ), vbox );

    g_signal_connect ( G_OBJECT ( MyFretArea ), "expose_event",
                       G_CALLBACK ( draw_fretboard_background ), NULL );

#if 0
    /* and the window */
    gtk_window_set_title ( GTK_WINDOW ( window ), "Elias Chorder" );
    gtk_window_set_default_size ( GTK_WINDOW ( window ), WindowWidth, 450 );
    gtk_window_set_policy ( GTK_WINDOW ( window ), TRUE, TRUE, FALSE );
#endif

    gtk_widget_show ( MyFretArea );
    gtk_widget_show_all ( window );
    /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
//   gtk_main ();
    return 0;
}

/*--------------------------------------------
 * Function:		draw_fretboard_background
 *
 * Description:	The drawing routine.
 *---------------------------------------------*/
gboolean draw_fretboard_background ( GtkWidget *widget, GdkEventExpose *event ) {
    GError* error = NULL;
    int Loop;
    GdkColor labeloffcolor =
    { 0, 0x00 << 8, 0x00 << 8, 0x00 << 8 };
//	GdkColor labeloffcolor = { 0, 0xff <<8, 0xff <<8, 0xff <<8 };
    GdkColor labeloncolor =
    { 0, 0xf0 << 8, 0xa0 << 8, 0xa0 << 8 };
    GdkColor stringcolor1 =
    { 0, 0xff << 8, 0xfb << 8, 0xa7 << 8 };
    GdkColor stringcolor2 =
    { 0, 0xe6 << 8, 0xb7 << 8, 0x2d << 8 };
    GdkColor stringcolor3 =
    { 0, 0x90 << 8, 0x41 << 8, 0x00 << 8 };
    GdkColor fretcolor1 =
    { 0, 0xff << 8, 0xfb << 8, 0xf7 << 8 };
    GdkColor fretcolor2 =
    { 0, 0xc6 << 8, 0xb7 << 8, 0xcd << 8 };
    GdkColor fretcolor3 =
    { 0, 0x90 << 8, 0x81 << 8, 0x80 << 8 };
    char StrBuf[10];
    char FretLoop;
    char StringLoop;
    GdkPixbuf *theBall;

    /* Allocate a new drawing surface.
     */
    GdkGC* gc = gdk_gc_new ( widget->window );

    /* How are we going to draw the frets.
     */
    gdk_gc_set_line_attributes ( gc, 2, GDK_LINE_SOLID, GDK_CAP_ROUND,
                                 GDK_JOIN_MITER );

    // Draw the background wood.
    gdk_draw_pixbuf ( widget->window, gc, Fingerboard, 0, 0, XOffset, YOffset,
                      FretOffset * ( MaxDisplayFrets ) + 60,
                      StringOffset * NumStrings + 30,
                      //                     0, 0, 0, 0, WindowWidth - 100, StringOffset * NumStrings + 30,
                      GDK_RGB_DITHER_MAX, 0, 0 );

    /* Create a context for drawing the fonts.
     */
    PangoContext* context = gtk_widget_get_pango_context ( widget );
    PangoLayout* layout = pango_layout_new ( context );
    // frets

    /* Draw three layers of frets.
     */
    for ( Loop = 0; Loop <= MaxDisplayFrets; ++Loop ) {
        gdk_gc_set_rgb_fg_color ( gc, &fretcolor1 );
        gdk_draw_line ( widget->window, gc, XOffset + 30 + Loop * FretOffset,
                        YOffset + 20, XOffset + 30 + Loop * FretOffset, YOffset + 262 );
        gdk_gc_set_rgb_fg_color ( gc, &fretcolor2 );
        gdk_draw_line ( widget->window, gc, XOffset + 32 + Loop * FretOffset,
                        YOffset + 20, XOffset + 32 + Loop * FretOffset, YOffset + 262 );
        gdk_gc_set_rgb_fg_color ( gc, &fretcolor3 );
        gdk_draw_line ( widget->window, gc, XOffset + 34 + Loop * FretOffset,
                        YOffset + 20, XOffset + 34 + Loop * FretOffset, YOffset + 262 );
        gdk_gc_set_rgb_fg_color ( gc, &labeloncolor );
        sprintf ( StrBuf, "%d", Loop + myChord.Position );
        pango_layout_set_text ( layout, StrBuf, 2 );
//		gdk_gc_set_line_attributes(gc, 2, GDK_LINE_ON_OFF_DASH, GDK_CAP_ROUND, GDK_JOIN_MITER );
        gdk_draw_layout ( widget->window, gc, XOffset + 30 + Loop * FretOffset,
                          YOffset + 270, layout );

        /* Draw the fret markers.
         */
        if ( Loop != MaxDisplayFrets )
            switch ( Loop + myChord.Position ) {
            case 2:
            case 4:
            case 6:
            case 8:
            case 14:
            case 16:
            case 18:
            case 20:
                gdk_draw_pixbuf ( widget->window, gc, FingerboardDot, 0, 0,
                                  XOffset + 15 + FretOffset / 2 + Loop * FretOffset,
                                  StringOffset * NumStrings / 2 - 15 + YOffset, -1, -1,
                                  GDK_RGB_DITHER_MAX, 0, 0 );

                break;

            case 11:
                gdk_draw_pixbuf ( widget->window, gc, FingerboardDot, 0, 0,
                                  XOffset + 15 + FretOffset / 2 + Loop * FretOffset,
                                  StringOffset * ( 3 * NumStrings / 4 ) - 15 + YOffset, -1,
                                  -1, GDK_RGB_DITHER_MAX, 0, 0 );
                gdk_draw_pixbuf ( widget->window, gc, FingerboardDot, 0, 0,
                                  XOffset + 15 + FretOffset / 2 + Loop * FretOffset,
                                  StringOffset * NumStrings / 4 + 15 + YOffset, -1, -1,
                                  GDK_RGB_DITHER_MAX, 0, 0 );
                break;

            default:
                break;
            }
    }

    // Draw the strings
    for ( Loop = 0; Loop < NumStrings; ++Loop ) {

        if ( StringLayout[Loop][0] == 'x' || StringLayout[Loop][0] == 'X' ) {
            gdk_gc_set_rgb_fg_color ( gc, &labeloffcolor );
            pango_layout_set_text ( layout, "x", 1 );
            gdk_gc_set_line_attributes ( gc, 2, GDK_LINE_ON_OFF_DASH,
                                         GDK_CAP_ROUND, GDK_JOIN_MITER );

        }
        else {
            gdk_gc_set_rgb_fg_color ( gc, &labeloncolor );
            pango_layout_set_text ( layout, NoteNames[StringLayout[Loop][0]], 2 );
            gdk_gc_set_line_attributes ( gc, 2, GDK_LINE_SOLID, GDK_CAP_ROUND,
                                         GDK_JOIN_MITER );
        }

        //FIXME take font size into account
        gdk_draw_layout ( widget->window, gc, XOffset + 5,
                          YOffset + 12 + 30 * Loop, layout );
        gdk_draw_layout ( widget->window, gc,
                          XOffset + 45 + FretOffset * MaxDisplayFrets,
                          YOffset + 12 + 30 * Loop, layout );
        gdk_draw_line ( widget->window, gc, XOffset + 30,
                        YOffset + 20 + Loop * StringOffset,
                        XOffset + 30 + FretOffset * MaxDisplayFrets,
                        YOffset + 20 + Loop * StringOffset );

        gdk_gc_set_rgb_fg_color ( gc, &stringcolor1 );
        gdk_draw_line ( widget->window, gc, XOffset + 30,
                        YOffset + 20 + Loop * StringOffset,
                        XOffset + 30 + FretOffset * MaxDisplayFrets,
                        YOffset + 20 + Loop * StringOffset );
        gdk_gc_set_rgb_fg_color ( gc, &stringcolor2 );
        gdk_draw_line ( widget->window, gc, XOffset + 30,
                        YOffset + 22 + Loop * StringOffset,
                        XOffset + 30 + FretOffset * MaxDisplayFrets,
                        YOffset + 22 + Loop * StringOffset );
        gdk_gc_set_rgb_fg_color ( gc, &stringcolor3 );
        gdk_draw_line ( widget->window, gc, XOffset + 30,
                        YOffset + 24 + Loop * StringOffset,
                        XOffset + 30 + FretOffset * MaxDisplayFrets,
                        YOffset + 24 + Loop * StringOffset );
    }

    /* OK here is the real stuff, drawing the dots.
     */
    for ( StringLoop = 0; StringLoop < NumStrings; StringLoop++ ) {
        for ( FretLoop = 0; FretLoop < MaxDisplayFrets; FretLoop++ ) {
            if ( myChord.ChordNotes[StringLoop][FretLoop] ) {

                /* Change the color of the dot based on the
                 * scale interval.
                 */
                switch ( myChord.ChordNotes[StringLoop][FretLoop] ) {
                case 1:
                    theBall = BouncieBallRed;
                    break;

                case 3:
                    theBall = BouncieBallYel;
                    break;

                case 5:
                    theBall = BouncieBallGr;
                    break;

                case 6:
                case 2:
                    theBall = BouncieBallBl;
                    break;

                default:
                    theBall = BouncieBall;
                    break;
                }

                /* OK, now acutally draw the dot.
                 */
                gdk_draw_pixbuf ( widget->window, gc, theBall, 0, 0,
                                  FretOffset / 2 + 15 + FretOffset * ( FretLoop ) + XOffset,
                                  YOffset + StringOffset * StringLoop, -1, -1,
                                  GDK_RGB_DITHER_MAX, 0, 0 );
                gdk_gc_set_rgb_fg_color ( gc, &labeloffcolor );
                sprintf ( StrBuf, "%d", myChord.ChordNotes[StringLoop][FretLoop] );
                pango_layout_set_text ( layout, StrBuf, 2 );
                gdk_draw_layout ( widget->window, gc,
                                  FretOffset / 2 + 30 + FretOffset * ( FretLoop ) + XOffset,
                                  YOffset + 15 + StringOffset * StringLoop, layout );
            }
        }
    }

    return TRUE;
}
#else
/*--------------------------------------------
 * Function:		ChorderMain
 *
 * Description: for GTK 3.X
 *---------------------------------------------*/
int ChorderMain ( GtkWidget *MainWindow, GtkWidget *window ) {
    GtkWidget *button;
    GtkWidget *rootBox;
    GtkWidget *rootBox1;
    GtkWidget *chdBox;
    GtkWidget *SclBox;
    GtkWidget *BottomBox;
    GtkWidget *vbox;
    GtkWidget *label1, *label2, *label3, *label4;
    GdkPixbuf *pix;
    GError *err = NULL;
    GtkWidget *area = NULL;
//    GdkGC *gc;
//    GdkPixmap *background;
    GtkWidget *RootFixed;
    GtkWidget *RootCombo;
    GtkWidget *ScaleFixed;
    GtkWidget *ScaleCombo;
    GtkWidget *FretFixed;
    GtkWidget *FretCombo;
    gboolean touchscreen_mode = TRUE;
    GtkWidget *PreChordFixed;
    GtkWidget *PreChordCombo;
    int	 Loop;
    char	Buffer[120];
    gfloat screen_width;
    gfloat screen_height;
    gfloat image_width;
    gfloat image_height;
    gfloat x_scaling;
    gfloat y_scaling;

    printd ( LogInfo, "FretOffset 0\n" );

    gtk_frame_set_shadow_type ( GTK_FRAME ( window ), GTK_SHADOW_IN );
    MyFretArea = gtk_drawing_area_new ();
    CSurface = cairo_image_surface_create_from_png ( GetResourceDir ( "RedWood.png", FileLocConfig ) );
    printd ( LogInfo, "FretOffset 1\n" );

    /* Scale the loaded image to occupy the entire screen  */
    image_width = cairo_image_surface_get_width ( CSurface );
    image_height = cairo_image_surface_get_height ( CSurface );
    gtk_window_get_size ( GTK_WIDGET ( MainWindow ), &Fwidth, &Fheight );
//  gtk_widget_get_size_request((window), &Fwidth, &Fheight);

    printd ( LogInfo, "FretOffset 2 %d %d\n", Fwidth, Fheight );
    Fwidth  = 1000;
    Fheight = 450;
    gtk_widget_set_size_request ( MyFretArea, Fwidth, Fheight );

    NoteNames = FlatNotes;
    SetOpenString();

//  FretOffset = width/MaxDisplayFrets;
    if ( !NumStrings ) {
        return ( 0 );
    }

    FretOffset = Fwidth / MaxDisplayFrets;
    StringOffset = ( Fheight ) / NumStrings;

    printd ( LogInfo, "FretOffset %d %d %d %d %d\n", FretOffset, Fwidth, Fheight, NumStrings, StringOffset );
    myChord.Position = 0;
    DisplayPosition = 0;

    SetupFretBoard();
    CurRootNote = NValueC;
    CurScale = chMajor;
    WindowWidth = Fwidth;
    SetScale ( CurRootNote, CurScale );

    printd ( LogInfo, "Chorder %x\n", window );

    /* Here is the Root Note pulldown.
     * ----------------------------------
     */
    RootFixed = gtk_fixed_new();
    RootCombo = gtk_combo_box_text_new();

    for ( Loop = 0; Loop < 12; Loop++ ) {
        sprintf ( Buffer, "%2s Root", NoteNames[Loop] );
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( RootCombo ), Buffer );
    }

    label1 = gtk_label_new ( "Root Select" );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( RootCombo ), CurRootNote );
    gtk_fixed_put ( GTK_FIXED ( RootFixed ), label1, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( RootFixed ), RootCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( RootCombo ), "changed",
                       G_CALLBACK ( SetRootCallBack ), ( gpointer ) label1 );
    gtk_widget_set_size_request ( RootCombo, 130, 60 );

    /* Here is the Scale pulldown.
     * ----------------------------------
     */
    ScaleFixed = gtk_fixed_new();
    ScaleCombo = gtk_combo_box_text_new();

    for ( Loop = 0; myChordMenu[Loop].ChordList; Loop++ ) {
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( ScaleCombo ),
                                         myChordMenu[Loop].Name );
    }

    label2 = gtk_label_new ( "Chord Select " );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( ScaleCombo ), 1 );
    gtk_fixed_put ( GTK_FIXED ( ScaleFixed ), label2, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( ScaleFixed ), ScaleCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( ScaleCombo ), "changed",
                       G_CALLBACK ( SetScaleCallBack ), ( gpointer ) label2 );
    gtk_widget_set_size_request ( ScaleCombo, 130, 60 );

    /* Here is the Fret Position pulldown.
     * ----------------------------------
     */
    FretFixed = gtk_fixed_new();
    FretCombo = gtk_combo_box_text_new();

    for ( Loop = 0; Loop < 9; Loop++ ) {
        sprintf ( Buffer, "%02d Pos", Loop );
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( FretCombo ), Buffer );
    }

    label3 = gtk_label_new ( "Fret Position " );
    gtk_combo_box_set_active ( GTK_COMBO_BOX ( FretCombo ), DisplayPosition );
    gtk_fixed_put ( GTK_FIXED ( FretFixed ), label3, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( FretFixed ), FretCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( FretCombo ), "changed",
                       G_CALLBACK ( GetFretPositionCallBack ), ( gpointer ) label3 );
    gtk_widget_set_size_request ( FretCombo, 130, 60 );

    /* Here is the Predefined Chord pulldown.
     * ----------------------------------
     */
    PreChordFixed = gtk_fixed_new();
    PreChordCombo = gtk_combo_box_text_new();
    gsize *bytesW;
    gchar *UTFString;
    GError *error = NULL;

//    for (Loop = 0; myPreChordMenu[Loop].PreChord; Loop++) {
    for ( Loop = 0; myPreChordMenu[Loop].Name[0]; Loop++ ) {

        UTFString = g_convert ( myPreChordMenu[Loop].Name, strlen ( myPreChordMenu[Loop].Name ), "UTF-8", "ISO-8859-1",
                                NULL, NULL, &error );

        // UTFString = g_filename_to_utf8(myPreChordMenu[Loop].Name,-1, NULL, bytesW, NULL );
        gtk_combo_box_text_append_text ( GTK_COMBO_BOX ( PreChordCombo ), UTFString );
    }

    label4 = gtk_label_new ( "PreChord Select" );
    gtk_widget_set_size_request ( PreChordCombo, 130, 60 );
    gtk_fixed_put ( GTK_FIXED ( PreChordFixed ), label4, 0, 0 );
    gtk_fixed_put ( GTK_FIXED ( PreChordFixed ), PreChordCombo, 0, 25 );
    g_signal_connect ( G_OBJECT ( PreChordCombo ), "changed",
                       G_CALLBACK ( SetChordCallBack ), ( gpointer ) label4 );

    /* When the window is given the "delete-event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event () function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function. */
    g_signal_connect ( window, "delete-event", G_CALLBACK ( delete_event ), NULL );

    /* Here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete-event" callback. */
    g_signal_connect ( window, "destroy", G_CALLBACK ( destroy ), NULL );

    /* Sets the border width of the window. */
    gtk_container_set_border_width ( GTK_CONTAINER ( window ), 10 );

#if 0
    /* Creates a new button with the label "Hello World". */
    button = gtk_button_new_with_label ( "Quit" );
    /* When the button receives the "clicked" signal, it will call the
     * function hello() passing it NULL as its argument.  The hello()
     * function is defined above. */
    g_signal_connect ( button, "clicked", G_CALLBACK ( hello ), NULL );
    /* This will cause the window to be destroyed by calling
     * gtk_widget_destroy(window) when "clicked".  Again, the destroy
     * signal could come from here, or the window manager. */
    g_signal_connect_swapped ( button, "clicked", G_CALLBACK ( gtk_widget_destroy ),
                               window );
#endif

    rootBox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 5 );
    chdBox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 5 );
    SclBox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 5 );
    vbox = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 10 );
    BottomBox = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 10 );
    Fingerboard = gtk_drawing_area_new ();
// ejk	gtk_container_add (GTK_CONTAINER (MainWindow), Fingerboard);
//    gtk_window_get_size(MainWindow, &Cwidth, &Cheight);

    FingerboardDot = cairo_image_surface_create_from_png (
                     GetResourceDir ( "FretboardDot.png", FileLocConfig ) );
    BouncieBall = cairo_image_surface_create_from_png ( GetResourceDir ( "ball.png", FileLocConfig ) );
    BouncieBallBl = cairo_image_surface_create_from_png ( GetResourceDir ( "blueball.png", FileLocConfig ) );
    BouncieBallGr = cairo_image_surface_create_from_png ( GetResourceDir ( "greenball.png", FileLocConfig ) );
    BouncieBallRed = cairo_image_surface_create_from_png ( GetResourceDir ( "redball.png", FileLocConfig ) );
    BouncieBallYel = cairo_image_surface_create_from_png ( GetResourceDir ( "yelball.png", FileLocConfig ) );

//   gtk_widget_set_size_request(MyFretArea,
//                                FretOffset * (MaxDisplayFrets + 1) + 30,
//                                StringOffset * NumStrings + 30);

//    gtk_box_set_homogeneous(vbox, FALSE);
    gtk_box_pack_start ( GTK_BOX ( vbox ), MyFretArea, TRUE, TRUE, 5 );
    gtk_box_pack_start ( GTK_BOX ( vbox ), rootBox, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), RootFixed, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), ScaleFixed, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), FretFixed, FALSE, FALSE, 2 );
    gtk_box_pack_start ( GTK_BOX ( rootBox ), PreChordFixed, FALSE, FALSE, 2 );

    gtk_box_pack_start ( GTK_BOX ( vbox ), BottomBox, FALSE, FALSE, 2 );
//	gtk_box_pack_start(GTK_BOX(BottomBox), button, FALSE, FALSE, 2);

    /* This packs the button into the window (a gtk container). */
    gtk_container_add ( GTK_CONTAINER ( window ), vbox );

    g_signal_connect ( G_OBJECT ( MyFretArea ), "draw",
                       G_CALLBACK ( draw_fretboard_background ), NULL );

#if 0
    /* and the window */
    gtk_window_set_title ( GTK_WINDOW ( window ), "Elias Chorder" );
    gtk_window_set_default_size ( GTK_WINDOW ( window ), WindowWidth, 450 );
    gtk_window_set_policy ( GTK_WINDOW ( window ), TRUE, TRUE, FALSE );
#endif

    gtk_widget_show ( MyFretArea );
    gtk_widget_show_all ( window );
    return 0;
}

#define GDK_DISABLE_DEPRECATION_WARNINGS
/*--------------------------------------------
 * Function:		draw_fretboard_background
 *
 * Description:	The drawing routine.
 *---------------------------------------------*/
gboolean draw_fretboard_background ( GtkWidget *widget, GdkEventExpose *event ) {
//	GError* error = NULL;
    int Loop;

    char StrBuf[10];
    int FretLoop;
    int StringLoop;
    GdkPixbuf *theBall;

//	printf("draw_fretboard_background \n");
    /*
     * NEW
     */
#if 1
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
//    cr = gdk_cairo_create ( gtk_widget_get_window ( widget ) );
G_GNUC_END_IGNORE_DEPRECATIONS
#else
    cr = gdk_window_begin_draw_frame(
	gdk_drawing_context_get_cairo_context(
		gtk_widget_get_window ( widget )), NULL );
#endif
    
    cairo_set_source_surface ( cr, CSurface, 0, 0 );
    cairo_paint ( cr );

    /* Create a context for drawing the fonts.
     */
    PangoContext* context = gtk_widget_get_pango_context ( widget );
    PangoLayout* layout = pango_layout_new ( context );
    cairo_set_operator ( cr, CAIRO_OPERATOR_ATOP );

//    CAIRO_OPERATOR_ATOP CAIRO_OPERATOR_SOURCE
    /* Draw three layers of frets.
     */
    for ( Loop = 0; Loop <= MaxDisplayFrets; ++Loop ) {
        cairo_set_line_width ( cr, 1 );
        cairo_set_source_rgb ( cr, 1, .9, .9 );
        cairo_move_to ( cr, XOffset + 30 + Loop * FretOffset, YOffset + 20 );
        cairo_line_to ( cr, XOffset + 30 + Loop * FretOffset, YOffset + Fheight );
        cairo_stroke ( cr );

        cairo_set_line_width ( cr, .5 );
        cairo_set_source_rgb ( cr, .8, .7, .7 );
        cairo_move_to ( cr, XOffset + 32 + Loop * FretOffset, YOffset + 20 );
        cairo_line_to ( cr, XOffset + 32 + Loop * FretOffset, YOffset + Fheight );
        cairo_stroke ( cr );

        cairo_set_line_width ( cr, .2 );
        cairo_set_source_rgb ( cr, .45, .43, .5 );
        cairo_move_to ( cr, XOffset + 34 + Loop * FretOffset, YOffset + 20 );
        cairo_line_to ( cr, XOffset + 34 + Loop * FretOffset, YOffset + Fheight );
        cairo_stroke ( cr );

        cairo_set_source_rgb ( cr, .95, .75, .75 );
        sprintf ( StrBuf, "%d", Loop + myChord.Position );
        cairo_move_to ( cr, XOffset + 30 + Loop * FretOffset, YOffset + Fheight + 20 );
        cairo_set_font_size ( cr, 12 );
        cairo_show_text ( cr, StrBuf );
        cairo_stroke ( cr );

        /* Draw the fret markers.
         */
        if ( Loop != MaxDisplayFrets )
            switch ( Loop + myChord.Position ) {
            case 2:
            case 4:
            case 6:
            case 8:
            case 14:
            case 16:
            case 18:
            case 20:
                cairo_set_source_surface ( cr, FingerboardDot,
                                           XOffset + 15 + FretOffset / 2 + Loop * FretOffset,
                                           ( ( StringOffset * NumStrings ) / 2 ) - 15 + YOffset );
                cairo_paint ( cr );

                break;

            case 11:
                cairo_set_source_surface ( cr, FingerboardDot,
                                           XOffset + 15 + FretOffset / 2 + Loop * FretOffset,
                                           ( ( StringOffset * NumStrings ) / 2 ) - 45 + YOffset );
                cairo_paint ( cr );

                cairo_set_source_surface ( cr, FingerboardDot,
                                           XOffset + 15 + FretOffset / 2 + Loop * FretOffset,
                                           ( ( StringOffset * NumStrings ) / 2 ) + 30 + YOffset );
                cairo_paint ( cr );
                break;

            default:
                break;
            }

    }

    // Draw the strings
    for ( Loop = 0; Loop < NumStrings; ++Loop ) {
        cairo_move_to ( cr, XOffset + 5, YOffset + 25 + Loop * StringOffset );
        cairo_set_font_size ( cr, 12 );

        if ( StringLayout[Loop][0] == 'x' || StringLayout[Loop][0] == 'X' ) {
            cairo_set_source_rgb ( cr, 0, 0, 0 );
            sprintf ( StrBuf, "%d", Loop + myChord.Position );
            cairo_show_text ( cr, "x" );

        }
        else {
            cairo_set_source_rgb ( cr, .95, .75, .75 );
            cairo_show_text ( cr, NoteNames[StringLayout[Loop][0]] );
        }

        //      cairo_move_to(cr, XOffset + 30 + Loop * FretOffset, YOffset + 275);
        cairo_stroke ( cr );
        cairo_set_line_width ( cr, 1 );
        cairo_set_source_rgb ( cr, 1, .99, .98 );
        cairo_move_to ( cr, XOffset + 30, YOffset + 20 + Loop * StringOffset );
        cairo_line_to ( cr, XOffset + 30 + FretOffset * MaxDisplayFrets, YOffset + 20 + Loop * StringOffset );
        cairo_stroke ( cr );
        GdkColor stringcolor2 = {0, 0xe6 << 8, 0xb7 << 8, 0x2d << 8};
        GdkColor stringcolor3 = {0, 0x90 << 8, 0x41 << 8, 0x00 << 8};

        cairo_set_line_width ( cr, 1 );
        cairo_set_source_rgb ( cr, .85, .8, .2 );
        cairo_move_to ( cr, XOffset + 30, YOffset + 22 + Loop * StringOffset );
        cairo_line_to ( cr, XOffset + 30 + FretOffset * MaxDisplayFrets, YOffset + 22 + Loop * StringOffset );
        cairo_stroke ( cr );

        cairo_set_line_width ( cr, 1 );

        cairo_set_source_rgb ( cr, .55, .3, 0x00 );
        cairo_move_to ( cr, XOffset + 30, YOffset + 24 + Loop * StringOffset );
        cairo_line_to ( cr, XOffset + 30 + FretOffset * MaxDisplayFrets, YOffset + 24 + Loop * StringOffset );
        cairo_stroke ( cr );
    }

    /* OK here is the real stuff, drawing the dots.
     */
    for ( StringLoop = 0; StringLoop < NumStrings; StringLoop++ ) {
        for ( FretLoop = 0; FretLoop < MaxDisplayFrets; FretLoop++ ) {
            if ( CurScale == (void *)1 ) {
                sprintf ( StrBuf, "%s", NoteNames[StringLayout[StringLoop][FretLoop]] );

                if ( StringLayout[StringLoop][FretLoop + DisplayPosition] == CurRootNote ) {
                    theBall = ( GdkPixbuf * ) BouncieBallRed;
                    cairo_set_source_surface ( cr, theBall,
                                               FretOffset * ( FretLoop ) + 32,
                                               YOffset + StringOffset * StringLoop );
                    cairo_paint ( cr );
                }

            }
            else {
                if ( myChord.ChordNotes[StringLoop][FretLoop] ) {

                    /* Change the color of the dot based on the
                     * scale interval.
                     */

                    switch ( myChord.ChordNotes[StringLoop][FretLoop] ) {
                    case 1:
                        theBall = ( GdkPixbuf * ) BouncieBallRed;
                        break;

                    case 3:
                        theBall = ( GdkPixbuf * ) BouncieBallYel;
                        break;

                    case 5:
                        theBall = ( GdkPixbuf * ) BouncieBallGr;
                        break;

                    case 6:
                    case 2:
                        theBall = ( GdkPixbuf * ) BouncieBallBl;
                        break;

                    default:
                        theBall = ( GdkPixbuf * ) BouncieBall;
                        break;
                    }

                    /* OK, now acutally draw the dot.
                     */
                    cairo_set_source_surface ( cr, theBall,
                                               FretOffset * ( FretLoop + 1 ),
                                               YOffset + StringOffset * StringLoop );
                    cairo_paint ( cr );
                    cairo_set_source_rgb ( cr, 1, 1, 1 );
                    sprintf ( StrBuf, "%d", myChord.ChordNotes[StringLoop][FretLoop] );
                    cairo_move_to ( cr, FretOffset * ( FretLoop + 1 ) + 18,
                                    YOffset + 25 + StringOffset * StringLoop );
                    cairo_set_font_size ( cr, 12 );

                    cairo_show_text ( cr, StrBuf );
                    cairo_stroke ( cr );

                }
            }

            /* Draw the note names here.
            */
            sprintf ( StrBuf, "%s", NoteNames[StringLayout[StringLoop][FretLoop + DisplayPosition]] );
            cairo_set_source_rgb ( cr, .7, .7, 1 );
            cairo_move_to ( cr, FretOffset * ( FretLoop ) + 14,
                            YOffset + 4 + StringOffset * StringLoop );
            cairo_set_font_size ( cr, 12 );

            cairo_show_text ( cr, StrBuf );
            cairo_stroke ( cr );
        }
    }

//	gtk_widget_draw (widget,cr);


//	return TRUE;
//gtk_widget_queue_draw(widget);

    gtk_widget_queue_resize_no_redraw ( widget );
    cairo_destroy ( cr );

    return TRUE;
}

#if 0
/*--------------------------------------------
 * Function:		hello
 *
 * Description:		This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below
 *
 *---------------------------------------------*/
static void hello ( GtkWidget *widget, gpointer data ) {
//   g_print ("Hello World\n");
//	SetupFretBoard();
//	PrintBoard();
}
#endif


#endif

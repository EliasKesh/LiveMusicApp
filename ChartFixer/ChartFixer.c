/*---------------------------------------------------------------------
|
|	File: 	ChartFixer
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Wed Oct 18 15:01:51 PDT 2017
|
|	Copyright ©: 	2017 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define ChartFixer_c

/*
 * Place Includes Here
 */
#include <printf.h>

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include <unistd.h>
#include "ChartFixer.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <libgen.h>

#define MaxStringLength		10000
#define MaxRefFiles			5
#define MaxMetaData			15
#define MaxDirName			200
#define MaxHTMLFiles		1000

char 	InputFileName[MaxStringLength];
//char 	OutputFileName[MaxStringLength];

FILE *InFile;
FILE *OutFile;

int 	InFileSize;
char 	*InString;
char 	DataString[MaxStringLength];

//char 	DirList[MaxDirName][MaxStringLength];
int 	DirCount;
//char 	FileList[MaxDirName][MaxStringLength];
int 	FileCount;
int 	TotalMetaData;

/*
 * Place defines and Typedefs here
 */
typedef struct {
	char 	Title[MaxStringLength];
	char 	Href[MaxRefFiles][MaxStringLength];
	char 	ChartSrc[MaxRefFiles][MaxStringLength];
	char 	SoloKey[30];
	char 	LiveMusicData[MaxStringLength];
	char 	MetaDataTag[20][MaxStringLength];
	char 	MetaData[20][MaxStringLength];

	char 	DrumFilePerFlag;
	char 	IntroFlag;
	char 	LoopLenFlag;
	char 	BeatsPerFlag;
	char 	LooperFilePerFlag;
	char 	TempoFlag;
	char 	PresetFlag;
	char 	SetNowFlag;

	char 	StructureFlag;
	char 	ChordsFlag;

} ChartDataStruct;

ChartDataStruct		myChartData;

char 	DebugString = 0;
char 	theCurFileName[MaxDirName];
char 	theCurDirectory[MaxDirName];
char 	theCurPath[MaxDirName];
char 	NewFile;
char FixFlag = 0;
char VerifyFlag = 0;
char RecursiveFlag = 0;
char AllHTML[MaxHTMLFiles][MaxDirName];
int  AllHTMLIndex;
/*
 * Place Local prototypes here
 */

char *FindMarker(char *STag, char *ETag, char *theBuffer, char *RetString);
char *FindRef(char *STag,
              char *theBuffer, char *RetString);
int ReadChart (char *InputFileName);
int WriteChart (char *OutputFileName);
char *get_filename_ext(char *filename);
int Get_dir_Files(char * path, char FileList[MaxDirName][MaxStringLength]);
int VerifyChart (char *thePath);
int MyPrintf( const char *restrict format, ... );
void ShowHelp(void);
int listFilesRecursively(char *path);
void MakeExchange(char *MetaTag, char *MetaData);
int ShouldWeAddHTML(char *thePath);
int GetAllHTML(char *basePath);
int Get_dir_content(char * path, char DirList[MaxDirName][MaxStringLength]);
void show_dir_content(char * path);

int UpdateIndexFile(char *Path);


/*
 * Place Static variables here
 */

int myCompare (const void * pa, const void * pb ) {
	char *a = (char *)pa;
	char *b = (char *)pb;
	return (strcmp(a, b));
}

/*--------------------------------------------------------------------
 * Function:		GetFileInfo
 *
 * Description:		Get the Paths and Filename to create
 * 	the master page.
 *
 *---------------------------------------------------------------------*/
int UpdateIndexFile(char *Path) {
	char IndexName[200];
	FILE *IndexFile;
	int Loop = 0;
	char Padding[] = "____________________";
	char *PadPtr;

	sprintf(IndexName, "%s/index.html", Path);
	printf("UpdateIndexFile: %s\n", IndexName);
	IndexFile = fopen(IndexName, "w+");
	fprintf(IndexFile, "<html>\n<style>\nbody {\nbackground-image: url(background.png);\nbackground-repeat: no-repeat;\n\
		background-size: 100% 100%;\ntext-align: center;\ncolor: #ff00ff;\n\
		background-color: #000000;\nheight: 100%;\n}");
	fprintf(IndexFile, "<div id=\"link_bar\">\n\
    <a href=\"html-anchor-link.htm\">Anchor link</a>\n\
    <a href=\"html-link-color.htm\">Link color</a>\n\
    <a href=\"../mailto.htm\">Email link</a>\n\
    <a href=\"html-image-link.htm\">Image link</a>\n\
    <a href=\"html-text-link.htm\">Text link</a>\n\
	</div></style>\n");

	fprintf(IndexFile, "<p style=\"line-height:135%\">\n\
	<body link=\"#ffC0C0\" vlink=\"#808080\" alink=\"#FF0000\">\n\
	<code><big><big>\n");

	/* Get all of the html files.
	*/
	AllHTMLIndex = 0;
	memset(AllHTML, 0 , sizeof(AllHTML));
	GetAllHTML(Path);

	/* Sort Them
	*/
	qsort(AllHTML, AllHTMLIndex, MaxDirName, myCompare);


	/* Printf them.
	*/
	for (Loop = 0; Loop < AllHTMLIndex; Loop++) {

		/* Use 3 colums.
		*/
		if (!(Loop % 3))
			fprintf(IndexFile, "<br>");

		/* The name for the link text.
		*/
		sprintf(IndexName, "%s", basename(AllHTML[Loop]));

		/* Remove the extension.
		*/
		IndexName[strlen(IndexName) - 5] = 0;
		PadPtr = Padding + strlen(IndexName);

//		printf("%d [%-30s]\n", Loop, IndexName);

		/* Write out the link.
		*/
		fprintf(IndexFile, "[<a href=%s>%s</a>] %s \n",
		        AllHTML[Loop], IndexName, PadPtr );
	}

//	fprintf(IndexFile,"[<a href=%s>%s_______________</a>]");

	/* Write Footer.
	*/
	fprintf(IndexFile, "</code></big></big>\n\
		</p>\n</body>\n</html>");

	fclose(IndexFile);
}


/*--------------------------------------------------------------------
 * Function:		GetFileInfo
 *
 * Description:		Get the Paths and Filename.
 *
 *---------------------------------------------------------------------*/
int GetFileInfo(char *FileInfo) {
	char *thepath;
	struct stat statbuf;

	theCurFileName[0] = 0;
	theCurDirectory[0] = 0;
	NewFile = 0;

	if (stat(FileInfo, &statbuf) != 0) {
		strcpy(theCurPath, dirname(FileInfo));
		strcpy(theCurDirectory, basename(FileInfo));
		sprintf(InputFileName, "%s/%s.html", theCurPath , theCurDirectory);
		return 0;
	}

	thepath = realpath(FileInfo, NULL);

	if (!S_ISDIR(statbuf.st_mode)) {
		strcpy(theCurFileName, basename(thepath));
		strcpy(theCurPath, dirname(thepath));
	} else {
		strcpy(theCurPath, thepath);
	}
	strcpy(theCurDirectory, basename(theCurPath));
//printf("[%s] [%s] [%s]\n", theCurFileName, theCurDirectory, theCurPath);

	/* If there is no file, use the directory name .
	*/
	if (!theCurFileName[0]) {
		sprintf(InputFileName, "%s/%s.html", theCurPath , theCurDirectory);
	} else {
		sprintf(InputFileName, "%s/%s", theCurPath , theCurFileName);
	}

	if ( access( InputFileName, F_OK ) != -1 ) {
		NewFile = 0;
	} else {
		NewFile = 1;
	}
}

#ifdef StandAlone
/*--------------------------------------------------------------------
 * Function:		main
 *
 * Description:		Main for Chart Fixer
 *
 *---------------------------------------------------------------------*/
int main (int argc, char *argv[]) {
	int i = 0;
	char *CPtr;
	int  DirLoop;
	int  FileLoop;
	char *StrPtr;
	char UpdateIndex = 0;

	DebugString = 1;
	if (argc < 2) {
		MyPrintf("Usage: [options] {input file} {output file}\n\
					-f Fix the file\n\
					-v Verify \n\
					-d Show Debug\n\
					-r Recusrive\n\
					-u Update Index\n\
					ChartFixer -r -f /home/Dropbox/FusionBlue/ChartsHTML/\n\
					-- \n");

		return (1);
	}
	DebugString = 0;

	// Parse the Arguments
	for (i = 0; i < argc; i++) {
		MyPrintf("Argument %s\n", argv[i]);

		if (strcmp(argv[i], "-f") == 0) {
			strcpy(InputFileName, argv[++i]);
//			strcpy(OutputFileName, argv[++i]);
			MyPrintf("File = %s\n", InputFileName);
			FixFlag = 1;
		} else if (strcmp(argv[i], "-v") == 0) {
			strcpy(InputFileName, argv[++i]);
			MyPrintf("File = %s\n", InputFileName);
			VerifyFlag = 1;
		} else if (strcmp(argv[i], "-h") == 0) {
			ShowHelp();
			exit(0);
		} else if (strcmp(argv[i], "-d") == 0) {
			DebugString = 1;
			printf("Debug On\n");
			MyPrintf("    argWord=TRUE\n");
		} else if (strcmp(argv[i], "-u") == 0) {
			UpdateIndex = 1;
			printf("Update Index\n");
		} else if (strcmp(argv[i], "-r") == 0) {
			RecursiveFlag = 1;
//			strcpy(InputFileName, argv[++i]);
//            MyPrintf("    argChar=TRUE\n");
		} else if (strcmp(argv[i], "--") == 0) {
			if (i + 1 <= argc) {
//                fileName1 = argv[++i];
//                MyPrintf("    fileName1=%s\n",fileName1);
			}
			if (i + 1 <= argc) {
//                fileName2 = argv[++i];
//                MyPrintf("    fileName2=%s\n",fileName2);
			}
		}
	}

	DoChartFix(InputFileName,FixFlag, VerifyFlag, RecursiveFlag, UpdateIndex);
}
#endif

/*--------------------------------------------------------------------
 * Function:		DoChartFix
 *
 * Description:		Given the file/path, do what the options
 * 		ask .
 *
 *---------------------------------------------------------------------*/
int DoChartFix(char *InputFileName,
			   char FixFlag,
               char VerifyFlag,
               char RecursiveFlag,
               char UpdateIndexFlag) {
	MyPrintf("Flags F%d V%d R%d U%d\n",
	         FixFlag, VerifyFlag,
	         RecursiveFlag, UpdateIndexFlag);
	GetFileInfo(InputFileName);

// Given a directory scan through the files.
	if (RecursiveFlag) {
		listFilesRecursively(theCurPath);
	} else {

		// If we do not think it's a LiveMusic file get out.
		if (ReadChart(InputFileName) == 2) {
			printf("Not a Valid LiveMusic file.\n");
//			return (1);
		}
//		printf("Calling Verify %s\n", theCurPath);
		VerifyChart (theCurPath);

//	MyPrintf("Flags1 %d %d %d\n",FixFlag,VerifyFlag, RecursiveFlag);
		// Write any changes back to the file.
		if (FixFlag) {
			MyPrintf("About to Fix\n");

			if (NewFile == 0) {
//				printf("New File\n");
				WriteChart(InputFileName);
			} else if (ShouldWeAddHTML(theCurPath)) {
//					printf("ShouldWeAddHTML\n");
				WriteChart(InputFileName);
			}
		}
	}

	if 	(UpdateIndexFlag)
		UpdateIndexFile(theCurPath);

	MyPrintf("\n");
	return 0;
}

/*--------------------------------------------------------------------
* Function:		MakeExchange
*
* Description:		Make changes if required.
*
*---------------------------------------------------------------------*/
void MakeExchange(char *MetaTag, char *MetaData) {

//	if (strcmp(MetaTag, "Preset"))
//		return;
//
	if (!strcmp(MetaData, "Chorus"))
		strcpy(MetaData, "BaseChorus");

	if (!strcmp(MetaData, "SatrianiComp"))
		strcpy(MetaData, "BaseDist");

	if (!strcmp(MetaData, "CleanDist"))
		strcpy(MetaData, "BaseDist");

	if (!strcmp(MetaData, "FunkyGuitar"))
		strcpy(MetaData, "BaseChorus");
}


/*--------------------------------------------------------------------
* Function:		VerifyChart
*
* Description:		Check to make sure links match.
*
*---------------------------------------------------------------------*/
int VerifyChart (char *thePath) {
	int 	Loop;
	int 	Loop1;
	int 	MetaCount;
	int 	MatchCount;
	char 	*StrPtr;
	char 	pwd[100];
	char FileList[MaxDirName][MaxStringLength];

	MetaCount = TotalMetaData;
	memset(FileList, 0, sizeof(FileList));
	FileCount = Get_dir_Files(thePath, FileList);

//printf("VerifyChart %d %d %s\n", MetaCount, FileCount, thePath);
	// Look for accurate file names for Media and links.
	Loop = Loop1 = MatchCount = 0;
	while (myChartData.Href[Loop][0] != 0) {
		for (Loop1 = 0; Loop1 < MaxDirName; Loop1++) {
//			MyPrintf("Comp %s %s\n",myChartData.Href[Loop], FileList[Loop1] );
//printf("V: %d %s %s\n", Loop1, FileList[Loop1], myChartData.Href[Loop]);
			if (!strcmp(myChartData.Href[Loop], FileList[Loop1])) {
				MatchCount++;
				MyPrintf("Match %s %s\n", myChartData.Href[Loop], FileList[Loop1] );
			}

			if (FileList[Loop1][0] == 0)
				break;
		}
		Loop++;
	}

//	if (Loop < 2) {
	Loop = 0;
	for (Loop1 = 0; Loop1 < MaxDirName; Loop1++) {
		StrPtr = get_filename_ext(FileList[Loop1]);

		if (!strcmp(StrPtr, "mp3"))
			strcpy(myChartData.Href[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "mid"))
			strcpy(myChartData.Href[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "tg"))
			strcpy(myChartData.Href[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "gp"))
			strcpy(myChartData.Href[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "mp4"))
			strcpy(myChartData.Href[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "tab"))
			strcpy(myChartData.Href[Loop++], FileList[Loop1]);
	}
//	}

	if (Loop != MatchCount)
		printf("** %s\n** Warning Media files (%d) and links don't match (%d)\n", thePath, Loop, MatchCount);

	// Look for accurate file names for Charts and links.
	Loop = Loop1 = MatchCount = 0;
	while (myChartData.ChartSrc[Loop][0] != 0) {
		for (Loop1 = 0; Loop1 < MaxDirName; Loop1++) {
			MyPrintf("Comp %s %s\n", myChartData.ChartSrc[Loop], FileList[Loop1] );

			if (!strcmp(myChartData.ChartSrc[Loop], FileList[Loop1])) {
				MatchCount++;
//				printf("Match %s %s\n", myChartData.ChartSrc[Loop], FileList[Loop1] );
			}
			if (FileList[Loop1][0] == 0)
				break;
		}
		Loop++;
	}

//	if (Loop < 2) {
	Loop = 0;
	for (Loop1 = 0; Loop1 < MaxDirName; Loop1++) {
		StrPtr = get_filename_ext(FileList[Loop1]);
//printf("Image: %d %s %s\n", Loop1, FileList[Loop1], myChartData.ChartSrc[Loop]);

		if (!strcmp(StrPtr, "jpg"))
			strcpy(myChartData.ChartSrc[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "jpeg"))
			strcpy(myChartData.ChartSrc[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "png"))
			strcpy(myChartData.ChartSrc[Loop++], FileList[Loop1]);

		if (!strcmp(StrPtr, "pdf"))
			strcpy(myChartData.ChartSrc[Loop++], FileList[Loop1]);
	}
//	}

	if (Loop != MatchCount)
		printf("** %s\n** Warning Chart (%d) and links don't match (%d)\n", thePath, Loop, MatchCount);

	// Set the title of the chart.
	if (myChartData.Title[0] == 0) {
		strcpy(myChartData.Title, basename(theCurFileName));
	}

	MyPrintf("MetaTag %d\n", MetaCount);

	/* IF there is no Meta Data then add the default.	*/
	if (myChartData.PresetFlag == 0) {
		MyPrintf("Filling MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "Preset1");
		strcpy(myChartData.MetaData[MetaCount++], "Base");
		strcpy(myChartData.MetaDataTag[MetaCount], "Preset2");
		strcpy(myChartData.MetaData[MetaCount++], "BaseDist");
		strcpy(myChartData.MetaDataTag[MetaCount], "Preset3");
		strcpy(myChartData.MetaData[MetaCount++], "BaseWah");
		strcpy(myChartData.MetaDataTag[MetaCount], "Preset4");
		strcpy(myChartData.MetaData[MetaCount++], "Expr Vol");
		strcpy(myChartData.MetaDataTag[MetaCount], "Preset5");
		strcpy(myChartData.MetaData[MetaCount++], "BaseWah");
	}

	if (!myChartData.DrumFilePerFlag) {
		MyPrintf("*** LooperFlagPerFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "DrumFile");
		strcpy(myChartData.MetaData[MetaCount++], "/dev/null");
	}

	if (!myChartData.LooperFilePerFlag) {
		MyPrintf("*** LooperFlagPerFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "LoopFile");
		strcpy(myChartData.MetaData[MetaCount++], "/dev/null");
	}

	if (!myChartData.IntroFlag) {
		MyPrintf("*** IntroFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "IntroCount");
		strcpy(myChartData.MetaData[MetaCount++], "4");
	}

	if (!myChartData.BeatsPerFlag) {
		MyPrintf("*** BeatsPerFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "BeatsPerMeasure");
		strcpy(myChartData.MetaData[MetaCount++], "4");
	}

	if (!myChartData.LoopLenFlag) {
		MyPrintf("*** LoopLenFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "LoopLength");
		strcpy(myChartData.MetaData[MetaCount++], "8");
	}

	if (!myChartData.TempoFlag) {
		MyPrintf("*** TempoFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "Tempo");
		strcpy(myChartData.MetaData[MetaCount++], "20");
	}

	if (!myChartData.SetNowFlag) {
		MyPrintf("*** SetNowFlag MetaTag %d\n", MetaCount);
		strcpy(myChartData.MetaDataTag[MetaCount], "SetNow");
		strcpy(myChartData.MetaData[MetaCount++], "Base");
	}

	MyPrintf("Leaving Verify\n");
}

/*--------------------------------------------------------------------
* Function:		WriteChart
*
* Description:		Check for the file and pull all of the relevant data.
*
*---------------------------------------------------------------------*/
int WriteChart (char *OutputFileName) {
	int 	Loop;
	char 	*ExtPtr;


	MyPrintf("Output filename %s.\n", OutputFileName);
#if 0
	ExtPtr = get_filename_ext(OutputFileName);

	if (!strcmp(ExtPtr, "html")) {
		sprintf(OutputFileName, "%s.html")
	}
#endif
	OutFile = fopen(OutputFileName, "w+");
	if (OutFile == NULL) {
		MyPrintf("Can't create Output file.\n");
		return (1);
	}

	MyPrintf("Writing to %s \n", OutputFileName);
	fprintf(OutFile, "<html>\n<head>\n<style type=\"text/css\">\n");
	fprintf(OutFile, "{text-align: left; width: 100%; height: 100%;}\n");
	fprintf(OutFile, "body {\n");
	fprintf(OutFile, "background-image: url(../background.png);\n");
	fprintf(OutFile, "background-repeat: no-repeat;\n");
	fprintf(OutFile, "background-size: 200% 200%;\n");
	fprintf(OutFile, "color: #ff9000;\n");
	fprintf(OutFile, "background-color: #000000;\n");
	fprintf(OutFile, "height: 100%; }\n");
	fprintf(OutFile, "</style>\n</head>\n<title>%s</title>\n", myChartData.Title);

	fprintf(OutFile, "<p style=\"line-height:125%\">\n");
	fprintf(OutFile, "<body>\n<big>\n");

	// Write the Meta Data
	Loop = 0;
	while (myChartData.MetaDataTag[Loop][0] != 0) {
		fprintf(OutFile, "<meta name=\"%s\" content=\"%s\" >\n",
		        myChartData.MetaDataTag[Loop], myChartData.MetaData[Loop]);
		Loop++;
	}

	fprintf(OutFile, "<font color=#880000>%s  </font>\n", myChartData.Title);
	fprintf(OutFile, "<font color=#8800FF>Solo %s  </font>\n", myChartData.SoloKey);

	// Write the Media files
	Loop = 0;
	while (myChartData.Href[Loop][0] != 0) {
		ExtPtr = get_filename_ext(myChartData.Href[Loop]);
//printf("In Loop %s %s\n", myChartData.Href[Loop], ExtPtr);
		if (!strcmp(ExtPtr, "mp3")) {
			fprintf(OutFile, "<a style=\"color:red\" href=%s>[%s-%d]</a>  \n",
			        myChartData.Href[Loop++], ExtPtr, Loop);
		} else {
			fprintf(OutFile, "<a style=\"color:yellow\" href=%s>[%s-%d]</a>  \n",
			        myChartData.Href[Loop++], ExtPtr, Loop);
		}
	}
	fprintf(OutFile, "<br>\n");

	if (strlen(myChartData.LiveMusicData) < 10) {
		fprintf(OutFile, "<LiveMusic><code>\n");
		fprintf(OutFile, "<font color=#88ff00>Chords:  </font> <br>\n");
		fprintf(OutFile, "<br>\n<br>\n<br>\n");
		fprintf(OutFile, "<font color=#88ffff>Structure:  </font> <br>\n");
		fprintf(OutFile, "<br>\n<br>\n<br>\n");
		fprintf(OutFile, " %s \n</code></LiveMusic>\n<tr>\n",
		        myChartData.LiveMusicData);
	} else {
		fprintf(OutFile, "<LiveMusic> %s ",
		        myChartData.LiveMusicData);

		if ( !myChartData.ChordsFlag )
			fprintf(OutFile, "<font color=#88ff00>Chords:  </font> <br>\n");

		if ( !myChartData.StructureFlag )
			fprintf(OutFile, "<font color=#88ffff>Structure:  </font> <br>\n");

		fprintf(OutFile, "</LiveMusic>\n<tr>\n");

	}

	// See if there are any image files.
	Loop = 0;
	while (myChartData.ChartSrc[Loop][0] != 0) {
		ExtPtr = get_filename_ext(myChartData.ChartSrc[Loop]);

		if (!strcmp(ExtPtr, "pdf")) {
			fprintf(OutFile, "<embed src=\"%s\" height=\"100%\" width=\"100%\" >\n",
			        myChartData.ChartSrc[Loop]);
		} else {
			fprintf(OutFile, "<img alt=\"\" src=\"%s\" height=\"100%\" >\n",
			        myChartData.ChartSrc[Loop]);
		}
		Loop++;
	}

	fprintf(OutFile, "</tr>\n</body>\n</html>\n");
	fclose(OutFile);
}

/*--------------------------------------------------------------------
* Function:		ReadChart
*
* Description:		Check for the file and pull all of the relevant data.
*
*---------------------------------------------------------------------*/
int ReadChart (char *InputFileName) {
	int i = 0;
	char *CPtr;
	int 	Loop;


	/* Clear the main data structure before starting.
	*/
//	memset(&FileList, 0, sizeof(FileList));
//	memset(&DirList, 0, sizeof(DirList));
	memset(&myChartData, 0, sizeof(myChartData));

	TotalMetaData = 0;

	/* Open the file.
	*/
	InFile = fopen(InputFileName, "r");
//printf("In Read %d %s\n", InFile, InputFileName);
	if (InFile == NULL) {
		MyPrintf("Input File not found.\n");
		return (1);
	}

	fseek(InFile, 0, SEEK_END);
	InFileSize = ftell(InFile);

	if (InFileSize < 10) {
		MyPrintf("Input File not found.\n");
		return (1);
	}

	fseek(InFile, 0, SEEK_SET);  //same as rewind(f);

	InString = (char *)malloc(InFileSize + 1);
	fread(InString, InFileSize, 1, InFile);

	// Clear the data structure.
	memset(&myChartData, 0, sizeof(ChartDataStruct));

	// Get the Title of the Chart
#if 1
	CPtr = FindMarker("<title>", "</title>",
	                  InString, (char *)&myChartData.Title);
	MyPrintf("Title=%s\n", myChartData.Title);
#endif
	// Get the Added information of the Chart
	CPtr = FindMarker("<LiveMusic>", "</LiveMusic>",
	                  InString, (char *)&myChartData.LiveMusicData);
	MyPrintf("LiveMusic=%s\n", myChartData.LiveMusicData);

	if (!CPtr)
		return (2);

	// Look for mp3,midi or tg files.
	CPtr = InString;
	for (Loop = 0; Loop < MaxRefFiles; Loop++) {
		CPtr = FindRef("href=", CPtr, (char *)&myChartData.Href[Loop]);
		if (CPtr == NULL)
			break;

		MyPrintf("MP3=%s\n", myChartData.Href[Loop]);
	}

	// Look for the solo Key
	CPtr = FindRef("Solo ", InString, (char *)&myChartData.SoloKey);
	MyPrintf("Solo=%s\n", myChartData.SoloKey);

	// Look for the graphics, jpg or PDF
	CPtr = InString;
	for (Loop = 0; Loop < MaxRefFiles; Loop++) {
		CPtr = FindRef("src=", CPtr, (char *)&myChartData.ChartSrc[Loop]);
		if (CPtr == NULL)
			break;
		MyPrintf("MP3=%s\n", myChartData.ChartSrc[Loop]);
	}

	// Look for the Meta Data
	CPtr = InString;
	for (Loop = 0; Loop < MaxMetaData; Loop++) {
		CPtr = FindRef("name=", CPtr,  (char *)&myChartData.MetaDataTag[Loop]);
		if (CPtr == NULL)
			break;
		MyPrintf("Name=%s\n", myChartData.MetaDataTag[Loop]);

		CPtr = FindRef("content=", CPtr,  (char *)&myChartData.MetaData[Loop]);
		if (CPtr == NULL)
			break;
		MyPrintf("content=%s\n", myChartData.MetaData[Loop]);
		TotalMetaData = Loop + 1;

		MakeExchange(myChartData.MetaDataTag[Loop], myChartData.MetaData[Loop]);

		if ( !strcmp(myChartData.MetaDataTag[Loop], "IntroCount") ) {
			myChartData.IntroFlag = 1;
			MyPrintf("*** Read Found IntroCount\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "LoopLength") ) {
			myChartData.LoopLenFlag = 1;
			MyPrintf("*** Read Found LoopLength\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "BeatsPerMeasure") ) {
			myChartData.BeatsPerFlag = 1;
			MyPrintf("*** Read Found BeatsPerMeasure\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "LoopFile") ) {
			myChartData.LooperFilePerFlag = 1;
			MyPrintf("*** Read Found LoopFile\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "DrumFile") ) {
			myChartData.DrumFilePerFlag = 1;
			MyPrintf("*** Read Found DrumFile\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "Tempo") ) {
			myChartData.TempoFlag = 1;
			MyPrintf("*** Read Found Tempo\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "Preset1") ) {
			myChartData.PresetFlag = 1;
			MyPrintf("*** Read Found Preset\n");
		}

		if ( !strcmp(myChartData.MetaDataTag[Loop], "SetNow") ) {
			myChartData.SetNowFlag = 1;
			MyPrintf("*** Read Found SetNow\n");
		}
	}

	if ( strstr(InString, "88ffff")) {
		myChartData.StructureFlag = 1;
		MyPrintf("*** Structure Found\n");
	}

	if ( strstr(InString, "88ff00")) {
		myChartData.ChordsFlag = 1;
		MyPrintf("*** Chords Found\n");
	}

	free(InString);
	fclose(InFile);
	MyPrintf("Done\n");
	return 0;
}

/*--------------------------------------------------------------------
 * Function:		FindMarker
 *
 * Description:		Find a marker and the test between.
 *
 *---------------------------------------------------------------------*/
char *FindMarker(char *STag, char *ETag,
                 char *theBuffer, char *RetString) {
	char *SPtr;
	char *EPtr;
	int   SLength;
	int   RLength;

	SPtr = strstr(theBuffer, STag);
	EPtr = strstr(theBuffer, ETag);

	if (SPtr && EPtr) {
		SLength = strlen(STag);
		RLength = EPtr - SPtr - SLength;
		MyPrintf("%x %x == %d %d\n", SPtr, EPtr, EPtr - SPtr, RLength);
		strncpy(RetString, (SLength + SPtr), RLength);
		RetString[RLength] = 0;
		return (EPtr + SLength + 1);
	} else
		return (NULL);

}

/*--------------------------------------------------------------------
* Function:		FindRef
*
* Description:		Look for HTML reference.
*
*---------------------------------------------------------------------*/
char *FindRef(char *STag,
              char *theBuffer, char *RetString) {
	char *SPtr;
	int 	Loop;
	int   SLength;
	char   *thePtr;

	SPtr = strstr(theBuffer, STag);

	if (SPtr == NULL)
		return (NULL);

	SLength = strlen(STag);

	thePtr = SLength + SPtr;

	// If the first character is a quote then look for the next one.
	if (*thePtr == '\"') {
		thePtr++;
		if (*thePtr == '.') {
			thePtr++;

			if (*thePtr == '/')
				thePtr++;
		}

		for (Loop = 0; Loop < 200; Loop++) {
			RetString[Loop] = *thePtr++;
			if (*thePtr == '\"') {
				RetString[Loop + 1] = 0;
				return (thePtr);
			}
		}
	} else {
		if (*thePtr == '.')
			thePtr++;

		if (*thePtr == '/')
			thePtr++;

		for (Loop = 0; Loop < 200; Loop++) {
			RetString[Loop] = *thePtr++;
			if (*thePtr == ' ' || *thePtr == '>') {
				RetString[Loop + 1] = 0;
				return (thePtr);
			}
		}
	}

	return (SLength + SPtr);
}

#if 0
/*--------------------------------------------------------------------
* Function:		show_dir_content
*
* Description:		List the directory contents.
*
*---------------------------------------------------------------------*/
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"
void show_dir_content(char * path) {
	DIR * d = opendir(path); // open the path
	if (d == NULL) return; // if was not able return
	struct dirent * dir; // for the directory entries
	while ((dir = readdir(d)) != NULL) { // if we were able to read somehting from the directory
		if (dir-> d_type != DT_DIR) // if the type is not directory just print it with blue
			MyPrintf("%s%s\n", BLUE, dir->d_name);
		else if (dir -> d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 ) { // if it is a directory
			MyPrintf("%s%s\n", GREEN, dir->d_name); // print its name in green
			char d_path[255]; // here I am using sMyPrintf which is safer than strcat
			sprintf(d_path, "%s/%s", path, dir->d_name);
//			show_dir_content(d_path); // recall with the new path
		}
	}
	closedir(d); // finally close the directory
}


/*--------------------------------------------------------------------
* Function:		Get_dir_content
*
* Description:		List the directory contents.
*
*---------------------------------------------------------------------*/
int Get_dir_content(char * path, char DirList[MaxDirName][MaxStringLength]) {
	int Loop = 0;
	DIR * d = opendir(path); // open the path
	if (d == NULL) return (0); // if was not able return
	struct dirent * dir; // for the directory entries
	while ((dir = readdir(d)) != NULL) { // if we were able to read somehting from the directory
		if (dir -> d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 ) {  // if it is a directory

//			MyPrintf("Hello %s%s\n", GREEN, dir->d_name); // print its name in green
			strncpy((char *)&DirList[Loop++], dir->d_name, MaxStringLength);
#if 0
			MyPrintf("%s%s\n", GREEN, dir->d_name); // print its name in green
			char d_path[255]; // here I am using sMyPrintf which is safer than strcat
			sprintf(d_path, "%s/%s", path, dir->d_name);
#endif
		}
	}

	closedir(d); // finally close the directory
	return (Loop);
}
#endif
/*--------------------------------------------------------------------
* Function:		Get_dir_Files
*
* Description:		List the directory contents.
*
*---------------------------------------------------------------------*/
int Get_dir_Files(char * path, char FileList[MaxDirName][MaxStringLength]) {
	int Loop = 0;
	DIR * d = opendir(path); // open the path
	if (d == NULL) return (0); // if was not able return
	struct dirent * dir; // for the directory entries
	while ((dir = readdir(d)) != NULL) { // if we were able to read somehting from the directory
		if (dir -> d_type != DT_DIR) { // if it is a directory
//			MyPrintf("Files: %d %s\n", strlen(dir->d_name),dir->d_name); // print its name in green
//			MyPrintf("Print %x\n",FileList[Loop][0] ); // print its name in green
			strncpy(FileList[Loop++], dir->d_name, MaxStringLength);
//			FileList[Loop++][strlen(dir->d_name)] = 0;
#if 0
			MyPrintf("%s%s\n", GREEN, dir->d_name); // print its name in green
			char d_path[255]; // here I am using sprintf which is safer than strcat
			sprintf(d_path, "%s/%s", path, dir->d_name);
#endif
		}
	}

	closedir(d); // finally close the directory
	return (Loop);
}


/*--------------------------------------------------------------------
* Function:		get_filename_ext
*
* Description:		Get file extension.
*
*---------------------------------------------------------------------*/
char *get_filename_ext(char *filename) {
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return (char *)(dot + 1);
}

/*--------------------------------------------------------------------
* Function:		MyMyPrintf
*
* Description:		Get file extension.
*
*---------------------------------------------------------------------*/
int MyPrintf( const char *restrict format, ... ) {

	va_list arg;
	int done;

	if (DebugString) {
		va_start (arg, format);
		done = vfprintf (stdout, format, arg);
		va_end (arg);
	}
	return done;


}

/*--------------------------------------------------------------------
* Function:		ShowHelp
*
* Description:		Get file extension.
*
*---------------------------------------------------------------------*/
void ShowHelp(void) {
	printf("Chart Fixer fixes html files for the LiveMusicApp\n");
	printf("ChartFixers [options] InputFile OutputFile\n");
	printf("-f  Fix the links based on the contents of the directory.\n");
	printf("-v  Verify links based on the contents of the directory.\n");
	printf("-d  Turn on Debugging messages.\n");
	printf("-r  Resursive.\n");

}

/*--------------------------------------------------------------------
* Function:		ShouldWeAddHTML
*
* Description:		Is this a valid directory for an html File.
*
*---------------------------------------------------------------------*/
int ShouldWeAddHTML(char *thePath) {
	int Loop = 0;
	int DirNum = 0;
	int AudioNum = 0;
	int HTMLNum = 0;
	int ImageNum = 0;
	char *StrPtr;

//printf("ShouldWeAddHTML %s\n", thePath);
	DIR * d = opendir(thePath); // open the path
	if (d == NULL) return (0); // if was not able return
	struct dirent * dir; // for the directory entries
	while ((dir = readdir(d)) != NULL) { // if we were able to read somehting from the directory
		if (strcmp(dir->d_name, ".") != 0 &&
		        strcmp(dir->d_name, "..") != 0) {

//			printf("*** N->%s \n", dir->d_name);
			if (dir->d_type == DT_DIR) {
				DirNum++;
			} else {
				Loop++;
				StrPtr = get_filename_ext(dir->d_name);

				if (!strcmp(StrPtr, "mp3"))
					AudioNum++;

				if (!strcmp(StrPtr, "html"))
					return (0);

				if (!strcmp(StrPtr, "png"))
					ImageNum++;

				if (!strcmp(StrPtr, "jpg"))
					ImageNum++;

				if (!strcmp(StrPtr, "jpeg"))
					ImageNum++;
			}
		}
	}
	closedir(d); // finally close the directory

	MyPrintf("******* L%d D%d A%d I%d\n", Loop, DirNum, AudioNum, ImageNum);
	if (DirNum == 0 ) {
		if ( (AudioNum + ImageNum) > 0 &&
		        (AudioNum + ImageNum) < 10)
//			printf("******* Found \n");
			return (1);
	}
	return (0);
}


/*--------------------------------------------------------------------
* Function:		listFilesRecursively
*
* Description:		Lists all files and sub-directories recursively
 * considering path as base path.
*
*---------------------------------------------------------------------*/
int listFilesRecursively(char *basePath) {
	char 	*StrPtr;
	char path[1000];
	char TheFullName[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	// Unable to open directory stream
	if (!dir)
		return (0);

	/* Loop thru the directories.
	*/
	while ((dp = readdir(dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 &&
		        strcmp(dp->d_name, "..") != 0) {

//			printf("Names %s\n", dp->d_name);
			StrPtr = get_filename_ext(dp->d_name);

			/* If it's an HTML file. let's check it out.
			*/
			if (!strcmp(StrPtr, "html")) {

				/* Create the Full Filename.
				*/
				sprintf(TheFullName, "%s/%s", basePath, dp->d_name);
//				printf("Full Name %s\n Base %s Name %s\n",
//				       TheFullName, basePath, dp->d_name);

				// If we do not think it's a LiveMusic file get out.
				if (ReadChart(TheFullName) == 2) {
					printf("Not a Valid LiveMusic file %s\n", TheFullName);
					return (1);
				}

				/* Check for the number of links.
				*/
				VerifyChart (basePath);

				// Write any changes back to the file.
				if (FixFlag) {
					MyPrintf("About to Fix\n");
					WriteChart(TheFullName);
				}
			}
			/* If the directory doesn't contain and HTML file/
			*/
			else {
				/* YOU ARE Here*/
				if (dp->d_type == DT_DIR) {
					sprintf(TheFullName, "%s/%s", basePath, dp->d_name);
//					printf("**ShouldWeAddHTML** %s %s %s\n", TheFullName, basePath, dp->d_name);
					if (ShouldWeAddHTML(TheFullName)) {
						ReadChart(TheFullName);
						VerifyChart (basePath);

						if (FixFlag) {
							sprintf(TheFullName, "%s/%s/%s.html", basePath, dp->d_name, dp->d_name);

							WriteChart(TheFullName);
						}

					}
				}
			}

			// Construct new path from our base path
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);

			listFilesRecursively(path);
		}
	}

	closedir(dir);
	return (0);
}

/*--------------------------------------------------------------------
* Function:		GetAllHTML
*
* Description:		Find all Valid HTML files.
*
*---------------------------------------------------------------------*/
int GetAllHTML(char *basePath) {
	char 	*StrPtr;
	char path[1000];
	char TheFullName[1000];
	struct dirent *dp;
	DIR *dir = opendir(basePath);

	// Unable to open directory stream
	if (!dir)
		return (0);

	/* Loop thru the directories.
	*/
	while ((dp = readdir(dir)) != NULL) {
		if (strcmp(dp->d_name, ".") != 0 &&
		        strcmp(dp->d_name, "..") != 0) {

//			printf("Names %s\n", dp->d_name);
			StrPtr = get_filename_ext(dp->d_name);

			/* If it's an HTML file. let's check it out.
			*/
			if (!strcmp(StrPtr, "html")) {

				/* Create the Full Filename.
				*/
//				if (VerifyChart(dirname(AllHTML[AllHTMLIndex])))
				sprintf(AllHTML[AllHTMLIndex++], "%s/%s", basePath, dp->d_name);
			}

			// Construct new path from our base path
			strcpy(path, basePath);
			strcat(path, "/");
			strcat(path, dp->d_name);

			GetAllHTML(path);
		}
	}
	closedir(dir);
	return (0);
}

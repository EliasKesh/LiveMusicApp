/*	File: 	PrefsFile
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Tue Dec 25 17:28:58 PST 2012
|
|	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define PrefsFile_c

/*
 * Place Includes Here
 */
#include "GTKMidiUI.h"
#include "PrefsFile.h"



/*
 * Place defines and Typedefs here
 */

#define PREFSFILENAME "LiveMusic.xml"
/*
 * Place Local prototypes here
 */

void parseDoc(char *docname);

/*
 * Place Static variables here
 */
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

/*--------------------------------------------------------------------
* Function:		InitPref
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void InitPref(void) {

//	WritePref();
	ReadPrefs();
}

/*--------------------------------------------------------------------
* Function:		WritePref
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void WritePref(void) {
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */
    char buff[256];
    int Loop;

    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST XML_BASIC_SETTINGS);
    xmlDocSetRootElement(doc, root_node);

    xmlNewChild(root_node, NULL, BAD_CAST "SongPath", BAD_CAST "/home/Dropbox/FusionBlue/ChartsHTML/");

    /*
     * Write out the Buttons
     */
    node1 = xmlNewChild(root_node, NULL, BAD_CAST "MainButtons", NULL);
	for (Loop = 0; Loop < Max_Main_Buttons; Loop++) {
//		doc->children = xmlNewDocNode(doc, NULL, gMyInfo.MyPatchInfo[Loop].Button, NULL);
		node = xmlNewChild(node1, NULL, gMyInfo.MyPatchInfo[Loop].Button, NULL);
		xmlSetProp(node, "Name", gMyInfo.MyPatchInfo[Loop].Name);
		sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Channel);
		xmlSetProp(node, "Channel", buff);
		sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].BankSelect);
		xmlSetProp(node, "Controller", buff);
		sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].OutPort);
		xmlSetProp(node, "OutPort", buff);
		sprintf(buff, "%03d", gMyInfo.MyPatchInfo[Loop].Patch);
		xmlSetProp(node, "Patch", buff);
	}

	sprintf(buff, "%03d", gMyInfo.NumOutPorts);
    xmlNewChild(root_node, NULL, BAD_CAST "NumOutPorts", BAD_CAST buff);

    node1 = xmlNewChild(root_node, NULL, BAD_CAST "OutPorts", NULL);
	for (Loop = 0; Loop < MaxOutPorts; Loop++) {
		sprintf(buff, "Port%03d", Loop);
		node = xmlNewChild(node1, NULL, buff, NULL);
		xmlSetProp(node, "Name", gMyInfo.OutPortName[Loop]);
	}


   /*
    * Dumping document to stdio or file
    */
    xmlSaveFormatFileEnc(PREFSFILENAME, doc, "UTF-8", 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();
}

/*--------------------------------------------------------------------
* Function:		ReadPrefs
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void ReadPrefs(void) {
	char *docname;

	docname = PREFSFILENAME;
	parseDoc (docname);
}

/*--------------------------------------------------------------------
* Function:		getReference
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void getReference (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *key;
	xmlNodePtr	node = NULL;
	printf("In Get Reference\n");
	int Loop=0;

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		printf("In Read Loop %s\n",cur->name);

	    if ((!xmlStrcmp(cur->name, (const xmlChar *)XML_BASIC_SETTINGS ))) {
	        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	        printf("keyword: %s\n", key);
	    	xmlFree(key);
	    }

	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"SongPath" ))) {
	    	        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    	        printf("Read Path: %s\n", key);
	    	        strncpy(gMyInfo.BasePath, key, 255);
	    	    	xmlFree(key);
	    }

	    /*
	     * Read the Button information
	     */
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"MainButtons" ))) {
	    	node = cur->xmlChildrenNode;
	    		while(node) {
	    			if (xmlStrcmp(node->name, (const xmlChar *)"text" )) {
	    	        printf("Button: %s ", node->name);

	    			key = xmlGetProp(node, "Name" );
	    	        printf("Name: %s ", key);
	    	        strcpy(gMyInfo.MyPatchInfo[Loop].Name, key);
	    	    	xmlFree(key);

	    			key = xmlGetProp(node, "Channel" );
	    	        printf("Chan: %d ", atoi(key));
	    	        gMyInfo.MyPatchInfo[Loop].Channel = atoi(key);
	    	    	xmlFree(key);

	    			key = xmlGetProp(node, "Controller" );
	    	        printf("Chan: %d ", atoi(key));
	    	        gMyInfo.MyPatchInfo[Loop].BankSelect = atoi(key);
	    	    	xmlFree(key);

	    			key = xmlGetProp(node, "OutPort" );
	    	        printf("Chan: %d ", atoi(key));
	    	        gMyInfo.MyPatchInfo[Loop].OutPort = atoi(key);
	    	    	xmlFree(key);

	    			key = xmlGetProp(node, "Patch" );
	    	        printf("Chan: %d \n", atoi(key));
	    	        gMyInfo.MyPatchInfo[Loop].Patch = atoi(key);
	    	    	xmlFree(key);

	    	    	/* This is Bad, Compare with Button index. */
	    	    	Loop++;
	    			}
	    	    	node = node->next;
	    		}
	    	}

	    /*
	     * Read the port information
	     */
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"NumOutPorts" ))) {
	    	        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	    	        printf("Read Path: %s\n", key);
	    	        gMyInfo.NumOutPorts = atoi(key);
	    	    	xmlFree(key);
	    }

	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"OutPorts" ))) {
    	gMyInfo.NumOutPorts = 0;
    	node = cur->xmlChildrenNode;
    		while(node) {
    			if (xmlStrcmp(node->name, (const xmlChar *)"text" )) {
    				printf("Button: %s ", node->name);

    			gMyInfo.NumOutPorts++;
    			key = xmlGetProp(node, "Name" );
    	        printf("Name: %s ", key);
    	        strcpy(gMyInfo.OutPortName[Loop], key);
    	    	xmlFree(key);


    	    	/* This is Bad, Compare with Button index. */
    	    	Loop++;
    			}
    	    	node = node->next;
    		}
    	}
    	cur = cur->next;
	}

	return;
}

/*--------------------------------------------------------------------
* Function:		parseDoc
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void parseDoc(char *docname) {
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) XML_BASIC_SETTINGS )) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return;
	}

	getReference (doc, cur);
	xmlFreeDoc(doc);
	return;
}


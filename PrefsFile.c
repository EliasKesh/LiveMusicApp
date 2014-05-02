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
#include <stdio.h>
#include <libxml/parser.h>
#include "xmltok/xmlparse.h"


// /usr/include/libxml2/
/*
 * Place defines and Typedefs here
 */

#define PREFSFILENAME "LiveMusic.xml"
#define Test2

/*
 * Place Local prototypes here
 */

void parseDoc(char *docname);

/*
 * Place Static variables here
 */

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

#ifdef Test
void startElement1(void *userData, const char *name, const char **atts)
{
  int i;
  int *depthPtr = userData;
  for (i = 0; i < *depthPtr; i++)
    putchar('\t');
//  puts(name);
//	printf("%d Start [%s] %s %s %s %s %s\n",*depthPtr, name, atts[1],atts[3],atts[5],atts[7],atts[9]);
//	printf("%d Start [%s] %s %s %s\n",*depthPtr, name, atts[1],atts[3],atts[5]);
	printf("%d Start [%s] %s %s\n",*depthPtr, name, atts[1],atts[3]);
//	printf("****  %d Start [%s] %s\n",*depthPtr, name, atts[1]);
  *depthPtr += 1;
}

void endElement1(void *userData, const char *name)
{
  int *depthPtr = userData;
  *depthPtr -= 1;	
//  printf("end [%s]\n",name);

}

/*--------------------------------------------------------------------
* Function:		ReadPrefs
*
* Description:		<Description/Comments>
*
*---------------------------------------------------------------------*/
void ReadPrefs(void) {
  char buf[200];
  XML_Parser parser = XML_ParserCreate(NULL);
  int done;
  int depth = 0;
  XML_SetUserData(parser, &depth);
  XML_SetElementHandler(parser, startElement1, endElement1);
	FILE *myFile;

//	parseDoc (PREFSFILENAME);
	myFile = fopen(PREFSFILENAME, "r" );

  do {
    size_t len = fread(buf, 1, sizeof(buf), myFile);
    done = len < sizeof(buf);
    if (!XML_Parse(parser, buf, len, done)) {
      fprintf(stderr,
	      "%s at line %d\n",
	      XML_ErrorString(XML_GetErrorCode(parser)),
	      XML_GetCurrentLineNumber(parser));
      return 1;
    }
  } while (!done);
  XML_ParserFree(parser);

}
// Test
#endif 
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


#ifdef Test1
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
 
static void print_element_names(xmlNode * a_node);
 
void ReadPrefs()
{
 
        xmlDoc         *doc = NULL;
        xmlNode        *root_element = NULL;
        const char     *Filename = PREFSFILENAME;
 
        doc = xmlReadFile(Filename, NULL, 0);
 
        if (doc == NULL)
          {
                  printf("error: could not parse file %s\n", Filename);
          }
        else
          {
 
                  /*
                   * Get the root element node
                   */
                  root_element = xmlDocGetRootElement(doc);
 
                  print_element_names(root_element);
 
                  /*
                   * free the document
                   */
                  xmlFreeDoc(doc);;
          }
        /*
         *Free the global variables that may
         *have been allocated by the parser.
         */
        xmlCleanupParser();
 
        return (0);
 
}
 
/* Recursive function that prints the XML structure */
 
static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
 
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s %s %s\n", cur_node->name, 
			xmlNodeGetContent(cur_node),xmlNodeGetContent(cur_node->next) );
        }
 
        print_element_names(cur_node->children);
    }
}
// Test1
#endif

#ifdef Test2
#include <libxml/xmlreader.h>
// NodeType: The node type, 
// 1 for start element, 
// 2 for attributes, 
// 3 for text nodes, 
// 4 for CData sections, 
// 5 for entity references, 
// 6 for entity declarations, 
// 7 for PIs, 
// 8 for comments, 
// 9 for the document nodes, 
// 10 for DTD/Doctype nodes, 
// 11 for document fragment and 
// 12 for notation nodes.
// 13 Whitespace
// 14 SignificantWhitespace
// 15 for end of element, 
// 16 EndEntity
// XML_ELEMENT_NODE
// 
static void processNode(xmlTextReaderPtr reader, char *Location) {
    xmlChar *name, *value;
	int		NodeType;
	int		Depth;
	
    name = xmlTextReaderName(reader);
    if (name == NULL)
        name = xmlStrdup(BAD_CAST "--");
    value = xmlTextReaderValue(reader);

	NodeType = xmlTextReaderNodeType(reader);
	Depth = xmlTextReaderDepth(reader);
//	if (NodeType != 14 && NodeType != 3) {
	if (NodeType == 1) {
    printf("%s D=%d T=%d N=[%s] E=%d ",Location,
            Depth,
            NodeType,
            name,
            xmlTextReaderIsEmptyElement(reader));
    xmlFree(name);
	}

#if 1
    if (value != NULL) {
        printf("V=[%s]", value);
        xmlFree(value);
    }
#endif
}

void ReadPrefs() {
    xmlTextReaderPtr reader;
    int ret;
	printf("----------------------\n");
	printf("Reading prefs file\n");
	printf("----------------------\n");
    reader = xmlNewTextReaderFilename(PREFSFILENAME);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader,"First");
				printf("\n");
			while(xmlTextReaderMoveToNextAttribute(reader))
				processNode(reader,"Sec");
				printf("\n");


            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            printf("%s : failed to parse\n", PREFSFILENAME);
        }
    } else {
        printf("Unable to open %s\n", PREFSFILENAME);
    }
	printf("----------------------\n");
	printf("Reading prefs file\n");
	printf("----------------------\n");
}

#endif

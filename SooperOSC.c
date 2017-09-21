/*---------------------------------------------------------------------
|
|	File: 	SooperOSC
|
|	Contains: 	
|
|
|	Written By: 	Elias Keshishoglou on Wed Sep 20 16:37:34 PDT 2017
|
|	Copyright ©: 	2017 Elias Keshishoglou all rights reserved.
|
|
|---------------------------------------------------------------------*/

#define SooperOSC_c

/*
 * Place Includes Here
 */

#include "GTKMidiUI.h"
#include "SooperOSC.h"
// #include <netinet/in.h>
#include <lo/lo.h>


/*
 * Place defines and Typedefs here
 */


/*
 * Place Local prototypes here
 */


/*
 * Place Static variables here
 */
static lo_address OSCaddr;


void MyOSCInit(void) {


	OSCaddr = lo_address_new(
			gMyInfo.OSCIPAddress, 
			gMyInfo.OSCPortNum );

}


#if 0
void MyOSCInit(void) {
struct sockaddr_in OSCAddress;
OSCAddress.sin_addr.s_addr = inet_addr("192.168.1.100");
OSCAddress.sin_port = htons(server_port);
}
#endif

void MyOSCClose(void) {

}
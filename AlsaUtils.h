/*
 * AlsaUtils.h
 *
 *  Created on: Dec 25, 2012
 *	  Author: elias
 */

#ifndef ALSAUTILS_H_
#define ALSAUTILS_H_
#include <asoundlib.h>
bool MyAlsaInit(void);

int SendMidi(char Type, char Port, char Channel, char Controller, int Value);
int SendMidiPatch( PatchInfo *thePatch);

#endif /* ALSAUTILS_H_ */

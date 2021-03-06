/*
 * AlsaUtils.h
 *
 *  Created on: Dec 25, 2012
 *	  Author: elias
 */

#ifndef ALSAUTILS_H_
#define ALSAUTILS_H_
#include <alsa/asoundlib.h>
bool MyAlsaInit(void);
bool MyAlsaClose();

int SendMidi(char Type, char Port, char Channel, char Controller, int Value);
int SendMidiPatch(PatchInfo *thePatch);
void SetAlsaCaptureVolume(long volume);
void SetAlsaMasterVolume(long volume);
void SetDAWLed(int Button, int Value);

#endif /* ALSAUTILS_H_ */

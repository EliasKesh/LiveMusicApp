/*---------------------------------------------------------------------
|
|	File: 	PrefsFile
|
|	Contains:
|
|
|	Written By: 	Elias Keshishoglou on Tue Dec 25 17:29:01 PST 2012
|
|	Copyright �: 	2012 Elias Keshishoglou all rights reserved.
|
|	This program is free software; you can redistribute it and/or
|	modify it under the terms of the GNU General Public License
|	as published by the Free Software Foundation; either version 2
|	of the License, or (at your option) any later version.
|
|	This program is distributed in the hope that it will be useful,
|	but WITHOUT ANY WARRANTY; without even the implied warranty of
|	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|	GNU General Public License for more details.
|
|	You should have received a copy of the GNU General Public License
|	along with this program; if not, write to the Free Software
|	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
|
|
|---------------------------------------------------------------------*/

#ifndef PrefsFile_h
#define PrefsFile_h

/*
 * Place Includes here.
 */

/*
 * Place Global prototypes here.
 */

void InitPref(void);
int ReadPrefs(void);
void WritePrefs(void);

/*
 * Place Static variables here.
 */
#define XML_BASIC_SETTINGS "MidiControlBase"
#define XML_SONGS_BASE_PATH "BaseSongPath"

#endif

/*---------------------------------------------------------------------
|
|	File: 	HTML.c
|
|	Contains: Routines for reading charts
|		and webkit interactions.
|
|
|	Written By: 	Elias Keshishoglou
|  	
|	Created on: Dec 25, 2012
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
|
|---------------------------------------------------------------------*/
/*
 * HTML.h
 *
|  Created on: Dec 25, 2012
|      Author: elias
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
*/

#ifndef HTML_H_
#define HTML_H_

void InitHTML(GtkBuilder *gxml);
void  OpenSetListSong(int SongNumber);
int ScrollDown(int	Amount);

#endif /* HTML_H_ */

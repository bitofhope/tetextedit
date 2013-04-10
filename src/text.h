/*
	text.h : text structure by lines.
	(C)2012-2013 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#ifndef TEXT_H
#define TEXT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char** gettext( void );
unsigned long int getlen( void );
void freelines( void );
int readlines( FILE *dict );
#endif

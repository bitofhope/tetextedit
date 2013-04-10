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
int lineslen( char **where );
char *mkline( const char *content );
char** insline( char **where, int at, char *ln );
void chline( char **where, int at, const char *content );
char** rmline( char **where, int at );
void freetext( char **where );
char **readlines( FILE *from );
#endif

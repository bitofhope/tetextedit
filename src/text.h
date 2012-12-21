/*
	text.h : text structure by lines.
	(C)2012 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#ifndef TEXT_H
#define TEXT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct line
{
	struct line *prev, *next;
	char *text;
} line_t;

line_t *mkline( const char *content );
void insline( line_t *at, line_t *ln );
void chline( line_t *at, const char *content );
void rmline( line_t *at );
line_t *lineat( line_t *lns, int pos );
line_t *lastline( line_t *lns );
void freetext( line_t *txt );
line_t *readlines( FILE *from );

#endif

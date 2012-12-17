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

line_t *mkline( line_t *at, const char *content );
void chline( line_t *at, const char *content );
void rmline( line_t *at );
line_t *lineat( line_t *lns, int pos );
line_t *lastline( line_t *lns );
void freetext( line_t *txt );
int readlines( FILE *from, line_t *to );

#endif

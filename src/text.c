/*
	text.c : text structure by lines.
	(C)2012 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include "text.h"

line_t *mkline( line_t *at, const char *content )
{
	line_t *other;
	other = at->next;
	line_t *new;
	new = malloc(sizeof(line_t));
	at->next = new;
	new->prev = at;
	new->next = other;
	other->prev = new;
	new->text = malloc(2);
	strcpy(new->text,(content)?content:"\n");
	return new;
}

void chline( line_t *at, const char *content )
{
	at->text = realloc(at->text,(content)?strlen(content+1):2);
	strcpy(at->text,(content)?content:"\n");
}

void rmline( line_t *at )
{
	at->prev->next = at->next;
	at->next->prev = at->prev;
	free(at->text);
	free(at);
}

line_t *lineat( line_t *lns, int pos )
{
	line_t *ret;
	int i = 0;
	for ( ret=lns; ret; ret=ret->next )
		if ( (i++ >= pos) || (!ret->next) )
			return ret;
	return NULL;	/* shouldn't happen */
}

line_t *lastline( line_t *lns )
{
	line_t *ret;
	for ( ret=lns; ret; ret=ret->next )
		if ( !ret->next )
			return ret;
	return NULL;	/* shouldn't happen */
}

void freetext( line_t *txt )
{
	line_t *lines;
	lines = lastline(txt);
	while ( lines )
	{
		if ( lines->text )
			free(lines->text);
		if ( lines->prev )
		{
			lines = lines->prev;
			free(lines->next);
		}
		else
		{
			free(lines);
			break;
		}
	}
}

int readlines( FILE *from, line_t *to )
{
	return 1;
}

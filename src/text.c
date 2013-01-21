/*
	text.c : text structure by lines.
	(C)2012-2013 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include "text.h"

line_t *mkline( const char *content )
{
	line_t *new;
	new = malloc(sizeof(line_t));
	new->prev = NULL;
	new->next = NULL;
	new->text = (content)?malloc(strlen(content+1)):malloc(2);
	strcpy(new->text,(content)?content:"");
	return new;
}

void insline( line_t *at, line_t *ln )
{
	if ( !at )
		return;
	line_t *other;
	other = at->next;
	at->next = ln;
	ln->prev = at;
	ln->next = other;
	if ( other )
		other->prev = ln;
}

void chline( line_t *at, const char *content )
{
	if ( at->text )
		at->text = realloc(at->text,(content)?strlen(content+1):2);
	else
		at->text = malloc((content)?strlen(content+1):2);
	strcpy(at->text,(content)?content:"");
}

line_t *rmline( line_t *at )
{
	line_t *ret = NULL;
	if ( at->prev )
		at->prev->next = at->next;
	if ( at->next )
		at->next->prev = at->prev;
	if ( at->next )
		ret = at->next;
	free(at->text);
	free(at);
	return ret;
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
	line_t *lines, *old;
	lines = lastline(txt);
	while ( lines )
	{
		old = lines;
		lines = old->prev;
		free(old->text);
		free(old);
	}
}

line_t *readlines( FILE *from )
{
	char *buf = NULL;
	line_t *to = NULL;
	line_t *cur = NULL;
	int i = 0, j = 0, ch = 0;
	line_t *nl = to;
	fseek(from,j,SEEK_SET);
	do
	{
		ch = fgetc(from);
		if ( (ch == EOF) && (i == 0) )
		{
			to = mkline(NULL);
			return to;
		}
		if ( ch != '\n' )
		{
			i++;
			continue;
		}
		buf = malloc((i-j)+1);
		fseek(from,j,SEEK_SET);
		fgets(buf,(i-j)+1,from);
		nl = mkline(buf);
		if ( !to )
		{
			to = nl;
			cur = to;
		}
		else
		{
			insline(cur,nl);
			cur = nl;
		}
		free(buf);
		buf = NULL;
		j = i+1;
		fseek(from,i+1,SEEK_SET);
		i++;
	}
	while ( ch != EOF );
	return to;
}

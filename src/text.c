/*
	text.c : text structure by lines.
	(C)2012-2013 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include "text.h"

int lineslen( char **where )
{
	int i = 0;
	while ( *(where++) )
		i++;
	return i;
}

char *mkline( const char *content )
{
	char *new;
	new = (content)?malloc(strlen(content+1)):malloc(2);
	strcpy(new,(content)?content:"");
	return new;
}

char** insline( char **where, int at, char *ln )
{
	if ( !where || !ln )
		return NULL;
	int len = lineslen(where);
	where = (char**)realloc(where,sizeof(char*)*(len+1));
	memmove(where+at+1,where+at,sizeof(char*)*(len-at));
	where[at] = ln;
	where[len] = NULL;
	return where;
}

void chline( char **where, int at, const char *content )
{
	if ( !where )
		return;
	if ( where[at] )
		where[at] = realloc(where[at],(content)?strlen(content+1):2);
	else
		where[at] = malloc((content)?strlen(content+1):2);
	strcpy(where[at],(content)?content:"");
}

char** rmline( char **where, int at )
{
	if ( !where )
		return NULL;
	int len = lineslen(where);
	memmove(where+at,where+at+1,sizeof(char*)*(len-at));
	where = (char**)realloc(where,sizeof(char*)*(len+1));
	where[len-1] = NULL;
	return where;
}

void freetext( char **where )
{
	if ( !where )
		return;
	while ( *(where++) )
	{
		printf("freeing line\n");
		free(*where);
	}
}

char **readlines( FILE *from )
{
	char *buf = NULL;
	char **to = NULL;
	char *nl;
	int i = 0, j = 0, ch = 0, k = 0;
	fseek(from,j,SEEK_SET);
	to = malloc(sizeof(char*)*2);
	do
	{
		ch = fgetc(from);
		if ( (ch == EOF) && (i == 0) )
		{
			to[0] = NULL;
			return to;
		}
		if ( ch != '\n' )
		{
			i++;
			continue;
		}
		buf = malloc((i-j)+2);
		fseek(from,j,SEEK_SET);
		fgets(buf,(i-j)+2,from);
		nl = mkline(buf);
		to = (char**)realloc(to,sizeof(char**)*(k+2));
		to[k] = nl;
		to[++k] = NULL;
		free(buf);
		j = i+1;
		fseek(from,i+1,SEEK_SET);
		i++;
	}
	while ( ch != EOF );
	return to;
}

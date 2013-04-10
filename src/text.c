/*
	text.c : text structure by lines.
	(C)2012-2013 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include "text.h"

char **text = NULL;
unsigned long int textlen = 0;

char** gettext( void )
{
	return text;
}

unsigned long int getlen( void )
{
	return textlen;
}

void freelines( void )
{
	unsigned long int i;
	for ( i=0; i<textlen; i++ )
		free(text[i]);
	free(text);
}

static void setentry( unsigned long int idx, const char *ln )
{
	if ( idx >= textlen )
		return;
	if ( text == NULL )
		return;
	if ( text[idx] != NULL )
		free(text[idx]);
	text[idx] = malloc(strlen(ln)+1);
	strcpy(text[idx],ln);
}

static void increasedict( void )
{
	if ( text == NULL )
	{
		text = malloc(sizeof(char*));
		text[0] = NULL;
	}
	else
	{
		text = realloc(text,sizeof(char*)*(textlen+1));
		text[textlen] = NULL;
	}
}

static char* readline( FILE *dict )
{
	unsigned long int i = 0;
	int ch = 0;
	char *line = NULL;
	line = malloc(2);
	while ( !feof(dict) )
	{
		ch = fgetc(dict);
		if ( ch == '\n' )
		{
			line = realloc(line,i+1);
			line[i] = '\0';
			break;
		}
		if ( feof(dict) )
			break;
		line = realloc(line,i+2);
		line[i] = ch;
		line[++i] = '\0';
	}
	return line;
}

int readlines( FILE *dict )
{
	unsigned long int i = 0;
	char *got = NULL;
	if ( dict == NULL )
		return 1;
	while ( !feof(dict) )
	{
		increasedict();
		got = readline(dict);
		if ( got == NULL )
			break;
		textlen++;
		setentry(i++,got);
		free(got);
		got = NULL;
	}
	return 0;
}

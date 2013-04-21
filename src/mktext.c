/* mktext.c : make text array from file -- (C)Marisa Kirisame - MIT Licensed */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* global vars */
char **text = NULL;
unsigned textlen = 0;

/* retrieve array */
char** gettext( void )
{
	return text;
}

/* retrieve length */
unsigned getlen( void )
{
	return textlen;
}

/* deallocate text */
void freelines( void )
{
	unsigned long int i;
	for ( i=0; i<textlen; i++ )
		free(text[i]);
	free(text);
}

/* make line */
static void setentry( unsigned idx, const char *ln )
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

/* increment array */
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

/* read a line */
static char* readline( FILE *dict )
{
	unsigned i = 0;
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

/* read text */
int readlines( FILE *dict )
{
	unsigned i = 0;
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

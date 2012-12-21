/*
	te.c : te.
	(C)2012 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "text.h"

FILE *doc = NULL;
line_t *lines = NULL;
int linec = 0;

int err( const char *mesg, ... )
{
	va_list args;
	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);
	return 1;
}

int parsecmd(const char *cmd)
{
	if ( !strcmp(cmd,"q") )
	{
		printf("bye\n");
		return 0;
	}
	if ( !strcmp(cmd,"te") )
	{
		printf("tetete\n");
		return 1;
	}
	if ( !strcmp(cmd,"p") )
	{
		line_t *lin;
		for ( lin = lines; lin; lin = lin->next )
			printf("%s\n",lin->text);
		return 1;
	}
	if ( cmd[0] == 'l' )
	{
		int pos = atoi(cmd+1);
		if ( (pos < 0) || (pos >= linec) )
			printf("error: out of range\n");
		else
			printf("%s\n",lineat(lines,pos)->text);
		return 1;
	}
	printf("?\n");
	return 1;
}

/* base skeleton, will extend later */
int main( int argc, char **argv )
{
	if ( argc > 1 )
	{
		if ( (doc = fopen(argv[1],"r")) == NULL )
			doc = fopen(argv[1],"w");
		if ( !doc )
			return err("te: could not create \"%s\"\n",argv[1]);
		fclose(doc);
		if ( (doc = fopen(argv[1],"r")) == NULL )
			return err("te: could not read \"%s\"\n",argv[1]);
	}
	lines = readlines(doc);
	char cmd[256];
	int ch = 0;
	line_t *cur;
	for ( cur = lines; cur; cur = cur->next )
		linec++;
	if ( linec > 0 )
		printf("read %d lines from \"%s\".\n",linec,argv[1]);
	for(;;)
	{
		printf(": ");
		int i = 0;
		while ( (i < 256) && (ch != '\n') )
		{
			ch = getchar();
			cmd[i] = (char)ch;
			i++;
		}
		cmd[i-1] = 0;
		if ( !parsecmd(cmd) )
			break;
		ch = 0;
		i = 0;
	}
	fclose(doc);
	freetext(lines);
	return 0;
}

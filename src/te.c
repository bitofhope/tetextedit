/*
	te.c : te.
	(C)2012-2013 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "text.h"

const char *_te_ver = "0.0.1";

char *filen = NULL;
FILE *doc = NULL;
char **lines = NULL;
int linec = 0;
int cur = 0;

int err( const char *mesg, ... )
{
	va_list args;
	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);
	return 1;
}

void savefileas( void )
{
	int i = 0;
	char *fname = malloc(2);
	int ch = getchar();
	while ( ch != '\n' )
	{
		fname[i] = ch;
		fname = realloc(fname,(++i)+1);
		ch = getchar();
	}
	fname[i] = 0;
	FILE *fil = fopen(fname,"w");
	if ( !fil )
	{
		err("te: could not open \"%s\"",fname);
		free(fname);
		return;
	}
	char **lin;
	for ( lin = lines; lin; lin++ )
		fprintf(fil,"%s\n",*lin);
	fclose(fil);
	free(fname);
}

void savefile( char *fname )
{
	FILE *fil = fopen(fname,"w");
	if ( !fil )
	{
		err("te: could not open \"%s\"",fname);
		return;
	}
	char **lin;
	for ( lin = lines; lin; lin++)
		fprintf(fil,"%s\n",*lin);
	fclose(fil);
}

int parsecmd( const char *cmd )
{
	if ( !strcmp(cmd,"q") )
	{
		return 0;
	}
	if ( !strcmp(cmd,"v") )
	{
		printf("te: version %s\n",_te_ver);
		return 1;
	}
	if ( !strcmp(cmd,"p") )
	{
		char **lin;
		for ( lin = lines; *lin; lin++ )
			printf("%s\n",*lin);
		return 1;
	}
	if ( !strcmp(cmd,"lc") )
	{
		printf("%d\n",linec);
		return 1;
	}
	if ( !strcmp(cmd,"sa") )
	{
		savefileas();
		return 1;
	}
	if ( !strcmp(cmd,"s") )
	{
		savefile(filen);
		return 1;
	}
	printf("?\n");
	return 1;
}

int main( int argc, char **argv )
{
	if ( argc <= 1 )
		return err("te: no file specified\n");
	if ( (doc = fopen(argv[1],"r")) == NULL )
		doc = fopen(argv[1],"w");
	if ( !doc )
		return err("te: could not create \"%s\"\n",argv[1]);
	fclose(doc);
	if ( (doc = fopen(argv[1],"r")) == NULL )
		return err("te: could not read \"%s\"\n",argv[1]);
	lines = readlines(doc);
	fclose(doc);
	filen = argv[1];
	char cmd[256];
	int ch = 0;
	linec = lineslen(lines);
	cur = linec-1;
	for ( ; ; )
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
	freetext(lines);
	free(lines);
	return 0;
}

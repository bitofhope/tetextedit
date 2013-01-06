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

const char *_te_ver = "0.0.4";

char *filen = NULL;
FILE *doc = NULL;
line_t *lines = NULL;
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
	line_t *lin;
	for ( lin = lines; lin; lin = lin->next )
		fprintf(fil,"%s\n",lin->text);
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
	line_t *lin;
	for ( lin = lines; lin; lin = lin->next )
		fprintf(fil,"%s\n",lin->text);
	fclose(fil);
}

void instext_tok( int pos, char tok )
{
	for ( ; ; )
	{
		int i = 0;
		char *buf = malloc(2);
		int ch = getchar();
		if ( ch == tok )
		{
			while ( getchar() != '\n' );	/* ignore anything until a newline */
			free(buf);
			break;
		}
		while ( ch != '\n' )
		{
			buf[i] = ch;
			buf = realloc(buf,(++i)+1);
			ch = getchar();
		}
		buf[i] = 0;
		if ( (linec == 1) && (cur == 0) && !lines->text[0] )	/* small hack for empty files */
			chline(lines,buf);
		else
		{
			line_t *toinsert = mkline(buf);
			insline(lineat(lines,cur),toinsert);
			cur++;
			linec++;
		}
		free(buf);
	}
}

void instext( int pos, int count )
{
	while ( count-- )
	{
		int i = 0;
		char *buf = malloc(2);
		int ch = getchar();
		while ( ch != '\n' )
		{
			buf[i] = ch;
			buf = realloc(buf,(++i)+1);
			ch = getchar();
		}
		buf[i] = 0;
		if ( (linec == 1) && (cur == 0) && !lines->text[0] )	/* small hack for empty files */
			chline(lines,buf);
		else
		{
			line_t *toinsert = mkline(buf);
			insline(lineat(lines,cur),toinsert);
			cur++;
			linec++;
		}
		free(buf);
	}
}

void edttext( int pos )
{
	int i = 0;
	char *buf = malloc(2);
	int ch = getchar();
	while ( ch != '\n' )
	{
		buf[i] = ch;
		buf = realloc(buf,(++i)+1);
		ch = getchar();
	}
	buf[i] = 0;
	chline(lineat(lines,cur),buf);
	free(buf);
	return;
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
		line_t *lin;
		for ( lin = lines; lin; lin = lin->next )
			printf("%s\n",lin->text);
		return 1;
	}
	if ( !strcmp(cmd,"lc") )
	{
		printf("%d\n",linec);
		return 1;
	}
	if ( cmd[0] == 'l' )
	{
		if ( cmd[1] == 0 )
		{
			printf("%s\n",lineat(lines,cur)->text);
			return 1;
		}
		if ( (cmd[1] < '0') || (cmd[1] > '9') )
		{
			printf("te: not a number\n");
			return 1;
		}
		int pos = atoi(cmd+1);
		if ( (pos < 0) || (pos >= linec) )
			printf("te: out of range\n");
		else
			printf("%s\n",lineat(lines,pos)->text);
		return 1;
	}
	if ( cmd[0] == 'm' )
	{
		if ( cmd[1] == 0 )
		{
			printf("%d\n",cur);
			return 1;
		}
		if ( cmd[1] == 'e' )
		{
			cur = linec;
			return 1;
		}
		if ( (cmd[1] < '0') || (cmd[1] > '9') )
		{
			printf("te: not a number\n");
			return 1;
		}
		int pos = atoi(cmd+1);
		if ( (pos < 0) || (pos >= linec) )
			printf("te: out of range\n");
		else
			cur = pos;
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
	if ( cmd[0] == 'i' )
	{
		if ( cmd[1] == 0 )
		{
			instext(cur,1);
			return 1;
		}
		if ( (cmd[1] == 't') && cmd[2] )
		{
			instext_tok(cur,cmd[2]);
			return 1;
		}
		if ( (cmd[1] < '0') || (cmd[1] > '9') )
		{
			printf("te: not a number\n");
			return 1;
		}
		int c = atoi(cmd+1);
		if ( c <= 0 )
			printf("te: out of range\n");
		else
			instext(cur,c);
		return 1;
	}
	if ( !strcmp(cmd,"e") )
	{
		edttext(cur);
		return 1;
	}
	if ( !strcmp(cmd,"r") )
	{
		if ( cur == 0 )
			lines = rmline(lines);
		else
		{
			rmline(lineat(lines,cur));
		}
		linec--;
		if ( cur >= linec )
			cur--;
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
	if ( (doc = fopen(argv[1],"r+")) == NULL )
		return err("te: could not read \"%s\"\n",argv[1]);
	lines = readlines(doc);
	fclose(doc);
	filen = argv[1];
	char cmd[256];
	int ch = 0;
	line_t *lin;
	for ( lin = lines; lin; lin = lin->next )
		linec++;
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
	return 0;
}

/*
	te.c : te.
	(C)2012-2013 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include "text.h"
#include <stdarg.h>

static const char *_te_ver = "0.0.1";

static char *filen = NULL;
static FILE *doc = NULL;
static char **lines = NULL;
static int linec = 0;
static int cur = 0;
static int terrno = 0;
static int verrmsg = 0;

static void lasterror( void )
{
	switch ( terrno )
	{
	case 0:
		puts("No error");
		break;
	case 1:
		puts("Out of range");
		break;
	case 2:
		puts("Not a number");
		break;
	case 3:
		puts("Out of memory");
		break;
	case 4:
		puts("Nothing to insert");
		break;
	case 5:
		puts("Invalid search format");
		break;
	case 6:
		puts("Internal error");
		break;
	case 7:
		puts("Unrecognized command");
		break;
	case 8:
		puts("tetete");
		break;
	case 9:
		puts("Could not open file for reading");
		break;
	default:
		puts("Unknown error");
		break;
	}
}

static int err( unsigned long int errno )
{
	terrno = errno;
	if ( verrmsg )
		lasterror();
	else
		puts((errno==8)?":3?":"?");
	return 1;
}

static int bail( const char *mesg, ... )
{
	va_list args;
	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);
	return 1;
}

static int savefileas( void )
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
		free(fname);
		return err(9);
	}
	for ( i = 0; i<linec; i++ )
		fprintf(fil,"%s\n",lines[i]);
	fclose(fil);
	free(fname);
	return 0;
}

static int savefile( char *fname )
{
	FILE *fil = fopen(fname,"w");
	if ( !fil )
		return err(9);
	int i;
	for ( i = 0; i<linec; i++ )
		fprintf(fil,"%s\n",lines[i]);
	fclose(fil);
	return 0;
}

static int parsecmd( const char *cmd )
{
	if ( !strcmp(cmd,":3") )
		return err(8);
	if ( !strcmp(cmd,"q") )
		return 0;
	if ( !strcmp(cmd,"E") )
	{
		verrmsg = !verrmsg;
		return 1;
	}
	if ( !strcmp(cmd,"e") )
	{
		lasterror();
		return 1;
	}
	if ( !strcmp(cmd,"v") )
	{
		printf("te: version %s\n",_te_ver);
		return 1;
	}
	if ( !strcmp(cmd,"p") )
	{
		int i;
		for ( i = 0; i<linec; i++ )
			printf("%s\n",lines[i]);
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
			printf("%s\n",lines[cur]);
			return 1;
		}
		if ( (cmd[1] < '0') || (cmd[1] > '9') )
			return err(2);
		int pos = atoi(cmd+1);
		if ( (pos < 0) || (pos > linec) )
			return err(1);
		printf("%s\n",lines[pos]);
		return 1;
	}
	if ( cmd[0] == 's' )
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
			return err(2);
		int pos = atoi(cmd+1);
		if ( (pos < 0) || (pos > linec) )
			return err(1);
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
	return err(7);
}

int main( int argc, char **argv )
{
	if ( argc <= 1 )
		return bail("te: no file specified\n");
	if ( (doc = fopen(argv[1],"r")) == NULL )
		doc = fopen(argv[1],"w");
	if ( !doc )
		return bail("te: could not create \"%s\"\n",argv[1]);
	fclose(doc);
	if ( (doc = fopen(argv[1],"r")) == NULL )
		return bail("te: could not read \"%s\"\n",argv[1]);
	readlines(doc);
	lines = gettext();
	linec = getlen()-1;
	fclose(doc);
	filen = argv[1];
	char cmd[256];
	int ch = 0;
	cur = linec;
	for ( ; ; )
	{
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
	freelines();
	return 0;
}

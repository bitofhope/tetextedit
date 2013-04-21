/* te.c : main code -- (C)Marisa Kirisame - MIT Licensed */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* prototypes from mktext */
char** gettext( void );
unsigned getlen( void );
void freelines( void );
int readlines( FILE *dict );

/* consts */
static const char *ver = "0.0.1";
static const char *errmsgs[] =
{
	"No error",
	"Out of range",
	"Not number",
	"Memory full",
	"Blank",
	"Syntax error",
	"Broken",
	"Unrecognized",
	"tetete",
	"File read fail",
	"Unknown",
};
static const unsigned errnum = 9;

/* global vars */
static char *filen = NULL;
static FILE *doc = NULL;
static char **lines = NULL;
static unsigned linec = 0;
static unsigned cur = 0;
static unsigned terrno = 0;
static unsigned verrmsg = 0;

/* error */
static const char* lasterror( void )
{
	return (terrno<errnum)?errmsgs[terrno]:errmsgs[errnum];
}

/* ? */
static int err( unsigned errno )
{
	terrno = errno;
	puts((verrmsg)?lasterror():(errno==8)?":3?":"?");
	return 1;
}

/* die */
static unsigned bail( const char *mesg, ... )
{
	va_list args;
	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);
	return 1;
}

/* save as */
static unsigned savefileas( void )
{
	unsigned i = 0;
	char fname[256];
	int ch = getchar();
	while ( (ch != '\n') && (i<255) )
	{
		fname[i++] = ch;
		ch = getchar();
	}
	fname[i] = 0;
	FILE *fil = fopen(fname,"w");
	if ( !fil )
		return err(9);
	for ( i = 0; i<linec; i++ )
		fprintf(fil,"%s\n",lines[i]);
	fclose(fil);
	return 0;
}

/* save */
static unsigned savefile( char *fname )
{
	FILE *fil = fopen(fname,"w");
	if ( !fil )
		return err(9);
	unsigned i;
	for ( i = 0; i<linec; i++ )
		fprintf(fil,"%s\n",lines[i]);
	fclose(fil);
	return 0;
}

/* spaghetti */
static unsigned parsecmd( const char *cmd )
{
	/* te */
	if ( !strcmp(cmd,":3") )
		return err(8);
	/* bye */
	if ( !strcmp(cmd,"q") )
		return 0;
	/* verbose */
	if ( !strcmp(cmd,"E") )
	{
		verrmsg = !verrmsg;
		return 1;
	}
	/* what happen */
	if ( !strcmp(cmd,"e") )
	{
		puts(lasterror());
		return 1;
	}
	/* version */
	if ( !strcmp(cmd,"v") )
	{
		puts(ver);
		return 1;
	}
	/* print */
	if ( !strcmp(cmd,"p") )
	{
		unsigned i;
		for ( i = 0; i<linec; i++ )
			printf("%s\n",lines[i]);
		return 1;
	}
	/* count */
	if ( !strcmp(cmd,"lc") )
	{
		printf("%d\n",linec);
		return 1;
	}
	/* line print */
	if ( cmd[0] == 'l' )
	{
		if ( cmd[1] == 0 )
		{
			printf("%s\n",lines[cur]);
			return 1;
		}
		if ( (cmd[1] < '0') || (cmd[1] > '9') )
			return err(2);
		unsigned pos = atoi(cmd+1);
		if ( pos > linec )
			return err(1);
		printf("%s\n",lines[pos]);
		return 1;
	}
	/* move cursor */
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
			return err(2);
		unsigned pos = atoi(cmd+1);
		if ( pos > linec )
			return err(1);
		cur = pos;
		return 1;
	}
	/* save */
	if ( cmd[0] == 's' )
	{
		(cmd[1]=='a')?savefileas():savefile(filen);
		return 1;
	}
	/* unrecognized */
	return err(7);
}

/* entry */
int main( int argc, char **argv )
{
	/* nofile */
	if ( argc <= 1 )
		return bail("te: no file\n");
	/* create if not exist */
	if ( (doc = fopen(argv[1],"r")) == NULL )
		doc = fopen(argv[1],"w");
	/* something broke */
	if ( !doc )
		return bail("te: cannot make \"%s\"\n",argv[1]);
	fclose(doc);
	/* cannot open for read */
	if ( (doc = fopen(argv[1],"r")) == NULL )
		return bail("te: cannot read \"%s\"\n",argv[1]);
	/* make array */
	readlines(doc);
	lines = gettext();
	linec = getlen()-1;
	fclose(doc);
	filen = argv[1];
	char cmd[256];
	int ch = 0;
	cur = linec;
	/* main loop */
	for ( ; ; )
	{
		unsigned i = 0;
		/* read expression */
		while ( (i < 256) && (ch != '\n') )
		{
			ch = getchar();
			cmd[i] = (char)ch;
			i++;
		}
		cmd[i-1] = 0;
		if ( !parsecmd(cmd) )	/* zero means close */
			break;
		ch = 0;
		i = 0;
	}
	/* endo */
	freelines();
	return 0;
}

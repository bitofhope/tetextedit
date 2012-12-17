/*
	te.c : te.
	(C)2012 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "text.h"

int err( const char *mesg, ... )
{
	va_list args;
	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);
	return 1;
}

/* base skeleton, will extend later */
int main( int argc, char **argv )
{
	FILE *doc = NULL;
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
	for(;;)
	{
		while ( getchar() != '\n' );
		printf("?\n");
	}
	fclose(doc);
	return 0;
}

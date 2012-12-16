/*
	te.c : te.
	(C)2012 Marisa Kirisame, UnSX Team.
	te is a text editor.
	Released under the MIT License.
*/
#include <stdio.h>

/* base skeleton, will extend later */
int main( int argc, char **argv )
{
	for(;;)
	{
		while ( getchar() != '\n' );
		printf("?\n");
	}
	return 0;
}

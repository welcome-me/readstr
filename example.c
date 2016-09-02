/*
This is a quick example of how to use readstr.h to read a string of unlimited
(memory and size_t permitting) size.

You can run `make example` or `gcc example.c -o example` to compile this example.
*/

#include "readstr.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
	
	//create char* for readstr().
	//readstr takes a pointer to a char* so that it can return the amount of characters
	//read in AND so that its more obvious that you have to free() the string that
	//it reads in.
	char *str = 0;
	
	//The following declarations are unrelated to readstr.h
	int linesread = 0,
		charsread = 0;
	
	/*
	Loop until we reach EOF. readstr() returns the total amount of chars
	read in including the line terminator. It returns 0 if EOF was reached.
	*/
	while (readstr(
			stdin,	// <-- stream to read from. Must be of type FILE *
			&str,	// <-- pointer to a char* pointer. If str is already
					// pointing to manually allocated memory,
					// this will be a memory leak!!!
			'\n'	// <-- char that represents the end of a line.
			))
		{
		
		//increment line counter
		++linesread;
		
		//this would normally be calculated in the loop. However, the loop
		//already looks messy enough.
		charsread = strlen(str);
		
		//print some info
		//Note that we have to print a line terminator because readstr removes the
		//line terminator from the string.
		printf("Line #%i was %3i character%slong excluding the line terminator.\n", linesread, charsread, charsread == 1 ? " " : "s ");
		
		//Very Important! We have to free the string when we are done with it and
		//readstr()'s return value is >= 1!
		free(str);
		}
	
	return 0;
	}

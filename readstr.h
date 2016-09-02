/*
This header file has the simple job of giving the includer of it a way to read
in a string of unlimited size (memory permitting) from a FILE pointer (eg. stdin).
The line terminating character is removed from the string.
*/

#ifndef READSTR_H_INCLUDED
#define READSTR_H_INCLUDED

//READBUFLEN *MUST* be >= 3
#define READBUFLEN 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t _readchrs(FILE *fin, char buf[READBUFLEN], char lter) {
	/*
	This reads at most READBUFLEN-1 characters into buf. If we read lter or EOF
	before reading in READBUFLEN-1 characters, we terminate buf with a lter and a
	'\0.' We return the amount of characters read in, including our lter, if applicable.
	We return 0 if there was nothing to read (ie. we reached EOF).
	
	!This is for internal use! !Note the leading underscore...!
	*/
	
	char curch;
	size_t charsreadin = 0;
	
	//read till lter or READBUFLEN-2 characters
	while (charsreadin < READBUFLEN-2) {
		curch = getc(fin);
		//we read in a valid character
		if (curch != EOF && curch != lter) {
			buf[charsreadin] = curch;
			++charsreadin;
			continue;
			}
		
		//we're done reading this line
		else if (curch == lter) {
			buf[charsreadin] = lter;
			buf[charsreadin+1] = 0;
			++charsreadin;
			break;
			}
		
		//we reached our EOF but we've already read in stuff.
		//Add an automatic lter and push a EOF back onto fin.
		else if (curch == EOF && charsreadin > 0) {
			buf[charsreadin] = lter;
			buf[charsreadin+1] = 0;
			ungetc(EOF, fin);
			break;
			}
		
		//we couldn't read in anything
		else {
			ungetc(EOF, fin);
			return 0;
			}
		}
	
	return charsreadin;
	}

size_t readstr(FILE *fin, char **str, char lter) {
	/*
	This reads in from *fin 'til it reaches a lter character. The lter character
	is removed and *str is pointed to the readin str. Don't forget to free *str
	when you're done with it!
	It returns the total amount of characters read, including lter, or 0 if we reached EOF.
	
	NOTE: If 0 characters are read in (ie. we reach EOF immediately), space is not
	allocated. In simple terms, *str does not need free()d if readstr() returns 0.
	*/
	
	char buf[READBUFLEN] = {};
	char *tmp1;
	size_t allocsize = 0, chreadin = 0, totreadin = 0;
	*str = tmp1 = NULL;
	
	while ((chreadin = _readchrs(fin, buf, lter)) != 0) {
		totreadin += chreadin;
		
		//calculate how much memory we must allocate to accomodate the new str
		allocsize = 1;
		allocsize += strlen(buf);/*XXX: 1*/
		if (*str != NULL)
			allocsize += strlen(*str);
		
		//allocate room for our new str
		tmp1 = malloc(allocsize);
		
		//set tmp[0] to '\0' so strcat knows how long tmp1 is
		tmp1[0] = 0;
		if (*str != NULL) {
			strcat(tmp1, *str);
			}
		strcat(tmp1, buf);
		
		//we're done with the old str
		if (*str != NULL)
			free(*str);
		*str = tmp1;
		tmp1 = NULL;
		
		//we didn't reach our line terminator
		if (buf[strlen(buf)-1] != lter) {
			continue;
			}
		
		//we're finished
		else {
			//remove terminating lter AND make **str NULL terminated
			(*str)[allocsize-2] = 0;
			break /*from while loop*/;
			}
		}
	
	//finished!
	return totreadin;
	}

#undef READBUFLEN //no point in leaving READBUFLEN hangin around.
#endif //READSTR_H_INCLUDED

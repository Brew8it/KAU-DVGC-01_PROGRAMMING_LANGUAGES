/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	
/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
        {"id", 	        id},
	{"number", 	number},
	{":=", 	        assign},
	{"undef", 	undef},
	{"predef", 	predef},
	{"tempty", 	tempty},
	{"error",        error},
	{"type",         typ},
	{"$",            '$'},
	{"(",            '('},
	{")",            ')'},
	{"*",            '*'},
	{"+",            '+'},
	{",",            ','},
	{"-",            '-'},
	{".",            '.'},
	{"/",            '/'},
	{":",            ':'},
	{";",            ';'},
	{"=",            '='},
	{"TERROR", 	nfound}
        };


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	input},
	{"output", 	output},
	{"var", 	var},
	{"begin", 	begin},
	{"end", 	end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	real},
	{"KERROR", 	nfound}
	} ;
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{
	int i = 0;
	printf("\n________________________________________________________\n");
	printf("THE PROGRAM KEYWORD");
	printf("\n________________________________________________________\n");
	while(!strcmp(keywordtab[i].text,"KERROR") == 0){
		printf("\t%s\t%d\n",keywordtab[i].text, keywordtab[i].token);
		i++;
	}
	i= 0;
	printf("\n________________________________________________________\n");
	printf("THE PROGRAM TOKENS");
	printf("\n________________________________________________________\n");
	while(!strcmp(tokentab[i].text,"TERROR") == 0){
		printf("\t%s \t %d\n",tokentab[i].text, tokentab[i].token);
		i++;
	}
	printf("\n________________________________________________________\n");
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
	int i = 0;
	while(tokentab[i].token != nfound){
		if (strcmp(fplex,tokentab[i].text) == 0)
		{
			return tokentab[i].token;
		}
		i++;
	}
	i = 0;
	while(keywordtab[i].token != nfound){
		if (strcmp(fplex,keywordtab[i].text) == 0)
		{
			return keywordtab[i].token;
		}
		i++;
	}
	return id;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
	int i = 0;
	while(keywordtab[i].token != nfound){
		if (strcmp(fplex,keywordtab[i].text) == 0)
		{
			return keywordtab[i].token;
		}
		i++;
	}

	return id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
	int i = 0;
	while(tokentab[i].token != nfound){
		if (ftok == tokentab[i].token)
		{
			return tokentab[i].text;
		}
		i++;
	}
	i = 0;
	while(keywordtab[i].token != nfound){
		if(ftok == keywordtab[i].token){
			return keywordtab[i].text;
		}
		i++;
	}
	return tokentab[i].text;
}


/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/


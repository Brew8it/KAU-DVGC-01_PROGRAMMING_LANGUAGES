/**********************************************************************/
/* lab 1 DVG C01 - Operator Table OBJECT                              */
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
#define NENTS 4

static int optab[][NENTS] = {
   {'+', integer, integer, integer},
   {'+', real,    real,    real},
   {'+', integer, real,    real},
   {'+', real,    integer, real},
   {'*', integer, integer, integer},
   {'*', real,    real,    real},
   {'*', integer, real,    real},
   {'*', real,    integer, real},
   {'$', undef,   undef,   undef}
   };

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static int find_result_optab(int op, int arg1, int arg2){
   int i = 0;
   while(optab[i][0] != '$'){
      if (optab[i][0] == op && optab[i][1] == arg1 && optab[i][2] == arg2)
      {
         return i;
      }
      i++;
   }
   return undef;
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* display the op tab                                                 */
/**********************************************************************/
void p_optab()
{
   int i = 0, j = 0;
   printf("\n________________________________________________________");
   printf("\n  THE OPERATOR TABLE");
   printf("\n________________________________________________________");
   printf("\n%10s%10s%10s%10s\n","operator","arg1","arg2", "result");
   while(optab[i][0] != '$'){
      j = 0;
      while(j < NENTS){
         printf("%10s",tok2lex(optab[i][j]));
         j++;
     }
     printf("\n");
     i++;
   }
   printf("________________________________________________________");
}

/**********************************************************************/
/* return the type of a binary expression op arg1 arg2                */
/**********************************************************************/
int get_otype(int op, int arg1, int arg2)
{  
   int Rowindex = find_result_optab(op,arg1,arg2);
   if (Rowindex != undef){ return optab[Rowindex][NENTS - 1];   }
   else{ return undef;  }
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

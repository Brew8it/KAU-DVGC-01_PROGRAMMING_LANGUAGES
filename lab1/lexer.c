/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE   30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int  pbuf  = 0;               /* current index program buffer  */
static int  plex  = 0;               /* current index lexeme  buffer  */
static int  Totalbuffer = 0; 

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/
/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog()
{
   int  i;
   memset(buffer,0,BUFSIZE);
   i=0; while ((buffer[i]=fgetc(stdin))!=EOF) i++;
   buffer[i]='$'; 
   Totalbuffer = i; 

}     

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/  

static void pbuffer()
{  
   printf("\n________________________________________________________ "); 
   printf("\n THE PROGRAM TEXT");
   printf("\n________________________________________________________ ");
   printf("\n%s", buffer);
   printf("\n________________________________________________________ ");
   
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char()
{   
   memset(lexbuf,0,LEXSIZE);
   int j = 0;    
   while(buffer[pbuf] != '$'){
      if(isalnum(buffer[pbuf])){
         while(!isspace(buffer[pbuf]) && (isalpha(buffer[pbuf]) || isdigit(buffer[pbuf]))){
            if (isdigit(buffer[pbuf]))
            {     
               lexbuf[j] = buffer[pbuf];
               pbuf++; 
               j++;
               break;
            }
            lexbuf[j] = buffer[pbuf];
            pbuf++; 
            j++;
         }
         plex = j;
         break;
      }            
      else if(isspace(buffer[pbuf])){
         while(isspace(buffer[pbuf])){
            pbuf++;
         }
      }    
      else {
         if(buffer[pbuf+1] == '='){
            lexbuf[j] = buffer[pbuf];
            lexbuf[j+1] = buffer[pbuf+1];
            pbuf++; 
            plex = j;
         }
         else{
            lexbuf[j] = buffer[pbuf];
         }
         pbuf++; 
         plex = j;
         break;
      }                    
   }       
   if (buffer[pbuf] == '$')
   {
      lexbuf[j] = buffer[pbuf];
      pbuf++;
      plex = j;
   }
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token()
{  
   if (pbuf == 0){
      get_prog();
      pbuffer();   
   }
   get_char();
   if (isdigit(lexbuf[0]))
   {
      return number;
   }
   else if(isalnum(lexbuf[0])){
      return key2tok(lexbuf);
   }
   else{
         return lex2tok(lexbuf);
      
   }   
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char * get_lexeme()
{  
   return lexbuf;
   }

/**********************************************************************/
/* Check is buff is empty.                                                    */
/**********************************************************************/
int parseEnd(){
   if (pbuf < Totalbuffer)
   {
      printf("\nSYNTAX:   Extra symbols after end of parse! \n");
      printf("          ");
      /*for (int i = pbuf - 1; i < Totalbuffer; ++i)
      {
         printf("%c",buffer[i]);
      }*/
      while(pbuf < Totalbuffer - 1){
         printf("%s ", get_lexeme());
         get_char();
      }
      return 0;
   }
   return 1;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/


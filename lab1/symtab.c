/**********************************************************************/
/* lab 1 DVG C01 - Symbol Table OBJECT                                */
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
#define TABSIZE 1024                   /* symbol table size           */
#define NAMELEN   20                   /* name length                 */

typedef char tname[NAMELEN];

static tname  name[TABSIZE];
static toktyp role[TABSIZE];
static toktyp type[TABSIZE];
static int    size[TABSIZE];
static int    addr[TABSIZE];

static int numvar = 0;                  /*numer of var that needs type*/
static int numrows=0;                  /* number of rows in the ST    */
static int startp =0;                  /* start position program in ST*/

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/*  GET methods (one for each attribute)                              */
/**********************************************************************/
static char * get_name(int ftref)   { return name[ftref]; }
static toktyp get_role(int ftref)   { return role[ftref]; }
static toktyp get_type(int ftref)   { return type[ftref]; }
static int    get_size(int ftref)   { return size[ftref]; }
static int    get_addr(int ftref)   { return addr[ftref]; }

/**********************************************************************/
/*  SET methods (one for each attribute)                              */
/**********************************************************************/
static void set_name(int ftref, char * fpname) { strcpy(name[ftref],
                                                        fpname);}
static void set_role(int ftref, toktyp frole)  { role[ftref] = frole; }
static void set_type(int ftref, toktyp ftype)  { type[ftref] = ftype; }
static void set_size(int ftref, int    fsize)  { size[ftref] = fsize; }
static void set_addr(int ftref, int    faddr)  { addr[ftref] = faddr; }

/**********************************************************************/
/*  Add a row to the symbol table                                     */
/**********************************************************************/
static void addrow(char *fname, toktyp frole, toktyp ftype, 
                   int fsize, int faddr)
{
  set_name(numrows, fname);
  set_role(numrows, frole);
  set_type(numrows, ftype);
  set_size(numrows, fsize);
  set_addr(numrows, faddr);
  numrows++;


}
/**********************************************************************/
/*  Initialise the symbol table                                       */
/**********************************************************************/

static void initst()
{
    addrow(tok2lex(predef),  typ, predef, 0, 0);
    addrow(tok2lex(undef),   typ, predef, 0, 0);
    addrow(tok2lex(error),   typ, predef, 0, 0);
    addrow(tok2lex(integer), typ, predef, 4, 0);
    addrow(tok2lex(boolean), typ, predef, 4, 0);
    addrow(tok2lex(real),    typ, predef, 8, 0);
    startp = numrows;
}

/**********************************************************************/
/*  return a reference to the ST (index) if name found else nfound    */
/**********************************************************************/
static int get_ref(char * fpname)
{
  int i = 0;
  while(i != numrows){
    if (strcmp(get_name(i),fpname) == 0)
    {
      return i;
    }
    i++; 
  }
  return 0;
}

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/*  Display the symbol table                                          */
/**********************************************************************/
static void p_symrow(int ftref)
{
  printf("%11s%11s%11s%10d%10d \n",get_name(ftref),tok2lex(get_role(ftref)),tok2lex(get_type(ftref)),get_size(ftref), get_addr(ftref)); 
}

void p_symtab()
{
  int i;
  if (numrows == 0){ i = 0;}
  else{ i = startp; }
  printf("\n________________________________________________________ ");
  printf("\n________________________________________________________ ");
  printf("\n THE SYMBOL TABLE");
  printf("\n________________________________________________________ ");
  printf("\n%11s%11s%11s%10s%10s     ","NAME","ROLE","TYPE","SIZE","ADDR");
  printf("\n________________________________________________________ \n");
  while(i != numrows){
    p_symrow(i);
    //printf("\t%s\t%s\t%s\t%d\t%d\n",get_name(i),tok2lex(get_role(i)),tok2lex(get_type(i)),get_size(i), get_addr(i));
    i++;
  } 
  printf("________________________________________________________ \n");
  printf(" STATIC STORAGE REQUIRED is %d BYTES\n", get_size(startp));
  printf("________________________________________________________ \n");
}

/**********************************************************************/
/*  Add a program name to the symbol table                            */
/**********************************************************************/
void addp_name(char * fpname) { 
  initst();
  set_name(numrows, fpname);
  set_role(numrows, program);
  set_type(numrows, program);
  set_addr(numrows, 0);
  numrows++;
}

/**********************************************************************/
/*  Add a variable name to the symbol table                           */
/******************************************************** **************/
void addv_name(char * fpname) {
  set_name(numrows,fpname);
  set_role(numrows,var);
  numvar++;
  numrows++;
}

/**********************************************************************/
/*  Find a name in the the symbol table                               */
/*  return a Boolean (true, false) if the name is in the ST           */
/**********************************************************************/
int find_name(char * fpname) {
  int nameindex = get_ref(fpname);
  if(nameindex> 0){return nameindex;} 
  else{return 0;} 
}

/**********************************************************************/
/*  Set the type of an id list in the symbol table                    */
/**********************************************************************/
void setv_type(toktyp ftype) {
  int programSize = get_size(startp);
  while(numvar != 0){
    set_type(numrows - numvar ,ftype);
    set_size(numrows - numvar, get_size(find_name(tok2lex(ftype))));
    programSize += get_size(numrows - numvar);
    set_addr(numrows - numvar,programSize - get_size(find_name(tok2lex(ftype))));
    numvar--;
  }
  set_size(startp,programSize);

}

/**********************************************************************/
/*  Get the type of a variable from the symbol table                  */
/**********************************************************************/
toktyp get_ntype(char * fpname) {
  if (find_name(fpname) > 0)
  {
    return get_type(find_name(fpname));
  }else{
    return undef;
  }
   
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

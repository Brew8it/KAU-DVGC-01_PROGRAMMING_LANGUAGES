/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
#include "keytoktab.h"          /* when the keytoktab is added   */
#include "lexer.h"             /* when the lexer     is added   */
#include "symtab.h"           /* when the symtab    is added   */
#include "optab.h"        /* when the optab     is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
#define LBUFFSIZE 20
static int  typeChecking = 1;
static int  lookahead=0;
static int  is_parse_ok=1;

/**********************************************************************/
/* RAPID PROTOTYPING - simulate the token stream & lexer (get_token)  */
/**********************************************************************/
/* define tokens + keywords NB: remove this when keytoktab.h is added */
/**********************************************************************/
// remove after stage 1
//enum tvalues { program = 257, id, input, output, var, integer, begin, assign, number, end};
/**********************************************************************/
/* Simulate the token stream for a given program                      */
/**********************************************************************/
int tokens[] = {program, id, '(', input, ',', output, ')', ';', 
                  var, id, ',', id, ',',  id, ':', integer, ';', 
                  begin, 
                     id, assign, id, '+', id, '*', number, 
                     end, '.',   
                  '$' }; 

/**********************************************************************/
/*  Simulate the lexer -- get the next token from the buffer          */
/**********************************************************************/
/*static int pget_token()
{  
   static int i=0;
   if (tokens[i] != '$') return tokens[i++]; else return '$';
   }
*/
/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/

/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static toktyp term();
static void match(int t)
{
   if (lookahead == t){
      if(DEBUG) printf("\n *** In  match      expected %s found    %s ",tok2lex(t), get_lexeme());
      if (lookahead == program)
      {
         lookahead = get_token();
         if (lookahead == '('){
            printf("\nSYNTAX:   ID expected found  %s ", get_lexeme());
            addp_name("???");
         }
         else{addp_name(get_lexeme());}
      }else{
         lookahead = get_token();   
      }
   }
   else {
      if(t != id){
         if(DEBUG)printf("\n *** In  match      ");
         printf("\nSYNTAX:   Symbol expected %s found %s ",tok2lex(t), get_lexeme()); 
         if (t == program){addp_name(get_lexeme());}  
      }
      is_parse_ok=0;
   }
}

static void type(){
   if (DEBUG) printf("\n *** In  type       ");
   if (lookahead == integer) {
      setv_type(integer);
      match(integer);
   }
   else if (lookahead == real)
   {
      setv_type(real);
      match(real);
   }
   else{
      setv_type(error);
      is_parse_ok = 0;
      printf("\nSYNTAX:   Type name expected found %s ", get_lexeme());
   }
   if (DEBUG) printf("\n *** Out type       ");
}
static void id_list(){ 
   char localbuf[LBUFFSIZE];
   strcpy(localbuf,get_lexeme());
   if (DEBUG) printf("\n *** In  id_list    ");
   if (lookahead != id){
      printf("\nSYNTAX:   ID expected found  %s ", localbuf);
      match(id);
   }else{
      match(id);
      if (find_name(localbuf)){printf("\nSEMANTIC: ID already declared: %s",localbuf);}
      else{addv_name(localbuf);}
      if (!find_name(localbuf)){printf("\nSEMANTIC: ID NOT declared: %s",localbuf);}
   }
   if (lookahead == ',')
   {
      match(',');
      id_list();
   }
   if (DEBUG) printf("\n *** Out id_list    ");
}
static toktyp expr(toktyp token){
   if (DEBUG) printf("\n *** In  expr       ");
   int sumType,type;
   int termtok = term(token);
   if (lookahead == '+')
   {
      match('+');
      sumType = get_otype('+',token,termtok);
      type = expr(sumType);
      if (DEBUG) printf("\n *** Out expr       ");
      return type;
   }
   if (DEBUG) printf("\n *** Out expr       ");
   return termtok;
}
static void var_dec(){
   if (DEBUG) printf("\n *** In  var_dec    ");
   id_list();
   match(':');
   type();
   match(';');
   if (DEBUG) printf("\n *** Out var_dec    ");
}
static void var_dec_list(){
   if (DEBUG) printf("\n *** In  var_dec_list ");
   var_dec();
   if (lookahead == id)
   {
      var_dec_list();
   }
   if (DEBUG) printf("\n *** Out var_dec_list ");
}
static toktyp operand(){
   if (DEBUG) printf("\n *** In  operand    ");
   char localbuf[LBUFFSIZE];
   strcpy(localbuf,get_lexeme());
   int opType = get_ntype(localbuf);
   if (opType == error)
   {
      opType = undef;
   }
   if (lookahead == id)
   {
      match(id);
      if (!find_name(localbuf)){
         printf("\nSEMANTIC: ID NOT declared: %s",localbuf);
         is_parse_ok = 0;
         if (DEBUG) printf("\n *** Out operand    ");
         typeChecking = 0;
         return undef;
      }
      if (DEBUG) printf("\n *** Out operand    ");
      return opType;
   }
   else if (lookahead == number)
   {
      match(number);
      if (DEBUG) printf("\n *** Out operand    ");
      return integer;
   }else{
      is_parse_ok = 0;
      printf("\nSYNTAX:   Operand Expected ");
      typeChecking = 0;
      return error;
    if (DEBUG) printf("\n *** Out operand    ");
   }
     return opType;
}
static toktyp factor(toktyp token){
   int optype = 0;
   if (DEBUG) printf("\n *** In  factor     ");
   if (lookahead == '(')
   {
      match('(');
      optype = expr(token);
      match(')');
   }else{
      optype = operand();
   }
   if (DEBUG) printf("\n *** Out factor     ");
   return optype;
}
static toktyp term(toktyp token){
   int multToken = 0;
   if (DEBUG) printf("\n *** In  term       ");
   int factype = factor(token);
   if (lookahead == '*')
   {
      match('*');
      multToken = get_otype('*',token,factype);
      term(multToken);
   }
   if (DEBUG) printf("\n *** Out term       ");
   return factype;
}
static void assign_stat(){
   if (DEBUG) printf("\n *** In  assign_stat ");
   char localbuf[LBUFFSIZE];
   strcpy(localbuf,get_lexeme());
   int type = get_ntype(get_lexeme());
   match(id);
   if (!find_name(localbuf) && lex2tok(localbuf) == id){printf("\nSEMANTIC: ID NOT declared: %s", localbuf);}
   if (!find_name(localbuf) && (lex2tok(localbuf) != id)){printf("\nSYNTAX:   ID expected found %s ",localbuf);}
   match(assign);
   int exprtype = expr(lookahead);
   if(type == undef){type = error;}
   if(!typeChecking && exprtype != error){exprtype = undef;}
   if (type != exprtype)
   {
      printf("\nSEMANTIC: Assign types: %s := %s",tok2lex(type),tok2lex(exprtype));
      is_parse_ok = 0;
   }
   if (DEBUG) printf("\n *** Out assign_stat ");
}
static void stat(){
   if (DEBUG) printf("\n *** In  stat       ");
   assign_stat();
}
static void stat_list(){
   if (DEBUG) printf("\n *** In  stat_list  ");
   stat();
   if (lookahead == ';')
   {
      match(';');
      stat_list();
   }
   if (DEBUG) printf("\n *** Out stat_list  ");
}
/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
static void program_header()
{
   if (DEBUG) printf("\n *** In  program_header ");
   match(program); match(id); match('('); match(input);
   match(','); match(output); match(')'); match(';');
   if (DEBUG) printf("\n *** Out program_header\n ");
}
static void var_part(){
   if (DEBUG) printf("\n *** In  var_part   ");
   match(var);
   var_dec_list();
   if (DEBUG) printf("\n *** Out var_part\n ");
}

static void stat_part(){
   if (DEBUG) printf("\n *** In  stat_part  ");
   match(begin);
   stat_list();
   match(end);
   match('.');
   if (DEBUG) printf("\n *** Out stat_part\n ");
}

   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser()
{
   int endCheck;
   if (DEBUG) printf("\n *** In  Parser     ");
      lookahead = get_token();         // get the first token
   if (lookahead != '$')
   {
      program_header();               // call the first grammar rule
      var_part();                     // Call the secound grammer rule.
      stat_part();                     //print symbat
   }else{
      is_parse_ok = 0;
      printf("\nSYNTAX:   Input file is empty ");
   }
   endCheck = parseEnd();
   if (endCheck && is_parse_ok)
   {
      printf("\nPARSE SUCCESSFUL! ");
   }
   p_symtab();
   return is_parse_ok;             // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/


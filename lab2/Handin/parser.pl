/******************************************************************************/
/* From Programming in Prolog (4th Ed.) Clocksin & Mellish, Springer (1994)   */
/* Chapter 5, pp 101-103 (DFR (140421) modified for input from a file)        */
/******************************************************************************/

read_in(File,[W|Ws]) :- see(File), get0(C), 
                        readword(C, W, C1), restsent(W, C1, Ws), nl, seen.

/******************************************************************************/
/* Given a word and the character after it, read in the rest of the sentence  */
/******************************************************************************/

restsent(W, _, [])         :- W = -1.                /* added EOF handling */
restsent(W, _, [])         :- lastword(W).
restsent(_, C, [W1 | Ws ]) :- readword(C, W1, C1), restsent(W1, C1, Ws).

/******************************************************************************/
/* Read in a single word, given an initial character,                         */
/* and remembering what character came after the word (NB!)                   */
/******************************************************************************/

readword(C, W, _)  :- C = -1, W = C.                    /* added EOF handling */
readword(C,W,C2)   :- C = 58, get0(C1), readwordaux(C,W,C1,C2).
readword(C, W, C1) :- single_character( C ), name(W, [C]), get0(C1).
readword(C, W, C2) :- 
                      in_num(C, NewC ),
                      get0(C1),
                      restnum(C1, Cs, C2),
                      name(W, [NewC|Cs]).
readword(C, W, C2) :-                                   
                      in_word(C, NewC ),
                      get0(C1),
                      restword(C1, Cs, C2),
                      name(W, [NewC|Cs]).

readword(C, W, C2) :- char_type(C,space),get0(C1),readword(C1, W, C2).
readword(C, W, C2) :- name(W,[C]), get0(C2).  


readwordaux(C,W,C1,C2) :- C1 = 61, name(W,[C,C1]), get0(C2).
readwordaux(C,W,C1,C2) :- C1 \= 61, name(W,[C]), C1 = C2.

restword(C, [NewC|Cs], C2) :-
   in_word(C, NewC),
   get0(C1),
   restword(C1, Cs, C2).

restword(C, [ ], C).

restnum(C, [NewC|Cs], C2) :-
   in_num(C, NewC),
   get0(C1),
   restnum(C1, Cs, C2).

restnum(C, [ ], C).
/******************************************************************************/
/* These characters form words on their own                                   */
/******************************************************************************/

single_character(40).                  /* ( */
single_character(41).                  /* ) */
single_character(42).                  /* + */
single_character(43).                  /* * */
single_character(44).                  /* , */
single_character(59).                  /* ; */
single_character(58).                  /* : */
single_character(61).                  /* = */
single_character(46).                  /* . */

/******************************************************************************/
/* These characters can appear within a word.                                 */
/* The second in_word clause converts character to lower case                 */
/******************************************************************************/

in_word(C, C) :- C>96, C<123.             /* a b ... z */
in_word(C, L) :- C>64, C<91, L is C+32.   /* A B ... Z */
in_word(C, C) :- C>47, C<58.              /* 1 2 ... 9 */
in_num(C, C)  :- C>47, C<58.

/******************************************************************************/
/* These words terminate a sentence                                           */
/******************************************************************************/

lastword('.').

/******************************************************************************/
/* The Lexer Part                                                             */
/******************************************************************************/

lexer([ ],[ ]).
lexer([H|T],[F|S])  :- match(H,F),lexer(T,S).

/******************************************************************************/
/* The lexer match part                                                       */
/******************************************************************************/
match(L,F) :- L = 'program',  F is 256.
match(L,F) :- L = 'input',    F is 257.
match(L,F) :- L = 'output',   F is 258.
match(L,F) :- L = 'var',      F is 259.
match(L,F) :- L = 'integer',  F is 260.
match(L,F) :- L = 'begin',    F is 261.
match(L,F) :- L = 'end',      F is 262.
match(L,F) :- L = 'real',     F is 264.
match(L,F) :- L = 'boolean',   F is 265. 

match(L,F) :- L = '(', F is 40.                  
match(L,F) :- L = ')', F is 41. 
match(L,F) :- L = '*', F is 42.
match(L,F) :- L = '+', F is 43.                 
match(L,F) :- L = ',', F is 44.
match(L,F) :- L = '.', F is 46.  
match(L,F) :- L = ':', F is 58.                
match(L,F) :- L = ';', F is 59.

match(L,F) :- L = ':=',       F is 271.

match(L,F) :- name(L,[T|S]),char_type(T,alpha),  F is 270.


match(L,F) :- name(L,[T|S]),char_type(T,digit), match_digit(S), F is 272.

match_digit([ ]).
match_digit([H|T]) :- char_type(H,digit),match_digit(T).

match(L,F) :- L = -1, F is 275.

/* 273 undef */
match(_,F) :- F is 273.

/******************************************************************************/
/* Terminal symbol defenition.                                                */
/******************************************************************************/

openpar      --> [40].
closepar     --> [41].
mult         --> [42].
add          --> [43].
comma        --> [44].
dot          --> [46].
colon        --> [58].
semicol      --> [59].
pgram        --> [256].
input        --> [257].
output       --> [258].
var          --> [259].
integer      --> [260].
begin        --> [261].
end          --> [262].
real         --> [264].
boolean      --> [265].
id           --> [270].
assign       --> [271].
number       --> [272].



/******************************************************************************/
/* The Parser part                                                            */
/******************************************************************************/

program       --> prog_head, var_part, stat_part.

/******************************************************************************/
/* Program Header                                                             */
/******************************************************************************/

prog_head     --> pgram, id, openpar, input, comma, output, closepar, semicol.

/******************************************************************************/
/* Var_part                                                                   */
/******************************************************************************/

var_part              --> var, var_dec_list.
var_dec_list          --> var_dec.
var_dec_list          --> var_dec, var_dec_list. 
var_dec               --> id_list, colon, type, semicol.
id_list               --> id.
id_list               --> id, comma, id_list.
type                  --> integer|real|boolean.

/******************************************************************************/
/* Stat part                                                                  */
/******************************************************************************/

stat_part            --> begin, stat_list, end, dot.
stat_list            --> stat.
stat_list            --> stat, semicol, stat_list.
stat                 --> assign_stat.
assign_stat          --> id, assign, expr.
expr                 --> term.
expr                 --> term, op, expr.
term                 --> factor.
term                 --> factor, op, term.
factor               --> operand | openpar, expr, closepar.
operand              --> id | number.
op                   --> add | mult.

/******************************************************************************/
/* The Parser end                                                             */
/******************************************************************************/

/******************************************************************************/
/* added for demonstration purposes 140421, updated 150301                    */
/* testa  - file input (characters + Pascal program)                          */
/* testb  - file input as testa + output to file                              */
/* ttrace - file input + switch on tracing (check this carefully)             */
/******************************************************************************/

testa   :- testread(['cmreader.txt', 'testok1.pas']).
testdev :- testread(['devtest.pas']).
testb   :- tell('cmreader.out'), testread(['cmreader.txt', 'testok1.pas']), told.
ttrace  :- trace, testread(['cmreader.txt']), notrace, nodebug.


/******************************************************************************/
/* testok  - Testing all testok*.pas files                                    */
/* testaz  - Testing all test a-z.pas files                                   */
/* testsem - Testing all sem*.pas files                                       */
/* testfun - Testing all fun*.pas files                                       */
/* runparser-Testing all tests and puts output in a file                      */
/******************************************************************************/


testok  :- testall(['testfiles/testok1.pas', 'testfiles/testok2.pas', 'testfiles/testok3.pas',
          'testfiles/testok4.pas', 'testfiles/testok5.pas', 'testfiles/testok6.pas', 
          'testfiles/testok7.pas']).

testaz  :- testall(['testfiles/testa.pas', 'testfiles/testb.pas', 'testfiles/testc.pas',
          'testfiles/testd.pas', 'testfiles/teste.pas', 'testfiles/testf.pas', 
          'testfiles/testg.pas', 'testfiles/testh.pas', 'testfiles/testi.pas',
          'testfiles/testj.pas',
          'testfiles/testk.pas', 'testfiles/testl.pas', 'testfiles/testm.pas',
          'testfiles/testn.pas', 'testfiles/testo.pas', 'testfiles/testp.pas',
          'testfiles/testq.pas', 'testfiles/testr.pas', 'testfiles/tests.pas',
          'testfiles/testt.pas', 'testfiles/testu.pas', 'testfiles/testv.pas',
          'testfiles/testw.pas', 'testfiles/testx.pas', 'testfiles/testy.pas',
          'testfiles/testz.pas']).

testsem :- testall(['testfiles/sem1.pas', 'testfiles/sem2.pas', 'testfiles/sem3.pas',
          'testfiles/sem4.pas', 'testfiles/sem5.pas']).

testfun :- testall(['testfiles/fun1.pas', 'testfiles/fun2.pas', 'testfiles/fun3.pas',
          'testfiles/fun4.pas', 'testfiles/fun5.pas']).

testall([]).
testall([H|T]) :- testparser([H]), testall(T).

runparser :- tell('parser.out'),write('Testing OK programs '),nl, testok,nl,write('Testing a-z programs '),nl, testaz,nl,
          write('Testing fun programs '),nl,testfun,nl,write('Testing sem programs '),nl,testsem,nl,told, 
          write('parser.out created'), nl, halt.

testparser([]).
testparser([H|T])  :- nl,write('Testing '),write(H),nl,read_in(H,L),write(L),nl,lexer(L,X), write(X), nl ,parser(X,_),
                   nl,write(H),write(' end of parse'),nl, testparser(T).

parser(X,_)        :- program(X,[]), write('Parse OK!') ; write('Parse Fail!').

/******************************************************************************/
/* end of program                                                             */
/******************************************************************************/
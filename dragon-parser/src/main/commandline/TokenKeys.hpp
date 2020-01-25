#ifndef TOKENKEYS
#define TOKENKEYS

//standard includes
#include <vector> 
#include <stdio.h>
#include <map>

//project includes

const int BOOLCONST = 2;

const int IDENTIFIER = 4;
const int TYPEIDENTIFIER = 5;


const int TYPEPARAM = 6;
const int FLOATING = 7;

const int INTEGER = 8;
const int CHARCONST = 9;
const int STRINGCONST = 10;

const int LINE_NO = 11;

const int ARROW = 12;
const int ANYTYPE = 13;
const int OPERATOR = 14;
const int STRUCTMEMBERACCESS = 15;
const int IF = 16;
const int WHILE = 17;
const int NAMESPACE = 18;
const int RETURN = 21;
const int LPARENS = 22;
const int RPARENS = 23;
const int LBRACKET = 24;
const int RBRACKET = 25;
const int LCURLY = 26;
const int RCURLY = 27;

const int GREATER = 28;
const int LESSER = 29;
const int WAVE = 30;
const int SEMICOLON = 31;
const int EQ = 32;

const int STRUCT = 34;
const int COMMA = 35;
const int LOOP = 36;
const int FN = 37;
const int ELSE = 38;
#endif
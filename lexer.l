/************************************
	Compilers 2013-2014	

File: lexer.l
Date: 25/5/2014
Created by: Thaleia-Dimitra Doudali
		Athanasios Papoutsidakis	
************************************/

%{
//#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parser.h"
#define T_eof		0

void yyerror(const char * msg);
extern int lineno;
int lineno = 1;
%}

%option noinput
%option nounput
%x COMMENT

%%

"and" 		{return T_and;}
"continue"	{return T_continue;}
"FOR"		{return T_FOR;}
"NEXT"		{return T_NEXT;}
"return"	{return T_return;}
"WRITE" 	{return T_WRITE;}
"bool"		{return T_bool;}
"default"	{return T_default;}
"FORM"		{return T_FORM;}
"not"		{return T_not;}
"STEP"		{return T_STEP;}
"WRITELN"	{return T_WRITELN;}
"break"		{return T_break;}
"do"		{return T_do;}
"FUNC"		{return T_FUNC;}
"or"		{return T_or;}
"switch"	{return T_switch;}
"WRITESP"	{return T_WRITESP;}
"case"		{return T_case;}
"DOWNTO"	{return T_DOWNTO;}
"if"		{return T_if;}
"PROC"		{return T_PROC;}
"TO"		{return T_TO;}
"WRITESPLN"	{return T_WRITESPLN;}
"char"		{return T_char;}
"else"		{return T_else;}
"int"		{return T_int;}
"PROGRAM"	{return T_PROGRAM;}
"true"		{return T_true;}
"const"		{return T_const;}
"false"		{return T_false;}
"MOD"		{return T_MOD;}
"REAL"		{return T_REAL;}
"while"		{return T_while;}

"=="        {return T_equal;}
"!="        {return T_inequal;}
">="        {return T_notst;}
"<="        {return T_notgt;}
"&&"        {return T_op_and;}
"||"        {return T_op_or;}
"++"        {return T_plus2;}
"--"        {return T_minus2;}
"+="        {return T_eqplus;}
"-="        {return T_eqminus;}
"*="        {return T_eqmul;}
"/="        {return T_eqdiv;}
"%="        {return T_eqmod;}


[a-zA-Z][_a-zA-Z0-9]*								{
														yylval.s = (char *)malloc(strlen(yytext) +1);
														strcpy(yylval.s, yytext);
														return T_ID;
													} 
[1-9][0-9]*|0										{	
														yylval.i = atoi(yytext);
														return T_intConst;
													}
[\&\;\.\(\)\:\,\[\]\{\}] 							{return yytext[0];}
[\>\<\+\-\*\/\%\!\=] 								{return yytext[0];}	

"'"([ !#$%&]|[(-[]|[]-~]|\\n|\\t|\\r|\\0|\\\\|\\\'|\\\")"'"		{	
																	yylval.c = yytext[1];
																	return T_charConst;
																}
"\""([ !#$%&]|[(-[]|[]-~]|\\n|\\t|\\r|\\0|\\\\|\\\'|\\\")+"\""	{	
																	yytext[yyleng-1] = '\0';
																	yylval.s = (char *)malloc(yyleng);
																	strcpy(yylval.s, &yytext[1]);
																	return T_stringLiteral;
																}
([1-9][0-9]*|0)\.[0-9]+((e|E)(\+|\-)?[0-9]+)?  					{	
																	yylval.r = atof(yytext);
																	return T_realConst;
																}

[ \t\r]+   										{/*ignore white spaces*/}
\n												{lineno++;}
"//".*\n								 		{lineno++;}
"/*"											{BEGIN(COMMENT);}
<COMMENT>"*/"									{BEGIN(INITIAL);}
<COMMENT>\n										{lineno++;}
<COMMENT>"*"									{/*nothing*/}
<COMMENT>[^*\n]+								{/*nothing*/}

<<EOF>> 										{return T_eof;}
.       										{fprintf(stderr, "Lexical error, line %d\n", lineno);
          	 									 exit(1);}

%%

void ERROR (const char * fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, "Line %d, error: ", lineno);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   exit(1);
}

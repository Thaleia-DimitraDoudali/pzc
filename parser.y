/************************************
	Compilers 2013-2014	

File: parser.y
Date: 25/5/2014
Created by:	Athanasios Papoutsidakis	
	      	Thaleia-Dimitra Doudali
************************************/
%{
#include <stdio.h>
#include <stdlib.h>
#include "parser_interface.h"
#include "error.h"

void ERROR(const char *fmt, ...);
int yylex();
void yyerror(const char * msg);

extern int lineno;
int array_size;
char *current_name;
Type currentType, valueType, result_type;
bool look, new, is_forward = false, is_proc;
%}

%union {

	int i;
	long double r;
	char c;
	char *s;
	struct Type_tag *t;
	struct Type_Value_tag * vt;
}

%token T_and
%token T_continue
%token T_FOR
%token T_NEXT
%token T_return
%token T_WRITE
%token T_bool
%token T_default
%token T_FORM
%token T_not
%token T_STEP
%token T_WRITELN
%token T_break
%token T_do
%token T_FUNC
%token T_or
%token T_switch
%token T_WRITESP
%token T_case
%token T_DOWNTO
%token T_if
%token T_PROC
%token T_TO
%token T_WRITESPLN
%token T_char
%token T_else
%token T_int
%token T_PROGRAM
%token <vt> T_true		"true"
%token T_const
%token <vt> T_false		"false"
%token T_MOD
%token T_REAL
%token T_while
%token<i> T_intConst
%token<r> T_realConst
%token<c> T_charConst
%token<s> T_stringLiteral
%token <s> T_ID
%token T_equal		"=="
%token T_inequal	"!="
%token T_notst		">="
%token T_notgt		"<="
%token T_op_and		"&&"
%token T_op_or		"||"
%token T_plus2		"++"
%token T_minus2		"--"
%token T_eqplus		"+="
%token T_eqminus	"-="
%token T_eqmul		"*="
%token T_eqdiv		"/="
%token T_eqmod		"%="

%type <t> type
%type <t> array_size_def_bracket_list
%type <vt> expr
%type <vt> call
%type <vt> l_value

%left THEN 
%left T_else

%left T_op_or T_or
%left T_op_and T_and
%left T_equal T_inequal
%left '>' '<' T_notst T_notgt
%left '+' '-' 
%left '*' '/' '%' T_MOD
%left SIGN NOT


%%

module:										
  /* nothing */
| declaration module


declaration:
  const_def
| var_def
| routine
| program
;

const_def:
  T_const type const_init_list ';'				
;

const_init_list:
  const_init
| const_init ',' const_init_list
;

const_init:
  T_ID '=' expr									{
													/*Lookup current scope to see if already declared*/
													look = lookup($1);
													if (look == false) {
														new_constant($1, currentType);
													} else {
														printf("Line: %d semantics error: redefinition of constant %s \n", lineno, $1);
														exit(1);
													}
													/*Type checking - save value at symbol table entry*/
													//if (currentType == typeReal && valueType == typeInteger) {
													//}
													/*According to specifications:
													
													if (currentType == $3.type) then ok
													if (currentType == Real && $3.type == int) 
													then cast int to real - kapoia sunartisi pou kati tha kanei gia tin assembly
													if (currentType == Int  && $3.type == char) then cast char to int
													if (currentType == Char && $3.type == int) then cast int to char
													else 
														type checking error
														tha typwsw to error
														den exei simasia i value, gt koitaei ton typo kai proxwraw*/
													/*genquad(assign, name, value) */
													//tetrades parallila me parsing, optimizations meta diatrexontas aptin arxi tis tetrades, kai otan ftiaxtoyn oi tetrades delete ta scopes

															}
;

var_def:
  type var_init_list ';'						
;

var_init_list:
  var_init
| var_init ',' var_init_list
;

var_init:
  T_ID											{
													/*Lookup current scope to see if already declared*/
													look = lookup($1);
													if (look == false) {
														new_variable($1, currentType);
													} else {
														printf("Line: %d error: redefinition of variable %s \n", lineno, $1);
														exit(1);
													}
													/*According to specifications:
													if (currentScope == initial)
														if (int) insert(var, currentType, $1, 0)
														if (real) insert(var, currentType, $1, 0.0)
														if (bool) insert(var, currentType, $1, false)
														if (char) insert(var, currentType, $1, '\0')
														gia tis tetrades einai auto
														genquad(assign, name, value)
													else insert(var, currentType, $1, none) */}

| T_ID '=' expr									{	
													/*Lookup current scope to see if already declared*/
													look = lookup($1);
													if (look == false) {
														new_variable($1, currentType);
													} else {
														printf("Line: %d error: redefinition of variable %s \n", lineno, $1);
														exit(1);
													}
													/* According to specifications if global initialized then it should 
													be treated as const
	
													According to specifications:
													if (currentType == $3.type) then ok
													if (currentType == Real && $3.type == int) then cast int to real
													if (currentType == Int  && $3.type == char) then cast char to int
													if (currentType == Char && $3.type == int) then cast int to char
													else type checking error

													I HAVE TO CHECK IF EXPR is constant expression	
													
													if (currentScope == initial) okk
1														new_constant
														insert(const, currentType, $1, $3)
													else 
														insert(var, currentType, $1, $3) 
			
													genquad(assign, name, value) */}

| T_ID array_size_def_bracket_list				{
													/*Lookup current scope to see if already declared*/
													look = lookup($1);
													if (look == false) {
														new_variable($1, typeArray(array_size, currentType));
													} else {
														printf("Line: %d error: redefinition of variable %s \n", lineno, $1);
														exit(1);
													}
												}
;														

array_size_def_bracket_list:						
  '[' expr ']'									{	
													if (!check_array_index($2, lineno)) {
														exit(1);
													} else {
														array_size = $2->val.vInteger;
													}
													/*if (currentScope == initial)
														initialize all array's elements to zero int, real, bool or char.//gia tetrada
														insert(array, currentType, currentName, dimensionsArray, zero)
														genquad that runs through the elements of the array and initializes them to zero.
													else
														insert(array, currentType, currentName, dimensionsArray, none)*/}
| '[' expr ']' array_size_def_bracket_list		{ 
													if (!check_array_index($2, lineno)) {
														//finish check array for negative and type checking
														exit(1);
													} else {
														array_size *= $2->val.vInteger;
													}
												}
;

routine:										
  routine_header ';'							{ 
													is_forward = true;
													if (!is_proc) {
														printf("%d ", result_type->type);
														new_function(current_name, result_type, is_forward, lineno);
													} else {
														new_process(current_name, is_forward, lineno);
													}
													new_parameter_list();

												}
| routine_header block							{
													is_forward = false;	
													if (!is_proc) {
														printf("%d ", result_type->type);
														new_function(current_name, result_type, is_forward, lineno);
													} else {
														new_process(current_name, is_forward, lineno);
													}
													new_parameter_list();

												}
;
routine_header:
  process_header
| function_header
;

process_header:
  T_PROC T_ID formal_argument_list_def			{	
													look = lookup($2);
													current_name = $2;
													is_proc = true;
													if (look == true){
														if (is_forward == false ) {
															printf("Line: %d error: redefinition of name %s \n", lineno, $2);
															exit(1);
														}
													}
												}
;

function_header:
  T_FUNC type T_ID formal_argument_list_def		{	
													look = lookup($3);
													current_name = $3;
													is_proc = false;
													result_type = $2;
													if (look == true){													
														if (is_forward == false ) {
															printf("Line: %d error: redefinition of name %s \n", lineno, $3);
															exit(1);
														}
													}
												}
;

formal_argument_list_def:					
  '(' ')'										
| '(' formal_argument_list ')'	
;

formal_argument_list:
  type formal
| type formal ',' formal_argument_list
;

formal:
  T_ID											{
													new_parameter($1, currentType, PASS_BY_VALUE);
												}
| '&' T_ID										{	
													new_parameter($2, currentType, PASS_BY_REFERENCE);
												}
| T_ID '[' ']' array_arg_brackets_list			{
													new_parameter($1, typeIArray(currentType), PASS_BY_REFERENCE);
												}
| T_ID '[' expr ']' array_arg_brackets_list		{
													if (!check_array_index($3, lineno)) {
														exit(1);
													} else {
														array_size *= $3->val.vInteger;
													}
													new_parameter($1, typeArray(array_size, currentType), PASS_BY_REFERENCE);
													array_size = 1;
												}
;

array_arg_brackets_list:
  /*nothing*/						
| array_arg_brackets_op						
;

array_arg_brackets_op:
  '[' expr ']'									{
													array_size = 1;
													if (!check_array_index($2, lineno)) {
														//finish check array for negative and type checking
														exit(1);
													} else {
														array_size = $2->val.vInteger;
													}
												}
| '[' expr ']' array_arg_brackets_op			{
													if (!check_array_index($2, lineno)) {
														//finish check array for negative and type checking
														exit(1);
													} else {
														array_size *= $2->val.vInteger;
													}
												}
;

program:
  program_header block							
;

program_header:
  T_PROGRAM T_ID '(' ')'						{	
													look = lookup($2);
													if (look == false) {
														new_process($2, is_forward, lineno);
													} else {
														printf("Line: %d error: redefinition of name %s \n", lineno, $2);
														exit(1);
													}	
												}
;

type:											
  T_bool										{$$ = typeBoolean; currentType = typeBoolean;}
| T_char										{$$ = typeChar; currentType = typeChar;}
| T_int											{$$ = typeInteger; currentType = typeInteger;}
| T_REAL										{$$ = typeReal; currentType = typeReal;}
;


expr:
  T_intConst									{
													$$ = (struct Type_Value_tag *) malloc(sizeof( struct Type_Value_tag *));
												 	$$->val.vInteger = $1;
												 	$$->type = typeInteger;
												}
| T_realConst									{
													$$ = (struct Type_Value_tag *) malloc(sizeof( struct Type_Value_tag *));
												 	$$->val.vReal = $1;
												 	$$->type = typeReal;

												}
| T_charConst									{
													$$ = (struct Type_Value_tag *) malloc(sizeof( struct Type_Value_tag *));
												 	$$->val.vChar = $1;
												 	$$->type = typeChar;
																			
												}
| T_stringLiteral								{
													$$ = (struct Type_Value_tag *) malloc(sizeof( struct Type_Value_tag *));
												 	$$->val.vString = $1;
												 	//array of chars??$$->type = typeReal;

												}
| T_true										{
													$$ = (struct Type_Value_tag *) malloc(sizeof( struct Type_Value_tag *));
												 	//?$$->val.vBoolean = $1;
												 	$$->type = typeBoolean;

												}
| T_false										{
													$$ = (struct Type_Value_tag *) malloc(sizeof( struct Type_Value_tag *));
												 	//?$$->val.vBoolean = $1;
												 	$$->type = typeBoolean;

												}
| '(' expr ')'									{
													$$->val = $2->val;
													$$->type = $2->type;
												}
| expr T_or expr								{/* type checking if arguments are boolean */}
| expr T_op_or expr
| expr T_and expr
| expr T_op_and expr
| expr T_equal expr
| expr T_inequal expr
| expr '>' expr									{/* type checking */}
| expr '<' expr
| expr T_notgt expr
| expr T_notst expr
| expr '+' expr									{/* petaw error an anepitreptos sunduasmos praxis -> error -> $$.type = type.none
													kai an prokupsei meta tupos type.none tote simainei oti exei petaxtei lathos opote apla proxwra
													chekarw px an int kai real tote cast apotelesma se real gia na dwsw 
													ta swsta orismata stin tetrada*/}
| expr '-' expr
| expr '*' expr
| expr '/' expr
| expr '%' expr
| expr T_MOD expr
| '+' expr			%prec SIGN					{$$ = $2;}
| '-' expr			%prec SIGN					{$$ = $2;}
| '!' expr			%prec NOT					{$$ = $2; /* genquad */}
| T_not expr		%prec NOT					{$$ = $2;}
| call											{$$ = $1;}
| l_value										{$$ = $1;}
;

call:
  T_ID '(' ')'									{look = lookup($1);/*lookup(name, globalScope) look up name, check number and type checking of parameters, ena flag mesa sti sunartisi gia to an exei lathos diplwsi apo definition wste tote na koite mono return type kai oxi arguments*/}
| T_ID '(' expr_list ')'						{look = lookup($1);/*lookup(name, globalScope)*/}
;

expr_list:
  expr
| expr ',' expr_list
;

l_value:
  T_ID										{	printf("Checking lvalue - lookup all scopes: ");
												look = lookup_all_scopes($1); 
												if (look)
													printf("lookup ok\n");/* lookup name */
												else printf("lookup failed \n");
											}
| T_ID array_selection						{look = lookup($1); /* lookup name, dimensions, check array bounds */}
;

array_selection:
  '[' expr ']'
| '[' expr ']' array_selection
;

block:
  '{' {open_scope();} '}' {close_scope();}
| '{' { open_scope();} block_operations '}' {close_scope();}
;

block_operations:
  block_operation
| block_operation block_operations
;

block_operation:
  local_def
| stmt
;

local_def:
  const_def
| var_def
;

stmt:
  ';'
| assign_stmt
| postincr_stmt
| postdecr_stmt
| call ';'
| if_stmt
| while_stmt
| do_stmt
| for_stmt
| switch_stmt
| T_break ';'								{/*look at specification for break*/}
| T_continue ';'
| return_stmt
| block
| write_stmt
;

assign_stmt:
  l_value assign expr ';'					{/*type checking*/}
;

assign:
  '='
| T_eqplus									{/*stin tetrada tha fanei*/}
| T_eqminus
| T_eqmul
| T_eqdiv
| T_eqmod
;

postincr_stmt:
  l_value T_plus2 ';'						{/* type checking*/}
;

postdecr_stmt:
  l_value T_minus2 ';'
;

while_stmt:									{/* type check expr bool */}
  T_while '(' expr ')' stmt
;

do_stmt:
  T_do stmt T_while '(' expr ')' ';'		{/* type check expr bool*/}
;

for_stmt:
  T_FOR '(' T_ID ',' range ')' stmt 		{look = lookup($3);/* lookup id name, type check id and range to be integers*/}

;

range:
  expr range_direction expr step
;

step:
  /*nothing*/
| T_STEP expr
;

range_direction:
  T_TO
| T_DOWNTO
;

switch_stmt:
  T_switch '(' expr ')' '{' cases '}'			{/* expr must be integer */}
;

cases:
  /* nothing */
| case cases
| caseheads T_default ':' casebody
;

case:
  T_case expr ':' case							{/*expr must be integer */}
| T_case expr ':' casebody
;

caseheads:
  /* nothing */
| T_case expr ':' caseheads
;

casebody:
  T_break ';'
| T_NEXT ';'
| stmt casebody
;

return_stmt:
  T_return ';'							
| T_return expr ';'								{/*expr must be same type as function result. proc should not return anything */}
;

write_stmt:
  write '(' write_args ')' ';'
;

write:
  T_WRITE
| T_WRITELN
| T_WRITESP
| T_WRITESPLN
;

write_args:
  /* nothing */
| formats
;

formats:
  format
| format ',' formats
;

format:
  expr
| T_FORM '(' expr ',' expr ')'					{/* $5 must be int  */}
| T_FORM '(' expr ',' expr ',' expr ')'			{/* $3 real, $5, $7 int */}
;

if_stmt:
  T_if '(' expr ')' thenelse					{/* expr should be bool */}
;

thenelse:
  stmt				%prec THEN
| stmt T_else stmt
;


%%

void yyerror(const char * msg)
{
	ERROR("Parser said, %s", msg);

}

int main(int argc, char * argv[])
{
	open_global_scope();
	yyparse();
	close_scope();
	print_all_scopes();
	return 0;
}

/* Interface between the compiler and the parser. */

#ifndef _PARSER_INTERFACE_H_
#define _PARSER_INTERFACE_H_ 1

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef int           RepInteger;
typedef unsigned char RepBoolean;
typedef char          RepChar;
typedef long double   RepReal;
typedef const char *  RepString;

typedef struct Type_tag * Type;

typedef enum {
  TYPE_NONE = 0,
  TYPE_VOID,
  TYPE_INTEGER,
  TYPE_BOOLEAN,                     
  TYPE_CHAR,                        
  TYPE_REAL,                       
  TYPE_ARRAY,                       
  TYPE_IARRAY,
  NTYPES
} type_e;

struct Type_tag {
  type_e type;
  Type refType;
  int size;
  unsigned int refCount;
};

static struct Type_tag typeConst [] = {
	{ TYPE_VOID,    NULL, 0, 0 },
	{ TYPE_INTEGER, NULL, 0, 0 },
	{ TYPE_BOOLEAN, NULL, 0, 0 },
	{ TYPE_CHAR,    NULL, 0, 0 },
	{ TYPE_REAL,    NULL, 0, 0 }
};

const Type typeVoid    = &(typeConst[0]);
const Type typeInteger = &(typeConst[1]);
const Type typeBoolean = &(typeConst[2]);
const Type typeChar    = &(typeConst[3]);
const Type typeReal    = &(typeConst[4]);

typedef enum {
	ENTRY_VARIABLE,
	ENTRY_CONSTANT,
	ENTRY_FUNCTION,
	ENTRY_PARAMETER,
	ENTRY_PROCESS
} EntryType;

typedef enum {
	PASS_BY_VALUE,                        
	PASS_BY_REFERENCE                   
} PassMode;

typedef union {
   RepInteger vInteger;
   RepBoolean vBoolean;
   RepChar    vChar;
   RepReal    vReal;
   RepString  vString;
} Value;

typedef struct Type_Value_tag * Type_Value;

struct Type_Value_tag {
	struct Type_tag *type;
	Value val;
};

  void new_parameter(const char* name, Type type, PassMode mode);
  void new_parameter_list();

  void new_function(const char* name, Type res_type, bool is_forw, int line) ;
  void new_process(const char* name, bool is_forw, int line) ;

  void new_variable(const char* name, Type type);

  void new_constant(const char* name, Type type);

  Type typeArray(RepInteger size, Type refType);
  Type typeIArray(Type refType);


  bool check_array_index(Type_Value tpvl, int line);


  bool lookup(const char* name);
  bool lookup_all_scopes(const char* name);


  void open_scope();
  void open_global_scope();
  void close_scope();
  void print_all_scopes();
/*********************************************************************************/


typedef int UnitType;

typedef struct{
    UnitType type;
    void * pointer;
} unit;

#include "parser.h"
#include "create_options.h"

extern unit module;

int createSymbolTable();

unit prepend(unit child, unit parent);


/* Generic unit constructors, to be used by others. */
unit    unit_new            (UnitType, void*);
unit    simpleConst_new     (UnitType);

/* Common generic unit destructor */
void    unit_delete (unit);

unit    ID_new      (const char*);
void    ID_delete   (unit);

/* Constants */

unit    stringLiteral_new       (const char*);
void    stringLiteral_delete    (unit);

unit    charConst_new       (const char*);
void    charConst_delete    (unit);

unit    intConst_new        (const char*);
void    intConst_delete     (unit);

unit    realConst_new       (const char*);
void    realConst_delete    (unit);

unit    trueConst_new       ();
unit    falseConst_new      ();

/* Expressions */

unit    unaryOp_new         (UnitType op, unit);
void    unaryOp_delete      (unit);

unit    binaryOp_new        (UnitType op, unit, unit);
void    binaryOp_delete     (unit);

/* Types */

unit    typeBool_new    ();
unit    typeInt_new     ();
unit    typeChar_new    ();
unit    typeReal_new    ();

/* Statements */

unit    assignment_new      (unit, UnitType op, unit);
void    assignment_delete   (unit);

unit    ifThenStmt_new      (unit then_stmt);
unit    ifThenElseStmt_new  (unit then_stmt, unit else_stmt);
unit    ifStmt_addCondition (unit if_stmt, unit condition);
void    ifStmt_delete       (unit if_stmt);

unit    increment_new       (UnitType op, unit);
void    increment_delete    (unit);

unit    write_new           (UnitType write_type, unit args);
void    write_delete        (unit);

unit    writeArgs_newEmpty  ();
unit    writeArgs_new       (unit format);
unit    writeArgs_prepend   (unit parent, unit child);

unit    writeFormat_new1    (unit arg, unit width);
unit    writeFormat_new2    (unit arg, unit width, unit digits);

/* Parse tree nodes */

unit    module_new      ();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _PARSER_INTERFACE_H_

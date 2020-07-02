#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Token.h"
#include "HashTable.h"

extern FILE* yyout_parser;
extern int line_idx;

/* Symbol Table Functions */
symbol_table* cur_table;
void insert_to_current_table(const char* key,ENTRY_ROLE role,ENTRY_TYPE type, void* list);


typedef struct VAR_ATTR {
	ENTRY_TYPE type;
	dim_list* dim_list;
	param_list* param_list;
	int value;
	char* id;
} VAR_ATTR;

VAR_ATTR* create_var_attr();
void delete_var_attr(VAR_ATTR* var);

// Main function
void parser();
void parse();

// Parsing functions
void Parse_PROG();
void Parse_GLOBAL_VARS();
void Parse_GLOBAL_VARS_1();
void Parse_FUNC_PREDEFS();
void Parse_FUNC_PREDEFS_1();
void Parse_FUNC_FULL_DEFS();
void Parse_FUNC_FULL_DEFS_1();
VAR_ATTR* Parse_FUNC_PROTOTYPE();
VAR_ATTR* Parse_VAR();
VAR_ATTR* Parse_VAR_1(dim_list* d_list,ENTRY_TYPE expected_type, const char* id);
void Parse_VAR_DEC();
VAR_ATTR* Parse_VAR_DEC_1();
void Parse_VAR_DEC_LIST();
void Parse_VAR_DEC_LIST_1();
VAR_ATTR* Parse_TYPE();
void Parse_DIM_SIZES(dim_list*);
void Parse_DIM_SIZES_1(dim_list*);
VAR_ATTR* Parse_RETURNED_TYPE();
VAR_ATTR* Parse_PARAM();
void Parse_PARAM_1(dim_list**);
void Parse_PARAMS(param_list**);
void Parse_PARAM_LIST(param_list* p_list);
void Parse_PARAM_LIST_1(param_list* p_list);
void Parse_FUNC_WITH_BODY();
VAR_ATTR* Parse_COMP_STMT(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag);
VAR_ATTR* Parse_STMT_LIST(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag);
VAR_ATTR* Parse_STMT_LIST_1(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag);
VAR_ATTR* Parse_STMT(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag);
VAR_ATTR* Parse_EXPR();
VAR_ATTR* Parse_EXPR_1(ENTRY_TYPE cur_type, int cur_value);
VAR_ATTR* Parse_EXPR_LIST(dim_node* node);
VAR_ATTR* Parse_EXPR_LIST_1(dim_node* node);
VAR_ATTR* Parse_IF_STMT(ENTRY_TYPE _type, int* has_return_stmt_flag);
VAR_ATTR* Parse_CALL();
VAR_ATTR* Parse_ARGS(param_list* p_list);
VAR_ATTR* Parse_ARG_LIST(param_node* node);
VAR_ATTR* Parse_ARG_LIST_1(param_node* node);
void Parse_CONDITION();
VAR_ATTR* Parse_TERM();
VAR_ATTR* Parse_TERM_1(ENTRY_TYPE cur_type, int cur_value);
VAR_ATTR* Parse_FACTOR();
VAR_ATTR* Parse_RETURN_STMT(ENTRY_TYPE expected_ret_type);
VAR_ATTR* Parse_RETURN_STMT_1();
void Parse_BB();
void Parse_FB();


// Variables of the Grammer
typedef enum eVariable {
	PROG,
	GLOBAL_VARS,
	GLOBAL_VARS_1,
	FUNC_PREDEFS,
	FUNC_PREDEFS_1,
	FUNC_FULL_DEFS,
	FUNC_FULL_DEFS_1,
	FUNC_PROTOTYPE,
	VAR,
	VAR_1,
	VAR_DEC,
	VAR_DEC_1,
	VAR_DEC_LIST,
	VAR_DEC_LIST_1,
	TYPE,
	DIM_SIZES,
	DIM_SIZES_1,
	RETURNED_TYPE,
	PARAM,
	PARAM_1,
	PARAMS,
	PARAM_LIST,
	PARAM_LIST_1,
	FUNC_WITH_BODY,
	COMP_STMT,
	STMT_LIST,
	STMT_LIST_1,
	STMT,
	EXPR,
	EXPR_1,
	EXPR_LIST,
	EXPR_LIST_1,
	IF_STMT,
	CALL,
	ARGS,
	ARG_LIST,
	ARG_LIST_1,
	CONDITION,
	TERM,
	TERM_1,
	FACTOR,
	RETURN_STMT,
	RETURN_STMT_1
} eVariable;

int Match(int type);
void Error_Recovery(eVariable variable, const char* expected_tokens);
int Follow(eVariable var, eTOKENS token);

// Print functions
void print_rule(const char* rule);
void print_semantic_error(const char* error_msg);
void print_semantic_warning(const char* warning_msg);
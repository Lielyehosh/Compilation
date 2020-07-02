%{ 
#pragma once
#include <stdio.h> 
#include <stdlib.h>
#include "Token.h"
#include "Parser.h"
#define TEST_1_FILE "C:\\temp\\test1.txt"
#define TEST_2_FILE "C:\\temp\\test2.txt"
#define TEST_LEX_1_OUTPUT_FILE "C:\\temp\\test1_313452583_205610074_205586910_lex.txt"
#define TEST_LEX_2_OUTPUT_FILE "C:\\temp\\test2_313452583_205610074_205586910_lex.txt"
#define TEST_SYNTACTIC_1_OUTPUT_FILE "C:\\temp\\test1_313452583_205610074_205586910_syntactic.txt"
#define TEST_SYNTACTIC_2_OUTPUT_FILE "C:\\temp\\test2_313452583_205610074_205586910_syntactic.txt"
#define TEST_SEMANTIC_1_OUTPUT_FILE "C:\\temp\\test2_313452583_205610074_205586910_semantic.txt"
#define TEST_SEMANTIC_2_OUTPUT_FILE "C:\\temp\\test2_313452583_205610074_205586910_semantic.txt"
int line_idx = 1;
FILE *yyout_parser = NULL;
FILE *yyout_semantic = NULL;
%}

/* Creating state for comments*/
%x comment

/* Numbers */
NUM_INTEGER				[1-9][0-9]*|[0-9]
NUM_FLOAT				{NUM_INTEGER}[.][0-9][0-9]*[eE][-+]?{NUM_INTEGER}

/* Operators */
OP_ASSIGNMENT			"="
OP_ADD					"+"
OP_MUL					"*"
OP_LESS_THAN			"<"
OP_GREATER_THAN			">"
OP_LESS_THAN_EQUAL		"<="
OP_GREATER_THAN_EQUAL	">="
OP_EQUAL				"=="
OP_NOT_EQUAL			"!="

/* Keywords */
KEYWORD_INT				"int"
KEYWORD_FLOAT			"float"
KEYWORD_VOID			"void"
KEYWORD_IF				"if"
KEYWORD_RETURN			"return"

/* Separation Signs */
COMMA 					","
COLON 					":"
SEMICOLON 				";"
OPEN_PARENTHESES 		"("
CLOSE_PARENTHESES 		")"
OPEN_BRACKET 			"["
CLOSE_BRACKET 			"]"
OPEN_CURLY_BRACE 		"{"
CLOSE_CURLY_BRACE 		"}"

/* Identifiers */
IDENTIFIER				[a-z](_?[A-Za-z0-9])*

NEW_LINE 				\n
END_OF_FILE   			<<EOF>>
ERROR 					.
IGNORE 					\t|" "|\b


%%
"/*" BEGIN(comment);
<comment>\n 	{line_idx++; BEGIN(comment);}
<comment>[^*\n]*
<comment>"*/" 	BEGIN(0);

{NUM_INTEGER}				return print_and_create_token(TOKEN_INTEGER, yytext ,line_idx);
{NUM_FLOAT}					return print_and_create_token(TOKEN_FLOAT, yytext ,line_idx);
{COMMA}						return print_and_create_token(TOKEN_COMMA, yytext ,line_idx);
{COLON}						return print_and_create_token(TOKEN_COLON, yytext ,line_idx);
{SEMICOLON}					return print_and_create_token(TOKEN_SEMICOLON, yytext ,line_idx);
{OPEN_PARENTHESES}			return print_and_create_token(TOKEN_OPEN_PARENTHESES, yytext ,line_idx);
{CLOSE_PARENTHESES}			return print_and_create_token(TOKEN_CLOSE_PARENTHESES, yytext ,line_idx);
{OPEN_BRACKET}				return print_and_create_token(TOKEN_OPEN_BRACKETS, yytext ,line_idx);
{CLOSE_BRACKET}				return print_and_create_token(TOKEN_CLOSE_BRACKETS, yytext ,line_idx);
{OPEN_CURLY_BRACE}			return print_and_create_token(TOKEN_OPEN_CURLY_BRACES, yytext ,line_idx);
{CLOSE_CURLY_BRACE}			return print_and_create_token(TOKEN_CLOSE_CURLY_BRACES, yytext ,line_idx);
{OP_ASSIGNMENT}				return print_and_create_token(TOKEN_OP_ASSIGN, yytext ,line_idx);
{OP_ADD}                    return print_and_create_token(TOKEN_AR_OP_ADD, yytext ,line_idx);
{OP_MUL}                    return print_and_create_token(TOKEN_AR_OP_MUL, yytext ,line_idx);
{OP_EQUAL}                  return print_and_create_token(TOKEN_REL_OP_EQUAL, yytext ,line_idx);
{OP_NOT_EQUAL}				return print_and_create_token(TOKEN_REL_OP_NOT_EQUAL, yytext ,line_idx);
{OP_LESS_THAN}              return print_and_create_token(TOKEN_REL_OP_LESS, yytext ,line_idx);
{OP_GREATER_THAN}           return print_and_create_token(TOKEN_REL_OP_GRATER, yytext ,line_idx);
{OP_LESS_THAN_EQUAL}        return print_and_create_token(TOKEN_REL_OP_LESS_EQUAL, yytext ,line_idx);
{OP_GREATER_THAN_EQUAL}     return print_and_create_token(TOKEN_REL_OP_GRATER_EQUAL, yytext ,line_idx);
{KEYWORD_INT}    			return print_and_create_token(TOKEN_KEYWORD_INT, yytext ,line_idx);			
{KEYWORD_FLOAT}    			return print_and_create_token(TOKEN_KEYWORD_FLOAT, yytext ,line_idx);
{KEYWORD_VOID}    			return print_and_create_token(TOKEN_KEYWORD_VOID, yytext ,line_idx);
{KEYWORD_IF}    			return print_and_create_token(TOKEN_KEYWORD_IF, yytext ,line_idx);
{KEYWORD_RETURN}    		return print_and_create_token(TOKEN_KEYWORD_RETURN, yytext ,line_idx);
{IDENTIFIER}    			return print_and_create_token(TOKEN_IDENTIFIER, yytext ,line_idx);
{NEW_LINE}					line_idx++;
{IGNORE}					{}
<<EOF>>						{print_and_create_token(TOKEN_END_OF_FILE, yytext ,line_idx); return 0;}
{ERROR}						{fprintf(yyout, "Character '%s' in line: {%d} does not begin any legal token in the language.\n", yytext, line_idx);}
%%

void performTest(const char* input_file,const char* lexer_output_file,const char* syntactic_output_file,const char* semantic_output_file);

void main(){
	/* First test */
	performTest(TEST_1_FILE, TEST_LEX_1_OUTPUT_FILE, TEST_SYNTACTIC_1_OUTPUT_FILE,TEST_SEMANTIC_1_OUTPUT_FILE);
	line_idx = 1;
	/* Second test */
	performTest(TEST_2_FILE, TEST_LEX_2_OUTPUT_FILE, TEST_SYNTACTIC_2_OUTPUT_FILE,TEST_SEMANTIC_2_OUTPUT_FILE);
}

/* This function rule is to handle a single test file */
void performTest(const char* input_file,const char* lexer_output_file,const char* syntactic_output_file,const char* semantic_output_file) {
	yyin = fopen(input_file, "r");
	yyout = fopen(lexer_output_file, "w");
	yyout_parser = fopen(syntactic_output_file, "w");
	yyout_semantic = fopen(semantic_output_file, "w");
	if (yyout && yyout_parser && yyout_semantic) {
		parser();
	}
	else {
		yyout = stdout;
		yyout_parser = stdout;
		yyout_semantic = stdout;
		parser();
	}
	fclose(yyin);
	fclose(yyout);
	fclose(yyout_parser);
	fclose(yyout_semantic);
	free_tokens();
	yyin = NULL;
	yyrestart(yyin);
}
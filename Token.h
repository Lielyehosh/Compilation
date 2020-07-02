#ifndef TOKEN_H
#define TOKEN_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern FILE* yyin, * yyout;

typedef enum eTOKENS
{
	// Numbers
	TOKEN_INTEGER,				/* Integer number */
	TOKEN_FLOAT,				/* Floating number */

	// Separation signs
	TOKEN_COMMA,				/* ',' */
	TOKEN_COLON,				/* ':' */
	TOKEN_SEMICOLON,			/* ';' */
	TOKEN_OPEN_PARENTHESES,		/* '(' */
	TOKEN_CLOSE_PARENTHESES,	/* ')' */
	TOKEN_OPEN_BRACKETS,		/* '[' */
	TOKEN_CLOSE_BRACKETS, 		/* ']' */
	TOKEN_OPEN_CURLY_BRACES,	/* '{' */
	TOKEN_CLOSE_CURLY_BRACES,	/* '}' */

	// Identifier
	TOKEN_IDENTIFIER,			/* Identifier */

	// Keywords
	TOKEN_KEYWORD_INT,			/* int */
	TOKEN_KEYWORD_FLOAT,		/* float */
	TOKEN_KEYWORD_VOID,			/* void */
	TOKEN_KEYWORD_IF,			/* if */
	TOKEN_KEYWORD_RETURN,		/* return */

	// Operations
	TOKEN_OP_ASSIGN,			/* Assignment operation - assignment(=)				*/
	TOKEN_AR_OP_ADD,			/* Arithmetic operation - addition(+)				*/
	TOKEN_AR_OP_MUL,			/* Arithmetic operation - multiply(*)				*/
	TOKEN_REL_OP_EQUAL,			/* Comparison operation - equal(==)					*/
	TOKEN_REL_OP_NOT_EQUAL,		/* Comparison operation - not equal(!=)				*/
	TOKEN_REL_OP_LESS,			/* Comparison operation - less than(<)				*/
	TOKEN_REL_OP_LESS_EQUAL,	/* Comparison operation - less than equal to(<=)	*/
	TOKEN_REL_OP_GRATER,		/* Comparison operation - greater than(>)			*/
	TOKEN_REL_OP_GRATER_EQUAL,	/* Comparison operation - greater than equal to(>=)	*/

	// Others
	TOKEN_END_OF_FILE
} eTOKENS;

#define NUMBER_OF_TOKENS TOKEN_END_OF_FILE+1

typedef struct Token
{
	eTOKENS kind;
	char* lexeme;
	int lineNumber;
} Token;

typedef struct Node
{
	Token* tokensArray;
	struct Node* prev;
	struct Node* next;
} Node;

// Functions for the Node data structure

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);

Node* create_new_node();
Token* next_token();
Token* back_token();

/* Additional functions */

int print_and_create_token(eTOKENS kind, char* lexeme, int numOfLine);

void initialize_token_list();
void free_tokens();

const char* get_eToken_name(eTOKENS kind);
const char* get_token_name(eTOKENS kind);

#endif

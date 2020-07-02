#include "Token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

#define TOKEN_ARRAY_SIZE 1000

int currentIndex = 0;
int backSteps = 0;
Node* currentNode = NULL;
Node* headNode = NULL;


Node* create_new_node()
{
	Node * newNode = (Node*)malloc(sizeof(Node));

	if (newNode == NULL)
	{
		fprintf(yyout, "\nUnable to allocate memory! \n");
		exit(0);
	}
	newNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
	if (newNode->tokensArray == NULL)
	{
		fprintf(yyout, "\nUnable to allocate memory! \n");
		exit(0);
	}
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	int length = strlen(lexeme) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = create_new_node();
		headNode = currentNode;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = create_new_node();

			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif		
}



int print_and_create_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	fprintf(yyout, "Token of type '{%s}', lexeme: '{%s}', found in line: {%d}.\n", get_eToken_name(kind), lexeme, numOfLine);
	create_and_store_token(kind, lexeme, numOfLine);
	return (int)kind;
}

void initialize_token_list()
{
	currentNode = headNode;
	currentIndex = 0;
}

void free_tokens()
{
	currentNode = headNode;
	while (currentNode != NULL) 
	{
		free(currentNode->tokensArray);
		headNode = currentNode->next;
		free(currentNode);
		currentNode = headNode;
	}
	currentIndex = 0;
}

int yywrap() { return 1; }

const char* get_eToken_name(eTOKENS kind)
{
	switch (kind)
	{
	case TOKEN_INTEGER:				return "TOKEN_INTEGER";
	case TOKEN_FLOAT:				return "TOKEN_FLOAT";
	case TOKEN_COMMA:				return "TOKEN_COMMA";
	case TOKEN_COLON:				return "TOKEN_COLON";
	case TOKEN_SEMICOLON:			return "TOKEN_SEMICOLON";
	case TOKEN_OPEN_PARENTHESES:	return "TOKEN_OPEN_PARENTHESES";
	case TOKEN_CLOSE_PARENTHESES:	return "TOKEN_CLOSE_PARENTHESES";
	case TOKEN_OPEN_BRACKETS:		return "TOKEN_OPEN_BRACKETS";
	case TOKEN_CLOSE_BRACKETS: 		return "TOKEN_CLOSE_BRACKETS";
	case TOKEN_OPEN_CURLY_BRACES:	return "TOKEN_OPEN_CURLY_BRACES";
	case TOKEN_CLOSE_CURLY_BRACES:	return "TOKEN_CLOSE_CURLY_BRACES";
	case TOKEN_IDENTIFIER:			return "TOKEN_IDENTIFIER";
	case TOKEN_KEYWORD_INT:			return "TOKEN_KEYWORD_INT";
	case TOKEN_KEYWORD_FLOAT:		return "TOKEN_KEYWORD_FLOAT";
	case TOKEN_KEYWORD_VOID:		return "TOKEN_KEYWORD_VOID";
	case TOKEN_KEYWORD_IF:			return "TOKEN_KEYWORD_IF";
	case TOKEN_KEYWORD_RETURN:		return "TOKEN_KEYWORD_RETURN";
	case TOKEN_OP_ASSIGN:			return "TOKEN_OP_ASSIGN";
	case TOKEN_AR_OP_ADD:			return "TOKEN_AR_OP_ADD";
	case TOKEN_AR_OP_MUL:			return "TOKEN_AR_OP_MUL";
	case TOKEN_REL_OP_EQUAL:		return "TOKEN_REL_OP_EQUAL";
	case TOKEN_REL_OP_NOT_EQUAL:	return "TOKEN_REL_OP_NOT_EQUAL";
	case TOKEN_REL_OP_LESS:			return "TOKEN_REL_OP_LESS";
	case TOKEN_REL_OP_LESS_EQUAL:	return "TOKEN_REL_OP_LESS_EQUAL";
	case TOKEN_REL_OP_GRATER:		return "TOKEN_REL_OP_GRATER";
	case TOKEN_REL_OP_GRATER_EQUAL:	return "TOKEN_REL_OP_GRATER_EQUAL";
	case TOKEN_END_OF_FILE:			return "TOKEN_END_OF_FILE";
	default:
		return NULL;
	}
}

const char* get_token_name(eTOKENS kind)
{
	switch (kind)
	{
	case TOKEN_INTEGER:				return "int_num";
	case TOKEN_FLOAT:				return "float_num";
	case TOKEN_COMMA:				return "comma";
	case TOKEN_COLON:				return "colon";
	case TOKEN_SEMICOLON:			return "semicolon";
	case TOKEN_OPEN_PARENTHESES:	return "(";
	case TOKEN_CLOSE_PARENTHESES:	return ")";
	case TOKEN_OPEN_BRACKETS:		return "[";
	case TOKEN_CLOSE_BRACKETS: 		return "]";
	case TOKEN_OPEN_CURLY_BRACES:	return "{";
	case TOKEN_CLOSE_CURLY_BRACES:	return "}";
	case TOKEN_IDENTIFIER:			return "Identifier";
	case TOKEN_KEYWORD_INT:			return "int";
	case TOKEN_KEYWORD_FLOAT:		return "float";
	case TOKEN_KEYWORD_VOID:		return "void";
	case TOKEN_KEYWORD_IF:			return "if";
	case TOKEN_KEYWORD_RETURN:		return "return";
	case TOKEN_OP_ASSIGN:			return "=";
	case TOKEN_AR_OP_ADD:			return "+";
	case TOKEN_AR_OP_MUL:			return "*";
	case TOKEN_REL_OP_EQUAL:		return "==";
	case TOKEN_REL_OP_NOT_EQUAL:	return "!=";
	case TOKEN_REL_OP_LESS:			return "<";
	case TOKEN_REL_OP_LESS_EQUAL:	return "<=";
	case TOKEN_REL_OP_GRATER:		return ">";
	case TOKEN_REL_OP_GRATER_EQUAL:	return ">=";
	case TOKEN_END_OF_FILE:			return "EOF";
	default:
		return NULL;
	}
}



/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token()
{
	if (backSteps > 0) {
		backSteps--;
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			if (currentNode->next == NULL) 
			{
				exit(0);
			}
			currentNode = currentNode->next;
			currentIndex = 0;
		}
		else {
			currentIndex++;
		}
		return &(currentNode->tokensArray[currentIndex]);
	}
	else {
		int t = 0;
		backSteps = 0;
		if (currentNode == NULL) {
			currentIndex = 0;
		}
		if (currentIndex == TOKEN_ARRAY_SIZE - 1) {
			currentIndex = 0;
			currentNode->next = create_new_node();
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
		}
		do {
			t = yylex();
		} while (t == -1);
		return &(currentNode->tokensArray[currentIndex]);
	}
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token* back_token()
{
	if (currentNode == NULL) 
	{
		exit(0);
	}
	backSteps++;
	if (currentIndex == 0)
	{
		if (currentNode->prev == NULL)
		{
			exit(1);
		}
		currentNode = currentNode->prev;
		currentIndex = TOKEN_ARRAY_SIZE - 1;
	}
	else {
		currentIndex--;
	}
	return &(currentNode->tokensArray[currentIndex]);
}


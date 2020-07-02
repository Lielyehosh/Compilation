#include "Parser.h"

#define MAX_EXPECTED_TOKENS_ARRAY 1000
#define SEMANTIC_ERROR_MSG_SIZE 250

FILE *syntactic_out = NULL;
Token* tok = NULL;
char expected_tokens[MAX_EXPECTED_TOKENS_ARRAY] = { 0 };
char semantic_error_msg[SEMANTIC_ERROR_MSG_SIZE];

void print_rule(const char* rule)
{
	fprintf(syntactic_out, "Rule (%s) \n",rule);
}

void print_semantic_error(const char* error_msg)
{
	fprintf(yyout_semantic, "Line[%d]\t- Error: %s \n", tok->lineNumber, error_msg);
}

void print_semantic_warning(const char* warning_msg)
{
	fprintf(yyout_semantic, "Line[%d]\t- Warning: %s \n", tok->lineNumber, warning_msg);
}

int Match(int type)
{
	// Token* nextToken = next_token();
	/*if (type != nextToken->kind)
	{
		fprintf(syntactic_out, "Expected token of type '{ %s }' at line: {%d}, \nActual token of type '{ %s }', lexeme: '{%s}'.\n", get_token_name(type), nextToken->lineNumber, get_token_name(nextToken->kind), nextToken->lexeme);
		return 0;
	}*/
	tok = next_token();
	if (type != tok->kind)
	{
		fprintf(syntactic_out, "Expected token of type '{ %s }' at line: {%d}, \nActual token of type '{ %s }', lexeme: '{%s}'.\n", get_token_name(type), tok->lineNumber, get_token_name(tok->kind), tok->lexeme);
		return 0;
	}
	return 1;
}


void Error_Recovery(eVariable variable,const char* expected_tokens) {
	fprintf(syntactic_out, "Expected token of type '{%s}' at line: {%d}, \nActual token of type '{%s}', lexeme: '{%s}'.\n", expected_tokens, tok->lineNumber, get_token_name(tok->kind), tok->lexeme);
	do {
		tok = next_token();
	} while ((Follow(variable, tok->kind) == 0) && (tok->kind != TOKEN_END_OF_FILE));
	tok = back_token();
}

int Follow(eVariable var, eTOKENS token) {
	int var_follow_array[NUMBER_OF_TOKENS] = { 0 };
	switch (var)
	{
		case PROG:
		case FUNC_FULL_DEFS:
		case FUNC_FULL_DEFS_1:
			var_follow_array[TOKEN_END_OF_FILE] = 1;
			break;
		case GLOBAL_VARS:
		case GLOBAL_VARS_1:
		case FUNC_PREDEFS:
		case FUNC_PREDEFS_1:
			var_follow_array[TOKEN_KEYWORD_INT] = 1;
			var_follow_array[TOKEN_KEYWORD_FLOAT] = 1;
			var_follow_array[TOKEN_KEYWORD_VOID] = 1;
			break;
		case FUNC_PROTOTYPE:
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_OPEN_CURLY_BRACES] = 1;
			break;
		case VAR:
		case VAR_1:
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			var_follow_array[TOKEN_AR_OP_ADD] = 1;
			var_follow_array[TOKEN_AR_OP_MUL] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_COMMA] = 1;
			var_follow_array[TOKEN_REL_OP_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_NOT_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_LESS] = 1;
			var_follow_array[TOKEN_REL_OP_LESS_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER_EQUAL] = 1;
			var_follow_array[TOKEN_OP_ASSIGN] = 1;
			break;
		case VAR_DEC:
		case VAR_DEC_1:
			var_follow_array[TOKEN_KEYWORD_INT] = 1;
			var_follow_array[TOKEN_KEYWORD_FLOAT] = 1;
			var_follow_array[TOKEN_KEYWORD_VOID] = 1;
			var_follow_array[TOKEN_IDENTIFIER] = 1;
			var_follow_array[TOKEN_KEYWORD_IF] = 1;
			var_follow_array[TOKEN_KEYWORD_RETURN] = 1;
			var_follow_array[TOKEN_OPEN_CURLY_BRACES] = 1;
			break;
		case VAR_DEC_LIST:
		case VAR_DEC_LIST_1:
			var_follow_array[TOKEN_IDENTIFIER] = 1;
			var_follow_array[TOKEN_KEYWORD_IF] = 1;
			var_follow_array[TOKEN_KEYWORD_RETURN] = 1;
			var_follow_array[TOKEN_OPEN_CURLY_BRACES] = 1;
			break;
		case TYPE:
		case RETURNED_TYPE:
			var_follow_array[TOKEN_IDENTIFIER] = 1;
			break;
		case DIM_SIZES:
		case DIM_SIZES_1:
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			break;
		case PARAM:
		case PARAM_1:
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			var_follow_array[TOKEN_COMMA] = 1;
			break;
		case PARAMS:
		case PARAM_LIST:
		case PARAM_LIST_1:
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			break;
		case FUNC_WITH_BODY:
			var_follow_array[TOKEN_KEYWORD_INT] = 1;
			var_follow_array[TOKEN_KEYWORD_FLOAT] = 1;
			var_follow_array[TOKEN_KEYWORD_VOID] = 1;
			var_follow_array[TOKEN_END_OF_FILE] = 1;
			break;
		case COMP_STMT:
			var_follow_array[TOKEN_KEYWORD_INT] = 1;
			var_follow_array[TOKEN_KEYWORD_FLOAT] = 1;
			var_follow_array[TOKEN_KEYWORD_VOID] = 1;
			var_follow_array[TOKEN_END_OF_FILE] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			break;
		case STMT_LIST:
		case STMT_LIST_1:
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			break;
		case STMT:
		case IF_STMT:
		case RETURN_STMT:
		case RETURN_STMT_1:
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			break;
		case EXPR:
		case EXPR_1:
			// ) , ; ] rel_op }
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_COMMA] = 1;
			var_follow_array[TOKEN_REL_OP_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_NOT_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_LESS] = 1;
			var_follow_array[TOKEN_REL_OP_LESS_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER_EQUAL] = 1;
			break;
		case EXPR_LIST:
		case EXPR_LIST_1:
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			break;
		case CALL:
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			var_follow_array[TOKEN_AR_OP_ADD] = 1;
			var_follow_array[TOKEN_AR_OP_MUL] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_COMMA] = 1;
			var_follow_array[TOKEN_REL_OP_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_NOT_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_LESS] = 1;
			var_follow_array[TOKEN_REL_OP_LESS_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER_EQUAL] = 1;
			break;
		case ARGS:
		case ARG_LIST:
		case ARG_LIST_1:
		case CONDITION:
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			break;
		case TERM:
		case TERM_1:
			// ) + , ; ] rel_op }
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_COMMA] = 1;
			var_follow_array[TOKEN_REL_OP_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_NOT_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_LESS] = 1;
			var_follow_array[TOKEN_REL_OP_LESS_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER_EQUAL] = 1;
			var_follow_array[TOKEN_AR_OP_ADD] = 1;
			break;
		case FACTOR:
			var_follow_array[TOKEN_CLOSE_CURLY_BRACES] = 1;
			var_follow_array[TOKEN_CLOSE_PARENTHESES] = 1;
			var_follow_array[TOKEN_CLOSE_BRACKETS] = 1;
			var_follow_array[TOKEN_SEMICOLON] = 1;
			var_follow_array[TOKEN_COMMA] = 1;
			var_follow_array[TOKEN_REL_OP_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_NOT_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_LESS] = 1;
			var_follow_array[TOKEN_REL_OP_LESS_EQUAL] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER] = 1;
			var_follow_array[TOKEN_REL_OP_GRATER_EQUAL] = 1;
			var_follow_array[TOKEN_AR_OP_ADD] = 1;
			var_follow_array[TOKEN_AR_OP_MUL] = 1;
			break;
	default:
		return 0;
	}
	return var_follow_array[token];
}



void insert_to_current_table(const char* key, ENTRY_ROLE role, ENTRY_TYPE type, void* list)
{
	entry_pair* entry = NULL;
	entry = cur_table->insert(cur_table, key);
	if (entry != NULL) {
		fprintf(yyout_semantic, "Line[%i] \t-\t",line_idx);
		entry->set_role(entry, role);
		entry->set_type(entry, type);
		entry->set_list(entry, list);
		print_single_entry(entry, yyout_semantic);
	}
	else {
		sprintf(semantic_error_msg, "The identifier [%s] is already exists in the symbol table", key);
		print_semantic_error(semantic_error_msg);
	}
}

VAR_ATTR* create_var_attr()
{
	VAR_ATTR* var = (VAR_ATTR*)malloc(sizeof(VAR_ATTR) * 1);
	if (var == NULL) {
		print_error_msg("Failed to allocate memory to VAR_ATTR object");
		exit(1);
	}
	var->id = NULL;
	var->dim_list = NULL;
	var->param_list = NULL;
	var->type = ERROR_TYPE;
	var->value = -1;
	return var;
}

void delete_var_attr(VAR_ATTR* var)
{
	if (var != NULL) {
		delete_dim_list(var->dim_list);
		delete_param_list(var->param_list);
		if (var->id != NULL)
			free(var->id);
	}
}

void parser()
{
	syntactic_out = yyout_parser;
	parse();
}


void parse()
{
	Parse_PROG();
}

void Parse_PROG()
{
	print_rule("PROG -> BB GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS FB");
	Parse_BB();
	Parse_GLOBAL_VARS();
	Parse_FUNC_PREDEFS();
	Parse_FUNC_FULL_DEFS();
	Parse_FB();
	Match(TOKEN_END_OF_FILE);
}

void Parse_FUNC_PREDEFS()
{
	VAR_ATTR* FUNC_PROTOTYPE_attr = NULL;
	print_rule("FUNC_PREDEFS -> FUNC_PROTOTYPE ; FUNC_PREDEFS_1");
	FUNC_PROTOTYPE_attr = Parse_FUNC_PROTOTYPE();

	// insert a new function definition to the symbol table
	insert_to_current_table(FUNC_PROTOTYPE_attr->id, PRE_DEFINITION, FUNC_PROTOTYPE_attr->type, clone_param_list(FUNC_PROTOTYPE_attr->param_list));

	delete_var_attr(FUNC_PROTOTYPE_attr);

	Match(TOKEN_SEMICOLON);
	Parse_FUNC_PREDEFS_1();
}

/* 
	Nullable
	First(FUNC_PREDEFS_1) = int float void
	Follow(FUNC_PREDEFS_1) = int float void
*/
void Parse_FUNC_PREDEFS_1()
{
	VAR_ATTR* FUNC_PROTOTYPE_attr = NULL;
	int steps = 0;
	tok = next_token();
	while (tok != NULL && tok->kind != TOKEN_CLOSE_PARENTHESES && tok->kind != TOKEN_END_OF_FILE) {
		tok = next_token();
		steps++;
	}
	tok = next_token();
	steps++;
	if (tok->kind == TOKEN_SEMICOLON) {
		print_rule("FUNC_PREDEFS_1 -> FUNC_PROTOTYPE ; FUNC_PREDEFS_1");
		while (steps >= 0) {
			tok = back_token();
			steps--;
		}
		// insert a new function definition to the symbol table
		FUNC_PROTOTYPE_attr = Parse_FUNC_PROTOTYPE();
		insert_to_current_table(FUNC_PROTOTYPE_attr->id, PRE_DEFINITION, FUNC_PROTOTYPE_attr->type, clone_param_list(FUNC_PROTOTYPE_attr->param_list));

		delete_var_attr(FUNC_PROTOTYPE_attr);

		Match(TOKEN_SEMICOLON);
		Parse_FUNC_PREDEFS_1();
		return;
	}
	else if (tok->kind == TOKEN_OPEN_CURLY_BRACES) {
		print_rule("FUNC_PREDEFS_1 -> epsilon");
		while (steps >= 0) {
			tok = back_token();
			steps--;
		}
		return;
	}
	else {
		while (steps >= 0) {
			tok = back_token();
			steps--;
		}
		sprintf(expected_tokens, "%s , %s", get_token_name(TOKEN_SEMICOLON),get_token_name(TOKEN_OPEN_CURLY_BRACES));
		Error_Recovery(FUNC_PREDEFS_1,expected_tokens);
	}
}

void Parse_FUNC_FULL_DEFS()
{
	print_rule("FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS_1");
	Parse_FUNC_WITH_BODY();
	Parse_FUNC_FULL_DEFS_1();
}

/* 
	Nullable
	First(FUNC_FULL_DEFS_1) = int float void
	Follow(FUNC_FULL_DEFS_1) = EOF
*/
void Parse_FUNC_FULL_DEFS_1()
{
	entry_pair* entry = NULL;
	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(FUNC_FULL_DEFS_1)
	case TOKEN_KEYWORD_INT:
	case TOKEN_KEYWORD_FLOAT:
	case TOKEN_KEYWORD_VOID:
		print_rule("FUNC_FULL_DEFS_1 -> FUNC_FULL_DEFS");
		tok = back_token();
		Parse_FUNC_FULL_DEFS();
		break;
	// case token in Follow(FUNC_FULL_DEFS_1)
	case TOKEN_END_OF_FILE:
		print_rule("FUNC_FULL_DEFS_1 -> epsilon");

		entry = is_no_pre_definitions(cur_table);
		if (entry != NULL) {
			sprintf(semantic_error_msg, "Function [%s] doesn't has a full definition", entry->get_name(entry));
			print_semantic_warning(semantic_error_msg);
		}

		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, "%s , %s , %s , %s", get_token_name(TOKEN_KEYWORD_INT), get_token_name(TOKEN_KEYWORD_FLOAT),get_token_name(TOKEN_KEYWORD_VOID),get_token_name(TOKEN_END_OF_FILE));
		Error_Recovery(FUNC_FULL_DEFS_1, expected_tokens);
		break;
	}
}

void Parse_GLOBAL_VARS()
{
	print_rule("GLOBAL_VARS -> VAR_DEC GLOBAL_VARS_1");
	Parse_VAR_DEC();
	Parse_GLOBAL_VARS_1();
}

/*
	Nullable
	First(GLOBAL_VARS_1) = int float
	Follow(GLOBAL_VARS_1) = int float void
	if token is int or float, then the next tokens will decide whether it epsilon or VAR_DEC.
	First token = int OR float
	Second token = id
	Third token:
		- if ';' or '[' then GLOBAL_VARS_1 -> VAR_DEC GLOBAL_VARS_1
		- if '(' then GLOBAL_VARS_1 -> epsilon
*/
void Parse_GLOBAL_VARS_1()
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_VOID:
		print_rule("GLOBAL_VARS_1 -> epsilon");
		tok = back_token();
		break;
	case TOKEN_KEYWORD_INT:
	case TOKEN_KEYWORD_FLOAT:
		tok = next_token();
		if (tok->kind == TOKEN_IDENTIFIER) {
			tok = next_token();
			switch (tok->kind)
			{
			case TOKEN_SEMICOLON:
			case TOKEN_OPEN_BRACKETS:
				print_rule("GLOBAL_VARS_1 -> VAR_DEC GLOBAL_VARS_1");
				tok = back_token();
				tok = back_token();
				tok = back_token();
				Parse_VAR_DEC();
				Parse_GLOBAL_VARS_1();
				break;
			case TOKEN_OPEN_PARENTHESES:
				print_rule("GLOBAL_VARS_1 -> epsilon");
				tok = back_token();
				tok = back_token();
				tok = back_token();
				break;
			default:
				sprintf(expected_tokens, "%s , %s , %s", get_token_name(TOKEN_SEMICOLON), get_token_name(TOKEN_OPEN_BRACKETS), get_token_name(TOKEN_OPEN_PARENTHESES));
				Error_Recovery(GLOBAL_VARS_1, expected_tokens);
				break;
			}
			break;
		}
		else {
			sprintf(expected_tokens, " %s ", get_token_name(TOKEN_IDENTIFIER));
			Error_Recovery(GLOBAL_VARS_1, expected_tokens);
		}
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s ", get_token_name(TOKEN_KEYWORD_INT), get_token_name(TOKEN_KEYWORD_FLOAT), get_token_name(TOKEN_KEYWORD_VOID));
		Error_Recovery(GLOBAL_VARS_1, expected_tokens);
		return;;
	}

}

VAR_ATTR* Parse_FUNC_PROTOTYPE()
{
	VAR_ATTR* FUNC_PROTOTYPE_attr = create_var_attr();
	VAR_ATTR* RETURNED_TYPE_attr = NULL;
	print_rule("FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )");
	
	// copy the return type attribute
	RETURNED_TYPE_attr = Parse_RETURNED_TYPE();
	FUNC_PROTOTYPE_attr->type = RETURNED_TYPE_attr->type;
	delete_var_attr(RETURNED_TYPE_attr);
	
	// copy the id attribute
	Match(TOKEN_IDENTIFIER);
	FUNC_PROTOTYPE_attr->id = clone_str(tok->lexeme);

	// update the param_list if it necessary
	Match(TOKEN_OPEN_PARENTHESES);
	Parse_PARAMS(&(FUNC_PROTOTYPE_attr->param_list));
	Match(TOKEN_CLOSE_PARENTHESES);

	return FUNC_PROTOTYPE_attr;
}

VAR_ATTR* Parse_VAR()
{
	VAR_ATTR* VAR_attr = create_var_attr();
	VAR_ATTR* VAR_1_attr = NULL;
	entry_pair* entry = NULL;
	dim_list* d_list = NULL;
	print_rule("VAR -> id VAR_1");
	Match(TOKEN_IDENTIFIER);

	VAR_attr->id = clone_str(tok->lexeme);
	entry = cur_table->find(cur_table, VAR_attr->id);
	if (entry == NULL) {
		sprintf(semantic_error_msg, "The identifier [%s] doesn't found in the symbol table", VAR_attr->id);
		print_semantic_error(semantic_error_msg);	
	}
	else if (entry->get_role(entry) != VARIABLE && entry->get_role(entry) != USED_VARIABLE) {
		sprintf(semantic_error_msg, "The identifier [%s] doesn't declared as a variable", VAR_attr->id);
		print_semantic_error(semantic_error_msg);
	}
	else {
		d_list = (dim_list*)entry->get_list(entry);

		// set the var to a used variable
		entry->set_role(entry, USED_VARIABLE);

		VAR_1_attr = Parse_VAR_1(d_list,entry->get_type(entry), VAR_attr->id);
		VAR_attr->type = VAR_1_attr->type;
		delete_var_attr(VAR_1_attr);

	}
	return VAR_attr;
}

/* 
	Nullable
	First(VAR_1) = [
	Follow(VAR_1) = ) * + , ; = ] rel_op }
*/
VAR_ATTR* Parse_VAR_1(dim_list* d_list, ENTRY_TYPE expected_type,const char* id)
{
	VAR_ATTR* VAR_1_attr = create_var_attr();
	VAR_ATTR* EXPR_LIST_attr = NULL;
	dim_node* node = NULL;

	// set the type to a non error type value
	VAR_1_attr->type = expected_type;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(VAR_1)
	case TOKEN_OPEN_BRACKETS:
		print_rule("VAR_1 -> [ EXPR_LIST ]");

		// check if the variable is an array
		if (d_list == NULL) {
			sprintf(semantic_error_msg, "The Identifier [%s] is not declared as an array", id);
			print_semantic_error(semantic_error_msg);
			VAR_1_attr->type = ERROR_TYPE;
		}
		else {
			node = d_list->head;
		}
		
		EXPR_LIST_attr = Parse_EXPR_LIST(node);
		if (EXPR_LIST_attr == ERROR_TYPE) {
			VAR_1_attr = ERROR_TYPE;
		}
		delete_var_attr(EXPR_LIST_attr);

		Match(TOKEN_CLOSE_BRACKETS);
		break;
	// case token in Follow(VAR_1)
	case TOKEN_CLOSE_PARENTHESES:		// )
	case TOKEN_CLOSE_CURLY_BRACES:		// }
	case TOKEN_CLOSE_BRACKETS:			// ]
	case TOKEN_AR_OP_MUL:				// *
	case TOKEN_AR_OP_ADD:				// +
	case TOKEN_OP_ASSIGN:				// =
	case TOKEN_COMMA:					// ,
	case TOKEN_SEMICOLON:				// ;
	case TOKEN_REL_OP_EQUAL:
	case TOKEN_REL_OP_NOT_EQUAL:
	case TOKEN_REL_OP_LESS:
	case TOKEN_REL_OP_LESS_EQUAL:
	case TOKEN_REL_OP_GRATER:
	case TOKEN_REL_OP_GRATER_EQUAL:
		print_rule("VAR_1 -> epsilon");
		tok = back_token();
		
		// check if the dim_list is NULL (if it is a non-array variable)
		if (d_list != NULL) {
			sprintf(semantic_error_msg, "The Identifier [%s] declared as an array", id);
			print_semantic_error(semantic_error_msg);
			VAR_1_attr->type = ERROR_TYPE;
		}

		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s ", get_token_name(TOKEN_OPEN_BRACKETS),
			get_token_name(TOKEN_CLOSE_PARENTHESES),
			get_token_name(TOKEN_CLOSE_CURLY_BRACES),
			get_token_name(TOKEN_CLOSE_BRACKETS),
			get_token_name(TOKEN_AR_OP_MUL),
			get_token_name(TOKEN_AR_OP_ADD),
			get_token_name(TOKEN_OP_ASSIGN),
			get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_SEMICOLON),
			get_token_name(TOKEN_REL_OP_EQUAL),
			get_token_name(TOKEN_REL_OP_NOT_EQUAL),
			get_token_name(TOKEN_REL_OP_LESS),
			get_token_name(TOKEN_REL_OP_LESS_EQUAL),
			get_token_name(TOKEN_REL_OP_GRATER),
			get_token_name(TOKEN_REL_OP_GRATER_EQUAL));
		Error_Recovery(VAR_1, expected_tokens);
		break;
	}
	return VAR_1_attr;
}

void Parse_VAR_DEC()
{
	VAR_ATTR* VAR_DEC_1_attr = NULL;
	VAR_ATTR* TYPE_attr = NULL;
	char* id_lexeme;
	void* list;
	print_rule("VAR_DEC -> TYPE id VAR_DEC_1");

	TYPE_attr = Parse_TYPE();
	Match(TOKEN_IDENTIFIER);
	id_lexeme = clone_str(tok->lexeme);
	VAR_DEC_1_attr = Parse_VAR_DEC_1();
	list = clone_dim_list(VAR_DEC_1_attr->dim_list);
	insert_to_current_table(id_lexeme, VARIABLE, TYPE_attr->type, list);
	delete_var_attr(VAR_DEC_1_attr);
	delete_var_attr(TYPE_attr);
	free(id_lexeme);
}

VAR_ATTR* Parse_VAR_DEC_1()
{
	VAR_ATTR* VAR_DEC_1_attr = create_var_attr();
	tok = next_token();
	switch (tok->kind)
	{
		case TOKEN_SEMICOLON:
			print_rule("VAR_DEC_1 -> ;");
			VAR_DEC_1_attr->dim_list = NULL;
			break;
		case TOKEN_OPEN_BRACKETS:
			print_rule("VAR_DEC_1 -> [ DIM_SIZES ] ;");
			// pass a dim_list attribute
			VAR_DEC_1_attr->dim_list = create_dim_list();
			Parse_DIM_SIZES(VAR_DEC_1_attr->dim_list);
			Match(TOKEN_CLOSE_BRACKETS);
			Match(TOKEN_SEMICOLON);
			break;
		default:
			sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_SEMICOLON),
				get_token_name(TOKEN_OPEN_BRACKETS));
			Error_Recovery(VAR_DEC_1, expected_tokens);
			break;
	}
	return VAR_DEC_1_attr;
}

void Parse_VAR_DEC_LIST()
{
	print_rule("VAR_DEC_LIST -> VAR_DEC_LIST_1");
	Parse_VAR_DEC_LIST_1();
}

/* 
	Nullable
	First(VAR_DEC_LIST_1) = int float
	Follow(VAR_DEC_LIST_1) = id if return {
 */
 void Parse_VAR_DEC_LIST_1()
{
	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(VAR_DEC_LIST_1)
	case TOKEN_KEYWORD_INT:
	case TOKEN_KEYWORD_FLOAT:
		print_rule("VAR_DEC_LIST_1 -> VAR_DEC VAR_DEC_LIST_1");
		tok = back_token();
		Parse_VAR_DEC();
		Parse_VAR_DEC_LIST_1();
		break;
	// case token in Follow(VAR_DEC_LIST_1)
	case TOKEN_KEYWORD_IF:
	case TOKEN_KEYWORD_RETURN:
	case TOKEN_IDENTIFIER:
	case TOKEN_OPEN_CURLY_BRACES:
		print_rule("VAR_DEC_LIST_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s , %s , %s ", get_token_name(TOKEN_KEYWORD_INT),
			get_token_name(TOKEN_KEYWORD_FLOAT),
			get_token_name(TOKEN_KEYWORD_IF),
			get_token_name(TOKEN_KEYWORD_RETURN),
			get_token_name(TOKEN_IDENTIFIER),
			get_token_name(TOKEN_OPEN_CURLY_BRACES));
		Error_Recovery(VAR_DEC_LIST_1, expected_tokens);
		break;
	}
}

/*
This function doesn't return a var_attr - it just insert elements to the dim_list (recursive)
*/
void Parse_DIM_SIZES(dim_list* list)
{
	print_rule("DIM_SIZES -> int_num DIM_SIZES_1");
	Match(TOKEN_INTEGER);
	list->insert(list, tok->lexeme);
	Parse_DIM_SIZES_1(list);
}

/* 
	Nullable
	First(DIM_SIZES_1) = ,
	Follow(DIM_SIZES_1) = ]
*/
void Parse_DIM_SIZES_1(dim_list* list)
{
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_COMMA:
		print_rule("DIM_SIZES_1 -> , DIM_SIZES");
		Parse_DIM_SIZES(list);
		break;
	// case tok in Follow(DIM_SIZES_1)
	case TOKEN_CLOSE_BRACKETS:
		print_rule("DIM_SIZES_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_CLOSE_BRACKETS));
		Error_Recovery(DIM_SIZES_1,expected_tokens);
		break;
	}
}

VAR_ATTR* Parse_RETURNED_TYPE()
{
	VAR_ATTR* RETURNED_TYPE_attr = create_var_attr();
	VAR_ATTR* TYPE_attr = NULL;
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_VOID:
		print_rule("RETURNED_TYPE -> void");
		RETURNED_TYPE_attr->type = VOID;
		break;
	// case token in First(TYPE)
	case TOKEN_KEYWORD_FLOAT:
	case TOKEN_KEYWORD_INT:
		print_rule("RETURNED_TYPE -> TYPE");
		tok = back_token();

		// copy the return type attribute
		TYPE_attr = Parse_TYPE();
		RETURNED_TYPE_attr->type = TYPE_attr->type;
		delete_var_attr(TYPE_attr);

		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s ", get_token_name(TOKEN_KEYWORD_VOID),
			get_token_name(TOKEN_KEYWORD_FLOAT),
			get_token_name(TOKEN_KEYWORD_INT));
		Error_Recovery(RETURNED_TYPE,expected_tokens);
		break;
	}
	return RETURNED_TYPE_attr;
}

VAR_ATTR* Parse_PARAM()
{
	VAR_ATTR* PARAM_attr = create_var_attr();
	VAR_ATTR* TYPE_attr = NULL;
	print_rule("PARAM -> TYPE id PARAM_1");

	// copy the type attribute
	TYPE_attr = Parse_TYPE();
	PARAM_attr->type = TYPE_attr->type;
	delete_var_attr(TYPE_attr);

	// copy the id attribute
	Match(TOKEN_IDENTIFIER);
	PARAM_attr->id = clone_str(tok->lexeme);

	// update the dim_list attribute
	Parse_PARAM_1(&PARAM_attr->dim_list);

	return PARAM_attr;
}


/*
	Nullable
	First(PARAM_1) = [
	Follow(PARAM_1) = ) ,
*/
void Parse_PARAM_1(dim_list** list)
{
	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(PARAM_1)
	case TOKEN_OPEN_BRACKETS:
		print_rule("PARAM_1 -> [ DIM_SIZES ]");
		*list = create_dim_list();
		Parse_DIM_SIZES(*list);
		Match(TOKEN_CLOSE_BRACKETS);
		break;
	// case token in Follow(PARAM_1)
	case TOKEN_CLOSE_PARENTHESES:		// )
	case TOKEN_COMMA:					// ,
		print_rule("PARAM_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s ", get_token_name(TOKEN_OPEN_BRACKETS),
			get_token_name(TOKEN_CLOSE_PARENTHESES),
			get_token_name(TOKEN_COMMA));
		Error_Recovery(PARAM_1,expected_tokens);
		break;
	}
}

/* 
	Nullable
	First(PARAMS) = int float
	Follow(PARAMS) = )
*/
void Parse_PARAMS(param_list** p_list)
{
	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(PARAMS)
	case TOKEN_KEYWORD_INT:
	case TOKEN_KEYWORD_FLOAT:
		print_rule("PARAMS -> PARAM_LIST");
		tok = back_token();
		// create a new param_list and update it by recursive calls
		*p_list = create_param_list();
		Parse_PARAM_LIST(*p_list);
		break;
	// case token in Follow(PARAMS)
	case TOKEN_CLOSE_PARENTHESES:
		print_rule("PARAMS -> epsilon");
		*p_list = NULL;
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s ", get_token_name(TOKEN_KEYWORD_INT),
			get_token_name(TOKEN_KEYWORD_FLOAT),
			get_token_name(TOKEN_CLOSE_PARENTHESES));
		Error_Recovery(PARAMS,expected_tokens);
		break;
	}
}

void Parse_PARAM_LIST(param_list* p_list)
{
	VAR_ATTR* PARAM_attr = NULL;
	print_rule("PARAM_LIST -> PARAM PARAM_LIST_1");
	
	// add a new node with attributes from PARAM_attr - { id, type, dim_list }
	PARAM_attr = Parse_PARAM();
	p_list->insert(p_list, PARAM_attr->id, PARAM_attr->type, PARAM_attr->dim_list);
	delete_var_attr(PARAM_attr);

	Parse_PARAM_LIST_1(p_list);
}

/*
	Nullable
	First(PARAM_LIST_1) = ,
	Follow(PARAM_LIST_1) = )
*/
void Parse_PARAM_LIST_1(param_list* p_list)
{
	VAR_ATTR* PARAM_attr = NULL;
	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(PARAM_LIST_1)
	case TOKEN_COMMA:
		print_rule("PARAM_LIST_1 -> , PARAM PARAM_LIST_1");
		PARAM_attr = Parse_PARAM();
		p_list->insert(p_list, PARAM_attr->id, PARAM_attr->type, PARAM_attr->dim_list);
		delete_var_attr(PARAM_attr);
		Parse_PARAM_LIST_1(p_list);
		break;
	// case token in Follow(PARAM_LIST_1)
	case TOKEN_CLOSE_PARENTHESES:
		print_rule("PARAM_LIST_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_CLOSE_PARENTHESES));
		Error_Recovery(PARAM_LIST_1, expected_tokens);
		break;
	}
}


void Parse_FUNC_WITH_BODY()
{
	VAR_ATTR* FUNC_PROTOTYPE_attr = NULL;
	VAR_ATTR* COMP_STMT_attr = NULL;
	entry_pair* entry = NULL;
	param_node* param = NULL;
	int valid_value;
	int has_return_stmt_flag = 0;
	print_rule("FUNC_WITH_BODY -> FUNC_PROTOTYPE COMP_STMT");

	FUNC_PROTOTYPE_attr = Parse_FUNC_PROTOTYPE();
	entry = cur_table->find(cur_table, FUNC_PROTOTYPE_attr->id);
	if (entry == NULL) {
		sprintf(semantic_error_msg, "The function [%s] doesn't have a pre-definition", FUNC_PROTOTYPE_attr->id);
		print_semantic_warning(semantic_error_msg);
		insert_to_current_table(FUNC_PROTOTYPE_attr->id, FULL_DEFINITION, FUNC_PROTOTYPE_attr->type, clone_param_list(FUNC_PROTOTYPE_attr->param_list));
		entry = cur_table->find(cur_table, FUNC_PROTOTYPE_attr->id);
	}
	/* Else -> case the entry isn't null and the function's id exist in the symbol table */
	else {
		if (entry->get_role(entry) == FULL_DEFINITION) {
			/* Case of duplicate function's id */
			sprintf(semantic_error_msg, "The function [%s] already has a full definition", FUNC_PROTOTYPE_attr->id);
			print_semantic_error(semantic_error_msg);
		}
		else if (entry->get_role(entry) != PRE_DEFINITION) {
			/* Case the function's id in the symbol table doesn't represent a function */
			sprintf(semantic_error_msg, "The identifier [%s] is not declared as a function", FUNC_PROTOTYPE_attr->id);
			print_semantic_error(semantic_error_msg);
		}
		else if (entry->get_type(entry) != FUNC_PROTOTYPE_attr->type) {
			/* Case the return type of the function pre-definition doesn't match to the function full definition */
			sprintf(semantic_error_msg, "The return type of the function [%s] does not match the return type in the declaration (expected: [%s], actual: [%s])",
				FUNC_PROTOTYPE_attr->id, get_entry_type_str(entry->get_type(entry)), get_entry_type_str(FUNC_PROTOTYPE_attr->type));
			print_semantic_error(semantic_error_msg);
			entry->set_type(entry, FUNC_PROTOTYPE_attr->type);
		}
	}

	Parse_BB();
	
	valid_value = check_param_list_validation(entry->get_list(entry), FUNC_PROTOTYPE_attr->param_list, FUNC_PROTOTYPE_attr->id, yyout_semantic, line_idx);
	
	if (valid_value == 0) {
		/* insert the input parameters to the symbol table */
		param = (FUNC_PROTOTYPE_attr->param_list == NULL) ? NULL : FUNC_PROTOTYPE_attr->param_list->head;
		while (param != NULL) {
			insert_to_current_table(param->name, VARIABLE, param->type, clone_dim_list(param->d_list));
			param = param->next_node;
		}
	}

	/* passing the expected return type to make sure there is a return statement that matches the expected return value */
	COMP_STMT_attr = Parse_COMP_STMT(FUNC_PROTOTYPE_attr->type, &has_return_stmt_flag);
	if (FUNC_PROTOTYPE_attr->type != VOID && has_return_stmt_flag == 0) {
		sprintf(semantic_error_msg, "Missing a return statement in the function [%s]", FUNC_PROTOTYPE_attr->id);
		print_semantic_error(semantic_error_msg);
	}

	Parse_FB();

	// set the function id entry to full definition 
	entry->set_role(entry, FULL_DEFINITION);

	fprintf(yyout_semantic, "Line[%d]\t-\tFunction [%s] Has a full definition\n", line_idx, entry->get_name(entry));

	delete_var_attr(FUNC_PROTOTYPE_attr);
	delete_var_attr(COMP_STMT_attr);
}

VAR_ATTR* Parse_COMP_STMT(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag)
{
	VAR_ATTR* COMP_STMT_attr = create_var_attr();
	VAR_ATTR* STMT_LIST_attr = NULL;

	print_rule("COMP_STMT -> { VAR_DEC_LIST STMT_LIST }");
	Match(TOKEN_OPEN_CURLY_BRACES);
	Parse_VAR_DEC_LIST();

	STMT_LIST_attr = Parse_STMT_LIST(expected_ret_type, has_return_stmt_flag);
	delete_var_attr(STMT_LIST_attr);

	Match(TOKEN_CLOSE_CURLY_BRACES);
	return COMP_STMT_attr;
}

VAR_ATTR* Parse_STMT_LIST(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag)
{
	VAR_ATTR* STMT_LIST_attr = create_var_attr();
	VAR_ATTR* STMT_LIST_attr_1 = NULL;
	VAR_ATTR* STMT_attr = NULL;
	print_rule("STMT_LIST -> STMT STMT_LIST_1");

	STMT_attr = Parse_STMT(expected_ret_type, has_return_stmt_flag);
	delete_var_attr(STMT_attr);

	STMT_LIST_attr_1 = Parse_STMT_LIST_1(expected_ret_type, has_return_stmt_flag);
	delete_var_attr(STMT_LIST_attr_1);

	return STMT_LIST_attr;
}

/*
	Nullable
	First(STMT_LIST_1) = ;
	Follow(STMT_LIST_1) = }
*/
VAR_ATTR* Parse_STMT_LIST_1(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag)
{
	VAR_ATTR* STMT_LIST_1_attr = create_var_attr();
	VAR_ATTR* STMT_LIST_1_attr_2 = NULL;
	VAR_ATTR* STMT_attr = NULL;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(STMT_LIST_1)
	case TOKEN_SEMICOLON:
		print_rule("STMT_LIST_1 -> ; STMT STMT_LIST_1");
		
		// check the return type attribute validation
		STMT_attr = Parse_STMT(expected_ret_type, has_return_stmt_flag);
		delete_var_attr(STMT_attr);

		STMT_LIST_1_attr_2 = Parse_STMT_LIST_1(expected_ret_type, has_return_stmt_flag);
		delete_var_attr(STMT_LIST_1_attr_2);

		break;
	// case token in Follow(STMT_LIST_1)
	case TOKEN_CLOSE_CURLY_BRACES:
		print_rule("STMT_LIST_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_SEMICOLON),
			get_token_name(TOKEN_CLOSE_CURLY_BRACES));
		Error_Recovery(STMT_LIST_1, expected_tokens);
		break;
	}
	return STMT_LIST_1_attr;
}


VAR_ATTR* Parse_TYPE()
{
	VAR_ATTR* TYPE_attr = create_var_attr();
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_INT:
		print_rule("TYPE -> int");
		TYPE_attr->type = INTEGER;
		return TYPE_attr;
	case TOKEN_KEYWORD_FLOAT:
		print_rule("TYPE -> float");
		TYPE_attr->type = FLOAT;
		return TYPE_attr;
	default:
		sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_KEYWORD_INT),
			get_token_name(TOKEN_KEYWORD_FLOAT));
		Error_Recovery(TYPE,expected_tokens);
		break;
	}
}

/* STMT -> VAR = EXPR | COMP_STMT | IF_STMT | CALL | RETURN_STMT
	First(VAR)			= id --> and '=' or '[' after
	First(CALL)			= id --> and '(' after
	First(COMP_STMT)	= {
	First(IF_STMT)		= if
	First(RETURN_STMT)	= return
*/
VAR_ATTR* Parse_STMT(ENTRY_TYPE expected_ret_type, int* has_return_stmt_flag)
{
	VAR_ATTR* STMT_attr = create_var_attr();
	VAR_ATTR* RETURN_STMT_attr = NULL;
	VAR_ATTR* COMP_STMT_attr = NULL;
	VAR_ATTR* IF_STMT_attr = NULL;
	VAR_ATTR* VAR_attr = NULL;
	VAR_ATTR* CALL_attr = NULL;
	VAR_ATTR* EXPR_attr = NULL;
	
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_KEYWORD_RETURN:
		print_rule("STMT -> RETURN_STMT");
		tok = back_token();

		RETURN_STMT_attr = Parse_RETURN_STMT(expected_ret_type);

		/* set the flag to 1 (contain at least one return statement with a returned value) */
		if (RETURN_STMT_attr->type != VOID) {
			*has_return_stmt_flag = 1;
		}
		delete_var_attr(RETURN_STMT_attr);

		break;
	case TOKEN_OPEN_CURLY_BRACES:
		print_rule("STMT -> COMP_STMT");
		tok = back_token();

		Parse_BB();

		COMP_STMT_attr = Parse_COMP_STMT(expected_ret_type, has_return_stmt_flag);
		delete_var_attr(COMP_STMT_attr);

		Parse_FB();

		break;
	case TOKEN_KEYWORD_IF:
		print_rule("STMT -> IF_STMT");
		tok = back_token();

		IF_STMT_attr = Parse_IF_STMT(expected_ret_type, has_return_stmt_flag);
		delete_var_attr(IF_STMT_attr);

		break;
	case TOKEN_IDENTIFIER:
		// case token is 'id', look at the next token
		tok = next_token();
		switch (tok->kind)
		{
		case TOKEN_OPEN_BRACKETS:
		case TOKEN_OP_ASSIGN:
			print_rule("STMT -> VAR = EXPR");
			tok = back_token();
			tok = back_token();
			
			VAR_attr = Parse_VAR();
			Match(TOKEN_OP_ASSIGN);
			EXPR_attr = Parse_EXPR();
			// Case left side is integer and right side is not an integer
			if (VAR_attr->type == INTEGER && EXPR_attr->type != INTEGER) {
				sprintf(semantic_error_msg, "Illegal assignment to %s (left value = int, right value = %s)", VAR_attr->id, get_entry_type_str(EXPR_attr->type));
				print_semantic_error(semantic_error_msg);
			}
			// Case left side is float
			else if (VAR_attr->type == FLOAT) {
				if (EXPR_attr->type != FLOAT && EXPR_attr->type != INTEGER) {
					sprintf(semantic_error_msg, "Illegal assignment to %s (left value = float, right value = %s)", VAR_attr->id, get_entry_type_str(EXPR_attr->type));
					print_semantic_error(semantic_error_msg);
				}
			}
			delete_var_attr(EXPR_attr);
			delete_var_attr(VAR_attr);

			return STMT_attr;
		case TOKEN_OPEN_PARENTHESES:
			print_rule("STMT -> CALL");
			tok = back_token();
			tok = back_token();
			
			CALL_attr = Parse_CALL();
			delete_var_attr(CALL_attr);
			
			return STMT_attr;
		default:
			sprintf(expected_tokens, " %s , %s , %s ", get_token_name(TOKEN_OPEN_BRACKETS),
				get_token_name(TOKEN_OP_ASSIGN),
				get_token_name(TOKEN_OPEN_PARENTHESES));
			Error_Recovery(STMT, expected_tokens);
			return STMT_attr;
		}
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s ", get_token_name(TOKEN_KEYWORD_RETURN),
			get_token_name(TOKEN_OPEN_CURLY_BRACES),
			get_token_name(TOKEN_KEYWORD_IF),
			get_token_name(TOKEN_IDENTIFIER));
		Error_Recovery(STMT,expected_tokens);
		break;
	}
	return STMT_attr;
}

VAR_ATTR* Parse_EXPR()
{
	VAR_ATTR* EXPR_attr = create_var_attr();
	VAR_ATTR* TERM_attr = NULL;
	VAR_ATTR* EXPR_1_attr = NULL;
	print_rule("EXPR -> TERM EXPR_1");
	
	TERM_attr = Parse_TERM();
	EXPR_1_attr = Parse_EXPR_1(TERM_attr->type, TERM_attr->value);
	EXPR_attr->type = EXPR_1_attr->type;
	EXPR_attr->value = EXPR_1_attr->value;
	delete_var_attr(TERM_attr);
	delete_var_attr(EXPR_1_attr);

	return EXPR_attr;
}

/*
	Nullable
	First(EXPR_1) = +
	Follow(EXPR_1) = ) , ; ] rel_op }
*/
VAR_ATTR* Parse_EXPR_1(ENTRY_TYPE cur_type, int cur_value)
{
	VAR_ATTR* EXPR_1_attr = create_var_attr();
	VAR_ATTR* EXPR_1_attr_2 = NULL;
	VAR_ATTR* TERM_attr = NULL;

	// set the given attributes
	EXPR_1_attr->type = cur_type;
	EXPR_1_attr->value = cur_value;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(EXPR_1)
	case TOKEN_AR_OP_ADD:		// +
		print_rule("EXPR_1 -> + TERM EXPR_1");
		
		TERM_attr = Parse_TERM();
		if (cur_type == ERROR_TYPE || TERM_attr->type == ERROR_TYPE) {
			EXPR_1_attr->type = ERROR_TYPE;
		}
		else if (cur_type == INTEGER && TERM_attr->type == INTEGER) {
			EXPR_1_attr->type = INTEGER;
			EXPR_1_attr->value = cur_value + TERM_attr->value;
		}
		else {
			EXPR_1_attr->type = FLOAT;
		}
		EXPR_1_attr_2 = Parse_EXPR_1(EXPR_1_attr->type, EXPR_1_attr->value);
		EXPR_1_attr->type = EXPR_1_attr_2->type;
		EXPR_1_attr->value = EXPR_1_attr_2->value;
		delete_var_attr(TERM_attr);
		delete_var_attr(EXPR_1_attr_2);

		break;
	// case token in Follow(EXPR_1)
	case TOKEN_CLOSE_PARENTHESES:			// )
	case TOKEN_COMMA:						// ,
	case TOKEN_SEMICOLON:					// ;
	case TOKEN_CLOSE_BRACKETS:				// ]
	case TOKEN_CLOSE_CURLY_BRACES:			// }
	case TOKEN_REL_OP_EQUAL:
	case TOKEN_REL_OP_NOT_EQUAL:
	case TOKEN_REL_OP_LESS:
	case TOKEN_REL_OP_LESS_EQUAL:
	case TOKEN_REL_OP_GRATER:
	case TOKEN_REL_OP_GRATER_EQUAL:
		print_rule("EXPR_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s ", get_token_name(TOKEN_AR_OP_ADD),
			get_token_name(TOKEN_CLOSE_PARENTHESES),
			get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_SEMICOLON),
			get_token_name(TOKEN_CLOSE_BRACKETS),
			get_token_name(TOKEN_CLOSE_CURLY_BRACES),
			get_token_name(TOKEN_REL_OP_EQUAL),
			get_token_name(TOKEN_REL_OP_NOT_EQUAL),
			get_token_name(TOKEN_REL_OP_LESS),
			get_token_name(TOKEN_REL_OP_LESS_EQUAL),
			get_token_name(TOKEN_REL_OP_GRATER),
			get_token_name(TOKEN_REL_OP_GRATER_EQUAL));
		Error_Recovery(EXPR_1,expected_tokens);
		break;
	}
	return EXPR_1_attr;
}

VAR_ATTR* Parse_EXPR_LIST(dim_node* node)
{
	VAR_ATTR* EXPR_LIST_attr = create_var_attr();
	VAR_ATTR* EXPR_LIST_1_attr = NULL;
	VAR_ATTR* EXPR_attr = NULL;
	print_rule("EXPR_LIST -> EXPR EXPR_LIST_1");
	EXPR_attr = Parse_EXPR();
	if (EXPR_attr->type != INTEGER) {
		print_semantic_error("The expression must be an integer");
		EXPR_LIST_attr->type = ERROR_TYPE;
	}
	else if (node != NULL) {
		if (EXPR_attr->value >= node->dim_size) {
			print_semantic_error("The expression exceed the array scope");
			EXPR_LIST_attr->type = ERROR_TYPE;
		}

		EXPR_LIST_1_attr = Parse_EXPR_LIST_1(node->next_node);
		if (EXPR_LIST_1_attr->type == ERROR_TYPE) {
			EXPR_LIST_attr->type = ERROR_TYPE;
		}
	}
	else {
		EXPR_LIST_1_attr = Parse_EXPR_LIST_1(NULL);
		if (EXPR_LIST_1_attr->type == ERROR_TYPE) {
			EXPR_LIST_attr->type = ERROR_TYPE;
		}
	}
	delete_var_attr(EXPR_LIST_1_attr);


	delete_var_attr(EXPR_attr);
	return EXPR_LIST_attr;
}

/*
	Nullable
	First(EXPR_LIST_1) = ,
	Follow(EXPR_LIST_1) = ]
*/
VAR_ATTR* Parse_EXPR_LIST_1(dim_node* node)
{
	VAR_ATTR* EXPR_LIST_1_attr = create_var_attr();
	VAR_ATTR* EXPR_LIST_1_attr_2 = NULL;
	VAR_ATTR* EXPR_attr = NULL;

	// set the default type value to void (to make sure it doesn't initialize with the error_type value)
	EXPR_LIST_1_attr->type = VOID;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(EXPR_LIST_1)
	case TOKEN_COMMA:
		print_rule("EXPR_LIST_1 -> , EXPR EXPR_LIST_1");

		if (node == NULL) {
			print_semantic_error("The expression exceed the array scope");
			EXPR_LIST_1_attr->type = ERROR_TYPE;
			return EXPR_LIST_1_attr;
		}

		EXPR_attr = Parse_EXPR();
		if (EXPR_attr->type != INTEGER) {
			print_semantic_error("The expression must be an integer");
			EXPR_LIST_1_attr->type = ERROR_TYPE;
		}
		else if (EXPR_attr->value >= node->dim_size) {
			print_semantic_error("The expression exceed the array scope");
			EXPR_LIST_1_attr->type = ERROR_TYPE;
		}
		delete_var_attr(EXPR_attr);
		
		EXPR_LIST_1_attr_2 = Parse_EXPR_LIST_1(node->next_node);
		if (EXPR_LIST_1_attr_2->type == ERROR_TYPE) {
			EXPR_LIST_1_attr->type = ERROR_TYPE;
		}
		delete_var_attr(EXPR_LIST_1_attr_2);

		break;
	// case token in Follow(EXPR_LIST_1)
	case TOKEN_CLOSE_BRACKETS:
		print_rule("EXPR_LIST_1 -> epsilon");
		
		if (node != NULL) {
			print_semantic_error("Illegal amount of dimensions");
			EXPR_LIST_1_attr->type = ERROR_TYPE;
		}

		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_CLOSE_BRACKETS));
		Error_Recovery(EXPR_LIST_1, expected_tokens);
		break;
	}
	return EXPR_LIST_1_attr;
}

VAR_ATTR* Parse_IF_STMT(ENTRY_TYPE expected_ret_type,int* has_return_stmt_flag)
{
	VAR_ATTR* IF_STMT_attr = create_var_attr();
	VAR_ATTR* STMT_attr = NULL;
	print_rule("IF_STMT -> if ( CONDITION ) STMT");
	Match(TOKEN_KEYWORD_IF);
	Match(TOKEN_OPEN_PARENTHESES);
	Parse_CONDITION();
	Match(TOKEN_CLOSE_PARENTHESES);

	STMT_attr = Parse_STMT(expected_ret_type,has_return_stmt_flag);
	delete_var_attr(STMT_attr);

	return IF_STMT_attr;
}

VAR_ATTR* Parse_CALL()
{
	VAR_ATTR* CALL_attr = create_var_attr();
	VAR_ATTR* ARGS_attr = NULL;
	entry_pair* entry = NULL;
	param_list* p_list = NULL;
	print_rule("CALL -> id ( ARGS )");
	Match(TOKEN_IDENTIFIER);

	CALL_attr->id = clone_str(tok->lexeme);

	/* search for the id in the symbol table */
	entry = cur_table->find(cur_table, CALL_attr->id);
	if (entry == NULL) {
		sprintf(semantic_error_msg, "The identifier [%s] doesn't exists in the symbol table", CALL_attr->id);
		print_semantic_error(semantic_error_msg);
		CALL_attr->type = ERROR_TYPE;
	} 
	else if (entry->get_role(entry) != FULL_DEFINITION && entry->get_role(entry) != PRE_DEFINITION) {
		sprintf(semantic_error_msg, "The identifier [%s] declared as a function", CALL_attr->id);
		print_semantic_error(semantic_error_msg);
		CALL_attr->type = ERROR_TYPE;
	}
	else {
		/* if the return value is an integer, set the value to 0 (for expression's validation) */
		CALL_attr->type = entry->get_type(entry);
		if (CALL_attr->type == INTEGER)
			CALL_attr->value = 0;
		p_list = (param_list*)entry->get_list(entry);
	}

	Match(TOKEN_OPEN_PARENTHESES);
	
	ARGS_attr = Parse_ARGS(p_list);
	delete_var_attr(ARGS_attr);

	Match(TOKEN_CLOSE_PARENTHESES);

	return CALL_attr;
}

/*
	Nullable
	First(ARGS) = id int_num float_num (
	Follow(ARGS) = )
*/
VAR_ATTR* Parse_ARGS(param_list* p_list)
{
	VAR_ATTR* ARGS_attr = create_var_attr();
	VAR_ATTR* ARG_LIST_attr = NULL;
	param_node* node = NULL;

	// set a default type to non error_type
	ARGS_attr->type = VOID;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(ARGS)
	case TOKEN_IDENTIFIER:
	case TOKEN_INTEGER:
	case TOKEN_FLOAT:
	case TOKEN_OPEN_PARENTHESES:
		print_rule("ARGS -> ARG_LIST");
		tok = back_token();

		if (p_list != NULL)
			node = p_list->head;
		ARG_LIST_attr = Parse_ARG_LIST(node);
		delete_var_attr(ARG_LIST_attr);

		break;
	// case token in Follow(ARGS)
	case TOKEN_CLOSE_PARENTHESES:
		print_rule("ARGS -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s , %s ", get_token_name(TOKEN_IDENTIFIER),
			get_token_name(TOKEN_INTEGER),
			get_token_name(TOKEN_FLOAT),
			get_token_name(TOKEN_OPEN_PARENTHESES),
			get_token_name(TOKEN_CLOSE_PARENTHESES));
		Error_Recovery(ARGS,expected_tokens);
		break;
	}
	return ARGS_attr;
}

VAR_ATTR* Parse_ARG_LIST(param_node* node)
{
	VAR_ATTR* ARG_LIST_attr = create_var_attr();
	VAR_ATTR* EXPR_attr = NULL;
	VAR_ATTR* ARG_LIST_1_attr = NULL;

	// set a default type attribute to non error type
	ARG_LIST_attr->type = INTEGER;

	print_rule("ARG_LIST -> EXPR ARG_LIST_1");

	EXPR_attr = Parse_EXPR();
	if (EXPR_attr->type != node->type) {
		print_semantic_error("The parameters doesn't match the function definition");
	}
	delete_var_attr(EXPR_attr);

	ARG_LIST_1_attr = Parse_ARG_LIST_1(node->next_node);
	delete_var_attr(ARG_LIST_1_attr);

	return ARG_LIST_attr;
}

/*
	Nullable
	First(ARG_LIST_1) = ,
	Follow(ARG_LIST_1) = )
*/
VAR_ATTR* Parse_ARG_LIST_1(param_node* node)
{
	VAR_ATTR* ARG_LIST_1_attr = create_var_attr();
	VAR_ATTR* ARG_LIST_1_attr_2 = NULL;
	VAR_ATTR* EXPR_attr = NULL;


	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(ARG_LIST_1)
	case TOKEN_COMMA:
		print_rule("ARG_LIST_1 -> , EXPR ARG_LIST_1");

		if (node == NULL) {
			print_semantic_error("Illegal amount of parameters to the function");
			break;
		}
		EXPR_attr = Parse_EXPR();
		if (EXPR_attr->type != node->type) {
			print_semantic_error("The parameters doesn't match the function definition");
		}
		delete_var_attr(EXPR_attr);

		ARG_LIST_1_attr_2 = Parse_ARG_LIST_1(node->next_node);
		delete_var_attr(ARG_LIST_1_attr_2);

		break;
	// case token in Follow(ARG_LIST_1)
	case TOKEN_CLOSE_PARENTHESES:
		print_rule("ARG_LIST_1 -> epsilon");

		if (node != NULL) {
			print_semantic_error("Illegal amount of parameters to the function");
		}

		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s ", get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_CLOSE_PARENTHESES));
		Error_Recovery(ARG_LIST_1,expected_tokens);
		break;
	}
	return ARG_LIST_1_attr;
}

void Parse_CONDITION()
{
	VAR_ATTR* EXPR_attr = NULL;
	print_rule("CONDITION -> EXPR rel_op EXPR");
	
	EXPR_attr = Parse_EXPR();
	delete_var_attr(EXPR_attr);

	tok = next_token();
	switch (tok->kind)
	{
	// rel_op tokens
	case TOKEN_REL_OP_EQUAL:
	case TOKEN_REL_OP_NOT_EQUAL:
	case TOKEN_REL_OP_LESS:
	case TOKEN_REL_OP_LESS_EQUAL:
	case TOKEN_REL_OP_GRATER:
	case TOKEN_REL_OP_GRATER_EQUAL:
		EXPR_attr = Parse_EXPR();
		delete_var_attr(EXPR_attr);
		break;
	default:
		// This function doesn't call to error_recovery:
		// The reason is that the switch-case is a replacement for the Match function, so it just print an error message
		sprintf(expected_tokens, "%s,%s,%s,%s,%s,%s", get_token_name(TOKEN_REL_OP_EQUAL),
			get_token_name(TOKEN_REL_OP_NOT_EQUAL),
			get_token_name(TOKEN_REL_OP_LESS),
			get_token_name(TOKEN_REL_OP_LESS_EQUAL),
			get_token_name(TOKEN_REL_OP_GRATER),
			get_token_name(TOKEN_REL_OP_GRATER_EQUAL));
		fprintf(syntactic_out, "Expected token of type '{%s}' at line: {%d}, \nActual token of type '{%s}', lexeme: '{%s}'.\n", expected_tokens, tok->lineNumber, get_token_name(tok->kind), tok->lexeme);
		break;
	}
}

VAR_ATTR* Parse_TERM()
{
	VAR_ATTR* TERM_attr = create_var_attr();
	VAR_ATTR* FACTOR_attr = NULL;
	VAR_ATTR* TERM_1_attr = NULL;
	print_rule("TERM -> FACTOR TERM_1");
	FACTOR_attr = Parse_FACTOR();
	TERM_1_attr = Parse_TERM_1(FACTOR_attr->type, FACTOR_attr->value);
	TERM_attr->type = TERM_1_attr->type;
	TERM_attr->value = TERM_1_attr->value;
	delete_var_attr(FACTOR_attr);
	delete_var_attr(TERM_1_attr);
	return TERM_attr;
}

/*
	Nullable
	First(TERM_1) = *
	Follow(TERM_1) = ) + , ; ] rel_op }
*/
VAR_ATTR* Parse_TERM_1(ENTRY_TYPE cur_type, int cur_value)
{
	VAR_ATTR* TERM_1_attr = create_var_attr();
	VAR_ATTR* TERM_1_attr_2 = NULL;
	VAR_ATTR* FACTOR_attr = NULL;

	// set the given attributes
	TERM_1_attr->type = cur_type;
	TERM_1_attr->value = cur_value;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(TERM_1)
	case TOKEN_AR_OP_MUL:
		print_rule("TERM_1 -> * FACTOR TERM_1");

		FACTOR_attr = Parse_FACTOR();
		if (FACTOR_attr->type == ERROR_TYPE || cur_type == ERROR_TYPE) {
			TERM_1_attr->type = ERROR_TYPE;
		}
		else if (cur_type == INTEGER && FACTOR_attr->type == INTEGER) {
			TERM_1_attr->type = INTEGER;
			TERM_1_attr->value = cur_value * FACTOR_attr->value;
		}
		else {
			TERM_1_attr->type = FLOAT;
		}
		delete_var_attr(FACTOR_attr);

		TERM_1_attr_2 = Parse_TERM_1(TERM_1_attr->type, TERM_1_attr->value);
		TERM_1_attr->type = TERM_1_attr_2->type;
		TERM_1_attr->value = TERM_1_attr_2->value;
		delete_var_attr(TERM_1_attr_2);

		break;
	// case token in Follow(TERM_1)
	case TOKEN_CLOSE_PARENTHESES:			// )
	case TOKEN_AR_OP_ADD:					// +
	case TOKEN_COMMA:						// ,
	case TOKEN_SEMICOLON:					// ;
	case TOKEN_CLOSE_BRACKETS:				// ]
	case TOKEN_CLOSE_CURLY_BRACES:			// }
	case TOKEN_REL_OP_EQUAL:
	case TOKEN_REL_OP_NOT_EQUAL:
	case TOKEN_REL_OP_LESS:
	case TOKEN_REL_OP_LESS_EQUAL:
	case TOKEN_REL_OP_GRATER:
	case TOKEN_REL_OP_GRATER_EQUAL:
		print_rule("TERM_1 -> epsilon");
		tok = back_token();
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s , %s ", get_token_name(TOKEN_AR_OP_MUL),
			get_token_name(TOKEN_CLOSE_PARENTHESES),
			get_token_name(TOKEN_AR_OP_ADD),
			get_token_name(TOKEN_COMMA),
			get_token_name(TOKEN_SEMICOLON),
			get_token_name(TOKEN_CLOSE_BRACKETS),
			get_token_name(TOKEN_CLOSE_CURLY_BRACES),
			get_token_name(TOKEN_REL_OP_EQUAL),
			get_token_name(TOKEN_REL_OP_NOT_EQUAL),
			get_token_name(TOKEN_REL_OP_LESS),
			get_token_name(TOKEN_REL_OP_LESS_EQUAL),
			get_token_name(TOKEN_REL_OP_GRATER),
			get_token_name(TOKEN_REL_OP_GRATER_EQUAL));
		Error_Recovery(TERM_1,expected_tokens);
		break;
	}
	return TERM_1_attr;
}

/*
	First(CALL) = First(VAR) = id 
	First(FACTOR) = int_num float_num (
*/
VAR_ATTR* Parse_FACTOR()
{
	VAR_ATTR* FACTOR_attr = create_var_attr();
	VAR_ATTR* EXPR_attr = NULL;
	VAR_ATTR* VAR_attr = NULL;
	VAR_ATTR* CALL_attr = NULL;
	tok = next_token();
	switch (tok->kind)
	{
	case TOKEN_INTEGER:
		print_rule("FACTOR -> int_num");
		FACTOR_attr->type = INTEGER;
		FACTOR_attr->value = atoi(tok->lexeme);
		break;
	case TOKEN_FLOAT:
		print_rule("FACTOR -> float_num");
		FACTOR_attr->type = FLOAT;
		break;
	case TOKEN_OPEN_PARENTHESES:
		print_rule("FACTOR -> ( EXPR )");
		
		EXPR_attr = Parse_EXPR();
		FACTOR_attr->type = EXPR_attr->type;
		FACTOR_attr->value = EXPR_attr->value;
		delete_var_attr(EXPR_attr);

		Match(TOKEN_CLOSE_PARENTHESES);
		break;
	// case token is 'id', there is 2 options (VAR or CALL)
	case TOKEN_IDENTIFIER:
		tok = next_token();
		switch (tok->kind)
		{
		// if the token '(' so it must be CALL, because Follow(VAR) doesn't contain '('
		case TOKEN_OPEN_PARENTHESES:
			print_rule("FACTOR -> CALL");
			tok = back_token();
			tok = back_token();
			
			CALL_attr = Parse_CALL();
			FACTOR_attr->type = CALL_attr->type;
			FACTOR_attr->value = CALL_attr->value;
			delete_var_attr(CALL_attr);

			break;
		default:
			print_rule("FACTPR -> VAR");
			tok = back_token();
			tok = back_token();
			
			VAR_attr = Parse_VAR();
			if (VAR_attr->type == INTEGER)
				FACTOR_attr->value = 0;
			FACTOR_attr->type = VAR_attr->type;
			delete_var_attr(VAR_attr);

			break;
		}
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s ", get_token_name(TOKEN_INTEGER),
			get_token_name(TOKEN_FLOAT),
			get_token_name(TOKEN_OPEN_PARENTHESES),
			get_token_name(TOKEN_IDENTIFIER));
		Error_Recovery(FACTOR,expected_tokens);
		break;
	}
	return FACTOR_attr;
}

VAR_ATTR* Parse_RETURN_STMT(ENTRY_TYPE expected_ret_type)
{
	VAR_ATTR* RETURN_STMT_attr = NULL;
	print_rule("RETURN_STMT -> return RETURN_STMT_1");
	Match(TOKEN_KEYWORD_RETURN);
	RETURN_STMT_attr = Parse_RETURN_STMT_1(expected_ret_type);
	if (RETURN_STMT_attr->type != expected_ret_type) {
		sprintf(semantic_error_msg, "The return type doesn't match the expected return type (expected: [%s] , actual: [%s])", get_entry_type_str(expected_ret_type), get_entry_type_str(RETURN_STMT_attr->type));
		print_semantic_error(semantic_error_msg);
	}
	return RETURN_STMT_attr;
}

/*
	Nullable
	First(RETURN_STMT_1) = id int_num float_num (
	Follow(RETURN_STMT_1) = ; }
*/
VAR_ATTR* Parse_RETURN_STMT_1()
{
	VAR_ATTR* RETURN_STMT_1_attr = create_var_attr();
	VAR_ATTR* EXPR_attr = NULL;

	tok = next_token();
	switch (tok->kind)
	{
	// case token in First(RETURN_STMT_1)
	case TOKEN_IDENTIFIER:
	case TOKEN_INTEGER:
	case TOKEN_FLOAT:
	case TOKEN_OPEN_PARENTHESES:
		print_rule("RETURN_STMT_1 -> EXPR");
		tok = back_token();
		
		// copy the return type attribute
		EXPR_attr = Parse_EXPR();
		RETURN_STMT_1_attr->type = EXPR_attr->type;
		delete_var_attr(EXPR_attr);

		break;
	// case token in Follow(RETURN_STMT_1)
	case TOKEN_SEMICOLON:
	case TOKEN_CLOSE_CURLY_BRACES:
		print_rule("RETURN_STMT_1 -> epsilon");
		tok = back_token();
		RETURN_STMT_1_attr->type = VOID;
		break;
	default:
		sprintf(expected_tokens, " %s , %s , %s , %s , %s , %s ", get_token_name(TOKEN_IDENTIFIER),
			get_token_name(TOKEN_INTEGER),
			get_token_name(TOKEN_FLOAT),
			get_token_name(TOKEN_OPEN_PARENTHESES),
			get_token_name(TOKEN_SEMICOLON),
			get_token_name(TOKEN_CLOSE_CURLY_BRACES));
		Error_Recovery(RETURN_STMT_1, expected_tokens);
		break;
	}
	return RETURN_STMT_1_attr;
}

void Parse_BB()
{
	cur_table = make_table(cur_table);
	fprintf(yyout_semantic, "Line[%d] - Create new block -\n", line_idx);
}


void Parse_FB()
{
	is_no_used_variables(cur_table->table,yyout_semantic,line_idx);
	fprintf(yyout_semantic, "Line[%d] - Delete current block -\n", line_idx);
	cur_table = pop_table(cur_table);
}


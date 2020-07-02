#pragma once
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 20000

extern FILE* yyout_semantic;

void print_error_msg(const char* msg);

char* clone_str(const char* str);

/* 
---------------------------------------
  Dimension list objects & functions
---------------------------------------
*/
typedef struct dim_node {
    int dim_size;
    struct dim_node* next_node;
} dim_node;


typedef struct dim_list {
    unsigned int dim_num;
    dim_node* head;
    void (*insert)(struct dim_list*, const char*);
} dim_list;

dim_node* create_dim_node(int dim_size);
dim_list* create_dim_list();
dim_list* clone_dim_list(dim_list* list);
void delete_dim_list(dim_list* list);
void add_to_dim_list(dim_list* list, dim_node* node);
void insert_to_dim_list(dim_list* list, const char* int_num);
int is_exceed(dim_list* domain_list, dim_list* coordinates);
int is_equal_dim_lists(dim_list* original_list, dim_list* test_list);


/*
---------------------------------------
  Parameter list objects & functions
---------------------------------------
*/

typedef enum ENTRY_ROLE {
    VARIABLE,
    USED_VARIABLE,
    PRE_DEFINITION,
    FULL_DEFINITION,
    UNKNOWN
} ENTRY_ROLE;

const char* get_entry_role_str(ENTRY_ROLE role);

typedef enum ENTRY_TYPE {
    INTEGER,
    FLOAT,
    VOID,
    ERROR_TYPE
} ENTRY_TYPE;

const char* get_entry_type_str(ENTRY_TYPE type);

typedef struct param_node {
    char* name;
    ENTRY_TYPE type;
    dim_list* d_list;
    struct param_node* next_node;
} param_node;

typedef struct param_list {
    unsigned int param_num;
    param_node* head;
    void (*insert)(struct param_list*, char*, ENTRY_TYPE, dim_list*);
} param_list;

param_node* create_param_node(const char* _name, ENTRY_TYPE _type, dim_list* _d_list);
param_list* create_param_list();
param_list* clone_param_list(param_list* list);
void insert_to_param_list(param_list* p_list, const char* _name, ENTRY_TYPE _type, dim_list* _d_list);
void delete_param_node(param_node* node);
void delete_param_list(param_list* list);
void add_to_param_list(param_list* list, param_node* node);
int check_param_lists_validation(param_list* original_list, param_list* test_list);
int check_param_list_validation(param_list* original_list, param_list* test_list, const char* function_id, FILE* output, int line_idx);


/*
---------------------------------------
  Symbol table objects & functions
---------------------------------------
*/

typedef struct entry_value {
    ENTRY_ROLE role;
    ENTRY_TYPE type;
    void* list;
} entry_value ;

entry_value* create_entry_value();
void delete_entry_value(entry_value* entry);

/*
---------------------------------------
    entry_pair objects & functions
---------------------------------------
*/
typedef struct entry_pair {
    char* key;
    entry_value* value;
    struct entry_pair* next;
    // getters
    const char* (*get_name)(struct entry_pair* self);
    ENTRY_ROLE (*get_role)(struct entry_pair* self);
    ENTRY_TYPE (*get_type)(struct entry_pair* self);
    void* (*get_list)(struct entry_pair* self);
    // setters
    void (*set_role)(struct entry_pair* self, ENTRY_ROLE _role);
    void (*set_type)(struct entry_pair* self, ENTRY_TYPE _type);
    void (*set_list)(struct entry_pair* self, void* _list);
} entry_pair;

entry_pair* create_new_entry_pair(const char* key);
void delete_entry_pair(entry_pair* entry);
// getters
const char* get_entry_name(entry_pair* self);
ENTRY_ROLE get_entry_role(entry_pair* self);
ENTRY_TYPE get_entry_type(entry_pair* self);
void* get_entry_list(entry_pair* self);
// setters
void set_entry_role(struct entry_pair* self, ENTRY_ROLE _role);
void set_entry_type(struct entry_pair* self, ENTRY_TYPE _type);
void set_entry_list(struct entry_pair* self, void* _list);

/*
---------------------------------------
    hash_table objects & functions
---------------------------------------
*/

typedef struct {
    entry_pair** entries;
} hash_table;

unsigned int hash(const char* key);
hash_table* create_hash_table();
entry_pair* insert_to_hash_table(hash_table* table, const char* key);
entry_pair* find_in_hash_table(hash_table* table, const char* key);
void delete_hash_table(hash_table* table);

/*
---------------------------------------
    symbol_table objects & functions
---------------------------------------
*/

typedef struct {
    hash_table* table;
    struct symbol_table* father;
    entry_pair* (*insert)(struct symbol_table* cur_table, const char* id_name);
    entry_pair* (*lookup)(struct symbol_table* cur_table, const char* id_name);
    entry_pair* (*find)(struct symbol_table* cur_table, const char* id_name);
} symbol_table;

void delete_symbol_table(symbol_table* s_table);
symbol_table* make_table(symbol_table* cur_table);
symbol_table* pop_table(symbol_table* cur_table);
entry_pair* insert_to_symbol_table(symbol_table* cur_table,const char *id_name);
entry_pair* lookup_in_symbol_table(symbol_table* cur_table,const char *id_name);
entry_pair* find_in_symbol_table(symbol_table* cur_table,const char *id_name);
entry_pair* is_no_pre_definitions(symbol_table* cur_table);
void is_no_used_variables(hash_table* cur_table, FILE* output, int line_idx);


// LY - Test functions
void print_parameter(const char* type, const char* name, dim_list* d_list, FILE* test_output);
void print_dim_list(dim_list* d_list, FILE* output);
void print_single_entry(entry_pair* entry, FILE* test_output);
void print_variable_signature(const char* name, const char* type, const char* role, dim_list* d_list, FILE* test_output);
void print_function_signature(const char* name, const char* type, const char* role, param_list* p_list, FILE* test_output);
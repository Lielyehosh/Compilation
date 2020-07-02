#include "HashTable.h"

#define TABLE_SIZE 1000


void print_error_msg(const char* function_name, const char* msg)
{
    fprintf(yyout_semantic, "Error: In %s - %s", function_name, msg);
}



void print_memory_error_msg(const char* obj_name) {
    fprintf(yyout_semantic, "Error: Failed to allocate memory to a %s object", obj_name);
}

char* clone_str(const char* str)
{
    char* new_str = NULL;
    if (str == NULL)
        return NULL;
    new_str = (char*)calloc(strlen(str) + 1, sizeof(char));
    if (new_str == NULL) {
        print_memory_error_msg("string");
        exit(1);
    }
    strcpy(new_str, str);
    return new_str;
}

dim_node* create_dim_node(int dim_size)
{
    dim_node* new_dim_node = (dim_node*)malloc(sizeof(dim_node));
    if (new_dim_node == NULL) {
        print_memory_error_msg("dim_list");
        exit(1);
    }
    new_dim_node->dim_size = dim_size;
    new_dim_node->next_node = NULL;
    return new_dim_node;
}

dim_list* create_dim_list()
{
    dim_list* new_dim_list = (dim_list*)malloc(sizeof(dim_list));
    if (new_dim_list == NULL) {
        print_memory_error_msg("dim_list");
        exit(1);
    }
    new_dim_list->dim_num = 0;
    new_dim_list->head = NULL;
    new_dim_list->insert = insert_to_dim_list;
    return new_dim_list;
}

// TODO - check the function works 
dim_list* clone_dim_list(dim_list* list)
{
    dim_list* new_list;;
    dim_node* target_node_ptr = NULL;
    dim_node* src_node_ptr;
    if (list == NULL)
        return NULL;
    new_list = create_dim_list();
    src_node_ptr = list->head;
    new_list->dim_num = list->dim_num;
    if (src_node_ptr != NULL) {
        target_node_ptr = create_dim_node(src_node_ptr->dim_size);
        new_list->head = target_node_ptr;
        src_node_ptr = src_node_ptr->next_node;
    }
    while (src_node_ptr != NULL) {
        target_node_ptr->next_node = create_dim_node(src_node_ptr->dim_size);
        target_node_ptr = target_node_ptr->next_node;
        src_node_ptr = src_node_ptr->next_node;
    }
    return new_list;
}

void add_to_dim_list(dim_list* list, dim_node* node)
{
    dim_node* temp;
    if (list == NULL || node == NULL) {
        print_error_msg("add_to_dim_list", "Illegal input parameters");
        exit(1);
    }
    temp = list->head;
    if (temp == NULL) {
        list->head = node;
    }
    else {
        while (temp->next_node != NULL) {
            temp = temp->next_node;
        }
        temp->next_node = node;
    }
    list->dim_num++;
}

void delete_dim_list(dim_list* list)
{
    if (list == NULL)
        return;
    dim_node* node = list->head;
    while (node != NULL) {
        list->head = node->next_node;
        free(node);
        node = list->head;
    }
    free(list);
}

/*
This function gets 2 lists of dimensions and check if the coordinates_list doesn't exceed the domain_list size
*/
int is_exceed(dim_list* domain_list, dim_list* coordinates_list)
{
    dim_node* domain_node = NULL;
    dim_node* coordinates_node = NULL;
    // TODO - check if its correct if stmt
    if (domain_list == NULL && coordinates_list == NULL) {
        return 1;
    }
    if (domain_list == NULL) {
        print_error_msg("is_exceed", "Illegal input parameters (domain_list can not be null)");
        exit(1);
    }
    if (coordinates_list == NULL) {
        print_error_msg("is_exceed", "Illegal input parameters  (coordinates_list can not be null)");
        exit(1);
    }
    if (domain_list->dim_num != coordinates_list->dim_num) {
        // Case the number of the dimensions is different
        return 0;
    }
    domain_node = domain_list->head;
    coordinates_node = coordinates_list->head;
    while (coordinates_node != NULL && domain_node != NULL) {
        if (coordinates_node->dim_size >= domain_node->dim_size)
            // the current coordinates dimension exceeded the dimension domain
            return 0;
        coordinates_node = coordinates_node->next_node;
        domain_node = domain_node->next_node;
    } 
    if (coordinates_node == NULL && domain_node == NULL) {
        // both node has the same number of dimensions
        return 1;
    }
    return 0;
}

int is_equal_dim_lists(dim_list* original_list, dim_list* test_list)
{
    dim_node* temp1 = NULL;
    dim_node* temp2 = NULL;
    if (original_list == NULL && test_list == NULL) {
        // Both of the lists are empty
        return 1;
    }
    if (original_list == NULL || test_list == NULL) {
        return 0;
    }
    temp1 = original_list->head;
    temp2 = test_list->head;
    while (temp1 != NULL && temp2 != NULL) {
        if (temp1->dim_size != temp2->dim_size)
            return 0;
        temp1 = temp1->next_node;
        temp2 = temp2->next_node;
    }
    if (temp1 == NULL && temp2 == NULL)
        return 1;
    return 0;
}

const char* get_entry_role_str(ENTRY_ROLE role)
{
    switch (role)
    {
    case VARIABLE:
    case USED_VARIABLE:
        return "VARIABLE";
    case PRE_DEFINITION:
        return "PRE_DEFINITION";
    case FULL_DEFINITION:
        return "FULL_DEFINITION";
    case UNKNOWN:
        return "UNKNOWN";
    default:
        return "DEFAULT";
    }
}

const char* get_entry_type_str(ENTRY_TYPE type)
{
    switch (type)
    {
    case INTEGER:
        return "int";
    case FLOAT:
        return "float";
    case VOID:
        return "void";
    case ERROR_TYPE:
        return "ERROR_TYPE";
    default:
        return "DEFAULT";
    }
}

void insert_to_dim_list(dim_list* list, const char* int_num)
{
    dim_node* node = create_dim_node(atoi(int_num));
    add_to_dim_list(list, node);
}

param_node* create_param_node(const char* _name, ENTRY_TYPE _type, dim_list* _d_list)
{
    param_node* new_param_node = (param_node*)malloc(sizeof(param_node));
    if (new_param_node == NULL) {
        print_memory_error_msg("param_node");
        exit(1);
    }
    if (_name == NULL) {
        print_error_msg("create_param_node", "Illegal input parameters (name is NULL)");
        exit(1);
    }
    if (_type != INTEGER && _type != FLOAT) {
        print_error_msg("create_param_node", "Illegal input parameters (type is not integer or float as expected)");
        exit(1);
    }
    new_param_node->name = clone_str(_name);
    new_param_node->type = _type;
    new_param_node->d_list = _d_list;
    new_param_node->next_node = NULL;
    return new_param_node;
}

param_list* create_param_list()
{
    param_list* new_param_list = (param_list*)malloc(sizeof(param_list));
    if (new_param_list == NULL) {
        print_memory_error_msg("param_list");
        exit(1);
    }
    new_param_list->param_num = 0;
    new_param_list->head = NULL;
    new_param_list->insert = insert_to_param_list;
    return new_param_list;
}

// TODO - check that the function works good
param_list* clone_param_list(param_list* list)
{
    param_list* new_list = NULL;
    if (list != NULL) {
        param_node* node = list->head;
        new_list = create_param_list();
        while (node != NULL) {
            // may be a low performance (TODO)
            new_list->insert(new_list, node->name, node->type, node->d_list);
            node = node->next_node;
        }
    }
    return new_list;
}

void insert_to_param_list(param_list* p_list, const char* _name, ENTRY_TYPE _type, dim_list* _d_list)
{
    _d_list = clone_dim_list(_d_list);
    param_node* node = create_param_node(_name, _type, _d_list);
    add_to_param_list(p_list, node);
}

void add_to_param_list(param_list* list, param_node* node)
{
    param_node* temp = NULL;
    if (list == NULL) {
        print_error_msg("add_to_param_list", "Illegal input parameters (list is NULL)");
        exit(1);
    }
    if (node == NULL) {
        print_error_msg("add_to_param_list", "Illegal input parameters (node is NULL)");
        exit(1);
    }
    temp = list->head;
    // Case the list is empty
    if (temp == NULL) {
        list->head = node;
    }
    // Case the list isn't empty
    else {
        while (temp->next_node != NULL) {
            temp = temp->next_node;
        }
        temp->next_node = node;
    }
    list->param_num++;
}



/*
This function checks the type and the dimensions (if exists)
This function DOESN'T check if the names of the parameters are identical
Return values:
    0 - the test_list is valid
    1 - one of the lists is null
    2 - Number of parameters does not match
    3 - illegal parameter type (one of the parameters)
    4 - illegal parameter dimention
*/
int check_param_lists_validation(param_list* original_list, param_list* test_list)
{
    param_node* original_node = NULL;
    param_node* test_node = NULL;
    if (original_list == NULL && test_list == NULL) {
        return 0;
    }
    if (original_list == NULL) {
        return 1;
    }
    if (test_list == NULL) {
        return 1;
    }
    if (original_list->param_num != test_list->param_num) {
        // case the number of parameters is different
        return 2;
    }
    original_node = original_list->head;
    test_node = test_list->head;
    while (original_node != NULL && test_node != NULL) {
        if (original_node->type != test_node->type) {
            return 3;
        }
        if (is_equal_dim_lists(original_node->d_list, test_node->d_list) == 0) {
            return 4;
        }
        original_node = original_node->next_node;
        test_node = test_node->next_node;
    }
    if (original_node == NULL && test_node == NULL) {
        // case the number of nodes is equal
        return 0;
    }
    return 2;
}

/*
    This function gets 2 lists (type - param_list) and compare between them according to the following attributes:
    1. The size of the lists (should be equal)
    2. The type of the parameter_i in the first list should be equal to the type of the parameter_i in the second list
    3. If parameter_i is an array (has a dim_list attribute), then it should compare between the dim_list attributes
    In addition, the function print to the 'output' file the error messages if it's neccesary
*/
int check_param_list_validation(param_list* original_list, param_list* test_list, const char* function_id, FILE* output,int line_idx)
{
    param_node* original_node = NULL;
    param_node* test_node = NULL;
    if (original_list == NULL && test_list == NULL) {
        return 0;
    }
    if (original_list == NULL) {
        fprintf(output, "Line[%d]\t- \tError: Function [%s]: The function shouldn't have input parameters\n", line_idx, function_id);
        return 1;
    }
    if (test_list == NULL) {
        fprintf(output, "Line[%d]\t- \tError: Function [%s]: The function should have input parameters\n", line_idx, function_id);
        return 1;
    }
    if (original_list->param_num != test_list->param_num) {
        // case the number of parameters is different
        fprintf(output, "Line[%d]\t- \tError: Function [%s]: The number of the input parameters doesn't match the definition (expected: [%d] , actual: [%d])\n", line_idx, function_id, original_list->param_num, test_list->param_num);
        return 2;
    }
    original_node = original_list->head;
    test_node = test_list->head;
    while (original_node != NULL && test_node != NULL) {
        if (original_node->type != test_node->type) {
            fprintf(output, "Line[%d]\t- \tError: Function[%s]: Parameter [%s] should be of type [%s] (actual type : [%s])\n", line_idx, function_id, test_node->name, get_entry_type_str(original_node->type), get_entry_type_str(test_node->type));
            return 3;
        }
        if (is_equal_dim_lists(original_node->d_list, test_node->d_list) == 0) {
            int original_dim = 0, test_dim = 0;
            if (original_node->d_list != NULL) {
                original_dim = original_node->d_list->dim_num;
            }
            if(test_node->d_list != NULL) {
                test_dim = test_node->d_list->dim_num;
            }
            fprintf(output, "Line[%d]\t- \tError: Function[%s]: Parameter [%s] should have [%d] (dimensions) (actual number of dimensions: [%d])\n", line_idx, function_id, test_node->name, original_dim, test_dim);
            return 4;
        }
        original_node = original_node->next_node;
        test_node = test_node->next_node;
    }
    if (original_node == NULL && test_node == NULL) {
        // case the number of nodes is equal
        return 0;
    }
    fprintf(output, "Line[%d]\t- \tError: Function [%s]: The number of the input parameters doesn't match the definition (expected: [%d] , actual: [%d])\n", line_idx, function_id, original_list->param_num, test_list->param_num);
    return 2;
}

void delete_param_node(param_node* node)
{
    if (node == NULL)
        return;
    if (node->name != NULL) 
        free(node->name);
    delete_dim_list(node->d_list);
    free(node);
}

void delete_param_list(param_list* list)
{
    if (list == NULL) {
        return;
    }
    param_node* temp = list->head;
    while (temp != NULL) {
        list->head = temp->next_node;
        delete_param_node(temp);
        temp = list->head;
    }
    free(list);
}


/*
----------------------------------------------
            Hash table functions
----------------------------------------------
*/


entry_value* create_entry_value()
{
    entry_value* new_entry = malloc(sizeof(entry_value) * 1);
    if (new_entry == NULL) {
        print_memory_error_msg("entry_value");
        exit(1);
    }
    new_entry->list = NULL;
    new_entry->role = UNKNOWN;
    new_entry->type = ERROR_TYPE;
    return new_entry;
}

void delete_entry_value(entry_value* entry) {
    if (entry->list != NULL) {
        switch (entry->role)
        {
        case PRE_DEFINITION:
        case FULL_DEFINITION:
            delete_param_list((param_list*)entry->list);
            break;
        case VARIABLE:
        case USED_VARIABLE:
            delete_dim_list((dim_list*)entry->list);
            break;
        default:
            break;
        }
    }
    free(entry);
}

unsigned int hash(const char* key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    // do several rounds of multiplication
    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    // make sure value is 0 <= value < TABLE_SIZE
    value = value % TABLE_SIZE;

    return value;
}

entry_pair* create_new_entry_pair(const char* key) {
    // allocate the entry
    entry_pair* entry = malloc(sizeof(entry_pair) * 1);
    if (entry == NULL) {
        print_memory_error_msg("entry_pair");
        exit(1);
    }
    entry->key = clone_str(key);
    entry->value = create_entry_value();
    entry->next = NULL;
    
    // assign the functions
    entry->get_name = get_entry_name;
    entry->get_type = get_entry_type;
    entry->get_role = get_entry_role;
    entry->get_list = get_entry_list;
    entry->set_type = set_entry_type;
    entry->set_role = set_entry_role;
    entry->set_list = set_entry_list;
    return entry;
}

void delete_entry_pair(entry_pair* entry)
{
    if (entry == NULL)
        return;
    if (entry->key != NULL)
        free(entry->key);
    if (entry->value != NULL)
        delete_entry_value(entry->value);
    free(entry);
}

const char* get_entry_name(entry_pair* self)
{
    return self->key;
}

ENTRY_ROLE get_entry_role(entry_pair* self)
{
    return self->value->role;
}

ENTRY_TYPE get_entry_type(entry_pair* self)
{
    return self->value->type;
}

void* get_entry_list(entry_pair* self)
{
    return self->value->list;
}

void set_entry_role(entry_pair* self, ENTRY_ROLE _role)
{
    self->value->role = _role;
}

void set_entry_type(entry_pair* self, ENTRY_TYPE _type)
{
    self->value->type = _type;
}

void set_entry_list(entry_pair* self, void* _list)
{
    self->value->list = _list;
}

hash_table* create_hash_table() {
    hash_table* table = (hash_table*)malloc(sizeof(hash_table) * 1);
    if (table == NULL) {
        print_memory_error_msg("hash_table");
        exit(1);
    }
    table->entries = malloc(sizeof(entry_pair*) * TABLE_SIZE);
    if (table->entries == NULL) {
        print_memory_error_msg("entry_pair[]");
        exit(1);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->entries[i] = NULL;
    }
    return table;
}

/*
Return values:
    1) NULL - if the key is already exists in the table
    2) entry_pair* of the new entry pair with the given key
*/
entry_pair* insert_to_hash_table(hash_table* table, const char* key) {
    unsigned int slot = hash(key);
    entry_pair* entry = table->entries[slot];
    entry_pair* temp = NULL;

    // case the slot empty, insert immediately
    if (entry == NULL) {
        table->entries[slot] = create_new_entry_pair(key);
        return table->entries[slot];
    }

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return NULL;
        }
        temp = entry;
        entry = temp->next;
    }
    temp->next = create_new_entry_pair(key);
    return temp->next;
}

entry_pair* find_in_hash_table(hash_table* table, const char* key) {
    unsigned int slot = hash(key);

    // look up an entry set
    entry_pair* entry = table->entries[slot];

    // case the key doesn't exists in the table
    if (entry == NULL) {
        return NULL;
    }

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            // match found, return the entry_pair
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

void delete_hash_table(hash_table* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        entry_pair* entry = table->entries[i];
        while (entry != NULL) {
            entry_pair* temp = entry->next;
            delete_entry_pair(entry);
            entry = temp;
        }
    }
    free(table->entries);
    free(table);
}

void delete_symbol_table(symbol_table* s_table)
{
    if (s_table != NULL) {
        delete_hash_table(s_table->table);
        free(s_table);
    }
}

symbol_table* make_table(symbol_table* cur_table)
{
    symbol_table* new_table = (symbol_table*)malloc(sizeof(symbol_table));
    if (new_table == NULL) {
        print_memory_error_msg("symbol_table");
        exit(1);
    }
    new_table->father = cur_table;
    new_table->insert = insert_to_symbol_table;
    new_table->lookup = lookup_in_symbol_table;
    new_table->find = find_in_symbol_table;
    new_table->table = create_hash_table();
    return new_table;
}

symbol_table* pop_table(symbol_table* cur_table)
{
    symbol_table* table = cur_table;
    if (cur_table == NULL)
        return NULL;
    cur_table = cur_table->father;
    delete_symbol_table(table);
    return cur_table;
}

entry_pair* insert_to_symbol_table(symbol_table* cur_table, const char* id_name)
{
    entry_pair* new_entry_pair = insert_to_hash_table(cur_table->table, id_name);
    if (new_entry_pair == NULL) {
        // failed to insert a new pair to the hash table
        return NULL;
    }
    return new_entry_pair;
}

/*
    Perform a local search of id_name in the cur_table
    Returns pointer to the found entry, or NULL if the name is not found in that table
*/
entry_pair* lookup_in_symbol_table(symbol_table* cur_table, const char* id_name)
{
    entry_pair* entry = find_in_hash_table(cur_table->table, id_name);
    if (entry == NULL) {
        return NULL;
    }
    return entry;
}

/*
    Called when id use is found
    Allows to check whether the id is declared
    Performs a hierarchical search of id_name, starting from the current_table
    Returns the found entry pointer, or NULL (if id_name is undeclared)
*/
entry_pair* find_in_symbol_table(symbol_table* cur_table, const char* id_name)
{
    symbol_table* temp = cur_table;
    entry_pair* id_entry = NULL;
    while (temp != NULL) {
        id_entry = temp->lookup(temp, id_name);
        if (id_entry != NULL)
            return id_entry;
        temp = temp->father;
    }
    return NULL;
}

entry_pair* is_no_pre_definitions(symbol_table* cur_table)
{
    entry_pair* entry = NULL;
    while (cur_table != NULL) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            entry = cur_table->table->entries[i];
            if (entry == NULL)
                continue;

            while (entry != NULL) {
                if (entry->get_role(entry) == PRE_DEFINITION) {
                    return entry;
                }
                entry = entry->next;
            }
        }
        cur_table = cur_table->father;
    }
    return NULL;
}

void is_no_used_variables(hash_table* cur_table, FILE* output,int line_idx)
{
    entry_pair* entry = NULL;
    for (int i = 0; i < TABLE_SIZE; i++) {
        entry = cur_table->entries[i];
        if (entry == NULL)
            continue;

        while (entry != NULL) {
            if (entry->get_role(entry) == VARIABLE) {
                fprintf(output, "Line[%d]\t- \tWarning: Variable [%s] never used\n", line_idx, entry->get_name(entry));
            }
            entry = entry->next;
        }
    }
}

/* LY - TESTS FUNCTIONS */

void print_single_entry(entry_pair* entry, FILE* test_output)
{
    const char* name = entry->get_name(entry);
    ENTRY_ROLE role = entry->get_role(entry);
    ENTRY_TYPE type = entry->get_type(entry);
    void* list = entry->get_list(entry);
    dim_list* d_list = NULL;
    param_list* p_list = NULL;
    if (role == VARIABLE || role == USED_VARIABLE) {
        fprintf(yyout_semantic, "New Variable: ");
        if (list != NULL)
            d_list = (dim_list*)list;
        print_variable_signature(name, get_entry_type_str(type), get_entry_role_str(role), d_list, test_output);
    }
    else if (role == PRE_DEFINITION || role == FULL_DEFINITION) {
        fprintf(yyout_semantic, "New Function: ");
        if (list != NULL)
            p_list = (param_list*)list;
        print_function_signature(name, get_entry_type_str(type), get_entry_role_str(role), p_list, test_output);
    }
}

void print_variable_signature(const char* name, const char* type, const char* role, dim_list* d_list, FILE* test_output) 
{
    fprintf(test_output, "Signature - %s %s", type, name);
    print_dim_list(d_list, test_output);
    fprintf(test_output, "\n");
}


void print_function_signature(const char* name, const char* type, const char* role, param_list* p_list, FILE* test_output)
{
    fprintf(test_output, "Signature - %s %s (", type, name);
    // Print the parameter list
    if (p_list != NULL) {
        param_node* p_node = p_list->head;
        while (p_node != NULL) {
            print_parameter(get_entry_type_str(p_node->type), p_node->name, p_node->d_list, test_output);
            p_node = p_node->next_node;
            if (p_node != NULL)
                fprintf(test_output, ", ");
        }
    }
    fprintf(test_output, ")\n");
}

void print_parameter(const char* type, const char* name, dim_list* d_list, FILE* test_output) 
{
    fprintf(test_output, "%s %s",type, name);
    print_dim_list(d_list, test_output);
}

void print_dim_list(dim_list* d_list, FILE* output)
{
    if (d_list != NULL) {
        dim_node* d_node = d_list->head;
        fprintf(output, "[");
        while (d_node != NULL) {
            fprintf(output, "%i", d_node->dim_size);
            d_node = d_node->next_node;
            if (d_node != NULL)
                fprintf(output, ",");
        }
        fprintf(output, "]");
    }
}
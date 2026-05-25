#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
    HT_INT,
    HT_FLOAT,
    HT_DOUBLE,
    HT_CHAR,
    HT_STRING,
    HT_EMPTY,
    HT_DELETED
} ht_data_type;

typedef struct {
    ht_data_type type;
    union {
        int i_val;
        float f_val;
        double d_val;
        char c_val;
        char* s_val;
    } data;
} ht_value;

typedef struct {
    ht_value* values;
    int size;
    int count;
    ht_data_type key_type;
} ht_hash_table;

ht_value create_int_value(int val);
ht_value create_float_value(float val);
ht_value create_double_value(double val);
ht_value create_char_value(char val);
ht_value create_string_value(const char* val);
ht_value create_empty_value(void);
ht_value create_deleted_value(void);

ht_hash_table* ht_new(ht_data_type key_type, int size);
void ht_insert(ht_hash_table* ht, ht_value key, ht_value value);
ht_value ht_search(ht_hash_table* ht, ht_value key);
void ht_delete(ht_hash_table* ht, ht_value key);
void ht_free(ht_hash_table* ht);
void ht_print(ht_hash_table* ht);
void print_value(ht_value v);

#endif
#include "../include/hash_table.h"

void free_value(ht_value v);
void ht_resize(ht_hash_table* ht, int new_size);

ht_value create_int_value(int val) {
    ht_value v;
    v.type = HT_INT;
    v.data.i_val = val;
    return v;
}

ht_value create_float_value(float val) {
    ht_value v;
    v.type = HT_FLOAT;
    v.data.f_val = val;
    return v;
}

ht_value create_double_value(double val) {
    ht_value v;
    v.type = HT_DOUBLE;
    v.data.d_val = val;
    return v;
}

ht_value create_char_value(char val) {
    ht_value v;
    v.type = HT_CHAR;
    v.data.c_val = val;
    return v;
}

ht_value create_string_value(const char* val) {
    ht_value v;
    v.type = HT_STRING;
    v.data.s_val = malloc(strlen(val) + 1);
    strcpy(v.data.s_val, val);
    return v;
}

ht_value create_empty_value(void) {
    ht_value v;
    v.type = HT_EMPTY;
    return v;
}

ht_value create_deleted_value(void) {
    ht_value v;
    v.type = HT_DELETED;
    return v;
}

unsigned long hash_string(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

unsigned long hash_primitive(ht_value value) {
    unsigned long hash = 0;
    switch (value.type) {
        case HT_INT:
            hash = value.data.i_val * 2654435761UL;
            break;
        case HT_FLOAT:
            hash = (unsigned long)(fabs(value.data.f_val) * 2654435761.0);
            break;
        case HT_DOUBLE:
            hash = (unsigned long)(fabs(value.data.d_val) * 2654435761.0);
            break;
        case HT_CHAR:
            hash = value.data.c_val * 2654435761UL;
            break;
        case HT_STRING:
            hash = hash_string(value.data.s_val);
            break;
        default:
            hash = 0;
    }
    return hash;
}

unsigned long second_hash(ht_value value, int size) {
    unsigned long h = hash_primitive(value);
    return (h % (size - 1)) + 1;
}

int values_equal(ht_value a, ht_value b) {
    if (a.type != b.type) return 0;
    
    switch (a.type) {
        case HT_INT:    return a.data.i_val == b.data.i_val;
        case HT_FLOAT:  return fabs(a.data.f_val - b.data.f_val) < 1e-6;
        case HT_DOUBLE: return fabs(a.data.d_val - b.data.d_val) < 1e-9;
        case HT_CHAR:   return a.data.c_val == b.data.c_val;
        case HT_STRING: return strcmp(a.data.s_val, b.data.s_val) == 0;
        default:        return 0;
    }
}

ht_hash_table* ht_new(ht_data_type key_type, int size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = size;
    ht->count = 0;
    ht->key_type = key_type;
    ht->values = malloc(sizeof(ht_value) * size);
    
    for (int i = 0; i < size; i++) {
        ht->values[i] = create_empty_value();
    }
    return ht;
}

void ht_insert(ht_hash_table* ht, ht_value key, ht_value value) {
    if (ht->count >= ht->size / 2) {
        ht_resize(ht, ht->size * 2);
    }
    
    unsigned long hash = hash_primitive(key);
    unsigned long step = second_hash(key, ht->size);
    int index = hash % ht->size;
    int original_index = index;
    
    while (ht->values[index].type != HT_EMPTY && 
           ht->values[index].type != HT_DELETED) {
        if (values_equal(ht->values[index], key)) {
            if (index + 1 < ht->size) {
                ht->values[index + 1] = value;
            }
            return;
        }
        index = (index + step) % ht->size;
        if (index == original_index) {
            printf("Hash table is full!\n");
            return;
        }
    }
    
    ht->values[index] = key;
    
    if (index + 1 >= ht->size) {
        printf("Error: Not enough space for value storage\n");
        return;
    }
    ht->values[index + 1] = value;
    ht->count++;
}

void ht_resize(ht_hash_table* ht, int new_size) {
    ht_value* old_values = ht->values;
    int old_size = ht->size;
    
    ht->size = new_size;
    ht->count = 0;
    ht->values = malloc(sizeof(ht_value) * new_size);
    for (int i = 0; i < new_size; i++) {
        ht->values[i] = create_empty_value();
    }
    
    for (int i = 0; i < old_size; i += 2) {
        if (old_values[i].type != HT_EMPTY && old_values[i].type != HT_DELETED) {
            ht_insert(ht, old_values[i], old_values[i + 1]);
        }
        free_value(old_values[i]);
        free_value(old_values[i + 1]);
    }
    free(old_values);
}

ht_value ht_search(ht_hash_table* ht, ht_value key) {
    unsigned long hash = hash_primitive(key);
    unsigned long step = second_hash(key, ht->size);
    int index = hash % ht->size;
    int original_index = index;
    
    while (ht->values[index].type != HT_EMPTY) {
        if (values_equal(ht->values[index], key)) {
            if (index + 1 < ht->size) {
                return ht->values[index + 1];
            }
            return create_empty_value();
        }
        index = (index + step) % ht->size;
        if (index == original_index) break;
    }
    
    return create_empty_value();
}

void ht_delete(ht_hash_table* ht, ht_value key) {
    unsigned long hash = hash_primitive(key);
    unsigned long step = second_hash(key, ht->size);
    int index = hash % ht->size;
    int original_index = index;
    
    while (ht->values[index].type != HT_EMPTY) {
        if (values_equal(ht->values[index], key)) {
            free_value(ht->values[index]);
            if (index + 1 < ht->size) {
                free_value(ht->values[index + 1]);
                ht->values[index + 1] = create_deleted_value();
            }
            ht->values[index] = create_deleted_value();
            ht->count--;
            return;
        }
        index = (index + step) % ht->size;
        if (index == original_index) break;
    }
}

void free_value(ht_value v) {
    if (v.type == HT_STRING && v.data.s_val != NULL) {
        free(v.data.s_val);
    }
}

void ht_free(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        free_value(ht->values[i]);
    }
    free(ht->values);
    free(ht);
}

void print_value(ht_value v) {
    switch (v.type) {
        case HT_INT:     printf("%d", v.data.i_val); break;
        case HT_FLOAT:   printf("%.2f", v.data.f_val); break;
        case HT_DOUBLE:  printf("%.2lf", v.data.d_val); break;
        case HT_CHAR:    printf("'%c'", v.data.c_val); break;
        case HT_STRING:  printf("\"%s\"", v.data.s_val); break;
        case HT_EMPTY:   printf("[EMPTY]"); break;
        case HT_DELETED: printf("[DELETED]"); break;
    }
}

void ht_print(ht_hash_table* ht) {
    printf("Hash Table (size=%d, count=%d):\n", ht->size, ht->count);
    for (int i = 0; i < ht->size; i += 2) {
        if (i + 1 < ht->size && 
            ht->values[i].type != HT_EMPTY && 
            ht->values[i].type != HT_DELETED) {
            printf("  ");
            print_value(ht->values[i]);
            printf(" -> ");
            print_value(ht->values[i + 1]);
            printf("\n");
        }
    }
}
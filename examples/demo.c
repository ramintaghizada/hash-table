#include "../include/hash_table.h"

int main(void) {
    printf("===== HASH TABLE WITH MULTIPLE PRIMITIVE TYPES =====\n\n");
    
    ht_hash_table* ht = ht_new(HT_STRING, 8);
    
    printf("1. Inserting integer values:\n");
    ht_insert(ht, create_string_value("age"), create_int_value(25));
    ht_insert(ht, create_string_value("score"), create_int_value(95));
    ht_print(ht);
    
    printf("\n2. Inserting float and double values:\n");
    ht_insert(ht, create_string_value("pi"), create_double_value(3.14159));
    ht_insert(ht, create_string_value("temperature"), create_float_value(36.6f));
    ht_print(ht);
    
    printf("\n3. Inserting string values:\n");
    ht_insert(ht, create_string_value("name"), create_string_value("Alice"));
    ht_insert(ht, create_string_value("city"), create_string_value("Baku"));
    ht_print(ht);
    
    printf("\n4. Searching for values:\n");
    ht_value result = ht_search(ht, create_string_value("age"));
    if (result.type != HT_EMPTY) {
        printf("  age = ");
        print_value(result);
        printf("\n");
    }
    
    result = ht_search(ht, create_string_value("name"));
    if (result.type != HT_EMPTY) {
        printf("  name = ");
        print_value(result);
        printf("\n");
    }
    
    printf("\n5. Deleting 'score':\n");
    ht_delete(ht, create_string_value("score"));
    ht_print(ht);
    
    result = ht_search(ht, create_string_value("score"));
    if (result.type == HT_EMPTY) {
        printf("\n6. 'score' was successfully deleted.\n");
    }
    
    ht_free(ht);
    
    return 0;
}
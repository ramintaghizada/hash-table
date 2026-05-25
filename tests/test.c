#include "../include/hash_table.h"
#include <stdio.h>
#include <assert.h>

// Test counter
int tests_passed = 0;
int tests_failed = 0;

#define TEST(name) void name(void)
#define ASSERT(condition) \
    do { \
        if (condition) { \
            printf("  ✓ %s:%d\n", __FILE__, __LINE__); \
            tests_passed++; \
        } else { \
            printf("  ✗ FAILED: %s:%d\n", __FILE__, __LINE__); \
            tests_failed++; \
        } \
    } while(0)
#define RUN_TEST(test) \
    do { \
        printf("Running: %s\n", #test); \
        test(); \
    } while(0)

TEST(test_create_int) {
    ht_value v = create_int_value(42);
    ASSERT(v.type == HT_INT);
    ASSERT(v.data.i_val == 42);
}

TEST(test_create_string) {
    ht_value v = create_string_value("hello");
    ASSERT(v.type == HT_STRING);
    ASSERT(strcmp(v.data.s_val, "hello") == 0);
    free(v.data.s_val);
}

TEST(test_insert_and_search) {
    ht_hash_table* ht = ht_new(HT_STRING, 8);
    
    ht_insert(ht, create_string_value("name"), create_string_value("Alice"));
    ht_insert(ht, create_string_value("age"), create_int_value(25));
    
    ht_value result = ht_search(ht, create_string_value("name"));
    ASSERT(result.type == HT_STRING);
    ASSERT(strcmp(result.data.s_val, "Alice") == 0);
    
    result = ht_search(ht, create_string_value("age"));
    ASSERT(result.type == HT_INT);
    ASSERT(result.data.i_val == 25);
    
    ht_free(ht);
}

TEST(test_delete) {
    ht_hash_table* ht = ht_new(HT_STRING, 8);
    
    ht_insert(ht, create_string_value("key"), create_int_value(100));
    ASSERT(ht_search(ht, create_string_value("key")).type != HT_EMPTY);
    
    ht_delete(ht, create_string_value("key"));
    ASSERT(ht_search(ht, create_string_value("key")).type == HT_EMPTY);
    
    ht_free(ht);
}

TEST(test_resize) {
    ht_hash_table* ht = ht_new(HT_STRING, 4);
    int initial_size = ht->size;
    
    for (int i = 0; i < 10; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        ht_insert(ht, create_string_value(key), create_int_value(i));
    }
    
    ASSERT(ht->size > initial_size);
    ASSERT(ht->count == 10);
    
    ht_free(ht);
}

TEST(test_multiple_types) {
    ht_hash_table* ht = ht_new(HT_STRING, 8);
    
    ht_insert(ht, create_string_value("int_val"), create_int_value(42));
    ht_insert(ht, create_string_value("float_val"), create_float_value(3.14f));
    ht_insert(ht, create_string_value("double_val"), create_double_value(2.71828));
    ht_insert(ht, create_string_value("char_val"), create_char_value('A'));
    ht_insert(ht, create_string_value("string_val"), create_string_value("test"));
    
    ASSERT(ht_search(ht, create_string_value("int_val")).data.i_val == 42);
    ASSERT(fabs(ht_search(ht, create_string_value("float_val")).data.f_val - 3.14f) < 0.01);
    ASSERT(fabs(ht_search(ht, create_string_value("double_val")).data.d_val - 2.71828) < 0.00001);
    ASSERT(ht_search(ht, create_string_value("char_val")).data.c_val == 'A');
    ASSERT(strcmp(ht_search(ht, create_string_value("string_val")).data.s_val, "test") == 0);
    
    ht_free(ht);
}

int main(int argc, char** argv) {
    printf("\n========================================\n");
    printf("RUNNING HASH TABLE TESTS\n");
    printf("========================================\n\n");
    
    RUN_TEST(test_create_int);
    RUN_TEST(test_create_string);
    RUN_TEST(test_insert_and_search);
    RUN_TEST(test_delete);
    RUN_TEST(test_resize);
    RUN_TEST(test_multiple_types);
    
    printf("\n========================================\n");
    printf("RESULTS: %d passed, %d failed\n", tests_passed, tests_failed);
    printf("========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
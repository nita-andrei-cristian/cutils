#ifndef CSTRING_NITA
#define CSTRING_NITA

#include <stddef.h>
#include "stdbool.h"

#define INIT_STRING_CAP 128

#define create_string_fixed(memory_space) ((String){ .p = (char[memory_space]){0}, .fixed = 1, .cap = (memory_space), .len = 0 })

typedef struct String {
	char* p;
	bool fixed;
	bool sync;
	size_t len;
	size_t cap;
} String;

void init_string_empty(String *ref);
void init_string_copy(String *ref, char* p, size_t len);

// Returns a string pointer, passes ownership upward.
String *create_string();

// Returns a string pointer, passes ownership upward.
String *create_string_copy(char* p, size_t len);

void free_string(String *ref);

void empty_string(String *s);

void copy_string_sc(String *A, char* p, size_t len);

void resize_string_over(String *A, size_t target);

// Cat character to string
void cat_sc(String *A, char* p, size_t len);

// Cat string to string
static inline void cat_ss(String* A, String* B){
	cat_sc(A, B->p, B->len);
}

static inline void copy_string_ss(String *A, String *B){
	copy_string_sc(A, B->p, B->len);
}

#endif

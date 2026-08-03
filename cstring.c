#include "cstring.h"
#include "cassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_string_empty(String *ref){
	cassert(ref, "init_empty_string called with an empty pointer");
	ref->p = malloc(INIT_STRING_CAP);
	cassert(ref, "Couldn't allocate memory for ref, tried to alloc %d", INIT_STRING_CAP);

	ref->cap = INIT_STRING_CAP;
	ref->len = 0;
	(*ref->p) = '\0';
	
	ref->fixed = 0;
	ref->sync = 0;
}

void init_string_copy(String *ref, char* p, size_t len){
	cassert(ref, "init_string_copy called with an empty ref");
	if (len == 0) {
		init_string_empty(ref);
		return;
	}

	cassert(p, "init_string_copy called with an empty p and len != 0, len = [%zu]", len);

	ref->p = malloc(len + len / 4);
	cassert(ref->p, "Couldn't allocate pointer for copying message of size [%zu] with contents: [%s]", len, p);
	
	ref->fixed = 0;
	ref->sync = 0;
	ref->cap = len + len / 4 + 1;
	ref->len = len;

	ref->p[len] = '\0';

	memcpy(ref->p, p, len);
}

void init_string_sync(String *ref, char* p, size_t len){
	cassert(ref, "init_string_copy called with an empty ref");
	if (len == 0) {
		init_string_empty(ref);
		return;
	}

	cassert(p, "init_string_copy called with an empty p and len != 0, len = [%zu]", len);

	ref->p = p;
	ref->fixed = 0;
	ref->sync = 1;
	ref->cap = len;
	ref->len = len;
}

// Returns a string pointer, passes ownership upward.
String *create_string(){
	String* ref = malloc(sizeof(String));
	cassert(ref, "Couldn't allocate memory for ref, tried to alloc %zu", sizeof(String));

	init_string_empty(ref);

	return ref;
}

// Returns a string pointer, passes ownership upward.
String *create_string_copy(char* p, size_t len){

	if (len == 0)
		return create_string();

	cassert(p, "copy_to_string function passed with a length greater than 0 of size [%zu] and a null pointer", len);

	String* ref = malloc(sizeof(String));
	cassert(ref, "Couldn't allocate memory for ref, tried to alloc %zu", sizeof(String));

	init_string_copy(ref, p, len);

	return ref;
}

// Returns a string pointer, passes ownership upward
String *create_string_sync(char* p, size_t len){
	if (len == 0)
		return create_string();

	cassert(p, "create_string_sync function passed with a length greater than 0 of size [%zu] and a null pointer", len);

	String* ref = malloc(sizeof(String));
	cassert(ref, "Couldn't allocate memory for ref, tried to alloc %zu", sizeof(String));

	init_string_sync(ref, p, len);

	return ref;
}

void free_string(String *ref){
	if (ref && !ref->fixed){
		if (ref->p)
			free(ref->p);
		free(ref);
	}
}

void empty_string(String *s){
	cassert(s, "empty_string: Passed no s value");

	s->len = 0;
	*s->p = '\0';
}

void copy_string_sc(String *A, char* p, size_t len){
	cassert(A, "copy_string_sc: No string value");

	if (len == 0){
		empty_string(A);
		return;
	}

	cassert(p, "copy_string_sc: No pointer value");

	resize_string_over(A, len);

	if (A->fixed && len >= A->cap){
		memcpy(A->p, p, A->cap-1);
		A->len = A->cap - 1;
	}else{
		memcpy(A->p, p, len);
		A->len = len;
	}
	*(A->p + A->len) = '\0';
}

void resize_string_over(String *A, size_t target){
	cassert(A, "resize_over: not provided with a string");

	if (A->fixed) return;

	target += 1; // account for \0

	if (target < A->cap) return;

	size_t req_capacity = A->cap;
	while (req_capacity <= target)
		req_capacity *= 2;

	cassert(req_capacity > A->cap, "Something wrong gone when resizing a string, Probably overflow, assuming couldn't resize to %zu from %zu", req_capacity, A->cap);

	char *t = realloc(A->p, req_capacity);
	cassert(t, "Coudln't reallocate memory for A at [%zu] from [%zu]", req_capacity, A->cap);

	if (!t) return;

	A->p = t;
	A->cap = req_capacity;
}

// Cat character to string
void cat_sc(String *A, char* p, size_t len){
	resize_string_over(A, A->len + len);

	if (A->fixed && A->len + len >= A->cap){
		memcpy(A->p + A->len, p, A->cap - A->len);
		A->len = A->cap - 1;
	}else{
		memcpy(A->p + A->len, p, len);
		A->len += len;
	}

	*(A->p + A->len) = '\0';

}

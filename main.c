#include "cstring.h"
#include <stdio.h>

int main(){
	String *test = create_string();

	char message[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc";
	String *test2 = create_string_copy(message, sizeof(message) - 1);

	char message2[] = "def";
	String *test3 = create_string_copy(message2, sizeof(message2) - 1);

	String test4 = create_string_fixed(32);

	copy_string_ss(test3, test2);
	cat_ss(&test4, test3);

	printf("|%s| %zu\n\n", test4.p, test4.len);


}

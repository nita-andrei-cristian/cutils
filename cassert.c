#include "cassert.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void cassert_impl(
	bool assertion,
	const char* file,
	int line,
	const char* func,
	const char* fmt,
	...
){
	if (assertion) return;

	if (!fmt) {
		fmt = "Assertion failed";
	}

	va_list args;
	va_start(args, fmt);

	va_list args_copy;
	va_copy(args_copy, args);

	int needed = vsnprintf(NULL, 0, fmt, args_copy);
	va_end(args_copy);

	char fallback_message[1024];
	char* user_message = NULL;

	if (needed < 0) {
		snprintf(fallback_message, sizeof(fallback_message), "Assertion message formatting failed");
		user_message = fallback_message;
	} else {
		user_message = malloc((size_t)needed + 1);

		if (user_message) {
			vsnprintf(user_message, (size_t)needed + 1, fmt, args);
		} else {
			vsnprintf(fallback_message, sizeof(fallback_message), fmt, args);
			user_message = fallback_message;
		}
	}

	va_end(args);

	time_t now = time(0);
	struct tm* tm_info = localtime(&now);

	char timestamp[64];

	if (tm_info) {
		strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", tm_info);
	} else {
		snprintf(timestamp, sizeof(timestamp), "unknown-time");
	}

	char final_message[4096];

	int final_len = snprintf(
		final_message,
		sizeof(final_message),
		"----------------------\n\n\n"
		"CASSERT FAILURE MESSAGE\n\n\n"
		"File: %s\n"
		"Line: %d\n"
		"Function: %s\n\n"
		"[%s]\n",
		file,
		line,
		func,
		user_message
	);

	if (final_len < 0) {
		fprintf(stderr, "CHANGE FAILURE MESSAGE\n[Could not format failure message]\n");
	} else {
		fprintf(stderr, "%s", final_message);
	}

	if (user_message != fallback_message) {
		free(user_message);
	}

	exit(EXIT_FAILURE);
}

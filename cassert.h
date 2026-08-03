#ifndef CASSERT_NITA
#define CASSERT_NITA

#include <stddef.h>
#include "stdbool.h"

#if defined(__GNUC__) || defined(__clang__)
	#define PRINTF_LIKE(fmt_index, first_arg) \
		__attribute__((format(printf, fmt_index, first_arg)))
#else
	#define PRINTF_LIKE(fmt_index, first_arg)
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#define cassert(assertion, fmt, ...) \
		cassert_impl((assertion), __FILE__, __LINE__, __func__, (fmt) __VA_OPT__(,) __VA_ARGS__)
#else
	#define cassert(assertion, fmt, ...) \
		cassert_impl((assertion), __FILE__, __LINE__, __func__, (fmt), ##__VA_ARGS__)
#endif

void cassert_impl(
	bool assertion,
	const char* file,
	int line,
	const char* func,
	const char* fmt,
	...
) PRINTF_LIKE(5, 6);

#endif

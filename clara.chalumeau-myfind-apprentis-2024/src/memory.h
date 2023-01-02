#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void *my_malloc(size_t size);
void *my_calloc(size_t nmemb, size_t size);
void *my_reallocarray(void *ptr, size_t nmemb, size_t size);

#endif /* ! MEMORY_H */

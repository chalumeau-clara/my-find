#include "memory.h"

#include <err.h>
#include <stdlib.h>

static inline void memory_exhausted(void)
{
    err(1, "Memory exhausted.");
}

void *my_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (size && !ptr)
        memory_exhausted();

    return ptr;
}

void *my_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (size && nmemb && !ptr)
        memory_exhausted();

    return ptr;
}

void *my_reallocarray(void *ptr, size_t nmemb, size_t size)
{
    ptr = reallocarray(ptr, nmemb, size);
    if (size && nmemb && !ptr)
        memory_exhausted();

    return ptr;
}

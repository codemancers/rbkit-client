#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include <pthread.h>


void* malloc(size_t size)
{
    void* (*real_malloc)(size_t);
    void* ptr = NULL;

    real_malloc = dlsym(RTLD_NEXT, "malloc");
    ptr = real_malloc(size);

    fprintf(stderr, "%lu:m%lu,%lu\n", pthread_self(),
            ptr, (unsigned long)size);

    return ptr;
}

void free(void* ptr)
{
    void (*real_free)(void*);
    real_free = dlsym(RTLD_NEXT, "free");

    fprintf(stderr, "%lu:f%lu\n", pthread_self(), ptr);

    return real_free(ptr);
}

void* realloc(void* ptr, size_t size)
{
    void* (*real_realloc)(void*, size_t);
    void* realloc_ptr = NULL;

    real_realloc = dlsym(RTLD_NEXT, "realloc");
    realloc_ptr = real_realloc(ptr, size);

    fprintf(stderr, "%lu:r%lu,%lu,%lu\n", pthread_self(),
            ptr, realloc_ptr, (unsigned long)size);

    return realloc_ptr;
}

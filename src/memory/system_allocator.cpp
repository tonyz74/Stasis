#include "system_allocator.h"

option<void *> system_allocator::alloc(u32 n) {
    void *ret = malloc(n);

    if (ret == NULL) {
        return option<void *>::none(); 
    }

    return option<void *>::some((void *) ret);
}

void system_allocator::dealloc(void *data) {
    free(data);
}

#include "system_allocator.h"

option<u8 *> system_allocator::alloc(u32 n) {
    void *ret = malloc(n);

    if (ret == NULL) {
        return option<u8 *>::none(); 
    }

    return option<u8 *>::some((u8 *) ret);
}

void system_allocator::dealloc(u8 *data) {
    free(data);
}

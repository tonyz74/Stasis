#ifndef STASIS_MEMORY_SYSTEM_ALLOCATOR_H
#define STASIS_MEMORY_SYSTEM_ALLOCATOR_H

#include "core/common.h"
#include <stdlib.h>

// Allocator that uses malloc and free
class system_allocator : public allocator {
    virtual option<void *> alloc(u32 n) override;
    virtual void dealloc(void *bytes) override;
};

#endif // !STASIS_MEMORY_SYSTEM_ALLOCATOR_H

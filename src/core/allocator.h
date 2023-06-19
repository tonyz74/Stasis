#ifndef STASIS_CORE_ALLOCATOR_H
#define STASIS_CORE_ALLOCATOR_H

#include "types.h"
#include "option.h"

class allocator {
public:
    virtual option<void *> alloc(u32 n) = 0;
    virtual void dealloc(void *bytes) = 0;
};

#endif // !STASIS_CORE_ALLOCATOR_H

#ifndef STASIS_CORE_ALLOCATOR_H
#define STASIS_CORE_ALLOCATOR_H

#include "types.h"
#include "option.h"

class allocator {
public:
    virtual option<u8 *> alloc(u32 n) = 0;
    virtual void dealloc(u8 *bytes) = 0;
};

#endif // !STASIS_CORE_ALLOCATOR_H

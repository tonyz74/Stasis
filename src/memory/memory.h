#ifndef STASIS_MEMORY_H
#define STASIS_MEMORY_H

#include "core/common.h"
#include "tag.h"

#define MAX_MEMORY_USAGE (2 * 1024 * 1024 * 1024)

struct memory_stats {
    u32 total_usage = 0;
    u32 tagged_usage[MEMORY_TAG_N_VALUES] = {0};
};


class memory {
public:
    static void * alloc(memory_tag tag, u32 n);
    static void dealloc(memory_tag tag, void *mem, u32 n);

    static const char * display_stats();
    static memory_stats get_stats();

private:
    static memory_stats stats;
};

#endif // !STASIS_MEMORY_H

#include "memory.h"
#include "core/asserts.h"
#include "memory/tag.h"
#include <stdlib.h>

memory_stats memory::stats = {};

void * memory::alloc(memory_tag tag, u32 n) {
    void *mem = malloc(n);
    ASSERT(mem != NULL, "Could not allocate any more memory!");

    memory::stats.total_usage += n;

    ASSERT(
        memory::stats.total_usage < MAX_MEMORY_USAGE,
        "Memory usage exceeded maximum!"
    );

    memory::stats.tagged_usage[(u32) tag] += n;

    return mem;
}

void memory::dealloc(memory_tag tag, void *mem, u32 n) {
    ASSERT(
        memory::stats.tagged_usage[(u32) tag] > n,
        "Deallocating too much memory!"
    );

    memory::stats.tagged_usage[(u32) tag] -= n;

    free(mem);
}

memory_stats memory::get_stats() {
    return memory::stats;
}

const char * memory::display_stats() {
    // Maximum the display can go up to
    static char display_buf[4096];
    u32 max_size = sizeof(display_buf);

    u32 len = 0;
    len += snprintf(display_buf + len, max_size - len, "Memory Usage Stats:\n");
    
    u32 tag = (u32) memory_tag::UNKNOWN;
    while (tag != MEMORY_TAG_N_VALUES) {
        len += snprintf(
            display_buf + len,
            max_size - len,
            "  %s: %u bytes\n",
            MEMORY_TAG_NAMES[tag],
            memory::stats.tagged_usage[tag]
        );
        tag++;
    }
    
    len += snprintf(
        display_buf + len,
        max_size - len,
        "\n  Total: %u bytes\n",
        memory::stats.total_usage
    );

    display_buf[len] = 0;

    return display_buf;
}

#ifndef STASIS_CORE_ASSERTS_H
#define STASIS_CORE_ASSERTS_H

#include "log.h"

#define PANIC(...) do {\
    FATAL(__VA_ARGS__);\
    exit(1);\
} while(0)

#define ASSERT(cond, ...) do {\
    if (!(cond)) {\
        fprintf(stderr, "Assertion Failed: (%s) == false\n", #cond);\
        PANIC(__VA_ARGS__);\
    }\
} while (0)


#endif // STASIS_CORE_ASSERTS_H

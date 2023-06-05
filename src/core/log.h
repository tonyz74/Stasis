#ifndef STASIS_CORE_LOG
#define STASIS_CORE_LOG

#include <stdio.h>
#include <stdlib.h>

#define PANIC(...) do {\
    fprintf(stderr, __VA_ARGS__);\
    exit(1);\
} while(0)


#endif // !STASIS_CORE_LOG

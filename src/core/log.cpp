#include "log.h"
#include <ctime>


static const char *LOG_LEVEL_NAMES[6] = {
    "TRACE",
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL"
};

static const char *LOG_COLORS[6] = {
    "\x1b[36m",
    "\x1b[34m",
    "\x1b[32m",
    "\x1b[35m",
    "\x1b[31m",
    "\x1b[31m",
};

void log_common(
    log_level lvl,
    const char *file,
    i32 line,
    const char *msg,
    ...
) {
#define STOP "\x1b[0m"
#define GRAY "\x1b[90m"

    time_t t = time(nullptr); 
    struct tm* local_t = localtime(&t);

    i32 idx = (i32) lvl;

    fprintf(
        stderr,
        "%s%d:%d:%d %s%s%s %s%s:%d:%s ",
        STOP,
        local_t->tm_hour,
        local_t->tm_min,
        local_t->tm_sec,
        LOG_COLORS[idx],
        LOG_LEVEL_NAMES[idx],
        STOP,
        GRAY,
        file, line,
        STOP
    );

    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
}

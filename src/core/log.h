#ifndef STASIS_CORE_LOG_H
#define STASIS_CORE_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "types.h"

enum struct log_level {
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};


void log_common(
    log_level lvl,
    const char *file,
    i32 line,
    const char *msg, ...
);

#define TRACE(...) log_common(log_level::TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG(...) log_common(log_level::DEBUG, __FILE__, __LINE, __VA_ARGS__)
#define INFO(...) log_common(log_level::INFO, __FILE__, __LINE__, __VA_ARGS__)
#define WARN(...) log_common(log_level::WARN, __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) log_common(log_level::ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define FATAL(...) log_common(log_level::FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif // !STASIS_CORE_LOG_H

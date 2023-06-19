#ifndef STASIS_MEMORY_TAG_H
#define STASIS_MEMORY_TAG_H

#define MEMORY_TAG_N_VALUES 11

enum struct memory_tag {
    UNKNOWN = 0,

    ECS,
    JOB,
    EVENT,
    FILES,
    TEXTURE,
    RENDER,
    SCRATCH,
    RESOURCE,
    SERIALIZATION,

    USER
};

static const char *MEMORY_TAG_NAMES[MEMORY_TAG_N_VALUES] = {
    "UNKNOWN",
    "ECS",
    "JOB",
    "EVENT",
    "FILES",
    "TEXTURE",
    "RENDER",
    "SCRATCH",
    "RESOURCE",
    "SERIALIZATION",
    "USER",
};

#endif // !STASIS_MEMORY_TAG_H

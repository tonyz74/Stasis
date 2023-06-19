#ifndef STASIS_PLATFORM_FS_H
#define STASIS_PLATFORM_FS_H

#include "core/common.h"

enum struct file_mode {
    READ,
    WRITE
};


struct file {
public:
    static option<file> open(const char *abs_path, file_mode mode);

    u32 get_size() const;

    u32 read(u8 *output_buf, u32 buf_size);
    u32 read_all(u8 *output_buf);
    u8 * read_all_alloc(allocator *a);

    u32 write(u8 *buf, u32 len);
    u32 write_all(u8 *buf, u32 len);

    void close();

private:
    i32 fd;
    bool read_only;
};

#endif // !STASIS_CORE_FS_H

#include "fs.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

option<file> file::open(const char *abs_path, file_mode mode) {
    const char *mode_str = "r";
    if (mode == file_mode::WRITE) {
        mode_str = "w";
    }

    FILE *f = fopen(abs_path, mode_str);
    if (f == NULL) {
        return option<file>::none();
    }

    file ret;
    ret.fd = fileno(f);
    ret.read_only = mode == file_mode::READ;

    return option<file>::some(ret);
}

u32 file::get_size() const {
    struct stat stat;
    fstat(this->fd, &stat);

    return stat.st_size;
}

u32 file::read(u8 *output_buf, u32 buf_size) {
    ssize_t read_amount = ::read(this->fd, output_buf, buf_size);
    return read_amount;
}

u32 file::read_all(u8 *output_buf) {
    u32 total = get_size();

    u32 total_read = 0;
    u32 remaining = total;

    while (total_read != total) {
        total_read += this->read(output_buf, remaining);
        remaining -= total_read;
    }
    
    return total_read;
}


u8 * file::read_all_alloc(allocator *a) {
    u8 *result = (u8 *) a->alloc(get_size() + 1).unwrap();
    u32 len = this->read_all(result);
    result[len] = '\0';

    return result;
}


u32 file::write(u8 *buf, u32 len) {
    return ::write(this->fd, buf, len);
}

u32 file::write_all(u8 *buf, u32 len) {
    u32 remaining = len;
    u32 n_written = 0;

    while (remaining != 0) {
        n_written += this->write(buf + n_written, remaining);
        remaining -= n_written;
    }

    return n_written;
}

void file::close() {
    ::close(this->fd);
}

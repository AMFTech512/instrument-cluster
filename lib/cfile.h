#include <stdio.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

size_t writeFile(int fd, void* ptr, size_t len) {
    return write(fd, ptr, len);
}
#include <fs.h>
#include <nano.h>


size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, serial_write},
  [FD_DISPINFO] = {"/proc/dispinfo", 23, 0, 0, dispinfo_read, invalid_write},
  [FD_FB] = {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  [FD_EVENT] = {"/dev/events", 0, 0, 0, events_read, invalid_write},
#include "files.h"
};

void init_fs() {
  AM_GPU_CONFIG_T config = io_read(AM_GPU_CONFIG);
  file_table[FD_FB].size = config.width * config.height * sizeof(uint32_t);

  for (int i = FD_EVENT + 1; i < sizeof(file_table) / sizeof(Finfo); i++) {
    file_table[i].open_offset = 0;
    file_table[i].read = ramdisk_read;
    file_table[i].write = ramdisk_write;
  }
}

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < sizeof(file_table) / sizeof(Finfo); i++) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      //printf("name: %s, fd: %d\n", pathname, i);
      file_table[i].open_offset = 0;
      return i;
    }
  }
  return -1;
}
size_t fs_read(int fd, void *buf, size_t len) {
  assert(fd < sizeof(file_table) / sizeof(Finfo));
  size_t real_len = len;
  size_t size = file_table[fd].size;
  size_t open_offset = file_table[fd].open_offset;
  if (open_offset + len > size) {
    real_len = size - open_offset;
  }
  size_t rt = file_table[fd].read(buf, open_offset + file_table[fd].disk_offset, real_len);
  file_table[fd].open_offset += rt;
  return rt;

}
size_t fs_write(int fd, const void *buf, size_t len) {
  assert(fd < sizeof(file_table) / sizeof(Finfo));
  size_t rt = file_table[fd].write(buf, file_table[fd].open_offset + file_table[fd].disk_offset, len);
  file_table[fd].open_offset += rt;
  return rt;

}
size_t fs_lseek(int fd, size_t offset, int whence) {
  size_t new_offset;
  if (whence == SEEK_SET) {
      new_offset = offset;
  } else if (whence == SEEK_CUR) {
      new_offset = file_table[fd].open_offset + offset;
  } else if (whence == SEEK_END) {
      new_offset = file_table[fd].size + offset;
  } else {
      return (size_t)-1; // Invalid whence
  }

  if (new_offset > file_table[fd].size || new_offset < 0) {
      return (size_t)-1; // Out of bounds
  }

  file_table[fd].open_offset = new_offset;
  return new_offset;

}
int fs_close(int fd) {
  return 0;
}
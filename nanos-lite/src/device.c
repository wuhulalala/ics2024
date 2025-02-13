#include <common.h>
#include <am.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; i++) {
    putch(((char*)buf)[i]);
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T event;
  ioe_read(AM_INPUT_KEYBRD, &event);
  int ret = 0;
  if (event.keycode == AM_KEY_NONE) {
    *(char*)buf = '\0';
  } else {
    ret = sprintf(buf, "%s %s\n\0", event.keydown ? "kd" : "ku", keyname[event.keycode]);
  }
  
  return ret;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int width = io_read(AM_GPU_CONFIG).width;
  int height = io_read(AM_GPU_CONFIG).height;

  int ret = snprintf(buf, len, "%d %d\n", width, height);
  
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}

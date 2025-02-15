#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;


int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
}

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (uint32_t)(tv.tv_sec * 1000);
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", 0, 0);
  int ret = read(fd, buf, len);
  close(fd);
  return ret;
}

void NDL_OpenCanvas(int *w, int *h) {
  char buf[64];
  int fd = open("/proc/dispinfo", 0, 0);
  if (fd < 0) {
      perror("Failed to open /proc/dispinfo");
      return;
  }

  int nread = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  
  if (nread > 0) {
      buf[nread] = '\0'; 
      //printf("%s\n", buf);
      sscanf(buf, "%d %d", &screen_w, &screen_h); 
  } else {
      printf("Failed to read screen size\n");
  }
  if (*w == 0 || *h == 0) {
    *w = screen_w;
    *h = screen_h;
  }
  printf("Canvas size: %d x %d\n", *w, *h); 
  printf("Screen size: %d x %d\n", screen_w, screen_h); 
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  if (w == 0 || h == 0) {
    w = screen_w;
    h = screen_h;
  }
  int fd = open("/dev/fb", 0, 0);
  if (fd < 0) return;
  
  int canvas_x = (screen_w - w) / 2;
  int canvas_y = (screen_h - h) / 2;
  
  for (int i = 0; i < h; i++) {
    size_t offset = ((canvas_y + y + i) * screen_w + (canvas_x + x)) * sizeof(uint32_t);
    lseek(fd, offset, SEEK_SET);  
    write(fd, pixels + i * w, w * sizeof(uint32_t));
  }
  
  close(fd);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
  assert(0 && "NDL_OpenAudio() is not implemented!");
}

void NDL_CloseAudio() {
  assert(0 && "NDL_CloseAudio() is not implemented!");
}

int NDL_PlayAudio(void *buf, int len) {
  assert(0 && "NDL_PlayAudio() is not implemented!");
  return 0;
}

int NDL_QueryAudio() {
  assert(0 && "NDL_QueryAudio() is not implemented!");
  return 0;
}


void NDL_Quit() {
}

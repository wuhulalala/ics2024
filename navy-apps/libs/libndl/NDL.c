#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int fd;


int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  fd = open("/dev/events", 0, 0);
}

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (uint32_t)(tv.tv_sec * 1000);
}

int NDL_PollEvent(char *buf, int len) {
  return read(fd, buf, len);
}

void NDL_OpenCanvas(int *w, int *h) {
    char buf[64];
    int fd = open("/proc/dispinfo", 0, 0);
    if (fd < 0) {
        perror("Failed to open /proc/dispinfo");
        return;
    }

    // 读取屏幕信息
    int nread = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    
    if (nread > 0) {
        buf[nread] = '\0'; // 确保字符串结束
        sscanf(buf, "%d %d", &screen_w, &screen_h); // 解析宽度和高度
        printf("Screen size: %d x %d\n", screen_w, screen_h); // 输出屏幕大小
    } else {
        printf("Failed to read screen size\n");
    }

    // 记录画布的大小
    *w = screen_w;
    *h = screen_h;
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}


void NDL_Quit() {
  close(fd);
}

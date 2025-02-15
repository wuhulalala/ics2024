#include <common.h>
#include "syscall.h"
#include <stdio.h>
#include <proc.h>
#include <nano.h>

//#define STRACE

typedef uint32_t (*sys_handler)(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);

static uint32_t sys_execve(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);

static Finfo file_names[] __attribute__((unused)) = {
  [0]  = {"stdin", 0, 0, 0, NULL, NULL},
  [1] = {"stdout", 0, 0, 0, NULL, NULL},
  [2] = {"stderr", 0, 0, 0, NULL, NULL},
  [3] = {"/proc/dispinfo", 0, 0, 0, NULL, NULL},
  [4] = {"/dev/fb", 0, 0, 0, NULL, NULL},
  [5] = {"/dev/events", 0, 0, 0, NULL, NULL},
#include "files.h"
};

static const char *syscall_names[] __attribute__((unused)) = {
  [SYS_exit]        = "exit",
  [SYS_yield]       = "yield",
  [SYS_open]        = "open",
  [SYS_read]        = "read",
  [SYS_write]       = "write",
  [SYS_kill]        = "kill",
  [SYS_getpid]      = "getpid",
  [SYS_close]       = "close",
  [SYS_lseek]       = "lseek",
  [SYS_brk]         = "brk",
  [SYS_fstat]       = "fstat",
  [SYS_signal]      = "signal",
  [SYS_execve]      = "execve",
  [SYS_fork]        = "fork",
  [SYS_link]        = "link",
  [SYS_unlink]      = "unlink",
  [SYS_wait]        = "wait",
  [SYS_times]       = "times",
  [SYS_gettimeofday] = "gettimeofday",
};

static uint32_t sys_yield(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  yield();
  return 0;
}

static uint32_t sys_exit(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  char * filename = "/bin/nterm";
  sys_execve((uintptr_t)filename, 0, 0);
  return 0;

}

static uint32_t sys_write(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  int fd = arg1;
  char *buf = (char*)arg2;
  size_t count = arg3;
  return fs_write(fd, buf, count);
}

static uint32_t sys_read(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  int fd = arg1;
  char *buf = (char*)arg2;
  size_t count = arg3;
  return fs_read(fd, buf, count);

}

static uint32_t sys_open(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  char *pathname = (char*)arg1;
  int flags = (int)arg2;
  int mode = (int)arg3;
  return fs_open(pathname, flags, mode);
  

}

static uint32_t sys_close(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  return fs_close((int)arg1);

}

static uint32_t sys_kill(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  return 0;

}
static uint32_t sys_getpid(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  return 0;

}
static uint32_t sys_lseek(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  int fd = (int)arg1;
  uint32_t offset = (uint32_t)arg2;
  int whence = (int)arg3;
  return fs_lseek(fd, offset, whence);

}
static uint32_t sys_brk(uintptr_t addr, uintptr_t arg2, uintptr_t arg3) {

  if (current->max_brk == 0) {
    //printf("brk == 0 world\n");
    current->max_brk = addr;
    return 0;
  }

  if (addr > current->max_brk) {
    //printf("addr > max_brk \n");

    current->max_brk = addr;
  }
  
  return 0;
}
static uint32_t sys_fstat(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  return 0;

}
static uint32_t sys_time(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  return 0;

}

static uint32_t sys_signal(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return 0;
}

static uint32_t sys_execve(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    const char* filename = (const char*)arg1;
    bool find = false;
    for (int i = 0; i < sizeof(file_names) / sizeof(Finfo); i++) {
      if (strcmp(filename, file_names[i].name) == 0) {
        find = true;
        break;
      }
    }
    if (!find) return -1;
    naive_uload(current, filename);
    return 0;
}

static uint32_t sys_fork(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return 0;
}

static uint32_t sys_link(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return 0;
}

static uint32_t sys_unlink(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return 0;
}

static uint32_t sys_wait(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return 0;
}

static uint32_t sys_times(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
    return 0;
}

static uint32_t sys_gettimeofday(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3) {
  timeval* tv = (timeval*)arg1;
  long us = io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec = us / 1000000;
  tv->tv_usec = us;
  return 0;
}

static sys_handler syscall_table[] = {
    [SYS_exit]       = sys_exit,
    [SYS_yield]      = sys_yield,
    [SYS_open]       = sys_open,
    [SYS_read]       = sys_read,
    [SYS_write]      = sys_write,
    [SYS_kill]       = sys_kill,
    [SYS_getpid]     = sys_getpid,
    [SYS_close]      = sys_close,
    [SYS_lseek]      = sys_lseek,
    [SYS_brk]        = sys_brk,
    [SYS_fstat]      = sys_fstat,
    [SYS_time]       = sys_time,
    [SYS_signal]     = sys_signal,
    [SYS_execve]     = sys_execve,
    [SYS_fork]       = sys_fork,
    [SYS_link]       = sys_link,
    [SYS_unlink]     = sys_unlink,
    [SYS_wait]       = sys_wait,
    [SYS_times]      = sys_times,
    [SYS_gettimeofday] = sys_gettimeofday,
};

#ifdef STRACE
  #define SYSCALL_WITH_LOG(name, fmt, ...) do { \
    printf(fmt, ##__VA_ARGS__); \
    c->GPRx = syscall_table[name](a[1], a[2], a[3]); \
    printf("= %d\n", c->GPRx); \
  } while(0)
#else
  #define SYSCALL_WITH_LOG(name, fmt, ...) \
    c->GPRx = syscall_table[name](a[1], a[2], a[3])
#endif

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

#ifdef STRACE
  printf("%s(", syscall_names[a[0]]);
#endif
  
  switch (a[0]) {
    case SYS_yield:
      SYSCALL_WITH_LOG(SYS_yield, ") ");
      break;

    case SYS_exit:
      SYSCALL_WITH_LOG(SYS_exit, "%d) ", (int)a[1]);
      break;

    case SYS_write:
      SYSCALL_WITH_LOG(SYS_write, "%s, 0x%x, %d) ", 
                      file_names[a[1]].name, (uint32_t)a[2], (int)a[3]);
      break;

    case SYS_brk:
      SYSCALL_WITH_LOG(SYS_brk, ") ");
      break;

    case SYS_open:
      SYSCALL_WITH_LOG(SYS_open, "\"%s\", %d, %d) ", 
                      (char*)a[1], (int)a[2], (int)a[3]);
      break;

    case SYS_read:
      SYSCALL_WITH_LOG(SYS_read, "%s, 0x%x, %d) ", 
                      file_names[a[1]].name, (uint32_t)a[2], (int)a[3]);
      break;

    case SYS_close:
      SYSCALL_WITH_LOG(SYS_close, "%d) ", (int)a[1]);
      break;

    case SYS_lseek:
      SYSCALL_WITH_LOG(SYS_lseek, "%d, %u, %d) ", 
                      (int)a[1], (uint32_t)a[2], (int)a[3]);
      break;

    case SYS_fstat:
      SYSCALL_WITH_LOG(SYS_fstat, "%d, %p) ", (int)a[1], (void*)a[2]);
      break;

    case SYS_time:
      SYSCALL_WITH_LOG(SYS_time, ") ");
      break;

    case SYS_signal:
      SYSCALL_WITH_LOG(SYS_signal, "%d, %p) ", (int)a[1], (void*)a[2]);
      break;

    case SYS_execve:
      SYSCALL_WITH_LOG(SYS_execve, "\"%s\", %p, %p) ", 
                      (char*)a[1], (void*)a[2], (void*)a[3]);
      break;

    case SYS_fork:
      SYSCALL_WITH_LOG(SYS_fork, ") ");
      break;

    case SYS_link:
      SYSCALL_WITH_LOG(SYS_link, "\"%s\", \"%s\") ", 
                      (char*)a[1], (char*)a[2]);
      break;

    case SYS_unlink:
      SYSCALL_WITH_LOG(SYS_unlink, "\"%s\") ", (char*)a[1]);
      break;

    case SYS_wait:
      SYSCALL_WITH_LOG(SYS_wait, ") ");
      break;

    case SYS_times:
      SYSCALL_WITH_LOG(SYS_times, ") ");
      break;

    case SYS_gettimeofday:
      SYSCALL_WITH_LOG(SYS_gettimeofday, "%p, %p) ", 
                      (void*)a[1], (void*)a[2]);
      break;

    default:
      panic("Unhandled syscall ID = %d", a[0]);
  }
}


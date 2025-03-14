#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern SDL_Surface *screen;
char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
    char command[256]; 
    strcpy(command, cmd); 
    int len = strlen(command);
    command[len - 1] = 0; 
    char *token = strtok(command, " ");
    char *args[10]; 
    int i = 0;

    while (token != NULL && i < 9) {
        args[i++] = token; 
        token = strtok(NULL, " "); 
    }
    args[i] = NULL; 

    if (setenv("PATH", "/bin", 0) == -1) {
        perror("fail to set PATH");
        return;
    }

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "/bin/%s", args[0]);

    SDL_FillRect(screen, NULL, 0);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    if (execve(full_path, args, environ) == -1) {
        perror("execve error"); 
    }
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}

#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static uint8_t key_state[SDLK_PAGEDOWN + 1] = {0}; 

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
    char buf[64];  
    int len = NDL_PollEvent(buf, sizeof(buf));
    
    if (len > 0) {
        if (buf[0] == 'k') {
            ev->type = (buf[1] == 'u' ? SDL_KEYUP : SDL_KEYDOWN);
            
            char key[32];
            sscanf(buf + 3, "%s", key);
            
            for (int i = 0; i <= SDLK_PAGEDOWN; i++) {
                if (strcmp(key, keyname[i]) == 0) {
                    ev->key.keysym.sym = i;
                    key_state[i] = (buf[1] == 'u' ? 0 : 1);
                    return 1;  
                }
            }
        }
    }
    
    return 0; 
}

int SDL_WaitEvent(SDL_Event *event) {
    assert(event != NULL);
    
    char buf[64];  
    int len;

    while (1) {
        len = NDL_PollEvent(buf, sizeof(buf));
        if (len > 0) {
            if (buf[0] == 'k') {
                event->type = (buf[1] == 'u' ? SDL_KEYUP : SDL_KEYDOWN);
                
                char key[32];
                sscanf(buf + 3, "%s", key);
                
                for (int i = 0; i <= SDLK_PAGEDOWN; i++) {
                    if (strcmp(key, keyname[i]) == 0) {
                        event->key.keysym.sym = i;
                        // 更新按键状态
                        key_state[i] = (buf[1] == 'u' ? 0 : 1);
                        return 1;  
                    }
                }
            }
        }
    }

    return 0; 
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
    if (numkeys != NULL) {
        *numkeys = SDLK_PAGEDOWN + 1;
    }
    return key_state;
}

#ifdef _WIN32
#include <windows.h>
#include "SDL_syswm.h"
#endif

#include "k_log.h"

#include "./k_window.h"

struct k_window_context k__window;

int k_window_set_always_on_top(int enable) {

#ifndef _WIN32
    (void)enable;
    k_log_error("sorry, not supported on this platform");
    return -1;
#else
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    if (SDL_TRUE != SDL_GetWindowWMInfo(k__window.window, &wmInfo)) {
        k_log_error("SDL error: %s", SDL_GetError());
        return -1;
    }

    HWND hwnd = wmInfo.info.win.window;

    WINBOOL success;
    if (enable) {
        success = SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    } else {
        success = SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    if (success)
        return 0;
    else
        return -1;
#endif
}

#ifndef K__WEBUI_CONTEXT_H
#define K__WEBUI_CONTEXT_H

#include "webui.h"

#include "k_webui.h"

struct k_webui_context {

    size_t window;

    unsigned int is_inited;
};

extern struct k_webui_context k__webui;

int k__webui_is_running(void);

#endif

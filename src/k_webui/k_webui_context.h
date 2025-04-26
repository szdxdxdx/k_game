#ifndef K__WEBUI_CONTEXT_H
#define K__WEBUI_CONTEXT_H

#include "webui.h"

#include "k_webui.h"

struct k_webui_context {

    size_t window;
};

extern struct k_webui_context k__webui;

int k__webui_exec_js(const char *js);

int k__webui_exec_js_fmt(const char *js, ...);

#endif

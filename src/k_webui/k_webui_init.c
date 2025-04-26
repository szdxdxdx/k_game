#include "./k_webui_context.h"

struct k_webui_context k__webui;

int k_webui_init(void) {
    size_t window = webui_new_window();
    webui_show(window, "./webui/index.html");
}

void k_webui_fini(void) {

}

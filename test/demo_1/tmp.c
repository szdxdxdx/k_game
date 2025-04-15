#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if 0

struct k_ui_context *ui;

void click(struct k_ui_elem *elem, void *data) {

}

void ui_init(void) {

    ui = k_ui_create_context();

    struct k_ui_elem *box = k_ui_create_elem(ui, "box");
    struct k_ui_elem *text = k_ui_create_elem(ui, "text");
    k_ui_add_child(box, text);
    struct k_ui_elem *button = k_ui_create_elem(ui, "button");
    k_ui_add_child(box, button);
}

void ui_update(void) {
    k_ui_update(tree);
}

void ui_draw(void) {
    k_ui_draw(tree);
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    return 0;
}

#endif

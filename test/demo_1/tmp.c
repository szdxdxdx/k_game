#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if 0

static void test_1(void) {

    float f;

    k_webui_bind_float("label", &f, NULL);

    while (1) {
        printf("%f\n", f);

        if (f > 300.0f)
            break;
    }

    k_webui_unbind("label");
}

static void test_2(void) {

    int i;

    struct k_webui_bind_int_options opt;
    opt.description = "this is a description";
    opt.input = K_WEBUI_INPUT_SLIDER;
    opt.slider.min = 100;
    opt.slider.max = 350;
    opt.slider.step = 10;
    k_webui_bind_int("group:label", &i, opt);

    while (1) {
        printf("%d\n", i);

        if (i > 300)
            break;
    }

    k_webui_unbind("label");
}

static void test_3(void) {

    struct k_webui_bind_config config;
    config.ctx = ...;
    config.fn_set = ...;
    config.fn_get = ...;

    k_webui_bind(config);
}

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    test();

    return 0;
}

#endif


#include <stdarg.h>

#include "_internal.h"

static void print_json(struct k_printf_buf *buf, struct k_json *json) {

    switch (json->type) {
        case K_JSON_NULL:
            buf->fn_puts(buf, "null", 4);
            break;

        case K_JSON_BOOL:
            if (((struct k__json_bool *)json)->is_true)
                buf->fn_puts(buf, "true", 4);
            else
                buf->fn_puts(buf, "false", 5);
            break;

        case K_JSON_STR:
            buf->fn_printf(buf, "\"%s\"", ((struct k__json_str *)json)->str);
            break;

        case K_JSON_NUM:
            buf->fn_printf(buf, "%s", k__json_num_get_s((struct k__json_num *)json));
            break;

        case K_JSON_ARR: {
            struct k__json_arr *json_arr = (struct k__json_arr *)json;

            if (json_arr->size == 0) {
                buf->fn_puts(buf, "[]", 2);
                break;
            }

            buf->fn_puts(buf, "[", 1);
            print_json(buf, json_arr->arr[0]);

            if (json_arr->size == 1) {
                buf->fn_puts(buf, "]", 1);
                break;
            }

            size_t i = 1;
            for (; i < json_arr->size; i++) {
                buf->fn_puts(buf, ",", 1);
                print_json(buf, (struct k_json *)json_arr->arr[i]);
            }

            buf->fn_puts(buf, "]", 1);
            break;
        }

        case K_JSON_OBJ: {
            buf->fn_puts(buf, "{", 1);

            struct k__json_obj *json_obj = (struct k__json_obj *)json;

            int is_first_pair = 1;

            size_t i = 0;
            for (; i < json_obj->lists_num; i++) {
                struct k_hash_list_node *node;
                for (k_hash_list_for_each(&(json_obj->lists[i]), node)) {
                    struct kv_pair *pair = container_of(node, struct kv_pair, link);

                    if (is_first_pair)
                        is_first_pair = 0;
                    else
                        buf->fn_puts(buf, ",", 1);

                    buf->fn_printf(buf, "\"%s\":", pair->key);
                    print_json(buf, pair->val);
                }
            }

            buf->fn_puts(buf, "}", 1);
            break;
        }
    }
}

void k_printf_callback_k_json(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {
    struct k_json *json = va_arg(*args, void *);
    if (NULL != json)
        print_json(buf, json);
    else
        buf->fn_puts(buf, "(null)", 6);
}

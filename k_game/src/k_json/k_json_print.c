#include <stdarg.h>

#include "./_internal.h"

static void print_json(struct k_printf_buf *buf, struct k_json *json) {

    switch (json->type) {
        case K__JSON_NULL:
            k_printf_buf_puts_n(buf, "null", 4);
            break;

        case K__JSON_BOOL:
            if (((struct k_json_bool *)json)->is_true)
                k_printf_buf_puts_n(buf, "true", 4);
            else
                k_printf_buf_puts_n(buf, "false", 5);
            break;

        case K__JSON_STR:
            k_printf_buf_printf(buf, "\"%s\"", ((struct k_json_str *)json)->str);
            break;

        case K__JSON_NUM:
            k_printf_buf_printf(buf, "%s", k__json_num_get_s((struct k_json_num *)json));
            break;

        case K__JSON_ARR: {
            struct k_json_arr *json_arr = (struct k_json_arr *)json;

            if (json_arr->arr_.size == 0) {
                k_printf_buf_puts_n(buf, "[]", 2);
                break;
            }

            k_printf_buf_puts_n(buf, "[", 1);
            print_json(buf, json_arr->arr_.storage[0]);

            if (json_arr->arr_.size == 1) {
                k_printf_buf_puts_n(buf, "]", 1);
                break;
            }

            size_t i = 1;
            for (; i < json_arr->arr_.size; i++) {
                k_printf_buf_puts_n(buf, ",", 1);
                print_json(buf, (struct k_json *)json_arr->arr_.storage[i]);
            }

            k_printf_buf_puts_n(buf, "]", 1);
            break;
        }

        case K__JSON_OBJ: {
            k_printf_buf_puts_n(buf, "{", 1);

            struct k_json_obj *json_obj = (struct k_json_obj *)json;

            int is_first_pair = 1;

            size_t i = 0;
            for (; i < json_obj->lists_num; i++) {
                struct k_hash_list_node *node;
                for (k_hash_list_for_each(&(json_obj->lists[i]), node)) {
                    struct k_json_obj_pair *pair = container_of(node, struct k_json_obj_pair, link);

                    if (is_first_pair)
                        is_first_pair = 0;
                    else
                        k_printf_buf_puts_n(buf, ",", 1);

                    k_printf_buf_printf(buf, "\"%s\":", pair->key);
                    print_json(buf, pair->val);
                }
            }

            k_printf_buf_puts_n(buf, "}", 1);
            break;
        }
    }
}

void k__json_print(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args) {
    struct k_json *json = va_arg(*args, void *);
    if (NULL != json)
        print_json(buf, json);
    else
        k_printf_buf_puts_n(buf, "(null)", 6);
}

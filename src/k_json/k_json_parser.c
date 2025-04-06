#include <stdarg.h>

#include "./_internal.h"

enum token_type {
    TOKEN_NULL  , TOKEN_STR   ,
    TOKEN_TRUE  , TOKEN_FALSE ,
    TOKEN_INT   , TOKEN_FLOAT ,
    TOKEN_ARR_L , TOKEN_ARR_R ,
    TOKEN_OBJ_L , TOKEN_OBJ_R ,
    TOKEN_COLON , TOKEN_COMMA ,
    TOKEN_ERR   , TOKEN_EOF   ,

    TOKEN_FMT_D,
    TOKEN_FMT_F,
    TOKEN_FMT_B,
    TOKEN_FMT_S,
    TOKEN_FMT_J,
};

struct json_parser {
    const char *curr;
    va_list     args;
    const char *token_start;
    size_t      token_len;
    size_t      token_type;
};

static struct k_json *parse_token(struct json_parser *parser);

static void init_parser(struct json_parser *parser, const char *str, va_list args) {
    parser->curr = str;
    parser->args = args;
    parser->token_type  = TOKEN_EOF;
    parser->token_start = parser->curr;
    parser->token_len   = 0;
}

static void extract_token(struct json_parser *parser) {

    const char *p = parser->curr;

    for (;;) {
        if (*p == ' ' || *p == '\n' || *p == '\r' || *p == '\t') {
            ++p;
            continue;
        }
        else if (' ' < *p) {
            break;
        }
        else if ('\0' == *p) {
            parser->token_type = TOKEN_EOF;
            return;
        }
        else
            goto err;
    }

    const char *s = p;
    switch (*p) {

        case '%': {
            switch (*++p) {
                case 'd': parser->curr = ++p; parser->token_type = TOKEN_FMT_D; return;
                case 'f': parser->curr = ++p; parser->token_type = TOKEN_FMT_F; return;
                case 'b': parser->curr = ++p; parser->token_type = TOKEN_FMT_B; return;
                case 's': parser->curr = ++p; parser->token_type = TOKEN_FMT_S; return;
                case 'j': parser->curr = ++p; parser->token_type = TOKEN_FMT_J; return;
                default: goto err;
            }
        }

        case '[': parser->curr = ++p; parser->token_type = TOKEN_ARR_L; return;
        case ']': parser->curr = ++p; parser->token_type = TOKEN_ARR_R; return;
        case '{': parser->curr = ++p; parser->token_type = TOKEN_OBJ_L; return;
        case '}': parser->curr = ++p; parser->token_type = TOKEN_OBJ_R; return;
        case ':': parser->curr = ++p; parser->token_type = TOKEN_COLON; return;
        case ',': parser->curr = ++p; parser->token_type = TOKEN_COMMA; return;

        case 'n': {
            if ('u' == *++p && 'l' == *++p && 'l' == *++p) {
                parser->curr = ++p;
                parser->token_type = TOKEN_NULL;
                return;
            }
            else {
                goto err;
            }
        }

        case 't': {
            if ('r' == *++p && 'u' == *++p && 'e' == *++p) {
                parser->curr = ++p;
                parser->token_type = TOKEN_TRUE;
                return;
            }
            else {
                goto err;
            }
        }

        case 'f': {
            if ('a' == *++p && 'l' == *++p && 's' == *++p && 'e' == *++p) {
                parser->curr = ++p;
                parser->token_type = TOKEN_FALSE;
                return;
            }
            else {
                goto err;
            }
        }

        case '-':
            ++p;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {

            const char *r = NULL;
            enum token_type token_type = TOKEN_INT;

            if ('1' <= *p && *p <= '9')
                do r = ++p; while ('0' <= *p && *p <= '9');
            else if (*p == '0')
                r = ++p;
            else
                goto err;

            if (*p == '.') {
                ++p;
                if ('0' <= *p && *p <= '9')
                    token_type = TOKEN_FLOAT;
                else
                    goto end_num;
                do r = ++p; while ('0' <= *p && *p <= '9');
            }

            if (*p == 'e' || *p == 'E') {
                ++p;
                if (*p == '-' || *p == '+')
                    ++p;
                if ('0' <= *p && *p <= '9') {
                    token_type = TOKEN_FLOAT;
                    do r = ++p; while ('0' <= *p && *p <= '9');
                }
            }

        end_num:
            parser->curr        = r;
            parser->token_type  = token_type;
            parser->token_start = s;
            parser->token_len   = r - s;
            return;
        }

        case '"': {
            while ('"' != *++p) {
                if (*p < ' ')
                    goto err;
                else if ('\\' != *p)
                    continue;
                else switch (*++p) {
                    case '/' : case 'b' : case 'f': case '\\':
                    case 'n' : case 'r' : case 't': case '\"':
                        continue;
                    case 'u': {
                        int i = 0;
                        for (; i < 4; ++i) {
                            ++p;
                            if (! (('0' <= *p && *p <= '9')
                                || ('a' <= *p && *p <= 'f')
                                || ('A' <= *p && *p <= 'F')))
                                goto err;
                        }
                    }
                    break;
                    default: goto err;
                }
            }

            ++p;
            parser->curr = p;
            parser->token_type  = TOKEN_STR;
            parser->token_start = s;
            parser->token_len   = p - s;
            return;
        }

        default: goto err;
    }

err:
    if ('\0' != *p)
        ++p;

    parser->curr = p;
    parser->token_type = TOKEN_ERR;
}

static struct k_json *parse_array(struct json_parser *parser) {
    struct k__json_arr *arr = k__create_json_arr();
    struct k_json *item = NULL;

    if (NULL == arr)
        goto arr_err;

    extract_token(parser);
    if (parser->token_type == TOKEN_ARR_R)
        goto arr_done;

    for (;;) {
        if (NULL == (item = parse_token(parser)))
            goto arr_err;

        if (0 != k__json_arr_push(arr, item))
            goto arr_err;
        item = NULL;

        extract_token(parser);
        if (parser->token_type != TOKEN_COMMA)
            break;

        extract_token(parser);
    }

    if (parser->token_type != TOKEN_ARR_R)
        goto arr_err;

arr_done:
    return (struct k_json *)arr;

arr_err:
    k_json_free((struct k_json *)arr);
    k_json_free(item);
    return NULL;
}

static struct k_json *parse_object(struct json_parser *parser) {
    struct k__json_obj *obj = k__create_json_obj();
    struct k_json *val = NULL;

    if (NULL == obj)
        goto obj_done;

    extract_token(parser);
    if (parser->token_type == TOKEN_OBJ_R)
        goto obj_done;

    for (;;) {
        if (parser->token_type != TOKEN_STR)
            goto obj_err;
        const char *key = parser->token_start + 1;
        size_t key_len  = parser->token_len - 2;

        extract_token(parser);
        if (parser->token_type != TOKEN_COLON)
            goto obj_err;

        extract_token(parser);
        if (NULL == (val = parse_token(parser)))
            goto obj_err;

        if (0 != k__json_obj_add(obj, key, key_len, val))
            goto obj_err;
        val = NULL;

        extract_token(parser);
        if (parser->token_type != TOKEN_COMMA)
            break;

        extract_token(parser);
    }

    if (parser->token_type != TOKEN_OBJ_R)
        goto obj_err;

obj_done:
    return (struct k_json *)obj;

obj_err:
    k_json_free((struct k_json *)obj);
    k_json_free(val);
    return NULL;
}

static struct k_json *parse_token(struct json_parser *parser) {

    switch (parser->token_type) {
        case TOKEN_NULL  : return (struct k_json *)k__create_json_null();
        case TOKEN_TRUE  : return (struct k_json *)k__create_json_bool(1);
        case TOKEN_FALSE : return (struct k_json *)k__create_json_bool(0);
        case TOKEN_STR   : return (struct k_json *)k__create_json_str(parser->token_start + 1, parser->token_len - 2);
        case TOKEN_INT   : return (struct k_json *)k__create_json_num_i_s(parser->token_start, parser->token_len);
        case TOKEN_FLOAT : return (struct k_json *)k__create_json_num_f_s(parser->token_start, parser->token_len);
        case TOKEN_ARR_L : return parse_array(parser);
        case TOKEN_OBJ_L : return parse_object(parser);

        case TOKEN_FMT_D: {
            int num = va_arg(parser->args, int);
            return (struct k_json *)k__create_json_num_i(num);
        }

        case TOKEN_FMT_F: {
            double num = va_arg(parser->args, double);
            return (struct k_json *)k__create_json_num_f(num);
        }

        case TOKEN_FMT_B: {
            int num = va_arg(parser->args, int);
            return (struct k_json *)k__create_json_bool(num);
        }

        case TOKEN_FMT_S: {
            const char *str = va_arg(parser->args, const char *);
            return (struct k_json *)k__create_json_str(str, strlen(str));
        }

        case TOKEN_FMT_J: {
            struct k_json *json = va_arg(parser->args, void *);
            if (NULL != json)
                return json;
            else
                return (struct k_json *)k__create_json_null();
        }

        default:
            return NULL;
    }
}

struct k_json *k__json_parse(const char *fmt, va_list args) {

    struct json_parser parser;
    init_parser(&parser, fmt, args);

    extract_token(&parser);
    struct k_json *json = parse_token(&parser);

    extract_token(&parser);
    if (parser.token_type != TOKEN_EOF) {
        /* TODO 预期为文件结尾 */
    }

    return json;
}

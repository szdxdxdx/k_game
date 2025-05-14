#include <stdarg.h>

#include "./_internal.h"

/* region [extract_token] */

static char *extract_number(const char *str) {

    enum state {
    // --------------------------------------------
    // 状态集
    // --------------------------------------------
        S0,             // start    开始
        I0, I1, I2, I3, // integer  识别整数部分
        F0, F1,         // fraction 识别小数部分
        E0, E1, E2,     // exponent 识别指数部分
        ED,             // end      结束，退出状态机
    // --------------------------------------------
    // 初态：S0        终态：I1, I2, I3, F1, E2
    };

    enum symbol {
    // --------------------------------
    // 输入符号集
    // --------------------------------
        PLUS,   // +   | 正号
        MINUS,  // -   | 负号
        POINT,  // .   | 小数点
        ZERO,   // 0   | 数字 0
        DIGIT,  // 1~9 | 数字 1~9
        EXP,    // e E | 指数符号 e 或 E
        OTHERS, //     | 其他符号
    // --------------------------------
        SYMBOL_SET_SIZE
    };

    static enum state transition_table[]  = {
    // ------------------------------------
    // 状态转换表
    // ------------------------------------
    //        +   -   .   0  1~9 eE  others
    /* S0 */ ED, I0, ED, I3, I1, ED, ED,
    /* I0 */ ED, ED, ED, I3, I1, ED, ED,
    /* I1 */ ED, ED, F0, I2, I2, E0, ED,
    /* I2 */ ED, ED, F0, I2, I2, E0, ED,
    /* I3 */ ED, ED, F0, ED, ED, E0, ED,
    /* F0 */ ED, ED, ED, F1, F1, ED, ED,
    /* F1 */ ED, ED, ED, F1, F1, E0, ED,
    /* E0 */ E1, E1, ED, E2, E2, ED, ED,
    /* E1 */ ED, ED, ED, E2, E2, ED, ED,
    /* E2 */ ED, ED, ED, E2, E2, ED, ED,
    // ------------------------------------
    };

    const char *p = str;
    const char *r = NULL;

    /* DFA 开始 */
    enum state state = S0;
    for (;;) {
        /* 读入字符，判断字符属于符号集中的哪个符号 */
        enum symbol input;
        switch (*p++) {
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8':
            case '9': input = DIGIT;  break;
            case '0': input = ZERO;   break;
            case '+': input = PLUS;   break;
            case '-': input = MINUS;  break;
            case '.': input = POINT;  break;
            case 'e':
            case 'E': input = EXP;    break;
            default:  input = OTHERS; break;
        }

        /* 状态转移 */
        state = transition_table[SYMBOL_SET_SIZE * state + input];

        /* 每到一次终态则记录一次位置 */
        switch (state) {
            case I1:
            case I2:
            case I3:
            case F1:
            case E2: r = p;
            default:;
        }

        /* 直到新读入的字符与已扫描的部分不再构成合法的数字 */
        if (ED == state)
            break;
    }

    return (char *)r;
}

static char *extract_string(const char *str, const char **err_pos) {

    enum state {
    // ---------------------------------------------
    // 状态集
    // ---------------------------------------------
        S0,             // start     开始
        CH,             // character 读取普通字符
        ES,             // escape    读取转义字符
        U0, U1, U2, U3, // unicode   读取十六进制转义
        ED,             // end       结束，退出状态机
        ER,             // error     错误，退出状态机
    // ---------------------------------------------
    // 初态：S0        终态：ED
    };

    enum symbol {
    // ----------------------------------------
    // 输入符号集
    // ----------------------------------------
        QUOTE,  // "   | 引号
        BACKS,  // \   | 反斜杠
        ESCAPE, // esc | 部分转义字符 n r t /
        LOW_U,  // u   | 小写字母 u
        LOW_BF, // b f | 小写字母 b 或 f
        HEX,    // hex | 十六进制数，除去 b 和 f
        OTHERS, //     | 其他符号
    // ----------------------------------------
        SYMBOL_SET_SIZE
    };

    static enum state transition_table[]  = {
    // ----------------------------------------
    // 状态转换表
    // ----------------------------------------
    //        "   \  esc  u  bf  hex others
    /* S0 */ CH, ER, ER, ER, ER, ER, ER,
    /* CH */ ED, ES, CH, CH, CH, CH, CH,
    /* ES */ CH, CH, CH, U0, CH, ER, ER,
    /* U0 */ ER, ER, ER, ER, U1, U1, ER,
    /* U1 */ ER, ER, ER, ER, U2, U2, ER,
    /* U2 */ ER, ER, ER, ER, U3, U3, ER,
    /* U3 */ ER, ER, ER, ER, CH, CH, ER,
    // ----------------------------------------
    };

    const char *p = str;

    /* DFA 开始 */
    enum state state = S0;
    for (;;) {
        /* 读入字符，如果字符编码不在合法的 unicode 范围内则结束 */
        char ch = *p++;
        if (ch < 0x0020) /* <- 包括 '\0' */ /* TODO: ch > 0x10FFFF */
            goto err;

        /* 判断属于符号集中的哪个符号 */
        enum symbol input;
        switch (ch) {
            case 'b':
            case 'f': input = LOW_BF; break;
            case 'n':
            case 'r':
            case 't':
            case '/': input = ESCAPE; break;
            case 'u': input = LOW_U;  break;
            case '"': input = QUOTE;  break;
            case '\\':input = BACKS;  break;
            default: {
                if (('0' <= ch && ch <= '9')
                 || ('a' <= ch && ch <= 'f')
                 || ('A' <= ch && ch <= 'F'))
                    input = HEX;
                else
                    input = OTHERS; break;
            }
        }

        /* 状态转移 */
        state = transition_table[SYMBOL_SET_SIZE * state + input];

        /* 直到识别出完整的字符串 */
        if (ED == state)
            break;

        /* 或是发现字符串格式错误 */
        if (ER == state)
            goto err;
    }

    return (char *)p;
err:
    *err_pos = --p;
    return NULL;
}

/* endregion */

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
    struct k_json_arr *arr = k__json_arr_create();
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
    struct k_json_obj *obj = k__json_obj_create();
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
        case TOKEN_NULL  : return (struct k_json *)k__json_null_create();
        case TOKEN_TRUE  : return (struct k_json *)k__json_bool_create(1);
        case TOKEN_FALSE : return (struct k_json *)k__json_bool_create(0);
        case TOKEN_STR   : return (struct k_json *)k__json_create_str(parser->token_start + 1, parser->token_len - 2);
        case TOKEN_INT   : return (struct k_json *)k__json_num_create_i_s(parser->token_start, parser->token_len);
        case TOKEN_FLOAT : return (struct k_json *)k__json_num_create_f_s(parser->token_start, parser->token_len);
        case TOKEN_ARR_L : return parse_array(parser);
        case TOKEN_OBJ_L : return parse_object(parser);

        case TOKEN_FMT_D: {
            int num = va_arg(parser->args, int);
            return (struct k_json *)k__json_num_create_i(num);
        }

        case TOKEN_FMT_F: {
            double num = va_arg(parser->args, double);
            return (struct k_json *)k__json_num_create_f(num);
        }

        case TOKEN_FMT_B: {
            int num = va_arg(parser->args, int);
            return (struct k_json *)k__json_bool_create(num);
        }

        case TOKEN_FMT_S: {
            const char *str = va_arg(parser->args, const char *);
            return (struct k_json *)k__json_create_str(str, strlen(str));
        }

        case TOKEN_FMT_J: {
            struct k_json *json = va_arg(parser->args, void *);
            if (NULL != json)
                return json;
            else
                return (struct k_json *)k__json_null_create();
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

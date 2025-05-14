#ifndef K_PRINTF_H
#define K_PRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

/* region [k_printf_callback] */

struct k_printf_buf;
struct k_printf_spec;

/**
 * \brief 用于打印自定义格式说明符的内容
 *
 * 当 k_printf 识别到自定义的格式说明符时，会执行该回调，交由你来输出内容。
 *
 * 在回调中，k_printf 提供给你一个抽象的缓冲区接口 `k_printf_buf`，
 * 你需要使用 `k_printf_buf_XXX()` 系列的函数往缓冲区中写入内容，
 * 不用考虑缓冲区类型是 `char []` 还是 `FILE *`。
 *
 * C printf 支持使用 `+-#0 *.*` 修饰格式说明符的转换行为，k_printf 也支持。
 * 在回调中，k_printf 会通过 `spec` 告诉你所识别到的格式说明符的详细信息。
 *
 * 对于自定义的格式说明符，你需要自行实现这些修饰符号对应的功能。你可以重载修饰符的含义，
 * 例如，可以认为 `+` 表示详细输出，`-` 表示简要输出，这取决于你自己的实现。
 * 若你的格式说明符不需要支持这些修饰功能，那你多半可以忽略 `spec`。
 *
 * 在回调中，k_printf 传入不定长参数列表的指针 `args`。
 * 传递的是指针，是因为 k_printf 需要知道执行完你在回调后消耗了多少实参。
 * 你应通过 `va_arg(*args, type)` 从列表中获取一个 type 类型的实参。
 *
 * 留意 C 在传递不定长参数时存在类型提升。例如 `char` 被提升为 `int`，
 * 若要读取一个 `char` 类型实参，应使用 `(char)(va_arg(*args, int))`。
 *
 * 你应按需消耗实参。不要残留下应该由你来处理的实参，不要多消耗不属于你的实参，
 * 否则后续的格式说明符会匹配到错误的实参，可能导致程序崩溃。
 */
typedef void (*k_printf_spec_print_fn)(struct k_printf_buf *buf, const struct k_printf_spec *spec, va_list *args);

/**
 * \brief 用于匹配自定义格式说明符
 *
 * 当 k_printf 遇到 `%` 符号时，会先提取格式说明符的修饰部分，再交由你来匹配格式说明符。
 * 若匹配到了你自定义的格式说明符，你需要移动字符串指针，并返回用于打印该格式说明符的回调。
 *
 * 例如：若你定义了格式说明符 `%k`，函数 `fn_k()` 分别用于打印它的内容，
 * 则你需要在本回调中匹配字符串的开头是否为 `k`，若不是，则直接返回 `NULL`。
 * 若是，则函数应返回函数指针 `fn_k`，并移动字符串指针 +1 越过该格式说明符。
 *
 * C printf 的格式说明符格式为 `%[标志][最小宽度][.精度][长度修饰符]转换说明符`。
 * k_printf 把 `[长度修饰符]转换说明符` 整体称作格式说明符的 `类型`，
 * 例如，在 k_printf 看来，C printf 的 `%d` 和 `%lld` 是两个不同的类型。
 * 若你只自定义了格式说明符为 `%k`，对 k_printf 而言 `%llk` 仍是未知的格式说明符。
 *
 * 在本回调中，你只需要匹配格式说明符的类型名。例如，当 k_printf 遇到 `%+.3kiss` 时，
 * k_printf 会识别 `%+.3` 的部分，交由你来匹配类型名 `k` 并将字符串指针移到 `iss`。
 *
 * 类型名可以使用除了字母和数字以外的字符，但开头不能是数字、空格或 `%+-#*` 中的任一个字符。
 * 你可以重载 C printf 的格式说明符，但会失去左右对齐、零填充、精度等修饰符的默认行为，
 * 若需要，你得在 `k_printf_spec_print_fn` 中再实现它们。
 *
 * 你可以使用 `k_printf_match_spec_helper()` 帮助你完成字符串匹配工作。
 */
typedef k_printf_spec_print_fn (*k_printf_spec_match_fn)(const char **str);

/* endregion */

/**
 * `k_printf()` 将字符串格式化写入到标准输出流 `stdout`。
 *
 * `fn_match` 用于匹配自定义格式说明符的回调，若为 `NULL` 则仅匹配 C printf 的格式说明符。
 *
 * `k_fprintf()` 和 `k_vfprintf()` 将字符串格式化写入到文件流 `file`。
 *
 * `k_sprintf()` 和 `k_vsprintf()` 将字符串格式化写入到缓冲区 `buf`，并以 `\0` 结尾。
 * `k_snprintf()` 和 `k_vsnprintf()` 是更安全的版本，因为有 `n` 来指定缓冲区的大小，
 * 若缓冲区不足以容纳完整的结果字符串，则在缓冲区末尾处用 `\0` 将字符串截断。
 *
 * `k_asprintf()` 和 `k_vasprintf()` 使用 `malloc()` 分配缓冲区存储结果字符串，
 * 出参 `get_s` 返回该字符串指针。该字符串的内存由用户负责使用 `free()` 将其释放。
 *
 * 所有函数，若成功，返回格式化字符串的长度（忽略缓冲区实际大小），失败则返回负值。
 *
 * @{
 */

int k_printf   (k_printf_spec_match_fn fn_match, const char *fmt, ...);
int k_fprintf  (k_printf_spec_match_fn fn_match, FILE *file, const char *fmt, ...);
int k_vfprintf (k_printf_spec_match_fn fn_match, FILE *file, const char *fmt, va_list args);
int k_sprintf  (k_printf_spec_match_fn fn_match, char *buf, const char *fmt, ...);
int k_vsprintf (k_printf_spec_match_fn fn_match, char *buf, const char *fmt, va_list args);
int k_snprintf (k_printf_spec_match_fn fn_match, char *buf, size_t n, const char *fmt, ...);
int k_vsnprintf(k_printf_spec_match_fn fn_match, char *buf, size_t n, const char *fmt, va_list args);
int k_asprintf (k_printf_spec_match_fn fn_match, char **get_s, const char *fmt, ...);
int k_vasprintf(k_printf_spec_match_fn fn_match, char **get_s, const char *fmt, va_list args);

/** @} */

/* region [k_printf_buf] */

/** \brief 缓冲区接口，统一操作 `char []` 和 `FILE *` 两类缓冲区 */
struct k_printf_buf;

/** \brief 往缓冲区中追加写入字符串 */
void k_printf_buf_puts(struct k_printf_buf *buf, const char *str);

/** \brief 往缓冲区中追加写入指定长度的字符串 */
void k_printf_buf_puts_n(struct k_printf_buf *buf, const char *str, size_t len);

/** \brief 格式化字符串并追加写入缓冲区，格式说明符同 C printf */
void k_printf_buf_printf(struct k_printf_buf *buf, const char *fmt, ...);

/** \brief 格式化字符串并追加写入缓冲区，格式说明符同 C printf */
void k_printf_buf_vprintf(struct k_printf_buf *buf, const char *fmt, va_list args);

/**
 * \brief 获取截至目前应打印出的字符的数量（忽略缓冲区的实际大小）
 *
 * 函数将忽略缓冲区的实际大小，返回到目前为止应该要打印出的字符的数量。
 * 若输出途中出现错误，则函数返回负值。
 *
 * 你可以用本函数实现与 C printf 的 `%n` 类似的功能。
 */
int k_printf_buf_get_n(struct k_printf_buf *buf);

/* endregion */

/* region [k_printf_spec] */

/** \brief 格式说明符 */
struct k_printf_spec {

    /** \brief `-` 左对齐 */
    unsigned int left_justified : 1;

    /** \brief `+` 始终显示符号 */
    unsigned int sign_prepended : 1;

    /** \brief ` ` 空格填充 */
    unsigned int space_padded : 1;

    /** \brief `0` 零填充 */
    unsigned int zero_padding : 1;

    /** \brief `#` 特殊格式修饰 */
    unsigned int alternative_form : 1;

    /** \brief `*` 使用最小宽度 */
    unsigned int use_min_width : 1;

    /** \brief `.*` 使用精度 */
    unsigned int use_precision : 1;

    /**
     * \brief 最小宽度
     *
     * 只有 `use_min_width` 为非 0 时，`min_width` 才有意义。
     *
     * 假定 `%k` 是你自定义的格式说明符，则：
     *
     * 若是静态指定的最小宽度，则 `min_width` 保存着该宽度值，一定为正，最大为 `INT_MAX`。
     * 例如：
     * - 遇到 `%2k` 时，`min_width` 的值为 2。
     * - 遇到 `%02k` 时，`0` 被识别成零填充修饰符，`min_width` 的值为 2。
     * - 遇到 `%-2k` 时，`-` 被识别成左对齐修饰符，`min_width` 的值为 2。
     * - 遇到 `%999999999999k` 时，`min_width` 的值为 `INT_MAX`。
     *
     * 若是动态指定的最小宽度，则 `min_width` 为 -1。
     * 例如，遇到 `%*k` 时，`use_min_width` 为非 0，`min_width` 为 -1，
     * 此时你应使用 `va_arg(*args, int)` 获取动态指定的最小宽度。
     */
    int min_width;

    /**
     * \brief 精度
     *
     * 只有 `use_precision` 为非 0 时，`precision` 才有意义。
     *
     * 假定 `%k` 是你自定义的格式说明符，则：
     *
     * 若是静态指定的精度，则 `precision` 保存着该精度值，一定为正，最大为 `INT_MAX`。
     * 例如：
     * - 遇到 `%.2k` 时，`precision` 的值为 2。
     * - 遇到 `%.k` 时，`precision` 的值为 0。
     * - 遇到 `%.999999999999k` 时，`precision` 的值为 `INT_MAX`。
     * - 遇到 `%.-2k` 时，无法正确识别出格式说明符`%k`。
     *
     * 若是动态指定的精度，则 `precision` 为 -1。
     * 例如，遇到 `%.*k` 时，`use_precision` 为非 0，`precision` 为 -1，
     * 此时你应使用 `va_arg(*args, int)` 获取动态指定的精度。
     */
    int precision;

    /** \brief 指向原格式字符串中该格式说明符 `%` 符号的位置 */
    const char *start;

    /** \brief 指向原格式字符串中该格式说明符的类型部分的起始位置 */
    const char *type;

    /** \brief 指向原格式字符串中该格式说明符的结束位置 */
    const char *end;
};

/* endregion */

/* region [k_printf_match_spec_helper] */

/** \brief 用于定义一对格式说明符与回调，仅用于 `k_printf_match_spec_helper` */
struct k_printf_spec_print_pair {

    /** \brief 格式说明符类型 */
    const char *spec_type;

    /** \brief 对应的回调函数 */
    k_printf_spec_print_fn fn_print;
};

/**
 * \brief 匹配格式说明符，若匹配成功则移动字符串指针，并返回对应的回调
 *
 * 本函数可以帮助你完成 `k_printf_spec_match_fn` 的实现。
 *
 * 函数顺序遍历一组格式说明符配置项，将每个说明符与字符串开头进行匹配。
 * 若匹配成功，则移动字符串指针跳过该说明符，函数返回相应的回调；
 * 若匹配失败，函数返回 `NULL`，且不移动字符串指针。
 *
 * 传递的 `pairs` 应是一个 `k_printf_spec_print_pair` 数组，
 * 且要求数组最后一项是哨兵值 `{ NULL, NULL }`。建议将该数组定义为静态常量。
 *
 * 若你的格式说明符有前缀包含关系，请把长的格式说明符写在前面。
 * 假定 `%k` 和 `%kk` 都是你自定义的格式说明符，请把 `%kk` 放在 `%k` 前面。
 */
k_printf_spec_print_fn k_printf_spec_match_helper(const struct k_printf_spec_print_pair *pairs, const char **str);

/* endregion */

#endif

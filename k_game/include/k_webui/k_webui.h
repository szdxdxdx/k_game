#ifndef K_WEBUI_H
#define K_WEBUI_H

#include <stddef.h>

#include "k_str_buf.h"

/* region [init] */

/** \brief 初始化 webui */
int k_webui_init(void);

/** \brief 关闭 webui */
void k_webui_close(void);

int k_webui_is_shown(void);

/* endregion */

/* region [log] */

/**
 * \brief 往 webui 的日志面板中输出日志
 *
 * `fmt` 是格式字符串，格式化说明符同 C printf。
 */
void k_webui_log_info(const char *fmt, ...);
void k_webui_log_warn(const char *fmt, ...);
void k_webui_log_error(const char *fmt, ...);

/* endregion */

/* region [bind] */

/* region [text] */

/** \brief 文本块控件 */
struct k_webui_text_config {

    /**
     * \brief webui 定时同步触发的回调
     *
     * webui 定期执行此回调，你需要通过 `buf` 告知 webui 要显示文本内容。
     * 若函数返回 0，则 webui 会将 `buf` 的内容同步到文本块上。
     * 若函数返回非 0，则 webui 不更新文本。
     *
     * 必须指定此回调。
     */
    int (*on_read)(void *data, struct k_str_buf *buf);
};

#define K_WEBUI_TEXT_CONFIG_INIT \
{ \
    .on_read = NULL, \
}

/**
 * \brief 绑定一个文本块控件
 *
 * 在 webui 中创建一个文本块控件，`config` 为控件的配置，`data` 为该控件要绑定到数据。
 * `group` 指定的控件的分组，`label` 指定控件前显示的文字标签。
 *
 * 若绑定成功，函数唯一的绑定的 id，否则返回 `SIZE_MAX`。
 */
size_t k_webui_bind_text(const char *group, const char *label, void *data, const struct k_webui_text_config *config);

/* endregion */

/* region [slider] */

enum k_webui_slider_type {
    K_WEBUI_INT_SLIDER,
    K_WEBUI_FLOAT_SLIDER,
};

/** \brief 滑动条控件 */
struct k_webui_int_slider_config {

    /** \brief 滑动条的最小值、最大值和步长 */
    int min;
    int max;
    int step;

    /**
     * \brief 在 webui 中拖动滑动条时触发的回调
     *
     * `val` 是 webui 中滑动条的新值，你可以将该值写入内存，或用于更新状态等等，
     * 若业务逻辑执行顺利，则函数应返回 0 表示接受该值，否则返回非 0 表示拒绝该值。
     *
     * 若 `on_input` 为 `NULL`，则默认将 `data` 视作指向 int 变量的指针，
     * 并将 `val` 的值写入 `data` 指向的内存。
     */
    int (*on_input)(void *data, int val);

    /**
     * \brief webui 定时同步触发的回调
     *
     * webui 定期执行此回调，你需要通过出参 `result` 告知 webui 当前绑定变量的值。
     * 若函数返回 0，则 webui 会将出参 `result` 的值同步到滑动条控件上。
     * 若函数返回非 0，则 webui 不更新控件。
     *
     * 若 `on_read` 为 `NULL`，则默认将 `data` 视作指向 int 变量的指针，
     * 读取 `data` 指向的内存，并同步滑动条控件的值。
     */
    int (*on_read)(void *data, int *result);
};

/** \brief 用于初始化滑动条控件配置 */
#define K_WEBUI_INT_SLIDER_CONFIG_INIT \
{ \
    .min      = 0,    \
    .max      = 100,  \
    .step     = 1,    \
    .on_input = NULL, \
    .on_read  = NULL, \
}

/**
 * \brief 绑定一个滑动条控件
 *
 * 在 webui 中创建一个滑动条控件，`config` 为控件的配置，`data` 为该控件要绑定到数据。
 * `group` 指定的控件的分组，`label` 指定控件前显示的文字标签。
 *
 * 若绑定成功，函数唯一的绑定的 id，否则返回 `SIZE_MAX`。
 */
size_t k_webui_bind_int_slider(const char *group, const char *label, void *data, const struct k_webui_int_slider_config *config);

/* endregion */

/* region [float_slider] */

/** \brief 滑动条控件 */
struct k_webui_float_slider_config {

    /* 与 `k_webui_int_slider_config` 类似，但绑定的数据是 float 类型 */

    float min;
    float max;
    float step;
    int (*on_input)(void *data, float val);
    int (*on_read)(void *data, float *result);
};

/** \brief 用于初始化滑动条控件配置 */
#define K_WEBUI_FLOAT_SLIDER_CONFIG_INIT \
{ \
    .min      = 0.0f,   \
    .max      = 100.0f, \
    .step     = 0.01f,  \
    .on_input = NULL,   \
    .on_read  = NULL,   \
}

/** \brief 绑定一个滑动条控件 */
size_t k_webui_bind_float_slider(const char *group, const char *label, void *data, const struct k_webui_float_slider_config *config);

/* endregion */

/* region [checkbox] */

/** \brief 复选框控件 */
struct k_webui_checkbox_config {

    /**
     * \brief 在 webui 中点击复选框时触发的回调
     *
     * 若勾选了复选框，则 `checked` 为 1，否则为 0。你可以将该值写入内存，或用于更新状态等等，
     * 若业务逻辑执行顺利，则函数应返回 0 表示接受该值，否则返回非 0 表示拒绝该值。
     *
     * 若 `on_change` 为 `NULL`，则默认将 `data` 视作指向 int 变量的指针，
     * 并将 `val` 的值写入 `data` 指向的内存。
     */
    int (*on_change)(void *data, int checked);

    /**
     * \brief webui 定时同步触发的回调
     *
     * webui 定期执行此回调，你需要通过出参 `result` 告知 webui 当前绑定变量的值。
     * 若函数返回 0，则 webui 会将出参 `result` 的值同步到复选框控件上。
     * 若 `result` 返回非 0，则勾选复选框，否则取消勾选。
     * 若函数返回非 0，则 webui 不更新控件。
     *
     * 若 `on_read` 为 `NULL`，则默认将 `data` 视作指向 int 变量的指针，
     * 读取 `data` 指向的内存，若为非 0 则勾选复选框，否则不勾选。
     */
    int (*on_read)(void *data, int *result);
};

/** \brief 用于初始化复选框控件配置 */
#define K_WEBUI_CHECKBOX_CONFIG_INIT \
{ \
    .on_change = NULL, \
    .on_read   = NULL, \
}

/**
 * \brief 绑定一个复选框
 *
 * 在 webui 中创建一个复选框控件，`config` 为控件的配置，`data` 为该控件要绑定到数据。
 * `group` 指定的控件的分组，`label` 指定控件前显示的文字标签。
 *
 * 若绑定成功，函数唯一的绑定的 id，否则返回 `SIZE_MAX`。
 */
size_t k_webui_bind_checkbox(const char *group, const char *label, void *data, const struct k_webui_checkbox_config *config);

/* endregion */

/* region [button] */

/** \brief 按钮控件 */
struct k_webui_button_config {

    /** \brief 按钮中的文本 */
    const char *text;

    /**
     * \brief 在 webui 中点击按钮时触发的回调
     *
     * 必须指定此回调。
     */
    void (*on_click)(void *data);
};

/** \brief 用于初始化按钮控件配置 */
#define K_WEBUI_BUTTON_CONFIG_INIT \
{ \
    .text     = "click me", \
    .on_click = NULL, \
}

/**
 * \brief 绑定一个按钮
 *
 * 在 webui 中创建一个按钮控件，`config` 为控件的配置，`data` 为该控件要绑定到数据。
 * `group` 指定的控件的分组，`label` 指定控件前显示的文字标签。
 *
 * 若绑定成功，函数唯一的绑定的 id，否则返回 `SIZE_MAX`。
 */
size_t k_webui_bind_button(const char *group, const char *label, void *data, const struct k_webui_button_config *config);

/* endregion */

/* region [int_select] */

/** \brief 下拉选择框控件的选项 */
struct k_webui_int_select_option {

    /** \brief 选项的值，用于提交 */
    int val;

    /** \brief 选项的文本，用于展示 */
    const char *text;
};

/** \brief 下拉选择框控件 */
struct k_webui_int_select_config {

    /** \brief 选择框的选项（数组），以及选项的个数（数组长度） */
    struct k_webui_int_select_option *options;
    size_t options_num;

    /**
     * \brief 在 webui 中更改滑选择框中的选项时触发的回调
     *
     * `val` 是 webui 中选择框的新增，你可以将该值写入内存，或用于更新状态等等，
     * 若业务逻辑执行顺利，则函数应返回 0 表示接受该值，否则返回非 0 表示拒绝该值。
     *
     * 若 `on_change` 为 `NULL`，则默认将 `data` 视作指向 int 变量的指针，
     * 并将 `val` 的值写入 `data` 指向的内存。
     */
    int (*on_change)(void *data, int val);

    /**
     * \brief webui 定时同步触发的回调
     *
     * webui 定期执行此回调，你需要通过出参 `result` 告知 webui 当前绑定变量的值。
     * 若函数返回 0，则 webui 会将出参 `result` 的值同步更新下拉选择框当前选中的项。
     * 若函数返回非 0，则 webui 不更新控件。
     *
     * 若 `on_read` 为 `NULL`，则默认将 `data` 视作指向 int 变量的指针，
     * 读取 `data` 指向的内存，并同步更新下拉选择框当前选中的项。
     */
    int (*on_read)(void *data, int *result);
};

/** \brief 用于初始化下拉选择框控件配置 */
#define K_WEBUI_INT_SELECT_CONFIG_INIT \
{ \
    .options     = NULL, \
    .options_num = 0,    \
    .on_change   = NULL, \
    .on_read     = NULL, \
}

/**
 * \brief 绑定一个下拉选择框
 *
 * 在 webui 中创建一个下拉选择框控件。`config` 为控件的配置，`data` 为该控件要绑定到数据。
 * `group` 指定的控件的分组，`label` 指定控件前显示的文字标签。
 *
 * 若绑定成功，函数唯一的绑定的 id，否则返回 `SIZE_MAX`。
 */
size_t k_webui_bind_int_select(const char *group, const char *label, void *data, const struct k_webui_int_select_config *config);

/* endregion */

/**
 * \brief 解除绑定
 *
 * 通过 `bind_id` 解除绑定关系，并移除 webui 中已绑定的控件。
 * 若指定的 `bind_id` 不存在，该函数将直接返回。
 *
 * 由于 webui 无法感知 C 中的内存状态，无法判断内存是否有效，
 * 所以必须在绑定对象所依赖的内存即将失效前手动解除绑定，
 * 否则 webui 后续可能会访问无效内存。
 */
void k_webui_unbind(size_t bind_id);

/** \brief 解除所有绑定 */
void k_webui_unbind_all(void);

/* endregion */

#endif

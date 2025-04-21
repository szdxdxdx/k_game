#ifndef K_XML_H
#define K_XML_H

#include <stddef.h>



/*
 * [!] 功能不完善的 xml 解析器
 *
 * ---
 *
 * 该解析器可以解析下述的 xml 文本：
 *
 * ```xml
 * <book id="1" available="true">
 *      <!-- 测试 -->
 *      <title>Learn &lt;C&gt;</title>
 *      price：39.99
 * </book>
 * ```
 *
 * 解析结果：
 * 根节点为元素节点，tag 为 book，有 id 和 available 两个属性。
 * 根节点下一共有 5 个子节点：
 *  - 第 1 个是文本节点，文本内容是换行字符和一些空格字符
 *  - 第 2 个是注释节点，文本内容是“ 测试 ”
 *  - 第 3 个是文本节点，文本内容是换行字符和一些空格字符
 *  - 第 4 个是元素节点，tag 为 title。 有一个文本子节点，内容是“Learn <C>”。
 *    子节点文本中的实体引用 &lt; 和 &gt; 被替换成 `<` 和 `>` 符号。
 *  - 第 5 个是文本节点，文本内容是 “price：39.99” 以及前后的空格与换行字符。
 *
 * ---
 *
 * 目前支持的功能：
 * - 解析【元素节点】，例如： <book></book>
 * - 解析【闭合】的元素节点： <book id="1" available="true" />
 * - 解析带【属性】的元素节点： <book id="1" available="true"></book>
 * - 解析【文本节点】，例如： <price>39.99</price>
 * - 解析【注释节点】，例如： <!-- 注释 -->
 * - 五个基本的【实体引用】： &lt; &gt; &amp; &apos; &quot;
 * - 解析 xml 格式的文本得到的树结构是【只读】的
 *
 * ---
 *
 * 该模块是我在项目的后期阶段匆匆赶出来的。临近结题，我没有时间设计。
 * 确定好要提供的 api 后，我用一个下午时间实现了最基础的文本解析，
 * 晚上将代码封装成模块，第二天简单修复了部分错误，并补充文档注释。
 * 目前它是一个摇摇欲坠的模块。我之后会重构它，api 可能会有所变动。
 *
 * json 和 xml 解析器分别是在项目初期和末期时写的，但写得都是一坨。
 */



/** \brief xml 节点 */
struct k_xml_node;

/**
 * \brief 解析 xml 文本
 *
 * 函数解析 `text` 文本内容，并构建对应的 xml 树结构。
 * 若解析成功，函数返回 xml 树根节点（元素节点）的指针，否则返回 `NULL`。
 */
struct k_xml_node *k_xml_parse(char *text);

/**
 * \brief 原地解析 xml 文本
 *
 * 函数解析 `text` 文本内容，并构建对应的 xml 树结构。
 *
 * 解析过程中，函数将修改 `text` 字符串，写入 `\0` 字符分割内容。
 * 请确保 `text` 是可修改的缓冲区。
 *
 * 若解析成功，函数返回 xml 树根节点（元素节点）的指针。
 * 得到的 xml 节点中的字符串指针均直接指向 `text` 分割出的子串，
 * 在销毁 xml 节点树之前，请不要修改或释放 `text` 的内存。
 * 销毁 xml 节点树之后，你还需要手动释放 `text` 的内存。
 *
 * 若解析失败，函数返回 `NULL`。
 */
struct k_xml_node *k_xml_parse_in_place(char *text);

/**
 * \brief 销毁 xml 节点所在的 xml 树
 *
 * 销毁 xml 节点 `node` 所在的 xml 树，并释放内存。
 * 无论节点位于树中的哪个位置，都会销毁整棵树。
 *
 * 若 `node` 为 `NULL`，则函数立即返回。
 */
void k_xml_free(struct k_xml_node *node);

/** \brief xml 节点的类型 */
enum k_xml_node_type {
    K_XML_ELEM_NODE,
    K_XML_TEXT_NODE,
    K_XML_COMMENT_NODE,
};

/**
 * \brief 获取 xml 节点的类型
 *
 * 函数返回 `node` 节点的类型。请确保传入有效的指针。
 */
enum k_xml_node_type k_xml_get_type(struct k_xml_node *node);

/**
 * \brief 获取 xml 节点的第一个子节点
 *
 * 函数返回元素节点 `elem_node` 的第一个子节点，
 * 若它没有子节点，或它不是元素节点，则返回 `NULL`。
 */
struct k_xml_node *k_xml_get_first_child(struct k_xml_node *elem_node);

/**
 * \brief 获取 xml 节点的下一个兄弟节点
 *
 * 函数返回 `node` 的下一个兄弟节点，若没有下一个节点则返回 `NULL`。
 */
struct k_xml_node *k_xml_get_next_sibling(struct k_xml_node *node);

/**
 * \brief 获取 xml 节点的前一个兄弟节点
 *
 * 函数返回 `node` 的前一个兄弟节点，若没有前一个节点则返回 `NULL`。
 */
struct k_xml_node *k_xml_get_prev_sibling(struct k_xml_node *node);

/**
 * \brief 遍历 xml 元素节点的子节点
 *
 * 示例：
 * ```C
 * struct k_xml_node *elem_node = ...;
 *
 * int child_count = 0;
 * int elem_child_count = 0;
 * int text_child_count = 0;
 *
 * struct k_xml_node *child_node;
 * for (k_xml_for_each_child(elem_node, child_node)) {
 *
 *     child_count++;
 *
 *     if (K_XML_ELEM_NODE == k_xml_get_type(child_node))
 *          elem_child_count++;
 *     if (K_XML_TEXT_NODE == k_xml_get_type(child_node))
 *          text_child_count++;
 * }
 *
 * printf("child_count = %d, ", child_count);
 * printf("elem_child_count = %d, ", elem_child_count);
 * printf("text_child_count = %d. ", text_child_count);
 * ```
 */
#define k_xml_for_each_child(elem_node, child_node) \
    child_node = k_xml_get_first_child(elem_node); \
    NULL != child_node; \
    child_node = k_xml_get_next_sibling(child_node)

/**
 * \brief 获取 xml 节点的父节点
 *
 * 函数返回 `node` 的父节点，若 `node` 是根节点则返回 `NULL`。
 */
struct k_xml_node *k_xml_get_parent(struct k_xml_node *node);

/**
 * \brief 查找第一个具有指定标签名的子节点
 *
 * 函数遍历 `node` 的所有直接子节点，查找并返回第一个标签名是 `tag` 的子节点，找不到则返回 `NULL`。
 */
struct k_xml_node *k_xml_find_child_by_tag(struct k_xml_node *node, const char *tag);

/**
 * \brief 查找下一个具有指定标签名的兄弟节点
 *
 * 函数从 `node` 的下一个兄弟节点开始，向后查找并返回第一个标签名是 `tag` 的节点，找不到则返回 `NULL`。
 */
struct k_xml_node *k_xml_find_next_by_tag(struct k_xml_node *node, const char *tag);

/**
 * \brief 获取 xml 节点的标签名
 *
 * 函数返回元素节点 `elem_node` 的标签名，
 * 若传入的节点不是元素节点，则返回 `NULL`。
 */
const char *k_xml_get_tag(struct k_xml_node *elem_node);

/**
 * \brief 获取 xml 元素节点的指定属性值
 *
 * 函数遍历元素节点 `elem_node` 的所有属性，
 * 查找并返回第一个键名是 `attr_key` 的属性的值。
 * 若未找到，或节点不是元素节点，则函数返回 `NULL`。
 */
const char *k_xml_get_attr(struct k_xml_node *elem_node, const char *attr_key);

/** \brief xml 元素节点的属性 */
struct k_xml_attr;

/**
 * \brief 获取 xml 元素节点的第一个属性
 *
 * 函数返回元素节点 `elem_node` 的第一个属性，用于遍历元素节点的属性列表。
 * 出参 `get_key` 和 `get_val` 分别返回属性的键和值。
 * 若节点没有属性，或者不是元素节点，则函数返回 `NULL`。
 */
struct k_xml_attr *k_xml_get_first_attr(struct k_xml_node *elem_node, const char **get_key, const char **get_val);

/**
 * \brief 获取 xml 元素节点的下一个属性
 *
 * 函数返回 `attr` 的下一个属性，用于继续元素节点的遍历属性列表。
 * 出参 `get_key` 和 `get_val` 分别返回该属性的键和值。
 * 若 `attr` 已是最后一个属性，则函数返回 `NULL`。
 */
struct k_xml_attr *k_xml_get_next_attr(struct k_xml_attr *attr, const char **get_key, const char **get_val);

/**
 * \brief 遍历 xml 元素节点的属性
 *
 * 示例：
 * ```C
 * struct k_xml_node *elem_node = ...;
 *
 * struct k_xml_attr *attr;
 * const char *key;
 * const char *val;
 * for (k_xml_for_each_attr(elem_node, attr, key, val)) {
 *
 *     printf("key: %s, val: %s", key, val);
 * }
 * ```
 */
#define k_xml_for_each_attr(elem_node, attr, key, val) \
    attr = k_xml_get_first_attr(elem_node, &key, &val); \
    NULL != attr; \
    attr = k_xml_get_next_attr(attr, &key, &val)

/**
 * \brief 获取 xml 节点的文本内容
 *
 * 若 `node` 是元素节点，则函数遍历它的所有子节点，找出第一个文本子节点，
 * 并返回该子节点的文本字符串，若找不到则返回空字符串 ""。
 *
 * 若 `node` 是文本节点，则函数返回文本字符串。
 *
 * 若 `node` 是注释节点，则函数返回注释内容字符串。
 *
 * 其余情况，则函数返回 `NULL`。
 */
const char *k_xml_get_text(struct k_xml_node *node);

#endif

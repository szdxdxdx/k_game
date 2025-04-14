#ifndef K_XML_H
#define K_XML_H

#include <stddef.h>

/** \brief xml 节点 */
struct k_xml_node;

/** \brief xml 节点的类型 */
enum k_xml_node_type {
    K_XML_ELEM_NODE,
    K_XML_TEXT_NODE,
    K_XML_COMMENT_NODE,
};

/** \brief xml 元素节点的属性 */
struct k_xml_attr;

/**
 * \brief 解析 xml 文本
 *
 * 函数解析 `text` 文本内容，并构建对应的 xml 树结构。
 *
 * 函数会直接修改 `text` 字符串，写入 `\0` 字符分割内容。
 * 请确保 `text` 是可修改的缓冲区，而非字符串常量。
 *
 * 若解析成功，函数返回 DOM 树根节点指针，否则返回 `NULL`。
 */
struct k_xml_node *k_xml_parse(char *text);

/**
 * \brief 释放 xml 节点所在的 xml 树
 *
 * 无论节点 `node` 位于树中的哪个位置，都会释放整棵树。
 *
 * 若 `node` 为 `NULL`，则函数立即返回。
 */
void k_xml_free(struct k_xml_node *node);

/**
 * \brief 获取 xml 节点的第一个子节点
 *
 * 函数返回 `node` 的第一个子节点，若没有子节点则返回 `NULL`。
 */
struct k_xml_node *k_xml_get_first_child(struct k_xml_node *node);

/**
 * \brief 获取 xml 节点的下一个兄弟节点
 *
 * 函数返回 `node` 的下一个兄弟节点，若没有下一个节点则返回 `NULL`。
 */
struct k_xml_node *k_xml_get_next_sibling(struct k_xml_node *node);

/**
 * \brief 获取  xml 节点的父节点
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
 * \brief 获取 xml 节点的类型
 *
 * 函数返回 `node` 节点的类型。请确保传入有效的指针。
 */
enum k_xml_node_type k_xml_get_type(struct k_xml_node *node);

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

/**
 * \brief 获取 xml 元素节点的第一个属性
 *
 * 函数返回元素节点 `elem_node` 的第一个属性，用于遍历属性列表的起点。
 * 出参 `get_key` 和 `get_val` 分别返回属性的键和值。
 * 若节点没有属性，或者不是元素节点，则函数返回 `NULL`。
 */
struct k_xml_attr *k_xml_get_first_attr(struct k_xml_node *elem_node, const char **get_key, const char **get_val);

/**
 * \brief 获取 xml 元素节点的下一个属性
 *
 * 函数返回 `attr` 的下一个属性，用于继续遍历属性列表。
 * 出参 `get_key` 和 `get_val` 分别返回下一个属性的键和值。
 * 若 `attr` 已是最后一个属性，则函数返回 `NULL`。
 */
struct k_xml_attr *k_xml_get_next_attr(struct k_xml_attr *attr, const char **get_key, const char **get_val);

/**
 * \brief 获取 xml 节点的文本内容
 *
 * 若 `node` 是元素节点，则遍历它的所有子节点，找出第一个文本子节点，
 * 并返回该子节点的文本字符串，若找不到则返回空字符串 ""。
 *
 * 若 `node` 是文本节点，则返回该文本节点的文本字符串。
 *
 * 若 `node` 是注释节点，则返回该注释节点的注释文本字符串。
 *
 * 其余情况返回 `NULL`。
 */
const char *k_xml_get_text(struct k_xml_node *node);

#endif

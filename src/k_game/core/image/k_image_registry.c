#include "k_game/core/k_image.h"

#include "./k_image.h"
#include "./k_image_registry.h"


static struct k_asset_registry image_registry; /* 图片资源的注册表 */

/* 初始化图片资源的注册表 */
int k__image_registry_init(void) {
    return k__asset_registry_init(&image_registry);
}
/* 用于释放图片资源的回调 */
static void k__image_registry_release(struct k_asset_registry_node *node) {
    struct k_image *image = container_of(node, struct k_image, registry_node);
    k_image_release(image);
}
/* 在程序结束时，释放所有图片资源 */
void k__image_registry_cleanup(void) {
    k__asset_registry_cleanup(&image_registry, k__image_registry_release);
}
/* 往注册表中添加资源 */
void k__image_registry_add(struct k_image *image) {
    k__asset_registry_add(&image_registry, &image->registry_node);
}
/* 为图片资源设置名字索引 */
int k_image_set_name(struct k_image *image, const char *image_name) {
    return NULL == image ? -1 : k__asset_set_name(&image_registry, &image->registry_node, image_name);
}

void k__image_registry_del(struct k_image *image) {
    k__asset_registry_del(&image->registry_node);
}


struct k_image *k_image_find(const char *image_name) {
    struct k_asset_registry_node *registry_node = k__asset_registry_find(&image_registry, image_name);
    if (NULL == registry_node)
        return NULL;

    struct k_image *image = container_of(registry_node, struct k_image, registry_node);
    return image;
}

# 创建房间

创建房间使用函数 `k_room_create()`，示例代码如下：

```C
/* 先定义房间携带的自定义数据结构体 */
struct my_room {
    int data;
};

static int my_room_on_create(void *param);

static void my_room_on_enter(void);

static int on_game_start(void) {
    /* 框架建议在游戏初始化时创建所有房间 */

    /* 编写房间相关的配置，K_ROOM_CONFIG_INIT 为默认值 */
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w     = 640;  /* 房间的宽高，表示游戏世界的大小 */
    config.room_h     = 480;
    config.data_size  = sizeof(struct my_room); /* 指定房间携带的关联数据 */
    config.on_create  = my_room_on_create; /* 创建房间时执行的回调 */
    config.on_enter   = my_room_on_enter;  /* 进入房间时执行的回调 */
    
    const char *param = "hello";
    
    struct k_room *room = k_room_create(&config, param); /* 创建房间 */

    ...
}

/* 创建房间时执行的回调 */
static int my_room_on_create(void *param) {

    /* param 是在调用 k_room_create() 时传入的参数。在上述代码中，param 传入了一个字符串字面量 */
    printf("create my_room. param='%s'\n", (const char *)param);

    return 0;
}

/* 进入房间时执行的回调 */
static void my_room_on_enter(void) {
    
    printf("enter my_room\n");
}
```

房间结构本身包含了大量字段，且涉及诸多框架内部细节，所以为了减轻使用者负担，框架不在 API 头文件中公开房间结构体 `struct k_room` 的定义。
为了使房间具备一定的扩展能力，本框架采用一种在C语言中的很常见、很典型的封装技巧：使用 void* 指针挂载用户自定义数据。
本框架的所有核心结构体（如房间、对象、组件等）都采用这种设计方式。
但这也是C语言不支持面向对象的折中实现。

房间是一个相对重型的概念，其创建过程涉及很多内部模块的初始化工作。
创建房间时，用户需指定自定义数据的结构体大小（上述示例代码中挂载数据的结构体是 `struct my_room`），框架会分配该数据的内存。
用户可在房间的 `on_create()` 回调中对这块内存进行初始化，以及执行其他初始化工作，例如：往房间中放置游戏对象、添加事件回调等。
若 `on_create()` 返回0，框架会继续完成后续的初始化流程，最终成功创建房间。若返回非0，框架会自动销毁房间。
通过这种方式，用户可以根据不同类型的关卡或场景定制化每个房间，且只用关注初始化过程中业务相关的部分。

由于房间肩负太多职责，所以太灵活地操作房间是一件很危险的事。出于对框架稳定性和使用安全性的考量，本框架暂不提供销毁房间的API。
因为一旦房间被不当销毁，资源可能被错误释放，造成悬空指针等一系列问题。目前只能在游戏结束时，由框架集中销毁所有房间。
销毁房间时，房间中的对象、组件等资源会被一并销毁，用户可在房间的 `on_destroy()` 回调中完成对其他资源的清理工作。


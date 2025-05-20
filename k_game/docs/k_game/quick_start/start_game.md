# 启动游戏

本框架中，游戏的入口函数为 `k_game_run()`。开启游戏的示例代码如下：

```C

#include "k_game.h"

static int on_game_start(void);

int main(void) {
    
    /* 编写游戏相关的配置，K_GAME_CONFIG_INIT 为默认值 */
    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w = ...; /* 游戏窗口的宽高 */
    config.window_h = ...;
    config.window_title = "..."; /* 游戏窗口的标题 */
    config.on_start = on_game_start; /* 必须指定游戏初始化的回调 */

    return k_game_run(&config); /* 开启游戏 */
}

/* 游戏初始化时执行的回调 */
static int on_game_start(void) {
    ...
    
    return 0; /* 返回 0 表示初始化成功，返回非 0 表示初始化失败。若失败，游戏无法启动，框架会回滚 */
}
```

你可以在游戏初始化的回调 `on_game_start` 中，加载资源（图片、音频等）、创建房间、注册组件类型等等。
该回调返回 0 表示初始化成功，返回非 0 表示失败。若失败则游戏无法启动，框架会释放已加载的资源、销毁房间等。

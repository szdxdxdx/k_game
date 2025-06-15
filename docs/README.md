# k_game API 文档

```C
#include "k_game.h"

int main(void) {

    struct k_game_config config = K_GAME_CONFIG_INIT;
    config.window_w     = 640;
    config.window_h     = 480;
    config.window_title = "hello world"
    config.on_start     = ...;

    return k_game_run(&config);
}
```

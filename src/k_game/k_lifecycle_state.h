#ifndef K_GAME__LIFECYCLE_STATE_H
#define K_GAME__LIFECYCLE_STATE_H

enum k_lifecycle_state {

    K_LIFECYCLE_INITIALIZING,
    K_LIFECYCLE_PENDING,
    K_LIFECYCLE_ACTIVE,
    K_LIFECYCLE_DELETING,

    K_LIFECYCLE_CALLBACK,

    // 初始状态
    K_LIFECYCLE_ALLOCATED,          // 已分配到内存
    K_LIFECYCLE_PARTIALLY_INITIALIZED, // 完成部分初始化
    K_LIFECYCLE_USER_INIT_CALLBACK_COMPLETE, // 完成用户自定义的初始化回调
    K_LIFECYCLE_FULLY_INITIALIZED,  // 完成所有初始化，启用

    // 运行状态
    K_LIFECYCLE_SUSPENDED,          // 实例被挂起
    K_LIFECYCLE_PAUSED,             // 实例被暂停
    K_LIFECYCLE_RESUMED,            // 实例从挂起或暂停状态恢复

    // 错误状态
    K_LIFECYCLE_ERROR,              // 实例处于错误状态

    // 删除和清理状态
    K_LIFECYCLE_MARKED_FOR_DELETION, // 用户手动删除或释放，标记删除
    K_LIFECYCLE_DELETED,            // 系统正式标记该实例被删除
    K_LIFECYCLE_USER_CLEANUP_CALLBACK_COMPLETE, // 完成用户定义的清理回调
    K_LIFECYCLE_DEINITIALIZED,      // 反初始化
    K_LIFECYCLE_MEMORY_RELEASED     // 以释放内存
};

#endif

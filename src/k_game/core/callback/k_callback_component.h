#ifndef K_GAME__CALLBACK_COMPONENT_H
#define K_GAME__CALLBACK_COMPONENT_H

#include "k_list.h"

#include "./k_callback_base.h"
#include "../component/k_component.h"

struct k_component_callback {

    struct k_list_node list_node;

    struct k_callback_base *base;
};

#endif

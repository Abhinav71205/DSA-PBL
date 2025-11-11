#ifndef ORDER_HISTORY_H
#define ORDER_HISTORY_H
#include "cart_order.h"
#include "inventory.h"

void save_order_to_file(const char *username, Cart *c, Inventory *inv);
#endif
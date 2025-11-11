#ifndef CART_ORDER_H
#define CART_ORDER_H
#include "product.h"
#include "inventory.h"

typedef struct CartItem {
    int product_id;
    int qty;
    struct CartItem *next;
} CartItem;
typedef struct Cart {
    CartItem *head;
    int size;
} Cart;

void init_cart(Cart *c);
int add_to_cart(Cart *c, int product_id, int qty);
int remove_from_cart(Cart *c, int product_id);
float compute_cart_total(Cart *c, struct Inventory *inv);
void list_cart(Cart *c, struct Inventory *inv);
void clear_cart(Cart *c);

#endif
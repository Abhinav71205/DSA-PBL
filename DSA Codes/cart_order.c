#include <stdio.h>
#include <stdlib.h>
#include "cart_order.h"
void init_cart(Cart *c) {
    c->head = NULL;
    c->size = 0;
}
int add_to_cart(Cart *c, int product_id, int qty) {
    if (qty <= 0) return 0; 
    if (qty > 1000) {
        printf("Quantity exceeds limit (1000). Setting to 1000.\n");
        qty = 1000;
    }
    CartItem *cur = c->head;
    while (cur) {
        if (cur->product_id == product_id) {
            cur->qty += qty;
            if (cur->qty > 1000) cur->qty = 1000; 
            return 1;
        }
        cur = cur->next;
    }
    CartItem *it = (CartItem*)malloc(sizeof(CartItem));
    if (!it) return 0;
    it->product_id = product_id;
    it->qty = qty;
    it->next = c->head;
    c->head = it;
    c->size++;
    return 1;
}
int remove_from_cart(Cart *c, int product_id) {
    CartItem *cur = c->head, *prev = NULL;
    while (cur) {
        if (cur->product_id == product_id) {
            if (prev) prev->next = cur->next;
            else c->head = cur->next;

            free(cur);
            c->size--;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}
float compute_cart_total(Cart *c, Inventory *inv) {
    float total = 0.0f;
    CartItem *cur = c->head;
    while (cur) {
        Product *p = find_product_by_id(inv, cur->product_id);
        if (p) total += p->price * cur->qty;
        cur = cur->next;
    }
    return total;
}
void list_cart(Cart *c, Inventory *inv) {
    if (c->head == NULL) {
        printf("Your cart is empty.\n");
        return;
    }
    printf("%-5s %-20s %-10s %-10s %-10s\n", "ID", "Name", "Qty", "Price", "Subtotal");
    printf("---------------------------------------------------------------\n");
    CartItem *cur = c->head;
    while (cur) {
        Product *p = find_product_by_id(inv, cur->product_id);
        if (p) {
            printf("%-5d %-20s %-10d %-10.2f %-10.2f\n",
                   p->id, p->name, cur->qty, p->price, p->price * cur->qty);
        } else {
            printf("%-5d %-20s %-10d %-10s %-10s\n",
                   cur->product_id, "(NotFound)", cur->qty, "-", "-");
        }
        cur = cur->next;
    }
}
void clear_cart(Cart *c) {
    CartItem *cur = c->head;
    while (cur) {
        CartItem *n = cur->next;
        free(cur);
        cur = n;
    }
    c->head = NULL;
    c->size = 0;
}  
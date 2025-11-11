#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cart_order.h"
#include "inventory.h"


void save_order_to_file(const char *username, Cart *c, Inventory *inv) {
    FILE *f = fopen("orders.txt", "a");
    if (!f) {
        printf("Error: Could not open orders.txt for writing.\n");
        return;
    }
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[40];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    float total = compute_cart_total(c, inv);
    fprintf(f, "-----------------------------------------\n");
    fprintf(f, "User: %s\n", username);
    fprintf(f, "Date: %s\n", time_str);
    fprintf(f, "-----------------------------------------\n");
    fprintf(f, "%-5s %-20s %-10s %-10s %-10s\n", "ID", "Name", "Qty", "Price", "Subtotal");
    CartItem *cur = c->head;
    while (cur) {
        Product *p = find_product_by_id(inv, cur->product_id);
        if (p) {
            fprintf(f, "%-5d %-20s %-10d %-10.2f %-10.2f\n",
                    p->id, p->name, cur->qty, p->price, p->price * cur->qty);
        }
        cur = cur->next;
    }
    fprintf(f, "-----------------------------------------\n");
    fprintf(f, "Total: %.2f\n\n", total);
    fclose(f);
    printf("Order saved successfully for user '%s'.\n", username);
}
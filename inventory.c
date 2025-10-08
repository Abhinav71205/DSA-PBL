#include <stdio.h>
#include "product_management.c"
void updateStock(int id, int qty) {
    int index = searchProduct(id);
    if (index != -1) {
        inventory[index].quantity -= qty;
        if (inventory[index].quantity < 0)
            inventory[index].quantity = 0;
        printf("Stock updated for Product ID %d\n", id);
    } else {
        printf("Product not found!\n");
    }
}
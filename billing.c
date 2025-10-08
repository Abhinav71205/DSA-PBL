#include <stdio.h>
#include "product_management.c"
void generateBill() {
    float total = 0.0;
    printf("\n--- Generating Bill ---\n");
    for (int i = 0; i < productCount; i++) {
        total += inventory[i].price * inventory[i].quantity;
    }
    printf("Total Amount: Rs. %.2f\n", total);
}
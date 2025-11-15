#include <stdio.h>
#include "billing.h"
void print_bill(Cart *c, Inventory *inv) {
    printf("\n=========================================\n");
    printf("               CUSTOMER BILL             \n");
    printf("=========================================\n");
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Qty", "Price");
    printf("-----------------------------------------\n");
    list_cart(c, inv);
    float total = compute_cart_total(c, inv);
    printf("-----------------------------------------\n");
    printf("%-30s %.2f\n", "Total Amount (Rs):", compute_cart_total(c, inv));
    printf("=========================================\n");
    printf("Thank you for shopping with us!\n");
    printf("=========================================\n");
}
#include <stdio.h>
#include <stdlib.h>
struct CartItem {
    int productId;
    int quantity;
    struct CartItem *next;
};
struct CartItem *cartHead = NULL;
void addToCart(int productId, int quantity) {
    struct CartItem *newItem = (struct CartItem*)malloc(sizeof(struct CartItem));
    newItem->productId = productId;
    newItem->quantity = quantity;
    newItem->next = cartHead;
    cartHead = newItem;
    printf("Item added to cart!\n");
}
void displayCart() {
    struct CartItem *temp = cartHead;
    if (temp == NULL) {
        printf("Cart is empty!\n");
        return;
    }
    printf("\n--- Cart Items ---\n");
    while (temp != NULL) {
        printf("Product ID: %d | Quantity: %d\n", temp->productId, temp->quantity);
        temp = temp->next;
    }
}
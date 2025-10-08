#include <stdio.h>
#include <string.h>
#define MAX_PRODUCTS 100
struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};
struct Product inventory[MAX_PRODUCTS];
int productCount = 0;
void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        printf("Inventory full!\n");
        return;
    }
    struct Product p;
    printf("Enter Product ID: ");
    scanf("%d", &p.id);
    printf("Enter Product Name: ");
    scanf("%s", p.name);
    printf("Enter Product Price: ");
    scanf("%f", &p.price);
    printf("Enter Product Quantity: ");
    scanf("%d", &p.quantity);
    inventory[productCount++] = p;
    printf("Product added successfully!\n");
}
void displayProducts() {
    printf("\n--- Product List ---\n");
    for (int i = 0; i < productCount; i++) {
        printf("ID: %d | Name: %s | Price: %.2f | Qty: %d\n",
               inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }
}
int searchProduct(int id) {
    for (int i = 0; i < productCount; i++) {
        if (inventory[i].id == id) return i;
    }
    return -1;
}
int binarySearchProduct(int id) {
    int low = 0, high = productCount - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (inventory[mid].id == id) return mid;
        else if (inventory[mid].id < id) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}
void sortProductsByPrice() {
    for (int i = 0; i < productCount - 1; i++) {
        for (int j = 0; j < productCount - i - 1; j++) {
            if (inventory[j].price > inventory[j + 1].price) {
                struct Product temp = inventory[j];
                inventory[j] = inventory[j + 1];
                inventory[j + 1] = temp;
            }
        }
    }
    printf("Products sorted by price successfully!\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"
void init_inventory(Inventory *inv) {
    inv->head = NULL;
    inv->size = 0;
}
Product* create_product(int id, const char *name, float price, int qty) {
    if (qty > 1000) {
        printf("Quantity exceeds limit (1000). Setting to 1000.\n");
        qty = 1000;
    }
    Product *p = (Product*)malloc(sizeof(Product));
    if (!p) return NULL;
    p->id = id;
    strncpy(p->name, name, NAME_LEN - 1);
    p->name[NAME_LEN - 1] = '\0';
    p->price = price;
    p->qty = qty;
    p->next = NULL;
    return p;
}
void free_product(Product *p) {
    free(p);
}
void load_inventory_from_file(Inventory *inv, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("No existing file found. Starting fresh inventory.\n");
        return;
    }
    int id, qty;
    char name[NAME_LEN];
    float price;
    while (fscanf(f, "%d,%49[^,],%f,%d\n", &id, name, &price, &qty) == 4) {
        Product *p = create_product(id, name, price, qty);
        add_product(inv, p);
    }
    fclose(f);
}
void save_inventory_to_file(Inventory *inv, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error saving inventory file.\n");
        return;
    }
    Product *cur = inv->head;
    while (cur) {
        fprintf(f, "%d,%s,%.2f,%d\n", cur->id, cur->name, cur->price, cur->qty);
        cur = cur->next;
    }
    fclose(f);
    printf("Inventory saved successfully.\n");
}
Product* find_product_by_id(Inventory *inv, int id) {
    Product *cur = inv->head;
    while (cur) {
        if (cur->id == id)
            return cur;
        cur = cur->next;
    }
    return NULL;
}
void add_product(Inventory *inv, Product *new_product) {
    if (find_product_by_id(inv, new_product->id)) {
        printf("Product ID %d already exists. Skipping duplicate.\n", new_product->id);
        free_product(new_product);
        return;
    }
    new_product->next = NULL;
    if (inv->head == NULL)
        inv->head = new_product;
    else {
        Product *temp = inv->head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_product;
    }
    inv->size++;
}
int update_product_qty(Inventory *inv, int id, int new_qty) {
    if (new_qty > 1000) {
        printf("Quantity exceeds limit (1000). Setting to 1000.\n");
        new_qty = 1000;
    }
    Product *p = find_product_by_id(inv, id);
    if (!p) return 0;
    p->qty = new_qty;
    return 1;
}
int remove_product(Inventory *inv, int id) {
    Product *cur = inv->head, *prev = NULL;
    while (cur) {
        if (cur->id == id) {
            if (prev)
                prev->next = cur->next;
            else
                inv->head = cur->next;

            free_product(cur);
            inv->size--;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}
void list_products(Inventory *inv) {
    if (!inv->head) {
        printf("No products available in inventory.\n");
        return;
    }
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Price", "Qty");
    printf("---------------------------------------------\n");
    Product *cur = inv->head;
    while (cur) {
        printf("%-5d %-20s %-10.2f %-10d\n", cur->id, cur->name, cur->price, cur->qty);
        cur = cur->next;
    }
}
void free_inventory(Inventory *inv) {
    Product *cur = inv->head;
    while (cur) {
        Product *next = cur->next;
        free_product(cur);
        cur = next;
    }
    inv->head = NULL;
    inv->size = 0;
}
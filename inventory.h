#ifndef INVENTORY_H
#define INVENTORY_H
#include "product.h"

typedef struct Inventory {
    Product *head;
    int size;
} Inventory;
void init_inventory(Inventory *inv);
void load_inventory_from_file(Inventory *inv, const char *filename);
void save_inventory_to_file(Inventory *inv, const char *filename);
Product* find_product_by_id(Inventory *inv, int id);
void add_product(Inventory *inv, Product *p);   
int update_product_qty(Inventory *inv, int id, int new_qty);
int remove_product(Inventory *inv, int id);
void list_products(Inventory *inv);
void free_inventory(Inventory *inv);
#endif

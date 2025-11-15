#ifndef PRODUCT_H
#define PRODUCT_H
#include <stdio.h>
#define NAME_LEN 50

typedef struct Product {
    int id;
    char name[NAME_LEN];
    float price;
    int qty;
    struct Product *next;
} Product;

Product* create_product(int id, const char *name, float price, int qty);
void free_product(Product *p);

#endif 

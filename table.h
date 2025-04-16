#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Table Table;

typedef struct Node
{
    bool busy;
    char *key;
    int info;
} Node;

Table *table_init(size_t, enum Err *);
void insert_el(Table *, const char *, int, Err *);
void remove_el(Table *, const char *, Err *);
const Node *search_el(const Table *, const char *, Err *);
void print_table(const Table *);
void free_table(Table *);

#endif

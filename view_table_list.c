#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "table.h"
#include "menu.h"

struct Table
{
    size_t csize;
    struct Node *head;
};

struct Node
{
    char *key;
    int info;
    struct Node *next;
};

Table *table_init(size_t, Err *err)
{
    Table *table = (Table *)malloc(sizeof(Table));
    if (!table)
    {
        *err = MEM_ERR;
        return NULL;
    }
    table->csize = 0;
    table->head = NULL;
    *err = OK;
    return table;
}

void insert_el(Table *table, const char *key, int info, Err *err)
{
    Node *next;
    for (Node *ptr = table->head; ptr; ptr = next)
    {
        next = ptr->next;
        if (!strcmp(key, ptr->key))
        {
            *err = NUNIQUE;
            return;
        }
    }
    Node *new_el = (Node *)malloc(sizeof(Node));
    if (!new_el)
    {
        *err = MEM_ERR;
        return;
    }
    if (!(new_el->key = strdup(key)))
    {
        *err = MEM_ERR;
        free(new_el);
        return;
    }
    new_el->info = info;
    new_el->next = table->head;
    table->head = new_el;
    table->csize++;
    *err = OK;
}

void remove_el(Table *table, const char *key, Err *err)
{
    Node *prev, *el = (Node *)search_el(table, key, &prev, err);
    if (*err != OK)
        return;
    if (el == table->head || !prev)
        table->head = table->head->next;
    else
        prev->next = el->next;
    table->csize--;
    free(el->key);
    free(el);
    *err = OK;
}

const Node *search_el(const Table *table, const char *key, Node **prev_ptr, Err *err)
{
    if (prev_ptr)
        *prev_ptr = NULL;
    Node *next;
    for (Node *ptr = table->head; ptr; ptr = next)
    {
        next = ptr->next;
        if (!strcmp(ptr->key, key))
        {
            *err = OK;
            return ptr;
        }
        if (prev_ptr)
            *prev_ptr = ptr;
    }
    *err = NFOUND;
    if (prev_ptr)
        *prev_ptr = NULL;
    return NULL;
}

void print_el(const Node *el)
{
    printf("{key: \"%s\", info: %d}\n", el->key, el->info);
}

void print_table(const Table *table)
{
    printf("Table of %zu elements\n", table->csize);
    Node *next;
    for (Node *ptr = table->head; ptr; ptr = next)
    {
        next = ptr->next;
        print_el(ptr);
    }
}

void free_table(Table *table)
{
    Node *next;
    for (Node *ptr = table->head; ptr; ptr = next)
    {
        next = ptr->next;
        free(ptr->key);
        free(ptr);
    }
    free(table);
}

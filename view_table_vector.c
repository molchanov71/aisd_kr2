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
    size_t msize;
    Node *arr;
};

Table *table_init(size_t msize, Err *err)
{
    Table *table = (Table *)malloc(sizeof(Table));
    if (!table)
    {
        *err = MEM_ERR;
        return NULL;
    }
    if (!(table->arr = (Node *)calloc(msize, sizeof(Node))))
    {
        free(table);
        *err = MEM_ERR;
        return NULL;
    }
    table->msize = msize;
    for (Node *ptr = table->arr; ptr < table->arr + msize; ptr++)
    {
        ptr->busy = false;
        ptr->key = NULL;
        ptr->info = 0;
    }
    *err = OK;
    return table;
}

void insert_el(Table *table, const char *key, int info, Err *err)
{
    Node *free_el = NULL;
    for (Node *ptr = table->arr; ptr < table->arr + table->msize; ptr++)
    {
        if (ptr->busy && !strcmp(key, ptr->key))
        {
            *err = NUNIQUE;
            return;
        }
        else if (!ptr->busy)
        {
            free_el = ptr;
            break;
        }
    }
    if (!free_el)
    {
        *err = TABLE_OVF;
        return;
    }
    if (!(free_el->key = strdup(key)))
    {
        *err = MEM_ERR;
        return;
    }
    free_el->info = info;
    free_el->busy = true;
    *err = OK;
}

void remove_el(Table *table, const char *key, Err *err)
{
    Node *ptr = (Node *)search_el(table, key, err);
    if (!ptr || *err != OK)
    {
        *err = NFOUND;
        return;
    }
    free(ptr->key);
    ptr->key = NULL;
    ptr->busy = false;
    *err = OK;
    return;

    *err = NFOUND;
}

const Node *search_el(const Table *table, const char *key, Err *err)
{
    for (Node *ptr = table->arr; ptr < table->arr + table->msize; ptr++)
        if (ptr->busy && !strcmp(key, ptr->key))
        {
            *err = OK;
            return ptr;
        }
    *err = NFOUND;
    return NULL;
}

void print_table(const Table *table)
{
    if (!table)
        return;
    for (Node *ptr = table->arr; ptr < table->arr + table->msize; ptr++)
        if (ptr->busy)
            print_el(ptr);
}

void free_table(Table *table)
{
    if (!table)
        return;
    for (Node *ptr = table->arr; ptr < table->arr + table->msize; ptr++)
        free(ptr->key);
    free(table->arr);
    free(table);
}

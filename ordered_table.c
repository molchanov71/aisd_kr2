#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "table.h"

struct Table
{
    size_t msize;
    size_t csize;
    struct Node *arr;
};

struct Node
{
    char *key;
    int info;
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
    table->csize = 0;
    for (Node *ptr = table->arr; ptr < table->arr + msize; ptr++)
    {
        ptr->key = NULL;
        ptr->info = 0;
    }
    *err = OK;
    return table;
}

void insert_el(Table *table, const char *key, int info, Err *err)
{
    if (table->csize >= table->msize)
    {
        *err = TABLE_OVF;
        return;
    }
    if (search_el(table, key, NULL, err))
    {
        *err = NUNIQUE;
        return;
    }
    char *key_cpy = strdup(key);
    if (!key_cpy)
    {
        *err = MEM_ERR;
        return;
    }
    size_t free_ind = table->csize;
    for (size_t i = 0; i < table->csize; i++)
        if (strcmp(key, table->arr[i].key) == -1)
        {
            for (size_t j = table->csize - 1; j >= i; j--)
            {
                memcpy(table->arr + j + 1, table->arr + j, sizeof(Node));
                if (j == 0)
                    break;
            }
            free_ind = i;
            break;
        }
    table->csize++;
    table->arr[free_ind].key = key_cpy;
    table->arr[free_ind].info = info;
    *err = OK;
    return;
}

void remove_el(Table *table, const char *key, Err *err)
{
    if (table->csize == 0)
    {
        *err = EMPTY;
        return;
    }
    Node *el = (Node *)search_el(table, key, NULL, err);
    if (!el)
    {
        *err = NFOUND;
        return;
    }
    free(el->key);
    size_t i = el - table->arr;
    for (size_t j = i; j < table->csize - 1; j++)
        memcpy(table->arr + j, table->arr + j + 1, sizeof(Node));
    table->csize--;
    *err = OK;
    return;
}

const Node *search_el(const Table *table, const char *key, Node **, Err *err)
{
    if (table->csize == 0)
    {
        *err = NFOUND;
        return NULL;
    }
    size_t i = 0, m = table->csize - 1, j = table->csize;
    bool flag = false;
    while (i <= m)
    {
        j = (i + m) / 2;
        switch (strcmp(table->arr[j].key, key))
        {
        case 0:
            flag = true;
            break;
        case -1:
            if (j == 0)
            {
                j = table->csize;
                flag = true;
            }
            else
                m = j - 1;
            break;
        case 1:
            i = j + 1;
            break;
        }
        if (flag)
            break;
    }
    if (!flag || j == table->csize)
    {
        *err = NFOUND;
        return NULL;
    }
    *err = OK;
    return table->arr + j;
}

void print_el(const Node *el)
{
    printf("{key: \"%s\", info: %d}\n", el->key, el->info);
}

void print_table(const Table *table)
{
    if (!table)
        return;
    printf("Table of %zu elements\n", table->csize);
    for (Node *ptr = table->arr; ptr < table->arr + table->csize; ptr++)
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

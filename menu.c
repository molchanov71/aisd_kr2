#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "errors.h"
#include "error_msgs.h"
#include "table.h"
#include "menu.h"

#define PROMPT "1. Insert value\n2. Remove value\n3. Search value\n4. Print table"

void menu(Table *table, Err *err)
{
    int opt;
    for (;;)
    {
        printf("\n%s\n", PROMPT);
        opt = input_num("Select option: ", 1, 4, err);
        if (*err != OK)
            break;
        switch (opt)
        {
        case 1:
            menu_insert(table, err);
            break;
        case 2:
            menu_remove(table, err);
            break;
        case 3:
            menu_search(table, err);
            break;
        case 4:
            print_table(table);
        }
        if (*err == EOF_ERR)
            break;
        else if (*err != OK)
            printf("\n%s\n", error_msgs[*err]);
    }
}

void menu_insert(Table *table, Err *err)
{
    if (!table)
    {
        *err = NINIT;
        return;
    }
    char *key = readline("Input key: ");
    if (!key)
    {
        *err = EOF_ERR;
        return;
    }
    if (search_el(table, key, err))
    {
        *err = NUNIQUE;
        return;
    }
    int info = input_num("Input info: ", INT_MIN, INT_MAX, err);
    if (*err != OK)
    {
        free(key);
        return;
    }
    insert_el(table, key, info, err);
    free(key);
}

void menu_remove(Table *table, Err *err)
{
    if (!table)
    {
        *err = NINIT;
        return;
    }
    char *key = readline("Input key: ");
    if (!key)
    {
        *err = EOF_ERR;
        return;
    }
    remove_el(table, key, err);
    free(key);
}

void menu_search(const Table *table, Err *err)
{
    if (!table)
    {
        *err = NINIT;
        return;
    }
    char *key = readline("Input key: ");
    if (!key)
    {
        *err = EOF_ERR;
        free(key);
        return;
    }
    const Node *el = search_el(table, key, err);
    if (el && *err == OK)
        print_el(el);
    free(key);
}

void print_el(const Node *el)
{
    printf("{key: \"%s\", info: %d}\n", el->key, el->info);
}

int input_num(const char *s, int min, int max, Err *err)
{
    char eol;
    int res, scan;
    do
    {
        printf(s);
        scan = scanf("%d%c", &res, &eol);
        if (scan == EOF)
        {
            *err = EOF_ERR;
            return 0;
        }
        if (scan != 2 || eol != '\n')
        {
            scanf("%*[^\n]");
            printf("Input error\n");
            continue;
        }
        if (res < min || res > max)
        {
            if (res < min && max == INT_MAX)
                printf("Number should be greater than or equal %d\n", min);
            else if (min == INT_MIN && res > max)
                printf("Number should be less than or equal %d\n", max);
            else
                printf("Number should be greater than or equal %d and less than or equal %d\n", min, max);
        }
    } while (scan != 2 || eol != '\n' || res < min || res > max);
    *err = OK;
    return res;
}

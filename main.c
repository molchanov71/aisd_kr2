#include <stddef.h>
#include <stdio.h>
#include "errors.h"
#include "error_msgs.h"
#include "table.h"
#include "menu.h"

size_t input_size(const char *, Err *);

#define ERR_CHECK(table, err)                           \
    do                                                  \
    {                                                   \
        if (err != OK)                                  \
        {                                               \
            fprintf(stderr, "\n%s\n", error_msgs[err]); \
            free_table(table);                          \
            return err;                                 \
        }                                               \
    } while (0);

int main()
{
    Err err;
    size_t msize = input_size("Input size of table: ", &err);
    ERR_CHECK(NULL, err);
    Table *table = table_init(msize, &err);
    ERR_CHECK(NULL, err);
    menu(table, &err);
    ERR_CHECK(table, err);
    return err;
}

size_t input_size(const char *s, Err *err)
{
    char eol;
    int scan;
    size_t res;
    do
    {
        printf(s);
        scan = scanf("%zu%c", &res, &eol);
        if (scan == EOF)
        {
            *err = EOF_ERR;
            return 0;
        }
        if (scan != 2 || eol != '\n')
        {
            scanf("%*[^\n]");
            printf("Input error\n");
        }
    } while (scan != 2 || eol != '\n');
    *err = OK;
    return res;
}

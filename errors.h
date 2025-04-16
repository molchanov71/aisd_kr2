#ifndef ERRORS_H
#define ERRORS_H

typedef enum Err
{
    OK = 0,
    MEM_ERR = 1,
    EOF_ERR = 2,
    NUNIQUE = 3,
    TABLE_OVF = 4,
    NFOUND = 5,
    NINIT = 6
} Err;

#endif

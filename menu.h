#ifndef MENU_H
#define MENU_H

void menu(struct Table *, enum Err *);
void menu_insert(struct Table *, enum Err *);
void menu_remove(struct Table *, enum Err *);
void menu_search(const struct Table *, enum Err *);
int input_num(const char *, int, int, enum Err *);

#endif

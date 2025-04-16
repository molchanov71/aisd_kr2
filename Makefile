view_vector:
	gcc -g -lreadline main.c menu.c view_table_vector.c -DMSIZE -o main.out -Wall -Wextra -Wpedantic

view_list:
	gcc -g -lreadline main.c menu.c view_table_list.c -o main.out -Wall -Wextra -Wpedantic

ordered:
	gcc -g -lreadline main.c menu.c ordered_table.c -DMSIZE -o main.out -Wall -Wextra -Wpedantic

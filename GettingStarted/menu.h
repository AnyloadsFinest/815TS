#ifndef __MENU_INCLUDED
#define __MENU_INCLUDED

#define MMAX 10    // maximum number of menu items

typedef int (*pt2Function)(void);

void menu_add(void (*pt2Func)(void),char string[]);
void execute_menu(int menuitem);
void init_menu(void);
void user_menu(int key);
void scan_tickets(void);
void menu_print(void);
void build_menu(void);



#endif
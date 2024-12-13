#ifndef MENU_H
#define MENU_H 1

void print_menu(const char* menu[], const int selections[], const int size);
int menu_choose(const int selections[], const int size);

extern const char* MAIN_MENU[];

extern const int MAIN_MENU_SIZE;

extern const int MAIN_MENU_SELECTIONS[];

#endif  // MENU_H
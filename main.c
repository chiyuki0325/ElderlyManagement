#include "menu.h"
#include "ui.h"

int main() {
  // 起手式
  set_utf8();

  int choice;  // 当前选择功能

  // do-while 循环结构
  do {
    choice = -1; // -1 代表未选择，0-8 代表功能
    while (choice == -1) {
      clear_screen();
      print_title();
      print_menu();
      flush_input();
      choice = menu_choose();
    }
    // 功能选择完毕
    // 采用分支结构处理功能
    switch (choice) {
        // TODO: 实现功能
    }
  } while (choice != 0);
  
  // choice == 0，退出系统
  // TODO: 保存数据
  return 0;
}
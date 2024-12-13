#include "bus.h"
#include "house.h"
#include "members.h"
#include "menu.h"
#include "poetry.h"
#include "save.h"
#include "ui.h"
#include "servants.h"
#include "venue.h"
#include <ctype.h>

int main() {
  // 起手式
  set_utf8();

  // 先读取数据
  load_state(1);

  int choice; // 当前选择功能

  // do-while 循环结构
  do {
    choice = -1; // -1 代表未选择，0-8 代表功能
    while (choice == -1) {
      clear_screen();
      print_title();
      printf("%s\n\n", random_poem());
      print_menu(MAIN_MENU, MAIN_MENU_SELECTIONS, MAIN_MENU_SIZE);
      choice = menu_choose(MAIN_MENU_SELECTIONS, MAIN_MENU_SIZE);
      flush_input();
    }
    // 功能选择完毕
    // 采用分支结构处理功能
    switch (choice) {
      // TODO: 实现功能
    case 1:
      members_mgmt_menu();
      break;
    case 2:
      houses_mgmt_menu();
      break;
    case 3:
      venue_mgmt_menu();
      break;
    case 4:
      servants_menu();
      break;
    case 5:
      bus_menu();
      break;
    case 6:
      save_menu();
      break;
    case 7:
      clear_screen();
      printf("如果社区管理人员在使用本系统的过程中发现 Bug 或遇到任何疑问，\n"
             "欢迎给以下邮箱地址发送邮件。\n\n"
             "> me@chyk.ink\n\n"
             "活力长者社区信息化与网络办公室将竭诚为您服务。\n\n"
             "按下回车键返回。");
      getchar();
      break;
    case 0:
      break;
    default:
      printf("功能尚未实现，请等待小组成员完善系统功能。\n");
      sleep_millis(1000);
      break;
    }
  } while (choice != 0);

  // choice == 0，退出系统

  printf("退出之前，是否要保存数据？（Y/n）");

  char c;
  scanf("%c", &c);
  if (tolower(c) != 'n') {
    save_state();
  }
  printf("感谢使用！\n");
  sleep_millis(2000);

  return 0;
}
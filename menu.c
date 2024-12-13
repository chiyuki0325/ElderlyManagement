#include <stdio.h>
#include "ui.h"

/*
 * menu.c
 * 菜单
 */

const char* MAIN_MENU[] = {
    "会员管理",
    "房屋和入住管理",
    "场馆设施管理",
    "服务人员管理",
    "班车管理",
    "保存数据",
    "问题反馈",
    "退出系统"
};

const int MAIN_MENU_SIZE = 8;

const int MAIN_MENU_SELECTIONS[] = {
    1, 2, 3, 4, 5, 6, 7, 0
};
void print_menu(const char* menu[], const int selections[], int size) {
  // 打印菜单
  // 按格式传入参数即可，可参考主菜单的实现
  printf("输入数字以选择功能：\n\n");
  for (int i = 0; i < size; i++) {
    printf(" - [%d] %s\n", selections[i], menu[i]);
  }
  printf("\n");
}

int menu_choose(int selections[], int size) {
  // 选择功能
  int choice;
  printf("> ");
  scanf("%d", &choice);
  for (int i = 0; i < size; i++) {
    if (choice == selections[i]) {
      return choice;
    }
  }
  printf("无效选择，请重新输入。\n");
  sleep_millis(1000);
  return -1;
}
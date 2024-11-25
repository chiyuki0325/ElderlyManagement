#include <stdio.h>

/*
 * menu.c
 * 菜单
 */

void print_menu() {
  // 打印菜单
  printf("输入数字以选择功能：\n");
  printf("- [1] 会员管理\n");
  printf("- [2] 房屋管理\n");
  printf("- [3] 入住管理\n");
  printf("- [4] 场馆设施管理\n");
  printf("- [5] 服务人员管理\n");
  printf("- [6] 班车管理\n");
  printf("- [7] 数据备份\n");
  printf("- [8] 问题反馈\n\n");
  printf("- [0] 退出系统\n\n");
}

int menu_choose() {
  // 选择功能
  int choice;
  printf("> ");
  scanf("%d", &choice);
  if (choice < 0 || choice > 8) {
    printf("输入错误，请重新输入\n");
    return -1;
  } else {
    return choice;
  }
}
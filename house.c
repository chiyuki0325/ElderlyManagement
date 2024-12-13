#include "house.h"
#include "members.h"
#include "menu.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * house.c
 * 房屋管理相关代码
 * 采用数组存储房屋信息
 * Author:
 * Fixed by
 */

// 采用数组存储房屋信息，最大数量为 100
House houses[100];

const char *HOUSES_MENU[] = {"新增房屋", "拆除房屋", "购买房屋",
                             "出租房屋", "退租房屋", "显示所有房屋信息",
                             "返回"};
const int HOUSES_MENU_SIZE = 7;
const int HOUSES_MENU_SELECTIONS[] = {1, 2, 3, 4, 5, 6, 0};

House *find_house_by_id(int id) {
  // 线性查找，按 ID 查找房屋
  for (int i = 0; i < 100; i++) {
    if (houses[i].id == id) {
      return &houses[i];
    }
  }
  // 要是没有找到，返回 NULL
  return NULL;
}

void purchase_house(MemberNode *member, int house_id);
void rent_out_house(MemberNode *member, int house_id);

void purchase_house_menu() {
  // 购买房屋
  clear_screen();
  print_subtitle("购买房屋");
  int member_id, house_id;
  MemberNode *member = NULL;
  do {
    printf("请输入会员 ID：");
    scanf("%d", &member_id);
    flush_input();
    member = query_member_by_id_impl(member_id);
    if (member == NULL) {
      printf("未找到会员。\n");
      sleep_millis(1000);
    }
  } while (member == NULL);
  printf("找到会员 [%d] %s。\n", member->id, member->name);
  do {
    printf("请输入房屋 ID ：");
    scanf("%d", &house_id);
    flush_input();
  } while (house_id <= 0);
  purchase_house(member, house_id);
}
void purchase_house(MemberNode *member, int house_id) {
  House *house = find_house_by_id(house_id);
  if (member != NULL && house != NULL) {
    // 房屋状态：0 - 空置，1 - 自住，2 - 出租
    if (house->status == 0) {
      house->status = 1;
      house->used_by = member->id;
      printf("[%d] %s 已购买房屋 %d。\n", member->id, member->name, house->id);
      printf("地址：%s\n", house->address);
      printf("基本信息：%s\n", house->basic_info);
    } else {
      MemberNode *used_by = query_member_by_id_impl(house->used_by);
      printf("房屋 %d 已被 [%d] %s 占用。\n", house->id, used_by->id,
             used_by->name);
    }
    printf("按下回车键继续。\n");
    getchar();
  } else {
    printf("无效的会员 ID 或房屋 ID。\n");
    sleep_millis(1000);
  }
}

void rent_out_house_menu() {
  // 出租房屋
  clear_screen();
  print_subtitle("出租房屋");
  int member_id, house_id;
  MemberNode *member = NULL;
  do {
    printf("请输入会员 ID：");
    scanf("%d", &member_id);
    flush_input();
    member = query_member_by_id_impl(member_id);
    if (member == NULL) {
      printf("未找到会员。\n");
      sleep_millis(1000);
    }
  } while (member == NULL);
  printf("找到会员 [%d] %s。\n", member->id, member->name);
  do {
    printf("请输入房屋 ID ：");
    scanf("%d", &house_id);
    flush_input();
  } while (house_id <= 0);
  rent_out_house(member, house_id);
}

void rent_out_house(MemberNode *member, int house_id) {
  House *house = find_house_by_id(house_id);
  if (member != NULL && house != NULL) {
    if (house->status == 0) {
      house->status = 2;
      house->used_by = member->id;
      printf("[%d] %s 已将房屋 %d 出租。\n", member->id, member->name,
             house->id);
      printf("地址：%s\n", house->address);
      printf("基本信息：%s\n", house->basic_info);
    } else {
      MemberNode *used_by = query_member_by_id_impl(house->used_by);
      printf("房屋 %d 已被 [%d] %s 占用。\n", house->id, used_by->id,
             used_by->name);
    }
    printf("按下回车键继续。\n");
    getchar();
  } else {
    printf("无效的会员 ID 或房屋 ID。\n");
    sleep_millis(1000);
  }
}

void rent_cancel_house(int house_id) {
  // 取消出租
  House *house = find_house_by_id(house_id);
  if (house != NULL) {
    if (house->status == 2) {
      house->status = 0;
      printf("房屋 %d 已取消出租。\n", house->id);
    } else {
      printf("房屋 %d 未出租。\n", house->id);
    }
    printf("按下回车键继续。\n");
    getchar();
  } else {
    printf("无效的房屋 ID。\n");
    sleep_millis(1000);
  }
}

void rent_cancel_house_menu() {
  // 取消出租
  clear_screen();
  print_subtitle("取消出租");
  int house_id;
  do {
    printf("请输入房屋 ID：");
    scanf("%d", &house_id);
    flush_input();
  } while (house_id <= 0);
  rent_cancel_house(house_id);
}

int get_house_count() {
  // 查找房屋数量
  int count = 0;
  for (int i = 0; i < 100; i++) {
    if (houses[i].id != 0) {
      count++;
    }
  }
  return count;
}

void new_house() {
  // 新增房屋
  int id;
  char address[100];
  char basic_info[200];
  do {
    printf("请输入房屋 ID (必须为正数)：");
    scanf("%d", &id);
    flush_input();
  } while (id <= 0);
  if (find_house_by_id(id) != NULL) {
    printf("房屋 ID 已存在。\n");
    sleep_millis(1000);
    return;
  }
  // 用 fgets 读取字符串，更安全
  printf("请输入房屋地址：\n");
  fgets(address, 100, stdin);
  address[strlen(address) - 1] = '\0';

  printf("请输入房屋基本信息：\n");
  fgets(basic_info, 200, stdin);
  basic_info[strlen(basic_info) - 1] = '\0';

  for (int i = 0; i < 100; i++) {
    if (houses[i].id == 0) {
      // 找到空闲位置，存储房屋信息
      houses[i].id = id;
      strcpy(houses[i].address, address);
      strcpy(houses[i].basic_info, basic_info);
      houses[i].status = 0;
      printf("房屋 %d 已添加。\n", id);
      sleep_millis(1000);
      return;
    }
  }
  printf("房屋数量已达上限，不能再建设更多房屋！\n");
  sleep_millis(1000);
}

void remove_house() {
  // 拆除房屋
  int id;
  do {
    printf("请输入要拆除的房屋 ID：");
    scanf("%d", &id);
  } while (id <= 0);
  flush_input();
  House *house = find_house_by_id(id);
  if (house != NULL) {
    if (house->status != 0) {
      printf("房屋 %d 已被占用，无法拆除。\n", id);
      sleep_millis(1000);
      return;
    }
    house->id = 0;
    printf("房屋 %d 已拆除。\n", id);
    sleep_millis(1000);
  } else {
    printf("无效的房屋 ID。\n");
    sleep_millis(1000);
  }
}
void list_houses() {
  clear_screen();
  print_subtitle("所有房屋信息");
  printf(" ID\t地址\t状态\t基本信息\n");
  MemberNode *member;
  for (int i = 0; i < 100; i++) {
    if (houses[i].id == 0) {
      break;
    }
    printf("[%d]\t%s\t", houses[i].id, houses[i].address);
    switch (houses[i].status) {
    case 0:
      printf("空置\t");
      break;
    case 1:
      member = query_member_by_id_impl(houses[i].used_by);
      printf("%s 居住\t", member->name);
      break;
    case 2:
      member = query_member_by_id_impl(houses[i].used_by);
      printf("出租给 %s\t", member->name);
      break;
    }
    printf("%s\n", houses[i].basic_info);
  }
  printf("\n按下回车键继续。\n");
  getchar();
}

void houses_mgmt_menu() {
  // 房屋管理系统主菜单
  int choice;
  do {
    choice = -1;
    while (choice == -1) {
      clear_screen();
      print_subtitle("房屋管理");

      printf("本社区目前有 %d 栋房屋。\n\n", get_house_count());

      // 打印菜单，选择功能
      print_menu(HOUSES_MENU, HOUSES_MENU_SELECTIONS, HOUSES_MENU_SIZE);
      choice = menu_choose(HOUSES_MENU_SELECTIONS, HOUSES_MENU_SIZE);
      flush_input();
    }
    // 功能选择完毕
    // 采用分支结构处理功能
    switch (choice) {
    case 1:
      new_house();
      break;
    case 2:
      remove_house();
      break;
    case 3:
      purchase_house_menu();
      break;
    case 4:
      rent_out_house_menu();
      break;
    case 5:
      rent_cancel_house_menu();
      break;
    case 6:
      list_houses();
      break;
    }
  } while (choice != 0);
  // 函数结束，返回主菜单
}
int serialize_houses_to_file(FILE *fp, House *houses) {
  // 将房屋信息序列化到文件
  int count = 0;
  fprintf(fp, "id,address,basic_info,status\n");
  for (int i = 0; i < 100; i++) {
    if (houses[i].id != 0) {
      fprintf(fp, "%d,%s,%s,%d\n", houses[i].id, houses[i].address,
              houses[i].basic_info, houses[i].status);
      count++;
    } else {
      break;
    }
  }
  return count;
}

int deserialize_houses_from_file(FILE *fp) {
  // 从文件中反序列化房屋信息
  char buffer[500];
  fgets(buffer, 500, fp); // 读取第一行，跳过标题
                          // 清空 houses
  for (int i = 0; i < 100; i++) {
    houses[i].id = 0;
  }
  int index = 0;
  while (fgets(buffer, 500, fp) != NULL) {
    // 逐行读取
    int id;
    char address[100];
    char basic_info[200];
    int status;
    sscanf(buffer, "%d,%[^,],%[^,],%d", &id, address, basic_info, &status);
    houses[index].id = id;
    strcpy(houses[index].address, address);
    strcpy(houses[index].basic_info, basic_info);
    houses[index].status = status;
    index++;
  }
  return index;
}

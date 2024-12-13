#include "venue.h"
#include "menu.h"
#include "ui.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * venue.c
 * 场馆设施管理相关代码
 * 使用两套链表存储
 * author:
 */

const char *VENUE_MENU[] = {"添加新场馆",
                            "预约场馆",
                            "发布场馆封闭通知",
                            "安排保洁人员",
                            "制定并发布场馆使用规则",
                            "资源分配",
                            "设备设施定期维护",
                            "显示所有场馆信息",
                            "返回主菜单"};

const int VENUE_MENU_SIZE = 9;
const int VENUE_MENU_SELECTIONS[] = {1, 2, 3, 4, 5, 6, 7, 8, 0};

// 保洁人员列表头指针
Cleaner *cleanersList = NULL;

// 场馆列表头指针
Venue *venuesList = NULL;

// 添加新场馆的函数
void addVenue(char *name, int capacity) {
  Venue *newVenue = (Venue *)malloc(sizeof(Venue));
  strcpy(newVenue->name, name);
  newVenue->capacity = capacity;
  newVenue->isAvailable = 1;
  newVenue->maintenancePeriod = 30; // 初始设置为 30天
  newVenue->next = venuesList;
  venuesList = newVenue;
  printf("场馆 %s 添加成功！\n", name);
}

// 预约场馆的函数，简单判断是否可预约
void bookVenue(char *name) {
  Venue *curr = venuesList;
  while (curr != NULL) {
    if (with_prefix(curr->name, name)) {
      if (curr->isAvailable) {
        printf("成功预约场馆 %s\n", name);
      } else {
        printf("场馆 %s 目前处于封闭状态，无法预约。\n", name);
      }
      return;
    }
    curr = curr->next;
  }
  printf("未找到名为 %s 的场馆，预约失败。\n", name);
}

// 发布场馆封闭通知的函数
void closeVenue(char *name) {
  Venue *curr = venuesList;
  while (curr != NULL) {
    if (with_prefix(curr->name, name)) {
      curr->isAvailable = 0;
      printf("已发布场馆 %s 的封闭通知，该场馆现在不可用。\n", name);
      return;
    }
    curr = curr->next;
  }
  printf("未找到名为 %s 的场馆，封闭通知发布失败。\n", name);
}

// 安排保洁人员的函数
void assignCleaner(char *cleanerName, char *venueName) {
  Cleaner *newCleaner = (Cleaner *)malloc(sizeof(Cleaner));
  strcpy(newCleaner->name, cleanerName);
  strcpy(newCleaner->assignedVenue, venueName);
  newCleaner->next = cleanersList;
  cleanersList = newCleaner;
  printf("保洁人员 %s 已安排到场馆 %s\n", cleanerName, venueName);
}

// 制定并发布场馆使用规则的函数
void setVenueRules(char *venueName, char *rules) {
  printf("场馆 %s 的使用规则已制定并发布：\n%s\n", venueName, rules);
}

// 资源分配函数，确保每位老人有活动空间（简单示例，假设按比例分配）
void allocateResources(int numElders) {
  int totalCapacity = 0;
  Venue *curr = venuesList;
  while (curr != NULL) {
    totalCapacity += curr->capacity;
    curr = curr->next;
  }
  if (totalCapacity == 0) {
    printf("没有可用场馆，无法进行资源分配。\n");
    return;
  }
  curr = venuesList;
  while (curr != NULL) {
    int allocated = (curr->capacity * numElders) / totalCapacity;
    printf("场馆 %s 分配到 %d 位老人活动空间。\n", curr->name, allocated);
    curr = curr->next;
  }
}

// 设备设施定期维修维护函数，模拟时间推移并检查是否需要维修
void performMaintenance() {
  char isNeedMaintain = 0;
  Venue *curr = venuesList;
  while (curr != NULL) {
    curr->maintenancePeriod--;
    if (curr->maintenancePeriod == 0) {
      printf("场馆 %s 需要进行设备设施维修维护。\n", curr->name);
      isNeedMaintain = 1;
      // 这里可添加实际维修维护的操作代码，比如标记为维修中状态等
      curr->maintenancePeriod = 30; // 维修完成后重新设置维护周期
    } else {
      printf("场馆 %s 距离下次设备设施维护还有 %d 天。\n", curr->name,
             curr->maintenancePeriod);
    }
    curr = curr->next;
  }
  if (!isNeedMaintain) {
    printf("今天没有场馆需要进行设备设施维护。\n");
  }
}

// 显示所有场馆信息的辅助函数（方便查看当前场馆情况）
void displayVenues() {
  clear_screen();
  print_subtitle("当前场馆列表");
  Venue *curr = venuesList;
  while (curr != NULL) {
    printf("场馆名称：%s，容纳人数：%d，状态：%s，距离下次维护时间：%d 天\n",
           curr->name, curr->capacity, curr->isAvailable ? "可用" : "封闭",
           curr->maintenancePeriod);
    curr = curr->next;
  }
}

/*
 * void venue_mgmt_menu();
 * author:
 * adapted by
 */

void venue_mgmt_menu() {
  int choice;
  char venueName[50];
  int venueCapacity;
  char cleanerName[50];
  char rules[200];
  int numElders;

  do {
    choice = -1;
    while (choice == -1) {
      clear_screen();
      print_subtitle("场馆设施管理");
      print_menu(VENUE_MENU, VENUE_MENU_SELECTIONS, VENUE_MENU_SIZE);
      choice = menu_choose(VENUE_MENU_SELECTIONS, VENUE_MENU_SIZE);
      flush_input();
    }

    char c;

    switch (choice) {
    case 1:
      do {
        printf("请输入场馆名称：");
        scanf("%s", venueName);
        process_gbk_input(venueName);
        printf("请输入场馆容纳人数：");
        scanf("%d", &venueCapacity);
        addVenue(venueName, venueCapacity);
        flush_input();

        printf("是否要继续添加？(y/N)\n");
        scanf("%c", &c);
        printf("\n");
      } while (tolower(c) == 'y');
      break;
    case 2:
      printf("请输入要预约的场馆名称：");
      scanf("%s", venueName);
      process_gbk_input(venueName);
      flush_input();
      bookVenue(venueName);
      printf("按回车键返回。\n");
      getchar();
      break;
    case 3:
      printf("请输入要封闭的场馆名称：");
      scanf("%s", venueName);
      process_gbk_input(venueName);
      flush_input();
      closeVenue(venueName);
      printf("按回车键返回。\n");
      getchar();
      break;
    case 4:
      printf("请输入保洁人员姓名：");
      scanf("%s", cleanerName);
      process_gbk_input(cleanerName);
      flush_input();
      printf("请输入负责的场馆名称：");
      scanf("%s", venueName);
      process_gbk_input(venueName);
      flush_input();
      assignCleaner(cleanerName, venueName);
      printf("按回车键返回。\n");
      getchar();
      break;
    case 5:
      printf("请输入场馆名称：");
      scanf("%s", venueName);
      process_gbk_input(venueName);
      flush_input();
      printf("请输入场馆使用规则：");
      scanf(" %[^\n]", rules);
      process_gbk_input(rules);
      flush_input();
      setVenueRules(venueName, rules);
      printf("按回车键返回。\n");
      getchar();
      break;
    case 6:
      printf("请输入老人数量：");
      scanf("%d", &numElders);
      flush_input();
      allocateResources(numElders);
      printf("按回车键返回。\n");
      getchar();
      break;
    case 7:
      performMaintenance();
      printf("按回车键返回。\n");
      getchar();
      break;
    case 8:
      displayVenues();
      printf("按回车键返回。\n");
      getchar();
      break;
    }
  } while (choice != 0);
}

/*
 * 序列化和反序列化函数（CSV）
 * author:
 */

// 保存场馆设施数据
int serialize_venues_to_file(FILE *fp, Venue *head) {
  if (head == NULL)
    return 0;
  Venue *p = head;
  int count = 0;
  fputs("name,capacity,isAvailable,maintenancePeriod\n", fp);
  while (p != NULL) {
    fprintf(fp, "%s,%d,%d,%d\n", p->name, p->capacity, p->isAvailable,
            p->maintenancePeriod);
    p = p->next;
    count++;
  }
  return count;
}

// 从文件加载场馆设施
int deserialize_venues_from_file(FILE *fp) {
  venuesList = NULL;
  Venue *tail = NULL;
  int count = 0;
  char buf[200];
  fgets(buf, 200, fp); // 读取第一行，跳过
  while (fgets(buf, 200, fp) != NULL) {
    Venue *newVenue = (Venue *)malloc(sizeof(Venue));
    sscanf(buf, "%[^,],%d,%d,%d", newVenue->name, &newVenue->capacity,
           &newVenue->isAvailable, &newVenue->maintenancePeriod);
    newVenue->next = NULL;
    if (venuesList == NULL) {
      venuesList = newVenue;
      tail = newVenue;
    } else {
      tail->next = newVenue;
      tail = newVenue;
    }
    count++;
  }
  return count;
}

// 保存保洁人员数据
int serialize_cleaners_to_file(FILE *fp, Cleaner *head) {
  if (head == NULL)
    return 0;
  Cleaner *p = head;
  int count = 0;
  fputs("name,assignedVenue\n", fp);
  while (p != NULL) {
    fprintf(fp, "%s,%s\n", p->name, p->assignedVenue);
    p = p->next;
    count++;
  }
  return count;
}

// 从文件加载保洁人员数据
int deserialize_cleaners_from_file(FILE *fp) {
  cleanersList = NULL;
  Cleaner *tail = NULL;
  int count = 0;
  char buf[200];
  fgets(buf, 200, fp); // 读取第一行，跳过
  while (fgets(buf, 200, fp) != NULL) {
    Cleaner *newCleaner = (Cleaner *)malloc(sizeof(Cleaner));
    sscanf(buf, "%[^,],%s", newCleaner->name, newCleaner->assignedVenue);
    newCleaner->next = NULL;
    if (cleanersList == NULL) {
      cleanersList = newCleaner;
      tail = newCleaner;
    } else {
      tail->next = newCleaner;
      tail = newCleaner;
    }
    count++;
  }
  return count;
}

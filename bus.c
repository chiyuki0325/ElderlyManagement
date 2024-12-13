#include "members.h"
#include "menu.h"
#include "ui.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bus.h"

/*
 * bus.c
 * 班车管理相关代码
 * 使用链表数据结构存储车站
 * 使用线性查找查询
 */
BUS *bus_routine = NULL;
const char *BUS_MENU[] = {
    "增加车站",
    "减少车站",
    "返回菜单"};
const int BUS_MENU_SIZE = 3;
const int BUS_MENU_LENGTH[] = {
    1, 2, 0};

const char *BUS_INPUT_LIST[] = {
    "在终点站后添加",
    "在选定节点前添加"};
const int BUS_INPUT_LENGTH[] = {
    1, 2};
const int BUS_INPUT_SIZE = 2;

void bus_menu()
{
  // 主菜单进入班车管理
  int choice; // 当前选择功能
  if (bus_routine == NULL)
  {
    // 第一次进入系统
    bus_routine = BUS_list_create();
  }

  BUS *tail;

  do
  {
    choice = -1;
    while (choice == -1)
    {
      clear_screen();
      print_subtitle("班车管理");

      // 打印一些车站以供预览
      ATraverseReturns returns = BUS_list_print(bus_routine, 5);
      if (returns.count > 0)
        printf("\n");
      printf("本社区共设置了 %d 个站。\n\n", returns.count);
      tail = returns.tail;

      // 打印菜单，选择功能
      print_menu(BUS_MENU, BUS_MENU_LENGTH, BUS_MENU_SIZE);
      choice = menu_choose(BUS_MENU_LENGTH, BUS_MENU_SIZE);
      flush_input();
    }
    // 功能选择完毕
    // 采用分支结构处理功能
    switch (choice)
    {
    case 1:
      BUS_STOP_JIA(bus_routine, tail);
      break;
    case 2:
      BUS_STOP_JIAN(bus_routine, tail);
      break;
    }
  } while (choice != 0);
  // 函数结束，返回主菜单
}

void BUS_STOP_JIA(BUS *head, BUS *tail)
{
  int choice = -1;
  while (choice == -1)
  {
    clear_screen();
    print_subtitle("添加车站");
    print_menu(BUS_INPUT_LIST, BUS_INPUT_LENGTH, BUS_INPUT_SIZE);
    choice = menu_choose(BUS_INPUT_LENGTH, BUS_INPUT_SIZE);
    flush_input();
  }
  switch (choice)
  {
  case 1:
    BUS *p = creat_a_stop();
    link_them(tail, p);
    tail = p;
    printf("操作成功\n");
    break;
  case 2:
    BUS *new_stop = creat_a_stop();
    insert_a_stop(new_stop, bus_routine);
    break;
  case 0:
    break;
  default:
    printf("请输入对应数字!\n");
    choice = -1;
    break;
    flush_input();
  }
}

void BUS_STOP_JIAN(BUS *head, BUS *tail)
{
  BUS *p = Find_the_stop(head, tail);
  if (p != NULL)
  {
    delete_a_stop(p, head);
  }
}

BUS *BUS_list_create()
{
  // 创建空链表, head 不存储数据
  BUS *head = (BUS *)malloc(sizeof(BUS));
  strcpy(head->name, "\0");
  head->next = NULL;
  return head;
}

BUS *creat_a_stop() // 创建一个车站
{
  BUS *p = (BUS *)malloc(sizeof(BUS));
  p->next = NULL;
  printf("请输入新车站的名称。");
  char *buf = (char *)malloc(50);
  fgets(buf, 50, stdin);
  process_gbk_input(buf);
  remove_line_feed_chars(buf);
  strcpy(p->name, buf);
  free(buf);
  return p;
}

void link_them(BUS *pre, BUS *after) // 链接两个车站
{
  pre->next = after;
  after->next = NULL;
}

ATraverseReturns BUS_list_print(BUS *head, int max_prints)
{
  // 打印链表
  // max_prints 代表最大打印数量，-1 代表全部打印
  BUS *tail = head;
  BUS *p = head->next;
  int count = 0;
  int ellipsis_printed = 0; // 是否打印了省略号
  while (p != NULL)
  {
    if (count < max_prints || max_prints == -1)
    {
      printf("- [%d] 站： %s\n", count + 1, p->name);
    }
    else if (!ellipsis_printed)
    {
      // 在末尾打印省略号
      printf("...\n");
      ellipsis_printed = 1;
    }
    tail = p;
    p = p->next;
    count++;
  }
  // 使用结构体以返回两个值
  return (ATraverseReturns){count, tail};
}

BUS *Find_the_stop(BUS *head, BUS *tail) // 找到想要找的那个车站
{
  int count = 0;

  printf("请输入要访问的车站名称：\n> ");
  char *name_to_find = (char *)malloc(50);
  fgets(name_to_find, 50, stdin);
  process_gbk_input(name_to_find);
  remove_line_feed_chars(name_to_find);

  BUS *p = head->next; // 从第一个车站开始遍历
  while (p != NULL)    // 只要p不为NULL就继续查找
  {
    if (with_prefix(p->name, name_to_find))
    {
      printf("\n找到了第%d站\n", count + 1);
      return p; // 找到目标车站，返回当前车站指针
    }
    p = p->next; // 继续向下一个车站移动
    count++;     // 车站计数
  }

  // 没有找到目标车站
  printf("\n不存在该车站哦qaq\n");
  sleep_millis(1000);
  return NULL;
}

void insert_a_stop(BUS *the_stop_you_creat, BUS *head)
{
  // 在find_stop前面加入一个新车站

  // parent: 当前比较的节点的上一个节点
  // current: 当前被比较的节点
  BUS *parent = head;
  BUS *current = head->next;

  printf("请输入一个车站名称，将在此车站前添加车站。\n> ");
  char *name_to_find = (char *)malloc(50);
  fgets(name_to_find, 50, stdin);
  process_gbk_input(name_to_find);
  remove_line_feed_chars(name_to_find);

  while (current != NULL)
  {
    // 开始线性查找
    if (with_prefix(current->name, name_to_find))
    {
      // 找到了。
      break;
    }
    parent = current;
    current = current->next;
  }

  if (parent == NULL || current == NULL)
  {
    // 啥也没找到...
    printf("未找到名称为 %s 的车站。\n", name_to_find);
    sleep_millis(1000);
    free(the_stop_you_creat);
    return;
  }

  parent->next = the_stop_you_creat;
  the_stop_you_creat->next = current;

  printf("操作成功，按回车键返回菜单。\n");
  getchar();
}

void delete_a_stop(BUS *the_stop_youwant_delete, BUS *head) // 删除一个指定车站
{
  BUS *pre = NULL;
  if (the_stop_youwant_delete != NULL)
  {
    if (the_stop_youwant_delete == head->next)
    {
      head->next = the_stop_youwant_delete->next;
      free(the_stop_youwant_delete);
    }
    else
    {
      pre = head;
      while (pre->next != the_stop_youwant_delete)
      {
        pre = pre->next;
      }
      pre->next = the_stop_youwant_delete->next;
      free(the_stop_youwant_delete);
    }
    printf("操作成功，按回车键返回菜单。\n");
    getchar();
  }
  else
  {
    printf("请输入有效的车站编号");
  }
}

int serialize_bus_stops_to_file(FILE *fp, BUS *head)
{
  // 使用 CSV 格式序列化班车车站数据
  // 其实，车站只有一个域 name，
  // 所以 C 不 CSV 的也无所谓
  if (head == NULL)
    return 0;
  BUS *p = head->next;
  int count = 0;
  fputs("name\n", fp);
  while (p != NULL)
  {
    fprintf(fp, "%s\n", p->name);
    p = p->next;
    count++;
  };
  return count;
}

int deserialize_bus_stops_from_file(FILE *fp)
{
  bus_routine = BUS_list_create();
  BUS *current = bus_routine;
  int count = 0;
  char buf[50];
  fgets(buf, 50, fp); // 读取第一行，跳过
  while (fgets(buf, 50, fp) != NULL)
  {
    remove_line_feed_chars(buf);
    if (strlen(buf) == 0)
    {
      break;
    }
    BUS *p = (BUS *)malloc(sizeof(BUS));
    strcpy(p->name, buf);
    p->next = NULL;
    current->next = p;
    current = current->next;
  }
  // 读取完毕
  return count;
}
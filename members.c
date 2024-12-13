#include "members.h"
#include "menu.h"
#include "ui.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * members.c
 * 会员管理相关代码
 * 使用链表数据结构存储会员
 * 使用线性查找查询
 */

// 函数声明
void create_member_menu(MemberNode *tail);
void query_member_menu();
void query_member_by_id_menu();
void query_member_by_name_menu();

void print_all_members_menu();
void enter_to_modify();

void modify_member_menu(MemberNode *p);

void delete_member_by_id(int id);

void sort_members_menu();
void sort_the_list(MemberNode **head_ref, SortType sort_type);

const char *MEMBERS_MENU[] = {"新增会员", "查找或修改会员", "会员整理排序",
                              "返回主菜单"};

const int MEMBERS_MENU_SIZE = 4;

const int MEMBERS_MENU_SELECTIONS[] = {1, 2, 3, 0};

const char *MEMBERS_QUERY_MENU[] = {"按 ID 查询", "按姓名查询", "查看所有会员",
                                    "返回会员管理菜单"};

const int MEMBERS_QUERY_MENU_SIZE = 4;
const int MEMBERS_QUERY_MENU_SELECTIONS[] = {1, 2, 3, 0};

const char *MEMBERS_MODIFY_MENU[] = {"修改姓名", "修改年龄", "删除会员",
                                     "返回"};
const int MEMBERS_MODIFY_MENU_SIZE = 4;
const int MEMBERS_MODIFY_MENU_SELECTIONS[] = {1, 2, 3, 0};

const char *MEMBERS_SORT_MENU[] = {"按年龄升序", "按年龄降序", "按 ID 排序",
                                   "返回"};

const int MEMBERS_SORT_MENU_SIZE = 4;
const int MEMBERS_SORT_MENU_SELECTIONS[] = {1, 2, 3, 0};

MemberNode *member_list = NULL;

MemberNode *member_list_create()
{
  // 创建空链表, head 不存储数据
  MemberNode *head = (MemberNode *)malloc(sizeof(MemberNode));
  head->id = -1;
  head->next = NULL;
  return head;
}

MemberNode *member_list_append(MemberNode *tail, int id, int age, char *name)
{
  // 在链表末尾添加节点
  MemberNode *p = (MemberNode *)malloc(sizeof(MemberNode));
  p->id = id;
  p->age = age;
  strcpy(p->name, name);
  p->next = NULL;
  tail->next = p;
  return p;
  // 返回新的以巴
}

TraverseReturns member_list_print(MemberNode *head, int max_prints)
{
  // 打印链表
  // max_prints 代表最大打印数量，-1 代表全部打印
  MemberNode *p = head->next;
  int count = 0;
  int ellipsis_printed = 0; // 是否打印了省略号
  MemberNode *tail = head;
  while (p != NULL)
  {
    if (count < max_prints || max_prints == -1)
    {
      printf("- [%d] %s (%d)\n", p->id, p->name, p->age);
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
  return (TraverseReturns){count, tail};
}

void members_mgmt_menu()
{
  // 会员管理菜单
  // 由主菜单进入
  int choice; // 当前选择功能
  if (member_list == NULL)
  {
    // 第一次进入系统
    member_list = member_list_create();
  }

  MemberNode *tail;

  do
  {
    choice = -1;
    while (choice == -1)
    {
      clear_screen();
      print_subtitle("会员管理");

      // 打印一些会员以供预览
      TraverseReturns returns = member_list_print(member_list, 5);
      if (returns.count > 0)
        printf("\n");
      printf("本社区共居住了 %d 位会员。\n\n", returns.count);
      tail = returns.tail;

      // 打印菜单，选择功能
      print_menu(MEMBERS_MENU, MEMBERS_MENU_SELECTIONS, MEMBERS_MENU_SIZE);
      choice = menu_choose(MEMBERS_MENU_SELECTIONS, MEMBERS_MENU_SIZE);
      flush_input();
    }
    // 功能选择完毕
    // 采用分支结构处理功能
    switch (choice)
    {
    case 1:
      create_member_menu(tail);
      break;
    case 2:
      query_member_menu();
      break;
    case 3:
      sort_members_menu();
      break;
    }
  } while (choice != 0);
  // 函数结束，返回主菜单
}

void create_member_menu(MemberNode *tail)
{
  // 新增会员

  // 定义一些变量
  int default_id = 1;
  int id = 0;
  int age = 0;
  char name[40];

  if (tail->id != -1)
  {
    // 存在末尾 ID，则继续自增
    default_id = tail->id + 1;
  }

  int exists = 0;

  do
  {
    printf("请输入会员 ID（默认为 %d）：", default_id);
    char buf[10];
    fgets(buf, 10, stdin);
    if (strlen(buf) > 1)
    {
      sscanf(buf, "%d", &id);
      // sscanf 函数：用格式化，将字符串转换为数字读入 id
    }

    if (id <= 0)
    {
      printf("* 使用默认 ID %d。\n", default_id);
      id = default_id;
    }

    // 检查 id 是否存在
    if (query_member_by_id_impl(id) != NULL)
    {
      printf("ID %d 已存在，请重新输入。\n", id);
      sleep_millis(1000);
      exists = 1;
    }
  } while (exists);

  while (1)
  {
    printf("请输入会员年龄：");
    scanf("%d", &age);
    flush_input();

    if (age <= 14)
    {
      printf("年龄太小，这种小登不能入住我们长者社区。请重新输入。\n");
    }
    else if (age > 150)
    {
      printf("我去，究极老登！别瞎输入了。请重新输入。\n");
    }
    else
    {
      break;
    }
  };

  printf("请输入会员姓名：");
  fgets(name, sizeof(name), stdin);
  remove_line_feed_chars(name);
  process_gbk_input(name);

  // 把输入的会员插入链表，并返回新的以巴
  tail = member_list_append(tail, id, age, name);

  printf("会员 %s 已添加。\n是否继续添加？（y/N）", name);
  char c;
  scanf("%c", &c);
  flush_input();
  if (tolower(c) == 'y')
  {
    printf("\n");
    create_member_menu(tail);
  }
}

void query_member_menu()
{
  // 查询会员。
  // 由于链表是线性的数据结构，因此只能使用线性查找
  int choice;
  do
  {
    choice = -1;
    while (choice == -1)
    {
      clear_screen();
      print_subtitle("查询会员");

      print_menu(MEMBERS_QUERY_MENU, MEMBERS_QUERY_MENU_SELECTIONS,
                 MEMBERS_QUERY_MENU_SIZE);
      choice =
          menu_choose(MEMBERS_QUERY_MENU_SELECTIONS, MEMBERS_QUERY_MENU_SIZE);
      flush_input();
    }
    // 功能选择完毕
    // 采用分支结构处理功能
    switch (choice)
    {
    case 1:
      query_member_by_id_menu();
      break;
    case 2:
      query_member_by_name_menu();
      break;
    case 3:
      print_all_members_menu();
      break;
    };
  } while (choice != 0);
}

void query_member_by_id_menu()
{
  // 按 ID 查询会员
  int id;
  printf("请输入会员 ID：");
  scanf("%d", &id);
  flush_input();
  MemberNode *p = query_member_by_id_impl(id);
  if (p == NULL)
  {
    printf("\n未找到 ID 为 %d 的会员。\n", id);
    printf("按回车键返回。");
    getchar();
  }
  else
  {
    printf("\n找了到 ID 为 %d 的会员：\n", id);
    printf("会员姓名：%s\n", p->name);
    printf("会员年龄：%d\n", p->age);
    printf("\n");
    printf("是否要修改或删除这位会员？（Y/n）");

    char c;
    scanf("%c", &c);

    if (tolower(c) != 'n')
    {
      modify_member_menu(p);
    }
  }
}

void query_member_by_name_menu()
{
  // 按姓名查询会员
  char name[40];
  printf("请输入会员的姓名 / 姓氏：");
  fgets(name, sizeof(name), stdin);
  remove_line_feed_chars(name);
  flush_input();
  process_gbk_input(name);
  MemberNode *results = query_members_by_name_impl(name);
  if (results->next == NULL)
  {
    printf("未找到姓名 / 姓氏为 %s 的会员。\n", name);
    printf("按回车键返回。");
    getchar();
  }
  else
  {
    clear_screen();
    print_subtitle("搜索结果");
    TraverseReturns returns = member_list_print(results, -1);
    printf("\n");
    printf("共找到 %d 位姓名 / 姓氏为 %s 的会员。\n", returns.count, name);
    enter_to_modify();
  }
}

void print_all_members_menu()
{
  // 打印所有会员
  clear_screen();
  print_subtitle("所有会员");
  TraverseReturns returns = member_list_print(member_list, -1);
  printf("\n");
  printf("本社区共居住了 %d 位会员。\n", returns.count);
  enter_to_modify();
}
void enter_to_modify()
{
  printf("输入一位会员的 ID 以修改或删除，输入 0 返回。\n");
  int id;

  while (1)
  {
    printf("> ");
    scanf("%d", &id);
    flush_input();
    if (id == 0)
    {
      break;
    }
    MemberNode *p = query_member_by_id_impl(id);
    if (p == NULL)
    {
      printf("未找到 ID 为 %d 的会员，请重新输入 ID。\n> ", id);
    }
    else
    {
      modify_member_menu(p);
      break;
    }
  }
}
MemberNode *query_member_by_id_impl(int id)
{
  // 按 ID 从 member_list 查找会员
  MemberNode *p = member_list->next;
  while (p != NULL)
  {
    if (p->id == id)
    {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

MemberNode *query_members_by_name_impl(char *name)
{
  // 按姓名 / 姓氏从 member_list 查找会员
  // 返回一个链表，存储了所有符合条件的会员
  MemberNode *return_list = member_list_create();
  MemberNode *tail = return_list;

  MemberNode *p = member_list->next;
  while (p != NULL)
  {
    if (with_prefix(p->name, name))
    {
      tail = member_list_append(tail, p->id, p->age, p->name);
    }
    p = p->next;
  }

  return return_list;
}

int serialize_members_to_file(FILE *fp, MemberNode *head)
{
  // 使用 CSV 格式序列化会员数据
  if (head == NULL)
    return 0;
  MemberNode *p = head->next;
  if (head == NULL)
    return 0;
  int count = 0;
  fputs("id,age,name\n", fp);
  while (p != NULL)
  {
    fprintf(fp, "%d,%d,%s\n", p->id, p->age, p->name);
    p = p->next;
    count++;
  };
  return count;
}

int deserialize_members_from_file(FILE *fp)
{
  member_list = member_list_create();
  MemberNode *tail = member_list;
  int count = 0;
  // 从 CSV 文件中反序列化会员数据
  char buf[100];       // name 的最大长度为 40，因此 100 绰绰有余
  fgets(buf, 100, fp); // 读取第一行，跳过
  while (fgets(buf, 100, fp) != NULL)
  {
    int id, age;
    char name[40];
    sscanf(buf, "%d,%d,%[^\n]", &id, &age, name);
    tail = member_list_append(tail, id, age, name);
    count++;
  }
  // 读取完毕
  return count;
}

void modify_member_menu(MemberNode *p)
{
  // 修改会员信息菜单
  // p 为当前选中的会员的指针
  int choice;

  do
  {
    choice = -1;
    while (choice == -1)
    {
      clear_screen();
      print_subtitle("修改会员信息");
      printf("当前选中的会员信息：\n");
      printf("ID：%d\n", p->id);
      printf("姓名：%s\n", p->name);
      printf("年龄：%d\n", p->age);
      printf("\n");
      print_menu(MEMBERS_MODIFY_MENU, MEMBERS_MODIFY_MENU_SELECTIONS,
                 MEMBERS_MODIFY_MENU_SIZE);
      choice =
          menu_choose(MEMBERS_MODIFY_MENU_SELECTIONS, MEMBERS_MODIFY_MENU_SIZE);
      flush_input();
    }
    char c;
    switch (choice)
    {
    case 1:
      printf("请输入新的姓名：");
      char buf[40];
      fgets(buf, sizeof(buf), stdin);
      remove_line_feed_chars(buf);
      process_gbk_input(buf);
      strcpy(p->name, buf);
      flush_input();
      break;
    case 2:
      printf("请输入新的年龄：");
      scanf("%d", &p->age);
      flush_input();
      break;
    case 3:
      // 再三确认以防老登误删
      printf("!!! 真的要删除这位会员吗？（y/N） !!!");
      scanf("%c", &c);
      flush_input();
      if (tolower(c) == 'y')
      {
        delete_member_by_id(p->id);
        printf("会员已删除，按回车键返回。");
        getchar();
        return;
      }
    }
  } while (choice != 0);
}

void delete_member_by_id(int id)
{
  // 删除会员
  MemberNode *p = member_list;
  while (p->next != NULL)
  {
    if (p->next->id == id)
    {
      MemberNode *to_delete = p->next;
      p->next = to_delete->next;
      free(to_delete);
      return;
    }
    p = p->next;
  }
}

/*
 * 链表排序
 * 基于归并排序实现
 * author:
 * reference:
 * https://algo.itcharge.cn/02.Linked-List/02.Linked-List-Sort/01.Linked-List-Sort/#_5-链表归并排序
 */

void split(MemberNode *source, MemberNode **frontRef, MemberNode **backRef);
MemberNode *merge(MemberNode *a, MemberNode *b, SortType sort_type);

void sort_the_list(MemberNode **head_ref, SortType sort_type)
{

  MemberNode *head = *head_ref;
  if (!head || !head->next)
    return;
  // 我寻思这也不需要排序啊，润了

  MemberNode *a;
  MemberNode *b;

  split(head, &a, &b);
  // 此处，链表已经被分割成 a 和 b 左右两段

  // 递归分割
  sort_the_list(&a, sort_type);
  sort_the_list(&b, sort_type);

  // 递归合并
  *head_ref = merge(a, b, sort_type);
}

// 核心是两步，先分割，再归并

void split(MemberNode *source, MemberNode **front, MemberNode **back)
{
  // 分割环节
  MemberNode *slow = source;
  MemberNode *fast = source->next;

  // 快慢指针，快指针每次移动两步，慢指针每次移动一步
  while (fast)
  {
    fast = fast->next;
    if (fast)
    {
      slow = slow->next;
      fast = fast->next;
    }
  }

  // 链表从中间断开
  *front = source;
  *back = slow->next;
  slow->next = NULL;
}

// 比较函数
int comp_age_asc(MemberNode *a, MemberNode *b) { return a->age <= b->age; }
int comp_age_desc(MemberNode *a, MemberNode *b) { return a->age >= b->age; }
int comp_id(MemberNode *a, MemberNode *b) { return a->id <= b->id; }

MemberNode *merge(MemberNode *a, MemberNode *b, SortType sort_type)
{
  // 归并环节
  if (!a)
    return b;
  if (!b)
    return a;

  MemberNode *result = NULL;

  int comp;
  switch (sort_type)
  {
  case AgeAsc:
    comp = comp_age_asc(a, b);
    break;
  case AgeDesc:
    comp = comp_age_desc(a, b);
    break;
  case Id:
    comp = comp_id(a, b);
    break;
  }

  if (comp)
  {
    result = a;
    result->next = merge(a->next, b, sort_type);
  }
  else
  {
    result = b;
    result->next = merge(a, b->next, sort_type);
  }
  return result;
}

void sort_members_menu()
{
  // 会员排序菜单
  int choice = -1;
  while (choice == -1)
  {
    clear_screen();
    print_subtitle("会员排序");

    print_menu(MEMBERS_SORT_MENU, MEMBERS_SORT_MENU_SELECTIONS,
               MEMBERS_SORT_MENU_SIZE);
    choice = menu_choose(MEMBERS_SORT_MENU_SELECTIONS, MEMBERS_SORT_MENU_SIZE);
    flush_input();
  }

  switch (choice)
  {
  case 1:
    sort_the_list(&(member_list->next), AgeAsc);
    printf("按年龄升序排序完成。\n");
    sleep_millis(1000);
    break;
  case 2:
    sort_the_list(&(member_list->next), AgeDesc);
    printf("按年龄降序排序完成。\n");
    sleep_millis(1000);
    break;
  case 3:
    sort_the_list(&(member_list->next), Id);
    printf("按 ID 排序完成。\n");
    sleep_millis(1000);
    break;
  case 0:
    break;
  }
}

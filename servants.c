#include "servants.h"
#include "ui.h"
#include "menu.h"
#include "utils.h"
#include "members.h"
#include <ctype.h>
#include <stdio.h>

const char *SERVANT_MENU[] = {
    "新增人员", "显示所有人员", "查询人员", "按条件筛选",
    "更新人员", "删除人员", "返回主菜单"};

const int SERVANT_MENU_SIZE = 7;

const int SERVANT_MENU_SELECTIONS[] = {1, 2, 3, 4, 5, 6, 0};

const char *SERVANT_MODIFY_MENU[] = {
    "修改姓名",
    "修改工作状态",
    "修改权限等级",
    "修改部门",
    "修改职务",
    "修改服务对象 (会将工作状态变化为“是”)",
    "退出修改，返回菜单"};

const int SERVANT_MODIFY_MENU_SIZE = 7;

const int SERVANT_MODIFY_MENU_SELECTIONS[] = {1, 2, 3, 4, 5, 6, 0};

Servant *servants = NULL;

void servants_menu()
{
  if (servants == NULL)
  {
    init_list(&servants);
  }
  int choice;

  do
  {
    choice = -1;
    while (choice == -1)
    {
      clear_screen();
      print_subtitle("服务人员管理");

      // 打印菜单
      print_menu(SERVANT_MENU, SERVANT_MENU_SELECTIONS, SERVANT_MENU_SIZE);
      choice = menu_choose(SERVANT_MENU_SELECTIONS, SERVANT_MENU_SIZE);
      flush_input();

      // 分支选择功能
      switch (choice)
      {
      case 1:
        add_person(servants);
        break;
      case 2:
        display_all(servants);
        break;
      case 3:
        search_person(servants);
        break;
      case 4:
        filter_servants(servants);
        break;
      case 5:
        update_person(servants);
        break;
      case 6:
        delete_person(&servants);
        break;
      }
    }
  } while (choice != 0);
}

// 链表初始化
Servant *init_list(Servant **head)
{
  *head = (Servant *)malloc(sizeof(Servant));
  if (!*head)
  {
    perror("内存分配失败");
    return NULL;
  }
  (*head)->next = NULL;
  return *head;
}

// 新增人员
void add_person(Servant *servant_list)
{
  Servant *new_person = (Servant *)malloc(sizeof(Servant));
  if (!new_person)
  {
    perror("内存分配失败");
    return;
  }
  printf("输入新增人员名称: ");

  char buf[40];
  fgets(buf, sizeof(buf), stdin);
  remove_line_feed_chars(buf);
  process_gbk_input(buf);
  strcpy(new_person->name, buf);

  printf("\n输入新增人员编号 (可以是字母或数字): ");
  int exists;
  do
  {
    if (exists)
    {
      printf("\n编号已存在，请重新输入: ");
    }
    scanf("%s", new_person->num);
    exists = check_num_exists(servant_list, new_person->num);
  } while (exists);

  print_permissions();
  do
  {
    printf("\n输入新增人员权限级别 (0-3): ");
    scanf("%d", &new_person->level);
  } while (new_person->level < 0 || new_person->level > 3);

  print_departments();
  do
  {
    printf("\n输入新增人员部门 (0-4): ");
    scanf("%d", &new_person->department);
  } while (new_person->department < 0 || new_person->department > 4);

  print_jobs();
  do
  {
    printf("\n输入新增人员职务 (0, 10, 11, 12, 14): ");
    scanf("%d", &new_person->job);
  } while (
      new_person->job != JOB_TEMPORARY &&
      new_person->job != JOB_GENERAL &&
      new_person->job != JOB_MANAGER &&
      new_person->job != JOB_LOGISTICS &&
      new_person->job != JOB_SECURITY);

  new_person->working = 0;
  new_person->master = -1;
  new_person->next = NULL;

  // 添加到链表尾部
  Servant *temp = servant_list;
  while (temp->next)
  {
    temp = temp->next;
  }
  temp->next = new_person;
}

// 打印单个人员信息
void display_single(const Servant *temp)
{
  printf("%s [%s]", temp->name, temp->num);
  if (temp->working)
  {
    printf(" (工作中");
    if (temp->master != -1)
    {
      MemberNode *master = query_member_by_id_impl(temp->master);
      printf(", 服务对象: %s)\n", master->name);
    }
    else
    {
      printf(")\n");
    }
  }
  else
  {
    printf(" (空闲)\n");
  }
  switch (temp->department)
  {
  case DEPARTMENT_LOGISTICS:
    printf("部门: 后勤部门");
    break;
  case DEPARTMENT_SERVICE:
    printf("部门: 服务部门");
    break;
  case DEPARTMENT_SECURITY:
    printf("部门: 保卫部门");
    break;
  case DEPARTMENT_HR:
    printf("部门: 人事部门");
    break;
  case DEPARTMENT_ADMIN:
    printf("部门: 行政部门");
    break;
  }
  printf("\t");
  switch (temp->job)
  {
  case JOB_TEMPORARY:
    printf("职务: 临时人员");
    break;
  case JOB_GENERAL:
    printf("职务: 一般服务人员");
    break;
  case JOB_MANAGER:
    printf("职务: 管理人员");
    break;
  case JOB_LOGISTICS:
    printf("职务: 后勤人员");
    break;
  case JOB_SECURITY:
    printf("职务: 保卫人员");
    break;
  }
  printf("\t");
  switch (temp->level)
  {
  case LEVEL_EXTERNAL:
    printf("权限等级: 外部人员权限\n");
    break;
  case LEVEL_STAFF:
    printf("权限等级: 一般工作人员权限\n");
    break;
  case LEVEL_MANAGER:
    printf("权限等级: 管理员权限\n");
    break;
  case LEVEL_ADMIN:
    printf("权限等级: 最高权限\n");
    break;
  }
}

// 显示所有人员信息
void display_all(const Servant *servant_list)
{
  const Servant *temp = servant_list->next;
  if (!temp)
  {
    printf("当前没有人员记录。\n");
    printf("按下回车键返回。");
    getchar();
    return;
  }
  printf("人员信息列表:\n");
  while (temp)
  {
    printf("- ");
    display_single(temp);
    temp = temp->next;
  }
  // 打印会员完毕，等待用户返回
  printf("按下回车键返回。");
  getchar();
}

// 查询人员信息
void search_person(const Servant *servant_list)
{
  char query[40];
  printf("输入要查询的人员姓名或编号: ");
  fgets(query, sizeof(query), stdin);
  remove_line_feed_chars(query);
  process_gbk_input(query);

  const Servant *temp = servant_list->next;
  while (temp)
  {
    if (with_prefix(temp->name, query) || strcmp(temp->num, query) == 0)
    {
      display_single(temp);
      printf("\n是否继续查找可能重名的人员？（y/N）");
      char c;
      scanf("%c", &c);
      if (tolower(c) == 'y')
      {
        continue;
      }
      else
      {
        return;
      }
    }
    temp = temp->next;
  }
  printf("未找到对应的人员。\n");
  printf("按下回车键返回。");
  getchar();
}

// 检查编号是否存在
int check_num_exists(Servant *servant_list, char *num)
{
  Servant *temp = servant_list->next;
  while (temp)
  {
    if (strcmp(temp->num, num) == 0)
    {
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}

// 更新人员信息
void update_person(Servant *servant_list)
{
  char query[15];
  printf("输入要修改的人员姓名或编号: ");
  scanf("%s", query);

  Servant *temp = servant_list->next;
  while (temp)
  {
    if (with_prefix(temp->name, query) || strcmp(temp->num, query) == 0)
    {
      int choice;
      do
      {
        // 找到了要修改的人员
        clear_screen();
        print_subtitle("修改服务人员信息");
        printf("找到人员：");
        display_single(temp);

        choice = -1;
        // 打印三级菜单
        print_menu(SERVANT_MODIFY_MENU, SERVANT_MODIFY_MENU_SELECTIONS, SERVANT_MODIFY_MENU_SIZE);
        choice = menu_choose(SERVANT_MODIFY_MENU_SELECTIONS, SERVANT_MODIFY_MENU_SIZE);
        flush_input();
        switch (choice)
        {
        case 1:
          printf("输入新的姓名: ");
          char buf[40];
          fgets(buf, sizeof(buf), stdin);
          remove_line_feed_chars(buf);
          process_gbk_input(buf);
          strcpy(temp->name, buf);
          printf("修改成功，新的名字为 %s。\n", temp->name);
          break;
        case 2:
          do
          {
            printf("\n修改工作状态 (0: 无任务, 1: 有任务): ");
            scanf("%d", &temp->working);
          } while (temp->working != 0 && temp->working != 1);
          printf("修改成功。\n");
          break;
        case 3:
          print_permissions();
          do
          {
            printf("\n修改权限等级 (0-3): ");
            scanf("%d", &temp->level);
          } while (temp->level < 0 || temp->level > 3);
          printf("修改成功。\n");
          break;
        case 4:
          print_departments();
          do
          {
            printf("\n修改部门 (0-4): ");
            scanf("%d", &temp->department);
          } while (temp->department < 0 || temp->department > 4);
          printf("修改成功。\n");
          break;
        case 5:
          print_jobs();
          do
          {
            printf("\n修改职务 (0, 10, 11, 12, 14): ");
            scanf("%d", &temp->job);
          } while (
              temp->job != JOB_TEMPORARY &&
              temp->job != JOB_GENERAL &&
              temp->job != JOB_MANAGER &&
              temp->job != JOB_LOGISTICS &&
              temp->job != JOB_SECURITY);
          printf("修改成功。\n");
          break;
        case 6:
          // 这里是设置服务的老灯的代码
          // 可能会很繁琐
          MemberNode *master = NULL;
          do
          {
            int master_id;
            printf("输入服务对象的会员 ID: ");
            scanf("%d", &master_id);
            flush_input();
            master = query_member_by_id_impl(master_id);
            if (master == NULL)
            {
              printf("未找到会员。\n");
              sleep_millis(1000);
            }
          } while (master == NULL);

          // 已经找到了老灯
          temp->master = master->id;
          temp->working = 1;
          printf("服务对象已设置为 %s。\n", master->name);
          break;
        }
        if (choice != 0)
          sleep_millis(1000);
      } while (choice != 0);
      return;
    }
    temp = temp->next;
  }
  printf("未找到对应的人员。\n");
  printf("按下回车键返回。");
  getchar();
}

// 删除人员
void delete_person(Servant **servant_list)
{
  char query[40];
  printf("输入要删除的人员姓名或编号: ");
  fgets(query, sizeof(query), stdin);
  remove_line_feed_chars(query);
  process_gbk_input(query);
  flush_input();

  Servant *temp = *servant_list;
  Servant *prev = NULL;
  while (temp)
  {
    if (with_prefix(temp->name, query) || strcmp(temp->num, query) == 0)
    {
      printf("找到人员：");
      printf("姓名: %s\n编号: %s\n权限等级: %d\n部门: %d\n职务: %d\n\n",
             temp->name, temp->num, temp->level, temp->department, temp->job);
      printf("是否删除？（y/N）");
      char c;
      scanf("%c", &c);
      if (tolower(c) == 'y')
      {
        if (prev)
        {
          prev->next = temp->next;
        }
        else
        {
          *servant_list = temp->next;
        }
        free(temp);
        printf("删除成功。\n");
      }
      else
      {
        printf("取消删除。\n");
      }
      printf("按下回车键返回。");
      getchar();
      return;
    }
    prev = temp;
    temp = temp->next;
  }
  printf("未找到对应的人员。\n");
  printf("按下回车键返回。");
  getchar();
}

// 按条件
void filter_servants(Servant *servant_list)
{
  int choice;
  do
  {
    choice = -1;
    while (choice == -1)
    {
      clear_screen();
      print_subtitle("按条件筛选服务人员");
      printf("1. 按部门筛选\n");
      printf("2. 按职务筛选\n");
      printf("3. 按权限等级筛选\n");
      printf("0. 返回\n");
      printf("请选择：");
      scanf("%d", &choice);
      flush_input();
      switch (choice)
      {
      case 1:
        filter_by_department(servant_list);
        break;
      case 2:
        filter_by_job(servant_list);
        break;
      case 3:
        filter_by_level(servant_list);
        break;
      }
    }
  } while (choice != 0);
}

void filter_by_department(Servant *servant_list)
{
  int department;
  print_departments();
  printf("请选择部门：");
  scanf("%d", &department);
  flush_input();
  int count = 0;
  Servant *temp = servant_list->next;
  while (temp)
  {
    if (temp->department == department)
    {
      display_single(temp);
      count++;
    }
    temp = temp->next;
  }
  if (count > 0)
  {
    printf("共找到 %d 位部门为 %d 的人员。\n", count, department);
  }
  else
  {
    printf("未找到部门为 %d 的人员。\n", department);
  }
  printf("按下回车键返回。");
  getchar();
}

void filter_by_job(Servant *servant_list)
{
  int job;
  print_jobs();
  printf("请选择职务：");
  scanf("%d", &job);
  flush_input();
  int count = 0;
  Servant *temp = servant_list->next;
  while (temp)
  {
    if (temp->job == job)
    {
      display_single(temp);
      count++;
    }
    temp = temp->next;
  }
  if (count > 0)
  {
    printf("共找到 %d 位职务为 %d 的人员。\n", count, job);
  }
  else
  {
    printf("未找到职务为 %d 的人员。\n", job);
  }
  printf("按下回车键返回。");
  getchar();
}

void filter_by_level(Servant *servant_list)
{
  int level;
  print_permissions();
  printf("请选择权限等级：");
  scanf("%d", &level);
  flush_input();
  int count = 0;
  Servant *temp = servant_list->next;
  while (temp)
  {
    if (temp->level == level)
    {
      display_single(temp);
      count++;
    }
    temp = temp->next;
  }
  if (count > 0)
  {
    printf("共找到 %d 位权限等级为 %d 的人员。\n", count, level);
  }
  else
  {
    printf("未找到权限等级为 %d 的人员。\n", level);
  }
  printf("按下回车键返回。");
  getchar();
}

void print_jobs()
{
  printf("可设置的人员职务如下:\n");
  printf("0: 临时人员\n"
         "10: 后勤人员\n"
         "11: 一般服务人员\n"
         "12: 保卫人员\n"
         "14: 管理人员\n");
}

void print_departments()
{

  printf("可设置的人员部门如下:\n");
  printf("0: 后勤部门\n"
         "1: 服务部门\n"
         "2: 保卫部门\n"
         "3: 人事部门\n"
         "4: 行政部门\n");
}

void print_permissions()
{
  printf("可设置的人员权限级别如下:\n");
  printf("0: 外部人员权限\n"
         "1: 一般工作人员权限\n"
         "2: 管理员权限\n"
         "3: 最高权限\n");
}

int serialize_servants_to_file(FILE *fp, Servant *servant_list)
{
  // 将服务人员信息序列化到文件
  if (servant_list == NULL)
    return 0;
  int count = 0;
  fprintf(fp, "name,num,master,level,working,job,department\n");
  Servant *temp = servant_list->next;
  while (temp)
  {
    fprintf(fp, "%s,%s,%d,%d,%d,%d,%d\n",
            temp->name, temp->num, temp->master, temp->level, temp->working, temp->job, temp->department);
    temp = temp->next;
    count++;
  }
  return count;
}

int deserialize_servants_from_file(FILE *fp)
{
  // 从文件中反序列化服务人员信息
  char buf[1024];
  int count = 0;
  init_list(&servants);
  fgets(buf, sizeof(buf), fp); // 读取第一行，跳过
  Servant *tail = servants;
  while (fgets(buf, sizeof(buf), fp) != NULL)
  {
    Servant *new_person = (Servant *)malloc(sizeof(Servant));
    if (!new_person)
    {
      perror("内存分配失败");
      return -1;
    }
    sscanf(buf, "%[^,],%[^,],%d,%d,%d,%d,%d",
           new_person->name, new_person->num, &new_person->master, &new_person->level, &new_person->working, &new_person->job, &new_person->department);
    new_person->next = NULL;
    tail->next = new_person;
    tail = tail->next;
    count++;
  }
  return count;
}
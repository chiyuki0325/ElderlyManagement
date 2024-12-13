#include "members.h"
#include "house.h"
#include "menu.h"
#include "venue.h"
#include "bus.h"
#include "ui.h"
#include "servants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * save.c
 * 保存数据菜单及调用
 * 保存每种数据的代码在各自的 .c 代码中分别实现
 * 支持 Windows 和 Linux 的配置文件目录
 * 采用 Unix (LF) 行尾，UTF-8 编码的 CSV 格式保存数据
 */

const char *SAVE_MENU[] = {"保存数据", "载入数据", "返回主菜单"};

const int SAVE_MENU_SIZE = 3;
const int SAVE_MENU_SELECTIONS[] = {1, 2, 0};

void save_state();
void load_state(int quiet);

void save_menu()
{
  // 保存数据菜单
  int choice = -1;
  while (choice == -1)
  {
    clear_screen();
    print_subtitle("保存数据");
    print_menu(SAVE_MENU, SAVE_MENU_SELECTIONS, SAVE_MENU_SIZE);
    choice = menu_choose(SAVE_MENU_SELECTIONS, SAVE_MENU_SIZE);
    flush_input();
  }
  // 功能选择完毕
  // 采用分支结构处理功能
  switch (choice)
  {
  case 1:
    save_state();
    sleep_millis(1000);
    break;
  case 2:
    load_state(0);
    sleep_millis(1000);
    break;
  }
}

char *get_config_folder()
{
// 获取配置文件的路径
// 涉及到字符串操作，这里使用 sprintf，比 strcat 方便些
#ifdef _WIN32
  // Windows，放在 AppData/Roaming/ElderlyManagement
  char *folder = getenv("APPDATA");
  char *path = (char *)malloc(strlen(folder) + 22);
  sprintf(path, "%s\\ElderlyManagement\\", folder);
#else
  // Linux，放在 $XDG_CONFIG_HOME/ElderlyManagement
  char *folder = getenv("XDG_CONFIG_HOME");
  char *path;
  if (folder == NULL)
  {
    folder = getenv("HOME");
    path = (char *)malloc(strlen(folder) + 34); // 22 显然不够长
    sprintf(path, "%s/.config/ElderlyManagement/", folder);
  }
  else
  {
    path = (char *)malloc(strlen(folder) + 22);
    sprintf(path, "%s/ElderlyManagement/", folder);
  }
#endif

// 检测文件夹是否存在
#ifdef _WIN32
  // Windows
  system("if not exist %APPDATA%\\ElderlyManagement "
         "mkdir %APPDATA%\\ElderlyManagement");
#else
  // Linux
  system("mkdir -p ${XDG_CONFIG_HOME:-$HOME/.config}/ElderlyManagement");
#endif

  return path;
}

void save_state()
{
  FILE *fp;
  char *folder = get_config_folder();
  char *filename;
  int count;

  // 储存会员数据
  filename = (char *)malloc(strlen(folder) + 12);
  sprintf(filename, "%smembers.csv", folder);
  fp = fopen(filename, "w");
  if (fp == NULL)
  {
    perror("无法保存会员数据");
    return;
  }
  count = serialize_members_to_file(fp, member_list);
  fclose(fp);
  printf("保存了 %d 位会员数据。\n", count);

  // 储存房屋数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 11);
  sprintf(filename, "%shouses.csv", folder);
  fp = fopen(filename, "w");
  if (fp == NULL)
  {
    perror("无法保存房屋数据");
    return;
  }
  count = serialize_houses_to_file(fp, houses);
  fclose(fp);
  printf("保存了 %d 栋房屋数据。\n", count);

  // 储存车站数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 10);
  sprintf(filename, "%sbuses.csv", folder);
  fp = fopen(filename, "w");
  if (fp == NULL)
  {
    perror("无法保存班车车站数据");
    return;
  }
  count = serialize_bus_stops_to_file(fp, bus_routine);
  fclose(fp);
  printf("保存了 %d 个班车车站数据。\n", count);

  // 储存场馆设施和保洁人员数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 11);
  sprintf(filename, "%svenues.csv", folder);
  fp = fopen(filename, "w");

  count = serialize_venues_to_file(fp, venuesList);
  fclose(fp);
  printf("保存了 %d 个场馆数据。\n", count);
  if (fp == NULL)
  {
    perror("无法保存场馆数据");
    return;
  }
  free(filename);
  filename = (char *)malloc(strlen(folder) + 13);
  sprintf(filename, "%scleaners.csv", folder);
  fp = fopen(filename, "w");
  if (fp == NULL)
  {
    perror("无法保存保洁人员数据");
    return;
  }
  count = serialize_cleaners_to_file(fp, cleanersList);
  fclose(fp);
  printf("保存了 %d 个保洁人员数据。\n", count);

  // 储存服务人员数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 13);
  sprintf(filename, "%sservants.csv", folder);
  fp = fopen(filename, "w");
  if (fp == NULL)
  {
    perror("无法保存服务人员数据");
    return;
  }
  count = serialize_servants_to_file(fp, servants);
  fclose(fp);
  printf("保存了 %d 个服务人员数据。\n", count);

  // 最后，你自由了！
  free(filename);
}

void load_state(int quiet)
{
  FILE *fp;
  char *folder = get_config_folder();
  char *filename;
  int count;

  // 加载会员数据
  filename = (char *)malloc(strlen(folder) + 12);
  sprintf(filename, "%smembers.csv", folder);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    count = deserialize_members_from_file(fp);
    fclose(fp);
    if (!quiet)
      printf("载入了 %d 位会员数据。\n", count);
  }
  else
  {
    if (!quiet)
      printf("未找到会员数据文件。\n");
  }

  // 加载房屋数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 11);
  sprintf(filename, "%shouses.csv", folder);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    count = deserialize_houses_from_file(fp);
    fclose(fp);
    if (!quiet)
      printf("载入了 %d 栋房屋数据。\n", count);
  }
  else
  {
    if (!quiet)
      printf("未找到房屋数据文件。\n");
  }

  // 加载班车数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 10);
  sprintf(filename, "%sbuses.csv", folder);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    count = deserialize_bus_stops_from_file(fp);
    fclose(fp);
    if (!quiet)
      printf("载入了 %d 个班车车站数据。\n", count);
  }
  else
  {
    if (!quiet)
      printf("未找到班车车站数据文件。\n");
  }

  // 加载场馆设施和保洁人员数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 11);
  sprintf(filename, "%svenues.csv", folder);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    count = deserialize_venues_from_file(fp);
    fclose(fp);
    if (!quiet)
      printf("载入了 %d 个场馆数据。\n", count);
  }
  else
  {
    if (!quiet)
      printf("未找到场馆数据文件。\n");
  }

  free(filename);
  filename = (char *)malloc(strlen(folder) + 13);
  sprintf(filename, "%scleaners.csv", folder);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    count = deserialize_cleaners_from_file(fp);
    fclose(fp);
    if (!quiet)
      printf("载入了 %d 个保洁人员数据。\n", count);
  }
  else
  {
    if (!quiet)
      printf("未找到保洁人员数据文件。\n");
  }

  // 加载服务人员数据
  free(filename);
  filename = (char *)malloc(strlen(folder) + 13);
  sprintf(filename, "%sservants.csv", folder);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    count = deserialize_servants_from_file(fp);
    fclose(fp);
    if (!quiet)
      printf("载入了 %d 个服务人员数据。\n", count);
  }
  else
  {
    if (!quiet)
      printf("未找到服务人员数据文件。\n");
  }

  // 最后，你自由了！
  free(filename);
}
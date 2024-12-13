#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ui.c
 * 存放一些 UI 相关函数
 * 特别地，部分函数需要根据操作系统分别实现
 */

#ifdef _WIN32
#include <windows.h> // Windows
#else
#include <unistd.h> // Linux
#endif

void print_title()
{
  // 打印项目标题，用于主屏幕或不带二级标题的屏幕
  printf("=======================\n");
  printf(" 活力长者社区管理系统 \n");
  printf("=======================\n\n");
}

void print_subtitle(char *subtitle)
{
  // 打印带副标题的标题，用于二级屏幕
  int i;
  int sublength = strlen(subtitle);
  printf("==========================");
  for (i = 0; i < sublength; i++)
  {
    printf("=");
  }
  printf("\n");
  printf(" 活力长者社区管理系统 - %s \n", subtitle);
  printf("==========================");
  for (i = 0; i < sublength; i++)
  {
    printf("=");
  }
  printf("\n\n");
}

void set_utf8()
{
// 在 Windows 下设置控制台编码为 UTF-8
// 由于项目采用 UTF-8 编码，所以需要设置
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  // Linux 下什么也不做即可
}

void clear_screen()
{
// 清空屏幕
#ifdef _WIN32
  system("cls");
#else
  // Linux
  system("clear");
#endif
}

void flush_input()
{
  // author:
  // Fixed by ChatGPT
  int ch = getchar();
  if (ch != '\n' && ch != EOF)
  {
    ungetc(ch, stdin); // 将字符放回缓冲区
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
      // 清空缓冲区
    }
  }
}

void sleep_millis(int millis)
{
  // 程序睡眠
  // 不命名为 sleep 是因为 unistd.h 中已经有 sleep
#ifdef _WIN32
  Sleep(millis);
#else
  usleep(millis * 1000);
#endif
}

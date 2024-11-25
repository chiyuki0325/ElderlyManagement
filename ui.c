#include <stdio.h>
#include <stdlib.h>

/*
 * ui.c
 * 存放一些 UI 相关函数
 * 特别地，部分函数需要根据操作系统分别实现
 */

#ifdef _WIN32
#include <windows.h>
#endif

void print_title() {
  // 打印项目标题
  printf("==================\n");
  printf(" 活力长者社区管理系统 \n");
  printf("==================\n\n");
}

void set_utf8() {
// 在 Windows 下设置控制台编码为 UTF-8
// 由于项目采用 UTF-8 编码，所以需要设置
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
}

void clear_screen() {
// 清空屏幕
#ifdef _WIN32
  system("cls");
#else
  // Linux
  system("clear");
#endif
}

void flush_input() {
// 清理输入缓冲区
#ifdef _WIN32
  fflush(stdin);
#else
  while (getchar() != '\n')
    ;
#endif
}
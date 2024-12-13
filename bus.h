#ifndef BUS_H
#define BUS_H

#include <stdio.h>

typedef struct bus_stop // 定义了车站
{
  char name[50];
  struct bus_stop *next;
} BUS;
typedef struct BUS_TraverseReturns
{
  // BUS_list_print 函数返回两个值
  // 故为其建立结构体
  int count;
  BUS *tail;
} ATraverseReturns;
extern BUS* bus_routine;
extern const char *BUS_MENU[];
extern const int BUS_MENU_SIZE;
extern const int BUS_MENU_LENGTH[];
extern const char *BUS_INPUT_LIST[];
extern const int BUS_INPUT_LENGTH[];
extern const int BUS_INPUT_SIZE;
void bus_menu();
void BUS_STOP_JIA(BUS *head, BUS *tail);
void BUS_STOP_JIAN(BUS *head, BUS *tail);
BUS *creat_a_stop();
void link_them(BUS *pre, BUS *after);
BUS *Find_the_stop(BUS *head, BUS *tail);
void insert_a_stop(BUS *the_stop_you_creat, BUS *head);
void delete_a_stop(BUS *the_stop_youwant_delete, BUS *head);
ATraverseReturns BUS_list_print(BUS *head, int max_prints);
BUS *BUS_list_create();

int serialize_bus_stops_to_file(FILE *fp, BUS *head);
int deserialize_bus_stops_from_file(FILE *fp);
#endif // BUS_H
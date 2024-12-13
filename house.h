#ifndef HOUSE_H
#define HOUSE_H

#include <stdio.h>
typedef struct House {
  int id;
  char address[100];
  char basic_info[200];
  char purchase_info[200];
  char usage_info[200];
  int used_by;
  // 由谁购买或出租给谁的 ID
  int status;
  // 房屋状态：0 - 空置，1 - 自住，2 - 出租
} House;

extern House houses[100];

void houses_mgmt_menu();
int serialize_houses_to_file(FILE *fp, House *houses);
int deserialize_houses_from_file(FILE *fp);

#endif
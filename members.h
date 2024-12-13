#ifndef MEMBERS_H
#define MEMBERS_H 1

#include <stdio.h>

// 采用链表存储会员信息
typedef struct MemberNode {
  int id;
  char name[40];
  int age;
  struct MemberNode *next;
} MemberNode;

typedef struct TraverseReturns {
  // member_list_print 函数返回两个值
  // 故为其建立结构体
  int count;
  MemberNode *tail;
} TraverseReturns;

typedef enum {
  AgeAsc = 0,
  AgeDesc,
  Id,
} SortType;

extern MemberNode *member_list;

void members_mgmt_menu();
int serialize_members_to_file(FILE *fp, MemberNode *head);
int deserialize_members_from_file(FILE *fp);


MemberNode *query_member_by_id_impl(int id);
MemberNode *query_members_by_name_impl(char *name);

#endif // MEMBERS_H
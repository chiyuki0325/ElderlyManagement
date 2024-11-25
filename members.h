#ifndef MEMBERS_H
#define MEMBERS_H

// 采用链表存储会员信息
typedef struct MemberNode {
  int id;
  char name[40];
  MemberNode *next;
} MemberNode;

#endif // MEMBERS_H
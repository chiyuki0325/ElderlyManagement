#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "members.h"

/*
 * members.c
 * 会员管理相关代码
 * 使用链表数据结构存储会员
 * 使用线性查找查询
 */

MemberNode* member_list_create() {
    // 创建空链表, head 不存储数据
    MemberNode *head = (MemberNode *)malloc(sizeof(MemberNode));
    head->id = -1;
    head->next = NULL;
    return head;
}

void member_list_append(MemberNode *head, int id, char name[]) {
    // 在链表末尾添加节点
    MemberNode *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    MemberNode *new_node = (MemberNode *)malloc(sizeof(MemberNode));
    new_node->id = id;
    strcpy(new_node->name, name);
    new_node->next = NULL;
    p->next = new_node;
}

void member_list_print(MemberNode *head) {
    // 打印链表
    MemberNode *p = head->next;
    while (p != NULL) {
        printf(" - [%d] %s\n", p->id, p->name);
        p = p->next;
    }
}

MemberNode* member_list_find(MemberNode *head, int id) {
    // 查找成员
    MemberNode *p = head->next;
    while (p != NULL) {
        if (p->id == id) {
            return p;
        }
        p = p->next;
    }
    return NULL;
    // 谁也没找到 ...
}

void members_mgmt() {
    // 会员管理菜单
    // 由主菜单进入
}
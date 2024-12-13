#ifndef VENUE_H
#define VENUE_H

#include <stdio.h>

// 定义场馆结构体
typedef struct Venue {
    char name[50];  // 场馆名称
    int capacity;   // 容纳人数
    int isAvailable; // 0表示封闭，1表示可用
    int maintenancePeriod; // 距离下次维修维护的时间（天），可根据实际情况调整单位和含义
    struct Venue *next; // 指向下一个场馆的指针
} Venue;

// 保洁人员结构体
typedef struct Cleaner {
    char name[50];
    char assignedVenue[50]; // 负责的场馆名称
    struct Cleaner *next;
} Cleaner;

// 保洁人员列表头指针
extern Cleaner *cleanersList;

// 场馆列表头指针
extern Venue *venuesList;

void venue_mgmt_menu();

int serialize_venues_to_file(FILE *fp, Venue *head);
int deserialize_venues_from_file(FILE *fp);
int serialize_cleaners_to_file(FILE *fp, Cleaner *head);
int deserialize_cleaners_from_file(FILE *fp);

#endif
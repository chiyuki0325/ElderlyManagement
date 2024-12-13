#ifndef SERVANT_H
#define SERVANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 数据结构定义
typedef struct Servant {
    char name[40];
    char num[10];
    int master;
    int level;
    int working;
    int job;
    int department;
    struct Servant* next;
} Servant;

// 枚举定义
typedef enum {
    DEPARTMENT_LOGISTICS = 0,
    DEPARTMENT_SERVICE,
    DEPARTMENT_SECURITY,
    DEPARTMENT_HR,
    DEPARTMENT_ADMIN
} Department;

typedef enum {
    JOB_TEMPORARY = 0,
    JOB_GENERAL = 11,
    JOB_MANAGER = 14,
    JOB_LOGISTICS = 10,
    JOB_SECURITY = 12
} Job;

typedef enum {
    LEVEL_EXTERNAL = 0,
    LEVEL_STAFF,
    LEVEL_MANAGER,
    LEVEL_ADMIN
} AccessLevel;


/*
对于level，0是外部人员权限，1是一般工作人员权限，2是管理员权限,3是最高权限
对于working，0表示无任务，1表示有任务
对于job，0表示临时人员，11表示一般服务人员，14表示管理人员，10表示后勤人员，12表示保卫人员，-1表示暂时离职人员
对于type，0表示后勤部门，1表示服务部门，2表示保卫部门，3表示人事部门，4表示行政部门，
*/

// 全局变量
extern Servant* servants;

// 函数声明
Servant* init_list(Servant** head);
void add_person(Servant* head);
void display_all(const Servant* head);
void search_person(const Servant* head);
void update_person(Servant* head);
void delete_person(Servant** head);
void servants_menu();
void print_jobs();
void print_departments();
void print_permissions();
int check_num_exists(Servant *servant_list, char *num);
int serialize_servants_to_file(FILE *fp, Servant *_servant_list);
int deserialize_servants_from_file(FILE *fp);

void filter_servants(Servant* servant_list);
void filter_by_department(Servant* servant_list);
void filter_by_job(Servant* servant_list);
void filter_by_level(Servant* servant_list);

#endif
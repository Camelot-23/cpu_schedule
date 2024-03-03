//
// Created by ELOT on 11/23/2023.
//

#ifndef CPU_SCHEDULE_RR_H
#define CPU_SCHEDULE_RR_H

#include <task.h>

struct RData {
    Task *task;
    struct RData *next;
};

typedef struct RData Rd;

// 向链表尾添加一个节点
void addTail(Rd *head,  Rd *rd);
// just show
void showRd(Rd *head);
// 删除第一个节点并将其返回
Rd *getFirst(Rd *head);

void rr(Tasks *tasks, char *s);

#endif //CPU_SCHEDULE_RR_H

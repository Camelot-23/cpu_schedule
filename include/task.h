//
// Created by ELOT on 11/22/2023.
//

#ifndef CPU_SCHEDULE_TASK_H
#define CPU_SCHEDULE_TASK_H

typedef char bool;
#define true  1
#define false 0

#define TASK_NAME_LEN 8

typedef struct {
    char    name[TASK_NAME_LEN];    // 任务名称
    int     arrivalTime;            // 到达时间
    int     needTime;               // 需要执行时间

    int     startTime;              // 开始时间
    int     endTime;                // 结束时间
    int     runTime;                // 已执行时间
    int     remainingTime;          // 剩余时间
    int     waitTime;               // 等待时间

    bool    inQueue;                // 对于RR，表示该任务是否在队列里
} Task;

// 初始化一个任务
void initTask(Task *task);
// 设置一个任务
void setTask(Task *task, const char *name, int arrivalTime, int needTime);
// 打印一个任务
void showTask(const Task *task);
// 计算任务的等待时间 wait = end - arrival - need
// 任务必须已经执行完毕
void calcWaitTime(Task *task);

// 最大任务长度
#define MAX_TASK_LEN 100

typedef struct {
    Task    tasks[MAX_TASK_LEN];    // 任务表
    int     tasksLen;               // 任务数量
} Tasks;

// 初始化多个任务
void initTasks(Task *task, int taskLen);
// 打印多个任务
void showTasks(const Task *task, int taskLen);
// 统计任务等待时间和平均等待时间
void showWaitTime(const Task *task, int taskLen, char *s);
// 计算任务等待时间
void calcWaitTimes(Tasks *tasks);

#endif //CPU_SCHEDULE_TASK_H

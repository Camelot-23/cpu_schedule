//
// Created by ELOT on 11/22/2023.
//

#include <task.h>

#include <string.h>
#include <stdio.h>

#define SHOW_TASK_MESSAGE 1

void initTask(Task *task) {
    if (!task) {
        printf("initTask: task(%p) cannot be null!", task);
        return;
    }

    memset(task->name, 0, sizeof(char) * TASK_NAME_LEN);
    task->arrivalTime   = 0;
    task->needTime      = 0;

    task->startTime     = -1;
    task->endTime       = 0;
    task->runTime       = 0;
    task->remainingTime = 0;
    task->waitTime      = 0;

    task->inQueue       = false;
}

void setTask(Task *task, const char *name, int arrivalTime, int needTime) {
    if (!task || !name) {
        printf("setTask: task(%p) and name(%p) can't be null!", task, name);
        return;
    }

    strncpy(task->name, name, TASK_NAME_LEN);
    task->arrivalTime   = arrivalTime;
    task->needTime      = needTime;
    task->remainingTime = needTime;
}

void showTask(const Task *task) {
    if (!task) {
        printf("showTask: task(%p) cannot be null!", task);
        return;
    }

    printf("Name: %-4s Arrival: %-4d Need: %-4d Start: %-4d End: %-4d Run: %-4d Remaining: %-4d " "Wait: %-4d\n",
           task->name, task->arrivalTime, task->needTime, task->startTime, task->endTime, task->runTime,
           task->remainingTime, task->waitTime);
}

void initTasks(Task *task, int taskLen) {
    if (!task || taskLen <= 0) {
        printf("initTasks: task(%p) cannot be null, taskLen(%d) cannot be less than 0!", task, taskLen);
        return;
    }

    for (int i = 0; i < taskLen; i++) {
        initTask(task++);
    }
}

void showTasks(const Task *task, int taskLen) {
#if SHOW_TASK_MESSAGE
    for (int i = 0; i < taskLen; i++) {
        showTask(&task[i]);
    }
    printf("\n");
#endif
}

void showWaitTime(const Task *task, int taskLen, char *s) {
    if (!task || taskLen <= 0) {
        printf("showWaitTime: task(%p) cannot be null, taskLen(%d) cannot be less than 0!", task, taskLen);
        return;
    }

    int waitTimeSum = 0;

    for (int i = 0; i < taskLen; i++) {
        const Task *cur = &task[i];
        waitTimeSum += cur->waitTime;

        sprintf(s + strlen(s), "Waiting Time %s: %d\n", cur->name, cur->waitTime);
    }

    sprintf(s + strlen(s), "Average Waiting Time: %.2f\n\n", 1. * waitTimeSum / taskLen);
}

void calcWaitTime(Task *task) {
    if (!task) {
        printf("calcWaitTime: task cannot be null!");
        return;
    }

    // 必须是已经执行完毕的任务
    if (task->startTime < 0 || task->endTime <= 0) {
        printf("required a completed task!\n");
        showTask(task);
        return;
    }

    task->waitTime = task->endTime - task->arrivalTime - task->needTime;
}

void calcWaitTimes(Tasks *tasks) {
    if (!tasks) {
        printf("calWaitTimes: task cannot be null!");
    }
    if (tasks->tasksLen <= 0) {
        return;
    }

    for (int i = 0; i < tasks->tasksLen; i++) {
        calcWaitTime(&tasks->tasks[i]);
    }
}

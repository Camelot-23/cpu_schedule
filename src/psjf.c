//
// Created by ELOT on 11/23/2023.
//

#include <psjf.h>

#include <stdio.h>
#include <string.h>

void psjf(Tasks *tasks, char *s) {
    if (!tasks) {
        printf("psjf: tasks(%p) cannot be null!", tasks);
        return;
    }

    int     nowTime     = 0;
    Task    *nowTask    = NULL;

    while (true) {
        Task *minTask = NULL;
        // 从已经到达中的任务中找出剩余时间最小的一个
        int minRemainingTime = 10000;
        for (int i = 0; i < tasks->tasksLen; i++) {
            Task *task = &tasks->tasks[i];
            // 跳过还未到达的任务
            if (task->arrivalTime > nowTime) {
                continue;
            }
            // 跳过已经完成的任务
            if (task->remainingTime == 0) {
                continue;
            }
            // 优先级更高的任务
            if (task->remainingTime < minRemainingTime) {
                minRemainingTime = task->remainingTime;
                minTask = task;
            }
        }

        // 所有任务已经执行完毕
        if (minRemainingTime == 10000) {
            sprintf(s + strlen(s), "%s\t%d\t%d\n", nowTask->name, nowTask->startTime, nowTime);
            break;
        }

        // 如果任务还未开始则开始
        if (minTask->startTime == -1) {
            minTask->startTime = nowTime;
        }

        // 如果这次的任务不跟上次是同一个
        if (minTask != nowTask) {
            // 第一个任务运行时，nowTask为NULL
            if (nowTask) {
                sprintf(s + strlen(s), "%s\t%d\t%d\n", nowTask->name, nowTask->startTime, nowTime);
            }
        }

        nowTask = minTask;

        // 执行一个时间片
        ++minTask->runTime;
        // 剩余时间减1
        --minTask->remainingTime;

        ++nowTime;
        if (minTask->remainingTime == 0) {
            minTask->endTime = nowTime;
        }
    }
    calcWaitTimes(tasks);
}

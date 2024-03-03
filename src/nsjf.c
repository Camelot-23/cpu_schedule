//
// Created by ELOT on 11/23/2023.
//

#include <nsjf.h>

#include <stdio.h>
#include <string.h>

void nsjf(Tasks *tasks, char *s) {
    if (!tasks) {
        printf("nsjf: tasks(%p) cannot be null!", tasks);
        return;
    }

    int     nowTime     = 0;

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
            // 跳过已经执行过的任务
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
            break;
        }
        // 如果任务还未开始则开始
        if (minTask->startTime == -1) {
            minTask->startTime = nowTime;
        }

        // 执行任务
        minTask->runTime += minTask->remainingTime;
        nowTime += minTask->runTime;
        // 执行完毕
        minTask->remainingTime = 0;

        minTask->endTime = nowTime;
        sprintf(s + strlen(s), "%s\t%d\t%d\n", minTask->name, minTask->startTime, minTask->endTime);
    }
    calcWaitTimes(tasks);
}

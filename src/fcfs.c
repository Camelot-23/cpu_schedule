//
// Created by ELOT on 11/23/2023.
//

#include <fcfs.h>

#include <stdio.h>
#include <string.h>

void fcfs(Tasks *tasks, char *s) {
    if (!tasks) {
        printf("fcfs: tasks(%p) cannot be null!", tasks);
        return;
    }

    int nowTime = 0;
    for (int i = 0; i < tasks->tasksLen; i++) {
        Task *task = &tasks->tasks[i];
        // 到达时空闲
        if (nowTime < task->arrivalTime) {
            nowTime = task->arrivalTime;
        }

        sprintf(s + strlen(s), "%s\t", task->name);
        // 开始执行
        task->startTime = nowTime;
        sprintf(s + strlen(s), "%d\t", task->startTime);

        // 执行整个任务
        task->endTime   = nowTime + task->needTime;
        sprintf(s + strlen(s), "%d\n", task->endTime);

        // 执行完毕
        nowTime         = task->endTime;
        task->runTime   = task->needTime;
    }

    calcWaitTimes(tasks);
}

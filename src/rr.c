//
// Created by ELOT on 11/23/2023.
//

#include <rr.h>

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>


void rr(Tasks *tasks, char *s) {
    if (!tasks) {
        printf("rr: tasks(%p) cannot be null!", tasks);
        return;
    }

    // 时间片大小(ms)
    const int timeSlice = 4;
    int nowTime         = 0;

    Rd head, tail;
    memset(&head, 0, sizeof (Rd));
    memset(&tail, 0, sizeof (Rd));

    Rd *lastRunTask = NULL;

    while (true) {
        // 本次剩余时间片
        int remainTime = timeSlice;

        bool allTaskDone = true;
        // 将上一次执行期间到达的新任务加到队列中
        for (int i = 0; i < tasks->tasksLen; i++) {
            Task *task = &tasks->tasks[i];
            // 跳过执行完毕的
            // 如果所有任务都执行完毕，则循环退出时allTaskDown为true
            if (task->remainingTime == 0) {
                continue;
            }
            allTaskDone = false;
            // 跳过还未到达的
            if (task->arrivalTime > nowTime) {
                continue;
            }
            // 跳过已经在队列中的
            if (task->inQueue) {
                continue;
            }
            // 为新到达的任务
            // 添加到等待队列尾部(链表)
            Rd *rd = (Rd *) malloc(sizeof(Rd));
            memset(rd, 0, sizeof(Rd));
            rd->task = task;
            rd->next = NULL;

            addTail(&head, rd);
            task->inQueue = true;
        }

        if (allTaskDone) {
            break;
        }

        // 如果上一次执行的任务还没有结束，则将其添加到队列尾部
        if (lastRunTask) {
            if (lastRunTask->task->remainingTime) {
                addTail(&head, lastRunTask);
            }
        }
        // 取出队列中的第一个任务
        Rd *first = getFirst(&head);
        // 如果队列中已经没有任务，但是还有未执行完的任务，则时间加一
        if (!first) {
            ++nowTime;
            continue;
        }

        Task *task = first->task;
        // 如果还未开始就开始
        if (task->startTime == -1) {
            task->startTime = nowTime;
        }

        sprintf(s + strlen(s), "%s\t%d\t", task->name, nowTime);
        if (task->remainingTime <= remainTime) {
            task->runTime += task->remainingTime;
            nowTime += task->remainingTime;

            task->remainingTime = 0;
            task->endTime = nowTime;

            lastRunTask = NULL;
            // 任务执行完毕，释放节点
            free(first);
        } else {
            task->runTime += remainTime;
            nowTime += remainTime;

            task->remainingTime -= remainTime;
            lastRunTask = first;
        }

        sprintf(s + strlen(s), "%d\n", nowTime);
    }

    calcWaitTimes(tasks);
}

void addTail(Rd *head, Rd *rd) {
    while (head->next != NULL) {
        head = head->next;
    }

    head->next = rd;
}

void showRd(Rd *head) {
    if (!head) {
        printf("showRd:");
        return;
    }
    while (head->next) {
        printf("%p ", head->task);

        head = head->next;
    }
    printf("%p ", head->task);
}

Rd *getFirst(Rd *head) {

    if (!head->next) {
        return NULL;
    }

    Rd *first = head->next;

    Rd *second = first->next;

    first->next = NULL;
    head->next = second;

    return first;
}


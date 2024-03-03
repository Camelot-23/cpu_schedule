#include <task.h>
#include <file.h>

#include <fcfs.h>
#include <rr.h>
#include <nsjf.h>
#include <psjf.h>
// os
#include <stdio.h>
#include <string.h>

#define USE_FCFS 1
#define USE_RR   1
#define USE_NSJF 1
#define USE_PSJF 1

int main() {
    Tasks tasks;
    tasks.tasksLen = 0;
    initTasks(tasks.tasks, MAX_TASK_LEN);

    char inputFileName[128] = "TaskSpec.txt";
    if (!readTasksFromFile(inputFileName, &tasks)) {
        return -1;
    }

    char outputFileName[128] = "OutPut.txt";
    char outputStr[10000];
    memset(outputStr, 0, 10000);

    Tasks tmpTasks;

#if USE_FCFS
    memcpy(&tmpTasks, &tasks, sizeof tasks);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
    sprintf(outputStr, "FCFS:\n");
    fcfs(&tmpTasks, outputStr);
    showWaitTime(tmpTasks.tasks, tmpTasks.tasksLen, outputStr);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
#endif

#if USE_RR
    memcpy(&tmpTasks, &tasks, sizeof tasks);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
    sprintf(outputStr +strlen(outputStr), "RR:\n");
    rr(&tmpTasks, outputStr);
    showWaitTime(tmpTasks.tasks, tmpTasks.tasksLen, outputStr);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
#endif

#if USE_NSJF
    memcpy(&tmpTasks, &tasks, sizeof tasks);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
    sprintf(outputStr + strlen(outputStr), "NSJF:\n");
    nsjf(&tmpTasks, outputStr);
    showWaitTime(tmpTasks.tasks, tmpTasks.tasksLen, outputStr);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
#endif

#if USE_PSJF
    memcpy(&tmpTasks, &tasks, sizeof tasks);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
    sprintf(outputStr + strlen(outputStr), "PSJF:\n");
    psjf(&tmpTasks, outputStr);
    showWaitTime(tmpTasks.tasks, tmpTasks.tasksLen, outputStr);
    showTasks(tmpTasks.tasks, tmpTasks.tasksLen);
#endif

    writeStrToFile(outputStr, outputFileName);

    printf("%s\n", outputStr);

    return 0;
}
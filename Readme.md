## 声明
### 输入输出格式
#### 输入
每一行的格式：任务名,到达时间，需要执行的时间  
注意：按照任务到达的先后排序，先到的在上  

    T1,0,14  
    T2,3,2  
    T3,3,6  
    T4,5,6  
    T5,6,3  

#### 输出
任务名 执行的开始时间 执行的结束时间(执行的一个小区间)

    PSJF:
    T1	0	3
    T2	3	5
    T3	5	6
    T5	6	9
    T3	5	14
    T4	14	20
    T1	0	31
    Waiting Time T1: 17
    Waiting Time T2: 0
    Waiting Time T3: 5
    Waiting Time T4: 9
    Waiting Time T5: 0
    Average Waiting Time: 6.20

### 任务的数据结构
```C
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

// 最大任务长度
#define MAX_TASK_LEN 100

typedef struct {
    Task    tasks[MAX_TASK_LEN];    // 任务表
    int     tasksLen;               // 任务数量
} Tasks;
```

### 测试输入
`TaskSpec.txt`

    T1,0,14
    T2,3,2
    T3,3,6
    T4,5,6
    T5,6,3

### 测试输出
`OutPut.txt`


    FCFS:
    T1	0	14
    T2	14	16
    T3	16	22
    T4	22	28
    T5	28	31
    Waiting Time T1: 0
    Waiting Time T2: 11
    Waiting Time T3: 13
    Waiting Time T4: 17
    Waiting Time T5: 22
    Average Waiting Time: 12.60
    
    RR:
    T1	0	4
    T2	4	6
    T3	6	10
    T1	10	14
    T4	14	18
    T5	18	21
    T3	21	23
    T1	23	27
    T4	27	29
    T1	29	31
    Waiting Time T1: 17
    Waiting Time T2: 1
    Waiting Time T3: 14
    Waiting Time T4: 18
    Waiting Time T5: 12
    Average Waiting Time: 12.40
    
    NSJF:
    T1	0	14
    T2	14	16
    T5	16	19
    T3	19	25
    T4	25	31
    Waiting Time T1: 0
    Waiting Time T2: 11
    Waiting Time T3: 16
    Waiting Time T4: 20
    Waiting Time T5: 10
    Average Waiting Time: 11.40
    
    PSJF:
    T1	0	3
    T2	3	5
    T3	5	6
    T5	6	9
    T3	5	14
    T4	14	20
    T1	0	31
    Waiting Time T1: 17
    Waiting Time T2: 0
    Waiting Time T3: 5
    Waiting Time T4: 9
    Waiting Time T5: 0
    Average Waiting Time: 6.20

## 算法

### RR(Round Robin) 时间片轮转

此处时间片大小为4  
首先需要一个先进先出队列，这里采用链表实现  

    1. 将上一个时间片内到达的任务加到队列的末尾
    2. 取出队列首的任务并执行
    3. 若执行一个时间片后此任务仍没有结束
    4. 则将其加到队列尾部
    重复1-4，直到所有的任务都执行完毕

#### 主要代码
```C
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
```

### FCFS(First Come, First Served) 先来先服务

    1. 取出最先到达的任务，执行完毕 
    重复1，直到所有任务执行完毕
#### 主要代码
```C
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

```

### PSJF(Preemptive Shortest Job First) 抢占式短作业优先
    1. 从已到达的任务中取出剩余执行时间最小的一个
    2. 执行一个时间片
    重复1-2，直到所有任务执行完毕
#### 主要代码
```C
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
```

### NSJF(Non-preemptive Shortest Job First) 非抢占式短作业优先
    1. 从已到达的任务中取出剩余执行时间最小的一个
    2. 将此任务执行完
    重复1-2，直到所有任务执行完毕
#### 主要代码
```C
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
```


### 构建运行
#### cmake
```shell
$ cd cpu_schedule
$ cmake -S . -B build
$ cmake --build build
$ cp build/cpu_schedule .
$ ./cpu_schedule
```
#### makefile
```shell
$ cd cpu_schedule
$ make
$ ./cpu_schedule
```

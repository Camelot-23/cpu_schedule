//
// Created by ELOT on 11/22/2023.
//

#ifndef CPU_SCHEDULE_FILE_H
#define CPU_SCHEDULE_FILE_H

#include <task.h>

// 从文件中读取任务队列
bool readTasksFromFile(const char *fileName, Tasks *tasks);
// 从一行中解析出一个任务
bool getTaskFromLine(char *line, Task *task);
// 去掉字符串两端的空白
char *trimWhitespace(char *str);
// 写字符数组到文件
bool writeStrToFile(const char *str, const char *fileName);

#endif //CPU_SCHEDULE_FILE_H

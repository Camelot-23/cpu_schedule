//
// Created by ELOT on 11/22/2023.
//

#include <file.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool readTasksFromFile(const char *fileName, Tasks *tasks) {
    if (!fileName || !tasks) {
        printf("readTasksFromFile: fileName(%p) cannot be null, tasks(%p) cannot be null!", fileName, tasks);
        return false;
    }

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("open file(%s) fail!", fileName);
        return false;
    }

    char line[1024];

    int lineId = 0;
    while (fgets(line, sizeof line, file) != NULL) {
        getTaskFromLine(line, &tasks->tasks[lineId]);

        ++tasks->tasksLen;
        ++lineId;
    }

    fclose(file);

    return true;
}

bool getTaskFromLine(char *line, Task *task) {
    if (!line || !task) {
        printf("getTaskFromLIne: line(%p) cannot be null, task(%p) cannot be null!", line, task);
        return false;
    }

    char *element = strtok(line, ",");

    char    *name       = NULL;
    int     arrivalTime = 0;
    int     execTime    = 0;

    int count = 0;
    while (element) {
        char *e = trimWhitespace(element);
//        printf("%s|", e);
        char *endPtr = NULL;
        switch (count) {
            case 0:
                name = e;
                break;
            case 1:
                arrivalTime = strtol(e, &endPtr, 10);
                if (*endPtr != '\0') {
                    arrivalTime = -1;
                    printf("getTaskFromLIne: get arrivalTime fail!line(%s)", line);
                }
                break;
            case 2:
                execTime = strtol(e, &endPtr, 10);
                if (*endPtr != '\0') {
                    execTime = -1;
                    printf("getTaskFromLIne: get needTime fail!line(%s)", line);
                }
                break;
            default:
                break;
        }

        if (count > 2) {
            break;
        }
        ++count;
        element = strtok(NULL, ",");
    }

    setTask(task, name, arrivalTime, execTime);

    return true;
}

char *trimWhitespace(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n') {
        str++;
    }

    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n')) {
        str[--len] = '\0';
    }

    return str;
}

bool writeStrToFile(const char *str, const char *fileName) {
    if (!str || !fileName || !strlen(fileName)) {
        printf("writeStrToFile: str(%p) cannot be null, fileName(%p %llu) cannot be null or empty!", str, fileName,
               strlen(fileName));
        return false;
    }

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("open file(%s) fail!", fileName);
        return false;
    }

    fwrite(str, strlen(str), 1, file);

    fclose(file);

    return true;
}

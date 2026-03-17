#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>

#define MAX_ID_LEN 16

typedef struct {
    char id[MAX_ID_LEN];
    int arrivalTime;
    int burstTime;
    int priority;
    
    // Tracking fields
    int remainingTime;
    int startTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    bool isStarted;
} Process;

typedef struct {
    char processId[MAX_ID_LEN];
    int startTime;
    int endTime;
} GanttRecord;

#endif // MODELS_H

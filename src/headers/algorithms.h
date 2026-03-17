#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "models.h"

#define MAX_GANTT_RECORDS 1000

// Common structure to hold algorithm results
typedef struct {
    char algorithmName[100];
    double avgWaitingTime;
    double avgTurnaroundTime;
    GanttRecord ganttChart[MAX_GANTT_RECORDS];
    int ganttSize;
    Process* processes;
    int processCount;
} ScheduleResult;

void scheduleFCFS(Process* processes, int numProcesses, ScheduleResult* result);
void scheduleSJF(Process* processes, int numProcesses, ScheduleResult* result);
void scheduleSRTF(Process* processes, int numProcesses, ScheduleResult* result);
void scheduleRoundRobin(Process* processes, int numProcesses, int timeQuantum, ScheduleResult* result);
void schedulePriority(Process* processes, int numProcesses, ScheduleResult* result);

void computeAverages(Process* processes, int numProcesses, ScheduleResult* result);

#endif // ALGORITHMS_H

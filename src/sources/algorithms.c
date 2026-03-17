#include "algorithms.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void computeAverages(Process* processes, int numProcesses, ScheduleResult* result) {
    double totalWait = 0, totalTurnaround = 0;
    for (int i = 0; i < numProcesses; i++) {
        totalWait += processes[i].waitingTime;
        totalTurnaround += processes[i].turnaroundTime;
    }
    result->avgWaitingTime = numProcesses > 0 ? totalWait / numProcesses : 0;
    result->avgTurnaroundTime = numProcesses > 0 ? totalTurnaround / numProcesses : 0;
}

void scheduleFCFS(Process* processes, int numProcesses, ScheduleResult* result) {
    strcpy(result->algorithmName, "First-Come, First-Served (FCFS)");
    result->ganttSize = 0;
    
    Process* pCopy = (Process*)malloc(numProcesses * sizeof(Process));
    copyProcessArray(pCopy, processes, numProcesses);
    qsort(pCopy, numProcesses, sizeof(Process), compareByArrival);
    
    int currentTime = 0;
    for (int i = 0; i < numProcesses; i++) {
        Process* p = &pCopy[i];
        if (currentTime < p->arrivalTime) {
            strcpy(result->ganttChart[result->ganttSize].processId, "Idle");
            result->ganttChart[result->ganttSize].startTime = currentTime;
            result->ganttChart[result->ganttSize].endTime = p->arrivalTime;
            result->ganttSize++;
            currentTime = p->arrivalTime;
        }
        
        p->startTime = currentTime;
        p->isStarted = true;
        p->responseTime = p->startTime - p->arrivalTime;
        
        strcpy(result->ganttChart[result->ganttSize].processId, p->id);
        result->ganttChart[result->ganttSize].startTime = currentTime;
        currentTime += p->burstTime;
        result->ganttChart[result->ganttSize].endTime = currentTime;
        result->ganttSize++;
        
        p->completionTime = currentTime;
        p->turnaroundTime = p->completionTime - p->arrivalTime;
        p->waitingTime = p->turnaroundTime - p->burstTime;
        p->remainingTime = 0;
    }
    
    result->processes = pCopy;
    result->processCount = numProcesses;
    computeAverages(pCopy, numProcesses, result);
}

void scheduleSJF(Process* processes, int numProcesses, ScheduleResult* result) {
    strcpy(result->algorithmName, "Shortest Job First (SJF) - Non-Preemptive");
    result->ganttSize = 0;
    
    Process* pCopy = (Process*)malloc(numProcesses * sizeof(Process));
    copyProcessArray(pCopy, processes, numProcesses);
    
    int currentTime = 0;
    int completedCount = 0;
    bool* isCompleted = (bool*)calloc(numProcesses, sizeof(bool));
    
    while (completedCount < numProcesses) {
        int shortestIdx = -1;
        int minBurst = 9999999;
        
        for (int i = 0; i < numProcesses; i++) {
            if (pCopy[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (pCopy[i].burstTime < minBurst) {
                    minBurst = pCopy[i].burstTime;
                    shortestIdx = i;
                } else if (pCopy[i].burstTime == minBurst) {
                    if (pCopy[i].arrivalTime < pCopy[shortestIdx].arrivalTime) {
                        shortestIdx = i;
                    }
                }
            }
        }
        
        if (shortestIdx == -1) {
            int nextArrival = 9999999;
            for (int i = 0; i < numProcesses; i++) {
                if (!isCompleted[i] && pCopy[i].arrivalTime < nextArrival) {
                    nextArrival = pCopy[i].arrivalTime;
                }
            }
            if (result->ganttSize > 0 && strcmp(result->ganttChart[result->ganttSize-1].processId, "Idle") == 0) {
                result->ganttChart[result->ganttSize-1].endTime = nextArrival;
            } else {
                strcpy(result->ganttChart[result->ganttSize].processId, "Idle");
                result->ganttChart[result->ganttSize].startTime = currentTime;
                result->ganttChart[result->ganttSize].endTime = nextArrival;
                result->ganttSize++;
            }
            currentTime = nextArrival;
        } else {
            Process* p = &pCopy[shortestIdx];
            p->startTime = currentTime;
            p->isStarted = true;
            p->responseTime = p->startTime - p->arrivalTime;
            
            strcpy(result->ganttChart[result->ganttSize].processId, p->id);
            result->ganttChart[result->ganttSize].startTime = currentTime;
            currentTime += p->burstTime;
            result->ganttChart[result->ganttSize].endTime = currentTime;
            result->ganttSize++;
            
            p->completionTime = currentTime;
            p->turnaroundTime = p->completionTime - p->arrivalTime;
            p->waitingTime = p->turnaroundTime - p->burstTime;
            p->remainingTime = 0;
            
            isCompleted[shortestIdx] = true;
            completedCount++;
        }
    }
    
    result->processes = pCopy;
    result->processCount = numProcesses;
    computeAverages(pCopy, numProcesses, result);
    free(isCompleted);
}

void scheduleSRTF(Process* processes, int numProcesses, ScheduleResult* result) {
    strcpy(result->algorithmName, "Shortest Remaining Time First (SRTF) - Preemptive SJF");
    result->ganttSize = 0;
    
    Process* pCopy = (Process*)malloc(numProcesses * sizeof(Process));
    copyProcessArray(pCopy, processes, numProcesses);
    
    int currentTime = 0;
    int completedCount = 0;
    bool* isCompleted = (bool*)calloc(numProcesses, sizeof(bool));
    
    char lastActiveProcess[MAX_ID_LEN] = "";

    while (completedCount < numProcesses) {
        int shortestIdx = -1;
        int minRemaining = 9999999;
        
        for (int i = 0; i < numProcesses; i++) {
            if (pCopy[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (pCopy[i].remainingTime < minRemaining) {
                    minRemaining = pCopy[i].remainingTime;
                    shortestIdx = i;
                } else if (pCopy[i].remainingTime == minRemaining) {
                    if (shortestIdx != -1 && pCopy[i].arrivalTime < pCopy[shortestIdx].arrivalTime) {
                        shortestIdx = i;
                    }
                }
            }
        }
        
        if (shortestIdx == -1) {
            if (strcmp(lastActiveProcess, "Idle") != 0) {
                strcpy(result->ganttChart[result->ganttSize].processId, "Idle");
                result->ganttChart[result->ganttSize].startTime = currentTime;
                result->ganttChart[result->ganttSize].endTime = currentTime + 1;
                result->ganttSize++;
                strcpy(lastActiveProcess, "Idle");
            } else {
                result->ganttChart[result->ganttSize-1].endTime = currentTime + 1;
            }
            currentTime++;
        } else {
            Process* p = &pCopy[shortestIdx];
            
            if (strcmp(lastActiveProcess, p->id) != 0) {
                strcpy(result->ganttChart[result->ganttSize].processId, p->id);
                result->ganttChart[result->ganttSize].startTime = currentTime;
                result->ganttChart[result->ganttSize].endTime = currentTime + 1;
                result->ganttSize++;
                strcpy(lastActiveProcess, p->id);
            } else {
                result->ganttChart[result->ganttSize-1].endTime = currentTime + 1;
            }
            
            if (!p->isStarted) {
                p->startTime = currentTime;
                p->isStarted = true;
                p->responseTime = p->startTime - p->arrivalTime;
            }
            
            p->remainingTime--;
            currentTime++;
            
            if (p->remainingTime == 0) {
                p->completionTime = currentTime;
                p->turnaroundTime = p->completionTime - p->arrivalTime;
                p->waitingTime = p->turnaroundTime - p->burstTime;
                isCompleted[shortestIdx] = true;
                completedCount++;
            }
        }
    }
    
    result->ganttSize = mergeGanttRecords(result->ganttChart, result->ganttSize);
    
    result->processes = pCopy;
    result->processCount = numProcesses;
    computeAverages(pCopy, numProcesses, result);
    free(isCompleted);
}

void scheduleRoundRobin(Process* processes, int numProcesses, int timeQuantum, ScheduleResult* result) {
    sprintf(result->algorithmName, "Round Robin (RR) - Quantum: %d", timeQuantum);
    result->ganttSize = 0;
    
    Process* pCopy = (Process*)malloc(numProcesses * sizeof(Process));
    copyProcessArray(pCopy, processes, numProcesses);
    
    // Initial sort by arrival time
    qsort(pCopy, numProcesses, sizeof(Process), compareByArrival);
    
    int* queue = (int*)malloc(10000 * sizeof(int));
    int head = 0, tail = 0;
    
    int currentTime = 0;
    int completedCount = 0;
    int processAdded = 0;
    
    // Add initial processes arriving at time 0
    while (processAdded < numProcesses && pCopy[processAdded].arrivalTime <= currentTime) {
        queue[tail++] = processAdded;
        processAdded++;
    }
    
    while (completedCount < numProcesses) {
        if (head == tail) {
            if (processAdded < numProcesses) {
                int nextArrival = pCopy[processAdded].arrivalTime;
                strcpy(result->ganttChart[result->ganttSize].processId, "Idle");
                result->ganttChart[result->ganttSize].startTime = currentTime;
                result->ganttChart[result->ganttSize].endTime = nextArrival;
                result->ganttSize++;
                currentTime = nextArrival;
                while (processAdded < numProcesses && pCopy[processAdded].arrivalTime <= currentTime) {
                    queue[tail++] = processAdded;
                    processAdded++;
                }
            }
        } else {
            int activeIdx = queue[head++];
            Process* p = &pCopy[activeIdx];
            
            if (!p->isStarted) {
                p->startTime = currentTime;
                p->isStarted = true;
                p->responseTime = p->startTime - p->arrivalTime;
            }
            
            int executeTime = (p->remainingTime < timeQuantum) ? p->remainingTime : timeQuantum;
            
            strcpy(result->ganttChart[result->ganttSize].processId, p->id);
            result->ganttChart[result->ganttSize].startTime = currentTime;
            currentTime += executeTime;
            result->ganttChart[result->ganttSize].endTime = currentTime;
            result->ganttSize++;
            
            p->remainingTime -= executeTime;
            
            while (processAdded < numProcesses && pCopy[processAdded].arrivalTime <= currentTime) {
                queue[tail++] = processAdded;
                processAdded++;
            }
            
            if (p->remainingTime > 0) {
                queue[tail++] = activeIdx;
            } else {
                p->completionTime = currentTime;
                p->turnaroundTime = p->completionTime - p->arrivalTime;
                p->waitingTime = p->turnaroundTime - p->burstTime;
                completedCount++;
            }
        }
    }
    
    result->ganttSize = mergeGanttRecords(result->ganttChart, result->ganttSize);
    
    result->processes = pCopy;
    result->processCount = numProcesses;
    computeAverages(pCopy, numProcesses, result);
    free(queue);
}

void schedulePriority(Process* processes, int numProcesses, ScheduleResult* result) {
    strcpy(result->algorithmName, "Priority Scheduling - Non-Preemptive (Lower number = Higher Priority)");
    result->ganttSize = 0;
    
    Process* pCopy = (Process*)malloc(numProcesses * sizeof(Process));
    copyProcessArray(pCopy, processes, numProcesses);
    
    int currentTime = 0;
    int completedCount = 0;
    bool* isCompleted = (bool*)calloc(numProcesses, sizeof(bool));
    
    while (completedCount < numProcesses) {
        int highestPriorityIdx = -1;
        int minPriority = 9999999;
        
        for (int i = 0; i < numProcesses; i++) {
            if (pCopy[i].arrivalTime <= currentTime && !isCompleted[i]) {
                if (pCopy[i].priority < minPriority) {
                    minPriority = pCopy[i].priority;
                    highestPriorityIdx = i;
                } else if (pCopy[i].priority == minPriority) {
                    if (pCopy[i].arrivalTime < pCopy[highestPriorityIdx].arrivalTime) {
                        highestPriorityIdx = i;
                    }
                }
            }
        }
        
        if (highestPriorityIdx == -1) {
            int nextArrival = 9999999;
            for (int i = 0; i < numProcesses; i++) {
                if (!isCompleted[i] && pCopy[i].arrivalTime < nextArrival) {
                    nextArrival = pCopy[i].arrivalTime;
                }
            }
            if (result->ganttSize > 0 && strcmp(result->ganttChart[result->ganttSize-1].processId, "Idle") == 0) {
                result->ganttChart[result->ganttSize-1].endTime = nextArrival;
            } else {
                strcpy(result->ganttChart[result->ganttSize].processId, "Idle");
                result->ganttChart[result->ganttSize].startTime = currentTime;
                result->ganttChart[result->ganttSize].endTime = nextArrival;
                result->ganttSize++;
            }
            currentTime = nextArrival;
        } else {
            Process* p = &pCopy[highestPriorityIdx];
            p->startTime = currentTime;
            p->isStarted = true;
            p->responseTime = p->startTime - p->arrivalTime;
            
            strcpy(result->ganttChart[result->ganttSize].processId, p->id);
            result->ganttChart[result->ganttSize].startTime = currentTime;
            currentTime += p->burstTime;
            result->ganttChart[result->ganttSize].endTime = currentTime;
            result->ganttSize++;
            
            p->completionTime = currentTime;
            p->turnaroundTime = p->completionTime - p->arrivalTime;
            p->waitingTime = p->turnaroundTime - p->burstTime;
            p->remainingTime = 0;
            
            isCompleted[highestPriorityIdx] = true;
            completedCount++;
        }
    }
    
    result->processes = pCopy;
    result->processCount = numProcesses;
    computeAverages(pCopy, numProcesses, result);
    free(isCompleted);
}

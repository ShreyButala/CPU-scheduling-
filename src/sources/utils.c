#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void copyProcessArray(Process* dest, const Process* src, int count) {
    for (int i = 0; i < count; i++) {
        dest[i] = src[i];
    }
}

int mergeGanttRecords(GanttRecord* records, int count) {
    if (count <= 1) return count;

    int newCount = 0;
    GanttRecord current = records[0];

    for (int i = 1; i < count; i++) {
        GanttRecord next = records[i];
        if (strcmp(current.processId, next.processId) == 0 && current.endTime == next.startTime) {
            current.endTime = next.endTime;
        } else {
            records[newCount++] = current;
            current = next;
        }
    }
    records[newCount++] = current;
    return newCount;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void printGanttChart(const GanttRecord* ganttChart, int count) {
    printf("\n--- Gantt Chart ---\n");

    if (count == 0) return;

    // Print top bar
    for (int i = 0; i < count; i++) {
        printf(" ");
        int duration = ganttChart[i].endTime - ganttChart[i].startTime;
        int len = max(duration * 2, (int)strlen(ganttChart[i].processId));
        for (int j = 0; j < len; j++) {
            printf("-");
        }
    }
    printf("\n");

    // Print process IDs
    for (int i = 0; i < count; i++) {
        printf("| %s ", ganttChart[i].processId);
        int duration = ganttChart[i].endTime - ganttChart[i].startTime;
        int len = max(duration * 2, (int)strlen(ganttChart[i].processId));
        int spaces = len - strlen(ganttChart[i].processId) - 1;
        for (int j = 0; j < spaces; j++) {
            printf(" ");
        }
    }
    printf("|\n");

    // Print bottom bar
    for (int i = 0; i < count; i++) {
        printf(" ");
        int duration = ganttChart[i].endTime - ganttChart[i].startTime;
        int len = max(duration * 2, (int)strlen(ganttChart[i].processId));
        for (int j = 0; j < len; j++) {
            printf("-");
        }
    }
    printf("\n");

    // Print timeline
    printf("0");
    for (int i = 0; i < count; i++) {
        char timeStr[16];
        sprintf(timeStr, "%d", ganttChart[i].endTime);
        int duration = ganttChart[i].endTime - ganttChart[i].startTime;
        int len = max(duration * 2, (int)strlen(ganttChart[i].processId));
        
        int spacing = len + 2 - strlen(timeStr) + (ganttChart[i].startTime == 0 ? -1 : 0);
        for (int j = 0; j < spacing; j++) {
            printf(" ");
        }
        printf("%s", timeStr);
    }
    printf("\n\n");
}

int compareByArrival(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    if (p1->arrivalTime == p2->arrivalTime) {
        return p1->priority - p2->priority; // Tie-breaker
    }
    return p1->arrivalTime - p2->arrivalTime;
}

int compareById(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    // Assuming IDs are like "P1", "P2"
    int id1 = atoi(p1->id + 1);
    int id2 = atoi(p2->id + 1);
    return id1 - id2;
}

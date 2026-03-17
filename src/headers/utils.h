#ifndef UTILS_H
#define UTILS_H

#include "models.h"

// Deep copy an array of processes
void copyProcessArray(Process* dest, const Process* src, int count);

// Merge continuous identical process executions in Gantt chart
// Modifies the records array in-place and returns the new count
int mergeGanttRecords(GanttRecord* records, int count);

// Print Text-based Gantt Chart
void printGanttChart(const GanttRecord* ganttChart, int count);

// Comparators for qsort
int compareByArrival(const void* a, const void* b);
int compareById(const void* a, const void* b);

#endif // UTILS_H

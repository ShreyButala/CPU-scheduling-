#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "algorithms.h"
#include "utils.h"
#include "html_render.h"

int main() {
    int numProcesses;
    
    printf("==================================================\n");
    printf("      CPU Scheduling & Visualization Tool\n");
    printf("==================================================\n");
    
    printf("Enter the number of processes: ");
    if (scanf("%d", &numProcesses) != 1 || numProcesses <= 0) {
        printf("Invalid input. Please enter a positive integer.\n");
        return 1;
    }
    
    Process* initialProcesses = (Process*)malloc(numProcesses * sizeof(Process));
    
    for (int i = 0; i < numProcesses; i++) {
        printf("\nProcess %d Details:\n", i + 1);
        sprintf(initialProcesses[i].id, "P%d", i + 1);
        
        printf("Arrival Time: ");
        scanf("%d", &initialProcesses[i].arrivalTime);
        
        printf("Burst Time: ");
        scanf("%d", &initialProcesses[i].burstTime);
        
        printf("Priority (Lower number = Higher priority): ");
        scanf("%d", &initialProcesses[i].priority);
        
        initialProcesses[i].remainingTime = initialProcesses[i].burstTime;
        initialProcesses[i].startTime = -1;
        initialProcesses[i].completionTime = 0;
        initialProcesses[i].waitingTime = 0;
        initialProcesses[i].turnaroundTime = 0;
        initialProcesses[i].responseTime = -1;
        initialProcesses[i].isStarted = false;
    }
    
    int timeQuantum;
    printf("\nEnter Time Quantum for Round Robin: ");
    scanf("%d", &timeQuantum);
    
    printf("\nExecuting Algorithms...\n");
    
    ScheduleResult results[5];
    
    scheduleFCFS(initialProcesses, numProcesses, &results[0]);
    scheduleSJF(initialProcesses, numProcesses, &results[1]);
    scheduleSRTF(initialProcesses, numProcesses, &results[2]);
    scheduleRoundRobin(initialProcesses, numProcesses, timeQuantum, &results[3]);
    schedulePriority(initialProcesses, numProcesses, &results[4]);
    
    printf("\n==================================================\n");
    printf("               INDIVIDUAL RESULTS\n");
    printf("==================================================\n");
    
    for (int i = 0; i < 5; i++) {
        printf("\n>>> Algorithm: %s\n", results[i].algorithmName);
        printf("Average Waiting Time: %.2f\n", results[i].avgWaitingTime);
        printf("Average Turnaround Time: %.2f\n", results[i].avgTurnaroundTime);
        
        printGanttChart(results[i].ganttChart, results[i].ganttSize);
        printf("--------------------------------------------------\n");
    }
    
    printf("\nGenerating HTML Visualization Dashboard...\n");
    generateHtmlReport(results, 5, "results.html");
    printf("Successfully generated results.html\n");
    
    // Free the dynamically allocated process copies in the result at the end
    for (int i = 0; i < 5; i++) {
        free(results[i].processes);
    }
    
    free(initialProcesses);
    return 0;
}

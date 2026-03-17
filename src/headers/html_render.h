#ifndef HTML_RENDER_H
#define HTML_RENDER_H

#include "algorithms.h"

// Generates an HTML report containing the visual Gantt charts and comparison tables.
// The provided results arrays must be fully populated.
void generateHtmlReport(ScheduleResult* results, int numAlgorithms, const char* filename);

#endif // HTML_RENDER_H

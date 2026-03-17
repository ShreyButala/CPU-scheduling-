#include "html_render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generateHtmlReport(ScheduleResult* results, int numAlgorithms, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating HTML report file!\n");
        return;
    }

    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(file, "    <title>CPU Scheduling Visualization</title>\n");
    fprintf(file, "    <style>\n");
    fprintf(file, "        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600;800&display=swap');\n");
    fprintf(file, "        :root {\n");
    fprintf(file, "            --bg-color: #0f172a;\n");
    fprintf(file, "            --surface-color: rgba(30, 41, 59, 0.7);\n");
    fprintf(file, "            --text-main: #f8fafc;\n");
    fprintf(file, "            --text-muted: #94a3b8;\n");
    fprintf(file, "            --accent: #3b82f6;\n");
    fprintf(file, "            --accent-glow: rgba(59, 130, 246, 0.5);\n");
    fprintf(file, "            --border: rgba(255, 255, 255, 0.1);\n");
    fprintf(file, "        }\n");
    fprintf(file, "        * { margin: 0; padding: 0; box-sizing: border-box; }\n");
    fprintf(file, "        body { font-family: 'Inter', sans-serif; background-color: var(--bg-color); color: var(--text-main); line-height: 1.6; padding: 40px 20px; overflow-x: hidden; }\n");
    fprintf(file, "        body::before { content: ''; position: fixed; top: -50%%; left: -50%%; width: 200%%; height: 200%%; background: radial-gradient(circle at 50%% 50%%, var(--accent-glow) 0%%, transparent 50%%); opacity: 0.15; z-index: -1; pointer-events: none; }\n");
    fprintf(file, "        .container { max-width: 1200px; margin: 0 auto; }\n");
    fprintf(file, "        header { text-align: center; margin-bottom: 50px; animation: fadeInDown 0.8s ease-out; }\n");
    fprintf(file, "        h1 { font-size: 3rem; font-weight: 800; background: linear-gradient(135deg, #fff, #94a3b8); -webkit-background-clip: text; -webkit-text-fill-color: transparent; margin-bottom: 10px; }\n");
    fprintf(file, "        p.subtitle { color: var(--text-muted); font-size: 1.2rem; }\n");
    fprintf(file, "        \n");
    fprintf(file, "        .glass-panel { background: var(--surface-color); backdrop-filter: blur(12px); -webkit-backdrop-filter: blur(12px); border: 1px solid var(--border); border-radius: 16px; padding: 30px; margin-bottom: 40px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); animation: fadeIn 1s ease-out; }\n");
    fprintf(file, "        \n");
    fprintf(file, "        /* Table Styles */\n");
    fprintf(file, "        h2 { font-size: 1.8rem; margin-bottom: 20px; border-bottom: 2px solid var(--border); padding-bottom: 10px; }\n");
    fprintf(file, "        table { width: 100%%; border-collapse: collapse; margin-top: 10px; }\n");
    fprintf(file, "        th, td { padding: 15px; text-align: left; border-bottom: 1px solid var(--border); }\n");
    fprintf(file, "        th { font-weight: 600; color: var(--text-muted); text-transform: uppercase; font-size: 0.85rem; letter-spacing: 1px; }\n");
    fprintf(file, "        tr:hover { background-color: rgba(255,255,255,0.02); transition: background-color 0.2s; }\n");
    fprintf(file, "        .highlight { color: #60a5fa; font-weight: 600; }\n");
    fprintf(file, "        \n");
    fprintf(file, "        /* Gantt Chart Styles */\n");
    fprintf(file, "        .alg-section { margin-bottom: 40px; }\n");
    fprintf(file, "        .alg-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 15px; }\n");
    fprintf(file, "        .alg-title { font-size: 1.4rem; font-weight: 600; color: #e2e8f0; }\n");
    fprintf(file, "        .alg-stats { font-size: 0.9rem; color: var(--text-muted); }\n");
    fprintf(file, "        .alg-stats span { background: rgba(0,0,0,0.3); padding: 4px 10px; border-radius: 20px; margin-left: 10px; }\n");
    fprintf(file, "        \n");
    fprintf(file, "        .gantt-wrapper { position: relative; width: 100%%; background: rgba(0,0,0,0.2); border-radius: 8px; padding: 20px 10px; overflow-x: auto; white-space: nowrap; }\n");
    fprintf(file, "        .gantt-chart { display: flex; align-items: center; height: 60px; min-width: max-content; }\n");
    fprintf(file, "        .gantt-block { position: relative; height: 100%%; display: flex; align-items: center; justify-content: center; font-weight: 600; font-size: 1.1rem; color: #fff; box-shadow: inset 0 0 0 1px rgba(255,255,255,0.1); margin-right: 2px; border-radius: 4px; overflow: hidden; transition: transform 0.2s, filter 0.2s; }\n");
    fprintf(file, "        .gantt-block:hover { transform: translateY(-2px); filter: brightness(1.2); z-index: 10; }\n");
    fprintf(file, "        .gantt-block.idle { background: repeating-linear-gradient(45deg, rgba(255,255,255,0.03), rgba(255,255,255,0.03) 10px, rgba(255,255,255,0.08) 10px, rgba(255,255,255,0.08) 20px); color: var(--text-muted); font-style: italic; font-size: 0.9rem; }\n");
    fprintf(file, "        \n");
    fprintf(file, "        .gantt-timeline { display: flex; position: relative; margin-top: 10px; height: 20px; min-width: max-content; }\n");
    fprintf(file, "        .time-marker { position: absolute; font-size: 0.8rem; color: var(--text-muted); transform: translateX(-50%%); }\n");
    fprintf(file, "        .time-marker::before { content: ''; position: absolute; top: -8px; left: 50%%; width: 1px; height: 5px; background: var(--text-muted); }\n");
    fprintf(file, "        \n");
    fprintf(file, "        /* Animations */\n");
    fprintf(file, "        @keyframes fadeIn { from { opacity: 0; transform: translateY(20px); } to { opacity: 1; transform: translateY(0); } }\n");
    fprintf(file, "        @keyframes fadeInDown { from { opacity: 0; transform: translateY(-20px); } to { opacity: 1; transform: translateY(0); } }\n");
    fprintf(file, "    </style>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    fprintf(file, "    <div class=\"container\">\n");
    fprintf(file, "        <header>\n");
    fprintf(file, "            <h1>CPU Scheduling Analysis</h1>\n");
    fprintf(file, "            <p class=\"subtitle\">Performance Comparison & Gantt Visualizations</p>\n");
    fprintf(file, "        </header>\n");
    
    fprintf(file, "        <div class=\"glass-panel\">\n");
    fprintf(file, "            <h2>Performance Comparison</h2>\n");
    fprintf(file, "            <table id=\"comparison-table\">\n");
    fprintf(file, "                <thead>\n");
    fprintf(file, "                    <tr>\n");
    fprintf(file, "                        <th>Algorithm</th>\n");
    fprintf(file, "                        <th>Avg Waiting Time</th>\n");
    fprintf(file, "                        <th>Avg Turnaround Time</th>\n");
    fprintf(file, "                    </tr>\n");
    fprintf(file, "                </thead>\n");
    fprintf(file, "                <tbody>\n");
    // Table rows injected via JS
    fprintf(file, "                </tbody>\n");
    fprintf(file, "            </table>\n");
    fprintf(file, "        </div>\n");

    fprintf(file, "        <div class=\"glass-panel\">\n");
    fprintf(file, "            <h2>Gantt Charts</h2>\n");
    fprintf(file, "            <div id=\"gantt-container\">\n");
    // Gantt charts injected via JS
    fprintf(file, "            </div>\n");
    fprintf(file, "        </div>\n");
    fprintf(file, "    </div>\n");

    // JavaScript Block to embed JSON and render UI
    fprintf(file, "    <script>\n");
    fprintf(file, "        const colors = [\n");
    fprintf(file, "            'linear-gradient(135deg, #ef4444, #b91c1c)', // Red\n");
    fprintf(file, "            'linear-gradient(135deg, #3b82f6, #1d4ed8)', // Blue\n");
    fprintf(file, "            'linear-gradient(135deg, #10b981, #047857)', // Green\n");
    fprintf(file, "            'linear-gradient(135deg, #f59e0b, #b45309)', // Amber\n");
    fprintf(file, "            'linear-gradient(135deg, #8b5cf6, #5b21b6)', // Purple\n");
    fprintf(file, "            'linear-gradient(135deg, #ec4899, #be185d)', // Pink\n");
    fprintf(file, "            'linear-gradient(135deg, #06b6d4, #0369a1)', // Cyan\n");
    fprintf(file, "            'linear-gradient(135deg, #84cc16, #4d7c0f)'  // Lime\n");
    fprintf(file, "        ];\n");
    fprintf(file, "        const processColors = {};\n");
    fprintf(file, "        let colorIndex = 0;\n");
    fprintf(file, "        \n");
    fprintf(file, "        function getColor(processId) {\n");
    fprintf(file, "            if (processId === 'Idle' || processId === 'idle') return '';\n");
    fprintf(file, "            if (!processColors[processId]) {\n");
    fprintf(file, "                processColors[processId] = colors[colorIndex %% colors.length];\n");
    fprintf(file, "                colorIndex++;\n");
    fprintf(file, "            }\n");
    fprintf(file, "            return processColors[processId];\n");
    fprintf(file, "        }\n");

    // Embed JSON Data from C
    fprintf(file, "        const scheduleData = [\n");
    for (int i = 0; i < numAlgorithms; i++) {
        fprintf(file, "            {\n");
        fprintf(file, "                name: \"%s\",\n", results[i].algorithmName);
        fprintf(file, "                avgWait: %.2f,\n", results[i].avgWaitingTime);
        fprintf(file, "                avgTurnaround: %.2f,\n", results[i].avgTurnaroundTime);
        fprintf(file, "                gantt: [\n");
        for (int j = 0; j < results[i].ganttSize; j++) {
            fprintf(file, "                    { id: \"%s\", start: %d, end: %d }%s\n", 
                results[i].ganttChart[j].processId,
                results[i].ganttChart[j].startTime,
                results[i].ganttChart[j].endTime,
                (j < results[i].ganttSize - 1) ? "," : ""
            );
        }
        fprintf(file, "                ]\n");
        fprintf(file, "            }%s\n", (i < numAlgorithms - 1) ? "," : "");
    }
    fprintf(file, "        ];\n");

    // Render Logic
    fprintf(file, "        \n");
    fprintf(file, "        // Render Comparison Table\n");
    fprintf(file, "        const tbody = document.querySelector('#comparison-table tbody');\n");
    fprintf(file, "        scheduleData.forEach((alg, index) => {\n");
    fprintf(file, "            const tr = document.createElement('tr');\n");
    fprintf(file, "            tr.style.animation = `fadeIn 0.5s ease-out ${(index * 0.1) + 0.3}s both`;\n");
    fprintf(file, "            tr.innerHTML = `\n");
    fprintf(file, "                <td style=\"font-weight: 600;\">${alg.name}</td>\n");
    fprintf(file, "                <td class=\"highlight\">${alg.avgWait.toFixed(2)} ms</td>\n");
    fprintf(file, "                <td class=\"highlight\">${alg.avgTurnaround.toFixed(2)} ms</td>\n");
    fprintf(file, "            `;\n");
    fprintf(file, "            tbody.appendChild(tr);\n");
    fprintf(file, "        });\n");
    fprintf(file, "        \n");
    fprintf(file, "        // Render Gantt Charts\n");
    fprintf(file, "        const ganttContainer = document.getElementById('gantt-container');\n");
    fprintf(file, "        scheduleData.forEach((alg, index) => {\n");
    fprintf(file, "            if(alg.gantt.length === 0) return;\n");
    fprintf(file, "            \n");
    fprintf(file, "            const totalDuration = alg.gantt[alg.gantt.length - 1].end || 1;\n");
    fprintf(file, "            // Base width per time unit to make short jobs visible, but prevent massive overflow\n");
    fprintf(file, "            const pixelsPerUnit = Math.max(30, Math.min(100, 1000 / totalDuration));\n");
    fprintf(file, "            \n");
    fprintf(file, "            const section = document.createElement('div');\n");
    fprintf(file, "            section.className = 'alg-section';\n");
    fprintf(file, "            section.style.animation = `fadeIn 0.6s ease-out ${(index * 0.15) + 0.5}s both`;\n");
    fprintf(file, "            \n");
    fprintf(file, "            let blocksHtml = '';\n");
    fprintf(file, "            let timelineHtml = `<div class=\"time-marker\" style=\"left: 0%%; margin-left: 5px;\">0</div>`;\n");
    fprintf(file, "            \n");
    fprintf(file, "            alg.gantt.forEach(block => {\n");
    fprintf(file, "                const duration = block.end - block.start;\n");
    fprintf(file, "                const widthPx = duration * pixelsPerUnit;\n");
    fprintf(file, "                const isIdle = block.id.toLowerCase() === 'idle';\n");
    fprintf(file, "                const bgStyle = isIdle ? '' : `background: ${getColor(block.id)};`;\n");
    fprintf(file, "                const className = isIdle ? 'gantt-block idle' : 'gantt-block';\n");
    fprintf(file, "                \n");
    fprintf(file, "                blocksHtml += `<div class=\"${className}\" style=\"width: ${widthPx}px; ${bgStyle}\" title=\"${block.id}: Time ${block.start} to ${block.end} (Duration: ${duration})\">${isIdle ? 'IDLE' : block.id}</div>`;\n");
    fprintf(file, "                \n");
    fprintf(file, "                timelineHtml += `<div class=\"time-marker\" style=\"left: ${(block.end / totalDuration) * 100}%%;\">${block.end}</div>`;\n");
    fprintf(file, "            });\n");
    fprintf(file, "            \n");
    fprintf(file, "            section.innerHTML = `\n");
    fprintf(file, "                <div class=\"alg-header\">\n");
    fprintf(file, "                    <div class=\"alg-title\">${alg.name}</div>\n");
    fprintf(file, "                    <div class=\"alg-stats\">\n");
    fprintf(file, "                        <span>Wait: ${alg.avgWait.toFixed(2)}</span>\n");
    fprintf(file, "                        <span>Turnaround: ${alg.avgTurnaround.toFixed(2)}</span>\n");
    fprintf(file, "                    </div>\n");
    fprintf(file, "                </div>\n");
    fprintf(file, "                <div class=\"gantt-wrapper\">\n");
    fprintf(file, "                    <div class=\"gantt-chart\" style=\"width: ${totalDuration * pixelsPerUnit}px\">\n");
    fprintf(file, "                        ${blocksHtml}\n");
    fprintf(file, "                    </div>\n");
    fprintf(file, "                    <div class=\"gantt-timeline\" style=\"width: ${totalDuration * pixelsPerUnit}px\">\n");
    fprintf(file, "                        ${timelineHtml}\n");
    fprintf(file, "                    </div>\n");
    fprintf(file, "                </div>\n");
    fprintf(file, "            `;\n");
    fprintf(file, "            ganttContainer.appendChild(section);\n");
    fprintf(file, "        });\n");
    fprintf(file, "    </script>\n");
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");

    fclose(file);
}

@echo off
echo Compiling C CPU Scheduler...
gcc -I src\headers -o cpu_scheduler.exe src\sources\main.c src\sources\utils.c src\sources\algorithms.c src\sources\html_render.c
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b %errorlevel%
)
echo Compilation successful. Running CPU Scheduler...
cpu_scheduler.exe
if exist results.html (
    echo Opening visualization dashboard...
    start results.html
)
pause

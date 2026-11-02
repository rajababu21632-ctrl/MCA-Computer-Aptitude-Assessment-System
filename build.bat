@echo off
echo =========================================================
echo Compiling MCA Computer Aptitude Assessment System...
echo =========================================================
g++ -std=c++17 -O2 src/*.cpp -Iinclude -o exam_system.exe
if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Compilation successful!
    echo Executable generated: exam_system.exe
) else (
    echo [ERROR] Compilation failed. Please check the errors above.
)
pause

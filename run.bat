@echo off
title MCA Computer Aptitude Assessment System - Chandigarh University
if not exist exam_system.exe (
    echo exam_system.exe not found! Compiling project first...
    call build.bat
)
exam_system.exe
pause

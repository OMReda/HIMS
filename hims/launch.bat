@echo off
title HIMS - Enterprise Hospital System
color 0B

echo =========================================================
echo               _   _  _____ __  __  _____ 
echo              ^| ^| ^| ^|^|_   _^|  \/  ^|/ ____^|
echo              ^| ^|_^| ^|  ^| ^| ^| \  / ^| (___  
echo              ^|  _  ^|  ^| ^| ^| ^|\/^| ^|\___ \ 
echo              ^| ^| ^| ^| _^| ^|_^| ^|  ^| ^|____) ^|
echo              ^|_^| ^|_^|^|_____^|_^|  ^|_^|_____/ 
echo.
echo     ENTERPRISE HOSPITAL INFORMATION MANAGEMENT SYSTEM
echo =========================================================
echo.
echo Checking for compiler and building HIMS...
gcc -Wall -Wextra -std=c99 -o hims.exe *.c

if %ERRORLEVEL% NEQ 0 (
    echo.
    color 0C
    echo [ERROR] Compilation failed! Please check the code or install GCC.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo Compilation successful. Starting HIMS...
timeout /t 2 /nobreak >nul
cls

hims.exe

pause

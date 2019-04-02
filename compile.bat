@echo off
set file=%1
set mode=%2
if %mode%==1 (g++ -std=c++11 %file%.cpp -o %file% -D_CLASS_CONVENTION) else (g++ -std=c++11 %file%.cpp -o %file%)
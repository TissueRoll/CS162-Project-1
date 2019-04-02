*---------------------*
| What is the program |
*---------------------*

It is a CPU scheduling simulator. It will output a kinda-Gantt chart and it will output statistics per test case.

*------------------------*
| How to run/use program |
*------------------------*

0. Please compile the code with argument -std=c++11
    e.g. g++ -std=c++11 CS162A_Project1_Dizon_code.cpp
0.1. To compile with the standard discussed in class (bonus), compule the code by the command below
    g++ -std=c++11 CS162A_Project1_Dizon_code.cpp -o CS162A_Project1_Dizon_code -D_CLASS_CONVENTION

1. Input data as follows:
# of test cases
[for # of test cases lines]
# of processes, scheduling algorithm (FCFS/SJF/SRTF/P/RR), timestamp for CPU utilization, timestamp for throughput
[for # of processes lines]
arrival time of process, job length of process, priority of process

2. Get output in this format:
[for every test case]
Test case #
[the kinda-Gantt chart]
time of execution, process ID, job length executed, X if process done

*--------------------------*
| Functions and Operations |
*--------------------------*

compute_metrics(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp, int util_until)
    - gets the statistics of an array of processes
    - cpu_util_timestamp provided by the user
    - throughput_timestamp provided by the user

xxxx(Process p[], int n, int cpu_util_timestamp, int throughput_timestamp)
    * xxxx is a placeholder, substitute it with any of the CPU scheduling algorithms
    - processes the processes using the CPU scheduling algorithm called


#pragma once
#include<string>
#include<algorithm>
#include<vector>
using namespace std;

struct data_to_output {
    double avgWaitingTime;
    double avgTurnAroundTime;
    vector<string> ganttChart;
};

struct Process {
    std::string processName;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnAroundTime;

    Process();
    Process(const string &processName, int arrivalTime, int burstTime, int priority);
};

bool sortByArrivalTime(const Process &a, const Process &b);

bool sortByPriority(const Process &a, const Process &b);

bool sortByRemainingTime(const Process &a, const Process &b);

data_to_output SJF_NonPreemptive(vector<Process>& processes);

data_to_output SJF_Preemptive(vector<Process>& processes);

data_to_output fcfs(vector<Process>& processes);

data_to_output non_preemptive_priority(vector<Process>processes);

data_to_output RR(vector<Process>& processes, int quantum);

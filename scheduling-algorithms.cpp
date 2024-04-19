#include"scheduling-algorithms.h"
#include<algorithm>
#include<queue>


Process::Process(){

}

Process::Process(const string &processName, int arrivalTime, int burstTime, int priority) {
    this->processName = processName;
    this->arrivalTime = arrivalTime;
    this->burstTime = burstTime;
    this->priority = priority;
    this->remainingTime = burstTime;
}

bool sortByArrivalTime(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

bool sortByPriority(const Process &a, const Process &b) {
    return a.priority < b.priority;
}

bool sortByRemainingTime(const Process &a, const Process &b) {
    return a.remainingTime > b.remainingTime;
}

data_to_output SJF_NonPreemptive(vector<Process>& processes) {
    vector<Process> v = processes;
    int numOfProcesses = v.size();
    sort(v.begin(),v.end(), sortByArrivalTime);

    int sum_of_time = 0;
    for (int i = 0; i < numOfProcesses; i++) sum_of_time += v[i].burstTime;
    int total_time = sum_of_time + v.back().arrivalTime;

    data_to_output ret;
    for(int i = 0 ; i < v[0].arrivalTime ; ++i) ret.ganttChart.push_back("x");



    int idx = 0;
    priority_queue<Process , vector<Process> , decltype(&sortByRemainingTime) > pq(&sortByRemainingTime);
    for(int second = v[0].arrivalTime ; second < total_time ; second++){
        while(v[idx].arrivalTime <= second && idx < numOfProcesses)
            pq.push(v[idx]), idx++;

        if(pq.empty()){
            ret.ganttChart.push_back("x");
            continue;
        }
        Process cur_process = pq.top();
        pq.pop();
        for(int j = second ; j < second + cur_process.burstTime ; ++j) ret.ganttChart.push_back(cur_process.processName);
        ret.avgWaitingTime += (second - cur_process.arrivalTime);
        ret.avgTurnAroundTime += (second + cur_process.burstTime - cur_process.arrivalTime);
        second += cur_process.burstTime - 1;
    }
    while(ret.ganttChart.back() == "x") ret.ganttChart.pop_back();
    ret.avgWaitingTime = ret.avgWaitingTime / (double)numOfProcesses;
    ret.avgTurnAroundTime = ret.avgTurnAroundTime / (double)numOfProcesses;
    return ret;
}

data_to_output SJF_Preemptive(vector<Process>& processes){
    vector<Process> v = processes;
    int numOfProcesses = v.size();
    sort(v.begin(),v.end(), sortByArrivalTime);
    int sum_of_time = 0;
    for (int i = 0; i < numOfProcesses; i++) sum_of_time += v[i].burstTime;
    int total_time = sum_of_time + v.back().arrivalTime;

    data_to_output ret;
    for(int i = 0 ; i < v[0].arrivalTime ; ++i) ret.ganttChart.push_back("x");

    int idx = 0;
    priority_queue<Process , vector<Process> , decltype(&sortByRemainingTime) > pq(&sortByRemainingTime);
    for(int second = v[0].arrivalTime ; second < total_time ; second++){
        while(v[idx].arrivalTime <= second && idx < numOfProcesses)
            pq.push(v[idx]), idx++;

        if(pq.empty()){
            ret.ganttChart.push_back("x");
            continue;
        }

        Process cur = pq.top() ; pq.pop();
        ret.ganttChart.push_back(cur.processName);
        cur.remainingTime--;
        if(cur.remainingTime == 0){
            //            cout<<cur.processName<<' '<<(second - cur.arrivalTime - cur.burstTime + 1)<<'\n';
            ret.avgTurnAroundTime += (second - cur.arrivalTime + 1);
            ret.avgWaitingTime += (second - cur.arrivalTime - cur.burstTime + 1);
        }
        else pq.push(cur);
    }
    while(ret.ganttChart.back() == "x") ret.ganttChart.pop_back();
    ret.avgWaitingTime = ret.avgWaitingTime / (double)numOfProcesses;
    ret.avgTurnAroundTime = ret.avgTurnAroundTime / (double)numOfProcesses;
    return ret;
}
data_to_output fcfs(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    int currentTime = 0;
    for (auto &process: processes) {
        currentTime = max(currentTime, process.arrivalTime);
        process.completionTime = currentTime + process.burstTime;
        process.waitingTime = currentTime - process.arrivalTime;
        process.turnAroundTime = process.completionTime - process.arrivalTime;
        currentTime = process.completionTime;
    }
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    for (const auto &process: processes) {
        totalWaitingTime += process.waitingTime;
        totalTurnaroundTime += process.turnAroundTime;
    }
    data_to_output data;
    int check = 0;
    for (auto &process: processes) {
        if (process.arrivalTime > check) {
            for (int k = 0; k < process.arrivalTime - check; ++k)
                data.ganttChart.emplace_back("x");
        }
        for (int j = 0; j < process.burstTime; ++j) {
            data.ganttChart.emplace_back(process.processName);
        }
        check = process.completionTime;
    }
    data.avgWaitingTime = totalWaitingTime / processes.size();
    data.avgTurnAroundTime = totalTurnaroundTime / processes.size();
    return data;
}

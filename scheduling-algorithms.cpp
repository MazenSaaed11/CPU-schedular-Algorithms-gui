#include "scheduling-algorithms.h"

// process struct
Process::Process() {
    priority = -1;
}
Process::Process(const string &processName, int arrivalTime, int burstTime, int priority) {
    this->processName = processName;
    this->arrivalTime = arrivalTime;
    this->burstTime = burstTime;
    this->priority = priority;
    this->remainingTime = burstTime;
}
void Process::initialize() {
    remainingTime = burstTime;
}

bool sortByArrivalTime(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

//note this function will sort descending so reverse after you sort if you need it amgad shrief el mo5eef 3amlha 3shan bs priority_queue f lazem tt3ml kda.
bool sortByRemainingTime(const Process &a, const Process &b) {
    return a.remainingTime > b.remainingTime;
}

data_to_output SJF_nonpreemptive(vector<Process>& processes) {
    vector<Process> v = processes;
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
    ret.avgWaitingTime = ret.avgWaitingTime / (float)numOfProcesses;
    ret.avgTurnAroundTime = ret.avgTurnAroundTime / (float)numOfProcesses;
    return ret;
}

data_to_output SJF_nonpreemptive(vector<Process>& processes){
    vector<Process> v = processes;
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

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
data_to_output preemptive_priority(vector<Process>& processes) {
    data_to_output out;
    vector<Process> readyQueue;
    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;

    out.avgWaitingTime = 0;
    out.avgTurnAroundTime = 0;

    while (completedProcesses < totalProcesses) {
        for (int i = 0; i < totalProcesses; ++i) {
            if (processes[i].arrivalTime == currentTime) {
                readyQueue.push_back(processes[i]);
                sort(readyQueue.begin(), readyQueue.end(), [](const Process& a, const Process& b) {
                    return a.priority < b.priority;
                });
            }
        }

        if (!readyQueue.empty()) {
            Process& currentProcess = readyQueue.front();
            currentProcess.remainingTime--;
            out.ganttChart.push_back(currentProcess.processName);
            if (currentProcess.remainingTime == 0) {
                currentProcess.completionTime = currentTime + 1;
                currentProcess.turnAroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnAroundTime - currentProcess.burstTime;
                out.avgWaitingTime += currentProcess.waitingTime;
                out.avgTurnAroundTime += currentProcess.turnAroundTime;
                readyQueue.erase(readyQueue.begin());
                completedProcesses++;
            }
        } else {
            out.ganttChart.emplace_back("x");
        }

        currentTime++;
    }

    out.avgWaitingTime /= totalProcesses;
    out.avgTurnAroundTime /= totalProcesses;

    return out;
}
data_to_output non_preemptive_priority(vector<Process> processes) {
    data_to_output out;
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;

    out.avgWaitingTime = 0;
    out.avgTurnAroundTime = 0;

    while (completedProcesses < totalProcesses) {
        int nextProcessIndex = -1;
        for (int i = 0; i < totalProcesses; ++i) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (nextProcessIndex == -1 || processes[i].priority < processes[nextProcessIndex].priority) {
                    nextProcessIndex = i;
                }
            }
        }

        if (nextProcessIndex != -1) {
            Process &currentProcess = processes[nextProcessIndex];
            for (int i = 0; i < currentProcess.burstTime; i++)
                out.ganttChart.push_back(currentProcess.processName);

            currentProcess.waitingTime = currentTime - currentProcess.arrivalTime;
            currentTime += currentProcess.burstTime;
            currentProcess.completionTime = currentTime;
            currentProcess.turnAroundTime = currentProcess.completionTime - currentProcess.arrivalTime;

            out.avgWaitingTime += currentProcess.waitingTime;
            out.avgTurnAroundTime += currentProcess.turnAroundTime;

            currentProcess.remainingTime = 0;
            completedProcesses++;
        } else {
            out.ganttChart.push_back("x");
            currentTime++;
        }
    }
    out.avgWaitingTime /= totalProcesses;
    out.avgTurnAroundTime /= totalProcesses;

    return out;
}

/**********************************************************************************
 *                           Function Name: RR                                    *
 *         Descripton: Implement the Round Robin scheduling algorithm             *
 *                Input: vector of processes and quantum time                     *
 **********************************************************************************/

data_to_output RR(vector<Process>& processes, int quantum)
{
    data_to_output out;

    /*
     * Check if the time quantum is zero or negative
     */
    if (quantum <= 0)
    {
        out.ganttChart.push_back("Zero or negative time quantum");
        return out;
    }


    /**********************************************************************************
     *                                  Variables                                     *
     **********************************************************************************/

    int numOfProcesses = (int)processes.size();
    vector<string> ganttChart;

    /*
     *  Vector to track whether each process is completed
     */
    vector<bool> done(numOfProcesses, false);

    int completedProcesses = 0;
    int currentTime = 0;
    int processIndex = 0;
    int sumWaitingTime = 0;
    int sumTurnAroundTime = 0;
    /**********************************************************************************/

    /*
     *  Sort processes by arrival time
     */
    sort(processes.begin(), processes.end(), sortByArrivalTime);

    /*
     * Handles case if the arrival time of the first process is greater than zero
     */
    if (processes[0].arrivalTime > 0)
    {
        /*
         * Mark gantChart as idle "x"
         */
        for (int t = 0; t < processes[0].arrivalTime; t++)
        {
            ganttChart.push_back("x");
        }
    }

    /*
     * Initialize remaining time for each process
     */
    for (int i = 0; i < numOfProcesses; i++)
    {
        processes[i].remainingTime = processes[i].burstTime;
    }

    while (completedProcesses < numOfProcesses)
    {
        bool executed = false;
        for (int i = 0; i < numOfProcesses; ++i) 
        {
            if (processes[i].remainingTime > 0 && processes[i].arrivalTime <= currentTime) 
            {
                int executeTime = min(quantum, processes[i].remainingTime);
                /*
                 * Add the process to the Gantt chart
                 */
                for (int j = 0; j < executeTime; ++j) 
                {
                    ganttChart.push_back(processes[i].processName);
                }

                processes[i].remainingTime -= executeTime;
                currentTime += executeTime;

                if (processes[i].remainingTime == 0) 
                {
                    processes[i].finishTime = currentTime;
                    processes[i].turnAroundTime = processes[i].finishTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].finishTime - processes[i].burstTime - processes[i].arrivalTime;
                    sumTurnAroundTime += processes[i].turnAroundTime;
                    sumWaitingTime += processes[i].waitingTime;
                    completedProcesses++;
                }
                executed = true;
            }
        }
        /*
         * If no process was executed in this iteration, mark it as idle in the Gantt chart 
         */
        if (!executed)
        {
            ganttChart.push_back("x");
            currentTime ++;
        }
    }

    /*
     * Calculate average waiting time and average turn-around time
     */

    float avgWaitingTime = (float)sumWaitingTime / numOfProcesses;
    float avgTurnAroundTime = (float)sumTurnAroundTime / numOfProcesses;

    /*
     * Fill struct of output
     */

    out.avgTurnAroundTime = avgTurnAroundTime;
    out.avgWaitingTime = avgWaitingTime;
    out.ganttChart = ganttChart;

    return out;
}

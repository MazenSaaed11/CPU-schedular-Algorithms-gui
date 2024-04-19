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
    void initialize();
};

bool sortByArrivalTime(const Process &a, const Process &b);

//note this function will sort descending so reverse after you sort if you need it amgad shrief el mo5eef 3amlha 3shan bs priority_queue f lazem tt3ml kda.
bool sortByRemainingTime(const Process &a, const Process &b);

//SJF
data_to_output SJF_preemptive(vector<Process>& processes);
data_to_output SJF_nonpreemptive(vector<Process>& processes);

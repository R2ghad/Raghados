#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int start_time;
    int remaining_time;
};

vector<Process> readProcesses(const string& filename) {
    ifstream file(filename);
    vector<Process> processes;
    int pid, arrival_time, burst_time;
    while (file >> pid >> arrival_time >> burst_time) {
        processes.push_back({ pid, arrival_time, burst_time, 0, 0, 0, 0, 0, burst_time });
    }
    file.close();
    return processes;
}

void FCFS(vector<Process>& processes, int context_switch_time) {
    int current_time = 0;
    for (auto& process : processes) {
        process.start_time = max(current_time, process.arrival_time);
        current_time = process.start_time + process.burst_time;
        process.completion_time = current_time;
        process.turnaround_time = process.completion_time - process.arrival_time;
        process.waiting_time = process.turnaround_time - process.burst_time;
        process.response_time = process.start_time - process.arrival_time;
        current_time += context_switch_time; // Context switch
    }
}

void SRT(vector<Process>& processes, int context_switch_time) {
    int current_time = 0;
    int completed = 0;
    while (completed < processes.size()) {
        int shortest_index = -1;
        int shortest_remaining_time = INT_MAX;
        for (int i = 0; i < processes.size(); ++i) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time < shortest_remaining_time && processes[i].remaining_time > 0) {
                shortest_remaining_time = processes[i].remaining_time;
                shortest_index = i;
            }
        }
        if (shortest_index == -1) {
            current_time++;
        }
        else {
            Process& current_process = processes[shortest_index];
            current_process.start_time = current_time;
            current_process.remaining_time--;
            current_time++;
            if (current_process.remaining_time == 0) {
                current_process.completion_time = current_time;
                current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
                current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;
                current_process.response_time = current_process.start_time - current_process.arrival_time;
                completed++;
                current_time += context_switch_time;
            }
        }
    }
}

void RoundRobin(vector<Process>& processes, int context_switch_time, int quantum) {
    int current_time = 0;
    int completed = 0;
    vector<int> remaining_time(processes.size());

    for (int i = 0; i < processes.size(); ++i) {
        remaining_time[i] = processes[i].burst_time;
    }

    while (completed < processes.size()) {
        for (int i = 0; i < processes.size(); ++i) {
            if (remaining_time[i] > 0) {
                if (remaining_time[i] > quantum) {
                    current_time += quantum;
                    remaining_time[i] -= quantum;
                }
                else {
                    current_time += remaining_time[i];
                    remaining_time[i] = 0;
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    processes[i].response_time = processes[i].start_time - processes[i].arrival_time;
                    completed++;
                }
                current_time += context_switch_time; // Context switch
            }
        }
    }
}

void displayResults(const vector<Process>& processes) {
    float total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
    float cpu_utilization = 0;
    cout << "Process No.\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n";
    for (const auto& process : processes) {
        cout << process.pid << "\t\t" << process.arrival_time << "\t\t" << process.burst_time << "\t\t" << process.completion_time << "\t\t"
            << process.turnaround_time << "\t\t" << process.waiting_time << "\t\t" << process.response_time << endl;
        total_waiting_time += process.waiting_time;
        total_turnaround_time += process.turnaround_time;
        total_response_time += process.response_time;
    }
    cpu_utilization = (total_turnaround_time - total_waiting_time) / processes.back().completion_time * 100;
    cout << "\nAverage Turnaround Time: " << total_turnaround_time / processes.size();
    cout << "\nAverage Waiting Time: " << total_waiting_time / processes.size();
    cout << "\nAverage Response Time: " << total_response_time / processes.size();
    cout << "\nCPU Utilization: " << cpu_utilization << "%\n";
}

int main() {
    vector<Process> processes;
    string filename = "C:\\Users\\Baraa\\Downloads\\RAGHAD.txt";
    processes = readProcesses(filename);
    int context_switch_time = 1;
    int quantum = 2;

    // FCFS
    vector<Process> fcfs_processes = processes;
    sort(fcfs_processes.begin(), fcfs_processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
        });
    FCFS(fcfs_processes, context_switch_time);
    cout << "FCFS Schedule:\n";
    displayResults(fcfs_processes);

    // SRT
    vector<Process> srt_processes = processes;
    sort(srt_processes.begin(), srt_processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
        });
    SRT(srt_processes, context_switch_time);
    cout << "\nSRT Schedule:\n";
    displayResults(srt_processes);

    // Round Robin
    vector<Process> rr_processes = processes;
    sort(rr_processes.begin(), rr_processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
        });
    RoundRobin(rr_processes, context_switch_time, quantum);
    cout << "\nRound Robin Schedule:\n";
    displayResults(rr_processes);

    return 0;
}
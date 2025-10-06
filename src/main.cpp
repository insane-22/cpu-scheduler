#include <iostream>
#include "scheduler.hpp"
#include "implementation.hpp"
#include "simulator.hpp"

void print_stats(const Result &r){
    std::cout << "Average Waiting Time: " << r.avg_waiting << "\n";
    std::cout << "Average Turnaround Time: " << r.avg_turnaround << "\n";
    std::cout << "Average Response Time: " << r.avg_response << "\n";
    std::cout << "CPU Utilization: " << r.cpu_utilization << "%\n";
    std::cout << "Throughput: " << r.throughput << " tasks/unit time\n";
    std::cout << "Context Switches: " << r.context_switches << "\n";
};

int main() {
    std::vector<Task> tasks = {
        Task(1, 0, 5),
        Task(2, 2, 3),
        Task(3, 4, 8)
    };

    FCFS fcfs;
    Result r1 = simulate(tasks, fcfs, 50);
    std::cout << "\n--- FCFS ---\n";
    std::cout << "Total CPU Busy Time: " << r1.cpu_busy << "\n";
    for (auto &e : r1.events)
        std::cout << "t=" << e.time << " pid=" << e.pid << " " << e.type << "\n";
    std::cout << "\n--- FCFS Stats ---\n";
    print_stats(r1);

    RoundRobin rr(2);
    Result r2 = simulate(tasks, rr, 50);
    std::cout << "\n--- RR (q=2) ---\n";
    for (auto &e : r2.events)
        std::cout << "t=" << e.time << " pid=" << e.pid << " " << e.type << "\n";

    std::cout << "\n--- RR Stats ---\n";
    print_stats(r2);

    SJF sjf;
    std::unordered_map<int,int> bm;
    for (auto &t : tasks) bm[t.pid] = t.burst;
    sjf.set_bursts(bm);
    Result r3 = simulate(tasks, sjf, 50);
    std::cout << "\n--- SJF ---\n";
    for (auto &e : r3.events)
        std::cout << "t=" << e.time << " pid=" << e.pid << " " << e.type << "\n";   
    std::cout << "\n--- SJF Stats ---\n";
    print_stats(r3);

    SRTF srtf;
    Result r4 = simulate(tasks, srtf, 50);
    std::cout << "\n--- SRTF ---\n";
    for (auto &e : r4.events)
        std::cout << "t=" << e.time << " pid=" << e.pid << " " << e.type << "\n";
    std::cout << "\n--- SRTF Stats ---\n";  
    print_stats(r4);
    return 0;
}

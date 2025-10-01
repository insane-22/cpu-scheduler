#include <iostream>
#include "scheduler.hpp"
#include "implementation.hpp"
#include "simulator.hpp"

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

    RoundRobin rr(2);
    Result r2 = simulate(tasks, rr, 50);
    std::cout << "\n--- RR (q=2) ---\n";
    for (auto &e : r2.events)
        std::cout << "t=" << e.time << " pid=" << e.pid << " " << e.type << "\n";

    return 0;
}

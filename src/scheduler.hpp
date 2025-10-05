#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <unordered_map>
#include <string>

struct Task {
    int pid = 0;
    int arrival = 0;
    int burst = 0;
    int remaining = 0;
    int priority = 0;
    int deadline = -1;

    int start_time = -1;
    int completion_time = -1;
    int response_time = -1;
    int waiting_time = 0;
    int turnaround_time = 0;

    Task() = default;
    Task(int id, int arr, int bur, int pri=0) : pid(id), arrival(arr), burst(bur), remaining(bur), priority(pri) {}
};

struct Event {
    int time;
    int pid;
    std::string type;  
};

struct Result {
    std::vector<Event> events;
    std::unordered_map<int, Task> tasks;
    int total_ticks = 0;
    int context_switches = 0;
    int cpu_busy = 0;

    double avg_waiting = 0;
    double avg_turnaround = 0;
    double avg_response = 0;
    double cpu_utilization = 0;
    double throughput = 0;
};


class Scheduler {
public:
    virtual void add_task(int pid) = 0;
    virtual int pick_next(int now) = 0;
    virtual ~Scheduler() {}
};

#endif

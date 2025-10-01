#include "simulator.hpp"
#include "implementation.hpp"
#include <algorithm>

Result simulate(std::vector<Task> tasks, Scheduler &sched, int max_time) {
    Result res;
    std::unordered_map<int, Task*> tmap;
    for (auto &t: tasks) tmap[t.pid] = &t;

    std::sort(tasks.begin(), tasks.end(), [](auto &a, auto &b){
        return a.arrival < b.arrival;
    });

    int n = tasks.size();
    int ai = 0;             
    int finished = 0;
    int now = 0;
    int running = -1;
    int quantum_used = 0;

    RoundRobin *rr = dynamic_cast<RoundRobin*>(&sched);

    while (now < max_time && finished < n) {
        while (ai < n && tasks[ai].arrival <= now) {
            sched.add_task(tasks[ai].pid);
            res.tasks[tasks[ai].pid] = tasks[ai];
            ai++;
        }
    
        if (running == -1) {
            running = sched.pick_next(now);
            quantum_used = 0;
            if (running != -1) {
                auto &t = *tmap[running];
                if (t.start_time == -1) {
                    t.start_time = now;
                    t.response_time = now - t.arrival;
                }
                res.events.push_back({now, running, "start"});
            }
        }

        if (running != -1) {
            auto &t = *tmap[running];
            t.remaining--;
            res.cpu_busy++;
            quantum_used++;
            now++;

            if (t.remaining == 0) {
                t.completion_time = now;
                t.turnaround_time = t.completion_time - t.arrival;
                t.waiting_time = t.turnaround_time - t.burst;
                res.events.push_back({now, running, "complete"});
                running = -1;
                finished++;
            } else if (rr && quantum_used >= rr->get_quantum()) {
                rr->requeue(running);
                res.events.push_back({now, running, "preempt"});
                running = -1;
                res.context_switches++;
            }
        } else {
            now++; 
        }
    }

    res.total_ticks = now;
    for (auto &t : tasks) res.tasks[t.pid] = t;
    return res;
}

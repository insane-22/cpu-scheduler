#ifndef IMPLEMENTATION_HPP
#define IMPLEMENTATION_HPP

#include "scheduler.hpp"
#include <queue>
#include <unordered_map>
#include <algorithm>

class FCFS : public Scheduler {
    std::queue<int>q;
public:
    void add_task(int pid) {
        q.push(pid); 
    }

    int pick_next(int now) {
        if (q.empty()) return -1;
        int pid = q.front(); 
        q.pop();
        return pid;
    }
};

class RoundRobin : public Scheduler {
    std::queue<int> rq;
    std::unordered_map<int,int> slice_used;
    int quantum;
public:
    RoundRobin(int q) : quantum(q) {}
    void add_task(int pid) {
        rq.push(pid);
        slice_used[pid] = 0;
    }
    int pick_next(int now) {
        if (rq.empty()) return -1;
        int pid = rq.front(); rq.pop();
        return pid;
    }
    void requeue(int pid) { rq.push(pid); }
    int get_quantum() const { return quantum; }
};

class SJF : public Scheduler {
    std::vector<int> ready;
    std::unordered_map<int,int> burst_map;
public:
    void add_task(int pid) override {
        ready.push_back(pid);
    }
    int pick_next(int now) override {
        if (ready.empty()) return -1;
        int pid = ready[0];
        for (int p : ready)
            if (burst_map[p] < burst_map[pid]) pid = p;
        ready.erase(std::remove(ready.begin(), ready.end(), pid), ready.end());
        return pid;
    }
    void set_bursts(const std::unordered_map<int,int>& m){ burst_map = m; }
};

class SRTF : public Scheduler {
    std::vector<int> ready;
    std::unordered_map<int,int> remaining;
public:
    void add_task(int pid) override {
        ready.push_back(pid);
    }
    int pick_next(int now) override {
        if (ready.empty()) return -1;
        int pid = ready[0];
        for (int p : ready)
            if (remaining[p] < remaining[pid]) pid = p;
        ready.erase(std::remove(ready.begin(), ready.end(), pid), ready.end());
        return pid;
    }
    void update_remaining(int pid, int rem){ remaining[pid] = rem; }
};



#endif

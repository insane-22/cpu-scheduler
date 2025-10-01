#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "scheduler.hpp"

Result simulate(std::vector<Task> tasks, Scheduler &sched, int max_time=100);

#endif

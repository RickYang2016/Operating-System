#pragma once

#include <map>
#include <queue>
#include <fstream>
#include "../include/Print_Output.h"
#include "../include/Choice.h"
#include "../include/Planner.h"
#include "../include/Sys_Parameter.h"

using namespace std;

class Simulation
{
public:
	Simulation( Planner *planner, Print_Output *print_output );
	void run( string &file );

private:
	map<int, Process*> pidMap;
	priority_queue<Event*, vector<const Event*>, comparison> events;
	Planner *planner;
	Print_Output *print_output;
	Thread *currentThread = NULL;
	Thread *previousThread = NULL;
	unsigned int thread_switch_overhead;
	unsigned int process_switch_overhead;
	unsigned int dispatch_time = 0;
	unsigned int start_time = 0;
	unsigned int finished_time = 0;

	void fileIn( string &file );
	void updateTimes( const Event *event );
	void thread_arrived_update( const Event *event );
	void dispatcher_invoked_update( const Event *event );
	void dispatch_complete_update( const Event *event );
	void thread_preempted_update( const Event *event );
	void cpu_burst_complete_update( const Event *event );
	void io_burst_complete_update( const Event *event );
	void thread_complete_update( const Event *event );
	void printState();
};
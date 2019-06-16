#pragma once

#include "../include/Planner.h"

using namespace std;

class FCFS : public Planner
{
public:
	Choice* get_next_thread( const Event* event );
	void enqueue( const Event* event, Thread* thread );

private:
	queue<Thread*> threads;
};
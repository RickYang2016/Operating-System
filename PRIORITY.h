#pragma once

#include "Planner.h"

using namespace std;

class PRIORITY : public Planner
{
public:
	Choice *get_next_thread( const Event* event );
	void enqueue( const Event* event, Thread* thread );

private:
	queue<Thread*> threads[4] = { queue<Thread*>(), queue<Thread*>(), queue<Thread*>(), queue<Thread*>() };
};
#pragma once

#include "../include/Planner.h"

using namespace std;

const static int RR_Num = 3;

class RR : public Planner
{
public:
	Choice* get_next_thread( const Event* event );
	void enqueue( const Event* event, Thread* thread );

private:
	queue<Thread*> threads;
};
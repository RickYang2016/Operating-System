#pragma once

#include "Planner.h"

using namespace std;

const static int CUSTOM_Num = 6;

struct CMP
{
	bool operator()( Thread *thread1, Thread *thread2 )
	{
		int priority1 = (int)thread1 -> process -> get_type() + 1;
		unsigned int responseTime1 = thread1 -> get_response_time();
		unsigned int turnaroundTime1 = thread1 -> get_turnaround_time();
		unsigned int weight1 = priority1 * responseTime1 + priority1 * turnaroundTime1;

		int priority2 = (int)thread2 -> process -> get_type() + 1;
		unsigned int responseTime2 = thread2 -> get_response_time();
		unsigned int turnaroundTime2 = thread2 -> get_turnaround_time();
		unsigned int weight2 = priority2 * responseTime2 + priority2 * turnaroundTime2;
		return weight1 <= weight2;
	}
};

class CUSTOM : public Planner
{
public:
	Choice *get_next_thread( const Event *event );
	void enqueue( const Event* event, Thread* thread );

private:
	priority_queue<Thread*, vector<Thread*>, CMP> new_queue;
};

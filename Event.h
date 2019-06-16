#pragma once

#include "../include/Thread.h"
#include "../include/Choice.h"

using namespace std;

class Event
{
public:
	enum Type{ THREAD_ARRIVED, THREAD_DISPATCH_COMPLETED, PROCESS_DISPATCH_COMPLETED, CPU_BURST_COMPLETED, IO_BURST_COMPLETED, THREAD_COMPLETED, THREAD_PREEMPTED, DISPATCHER_INVOKED };
	Event( Type type, unsigned int time, Thread* thread, Choice* choice );
	~Event();
	unsigned int get_time() const;
	Type get_type() const;
	const Choice* get_choice() const;
	static string get_type_name( Type type );
	Thread* thread;

private:
	Type type;
	unsigned int time;
	const Choice* choice;
	static string TypeArray[8];
};

struct comparison
{
	bool operator()( const Event* e1, const Event* e2 )
	{
		return e1 -> get_time() >= e2 -> get_time();
	}
};


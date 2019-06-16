#include "../include/Event.h"

using namespace std;

string Event::TypeArray[] = { "THREAD_ARRIVED", "THREAD_DISPATCH_COMPLETED", "PROCESS_DISPATCH_COMPLETED", "CPU_BURST_COMPLETED", "IO_BURST_COMPLETED", "THREAD_COMPLETED", "THREAD_PREEMPTED", "DISPATCHER_INVOKED" };

Event::Event( Type type, unsigned int time, Thread* thread, Choice* choice )
{
	this -> type = type;
	this -> time = time;
	this -> thread = thread;
	this -> choice = choice;
}

Event::~Event()
{
	if( choice )
		delete choice;
}

unsigned int Event::get_time() const
{
	return time;
}

Event::Type Event::get_type() const
{
	return type;
}

const Choice* Event::get_choice() const
{
	return choice;
}

string Event::get_type_name( Event::Type type )
{
	return TypeArray[type];
}
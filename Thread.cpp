#include "../include/Thread.h"

using namespace std;

string Thread::StateArray[] = { "NEW", "READY", "RUNNING", "BLOCKED", "EXIT" };

Thread::Thread( int id, Process* process, unsigned int arrival_time )
{
	this -> id = id;
	this -> process = process;
	this -> arrival_time = arrival_time;
}

int Thread::get_id()
{
	return id;
}

void Thread::set_ready( unsigned int time )
{
	preview_state = current_state;
	current_state = State::READY;

	if( preview_state == State::RUNNING || preview_state == State::BLOCKED )
	{
		do_update( time );
	}

	last_state_change = time;
}

unsigned int Thread::set_running( unsigned int time )
{
	preview_state = current_state;
	current_state = State::RUNNING;

	if( bursts.size() == 0 )
	{
		return -1;
	}

	if( start_time == -1 )
	{
		start_time = time;
	}

	last_state_change = time;
	return bursts.front() -> get_length();
}

unsigned int Thread::set_blocked( unsigned int time )
{
	do_update( time );
	if( bursts.size() == 0 )
	{
		return -1;
	}
	preview_state = current_state;
	current_state = State::BLOCKED;

	last_state_change = time;
	return bursts.front() -> get_length();
}

void Thread::set_finished( unsigned int time )
{
	preview_state = current_state;
	current_state = State::EXIT;
	finished_time = time;
	last_state_change = time;
}

void Thread::pushBurst( Burst* burst )
{
	bursts.push( burst );
}

unsigned int Thread::get_arrival_time()
{
	return arrival_time;
}

unsigned int Thread::get_service_time()
{
	return service_time;
}

unsigned int Thread::get_io_time()
{
	return io_time;
}

unsigned int Thread::get_finished_time()
{
	return finished_time;
}

unsigned int Thread::get_response_time()
{
	return start_time - arrival_time;
}

unsigned int Thread::get_turnaround_time()
{
	return finished_time - arrival_time;
}

string Thread::get_state_name( Thread::State thread_state )
{
	return StateArray[thread_state];
}

void Thread::do_update( unsigned int time )
{
	Burst *burst = bursts.front();

	if( burst -> get_length() <= time - last_state_change )
	{
		bursts.pop();
	}
	else
	{
		burst -> decrement_length( time - last_state_change );
	}

	if( burst -> get_type() == Burst::Type::CPU )
	{
		service_time += time - last_state_change;
	}
	else if( burst -> get_type() == Burst::Type::IO )
	{
		io_time += time - last_state_change;
	}
}
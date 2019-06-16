#include "../include/Simulation.h"

using namespace std;

Simulation::Simulation( Planner *planner, Print_Output *print_output )
{
	this -> planner = planner;
	this -> print_output = print_output;
}

void Simulation::run( string &file )
{
	fileIn( file );

	while( !events.empty() )
	{
		const Event* event = events.top();
		events.pop();

		updateTimes( event );

		switch( event -> get_type() )
		{
			case Event::THREAD_ARRIVED:
			{
				thread_arrived_update( event );
				break;
			}

			case Event::DISPATCHER_INVOKED:
			{
				dispatcher_invoked_update( event );
				break;
			}

			case Event::THREAD_DISPATCH_COMPLETED:
			{
				dispatch_complete_update( event );
				break;
			}

			case Event::PROCESS_DISPATCH_COMPLETED:
			{
				dispatch_complete_update( event );
				break;
			}

			case Event::THREAD_PREEMPTED:
			{
				thread_preempted_update( event );
				break;
			}

			case Event::CPU_BURST_COMPLETED:
			{
				cpu_burst_complete_update( event );
				break;
			}

			case Event::IO_BURST_COMPLETED:
			{
				io_burst_complete_update( event );
				break;
			}

			case Event::THREAD_COMPLETED:
			{
				thread_complete_update( event );
				break;
			}
		}
		delete event;
	}
	printState();
}

void Simulation::thread_arrived_update( const Event *event )
{
	event -> thread -> set_ready( event -> get_time() );
	planner -> enqueue( event, event -> thread );

	if( !currentThread )
	{
		events.push( new Event( Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL ) );
	}
	print_output -> transitions( event, Thread::State::NEW, Thread::State::READY );
}

void Simulation::dispatcher_invoked_update( const Event *event )
{
	Choice *choice = planner -> get_next_thread( event );
	if( !choice -> thread )
	{
		return;
	}

	currentThread = choice -> thread;

	if( currentThread && previousThread && previousThread -> process == currentThread -> process )
	{
		events.push( new Event( Event::Type::THREAD_DISPATCH_COMPLETED, event -> get_time() + thread_switch_overhead, choice -> thread, choice ) );
		dispatch_time += thread_switch_overhead;
	}
	else
	{
		events.push( new Event( Event::Type::PROCESS_DISPATCH_COMPLETED, event -> get_time() + process_switch_overhead, choice -> thread, choice ) );
		dispatch_time += process_switch_overhead;
	}
	print_output -> dispatcher( event, choice -> thread, choice -> get_explanation() );
}

void Simulation::dispatch_complete_update( const Event *event )
{
	int burst_length = event -> thread -> set_running( event -> get_time() );
	int slice_length = event -> get_choice() -> get_slice();

	if( slice_length != -1 && slice_length < burst_length )
	{
		events.push( new Event( Event::Type::THREAD_PREEMPTED, event -> get_time() + slice_length, event -> thread, NULL ) );
	}
	else if( burst_length == -1 )
	{
		events.push( new Event( Event::Type::THREAD_COMPLETED, event -> get_time(), event -> thread, NULL ) );
		return;
	}
	else
	{
		events.push( new Event( Event::Type::CPU_BURST_COMPLETED, event -> get_time() + burst_length, event -> thread, NULL ) );
	}

	print_output -> transitions( event, Thread::State::READY, Thread::State::RUNNING );
}

void Simulation::thread_preempted_update( const Event *event )
{
	event -> thread -> set_ready( event -> get_time() );
	planner -> enqueue( event, event -> thread );
	previousThread = currentThread;
	currentThread = NULL;

	if( !currentThread )
	{
		events.push( new Event( Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL ) );
	}

	print_output -> transitions( event, Thread::State::RUNNING, Thread::State::READY );
}

void Simulation::cpu_burst_complete_update( const Event *event )
{
	int burst_length = event -> thread -> set_blocked( event -> get_time() );

	if( burst_length == -1 )
	{
		events.push( new Event( Event::Type::THREAD_COMPLETED, event -> get_time(), event -> thread, NULL ) );
		return;
	}

	previousThread = currentThread;
	currentThread = NULL;

	events.push( new Event( Event::Type::IO_BURST_COMPLETED, event -> get_time() + burst_length, event -> thread, NULL ) );

	if( !currentThread )
	{
		events.push( new Event( Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL ) );

	}

	print_output -> transitions( event, Thread::State::RUNNING, Thread::State::BLOCKED );
}

void Simulation::io_burst_complete_update( const Event *event )
{
	event -> thread -> set_ready( event -> get_time() );
	planner -> enqueue( event, event -> thread );

	if( !currentThread )
	{
		events.push( new Event( Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL ) );
	}

	print_output -> transitions( event, Thread::State::BLOCKED, Thread::State::READY );
}

void Simulation::thread_complete_update( const Event *event )
{
	event -> thread -> set_finished( event -> get_time() );

	previousThread = currentThread;
	currentThread = NULL;

	if( !currentThread )
	{
		events.push( new Event( Event::Type::DISPATCHER_INVOKED, event -> get_time(), NULL, NULL ) );
	}

	print_output -> transitions( event, Thread::State::RUNNING, Thread::State::EXIT );
}

void Simulation::fileIn( string &file )
{
	ifstream simFile( file );

	if( !simFile )
	{
		cerr << "Cannot open the file: " << file << endl;
		exit( -2 );
	}

	int num_processes = -1;
	int pid = 0;
	int type = 0;
	int num_threads = 0;
	int arrival_time = 0;
	int num_bursts = 0;
	int burst_length = 0;

	simFile >> num_processes >> thread_switch_overhead >> process_switch_overhead;

	for( int i = 0; i < num_processes; i++ )
	{
		simFile >> pid >> type >> num_threads;

		Process* process = new Process( pid, static_cast<Process::Type>(type) );

		for( int j = 0; j < num_threads; j++ )
		{
			simFile >> arrival_time >> num_bursts;

			Thread* thread = new Thread( j, process, arrival_time );

			for( int k = 0; k < num_bursts; k++ )
			{
				for( int l = 0; l < 2 && k + l < num_bursts; l++ )
				{
					simFile >> burst_length;
					Burst *burst = new Burst( static_cast<Burst::Type>(l), burst_length );

					thread -> pushBurst( burst );
				}
			}

			process -> pushThread( thread );

			Event* event = new Event( Event::Type::THREAD_ARRIVED, arrival_time, thread, NULL );

			events.push( event );
		}

		pidMap[pid] = process;
	}
}

void Simulation::updateTimes( const Event *event )
{
	if( event -> get_time() > finished_time )
	{
		finished_time = event -> get_time();
	}

	if( event -> get_time() < start_time )
	{
		start_time = event -> get_time();
	}
}

void Simulation::printState()
{
	Sys_Parameter sys_state = {0};

	for( map<int, Process*>::iterator iterate = pidMap.begin(); iterate != pidMap.end(); iterate++ )
	{
		print_output -> process( iterate -> second );
		Process::Type type = iterate -> second -> get_type();
		const vector<Thread*> threads = iterate -> second -> getThreads();

		for( int i = 0; i< (int)threads.size(); i++ )
		{
			int n = ++sys_state.count_threads[type];

			sys_state.response_time[type] = sys_state.response_time[type] * ( n - 1 ) / (n) + threads[i] -> get_response_time() * 1.0 / n;
			sys_state.tat_time[type] = sys_state.tat_time[type] * ( n - 1 ) / (n) + threads[i] -> get_turnaround_time() * 1.0 / n;
			sys_state.service_time += threads[i] -> get_service_time();
			sys_state.io_time += threads[i] -> get_io_time();
		}
	}

	sys_state.elapsed_time = finished_time - start_time;
	sys_state.dispatch_time = dispatch_time;
	sys_state.idle_time = sys_state.elapsed_time - sys_state.dispatch_time - sys_state.service_time;
	sys_state.cpu_utility = ( 100.0 * ( sys_state.dispatch_time + sys_state.service_time ) ) / sys_state.elapsed_time;
	sys_state.cpu_efficiency = ( 100.0 * sys_state.service_time ) / sys_state.elapsed_time;

	print_output -> state( sys_state );
}
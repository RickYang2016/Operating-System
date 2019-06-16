#include "../include/Print_Output.h"

using namespace std;

void Print_Output::transitions( const Event *event, Thread::State before_state, Thread::State after_state )
{
	if( verbose )
	{
		cout << "At the time " << event -> get_time() << ":" << endl;
		cout << Event::get_type_name( event -> get_type() ) << endl;
		cout << "Thread " << event -> thread -> get_id() << " in process " << event -> thread -> process -> get_pid();
		cout << "[" << Process::get_type_name( event -> thread -> process -> get_type() ) << "]" << endl;
		cout << "Transitioned from " << Thread::get_state_name( before_state ) << " to " << Thread::get_state_name( after_state ) << endl;
		cout << endl;
	}
}

void Print_Output::dispatcher( const Event *event, Thread *thread, string message )
{
	if( verbose )
	{
		cout << " At the time " << event -> get_time() << ":" << endl;
		cout << Event::get_type_name( event -> get_type() ) << endl;
		cout << "Thread " << thread -> get_id() + 1 << " in process " << thread -> process -> get_pid();
		cout << "[" << Process::get_type_name( thread -> process -> get_type() ) << "]" << endl;
		cout << message << endl;
		cout << endl;
	}
}

void Print_Output::process( Process *process ) const
{
	if( perThread )
	{
		cout << "Process " << process -> get_pid() << " [" << Process::get_type_name( process -> get_type() ) << "]:" << endl;
		const vector<Thread*> threads = process -> getThreads();
		for( int i = 0; i < (int)threads.size(); i++ )
		{
			cout << left << "\tThread " << threads[i] -> get_id() << ": " << "   ARR: " << setw(6) << threads[i] -> get_arrival_time();
			cout << "CPU: " << setw(6) << threads[i] -> get_service_time() << "I/O: " << setw(6) << threads[i] -> get_io_time();
			cout << "TRT: " << setw(6) << threads[i] -> get_turnaround_time() << "Finish: " << setw(6) << threads[i] -> get_finished_time();
			cout << endl;
		}
		cout << endl;
	}
}

void Print_Output::state( Sys_Parameter state ) const
{
	for( int  i = 0; i < 4; i++ )
	{
		cout << Process::get_type_name( ( Process::Type ) i ) << " Threads:" << endl;
		cout << "\t" << "Total count: " <<  "\t\t" << right << setw(6) << state.count_threads[i] << endl;
		cout << "\t" << "Average response time:" << "\t" << right << setw(6) << fixed << setprecision(2) << state.response_time[i] << endl;
		cout << "\t" << "Average turnaround time:" << right << setw(6) << fixed << setprecision(2) << state.tat_time[i] << endl;
		cout << endl;
	}

	cout << "Total elapsed time:" << "\t\t" << right << setw(6) << state.elapsed_time << endl;
	cout << "Total service time:" << "\t\t" << right << setw(6) << state.service_time << endl;
	cout << "Total I/O time:" << "\t\t\t" << right << setw(6) << state.io_time << endl;
	cout << "Total dispatch time:" << "\t\t" << right << setw(6) << state.dispatch_time << endl;
	cout << "Total idle time:" << "\t\t" << right << setw(6) << state.idle_time << endl;
	cout << "CPU utilization:" << "\t\t" << right << setw(6) << state.cpu_utility << "%" << endl;
	cout << "CPU efficiency:" << "\t\t\t" << right << setw(6) << state.cpu_efficiency << "%" << endl;
}
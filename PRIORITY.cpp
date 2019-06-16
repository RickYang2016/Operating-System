#include "../include/PRIORITY.h"

using namespace std;

Choice* PRIORITY::get_next_thread( const Event* event )
{
	Thread *nextThread = NULL;
	int j = 0;

	for( int i = 0; i < 4; i++ )
	{
		if( !threads[i].empty() )
		{
			nextThread = threads[i].front();
			threads[i].pop();
			break;
		}
	}

	char issue[300];
	sprintf( issue, "Selected from %s queue with %u thread; will run to completion of burst.", nextThread ? Process::get_type_name( nextThread -> process -> get_type() ).c_str() : "", ( unsigned int )threads[j].size() + 1 );
	string reason = string( issue );

	Choice *choice = new Choice( nextThread, -1, reason );
	return choice;
}

void PRIORITY::enqueue( const Event* event, Thread* thread )
{
	int priority = thread -> process -> get_type();
	threads[priority].push( thread );
}
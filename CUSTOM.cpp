#include "../include/CUSTOM.h"

using namespace std;

Choice* CUSTOM::get_next_thread( const Event *event )
{
	Thread *nextThread = NULL;

	if( !new_queue.empty() )
	{
		nextThread = new_queue.top();
		new_queue.pop();
	}

	char issue[300];
	sprintf( issue, "Selected from %u threads; will run for %d time units to completion of burst.", ( unsigned int ) new_queue.size() + 1, CUSTOM_Num );
	string reason = string( issue );

	Choice *choice = new Choice( nextThread, CUSTOM_Num, reason );
	return choice;
}

void CUSTOM::enqueue( const Event* event, Thread* thread )
{
	new_queue.push( thread );
}

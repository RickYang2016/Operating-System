#include "../include/RR.h"

using namespace std;

Choice* RR::get_next_thread( const Event* event )
{
	Thread *nextThread = NULL;

	if( !threads.empty() )
	{
		nextThread = threads.front();
		threads.pop();
	}

	char issue[300];
	sprintf( issue, "Selected from %u threads; will run for %d time units to completion of burst.", ( unsigned int ) threads.size() + 1, RR_Num );
	string reason = string( issue );

	Choice *choice = new Choice( nextThread, RR_Num, reason );
	return choice;
}

void RR::enqueue( const Event *event, Thread *thread )
{
	threads.push( thread );
}
#pragma once

#include "../include/Event.h"
#include "../include/Sys_Parameter.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

class Print_Output
{
public:
	Print_Output( bool vb, bool pt )
	{
		verbose = vb;
		perThread = pt;
	}

	void transitions( const Event *event, Thread::State before_state, Thread::State after_state );
	void dispatcher( const Event *event, Thread *thread, string message );
	void process( Process *process ) const;
	void state( Sys_Parameter state ) const;

private:
	bool verbose;
	bool perThread;
};
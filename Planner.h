#pragma once

#include "../include/Event.h"

class Planner
{
public:
	virtual Choice* get_next_thread( const Event* event ) = 0;
	virtual void enqueue( const Event* event, Thread* thread ) = 0;
};
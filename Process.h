#pragma once

#include <string>
#include "../include/Thread.h"
#include <vector>

using namespace std;

class Thread;

class Process
{
public:
	enum Type{ SYSTEM, INTERACTION, NORMAL, BATCH };
	Process( int pid, Type type );
	int get_pid();
	Type get_type();
	static string get_type_name( Type type );
	void pushThread( Thread* thread );
	const vector<Thread*>& getThreads();

private:
	int pid;
	Type type;
	vector<Thread*> threads;
	static string TypeArray[4];
};
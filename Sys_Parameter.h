#pragma once

struct Sys_Parameter
{
	int count_threads[4];
	double response_time[4];
	double tat_time[4];
	int elapsed_time;
	int service_time;
	int io_time;
	int dispatch_time;
	int idle_time;
	double cpu_utility;
	double cpu_efficiency;
};
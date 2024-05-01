#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
#include <queue>
#include <random>
#include <vector>

#include "Machine.h"
#include "Vehicle.h"
#include "Point.h"
#include "TransportJob.h"
#include "RouteCreator.h"
#include "BasicThread.h"
/*
	작업 관련 데이터들의 관리, Job 수행 관리 등 업무 수행
*/
class TaskManager : public BasicThread{

private:
	RouteCreator* rc;
	unsigned char(*map)[100][70];
	vector<Machine*> *machine_list;
	vector<Vehicle*> *vehicle_list;
	queue<TransportJob*> *job_list;
	HWND hWnd;

protected:
	DWORD Run(void)
	{
		while (true) 
		{
			if (!job_list->empty()) 
			{
				
				Vehicle* vehicle = find_vehicle(vehicle_list);

				if (vehicle == NULL) {
					continue;
				}

				TransportJob *job = job_list->front();
				job_list->pop();

				string* job_str = new string("Start Job, ");
				job_str->append(job->getLog());
				SendMessage(hWnd, WM_COMMAND, 1000, (LPARAM)job_str);

				vehicle->setSourcePoint(job->getSourcePositon());
				vehicle->setSourceMachineName(job->getSourceMachineName());
				vehicle->setDestPoint(job->getDestPositon());
				vehicle->setDestMachineName(job->getDestMachineName());

				
				rc->create(*vehicle->getSourcePoint(), *vehicle->getDestPoint(), vehicle->getRoutes());
				vehicle->setStatus('m');

				string* str = new string("start moving vehicle[");
				str->append(vehicle->getName());
				str->append("], source : ");
				str->append(vehicle->getSourceMachineName());
				str->append(", dest : ");
				str->append(vehicle->getDestMachineName());
				SendMessage(hWnd, WM_COMMAND, 1001, (LPARAM)str);
				cout << "Starting Job, vehicle: "<< vehicle->getName()<< ", " << job->getLog() << "\n";
				vehicle = NULL;

				delete job;
				job = NULL;
			}


		}
		return 0;
	}

public:

	TaskManager(unsigned char(*map)[100][70], vector<Machine*> *machine_list, vector<Vehicle*> *vehicle_list, queue<TransportJob*> *job_list, HWND hWnd)
	{
		this->hWnd = hWnd;
		this->map = map;
		this->machine_list = machine_list;
		this->vehicle_list = vehicle_list;
		this->job_list = job_list;
		this->rc = new RouteCreator(map);
		this->initialize_map(map);
		this->initialize_machine(map, machine_list);
		

	}

	void initialize_map(unsigned char(*map)[100][70]);

	void initialize_machine(unsigned char(*map)[100][70], vector<Machine*>* out);



	//작업을 수행할 vehicle 선택
	Vehicle *find_vehicle(vector<Vehicle*>* out);

};


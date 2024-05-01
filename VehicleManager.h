#pragma once
#include <vector>
#include <iostream>
#include <atlstr.h>
#include "BasicThread.h"
#include "Vehicle.h"

class VehicleManager : public BasicThread {

private:
	vector<Vehicle*>* vehicle_list;
	HWND hWnd;

protected:
	DWORD Run(void)
	{

		while (true)
		{
			//std::cout << "Running Vehicle Manager...\n";


			for (int i = 0; i < vehicle_list->size(); i++) {
				Vehicle* v = vehicle_list->at(i);

				if (v->isMoved()) {
					move(v);
				}
			}
			Sleep(300);
		}
		return 0;
	}
public:
	VehicleManager(vector<Vehicle*>* vehicle_list, HWND hWnd)
	{
		this->hWnd = hWnd;
		this->vehicle_list = vehicle_list;
		this->initialize_vehicle(vehicle_list);
	}

	void initialize_vehicle(vector<Vehicle*>* out);
	
	void move(Vehicle* vehicle);

};
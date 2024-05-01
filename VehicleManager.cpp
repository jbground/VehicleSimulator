#include "VehicleManager.h"


void VehicleManager::move(Vehicle* vehicle) {

	if (!vehicle->isFinished()) {

		Point* dest = vehicle->getDestPoint();
		Point* next = vehicle->getNextPoint();

		if (*dest == *next)
		{
			

			string* str = new string("arrival complete vehicle[");
			str->append(vehicle->getName());
			str->append("], dest : ");
			str->append(vehicle->getDestMachineName());

			SendMessage(hWnd, WM_COMMAND, 1001, (LPARAM)str);
			std::cout << "Completed Job, vehicle : " << vehicle->getName() << ", desc :" << vehicle->getX() << ", " << vehicle->getY() << "\n";
			vehicle->clear();
		}
		else
		{
			vehicle->setPosition(next->getX(), next->getY());
			delete next;
			next = NULL;
			
		}
	}

}

void VehicleManager::initialize_vehicle(vector<Vehicle*>* out) {
	Vehicle* v = NULL;
	v = new Vehicle("V101", 5, 9);
	out->push_back(v);
	v = new Vehicle("V102", 46, 10);
	out->push_back(v);
	v = new Vehicle("V103", 25, 34);
	out->push_back(v);
	v = new Vehicle("V104", 45, 31);
	out->push_back(v);
	v = new Vehicle("V105", 65, 55);
	out->push_back(v);
	v = new Vehicle("V106", 74, 15);
	out->push_back(v);
	v = new Vehicle("V107", 74, 45);
	out->push_back(v);
	v = new Vehicle("V108", 24, 60);
	out->push_back(v);
	v = new Vehicle("V109", 50, 61);
	out->push_back(v);
	v = new Vehicle("V110", 87, 60);
	out->push_back(v);
	v = new Vehicle("V111", 20, 6);
	out->push_back(v);
	v = new Vehicle("V112", 60, 5);
	out->push_back(v);
	v = new Vehicle("V113", 80, 6);
	out->push_back(v);
	v = new Vehicle("V114", 6, 45);
	out->push_back(v);
	v = new Vehicle("V115", 95, 55);
	out->push_back(v);
	v == NULL;

	cout << "Completed Vehicle's Initailization, vehicle_list's size is " << out->size() << "\n";
}
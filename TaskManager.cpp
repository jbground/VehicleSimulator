#include "TaskManager.h"


void TaskManager::initialize_map(unsigned char(*map)[100][70]) {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 70; j++) {

			(*map)[i][j] = '1';

			if (i >= 5 && i <= 95 && (j == 5 || j == 6))
			{
				(*map)[i][j] = '0';
			}

			if ((i == 5 || i == 6) && j >= 5 && j <= 60) {
				(*map)[i][j] = '0';
			}

			if ((i == 25 || i == 26) && j >= 5 && j <= 60) {
				(*map)[i][j] = '0';
			}

			if ((i == 45 || i == 46) && j >= 5 && j <= 60) {
				(*map)[i][j] = '0';
			}

			if ((i == 65 || i == 66) && j >= 5 && j <= 60) {
				(*map)[i][j] = '0';
			}

			if ((i == 94 || i == 95) && j >= 5 && j <= 60) {
				(*map)[i][j] = '0';
			}

			if (i >= 5 && i <= 95 && (j == 60 || j == 61))
			{
				(*map)[i][j] = '0';
			}

			if (i >= 25 && i <= 46 && (j == 30 || j == 31)) {
				(*map)[i][j] = '0';
			}

			if (i >= 65 && i <= 95 && (j == 15 || j == 16 || j == 30 || j == 31 || j == 45 || j == 46)) {
				(*map)[i][j] = '0';
			}

		}
	}

	cout << "Completed Map's Initailization" << "\n";
}

void TaskManager::initialize_machine(unsigned char(*map)[100][70], vector<Machine*> *out) {
	Machine* m = NULL;
	m = new Machine("M01", 15, 4);
	out->push_back(m);
	m = new Machine("M02", 30, 4);
	out->push_back(m);
	m = new Machine("M03", 45, 4);
	out->push_back(m);
	m = new Machine("M04", 60, 4);
	out->push_back(m);
	m = new Machine("M05", 75, 4);
	out->push_back(m);
	m = new Machine("M06", 90, 4);
	out->push_back(m);

	m = new Machine("M07", 7, 15);
	out->push_back(m);
	m = new Machine("M08", 7, 35);
	out->push_back(m);
	m = new Machine("M09", 7, 55);
	out->push_back(m);

	m = new Machine("M10", 24, 15);
	out->push_back(m);
	m = new Machine("M11", 24, 35);
	out->push_back(m);
	m = new Machine("M12", 24, 55);
	out->push_back(m);

	m = new Machine("M13", 27, 13);
	out->push_back(m);
	m = new Machine("M14", 27, 22);
	out->push_back(m);
	m = new Machine("M15", 27, 40);
	out->push_back(m);
	m = new Machine("M16", 27, 48);
	out->push_back(m);

	m = new Machine("M17", 44, 13);
	out->push_back(m);
	m = new Machine("M18", 44, 22);
	out->push_back(m);
	m = new Machine("M19", 44, 40);
	out->push_back(m);
	m = new Machine("M20", 44, 48);
	out->push_back(m);

	m = new Machine("M21", 68, 14);
	out->push_back(m);
	m = new Machine("M22", 80, 14);
	out->push_back(m);
	m = new Machine("M23", 70, 29);
	out->push_back(m);
	m = new Machine("M24", 80, 29);
	out->push_back(m);
	m = new Machine("M25", 80, 47);
	out->push_back(m);
	m = new Machine("M26", 90, 47);
	out->push_back(m);
	m = new Machine("M27", 73, 59);
	out->push_back(m);
	m = new Machine("M28", 85, 59);
	out->push_back(m);





	m = NULL;

	for (int i = 0; i < out->size(); i++) {
		Machine* machine = out->at(i);
		(*map)[machine->getX()][machine->getY()] = 'm';
	}

	cout << "Completed Machine's Initailization, machine_list's size is " << out->size() << "\n";
}


Vehicle* TaskManager::find_vehicle(vector<Vehicle*>* out) {
	for (int i = 0; i < out->size(); i++) {
		Vehicle* v = out->at(i);
		if (!v->isMoved()) {
			return v;
		}
	}
	return NULL;
}

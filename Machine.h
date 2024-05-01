#pragma once
#include <string>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

#include "Point.h"

using namespace std;

class Machine {
private:
	UUID id;
	string name;
	int x;
	int y;

public:

	Machine(string name, int x, int y) {
		UuidCreate(&id);
		this->name = name;
		this->x = x;
		this->y = y;
	}
	char* getId() {
		char* tmp;
		UuidToStringA(&id, (RPC_CSTR*)&tmp);
		return tmp;
	}
	string getName() {
		return name;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}

	Point *getPosition()
	{
		return new Point(x, y);
	}
};

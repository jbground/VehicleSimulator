#pragma once
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

using namespace std;

class Point {
private:
	int x;
	int y;

public:

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	int getX() {
		return x;
	}

	void setX(int x) {
		this->x = x;
	}

	int getY() {
		return y;
	}

	void setY(int y) {
		this->y = y;
	}

	string getCoordinate() {
		return "(" + to_string(x) + ", " + to_string(y) + ")";
	}

	bool operator==(const Point& o) const {
		return x == o.x && y == o.y;
	}
};
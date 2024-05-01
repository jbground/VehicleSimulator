#pragma once

#include <string>

#include "Point.h"
using namespace std;

class Vehicle {
private:
	string name; //���� �̸�
	string sourceMachineName;
	Point* source; //�����
	string destMachineName;
	Point* dest; // ������
	int x, y; //���� ���� ��ġ
	vector<Point*> routes; //���� ���
	char status; //s : stay, standby ����, m : move ����

public:
	Vehicle(string name, int x, int y) {
		this->name = name;
		this->x = x;
		this->y = y;
		this->status = 's';
	}

	string getName() {
		return name;
	}

	void setPosition(int x, int y) {
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

	Point* getSourcePoint() {
		return source;
	}

	void setSourcePoint(int x, int y) {
		delete source;
		source = new Point(x, y);
	}

	void setSourcePoint(Point& p) {
		delete source;
		source = new Point(p.getX(), p.getY());
	}

	void setSourcePoint(Point* p) {
		delete source;
		source = new Point(p->getX(), p->getY());
	}

	void setSourceMachineName(string name) {
		sourceMachineName = name;
	}

	string getSourceMachineName() {
		return sourceMachineName;
	}

	Point* getDestPoint() {
		return dest;
	}

	void setDestPoint(int x, int y) {
		delete dest;
		dest = new Point(x, y);
	}

	void setDestPoint(Point& p) {
		delete dest;
		dest = new Point(p.getX(), p.getY());
	}

	void setDestPoint(Point* p) {
		delete dest;
		dest = new Point(p->getX(), p->getY());
	}

	void setDestMachineName(string name) {
		destMachineName = name;
	}

	string getDestMachineName() {
		return destMachineName;
	}

	vector<Point*>* getRoutes() {
		return &routes;
	}

	Point* getNextPoint() {
		Point* p = routes.front();
		routes.erase(routes.begin());
		return p;
	}

	bool isFinished() {
		return routes.empty();
	}

	char getStatus() {
		return status;
	}

	void setStatus(char status) {
		this->status = status;
	}

	bool isMoved() {
		return this->status == 'm';
	}

	void clear() {
		//cout << "Completed Transport, vehicle name : " << getName() << ", source : " << source->print()<< ", dest : " << "\n";
		delete source;
		source = NULL;
		delete dest;
		dest = NULL;
		status = 's';
		routes.clear();
		routes = vector<Point*>();
	}


};

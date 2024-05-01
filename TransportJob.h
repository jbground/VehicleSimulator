#pragma once
#include <string>
#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

#include "Machine.h"

using namespace std;

class TransportJob {
private:
	UUID id;
	Machine* source;
	Machine* dest;

public:


	TransportJob(Machine* source, Machine* dest) {
		UuidCreate(&id);
		this->source = source;
		this->dest = dest;
	}

	Point* getSourcePositon() 
	{
		return source->getPosition();
	}

	string getSourceMachineName() {
		return source->getName();
	}

	Point *getDestPositon()
	{
		return dest->getPosition();
	}

	string getDestMachineName() {
		return dest->getName();
	}

	string getId() {
		char* tmp;
		UuidToStringA(&id, (RPC_CSTR*)&tmp);
		return string(tmp);
	}

	string getLog() {
		return "jobId : " + getId() + ", source : " + source->getName() + ", dest : " + dest->getName();
	}
};
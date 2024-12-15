/**
	@file
	This head files provides SWIGed access to the importhandler 'shared pointer'
	mechanism used to pass data from 'extpy' to 'ascpy' in particular (and
	hopefully between other scripting languages/GUI combinations in future)
*/

#ifndef WEBCXX_REGISTRY_H
#define WEBCXX_REGISTRY_H

#include "config.h"


/*
extern "C"{
#ifdef WEBCXX_USE_PYTHON
ASC_IMPORT void * importhandler_getsharedpointer(const char *);
#endif
}
*/

#include "instance.h"

class Registry{
public:
//	void setInteger(const char *key, int value);
	void setPointer(const char *key, void *value);
	aw_Instance *getInstance(const char *key);
	Simulation &getSimulation(const char *key);
};

#endif /* WEBCXX_REGISTRY_H */

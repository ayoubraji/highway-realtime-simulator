/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#ifndef THREADS_CONTROLLER_H
#define THREADS_CONTROLLER_H

#include "Graphics/highway_gui.h"

void initStandardThreads();
void initCustomThreads(highway_parameters_t parameters);
void initGuiThread();
void initVehiclesThreads();
void findNextVehicle(int vehicle_id, struct highway_t *h);

#endif // THREADS_CONTROLLER_H

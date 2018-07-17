#ifndef THREADS_CONTROLLER_H
#define THREADS_CONTROLLER_H

#include "Graphics/highway_gui.h"

void initStandardThreads();
void initCustomThreads(highway_parameters_t parameters);
void initGuiThread();
void initVehiclesThreads();

#endif // THREADS_CONTROLLER_H

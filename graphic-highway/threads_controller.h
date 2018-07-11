#ifndef THREADS_CONTROLLER_H
#define THREADS_CONTROLLER_H

//#include "highway.h"
#include "Graphics/highway_gui.h"
//struct highway_t highway;

//methods
//HighwayGui highway_gui = HighwayGui::getInstance();
//HighwayGui& gui = HighwayGui::getInstance();

void initThreads();
void initGuiThread();
void initVehiclesThreads();

#endif // THREADS_CONTROLLER_H

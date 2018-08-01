/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#ifndef HIGHWAY_H
#define HIGHWAY_H

#include "pthread.h"
#include "semaphore.h"
#include "vehicle.h"
#include "Graphics/highway_gui.h"
#include "highway_parameters.h"

//Highway lanes
#define LANES 4

//Road lenght
//#define ROAD_LENGHT 5000
#define ROAD_LENGHT 2500

//Movements
enum MOVEMENTS{GO_AHEAD, OVERTAKE, TURN_RIGHT};
#define GO_AHEAD 10
#define OVERTAKE 11
#define TURN_RIGHT 12

/* Shared structure */
struct highway_t
{
    //array containing all the vehicles
    struct vehicle_t *vehicles;

    pthread_mutex_t mutex;

	//private conditional variables, one for each vehicle
	pthread_cond_t *priv_Vehicle;

	//vehicles quantity
	int n_vehicles;

    //Array that represents each portion of the highway
	//-1 if there is no vehicle in the [lane][x][y] position
    //the vehicle id otherwise
	//the second dimension rappresents the range of each lane
    int road[LANES][LANES][ROAD_LENGHT];

	//number of waiting vehicles
	int waiting_to_start;
	int next_vehicle = 0;

    //queue iterator
    int *next;

	bool highway_start;

	//vehicle that will be tracked on the gui
	int vehicle_to_track;

	//frequency of lateral movement inside the lane
	bool rare_frequency;

	bool only_one_left;

};

void initStandardHighway(struct highway_t *h);
void initCustomHighway(struct highway_t *h, struct highway_parameters_t parameters);

void graphic_update(int vehicle_id, struct highway_t *h, int movement);

#endif // HIGHWAY_H

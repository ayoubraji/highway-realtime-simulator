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
//Lo si può vedere anche come numero di caselle,
//dove ogni veicolo occupa n caselle
#define ROAD_LENGHT 5000

//Movements
enum MOVEMENTS{GO_AHEAD, OVERTAKE, TURN_RIGHT};
#define GO_AHEAD 10
#define OVERTAKE 11
#define TURN_RIGHT 12

/* struttura condivisa */
struct highway_t
{
    //array containing all the vehicles
    struct vehicle_t *vehicles;

    pthread_mutex_t mutex;
	pthread_cond_t *priv_Vehicle;

	//vehicles quantity... da cambiare e rendere dinamico
	int n_vehicles;

    //Array that represents each portion of the highway
    //NULL (or -1) if there is no vehicle in the [L][L][ROAD_LENGTH] position
    //the vehicle id otherwise
    int road[LANES][LANES][ROAD_LENGHT];

	//number of waiting vehicles
	int waiting_to_start;
	int next_vehicle = 0;

    //queue iterator
    int *next;

	//da spostare l'inizializzazione in initHi...
	bool highway_start;

	int vehicle_to_track;
	bool rare_frequency;

};

void initStandardHighway(struct highway_t *h);
void initCustomHighway(struct highway_t *h, struct highway_parameters_t parameters);

void graphic_update(int vehicle_id, struct highway_t *h, int movement);

#endif // HIGHWAY_H
